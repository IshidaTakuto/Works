//*****************************************************************************
//
//     シーン(3D)の処理[scene3D.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "scene3D.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define POLYGON3D_TEXTURE_NAME  "data/TEXTURE/field004.jpg"     // テクスチャのファイル名

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CScene3D::m_apTexture = NULL;  // 3Dポリゴンのテクスチャ画像

//=============================================================================
//    コンストラクタ
//=============================================================================
CScene3D::CScene3D(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pTexture = NULL;                         // テクスチャへのポインタ
	m_pVtxBuff = NULL;                         // 頂点バッファへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 向き
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // 色
	D3DXMatrixIdentity(&m_MtxWorld);           // ワールドマトリックス
}

//=============================================================================
//    デストラクタ
//=============================================================================
CScene3D::~CScene3D()
{

}

//=============================================================================
//    ポリゴンの生成処理
//=============================================================================
CScene3D *CScene3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nPriority)
{
	CScene3D *pScene3D = NULL;             // シーン3Dクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pScene3D == NULL)
		{// メモリが空になっている
			pScene3D = new CScene3D(nPriority);
			if (pScene3D != NULL)
			{// インスタンスを生成できた
			 // 各種値の代入
				pScene3D->SetPos(pos);  // 座標
				pScene3D->SetRot(rot);  // 向き
				pScene3D->SetCol(col);  // 色
				if (FAILED(pScene3D->Init()))
				{// 初期化に失敗した
					return NULL;
				}
				else
				{// 初期化に成功した
				 // インスタンスのアドレスを返す
					return pScene3D;
				}
			}
			else
			{// インスタンスを生成できなかった
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}
}

//=============================================================================
//    ポリゴンのテクスチャ読み込み処理
//=============================================================================
HRESULT CScene3D::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	 // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			D3DXCreateTextureFromFile(pDevice, POLYGON3D_TEXTURE_NAME, &m_apTexture);
		}
	}
	return S_OK;
}

//=============================================================================
//    ポリゴンのテクスチャ解放処理
//=============================================================================
void CScene3D::UnLoad(void)
{
	// テクスチャの破棄
	if (m_apTexture != NULL)
	{
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
//    ポリゴンの初期化処理
//=============================================================================
HRESULT CScene3D::Init(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			// 頂点バッファの生成
			MakeVertex(pDevice);
		}
		else
		{// デバイスを取得できなかった
			return E_FAIL;
		}
	}
	else
	{// レンダリングクラスが生成されていない
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    ポリゴンの終了処理
//=============================================================================
void CScene3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// リリース処理
	CScene::Release();
}

//=============================================================================
//    ポリゴンの更新処理
//=============================================================================
void CScene3D::Update(void)
{

}

//=============================================================================
//    ポリゴンの描画処理
//=============================================================================
void CScene3D::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			// ワールドマトリックスの設定処理
			SetMtxWorld(pDevice);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, m_pTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
}

//=============================================================================
//    ポリゴンのワールドマトリックス計算処理
//=============================================================================
void CScene3D::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // 計算用マトリックス

    // ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    ポリゴンの頂点バッファ生成処理
