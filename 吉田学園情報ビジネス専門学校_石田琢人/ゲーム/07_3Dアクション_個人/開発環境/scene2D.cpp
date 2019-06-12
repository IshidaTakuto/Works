//=============================================================================
//
// 2Dオブジェクト処理 [scene.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//==================================
// マクロ定義
//==================================
#define SIZE_X	(50.0f)	// ポリゴンの横の長さ
#define SIZE_Y	(50.0f)	// ポリゴンの縦の長さ

//==================================
// 静的メンバ変数宣言
//==================================

//=========================================
// 生成処理
//=========================================
CScene2D *CScene2D::Create(void)
{
	CScene2D *pScene2D = new CScene2D;

	if (pScene2D != NULL)
	{// メモリ確保成功
		pScene2D->Init();
	}

	return pScene2D;
}

//=========================================
// テクスチャを割り当てる
//=========================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// コンストラクタ
//=========================================
CScene2D::CScene2D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ポリゴンの位置
	m_bDisp = true;
	m_fSizeX = 0.0f;
	m_fSizeY = 0.0f;

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=========================================
// デストラクタ
//=========================================
CScene2D::~CScene2D()
{
}

//=========================================
// ポリゴンの初期化処理
//=========================================
HRESULT CScene2D::Init(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

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
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - SIZE_X, m_pos.y - SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + SIZE_X, m_pos.y - SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - SIZE_X, m_pos.y + SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + SIZE_X, m_pos.y + SIZE_Y, 0.0f);

	// 1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=========================================
// ポリゴンの終了処理
//=========================================
void CScene2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (NULL != m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 自分の破棄
	CScene::Release();
}

//=========================================
// ポリゴンの更新処理
//=========================================
void CScene2D::Update(void)
{
}

//=========================================
// ポリゴンの描画処理
//=========================================
void CScene2D::Draw(void)
{
	if (m_bDisp)
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
}

//=========================================================================================================================
// 当たり判定
//=========================================================================================================================
bool CScene2D::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	if (this != NULL)
	{
		if (pos.x - fRange < m_pos.x
			&& pos.x + fRange > m_pos.x
			&& pos.y - fRange < m_pos.y
			&& pos.y + fRange > m_pos.y)
		{// 相手と自分の判定
			if (false == bHit)
			{// 攻撃判定を有効にする
				bHit = true;
			}
		}
	}
	return bHit;
}

//=========================================
// 位置の設定
//=========================================
const void CScene2D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fSizeX, m_pos.y - m_fSizeY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y - m_fSizeY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fSizeX, m_pos.y + m_fSizeY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y + m_fSizeY, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// サイズの設定
//=========================================
const void CScene2D::SetSize(float fSizeX, float fSizeY)
{
	// サイズの値を入れる
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f) + m_pos;
	pVtx[1].pos = D3DXVECTOR3(m_fSizeX, 0.0f, 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(0.0f, m_fSizeY, 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3(m_fSizeX, m_fSizeY, 0.0f) + m_pos;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// サイズの設定
//=========================================
const void CScene2D::SetWidth(float fWidth)
{
	// サイズの値を入れる
	m_fSizeX = fWidth;

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	//pVtx[0].pos.x = m_pos.x - m_fSizeX;
	pVtx[1].pos.x = m_pos.x + m_fSizeX;
	//pVtx[2].pos.x = m_pos.x - m_fSizeX;
	pVtx[3].pos.x = m_pos.x + m_fSizeX;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 高さの設定
//=========================================
const void CScene2D::SetHeight(float fHeight)
{
	// サイズの値を入れる
	m_fSizeY = fHeight;

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos.y = m_pos.y - m_fSizeY;
	pVtx[1].pos.y = m_pos.y - m_fSizeY;
	pVtx[2].pos.y = m_pos.y + m_fSizeY;
	pVtx[3].pos.y = m_pos.y + m_fSizeY;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 色の設定
//=========================================
const void CScene2D::SetColor(D3DXCOLOR col)
{
	// 色の設定
	m_col = col;

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 色の加算
//=========================================
const void CScene2D::AddColor(D3DXCOLOR col)
{
	// 色の設定
	m_col += col;

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
