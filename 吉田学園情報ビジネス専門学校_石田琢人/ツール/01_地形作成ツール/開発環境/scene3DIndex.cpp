//=============================================================================
//
// インデックス3Dオブジェクト処理 [scene3DIndex.h]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "scene3DIndex.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "input.h"

#include <stdio.h>

//==================================
// マクロ定義
//==================================
#define TEXT_FILE "data/TEXT/field.ini"
#define BIN_SAVE_FILENAME	"data/TEXT/field_save.bin"
#define TEXT_SAVE_FILENAME	"data/TEXT/field_save.txt"

#define SIZE_X	(50.0f)	// ポリゴンの横の長さ
#define SIZE_Y	(50.0f)	// ポリゴンの縦の長さ

//==================================
// 静的メンバ変数宣言
//==================================
char CScene3DIndex::m_aTxtFileName[64] = {};		// 読み込むテキストファイル名

//=========================================
// システムファイルを読み込む
//=========================================
HRESULT CScene3DIndex::Load(void)
{
	FILE *pFile;	// ファイルのポインタ変数
	char aStr[128];	// 取得する文字列

	// ファイルを開く
	pFile = fopen(TEXT_FILE, "r");

	if (NULL != pFile)
	{// ファイルがあった
		while (strcmp(aStr, "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", aStr);
			if (strcmp(aStr, "SCRIPT_FILENAME") == 0)
			{// 読み込むバイナリファイル
				fscanf(pFile, " = %s", &m_aTxtFileName[0]);
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開けない
		MessageBox(0, "ファイルがありません！", "field.ini", MB_OK);
	}

	return S_OK;
}

//=========================================
// テクスチャを割り当てる
//=========================================
void CScene3DIndex::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// コンストラクタ
//=========================================
CScene3DIndex::CScene3DIndex(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ポリゴンの位置
	m_fLengthX = 0;								// 一つのブロックの幅
	m_fLengthZ = 0;								// 一つのブロックの奥行
	m_nMeshX = 0;								// X軸の分割数
	m_nMeshZ = 0;								// Z軸の分割数
}

//=========================================
// デストラクタ
//=========================================
CScene3DIndex::~CScene3DIndex()
{
}

//=========================================
// ポリゴンの初期化処理
//=========================================
HRESULT CScene3DIndex::Init(void)
{
	// ファイル読み込み
	Input();

	return S_OK;
}

//=========================================
// ポリゴンの終了処理
//=========================================
void CScene3DIndex::Uninit(void)
{
	if (NULL != m_pVtxBuff)
	{// 頂点バッファの破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	if (NULL != m_pIdxBuff)
	{// インデックスバッファの破棄
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// 死亡フラグ
	Release();
}

//=========================================
// ポリゴンの更新処理
//=========================================
void CScene3DIndex::Update(void)
{
	// キーボードの入力を取得
	CInputKeyboard *pKeyboard = CManager::GetInput();

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pKeyboard->GetTrigger(DIK_F5))
	{// 法線の再計算
		SetNor(pVtx);
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	if (pKeyboard->GetTrigger(DIK_F9))
	{// ファイルに出力
		Output();
		OutputVtx();
	}

	if (pKeyboard->GetTrigger(DIK_F3))
	{// ファイルから読み込む
		if (NULL != m_pVtxBuff)
		{// 頂点バッファの破棄
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}

		if (NULL != m_pIdxBuff)
		{// インデックスバッファの破棄
			m_pIdxBuff->Release();
			m_pIdxBuff = NULL;
		}

		// 情報をファイルから読み込む
		Input();

		// 頂点情報の作成
		MakeVertex();

		// 頂点情報をファイルから読み込む
		InputVtx();
	}

	// デバッグ表示
	CDebugProc::Print("分割数 　：%d %d", m_nMeshX, m_nMeshZ);
	CDebugProc::Print("長さ　 　：%.1f %.1f", m_fLengthX, m_fLengthZ);

}

//=========================================
// ポリゴンの描画処理
//=========================================
void CScene3DIndex::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// ワールドマトリックスの保存用

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVtx, 0, m_nNumPolygon);
}

//=========================================
// Xの分割数の変更
//=========================================
void CScene3DIndex::SetNumMeshX(int nValue)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	if ((m_nMeshX > 1 || nValue > 0) && m_nMeshX + nValue > 0)
	{// １より大きい時のみ
		VERTEX_3D *pVtx;					// 頂点情報のポインタ
		VERTEX_3D *pVtxInfo;				// 頂点情報の保存
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// 保存用頂点バッファ
		int nCntAdd = 0;					// 頂点カウント用カウンタ
		int nCntMeshX, nCntMeshZ, nCntVtx;	// for文用カウンタ

		// 頂点バッファを生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (((m_nMeshX + 1) + nValue) * (m_nMeshZ + 1)),
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pVtxBuff,
			NULL);

		// 頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 保存用頂点バッファをロックし、頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtxInfo, 0);

		for (nCntMeshZ = 0, nCntVtx = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
		{// Zの分割数分ループ
			for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++, nCntVtx++)
			{// Xの分割数分ループ
				if (nValue < 0)
				{// 分割数を減らすとき
					if (nCntMeshX != m_nMeshX)
					{// Xが分割数のとき以外を保存
						pVtxInfo[nCntAdd] = pVtx[nCntVtx];
						nCntAdd++;
					}
				}
				else
				{// 分割数を増やすとき
					pVtxInfo[nCntVtx + nCntAdd] = pVtx[nCntVtx];

					if (nCntMeshX == m_nMeshX)
					{// Xが分割数のとき以外を保存
						for (int nCount = 0; nCount < nValue; nCount++)
						{// Xが分割数以上も保存
							pVtxInfo[nCntVtx + nCntAdd].pos = pVtx[nCntVtx].pos;
							pVtxInfo[nCntVtx + nCntAdd].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
							pVtxInfo[nCntVtx + nCntAdd].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
							pVtxInfo[nCntVtx + nCntAdd].tex = D3DXVECTOR2(nCntMeshX * 1.0f, nCntMeshZ * 1.0f);

							nCntAdd++;
						}
					}
				}
			}
		}

		// 保存用頂点バッファをアンロック
		pVtxBuff->Unlock();

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		if (m_pVtxBuff != NULL)
		{// 頂点バッファを開放
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}

		if (m_pIdxBuff != NULL)
		{// インデックスバッファを開放
			m_pIdxBuff->Release();
			m_pIdxBuff = NULL;
		}

		// 分割数
		m_nMeshX += nValue;

		if (m_nMeshX < 1)
		{// Xの分割数が１より小さくなった
			m_nMeshX = 1;
		}

		// 頂点情報の作成
		MakeVertex();

		// 頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 保存用頂点バッファをロックし、頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtxInfo, 0);

		for (nCntMeshZ = 0, nCntVtx = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
		{// Zの分割数分ループ
			for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++, nCntVtx++)
			{// Xの分割数分ループ
				pVtx[nCntVtx].pos = pVtxInfo[nCntVtx].pos;

				pVtx[nCntVtx].pos.x = -(m_fLengthX * m_nMeshX * 0.5f) + (m_fLengthX * nCntMeshX) * 1.0f;
				pVtx[nCntVtx].pos.z = (m_fLengthZ * m_nMeshZ * 0.5f) - (m_fLengthZ * nCntMeshZ) * 1.0f;
			}
		}

		// 法線の再計算
		SetNor(pVtx);

		// 保存用頂点バッファをアンロック
		pVtxBuff->Unlock();

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		if (pVtxBuff != NULL)
		{// 保存用頂点バッファを開放
			pVtxBuff->Release();
			pVtxBuff = NULL;
		}
	}
}