//=============================================================================
void CScene3D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-300.0f, 80.0f, 300.0f);
	pVtx[1].pos = D3DXVECTOR3(300.0f, 40.0f, 300.0f);
	pVtx[2].pos = D3DXVECTOR3(-300.0f, 20.0f, -300.0f);
	pVtx[3].pos = D3DXVECTOR3(300.0f, 100.0f, -300.0f);

	// 法線計算用ベクトル
	D3DXVECTOR3 vecVer[4];

	// 頂点の法線
	D3DXVECTOR3 nor1, nor2;

	// 外積用にベクトルを作る
	vecVer[0] = pVtx[0].pos - pVtx[2].pos;
	vecVer[1] = pVtx[1].pos - pVtx[2].pos;
	vecVer[2] = pVtx[2].pos - pVtx[1].pos;
	vecVer[3] = pVtx[3].pos - pVtx[1].pos;

	// ベクトルを外積で計算して法線を出す
	D3DXVec3Cross(&nor1, &vecVer[0], &vecVer[1]);
	D3DXVec3Cross(&nor2, &vecVer[3], &vecVer[2]);
	D3DXVec3Normalize(&nor1, &nor1);
	D3DXVec3Normalize(&nor2, &nor2);

	// 法線を代入
	pVtx[0].nor = nor1;
	pVtx[1].nor = (nor1 + nor2) / 2;
	pVtx[2].nor = (nor1 + nor2) / 2;
	pVtx[3].nor = nor2;

	// 頂点カラー
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ポリゴンのテクスチャ設定処理
//=============================================================================
void CScene3D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    ポリゴンの頂点バッファ設定処理
//=============================================================================
void CScene3D::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    ポリゴンの座標設定処理
//=============================================================================
void CScene3D::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ポリゴンの向き設定処理
//=============================================================================
void CScene3D::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ポリゴンの色設定処理
//=============================================================================
void CScene3D::SetCol(const D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
//    ポリゴンのワールドマトリックス設定処理
//=============================================================================
void CScene3D::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    ポリゴンの頂点バッファ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CScene3D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    ポリゴンのテクスチャ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CScene3D::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    ポリゴンの座標取得処理
//=============================================================================
D3DXVECTOR3 CScene3D::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ポリゴンの向き取得処理
//=============================================================================
D3DXVECTOR3 CScene3D::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ポリゴンの色取得処理
//=============================================================================
D3DXCOLOR CScene3D::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    ポリゴンのワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CScene3D::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    ポリゴンの高さ取得処理
//=============================================================================
float CScene3D::GetPolyHeight(D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;        // 高さ
	D3DXVECTOR3 PolygonNormal;   // 頂点の法線
	D3DXVECTOR3 PolygonPos;      // プレイヤーとのベクトルを作るポリゴンの頂点座標
	D3DXVECTOR3 Vec;             // プレイヤーと頂点座標とのベクトル
	D3DXVECTOR3 VerPos[4];       // 3Dポリゴンの頂点座標
	D3DXVECTOR3 VerNor[4];       // 3Dポリゴンの頂点法線
	D3DXMATRIX VerMtxWorld[4];   // 3Dポリゴンの頂点ワールドマトリックス
	bool bLeftPolygon = false;   // 左側のポリゴンに乗っているかどうか
	bool bRightPolygon = false;  // 右側のポリゴンに乗っているかどうか
	int nCntLeftPolygon = 0;     // 左側ポリゴンのベクトルの判定を通った回数
	int nCntRightPolygon = 0;    // 右側ポリゴンのベクトルの判定を通った回数

	// 頂点情報を取得する
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 判定するベクトルの座標を取得
	VerPos[0] = pVtx[0].pos;
	VerPos[1] = pVtx[1].pos;
	VerPos[2] = pVtx[2].pos;
	VerPos[3] = pVtx[3].pos;

	// 判定に使う法線を取得
	VerNor[0] = pVtx[0].nor;
	VerNor[1] = pVtx[1].nor;
	VerNor[2] = pVtx[2].nor;
	VerNor[3] = pVtx[3].nor;

	CDebugProc::Print("\n頂点0番目の法線 : ( %.1f %.1f %.1f )\n", VerNor[0].x, VerNor[0].y, VerNor[0].z);
	CDebugProc::Print("頂点1番目の法線 : ( %.1f %.1f %.1f )\n", VerNor[1].x, VerNor[1].y, VerNor[1].z);
	CDebugProc::Print("頂点2番目の法線 : ( %.1f %.1f %.1f )\n", VerNor[2].x, VerNor[2].y, VerNor[2].z);
	CDebugProc::Print("頂点3番目の法線 : ( %.1f %.1f %.1f )\n", VerNor[3].x, VerNor[3].y, VerNor[3].z);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 3Dポリゴンの向きを考慮して頂点のワールドマトリックスを計算する
	D3DXMATRIX mtxRot, mtxTrans;        // 計算用マトリックス
	D3DXMATRIX mtxParent = m_MtxWorld;  // 3Dポリゴンのワールドマトリックスを取得
	D3DXVECTOR3 VerRot = m_Rot;         // 3Dポリゴンの向きを取得
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// 判定する頂点の数だけ繰り返し
	 // ワールドマトリックスの初期化
		D3DXMatrixIdentity(&VerMtxWorld[nCntVer]);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, VerRot.y, VerRot.x, VerRot.z);
		D3DXMatrixMultiply(&VerMtxWorld[nCntVer], &VerMtxWorld[nCntVer], &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, VerPos[nCntVer].x, VerPos[nCntVer].y, VerPos[nCntVer].z);
		D3DXMatrixMultiply(&VerMtxWorld[nCntVer], &VerMtxWorld[nCntVer], &mtxTrans);

		// 親のワールドマトリックスを掛け合わせる
		D3DXMatrixMultiply(&VerMtxWorld[nCntVer], &VerMtxWorld[nCntVer], &mtxParent);
	}


	// 各種値の取得
	D3DXVECTOR3 WorldPolygonPos[4] =
	{// 向きを含めた3Dポリゴンの頂点ワールド座標
		D3DXVECTOR3(VerMtxWorld[0]._41,VerMtxWorld[0]._42,VerMtxWorld[0]._43),
		D3DXVECTOR3(VerMtxWorld[1]._41,VerMtxWorld[1]._42,VerMtxWorld[1]._43),
		D3DXVECTOR3(VerMtxWorld[2]._41,VerMtxWorld[2]._42,VerMtxWorld[2]._43),
		D3DXVECTOR3(VerMtxWorld[3]._41,VerMtxWorld[3]._42,VerMtxWorld[3]._43),
	};

	D3DXVECTOR3 WorldLeftPolygonPos[3] =
	{// 向きを含めた3Dポリゴン(左側)のワールド座標
		WorldPolygonPos[1],
		WorldPolygonPos[2],
		WorldPolygonPos[0],
	};
	D3DXVECTOR3 WorldRightPolygonPos[3] =
	{// 向きを含めた3Dポリゴン(右側)のワールド座標
		WorldPolygonPos[1],
		WorldPolygonPos[3],
		WorldPolygonPos[2],
	};
	CDebugProc::Print("( 左側のポリゴン )\n");
	for (int nCntVer = 0; nCntVer < 3; nCntVer++)
	{// 判定する頂点の数だけ繰り返し
		CDebugProc::Print("頂点の座標%d番目 : ( %.1f %.1f %.1f )\n", nCntVer, WorldLeftPolygonPos[nCntVer].x, WorldLeftPolygonPos[nCntVer].y, WorldLeftPolygonPos[nCntVer].z);
	}
	CDebugProc::Print("\n( 右側のポリゴン )\n");
	for (int nCntVer = 0; nCntVer < 3; nCntVer++)
	{// 判定する頂点の数だけ繰り返し
		CDebugProc::Print("頂点の座標%d番目 : ( %.1f %.1f %.1f )\n", nCntVer, WorldLeftPolygonPos[nCntVer].x, WorldLeftPolygonPos[nCntVer].y, WorldLeftPolygonPos[nCntVer].z);
	}

	CDebugProc::Print("\n( ベクトルの判定開始 )\n\n");
	// 計算用外積ベクトル
	D3DXVECTOR3 VecVerToVer, VecPlayerToVer;
	// 左側のポリゴンに乗っているか判定
	for (int nCntVer = 0; nCntVer < 3; nCntVer++)
	{// 判定する頂点の数だけ繰り返し
		VecVerToVer = WorldRightPolygonPos[(nCntVer + 1) % 3] - WorldRightPolygonPos[nCntVer];
		VecPlayerToVer = pos - WorldRightPolygonPos[nCntVer];

		if ((VecVerToVer.x * VecPlayerToVer.z) - (VecVerToVer.z * VecPlayerToVer.x) <= 0)
		{// ベクトルの右側にいる
			nCntRightPolygon++;
		}
	}

	// 右側のポリゴンに乗っているか判定
	for (int nCntVer = 0; nCntVer < 3; nCntVer++)
	{// 判定する頂点の数だけ繰り返し
		VecVerToVer = WorldLeftPolygonPos[(nCntVer + 1) % 3] - WorldLeftPolygonPos[nCntVer];
		VecPlayerToVer = pos - WorldLeftPolygonPos[nCntVer];

		if ((VecVerToVer.x * VecPlayerToVer.z) - (VecVerToVer.z * VecPlayerToVer.x) <= 0)
		{// ベクトルの右側にいる
			nCntLeftPolygon++;
		}
	}


	D3DXVECTOR3 PolygonVerPos[3];
	D3DXVECTOR3 VecPolygon[2];
	D3DXVECTOR3 nor;
	float fData = 0.0f;  // 補正値

	if (nCntLeftPolygon == 3)
	{// 左側のポリゴンのすべてのベクトルの内側にいる
		bLeftPolygon = true;
		CDebugProc::Print("左側のポリゴンに乗ってます\n");
		PolygonVerPos[0] = D3DXVECTOR3(WorldPolygonPos[0].x, WorldPolygonPos[0].y - WorldPolygonPos[0].y, WorldPolygonPos[0].z);
		PolygonVerPos[1] = D3DXVECTOR3(WorldPolygonPos[1].x, WorldPolygonPos[1].y - WorldPolygonPos[0].y, WorldPolygonPos[1].z);
		PolygonVerPos[2] = D3DXVECTOR3(WorldPolygonPos[2].x, WorldPolygonPos[2].y - WorldPolygonPos[0].y, WorldPolygonPos[2].z);

		VecPolygon[0] = PolygonVerPos[1] - PolygonVerPos[0];
		VecPolygon[1] = PolygonVerPos[2] - PolygonVerPos[0];

		D3DXVec3Cross(&PolygonNormal, &VecPolygon[0], &VecPolygon[1]);
		D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);
		fData = WorldPolygonPos[0].y;

		// 引数の座標と頂点座標のベクトルを計算
		Vec = pos - PolygonVerPos[0];

		// 内積の概念を利用して高さを割り出す
		float DotX = (PolygonNormal.x * Vec.x);      // X成分の内積を計算
		float DotZ = (PolygonNormal.z * Vec.z);      // Z成分の内積を計算
		float Dot = -DotX - DotZ;                    // X成分とZ成分の内積同士を引く
		fHeight = (Dot / PolygonNormal.y) + fData;   // 引いたものをY成分の法線で割る
		CDebugProc::Print("地面の法線 : ( %.3f %.3f %.3f )\n", PolygonNormal.x, PolygonNormal.y, PolygonNormal.z);
		CDebugProc::Print("補正値: ( %.3f )\n", fData);
		CDebugProc::Print("地面の高さ : ( %.3f )\n", fHeight);
	}
	else
	{// 左側のポリゴンに乗っていない
		CDebugProc::Print("左側のポリゴンに乗ってません\n");
	}
	if (nCntRightPolygon == 3)
	{// 右側のポリゴンのすべてのベクトルの内側にいる
		bRightPolygon = true;
		CDebugProc::Print("右側のポリゴンに乗ってます\n");
		PolygonVerPos[0] = D3DXVECTOR3(WorldPolygonPos[3].x, WorldPolygonPos[3].y - WorldPolygonPos[3].y, WorldPolygonPos[3].z);
		PolygonVerPos[1] = D3DXVECTOR3(WorldPolygonPos[1].x, WorldPolygonPos[1].y - WorldPolygonPos[3].y, WorldPolygonPos[1].z);
		PolygonVerPos[2] = D3DXVECTOR3(WorldPolygonPos[2].x, WorldPolygonPos[2].y - WorldPolygonPos[3].y, WorldPolygonPos[2].z);

		VecPolygon[0] = PolygonVerPos[1] - PolygonVerPos[0];
		VecPolygon[1] = PolygonVerPos[2] - PolygonVerPos[0];

		D3DXVec3Cross(&PolygonNormal, &VecPolygon[0], &VecPolygon[1]);
		D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);
		fData = WorldPolygonPos[3].y;

		// 引数の座標と頂点座標のベクトルを計算
		Vec = pos - PolygonVerPos[0];

		// 内積の概念を利用して高さを割り出す
		float DotX = (PolygonNormal.x * Vec.x);      // X成分の内積を計算
		float DotZ = (PolygonNormal.z * Vec.z);      // Z成分の内積を計算
		float Dot = -DotX - DotZ;                    // X成分とZ成分の内積同士を引く
		fHeight = (Dot / PolygonNormal.y) + fData;   // 引いたものをY成分の法線で割る
		CDebugProc::Print("地面の法線 : ( %.3f %.3f %.3f )\n", PolygonNormal.x, PolygonNormal.y, PolygonNormal.z);
		CDebugProc::Print("補正値: ( %.3f )\n", fData);
		CDebugProc::Print("地面の高さ : ( %.3f )\n", fHeight);
	}
	else
	{// 右側のポリゴンに乗っていない
		CDebugProc::Print("右側のポリゴンに乗ってません\n");
	}


	if (bLeftPolygon == true || bRightPolygon == true)
	{// どちらかのポリゴンに乗ってい
	}

	return fHeight;  // 高さを返す
}