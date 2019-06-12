//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "bg.h"
#include "renderer.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_TEXTURE00	"data/TEXTURE/sky.jpg"		// 背景のテクスチャ名
#define BG_TEXTURE01	"data/TEXTURE/cloud.jpg"	// 背景のテクスチャ名
#define BG_TEXTURE02	"data/TEXTURE/mountain000.png"	// 背景のテクスチャ名

//==================================
// 静的メンバ変数宣言
//==================================
LPDIRECT3DTEXTURE9 CBg::m_pTexture[MAX_BG] = {};
int CBg::m_nPriority = 0;

//==================================
// 生成処理
//==================================
CBg* CBg::Create(int nTexture, bool bAdd, int nPriority)
{
	CBg *pBg;

	// 優先番号の設定
	m_nPriority = nPriority;

	pBg = new CBg;	// メモリを確保

	if (pBg != NULL)
	{// メモリ確保成功
		pBg->Init();								// 初期化処理
		pBg->BindTexture(m_pTexture[nTexture]);		// テクスチャの設定
		pBg->m_bAdd = bAdd;							// 加算合成するかどうか
	}

	return pBg;
}

//=========================================
// テクスチャの読み込み
//=========================================
HRESULT CBg::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE00, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE01, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE02, &m_pTexture[2]);

	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CBg::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_BG; nCntTex++)
	{
		if (NULL != m_pTexture[nCntTex])
		{// テクスチャの破棄
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=========================================
// コンストラクタ
//=========================================
CBg::CBg(CScene::OBJTYPE objType) : CScene2D(m_nPriority, objType)
{
}

//=========================================
// デストラクタ
//=========================================
CBg::~CBg()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CBg::Init()
{
	CScene2D::Init();

	// 頂点情報の取得
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene2D::GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// 頂点バッファをアンロック
	vtxBuff->Unlock();

	// ポリゴンの位置を設定
	CScene2D::SetVtxBuff(vtxBuff);

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CBg::Uninit(void)
{
	// 自分を破棄
	CScene2D::Release();
}

//=========================================
// 更新処理
//=========================================
void CBg::Update(void)
{
}

//=========================================
// 描画処理
//=========================================
void CBg::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 被らないようにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	if (m_bAdd)
	{// αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	CScene2D::Draw();

	if (m_bAdd)
	{// αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// 被らないようにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=========================================
// テクスチャ座標移動
//=========================================
void CBg::TexMove(float fMove)
{
	CScene::SetObjType(CScene::OBJTYPE_FADE);		// オブジェクトタイプをフェードに設定

	// 頂点情報の取得
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene2D::GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標を更新
	pVtx[0].tex.x += fMove;
	pVtx[1].tex.x += fMove;
	pVtx[2].tex.x += fMove;
	pVtx[3].tex.x += fMove;

	// 頂点バッファをアンロック
	vtxBuff->Unlock();

	// ポリゴンの位置を設定
	CScene2D::SetVtxBuff(vtxBuff);
}