//=========================================
// Zの分割数の変更
//=========================================
void CScene3DIndex::SetNumMeshZ(int nValue)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	if ((m_nMeshZ > 1 || nValue > 0) && m_nMeshZ + nValue > 0)
	{// １より大きい時のみ
		VERTEX_3D *pVtx;					// 頂点情報のポインタ
		VERTEX_3D *pVtxInfo;				// 頂点情報の保存
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// 保存用頂点バッファ
		int nCntAdd = 0;					// 頂点カウント用カウンタ
		int nCntMeshX, nCntMeshZ, nCntVtx;	// for文用カウンタ

		// 頂点バッファを生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((m_nMeshX + 1) * (m_nMeshZ + 1 + nValue)),
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pVtxBuff,
			NULL);

		// 頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 保存用頂点バッファをロックし、頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtxInfo, 0);

		for (nCntMeshZ = 0, nCntVtx = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
		{// Zの分割数分ループ
			for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++, nCntVtx++)
			{// Xの分割数分ループ
				if (nValue < 0)
				{// 分割数を減らすとき
					if (nCntMeshZ != m_nMeshZ)
					{// Zが分割数のとき以外を保存
						pVtxInfo[nCntAdd] = pVtx[nCntVtx];
						nCntAdd++;
					}
				}
				else
				{// 分割数を増やすとき
					pVtxInfo[nCntVtx + nCntAdd] = pVtx[nCntVtx];

					if (nCntMeshZ == m_nMeshZ)
					{// Zが分割数のときを保存
						for (int nCount = 0; nCount < nValue; nCount++)
						{// Xが分割数以上も保存
							pVtxInfo[nCntVtx + nCntAdd].pos = pVtx[nCntVtx].pos;
							pVtxInfo[nCntVtx + nCntAdd].pos.y = 0.0;
							pVtxInfo[nCntVtx + nCntAdd].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
							pVtxInfo[nCntVtx + nCntAdd].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
							pVtxInfo[nCntVtx + nCntAdd].tex = D3DXVECTOR2(nCntMeshX * 1.0f, nCntMeshZ * 1.0f);

							nCntAdd++;
						}
					}
				}
			}
		}

		// 保存用頂点バッファをアンロック
		pVtxBuff->Unlock();

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		if (m_pVtxBuff != NULL)
		{// 頂点バッファを開放
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}

		if (m_pIdxBuff != NULL)
		{// インデックスバッファを開放
			m_pIdxBuff->Release();
			m_pIdxBuff = NULL;
		}

		// 分割数
		m_nMeshZ += nValue;

		if (m_nMeshZ < 1)
		{// Zの分割数が１より小さくなった
			m_nMeshZ = 1;
		}

		// 頂点情報の作成
		MakeVertex();

		// 頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 保存用頂点バッファをロックし、頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtxInfo, 0);

		for (nCntMeshZ = 0, nCntVtx = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
		{// Zの分割数分ループ
			for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++, nCntVtx++)
			{// Xの分割数分ループ
				pVtx[nCntVtx].pos = pVtxInfo[nCntVtx].pos;

				pVtx[nCntVtx].pos.x = -(m_fLengthX * m_nMeshX * 0.5f) + (m_fLengthX * nCntMeshX) * 1.0f;
				pVtx[nCntVtx].pos.z = (m_fLengthZ * m_nMeshZ * 0.5f) - (m_fLengthZ * nCntMeshZ) * 1.0f;
			}
		}

		// 法線の再計算
		SetNor(pVtx);

		// 保存用頂点バッファをアンロック
		pVtxBuff->Unlock();

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		if (pVtxBuff != NULL)
		{// 保存用頂点バッファを開放
			pVtxBuff->Release();
			pVtxBuff = NULL;
		}
	}
}

