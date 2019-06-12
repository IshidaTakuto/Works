//=============================================================================
//
// モデル処理 [Object3.cpp]
// Author : 有馬 武志
//
//=============================================================================
#include "object3.h"	//オブジェクト
#include "input.h"		//キーボード
#include "camera.h"		//カメラ
#include "shadow.h"		//影
#include "player.h"		//プレイヤー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OBJECT3_NAME		"data\\MODEL\\ビル200.x"		//読み込むテクスチャファイル
#define MAX_TEX3			(1)								//オブジェクトのテクスチャ最大数
#define MAX_OBJECT3			(20)								//オブジェクトの最大数
#define MAX_VARIATION3		(1)								//オブジェクトの種類
#define PULS3				(35)							//あたり判定		

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Object3 g_Object3[MAX_OBJECT3];

//=============================================================================
// 初期化処理
//=============================================================================
void InitObject3(void)
{
	//デバイスポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// Xファイルの読み込み	
	D3DXLoadMeshFromX
	(
		OBJECT3_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Object3[0].pBuffMatObject,
		NULL, &g_Object3[0].nNumMatObject,
		&g_Object3[0].pMeshObject
	);
	
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 *pMatDef;

	//テクスチャをつける
	for (int nCount = 0; nCount < MAX_VARIATION3; nCount++)
	{
		pMat = (D3DXMATERIAL*)g_Object3[nCount].pBuffMatObject->GetBufferPointer();
		pMatDef = new D3DMATERIAL9[g_Object3[nCount].nNumMatObject];
		g_Object3[nCount].pMeshTextures = new LPDIRECT3DTEXTURE9[g_Object3[nCount].nNumMatObject];

		for (DWORD tex = 0; tex < g_Object3[nCount].nNumMatObject; tex++)
		{
			pMatDef[tex] = pMat[tex].MatD3D;
			pMatDef[tex].Ambient = pMatDef[tex].Diffuse;
			g_Object3[nCount].pMeshTextures[tex] = NULL;
			if (pMat[tex].pTextureFilename != NULL &&
				lstrlen(pMat[tex].pTextureFilename) > 0)
			{
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pMat[tex].pTextureFilename,
					&g_Object3[nCount].pMeshTextures[tex])))
				{
					//MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
				}
			}
		}
	}

	for (int nCntObject = 0; nCntObject < MAX_OBJECT3; nCntObject++)
	{
		g_Object3[nCntObject].VtxMinObject = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		g_Object3[nCntObject].VtxMaxObject = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_Object3[nCntObject].pos = D3DXVECTOR3(10000, 0, 0);
		g_Object3[nCntObject].rot = D3DXVECTOR3(0, 0, 0);
		g_Object3[nCntObject].bUse = false;
	}

	int nNumVtx;			//頂点数
	DWORD sizeFVF;			//頂点フォーマット7のサイズ
	BYTE *pVtxBuff;			//頂点バッファへのポインタ

	for (int nCount = 0; nCount < MAX_VARIATION3; nCount++)
	{
		//頂点数を取得
		nNumVtx = g_Object3[nCount].pMeshObject->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_Object3[nCount].pMeshObject->GetFVF());

		//頂点バッファをロック
		g_Object3[nCount].pMeshObject->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標の代入

													   //範囲指定
			//最大値
			if (vtx.x < g_Object3[nCount].VtxMinObject.x)
			{
				g_Object3[nCount].VtxMinObject.x = vtx.x;
			}
			if (vtx.z < g_Object3[nCount].VtxMinObject.z)
			{
				g_Object3[nCount].VtxMinObject.z = vtx.z;
			}
			if (vtx.y < g_Object3[nCount].VtxMinObject.y)
			{
				g_Object3[nCount].VtxMinObject.y = vtx.y;
			}
			//最小値
			if (vtx.x > g_Object3[nCount].VtxMaxObject.x)
			{
				g_Object3[nCount].VtxMaxObject.x = vtx.x;
			}
			if (vtx.z > g_Object3[nCount].VtxMaxObject.z)
			{
				g_Object3[nCount].VtxMaxObject.z = vtx.z;
			}
			if (vtx.y > g_Object3[nCount].VtxMaxObject.y)
			{
				g_Object3[nCount].VtxMaxObject.y = vtx.y;
			}
			pVtxBuff += sizeFVF;		//サイズ分ポインタを進める
		}
		//頂点バッファをアンロック
		g_Object3[nCount].pMeshObject->UnlockVertexBuffer();
	}

	//オブジェクトの代入
	//for(カウント　= 全体のオブジェクトの数を最初に+する)
	for (int nPotaCountObject = MAX_VARIATION3; nPotaCountObject < MAX_OBJECT3; nPotaCountObject++)
	{
		if (nPotaCountObject < MAX_OBJECT3 + MAX_VARIATION3)
		{//周りのでかい岩
			g_Object3[nPotaCountObject] = g_Object3[0];
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitObject3(void)
{
	for (int nCntObject = 0; nCntObject < MAX_VARIATION3; nCntObject++)
	{
		// メッシュの開放
		if (g_Object3[nCntObject].pMeshObject != NULL)
		{
			g_Object3[nCntObject].pMeshObject->Release();
			g_Object3[nCntObject].pMeshObject = NULL;
		}
		// マテリアルの開放
		if (g_Object3[nCntObject].pBuffMatObject != NULL)
		{
			g_Object3[nCntObject].pBuffMatObject->Release();
			g_Object3[nCntObject].pBuffMatObject = NULL;
		}

		//テクスチャ
		for (DWORD nTex = 0; nTex < g_Object3[nCntObject].nNumMatObject; nTex++)
		{
			if (g_Object3[nCntObject].pMeshTextures[nTex] != NULL)
			{
				g_Object3[nCntObject].pMeshTextures[nTex]->Release();
				g_Object3[nCntObject].pMeshTextures[nTex] = NULL;
			}
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateObject3(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawObject3(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	for (int nCount2 = 0; nCount2 < MAX_OBJECT3; nCount2++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Object3[nCount2].mtxWorldObject);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Object3[nCount2].rot.y, g_Object3[nCount2].rot.x, g_Object3[nCount2].rot.z);

		D3DXMatrixMultiply(&g_Object3[nCount2].mtxWorldObject, &g_Object3[nCount2].mtxWorldObject, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTrans, g_Object3[nCount2].pos.x, g_Object3[nCount2].pos.y, g_Object3[nCount2].pos.z);

		D3DXMatrixMultiply(&g_Object3[nCount2].mtxWorldObject, &g_Object3[nCount2].mtxWorldObject, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Object3[nCount2].mtxWorldObject);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_Object3[nCount2].pBuffMatObject->GetBufferPointer();

		for (int nCntMat = 0; nCntMat<(int)g_Object3[nCount2].nNumMatObject; nCntMat++)
		{
			if (g_Object3[nCount2].bUse == true)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				// テクスチャの設定
				pDevice->SetTexture(0, g_Object3[nCount2].pMeshTextures[nCntMat]);
				// モデル(パーツ)の描画
				g_Object3[nCount2].pMeshObject->DrawSubset(nCntMat);
			}
		}
		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
}
//=============================================================================
// セットモデル
//=============================================================================
void SetObject3(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntObject;

	for (nCntObject = 0; nCntObject < MAX_OBJECT3; nCntObject++)
	{
		if (g_Object3[nCntObject].bUse == false)
		{
			g_Object3[nCntObject].pos = pos;
			g_Object3[nCntObject].rot = rot;
			g_Object3[nCntObject].bUse = true;
			break;
		}
	}
}
//=============================================================================
// モデルの当たり判定
//=============================================================================
bool CollisionObject3(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove)
{
	int nCount;
	bool bLand = false;
	Player * pPlayer = GetPlayer();

	for (nCount = 0; nCount < MAX_OBJECT3; nCount++)
	{
		if (pPos->x > g_Object3[nCount].pos.x + g_Object3[nCount].VtxMinObject.x - PULS3 && pPos->x < g_Object3[nCount].pos.x + g_Object3[nCount].VtxMaxObject.x + PULS3)
		{	//Xの範囲
			if (pPosold->z >= g_Object3[nCount].pos.z + g_Object3[nCount].VtxMaxObject.z + PULS3 && pPos->z < g_Object3[nCount].pos.z + g_Object3[nCount].VtxMaxObject.z + PULS3)
			{
				pPos->z = g_Object3[nCount].pos.z + g_Object3[nCount].VtxMaxObject.z + PULS3;
				pPos->z = pPosold->z;
			}
			else if (pPosold->z <= g_Object3[nCount].pos.z + g_Object3[nCount].VtxMinObject.z - PULS3 && pPos->z > g_Object3[nCount].pos.z + g_Object3[nCount].VtxMinObject.z - PULS3)
			{
				pPos->z = g_Object3[nCount].pos.z + g_Object3[nCount].VtxMinObject.z - PULS3;
				pPos->z = pPosold->z;
			}
		}
		if (pPos->z > g_Object3[nCount].pos.z + g_Object3[nCount].VtxMinObject.z - PULS3 && pPos->z < g_Object3[nCount].pos.z + g_Object3[nCount].VtxMaxObject.z + PULS3)
		{	//Zの範囲
			if (pPosold->x >= g_Object3[nCount].pos.x + g_Object3[nCount].VtxMaxObject.x + PULS3 && pPos->x < g_Object3[nCount].pos.x + g_Object3[nCount].VtxMaxObject.x + PULS3)
			{
				pPos->x = g_Object3[nCount].pos.x + g_Object3[nCount].VtxMaxObject.x + PULS3;
				pPos->x = pPosold->x;
			}
			else if (pPosold->x <= g_Object3[nCount].pos.x + g_Object3[nCount].VtxMinObject.x - PULS3 && pPos->x > g_Object3[nCount].pos.x + g_Object3[nCount].VtxMinObject.x - PULS3)
			{
				pPos->x = g_Object3[nCount].pos.x + g_Object3[nCount].VtxMinObject.x - PULS3;
				pPos->x = pPosold->x;
			}
		}
	}
	return bLand;
}
//=========================================================================================================================
// オブジェクトの取得処理
//=========================================================================================================================
Object3 *GetObject3(void)
{
	return &g_Object3[0];
}
//
//
//
void DeleteObject3(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT3; nCntObject++)
	{
		//g_Object3[nCntObject].VtxMinObject = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		//g_Object3[nCntObject].VtxMaxObject = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_Object3[nCntObject].pos = D3DXVECTOR3(10000, 0, 0);
		g_Object3[nCntObject].rot = D3DXVECTOR3(0, 0, 0);
		g_Object3[nCntObject].bUse = false;
	}
}