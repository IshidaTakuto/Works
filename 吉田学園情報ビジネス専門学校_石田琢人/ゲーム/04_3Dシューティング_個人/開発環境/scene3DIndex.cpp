//=============================================================================
//
// インデックスバッファ3Dオブジェクト処理 [scene3DIndex.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "scene3DIndex.h"
#include "renderer.h"
#include "manager.h"

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
void CScene3DIndex::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=========================================
// コンストラクタ
//=========================================
CScene3DIndex::CScene3DIndex(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;		// テクスチャへのポインタ
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ポリゴンの位置
	m_nLengthX = 0;
	m_nLengthY = 0;
	m_nLengthZ = 0;
	m_nMeshX = 0;
	m_nMeshY = 0;
	m_nMeshZ = 0;
	m_type = TYPE_NONE;
}

//=========================================
// デストラクタ
//=========================================
CScene3DIndex::~CScene3DIndex()
{
}

//=========================================
// ポリゴンの初期化処理
//=========================================
HRESULT CScene3DIndex::Init(void)
{
	return S_OK;
}

//=========================================
// ポリゴンの終了処理
//=========================================
void CScene3DIndex::Uninit(void)
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
void CScene3DIndex::Update(void)
{
}

//=========================================
// ポリゴンの描画処理
//=========================================
void CScene3DIndex::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = pRenderer->GetDevice();

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

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVtx, 0, m_nNumPolygon);
}

//=========================================================================================================================
// 当たり判定
//=========================================================================================================================
bool CScene3DIndex::Collision(D3DXVECTOR3 *pos, float fRadius)
{
	bool bLand = false;

	if (this != NULL)
	{
		if (pos->x - fRadius > m_pos.x - m_nLengthX
			&& pos->x + fRadius < m_pos.x + m_nLengthX
			&& pos->z - fRadius > m_pos.z - m_nLengthZ
			&& pos->z + fRadius < m_pos.z + m_nLengthZ)
		{// 相手と自分の判定
			if (false == bLand)
			{// 攻撃判定を有効にする
				bLand = true;
				pos->y = m_pos.y;
			}
		}
	}
	return bLand;
}

//=========================================================================================================================
// 頂点情報の作成
//=========================================================================================================================
void CScene3DIndex::MakeVertex(int meshX, int meshY, int meshZ, int nLengthX, int nLengthY, int nLengthZ, TYPE type)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = pRenderer->GetDevice();

	int nCntMeshYZ;
	int nCntMeshX;
	int nCntIdx;

	m_nLengthX = nLengthX;
	m_nLengthY = nLengthY;
	m_nLengthZ = nLengthZ;

	m_nMeshX = meshX;
	m_nMeshY = meshY;
	m_nMeshZ = meshZ;

	m_type = type;

	m_nNumVtx = (meshX + 1) * ((meshZ + meshY) + 1);
	m_nNumPolygon = meshX * (meshZ + meshY) * 2 + (4 * ((meshZ + meshY) - 1));

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスバッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (meshX * (meshZ + meshY) * 2 + (4 * ((meshZ + meshY) - 1)) + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,			// 16ビットのデータを確保
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshYZ = 0; nCntMeshYZ <= (meshZ + meshY); nCntMeshYZ++)
	{
		for (nCntMeshX = 0; nCntMeshX <= meshX; nCntMeshX++)
		{
			// 頂点の座標
			switch(type)
			{
			case TYPE_FIELD:		// 地面
				pVtx[0].pos = D3DXVECTOR3(((nLengthX / 2.0f) - ((nLengthX * nCntMeshX) / meshX)) * 1.0f, 0.0f, ((nLengthZ * nCntMeshYZ) / meshZ) * 1.0f);
				break;

			case TYPE_WALL:			// 壁
				pVtx[0].pos = D3DXVECTOR3((nLengthX / 2.0f) - ((nLengthX * nCntMeshX) / meshX), (nLengthY * nCntMeshYZ * 1.0f) / meshY, 0);
				break;

			case TYPE_CYLINDER:		// シリンダー
				float fAngle = 1.0f / (meshX / 2);	// 角度

				pVtx[0].pos.x = sinf(D3DX_PI * (0.0f + (fAngle * nCntMeshX))) * nLengthX;		// ずれる
				pVtx[0].pos.y = nLengthY - (nCntMeshYZ * nLengthY) * 1.0f;								// Y軸を下へずらす
				pVtx[0].pos.z = cosf(D3DX_PI * (0.0f + (fAngle * nCntMeshX))) * nLengthZ;		// ずれる
				break;
			}

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(nCntMeshX * 1.0f, nCntMeshYZ * 1.0f);

			pVtx += 1;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	WORD *pIdx;		// インデックスデータへのポインタ

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	if (TYPE_CYLINDER == type)
	{// シリンダーのとき
		for (int nCntMeshH = 0; nCntMeshH <= meshX; nCntMeshH++)
		{
			pIdx[0] = (meshX + 1) + nCntMeshH;
			pIdx[1] = 0 + nCntMeshH;

			pIdx += 2;
		}
	}
	else
	{// 地面と壁
		for (nCntMeshYZ = 0, nCntIdx = 0; nCntMeshYZ < (meshZ + meshY); nCntMeshYZ++)
		{
			for (nCntMeshX = 0; nCntMeshX <= meshX; nCntMeshX++, nCntIdx++)
			{
				pIdx[0] = (meshX + 1) + nCntIdx;
				pIdx[1] = 0 + nCntIdx;

				pIdx += 2;

				if (nCntMeshYZ < (meshZ + meshY) - 1 && nCntMeshX == meshX)
				{// 折り返し時
					pIdx[0] = 0 + nCntIdx;
					pIdx[1] = (meshX + 1) + nCntIdx + 1;

					pIdx += 2;
				}
			}
		}
	}

	// 頂点バッファをアンロック
	m_pIdxBuff->Unlock();
}

//=========================================================================================================================
// 位置の設定
//=========================================================================================================================
const void CScene3DIndex::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshYZ = 0; nCntMeshYZ <= (m_nMeshZ + m_nMeshY); nCntMeshYZ++)
	{
		for (int nCntMeshX = 0; nCntMeshX <= m_nMeshX; nCntMeshX++)
		{
			// 頂点の座標
			switch (m_type)
			{
			case TYPE_FIELD:		// 地面
				pVtx[0].pos = D3DXVECTOR3(((m_nLengthX / 2.0f) - ((m_nLengthX * nCntMeshX) / m_nMeshX)) * 1.0f, 0.0f, ((m_nLengthZ * nCntMeshYZ) / m_nMeshZ) * 1.0f) + m_pos;
				break;

			case TYPE_WALL:			// 壁
				pVtx[0].pos = D3DXVECTOR3((m_nLengthX / 2.0f) - ((m_nLengthX * nCntMeshX) / m_nMeshX), (m_nLengthY * nCntMeshYZ * 1.0f) / m_nMeshY, 0) + m_pos;
				break;

			case TYPE_CYLINDER:		// シリンダー
				float fAngle = 1.0f / (m_nMeshX / 2);	// 角度

				pVtx[0].pos.x = sinf(D3DX_PI * (0.0f + (fAngle * nCntMeshX))) * m_nLengthX;		// ずれる
				pVtx[0].pos.y = m_nLengthY - (nCntMeshYZ * m_nLengthY) + m_pos.y;				// Y軸を下へずらす
				pVtx[0].pos.z = cosf(D3DX_PI * (0.0f + (fAngle * nCntMeshX))) * m_nLengthZ;		// ずれる
				break;
			}

			pVtx++;
		}
	}
}
