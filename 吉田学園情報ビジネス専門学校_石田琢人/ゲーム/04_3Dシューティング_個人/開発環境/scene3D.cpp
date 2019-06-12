//=============================================================================
//
// 3Dオブジェクト処理 [scene3D.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "scene3D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//==================================
// マクロ定義
//==================================
#define SIZE_X	(50.0f)	// ポリゴンの横の長さ
#define SIZE_Y	(50.0f)	// ポリゴンの縦の長さ
#define SIZE_Z	(50.0f)	// ポリゴンの奥の長さ

//==================================
// 静的メンバ変数宣言
//==================================

//=========================================
// テクスチャを割り当てる
//=========================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// コンストラクタ
//=========================================
CScene3D::CScene3D(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ポリゴンの位置
	m_length = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================
// デストラクタ
//=========================================
CScene3D::~CScene3D()
{
}

//=========================================
// ポリゴンの初期化処理
//=========================================
HRESULT CScene3D::Init(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_pos.x - SIZE_X, m_pos.y - SIZE_Y, m_pos.z - SIZE_Z);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + SIZE_X, m_pos.y - SIZE_Y, m_pos.z - SIZE_Z);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - SIZE_X, m_pos.y - SIZE_Y, m_pos.z + SIZE_Z);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + SIZE_X, m_pos.y - SIZE_Y, m_pos.z + SIZE_Z);

	//1.0fで固定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

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
void CScene3D::Uninit(void)
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
void CScene3D::Update(void)
{
}

//=========================================
// ポリゴンの描画処理
//=========================================
void CScene3D::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

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

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// αテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================================================================================================
// 当たり判定
//=========================================================================================================================
bool CScene3D::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	if (this != NULL)
	{
		float fLengthOneself;
		float fRange0 = fRange;

		fLengthOneself = powf(fRange0 + fRange, 2);		// 相手のXZの範囲

		float fLengthX = pos.x - m_mtxWorld._41;	// Xの差
		float fLengthZ = pos.z - m_mtxWorld._43;	// Zの差
		float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2);			// XZの差の二乗

		if (fLengthOneself >= fLengthTotal)
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
const void CScene3D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=========================================
// 回転の設定
//=========================================
const void CScene3D::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=========================================
// 長さの設定
//=========================================
const void CScene3D::SetLength(D3DXVECTOR3 length)
{
	m_length = length;

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_length.x, m_length.y, m_length.z);
	pVtx[1].pos = D3DXVECTOR3(m_length.x, m_length.y, m_length.z);
	pVtx[2].pos = D3DXVECTOR3(-m_length.x, -m_length.y, -m_length.z);
	pVtx[3].pos = D3DXVECTOR3(m_length.x, -m_length.y, -m_length.z);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// ライフの設定
//=========================================
const int CScene3D::SubtractLife(int nDamage)
{
	m_nLife -= nDamage;	// ライフを引く
	return m_nLife;		// 計算後のライフを返す
};

//=========================================
// 色の設定
//=========================================
const void CScene3D::SetColor(D3DXCOLOR col)
{
	m_col = col;

	// 頂点情報を設定
	VERTEX_3D *pVtx;		// 頂点情報のポインタ

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
const void CScene3D::AddColor(D3DXCOLOR col)
{
	m_col += col;

	// 頂点情報を設定
	VERTEX_3D *pVtx;		// 頂点情報のポインタ

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
// テクスチャ座標の設定
//=========================================
const void CScene3D::SetTexPos(D3DXVECTOR2 *texPos)
{
	// 頂点情報を設定
	VERTEX_3D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = texPos[0];
	pVtx[1].tex = texPos[1];
	pVtx[2].tex = texPos[2];
	pVtx[3].tex = texPos[3];

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// テクスチャ座標を動かす
//=========================================
void CScene3D::MoveTexPos(D3DXVECTOR2 speed)
{
	// 頂点情報を設定
	VERTEX_3D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex += speed;
	pVtx[1].tex += speed;
	pVtx[2].tex += speed;
	pVtx[3].tex += speed;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}