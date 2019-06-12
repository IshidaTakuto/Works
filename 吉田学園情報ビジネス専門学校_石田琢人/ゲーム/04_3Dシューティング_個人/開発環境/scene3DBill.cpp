//=============================================================================
//
// 3Dビルボードオブジェクト処理 [scene3DBill.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "scene3DBill.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "game.h"
#include "player.h"

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
void CScene3DBill::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// コンストラクタ
//=========================================
CScene3DBill::CScene3DBill(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ポリゴンの位置
	m_fLengthX = 0.0f;							// X軸の大きさ
	m_fLengthY = 0.0f;							// Z軸の大きさ
}

//=========================================
// デストラクタ
//=========================================
CScene3DBill::~CScene3DBill()
{
}

//=========================================
// ポリゴンの初期化処理
//=========================================
HRESULT CScene3DBill::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = pRenderer->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(SIZE_X, SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SIZE_X, SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SIZE_X, SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SIZE_X, SIZE_Y, 0.0f);

	// 法線をカメラの位置で変える
	CPlayer *pPlayer = CGame::GetPlayer();	// プレイヤーの情報を取得
	if (NULL != pPlayer)
	{// プレイヤーの情報の取得
		CPlayer::TYPE type = pPlayer->GetType();
		if (type == CPlayer::TYPE_AIRCRAFT || type == CPlayer::TYPE_TANK)
		{// 戦闘機
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
		else if (type == CPlayer::TYPE_SHIP)
		{// 戦艦
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
	}
	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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
void CScene3DBill::Uninit(void)
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
void CScene3DBill::Update(void)
{
}

//=========================================
// ポリゴンの描画処理
//=========================================
void CScene3DBill::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//フォグを使用する
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	D3DXMATRIX mtxTrans, mtxView;	// ワールドマトリックスの保存用

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

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

	//フォグを使用する
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	// αテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=========================================================================================================================
// 当たり判定
//=========================================================================================================================
bool CScene3DBill::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	if (this != NULL)
	{
		float fLengthOneself;
		float fRange0 = fRange;

		fLengthOneself = powf(fRange0 * 3, 2);		// 相手のXYZの範囲

		float fLengthX = pos.x - m_mtxWorld._41;	// Xの差
		float fLengthY = pos.y - m_mtxWorld._42;	// Yの差
		float fLengthZ = pos.z - m_mtxWorld._43;	// Zの差
		float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);			// XYZの二乗の合計

		if (fLengthOneself >= fLengthTotal)
		{// 相手と自分の判定
			if (false == bHit)
			{// 判定を有効にする
				bHit = true;
			}
		}
	}
	return bHit;
}

//=========================================================================================================================
// 当たり判定(XY)
//=========================================================================================================================
bool CScene3DBill::CollisionXY(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	if (this != NULL)
	{
		float fLengthOneself;
		float fRange0 = fRange;

		fLengthOneself = powf(fRange0 * 2, 2);		// 相手のXYの範囲

		float fLengthX = pos.x - m_mtxWorld._41;	// Xの差
		float fLengthY = pos.y - m_mtxWorld._42;	// Yの差
		float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2);			// XYの差の二乗

		if (fLengthOneself >= fLengthTotal)
		{// 相手と自分の判定
			if (false == bHit)
			{// 判定を有効にする
				bHit = true;
			}
		}
	}
	return bHit;
}

//=========================================================================================================================
// 当たり判定(XZ)
//=========================================================================================================================
bool CScene3DBill::CollisionXZ(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	if (this != NULL)
	{
		float fLengthOneself;
		float fRange0 = fRange;

		fLengthOneself = powf(fRange0 * 2, 2);		// 相手のXZの範囲

		float fLengthX = pos.x - m_mtxWorld._41;	// Xの差
		float fLengthZ = pos.z - m_mtxWorld._43;	// Zの差
		float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2);			// XZの差の二乗

		if (fLengthOneself >= fLengthTotal)
		{// 相手と自分の判定
			if (false == bHit)
			{// 判定を有効にする
				bHit = true;
			}
		}
	}
	return bHit;
}

//=========================================================================================================================
// 当たり判定(Y)
//=========================================================================================================================
bool CScene3DBill::CollisionY(float posY, float fRange)
{
	bool bHit = false;	// 返す値

	if (this != NULL)
	{
		if (posY - fRange <= m_pos.y + m_fLengthY && posY + fRange >= m_pos.y - m_fLengthY)
		{// Y軸が範囲内
			if (false == bHit)
			{// 判定を有効にする
				bHit = true;
			}
		}
	}
	return bHit;
}

//=========================================
// 位置の設定
//=========================================
const void CScene3DBill::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=========================================
// ライフの設定
//=========================================
const int CScene3DBill::SubtractLife(int nDamage)
{
	m_nLife -= nDamage;	// ライフを引く
	return m_nLife;		// 計算後のライフを返す
};

//=========================================
// 色の設定
//=========================================
const void CScene3DBill::SetColor(D3DXCOLOR col)
{
	// 頂点情報を設定
	VERTEX_3D *pVtx;		// 頂点情報のポインタ

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
const void CScene3DBill::AddColor(D3DXCOLOR col)
{
	// 頂点情報を設定
	VERTEX_3D *pVtx;		// 頂点情報のポインタ

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

//=========================================
// 大きさの設定
//=========================================
const void CScene3DBill::SetLength(float fLength0, float fLength1)
{
	m_fLengthX = fLength0;
	m_fLengthY = fLength1;

	// 頂点情報を設定
	VERTEX_3D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].pos = D3DXVECTOR3(-fLength0, fLength1,0.0f);
	pVtx[1].pos = D3DXVECTOR3(fLength0, fLength1, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fLength0, -fLength1, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fLength0, -fLength1, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// 頂点座標の設定
//=========================================
const void CScene3DBill::SetVtxPos(D3DXVECTOR3 *vtxPos)
{
	// 頂点情報を設定
	VERTEX_3D *pVtx;		// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].pos = vtxPos[0];
	pVtx[1].pos = vtxPos[1];
	pVtx[2].pos = vtxPos[2];
	pVtx[3].pos = vtxPos[3];

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=========================================
// テクスチャ座標の設定
//=========================================
const void CScene3DBill::SetTexPos(D3DXVECTOR2 *tex)
{
	// 頂点情報を設定
	VERTEX_3D *pVtx;		// 頂点情報のポインタ

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
