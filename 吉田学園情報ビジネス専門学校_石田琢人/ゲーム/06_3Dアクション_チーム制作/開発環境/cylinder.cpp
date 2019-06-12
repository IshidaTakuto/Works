//*****************************************************************************
//
//     円筒の処理[cylinder.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "cylinder.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define CYLINDER_TEXTURE_NAME  "data/TEXTURE/COMMON/mountain000.png"  // テクスチャのファイル名(山)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CCylinder::m_apTexture = NULL;          // 円筒のテクスチャ画像

//=============================================================================
//    コンストラクタ
//=============================================================================
CCylinder::CCylinder(int nPriority, OBJTYPE objType) : CScene3DMesh(nPriority, objType)
{
	// 各種値の初期化
	m_fRadius = 0.0f; // 円筒の半径
	m_fHeight = 0.0f; // 1ブロック分の奥行
	m_nXBlock = 0;    // 横の分割数
	m_nYBlock = 0;    // 縦の分割数
}

//=============================================================================
//    デストラクタ
//=============================================================================
CCylinder::~CCylinder()
{

}

//=============================================================================
//    円筒の生成処理
//=============================================================================
CCylinder *CCylinder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, int nXBlock, int nYBlock, int nPriority)
{
	CCylinder *pCylinder = NULL;           // 円筒クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pCylinder == NULL)
		{// メモリが空になっている
			pCylinder = new CCylinder(nPriority);
			if (pCylinder != NULL)
			{// インスタンスを生成できた
				if (FAILED(pCylinder->Init(pos, rot, fRadius, fHeight, nXBlock, nYBlock)))
				{// 初期化に失敗した
					return NULL;
				}
				else
				{// 初期化に成功した
				    // インスタンスのアドレスを返す
					return pCylinder;
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
//    円筒のテクスチャ読み込み処理
//=============================================================================
HRESULT CCylinder::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			D3DXCreateTextureFromFile(pDevice, CYLINDER_TEXTURE_NAME, &m_apTexture);
		}
	}
	return S_OK;
}

//=============================================================================
//    円筒のテクスチャ解放処理
//=============================================================================
void CCylinder::UnLoad(void)
{
	// テクスチャの破棄
	if (m_apTexture != NULL)
	{
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
//    円筒の初期化処理
//=============================================================================
HRESULT CCylinder::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, int nXBlock, int nYBlock)
{
	int nNumVertex;  // 頂点数設定用
	int nNumIndex;   // インデックス数計算用
	int nNumPolygon; // ポリゴン数計算用

	// 各種値の設定
	BindTexture(m_apTexture);  // テクスチャ
	SetPos(pos);               // 座標
	SetRot(rot);               // 向き
	m_fRadius = fRadius;       // 円筒の半径
	m_fHeight = fHeight;       // 1ブロック分の奥行
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
//    円筒の終了処理
//=============================================================================
void CCylinder::Uninit(void)
{
	// 共通の終了処理
	CScene3DMesh::Uninit();
}

//=============================================================================
//    円筒の更新処理
//=============================================================================
void CCylinder::Update(void)
{

}

//=============================================================================
//    円筒の描画処理
//=============================================================================
void CCylinder::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
	        // アルファテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);     // 有効に
			pDevice->SetRenderState(D3DRS_ALPHAREF, 200);             // 透明度が200
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // より大きいものを描画する

			// 共通の描画処理
			CScene3DMesh::Draw();

			// アルファテストを無効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);  // 無効に
		}
	}
}

//=============================================================================
//    円筒の頂点バッファ生成処理
//=============================================================================
void CCylinder::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	int nNumVertex = GetNumVertex();  // 頂点数

	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;  // 頂点バッファ生成用
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nNumVertex,
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
	D3DXVECTOR3 pos;    // 座標
	float Utex;         // テクスチャU座標
	float Vtex;         // テクスチャV座標

	fAngle = 0.0f;                          // 角度は0に
	fRadius = m_fRadius;                    // 半径を設定
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 座標を設定
	Utex = 0.0f;                            // テクスチャU座標を設定
	Vtex = 0.0f;                            // テクスチャV座標を設定

	for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
	{// 垂直方向の分割数だけ繰り返し
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// 水平方向の分割数だけ繰り返し
			pos.x = sinf(fAngle) * fRadius;
			pos.z = cosf(fAngle) * fRadius;

			// 頂点座標
			pVtx[nCntH].pos = pos;

			// 法線ベクトル
			pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

			// 頂点カラー
			pVtx[nCntH].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標
			pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);

			fAngle -= D3DX_PI / (m_nXBlock / 2.0f);  // 角度を進める
			Utex += 1.0f / (m_nXBlock / 2.0f);       // テクスチャU座標を進める

			if (fAngle > D3DX_PI)
			{// 角度が円周率を超えた
				fAngle -= D3DX_PI * 2.0f;
			}
		}
		pos.y += m_fHeight / m_nYBlock;  // 座標を上げる
		fAngle = 0.0f;                   // 角度は0に戻す
		Utex = 0.0f;                     // テクスチャU座標を戻す
		Vtex -= 1.0f / m_nYBlock;        // テクスチャV座標を進める
		pVtx += m_nXBlock + 1;           // 頂点バッファの番号を横の分割数分進める
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//     円筒のインデックスバッファ生成処理
//=============================================================================
void CCylinder::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	int nNumIndex = GetNumIndex();   // インデックス数

	// インデックスバッファの生成
	LPDIRECT3DINDEXBUFFER9 pIdxBuff = NULL;  // インデックスバッファ生成用
	pDevice->CreateIndexBuffer(sizeof(WORD) * nNumIndex,
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

	// インデックスバッファをアンロックする
	pIdxBuff->Unlock();

	// インデックスバッファを設定する
	SetIdxBuff(pIdxBuff);
}