//=========================================
// X軸長さの変更
//=========================================
void CScene3DIndex::SetLengthX(float fValue)
{
	if (m_fLengthX > 1 || fValue > 0)
	{// １より大きい時のみ
		// サイズ
		m_fLengthX += fValue;

		if (m_fLengthX < 5.0f)
		{// 長さが1より小さくなったとき
			m_fLengthX = 5.0f;
		}

		int nCntMeshZ, nCntMeshX, nCntVtx;	// for文用カウンタ
		VERTEX_3D *pVtx;	// 頂点情報のポインタ

		// 頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntMeshZ = 0, nCntVtx = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
		{// Zの分割数分ループ
			for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++, nCntVtx++)
			{// Xの分割数分ループ
				pVtx[nCntVtx].pos.x = -(m_fLengthX * m_nMeshX * 0.5f) + (m_fLengthX * nCntMeshX) * 1.0f;
			}
		}

		// 法線の再計算
		SetNor(pVtx);

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}
}

//=========================================
// Z軸長さの変更
//=========================================
void CScene3DIndex::SetLengthZ(float fValue)
{
	if (m_fLengthZ > 1 || fValue > 0)
	{// １より大きい時のみ
		// サイズ
		m_fLengthZ += fValue;

		if (m_fLengthZ < 5.0f)
		{// 長さが1より小さくなったとき
			m_fLengthZ = 5.0f;
		}

		int nCntMeshZ, nCntMeshX, nCntVtx;	// for文用カウンタ
		VERTEX_3D *pVtx;	// 頂点情報のポインタ

		// 頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntMeshZ = 0, nCntVtx = 0; nCntMeshZ <= m_nMeshZ; nCntMeshZ++)
		{// Zの分割数分ループ
			for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++, nCntVtx++)
			{// Xの分割数分ループ
				pVtx[nCntVtx].pos.z = (m_fLengthZ * m_nMeshZ * 0.5f) - (m_fLengthZ * nCntMeshZ) * 1.0f;
			}
		}

		// 法線の再計算
		SetNor(pVtx);

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}
}

