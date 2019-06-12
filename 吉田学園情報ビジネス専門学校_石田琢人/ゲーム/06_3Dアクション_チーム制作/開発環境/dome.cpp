//*****************************************************************************
//
//     半球の処理[dome.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "dome.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define DOME_TEXTURE_NAME  "data/TEXTURE/COMMON/sky000.jpg"  // テクスチャのファイル名(空)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CDome::m_apTexture = NULL;          // 半球のテクスチャ画像

//=============================================================================
//    コンストラクタ
//=============================================================================
CDome::CDome(int nPriority, OBJTYPE objType) : CScene3DMesh(nPriority, objType)
{
	// 各種値の初期化
	m_fRadius = 0.0f;        // 半球の半径
	m_nXBlock = 0;           // 横の分割数
	m_nYBlock = 0;           // 縦の分割数
}

//=============================================================================
//    デストラクタ
//=============================================================================
CDome::~CDome()
{

}

//=============================================================================
//    半球の生成処理
//=============================================================================
CDome *CDome::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nXBlock, int nYBlock, int nPriority)
{
	CDome *pDome = NULL;                   // 半球クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pDome == NULL)
		{// メモリが空になっている
			pDome = new CDome(nPriority);
			if (pDome != NULL)
			{// インスタンスを生成できた
				if (FAILED(pDome->Init(pos, rot, fRadius, nXBlock, nYBlock)))
				{// 初期化に失敗した
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
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pDome;   // インスタンスのアドレスを返す
}

//=============================================================================
//    半球のテクスチャ読み込み処理
//=============================================================================
HRESULT CDome::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			D3DXCreateTextureFromFile(pDevice, DOME_TEXTURE_NAME, &m_apTexture);
		}
	}
	return S_OK;
}

//=============================================================================
//    半球のテクスチャ解放処理
//=============================================================================
void CDome::UnLoad(void)
{
	// テクスチャの破棄
	if (m_apTexture != NULL)
	{
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
//    半球の初期化処理
//=============================================================================
HRESULT CDome::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nXBlock, int nYBlock)
{
	int nNumVertex;  // 頂点数設定用
	int nNumIndex;   // インデックス数計算用
	int nNumPolygon; // ポリゴン数計算用

	// 各種値の設定
	BindTexture(m_apTexture);  // テクスチャ
	SetPos(pos);               // 座標
	SetRot(rot);               // 向き
	m_fRadius = fRadius;       // 半球の半径
	m_nXBlock = nXBlock;       // 横の分割数
	m_nYBlock = nYBlock;       // 縦の分割数

	// メッシュの作成
	// 作成に必要な頂点数,インデックス数,ポリゴン数を計算
	nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);                                                               // 頂点数を計算
	nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);  // インデックス数を設定
	nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);                                          // 描画するポリゴン数を設定

	// 計算した値を設定する
	SetNumVertex(nNumVertex);    // 頂点数
	SetNumIndex(nNumIndex);      // インデックス数
	SetNumPolygon(nNumPolygon);  // ポリゴン数

	// 共通の初期化処理
	CScene3DMesh::Init();

	return S_OK;
}

//=============================================================================
//    半球の終了処理
//=============================================================================
void CDome::Uninit(void)
{
	// 共通の終了処理
	CScene3DMesh::Uninit();
}

//=============================================================================
//    半球の更新処理
//=============================================================================
void CDome::Update(void)
{
	D3DXVECTOR3 rot = GetRot();  // 向きの取得

	// 向きを回転させる
	rot.y += D3DX_PI / 14400;
	if (rot.y > D3DX_PI)
	{// 円周率を超えた
		rot.y -= D3DX_PI * 2.0f;
	}

	// 向きの設定
	SetRot(rot);
}

//=============================================================================
//    半球の描画処理
//=============================================================================
void CDome::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			// 共通の描画処理
			CScene3DMesh::Draw();

			// 蓋の描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (m_nXBlock + 1), GetNumIndex(), (m_nXBlock));
		}
	}
}

