//=============================================================================
//
// 2Dオブジェクト処理 [scene2D.cpp]
// Author : Ishida Takuto
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
// テクスチャを割り当てる
//=========================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// コンストラクタ
//=========================================
CScene2D::CScene2D(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ポリゴンの位置
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

	//// 頂点の角度
	//m_fAngle = atan2f(m_pos.x - (m_pos.x + SIZE_X), m_pos.y - (m_pos.y + SIZE_Y));

	//// 中心から頂点までの距離
	//m_fLength = sqrtf((SIZE_X * SIZE_X) + (SIZE_Y * SIZE_Y));

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

	pVtx[0].pos = D3DXVECTOR3(m_pos.x - SIZE_X, m_pos.y - SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + SIZE_X, m_pos.y - SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - SIZE_X, m_pos.y + SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + SIZE_X, m_pos.y + SIZE_Y, 0.0f);

	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
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

	Release();
}

//=========================================
// ポリゴンの更新処理
//=========================================
void CScene2D::Update(void)
{
#if 0	// ポリゴンの回転
	m_fCntRot += 0.01f;		// 回転のカウンタ

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = (m_pos.x) - sinf(m_fAngle + m_fCntRot) * m_fLength;
	pVtx[0].pos.y = (m_pos.y) - cosf(m_fAngle + m_fCntRot) * m_fLength;
	pVtx[1].pos.x = (m_pos.x) + sinf(m_fAngle - m_fCntRot) * m_fLength;
	pVtx[1].pos.y = (m_pos.y) - cosf(m_fAngle - m_fCntRot) * m_fLength;
	pVtx[2].pos.x = (m_pos.x) - sinf(m_fAngle - m_fCntRot) * m_fLength;
	pVtx[2].pos.y = (m_pos.y) + cosf(m_fAngle - m_fCntRot) * m_fLength;
	pVtx[3].pos.x = (m_pos.x) + sinf(m_fAngle + m_fCntRot) * m_fLength;
	pVtx[3].pos.y = (m_pos.y) + cosf(m_fAngle + m_fCntRot) * m_fLength;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
#endif
}

//=========================================
// ポリゴンの描画処理
//=========================================
void CScene2D::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = pRenderer->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
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
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y - m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y - m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y + m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y + m_fLength, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// サイズの設定
//=========================================
const void CScene2D::SetLength(float fLength)
{
	m_fLength = fLength;

	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y - m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y - m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y + m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y + m_fLength, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// テクスチャ座標の設定
//=========================================
const void CScene2D::SetTexPos(D3DXVECTOR2 *tex)
{
	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = tex[0];
	pVtx[1].tex = tex[1];
	pVtx[2].tex = tex[2];
	pVtx[3].tex = tex[3];

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}

//=========================================
// 頂点座標の設定
//=========================================
const void CScene2D::SetVtxPos(D3DXVECTOR3 *pos)
{
	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].pos = pos[0];
	pVtx[1].pos = pos[1];
	pVtx[2].pos = pos[2];
	pVtx[3].pos = pos[3];

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// ライフの設定
//=========================================
const int CScene2D::SubtractLife(int nDamage)
{
	m_nLife -= nDamage;	// ライフを引く
	return m_nLife;		// 計算後のライフを返す
};

//=========================================
// 色の設定
//=========================================
const void CScene2D::SetColor(D3DXCOLOR col)
{
	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 色の加算
//=========================================
const void CScene2D::AddColor(D3DXCOLOR col)
{
	// 頂点情報を設定
	VERTEX_2D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].col += col;
	pVtx[1].col += col;
	pVtx[2].col += col;
	pVtx[3].col += col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