//=========================================
// 地面の高さの取得
//=========================================
float CScene3DIndex::GetHeight(D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;	// 返す高さ
	float fDiff = 0.0f;		// 判定用にY軸下げた分を保存
	int nPosBrock[2];		// プレイヤーのいるポリゴンのブロック
	int nVtxNumber[4];		// ブロックの頂点番号

	// プレイヤーのベクトルを求める
	D3DXVECTOR3 vecPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 vecA, vecB, collisionNor, collisionPos[3];

	// プレイヤーのいるポリゴン判定
	nPosBrock[0] = (int)(pos.x + (m_fLengthX * m_nMeshX * 0.5f)) / (int)m_fLengthX;		// X軸のブロック割り出し
	nPosBrock[1] = (int)(pos.z - (m_fLengthZ * m_nMeshZ * 0.5f)) / (int)m_fLengthZ * -1;	// Z軸のブロック割り出し

	if (nPosBrock[0] >= m_nMeshX || (int)(pos.x + (m_fLengthX * m_nMeshX * 0.5f)) < 0 ||
		nPosBrock[1] >= m_nMeshZ || (int)(pos.z - (m_fLengthZ * m_nMeshZ * 0.5f)) > 0)
	{// ポリゴンに乗っていないとき
		CDebugProc::Print("乗ってない");
		return 0;
	}

	// 頂点番号を計算
	nVtxNumber[0] = nPosBrock[0] + ((nPosBrock[1] + 1) * (m_nMeshX + 1));			// 自分のブロックの左下番号
	nVtxNumber[1] = nPosBrock[0] + (nPosBrock[1] * (m_nMeshX + 1));					// 自分のブロックの左上番号
	nVtxNumber[2] = (nPosBrock[0] + 1) + ((nPosBrock[1] + 1) * (m_nMeshX + 1));		// 自分のブロックの右下番号
	nVtxNumber[3] = (nPosBrock[0] + 1) + (nPosBrock[1] * (m_nMeshX + 1));			// 自分のブロックの右上番号

	// 外積で左右どちらのポリゴンか判別
	vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[1]].pos;	// 頂点から目的の頂点への方向
	vecB = pos - pVtx[nVtxNumber[1]].pos;						// プレイヤー　－　vecAの始まりの頂点

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) >= 0)
	{// 左側
		// 判定用頂点座標
		collisionPos[0] = pVtx[nVtxNumber[0]].pos;
		collisionPos[1] = pVtx[nVtxNumber[1]].pos;
		collisionPos[2] = pVtx[nVtxNumber[2]].pos;
	}
	else
	{// 右側
		// 判定用頂点座標
		collisionPos[0] = pVtx[nVtxNumber[3]].pos;
		collisionPos[1] = pVtx[nVtxNumber[2]].pos;
		collisionPos[2] = pVtx[nVtxNumber[1]].pos;
	}

	// 判定用に下げる値を保存
	fDiff = collisionPos[0].y;

	// 中心の頂点の高さが0になるように引く
	collisionPos[0].y -= fDiff;
	collisionPos[1].y -= fDiff;
	collisionPos[2].y -= fDiff;

	// 判定用法線
	vecA = collisionPos[1] - collisionPos[0];	// 左側
	vecB = collisionPos[2] - collisionPos[0];	// 右側
	D3DXVec3Cross(&collisionNor, &vecA, &vecB);			// 外積を求める
	D3DXVec3Normalize(&collisionNor, &collisionNor);	// 正規化

	// プレイヤーのベクトル
	vecPlayer = pos - collisionPos[0];

	// プレイヤーのY軸を求める
	fHeight = (-(collisionNor.x * vecPlayer.x) - (collisionNor.z * vecPlayer.z)) / collisionNor.y;

	// 判定用に下げた分を補正
	fHeight += fDiff;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	// デバッグ表示
	CDebugProc::Print("４頂点　　：%d %d %d %d", nVtxNumber[0], nVtxNumber[1], nVtxNumber[2], nVtxNumber[3]);
	CDebugProc::Print("高さ　　　：%.1f", fHeight);
	CDebugProc::Print("ブロック　：%d %d", nPosBrock[0], nPosBrock[1]);

	return fHeight;
}

