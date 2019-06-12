//=============================================================================
//
// 数字処理 [number.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "number.h"
#include "manager.h"
#include "renderer.h"

//==================================
// マクロ定義
//==================================
#define TEXTURE_NAME	"data/TEXTURE/number000.png"

//==================================
// 静的メンバ変数宣言
//==================================
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//==================================
// 生成処理
//==================================
CNumber* CNumber::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CNumber *pNumber = NULL;

	pNumber = new CNumber;	// メモリを確保

	if (pNumber)
	{// メモリ確保成功
		pNumber->Init(pos, fWidth, fHeight);	// 初期化処理
	}

	return pNumber;
}

//=========================================
// テクスチャの読み込み
//=========================================
HRESULT CNumber::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);

	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CNumber::Unload(void)
{
	if (NULL != m_pTexture)
	{// テクスチャの破棄
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=========================================
// コンストラクタ
//=========================================
CNumber::CNumber(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_nNumber = 0;
}

//=========================================
// デストラクタ
//=========================================
CNumber::~CNumber()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// 初期化処理
	CScene2D::Init();

	// テクスチャの設定
	CScene2D::BindTexture(m_pTexture);

	// 位置の設定
	CScene2D::SetPos(pos);

	// サイズの設定
	CScene2D::SetSize(fWidth, fHeight);

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CNumber::Uninit(void)
{
	// 終了処理
	CScene2D::Uninit();
}

//=========================================
// 更新処理
//=========================================
void CNumber::Update(void)
{
}

//=========================================
// 描画処理
//=========================================
void CNumber::Draw(void)
{
	// 描画処理
	CScene2D::Draw();
}

//=========================================
// 数字の設定
//=========================================
const void CNumber::SetNumber(int nNumber)
{
	// 値の保存
	m_nNumber = nNumber;

	// 頂点情報の取得
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene2D::GetVtxBuff();

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f + m_nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + m_nNumber * 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + m_nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + m_nNumber * 0.1f, 1.0f);

	// 頂点バッファをアンロック
	vtxBuff->Unlock();
}

//=========================================
// サイズの取得
//=========================================
void CNumber::GetSize(float *pWidth, float *pHeight)
{
	if (pWidth)
	{// NULLでないとき値を入れる
		*pWidth = CScene2D::GetWidth();
	}

	if (pHeight)
	{// NULLでないとき値を入れる
		*pHeight = CScene2D::GetHeight();
	}
}
