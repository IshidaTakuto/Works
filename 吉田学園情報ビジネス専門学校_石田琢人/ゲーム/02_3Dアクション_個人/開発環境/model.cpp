//=============================================================================
//
// モデル処理 [model.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "model.h"
#include "score.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODELNAME00		"data/MODEL/sakura.x"		// 読み込むxファイル名
#define MODELNAME01		"data/MODEL/tree.x"		// 読み込むxファイル名
#define MODELNAME02		"data/MODEL/momiji.x"			// 読み込むxファイル名
#define MODELNAME03		"data/MODEL/snowtree.x"			// 読み込むxファイル名
#define MODELNAME04		"data/MODEL/item.x"			// 読み込むxファイル名

#define MAX_TYPE	(5)
#define MAX_MODEL	(61)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH g_pMeshModel[MAX_TYPE] = {};				// メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatModel[MAX_TYPE] = {};		// マテリアル情報へのポインタ
LPDIRECT3DTEXTURE9 g_pBuffTexModel[MAX_TYPE] = {};
DWORD g_nNumMatModel = 0;				// マテリアル情報の数
int g_nIdxShadow;						// 影の番号
Model g_Model[MAX_MODEL];				// モデルの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
	int nCntModel;

	// 位置・向きの初期設定
	for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_Model[nCntModel].pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
		g_Model[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Model[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Model[nCntModel].fDestAngle = 0.0f;
		g_Model[nCntModel].nType = nCntModel;

		g_Model[nCntModel].vtxMin = D3DXVECTOR3(10000, 10000, 10000);
		g_Model[nCntModel].vtxMax = D3DXVECTOR3(1, 1, 1);

		g_Model[nCntModel].bUse = false;
	}

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODELNAME00,		// 桜
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[0],
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel[0]);

	D3DXLoadMeshFromX(MODELNAME01,		// 木
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[1],
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel[1]);

	D3DXLoadMeshFromX(MODELNAME02,		// 紅葉
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[2],
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel[2]);

	D3DXLoadMeshFromX(MODELNAME03,		// 冬の木
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[3],
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel[3]);

	D3DXLoadMeshFromX(MODELNAME04,		// アイテム
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[4],
		NULL,
		&g_nNumMatModel,
		&g_pMeshModel[4]);


	for (nCntModel = 0; nCntModel < MAX_TYPE; nCntModel++)
	{
		pMat = (D3DXMATERIAL*)g_pBuffMatModel[nCntModel]->GetBufferPointer();

		D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &g_pBuffTexModel[nCntModel]);
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(void)
{
	int nCntModel;

	for (nCntModel = 0; nCntModel < MAX_TYPE; nCntModel++)
	{
		// メッシュの開放
		if (g_pMeshModel[nCntModel] != NULL)
		{
			g_pMeshModel[nCntModel]->Release();
			g_pMeshModel[nCntModel] = NULL;
		}

		// マテリアルの開放
		if (g_pBuffMatModel[nCntModel] != NULL)
		{
			g_pBuffMatModel[nCntModel]->Release();
			g_pBuffMatModel[nCntModel] = NULL;
		}
	}

	for (int nCntTex = 0; nCntTex < MAX_TYPE; nCntTex++)
	{
		if (g_pBuffTexModel[nCntTex] != NULL)
		{
			g_pBuffTexModel[nCntTex]->Release();
			g_pBuffTexModel[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateModel(void)
{
	int nCntModel;

	for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_Model[nCntModel].bUse == true && g_Model[nCntModel].type == MODELTYPE_ITEM)
		{
			g_Model[nCntModel].rot.y += 0.1f;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	int nCntModel;

	for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Model[nCntModel].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Model[nCntModel].rot.y, g_Model[nCntModel].rot.x, g_Model[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_Model[nCntModel].mtxWorld, &g_Model[nCntModel].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTrans, g_Model[nCntModel].pos.x, g_Model[nCntModel].pos.y, g_Model[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_Model[nCntModel].mtxWorld, &g_Model[nCntModel].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Model[nCntModel].mtxWorld);

		if (g_Model[nCntModel].bUse == true)
		{
			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatModel[g_Model[nCntModel].nType]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_pBuffTexModel[g_Model[nCntModel].nType]);

				// モデル(パーツ)の描画
				g_pMeshModel[g_Model[nCntModel].nType]->DrawSubset(nCntMat);
			}

		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// 当たり判定の処理
//=============================================================================
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange)
{
	int nCount;			// for文のカウンタ
	bool bHit = false;	// 接触していたらtrue

	for (nCount = 0; nCount < MAX_MODEL; nCount++)
	{// 全ての敵分回す
		if (true == g_Model[nCount].bUse)
		{// 使用されている敵のみ
			float fLengthOneself;				// 相手の円状範囲

			fLengthOneself = powf(fRange, 2);	// 相手のXZの範囲

			float fLengthX = pPos->x - g_Model[nCount].mtxWorld._41;		// Xの差
			float fLengthZ = pPos->z - g_Model[nCount].mtxWorld._43;		// Zの差
			float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2);		// XZの差の二乗

			if (fLengthOneself >= fLengthTotal)
			{// 相手と自分の判定
				if (MODELTYPE_OBJECT == g_Model[nCount].type)
				{// 障害物の場合
					*pPos = *pPosold;							// 位置を戻す
					*pMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量を0にする
				}
				else if (MODELTYPE_ITEM == g_Model[nCount].type)
				{// アイテムの場合
					DeleteShadow(g_Model[nCount].nIdxShadow);	// 影を消す
					g_Model[nCount].bUse = false;				// モデルを使用していない状態にする
				}
				bHit = true;
			}
		}
	}
	return bHit;
}

//=============================================================================
// モデルの設定
//=============================================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, MODELTYPE type)
{
	int nCntModel;

	for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_Model[nCntModel].bUse == false)
		{
			g_Model[nCntModel].pos = pos;
			g_Model[nCntModel].rot = rot;
			g_Model[nCntModel].nType = nType;
			g_Model[nCntModel].type = type;

			g_Model[nCntModel].bUse = true;

			g_Model[nCntModel].nIdxShadow = SetShadow(pos, rot, 10.0f, 10.0f);

			int nNumVtx;		// 頂点数
			DWORD sizeFVF;		// 頂点フォーマットのサイズ
			BYTE *pVtxBuff;		// 頂点バッファへのポインタ

			// 頂点数を取得
			nNumVtx = g_pMeshModel[g_Model[nCntModel].nType]->GetNumVertices();

			// 頂点フォーマットのサイズを取得
			sizeFVF = D3DXGetFVFVertexSize(g_pMeshModel[g_Model[nCntModel].nType]->GetFVF());

			// 頂点バッファをロック
			g_pMeshModel[g_Model[nCntModel].nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// 頂点座標の比較

				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

				// Xの値の比較
				if (g_Model[nCntModel].vtxMin.x > vtx.x)
				{// 最小値と比較
					g_Model[nCntModel].vtxMin.x = vtx.x;	// 最小値より小さければ代入
				}

				if (g_Model[nCntModel].vtxMax.x < vtx.x)
				{// 最大値と比較
					g_Model[nCntModel].vtxMax.x = vtx.x;	// 最大値より大きければ代入
				}

				// Yの値の比較
				if (g_Model[nCntModel].vtxMin.y > vtx.y)
				{// 最小値と比較
					g_Model[nCntModel].vtxMin.y = vtx.y;	// 最小値より小さければ代入
				}

				if (g_Model[nCntModel].vtxMax.y < vtx.y)
				{// 最大値と比較
					g_Model[nCntModel].vtxMax.y = vtx.y;	// 最大値より大きければ代入
				}

				// Zの値の比較
				if (g_Model[nCntModel].vtxMin.z > vtx.z)
				{// 最小値と比較
					g_Model[nCntModel].vtxMin.z = vtx.z;	// 最小値より小さければ代入
				}

				if (g_Model[nCntModel].vtxMax.z < vtx.z)
				{// 最大値と比較
					g_Model[nCntModel].vtxMax.z = vtx.z;	// 最大値より大きければ代入
				}
			}

			// 頂点バッファをアンロック
			g_pMeshModel[g_Model[nCntModel].nType]->UnlockVertexBuffer();

			break;
		}
	}
}