//=========================================================================================================================
// 頂点を上げる
//=========================================================================================================================
void CScene3DIndex::SetHeight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{// 全頂点の距離を計算
		float fLength = powf(pVtx[nCntVtx].pos.x - pos.x, 2.0f) + powf(pVtx[nCntVtx].pos.z - pos.z, 2.0f);

		if (fLength <= powf(fRange, 2.0f))
		{// 範囲内にある頂点
			float fHeight = cosf((D3DX_PI * 0.5f) / (fRange / sqrtf(fLength))) * fValue;

			pVtx[nCntVtx].pos.y += fHeight;

#if 0	// sin,cosを使わない
			float fDiff = powf(fRange, 2) - (powf(pVtx[nCntVtx].pos.x - pos.x, 2) + powf(pVtx[nCntVtx].pos.z - pos.z, 2));
			pVtx[nCntVtx].pos.y += fDiff / powf(fRange, 2);
#endif
		}
	}

	// 法線の再計算
	SetNor(pVtx);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================================================================================================
// 頂点情報の設定
//=========================================================================================================================
void CScene3DIndex::SetVtx(void)
{
	// ポリゴン数
	m_nNumPolygon = m_nMeshX * m_nMeshZ * 2 + (4 * (m_nMeshZ - 1));

	// カウント用
	int nCntMeshYZ, nCntMeshX, nCntIdx;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshYZ = 0; nCntMeshYZ <= m_nMeshZ; nCntMeshYZ++)
	{
		for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++)
		{
			// 頂点座標
			switch (CScene::GetObjType())
			{
			case CScene::OBJTYPE_FIELD:		// フィールド
				pVtx[0 + (nCntMeshX + (nCntMeshYZ * (m_nMeshX + 1)))].pos = D3DXVECTOR3(-(m_fLengthX * m_nMeshX * 0.5f) + (m_fLengthX * nCntMeshX) * 1.0f,
					0.0f,
					(m_fLengthZ * m_nMeshZ * 0.5f) - (m_fLengthZ * nCntMeshYZ) * 1.0f);
				break;

			case CScene::OBJTYPE_CYLINDER:		// シリンダー

				float fAngle = 1.0f / (m_nMeshX / 2);	// 角度

				pVtx[0].pos.x = sinf(D3DX_PI * (0.0f + (fAngle * nCntMeshX))) * m_fLengthX;		// ずれる
				pVtx[0].pos.y = 200.0f - (nCntMeshYZ * 200.0f) * 1.0f;		// Y軸を下へずらす
				pVtx[0].pos.z = cosf(D3DX_PI * (0.0f + (fAngle * nCntMeshX))) * m_fLengthZ;		// ずれる
				break;
			}

			// 頂点カラー
			pVtx[0 + (nCntMeshX + (nCntMeshYZ * (m_nMeshX + 1)))].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標
			pVtx[0 + (nCntMeshX + (nCntMeshYZ * (m_nMeshX + 1)))].tex = D3DXVECTOR2(nCntMeshX * 1.0f, nCntMeshYZ * 1.0f);
		}
	}

	// 法線の計算
	SetNor(pVtx);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	WORD *pIdx;		// インデックスデータへのポインタ

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// 地面
	switch (CScene::GetObjType())
	{
	case CScene::OBJTYPE_FIELD:		// フィールド
		for (nCntMeshYZ = 0, nCntIdx = 0; nCntMeshYZ < m_nMeshZ; nCntMeshYZ++)
		{
			for (nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++, nCntIdx++)
			{

				pIdx[0] = (m_nMeshX + 1) + nCntIdx;
				pIdx[1] = 0 + nCntIdx;

				pIdx += 2;

				if (nCntMeshYZ < m_nMeshZ - 1 && nCntMeshX == m_nMeshX)
				{// 折り返し時
					pIdx[0] = 0 + nCntIdx;
					pIdx[1] = (m_nMeshX + 1) + nCntIdx + 1;

					pIdx += 2;
				}
			}
		}
		break;

	case CScene::OBJTYPE_CYLINDER:		// シリンダー
		for (int nCntMeshH = 0; nCntMeshH <= m_nMeshX; nCntMeshH++)
		{
			pIdx[0] = (m_nMeshX + 1) + nCntMeshH;
			pIdx[1] = 0 + nCntMeshH;

			pIdx += 2;
		}
		break;
	}

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();
}

