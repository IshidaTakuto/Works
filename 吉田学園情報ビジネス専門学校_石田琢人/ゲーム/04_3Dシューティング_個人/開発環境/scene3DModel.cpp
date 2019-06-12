//=============================================================================
//
// 3Dモデルオブジェクト処理 [scene3DModel.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "scene3DModel.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "light.h"

//==================================
// マクロ定義
//==================================

//==================================
// 静的メンバ変数宣言
//==================================

//=========================================
// モデル情報を割り当てる
//=========================================
const void CScene3DModel::BindInfo(LPDIRECT3DTEXTURE9 pTexture, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat)
{
	m_pTexture = pTexture;
	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_nNumMat = nNumMat;
}

//=========================================
// コンストラクタ
//=========================================
CScene3DModel::CScene3DModel(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pBuffMat = NULL;		// マテリアルへのポインタ
	m_pMesh = NULL;
	m_nIdxShadow = 0;			// 影の番号
	m_vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	m_vtxMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ポリゴンの位置
}

//=========================================
// デストラクタ
//=========================================
CScene3DModel::~CScene3DModel()
{
}

//=========================================
// ポリゴンの初期化処理
//=========================================
HRESULT CScene3DModel::Init(void)
{
	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

	// Xの値の比較
	if (m_vtxMin.x > vtx.x)
	{// 最小値と比較
		m_vtxMin.x = vtx.x;	// 最小値より小さければ代入
	}

	if (m_vtxMax.x < vtx.x)
	{// 最大値と比較
		m_vtxMax.x = vtx.x;	// 最大値より大きければ代入
	}

	// Yの値の比較
	if (m_vtxMin.y > vtx.y)
	{// 最小値と比較
		m_vtxMin.y = vtx.y;	// 最小値より小さければ代入
	}

	if (m_vtxMax.y < vtx.y)
	{// 最大値と比較
		m_vtxMax.y = vtx.y;	// 最大値より大きければ代入
	}

	// Zの値の比較
	if (m_vtxMin.z > vtx.z)
	{// 最小値と比較
		m_vtxMin.z = vtx.z;	// 最小値より小さければ代入
	}

	if (m_vtxMax.z < vtx.z)
	{// 最大値と比較
		m_vtxMax.z = vtx.z;	// 最大値より大きければ代入
	}

	// 頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=========================================
// ポリゴンの終了処理
//=========================================
void CScene3DModel::Uninit(void)
{
	Release();
}

//=========================================
// ポリゴンの更新処理
//=========================================
void CScene3DModel::Update(void)
{
}

//=========================================
// ポリゴンの描画処理
//=========================================
void CScene3DModel::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);

		//オブジェクトのテクスチャーをステージ０にセットする
		pDevice->SetTexture(0, m_pTexture);

		// モデルの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=========================================================================================================================
// 当たり判定
//=========================================================================================================================
bool CScene3DModel::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange)
{
	bool bTouch = false;

	float fLengthOneself;		// 相手の円状範囲
	float fRange0 = fRange;		// 相手の範囲の半径の長さ

	fLengthOneself = powf(fRange0, 2);		// 相手のXZの範囲

	float fLengthX = pPos->x - m_mtxWorld._41;	// Xの差
	float fLengthZ = pPos->z - m_mtxWorld._43;	// Zの差
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2);		// XZの差の二乗

	if (fLengthOneself >= fLengthTotal)
	{// 相手と自分の判定
		*pPos = *pPosold;							// 位置を戻す
		*pMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量を0にする

		bTouch = true;
	}
	return bTouch;
}

//=========================================================================================================================
// 攻撃の当たり判定
//=========================================================================================================================
bool CScene3DModel::CollisionAttack(D3DXVECTOR3 pos, float fRange)
{
	bool bAttack = false;

	float fLengthOneself;		// 相手の円状範囲
	float fRange0 = fRange;		// 相手の範囲の半径の長さ

	fLengthOneself = powf(fRange0, 2);		// 相手のXZの範囲

	float fLengthX = pos.x - m_mtxWorld._41;	// Xの差
	float fLengthZ = pos.z - m_mtxWorld._43;	// Zの差
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2);		// XZの差の二乗

	if (pos.y + fRange >= m_mtxWorld._42 + (fLengthTotal / 2) && pos.y - fRange <= m_mtxWorld._42 - (fLengthTotal / 2))
	{
		if (fLengthOneself >= fLengthTotal)
		{// 相手と自分の判定
			bAttack = true;
		}
	}
	return bAttack;
}

//=========================================
// 位置の設定
//=========================================
const void CScene3DModel::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=========================================
// ライフの設定
//=========================================
const int CScene3DModel::SubtractLife(int nDamage)
{
	m_nLife -= nDamage;	// ライフを引く
	return m_nLife;		// 計算後のライフを返す
};

//=========================================
// 色の設定
//=========================================
const void CScene3DModel::SetColor(D3DXCOLOR col)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// 色を付ける
	m_pMat->MatD3D.Diffuse = col;
	m_pMat->MatD3D.Ambient = col;

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=========================================
// 色の加算
//=========================================
const void CScene3DModel::AddColor(D3DXCOLOR col)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
									// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// 色を付ける
	m_pMat->MatD3D.Diffuse.r += col.r;
	m_pMat->MatD3D.Diffuse.g += col.g;
	m_pMat->MatD3D.Diffuse.b += col.b;
	m_pMat->MatD3D.Diffuse.a += col.a;

	m_pMat->MatD3D.Ambient.r += col.r;
	m_pMat->MatD3D.Ambient.g += col.g;
	m_pMat->MatD3D.Ambient.b += col.b;
	m_pMat->MatD3D.Ambient.a += col.a;

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}