//=============================================================================
//    半球の頂点バッファ生成処理
//=============================================================================
void CDome::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	int nNumVertex = GetNumVertex();  // 頂点数

	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;  // 頂点バッファ生成用
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (nNumVertex + (m_nXBlock + 2)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 0;   // 角度
	float fRadius = 0;  // 半径
	float fDome = 0.0f; // ドーム計算用(半径を縮める倍率)
	D3DXVECTOR3 pos;    // 座標
	float Utex;         // テクスチャU座標
	float Vtex;         // テクスチャV座標

	fAngle = 0.0f;                       // 角度は0に
	fRadius = cosf(fDome) * m_fRadius;   // 半径を設定
	fDome = 0.0f;                        // 倍率を設定
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 座標を設定
	Utex = 0.0f;                         // テクスチャU座標を設定
	Vtex = -1.0f;                        // テクスチャV座標を設定

	for (int nCntV = 0; nCntV <  m_nYBlock + 1; nCntV++)
	{// 垂直方向の分割数だけ繰り返し
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// 水平方向の分割数だけ繰り返し
			pos.x = sinf(fAngle) * fRadius;
			pos.y = sinf(fDome) * m_fRadius;
			pos.z = cosf(fAngle) * fRadius;

			// 頂点座標
			pVtx[nCntH].pos = pos;

			// 法線ベクトル
			pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

			// 頂点カラー
			pVtx[nCntH].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標
			pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);

			fAngle -= D3DX_PI / (m_nXBlock / 2);  // 角度を進める
			Utex += 1.0f / (m_nXBlock / 2);    // テクスチャU座標を進める

			if (fAngle <= -D3DX_PI)
			{// 倍率が円周率を超えた
				fAngle = D3DX_PI;
			}
		}
		fDome += 1.0f / m_nYBlock;           // 半径を縮める倍率を上げる
		fRadius = cosf(fDome) * m_fRadius;   // 半径をコサインカーブで縮めていく
		fAngle = 0.0f;                       // 角度は0に
		Utex = 0.0f;                         // テクスチャU座標を設定
		Vtex -= 1.0f / (m_nYBlock);          // テクスチャV座標を進める
		pVtx += m_nXBlock + 1;               // 頂点の先頭の番号を進める
	}

	// 蓋用の頂点情報を作成
	fDome += 2.0f / m_nYBlock;   // 半径を縮める倍率を上げる
	// 頂点座標
	pos = D3DXVECTOR3(0.0f, sinf(fDome) * m_fRadius, 0.0f);
	pVtx[0].pos = pos;

	// 法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -2.2f);

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, -0.99f);
	fDome -= 2.0f / m_nYBlock;   // 半径を縮める倍率を下げる
	pVtx++;

	fDome -= 1.0f / m_nYBlock;           // 半径を縮める倍率を戻しておく
	fRadius = cosf(fDome) * m_fRadius;   // 半径計算を戻しておく
	for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
	{// 水平方向の分割数だけ繰り返し
		pos.x = sinf(fAngle) * fRadius;
		pos.y = sinf(fDome) * m_fRadius;
		pos.z = cosf(fAngle) * fRadius;

		// 頂点座標
		pVtx[nCntH].pos = pos;

		// 法線ベクトル
		pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

		// 頂点カラー
		pVtx[nCntH].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標
		pVtx[nCntH].tex = D3DXVECTOR2(0.0f, -0.98f);

		fAngle += -D3DX_PI / (m_nXBlock / 2);  // 角度を進める
		Utex -= 1.0f / (m_nXBlock / 2);    // テクスチャU座標を進める

		if (fAngle <= -D3DX_PI)
		{// 倍率が円周率を超えた
			fAngle = D3DX_PI;
		}
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//     半球のインデックスバッファ生成処理
//=============================================================================
void CDome::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	int nNumIndex = GetNumIndex();   // インデックス数

	// インデックスバッファの生成
	LPDIRECT3DINDEXBUFFER9 pIdxBuff = NULL;  // インデックスバッファ生成用
	pDevice->CreateIndexBuffer(sizeof(WORD) * (nNumIndex + (m_nXBlock + 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pIdxBuff,
		NULL);

	WORD *pIdx;       // インデックスデータへのポインタ
	int nCntIdx = 0;  // インデックス番号

	// インデックスバッファをロックし,インデックスデータへのポインタを取得
	pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < m_nYBlock; nCntIdxY++)
	{// 縦の分割数分繰り返し
		for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
		{// 横の分割数+１繰り返し
		 // 上下の二つ分インデックス番号を設定
			pIdx[0] = nCntIdx + m_nXBlock + 1; // 下側
			pIdx[1] = nCntIdx;                 // 上側

			pIdx += 2;  // 2つ分進める
			if (nCntIdxY < m_nYBlock - 1 && nCntIdxX == m_nXBlock)
			{// 1 , 下側が縦の分割数の最下層ではない
			 // 2 , 横の分割数分設定が終わった
				pIdx[0] = nCntIdx;                       // 上側
				pIdx[1] = nCntIdx + (m_nXBlock + 1) + 1; // 次の下側

				pIdx += 2; // 2つ分進める
			}
		}
	}

	// 蓋のインデックスを設定
	nCntIdx += m_nXBlock + 1;
	// 蓋の先頭番号を設定
	for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 2; nCntIdxX++, nCntIdx++)
	{// 横の分割数+2繰り返し
		pIdx[0] = nCntIdx;
		pIdx++;  // 1つ分進める
	}

	// インデックスバッファをアンロックする
	pIdxBuff->Unlock();

	// インデックスバッファを設定する
	SetIdxBuff(pIdxBuff);
}