//=========================================================================================================================
// 法線の計算
//=========================================================================================================================
void CScene3DIndex::SetNor(VERTEX_3D *pVtx)
{
	// カウント用
	int nCntMeshYZ, nCntMeshX, nCntVtx;

	for (nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{// 全頂点の法線を初期化
		pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (nCntMeshYZ = 0; nCntMeshYZ < m_nMeshZ; nCntMeshYZ++)
	{
		for (nCntMeshX = 0; nCntMeshX < m_nMeshX; nCntMeshX++)
		{// 法線
			D3DXVECTOR3 vecA;	// 頂点０から頂点１への方向
			D3DXVECTOR3 vecB;	// 頂点０から頂点２への方向
			D3DXVECTOR3 nor[2];	// 法線の計算結果
			int nVtxNumber[4];	// ４頂点の番号

			nVtxNumber[0] = nCntMeshX + ((nCntMeshYZ + 1) * (m_nMeshX + 1));			// 自分のブロックの左下番号
			nVtxNumber[1] = nCntMeshX + (nCntMeshYZ * (m_nMeshX + 1));					// 自分のブロックの左上番号
			nVtxNumber[2] = (nCntMeshX + 1) + ((nCntMeshYZ + 1) * (m_nMeshX + 1));		// 自分のブロックの右下番号
			nVtxNumber[3] = (nCntMeshX + 1) + (nCntMeshYZ * (m_nMeshX + 1));			// 自分のブロックの右上番号

			// 各ポリゴンの頂点を求める
			// 左側-------------------------------------------------------------------------------------------
			// 0
			vecA = pVtx[nVtxNumber[1]].pos - pVtx[nVtxNumber[0]].pos;	// 左側
			vecB = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[0]].pos;	// 右側

			D3DXVec3Cross(&nor[0], &vecA, &vecB);	// 外積を求める
			D3DXVec3Normalize(&nor[0], &nor[0]);	// 正規化

			// 右側-------------------------------------------------------------------------------------------
			// 3
			vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[3]].pos;	// 左側
			vecB = pVtx[nVtxNumber[1]].pos - pVtx[nVtxNumber[3]].pos;	// 右側

			D3DXVec3Cross(&nor[1], &vecA, &vecB);	// 外積を求める
			D3DXVec3Normalize(&nor[1], &nor[1]);	// 正規化

													// 各頂点の法線を算出
			pVtx[nVtxNumber[0]].nor += nor[0];
			pVtx[nVtxNumber[1]].nor += nor[0] + nor[1];
			pVtx[nVtxNumber[2]].nor += nor[0] + nor[1];
			pVtx[nVtxNumber[3]].nor += nor[1];
		}
	}

	for (nCntMeshYZ = 0; nCntMeshYZ < m_nMeshZ; nCntMeshYZ++)
	{
		for (nCntMeshX = 0; nCntMeshX < m_nMeshX; nCntMeshX++)
		{// 法線の平均を出す
			D3DXVECTOR3 vecA;	// 頂点０から頂点１への方向
			D3DXVECTOR3 vecB;	// 頂点０から頂点２への方向
			int nVtxNumber[4];	// ４頂点の番号

			nVtxNumber[0] = nCntMeshX + ((nCntMeshYZ + 1) * (m_nMeshX + 1));			// ブロックの左下番号
			nVtxNumber[1] = nCntMeshX + (nCntMeshYZ * (m_nMeshX + 1));					// ブロックの左上番号
			nVtxNumber[2] = (nCntMeshX + 1) + ((nCntMeshYZ + 1) * (m_nMeshX + 1));		// ブロックの右下番号
			nVtxNumber[3] = (nCntMeshX + 1) + (nCntMeshYZ * (m_nMeshX + 1));			// ブロックの右上番号

			if (nCntMeshX == 0 && nCntMeshYZ == 0)
			{// 左上端
				pVtx[nVtxNumber[0]].nor /= 3;
				pVtx[nVtxNumber[1]].nor /= 2;
				pVtx[nVtxNumber[2]].nor /= 6;
				pVtx[nVtxNumber[3]].nor /= 3;
			}
			else if (nCntMeshX == 0 && nCntMeshYZ > 0 && nCntMeshYZ < m_nMeshZ - 1)
			{// 左端
				pVtx[nVtxNumber[0]].nor /= 3;
				pVtx[nVtxNumber[2]].nor /= 6;
			}
			else if (nCntMeshX == m_nMeshX - 1 && nCntMeshYZ > 0 && nCntMeshYZ < m_nMeshZ - 1)
			{
				pVtx[nVtxNumber[2]].nor /= 3;
			}
			else if (nCntMeshYZ == 0)
			{// 上端
				if (nCntMeshX == m_nMeshX - 1)
				{// 右上端
					pVtx[nVtxNumber[2]].nor /= 3;
				}
				else
				{// 上中
					pVtx[nVtxNumber[2]].nor /= 6;
					pVtx[nVtxNumber[3]].nor /= 3;
				}
			}
			else if (nCntMeshYZ == m_nMeshZ - 1)
			{// 下端
				if (nCntMeshX == m_nMeshX - 1)
				{// 右下端
					pVtx[nVtxNumber[2]].nor /= 2;
				}
				else
				{// 下中
					pVtx[nVtxNumber[2]].nor /= 3;
				}
			}
			else if (nCntMeshX > 0 && nCntMeshYZ > 0 && nCntMeshX < m_nMeshX - 1 && nCntMeshYZ < m_nMeshZ - 1)
			{// 中
				pVtx[nVtxNumber[2]].nor /= 6;
			}
		}
	}
}

