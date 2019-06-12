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
CNumber* CNumber::Create(D3DXVECTOR3 pos, float fLength)
{
	CNumber *pNumber;

	pNumber = new CNumber;	// メモリを確保
	pNumber->Init(pos, fLength);

	return pNumber;
}

//=========================================
// テクスチャの読み込み
//=========================================
HRESULT CNumber::Load(void)
{
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
	// テクスチャの破棄
	if (NULL != m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=========================================
// コンストラクタ
//=========================================
CNumber::CNumber()
{
	m_nCountAnim = 0;
	m_nPatternAnim = 0;
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
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fLength)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = pRenderer->GetDevice();

	// ポリゴンの位置を設定
	m_pos = pos;
	m_fLength = fLength;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y - (m_fLength * 1.3f), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y - (m_fLength * 1.3f), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y + (m_fLength * 1.3f), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y + (m_fLength * 1.3f), 0.0f);

	// 1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CNumber::Uninit(void)
{
	if (m_pTexture != NULL)
	{// テクスチャのポインタを開放
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
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
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// αテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================
// 数字の設定
//=========================================
const void CNumber::SetNumber(int nNumber)
{
	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + nNumber * 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + nNumber * 0.1f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 位置の設定
//=========================================
const void CNumber::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y - m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y - m_fLength, 0.0f);;
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y + m_fLength, 0.0f);;
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y + m_fLength, 0.0f);;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// サイズの設定
//=========================================
const void CNumber::SetLength(float fLength)
{
	m_fLength = fLength;

	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y - m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y - m_fLength, 0.0f);;
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y + m_fLength, 0.0f);;
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y + m_fLength, 0.0f);;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 色の設定
//=========================================
const void CNumber::SetColor(D3DXCOLOR col)
{
	// 頂点情報を設定
	VERTEX_2D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラー
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