//=========================================================================================================================
// 頂点情報の作成
//=========================================================================================================================
void CScene3DIndex::MakeVertex(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 頂点数の計算
	m_nNumVtx = (m_nMeshX + 1) * (m_nMeshZ + 1);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (m_nMeshX * m_nMeshZ * 2 + (4 * (m_nMeshZ - 1)) + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16ビットのデータを確保
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// 頂点情報の設定
	SetVtx();
}

//=========================================================================================================================
// 位置の設定
//=========================================================================================================================
const void CScene3DIndex::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=========================================================================================================================
// ファイルに出力
//=========================================================================================================================
void CScene3DIndex::Output(void)
{
	FILE *pFile;

	// ファイルを開く
	pFile = fopen(TEXT_SAVE_FILENAME, "w");

	if (NULL != pFile)
	{// ファイルがあった
		fprintf(pFile, "#********************************************************************\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#    地面スクリプトファイル[field.txt]\n");
		fprintf(pFile, "#    Auther : Ishida Takuto\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#********************************************************************\n");

		fprintf(pFile, "FIELDSET\n");

		fprintf(pFile, "POS = %.1f %.1f %.1f	# 位置\n", m_pos.x, m_pos.y, m_pos.z);
		fprintf(pFile, "ROT = %.1f %.1f %.1f	# 向き\n", m_rot.x, m_rot.y, m_rot.z);
		fprintf(pFile, "WIDTH = %.1f	# 1ブロック分の幅の大きさ\n", m_fLengthX);
		fprintf(pFile, "DEPTH = %.1f	# 1ブロック分の奥行の大きさ\n", m_fLengthZ);
		fprintf(pFile, "XBLOCK = %d	# 横の分割数\n", m_nMeshX);
		fprintf(pFile, "ZBLOCK = %d	# 奥行の分割数\n", m_nMeshZ);
		fprintf(pFile, "VERTEX_FILENAME = %s	# 読み込む頂点情報のバイナリファイル名\n", &m_aFileName[0]);

		fprintf(pFile, "END_FIELDSET\n");

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開けない
		MessageBox(0, "ファイルがありません！\nファイルを作成します。", "field.txt", MB_OK);
	}
}

//=========================================================================================================================
// ファイルから読み込む
//=========================================================================================================================
void CScene3DIndex::Input(void)
{
	FILE *pFile;	// ファイルのポインタ変数
	char aStr[128];	// 取得する文字列

	// ファイルを開く
	pFile = fopen(&m_aTxtFileName[0], "r");

	if (NULL != pFile)
	{// ファイルがあった
		while (strcmp(aStr, "END_FIELDSET") != 0)
		{
			fscanf(pFile, "%s", aStr);
			if (strcmp(aStr, "POS") == 0)
			{// 位置
				fscanf(pFile, " = %f %f %f", &m_pos.x, &m_pos.y, &m_pos.z);
			}
			if (strcmp(aStr, "ROT") == 0)
			{// 向き
				fscanf(pFile, " = %f %f %f", &m_rot.x, &m_rot.y, &m_rot.z);
			}
			if (strcmp(aStr, "WIDTH") == 0)
			{// 幅
				fscanf(pFile, " = %f", &m_fLengthX);
			}
			if (strcmp(aStr, "DEPTH") == 0)
			{// 奥行
				fscanf(pFile, " = %f", &m_fLengthZ);
			}
			if (strcmp(aStr, "XBLOCK") == 0)
			{// 横の分割数
				fscanf(pFile, " = %d", &m_nMeshX);
			}
			if (strcmp(aStr, "ZBLOCK") == 0)
			{// 奥行の分割数
				fscanf(pFile, " = %d", &m_nMeshZ);
			}
			if (strcmp(aStr, "VERTEX_FILENAME") == 0)
			{// 読み込むバイナリファイル
				fscanf(pFile, " = %s", &m_aFileName[0]);
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開けない
		MessageBox(0, "ファイルがありません！", "field.txt", MB_OK);
	}
}

//=========================================================================================================================
// 頂点情報をファイルに出力
//=========================================================================================================================
void CScene3DIndex::OutputVtx(void)
{
	FILE *pFile;

	// ファイルを開く
	pFile = fopen(BIN_SAVE_FILENAME, "wb");

	if (NULL != pFile)
	{// ファイルがあった
		VERTEX_3D *pVtx;	// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点情報をファイルに書き込む
		fwrite(pVtx, sizeof(VERTEX_3D), m_nNumVtx, pFile);

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開けない
		MessageBox(0, "ファイルがありません！\nファイルを作成します。", "field.bin", MB_OK);
	}
}

//=========================================================================================================================
// 頂点情報をファイルから読み込む
//=========================================================================================================================
void CScene3DIndex::InputVtx(void)
{
	// ファイルのポインタ変数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen(m_aFileName, "rb");

	if (NULL != pFile)
	{// ファイルがあった
		VERTEX_3D *pVtx;	// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点情報をファイルに書き込む
		fread(pVtx, sizeof(VERTEX_3D), m_nNumVtx, pFile);

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開けない
		MessageBox(0, "ファイルがありません！", "field.bin", MB_OK);
	}
}