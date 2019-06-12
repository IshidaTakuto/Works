//=============================================================================
//
// モデル処理 [Object2.cpp]
// Author : 有馬 武志
//
//=============================================================================
#include "object2.h"	//オブジェクト
#include "input.h"		//キーボード
#include "camera.h"		//カメラ
#include "shadow.h"		//影
#include "player.h"		//プレイヤー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OBJECT2_NAME		"data\\MODEL\\岩3.x"			//読み込むテクスチャファイル
#define MAX_TEX2			(1)								//オブジェクトのテクスチャ最大数
#define MAX_OBJECT2			(83)								//オブジェクトの最大数
#define MAX_VARIATION2		(1)								//オブジェクトの種類
#define PULS2				(35)							//あたり判定
#define PULS2_X				(35)							//あたり判定		

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Object2 g_Object2[MAX_OBJECT2];

//=============================================================================
// 初期化処理
//=============================================================================
void InitObject2(void)
{
	//デバイスポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// Xファイルの読み込み	
	D3DXLoadMeshFromX
	(
		OBJECT2_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Object2[0].pBuffMatObject,
		NULL, &g_Object2[0].nNumMatObject,
		&g_Object2[0].pMeshObject
	);
	
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 *pMatDef;

	//テクスチャをつける
	for (int nCount = 0; nCount < MAX_VARIATION2; nCount++)
	{
		pMat = (D3DXMATERIAL*)g_Object2[nCount].pBuffMatObject->GetBufferPointer();
		pMatDef = new D3DMATERIAL9[g_Object2[nCount].nNumMatObject];
		g_Object2[nCount].pMeshTextures = new LPDIRECT3DTEXTURE9[g_Object2[nCount].nNumMatObject];

		for (DWORD tex = 0; tex < g_Object2[nCount].nNumMatObject; tex++)
		{
			pMatDef[tex] = pMat[tex].MatD3D;
			pMatDef[tex].Ambient = pMatDef[tex].Diffuse;
			g_Object2[nCount].pMeshTextures[tex] = NULL;
			if (pMat[tex].pTextureFilename != NULL &&
				lstrlen(pMat[tex].pTextureFilename) > 0)
			{
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pMat[tex].pTextureFilename,
					&g_Object2[nCount].pMeshTextures[tex])))
				{
					//MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
				}
			}
		}
	}

	for (int nCntObject = 0; nCntObject < MAX_OBJECT2; nCntObject++)
	{
		g_Object2[nCntObject].VtxMinObject = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		g_Object2[nCntObject].VtxMaxObject = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_Object2[nCntObject].pos = D3DXVECTOR3(10000, 0, 0);
		g_Object2[nCntObject].rot = D3DXVECTOR3(0, 0, 0);
		g_Object2[nCntObject].bUse = false;
	}

	int nNumVtx;			//頂点数
	DWORD sizeFVF;			//頂点フォーマット7のサイズ
	BYTE *pVtxBuff;			//頂点バッファへのポインタ

	for (int nCount = 0; nCount < MAX_VARIATION2; nCount++)
	{
		//頂点数を取得
		nNumVtx = g_Object2[nCount].pMeshObject->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_Object2[nCount].pMeshObject->GetFVF());

		//頂点バッファをロック
		g_Object2[nCount].pMeshObject->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標の代入

													   //範囲指定
													   //最大値
			if (vtx.x < g_Object2[nCount].VtxMinObject.x)
			{
				g_Object2[nCount].VtxMinObject.x = vtx.x;
			}
			if (vtx.z < g_Object2[nCount].VtxMinObject.z)
			{
				g_Object2[nCount].VtxMinObject.z = vtx.z;
			}
			if (vtx.y < g_Object2[nCount].VtxMinObject.y)
			{
				g_Object2[nCount].VtxMinObject.y = vtx.y;
			}
			//最小値
			if (vtx.x > g_Object2[nCount].VtxMaxObject.x)
			{
				g_Object2[nCount].VtxMaxObject.x = vtx.x;
			}
			if (vtx.z > g_Object2[nCount].VtxMaxObject.z)
			{
				g_Object2[nCount].VtxMaxObject.z = vtx.z;
			}
			if (vtx.y > g_Object2[nCount].VtxMaxObject.y)
			{
				g_Object2[nCount].VtxMaxObject.y = vtx.y;
			}
			pVtxBuff += sizeFVF;		//サイズ分ポインタを進める
		}
		//頂点バッファをアンロック
		g_Object2[nCount].pMeshObject->UnlockVertexBuffer();
	}

	//オブジェクトの代入
	//for(カウント　= 全体のオブジェクトの数を最初に+する)
	for (int nPotaCountObject = MAX_VARIATION2; nPotaCountObject < MAX_OBJECT2; nPotaCountObject++)
	{
		if (nPotaCountObject < MAX_OBJECT2 + MAX_VARIATION2)
		{//周りのでかい岩
			g_Object2[nPotaCountObject] = g_Object2[0];
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitObject2(void)
{
	for (int nCntObject = 0; nCntObject < MAX_VARIATION2; nCntObject++)
	{
		// メッシュの開放
		if (g_Object2[nCntObject].pMeshObject != NULL)
		{
			g_Object2[nCntObject].pMeshObject->Release();
			g_Object2[nCntObject].pMeshObject = NULL;
		}
		// マテリアルの開放
		if (g_Object2[nCntObject].pBuffMatObject != NULL)
		{
			g_Object2[nCntObject].pBuffMatObject->Release();
			g_Object2[nCntObject].pBuffMatObject = NULL;
		}

		//テクスチャ
		for (DWORD nTex = 0; nTex < g_Object2[nCntObject].nNumMatObject; nTex++)
		{
			if (g_Object2[nCntObject].pMeshTextures[nTex] != NULL)
			{
				g_Object2[nCntObject].pMeshTextures[nTex]->Release();
				g_Object2[nCntObject].pMeshTextures[nTex] = NULL;
			}
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateObject2(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawObject2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	for (int nCount2 = 0; nCount2 < MAX_OBJECT2; nCount2++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Object2[nCount2].mtxWorldObject);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Object2[nCount2].rot.y, g_Object2[nCount2].rot.x, g_Object2[nCount2].rot.z);

		D3DXMatrixMultiply(&g_Object2[nCount2].mtxWorldObject, &g_Object2[nCount2].mtxWorldObject, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTrans, g_Object2[nCount2].pos.x, g_Object2[nCount2].pos.y, g_Object2[nCount2].pos.z);

		D3DXMatrixMultiply(&g_Object2[nCount2].mtxWorldObject, &g_Object2[nCount2].mtxWorldObject, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Object2[nCount2].mtxWorldObject);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_Object2[nCount2].pBuffMatObject->GetBufferPointer();

		for (int nCntMat = 0; nCntMat<(int)g_Object2[nCount2].nNumMatObject; nCntMat++)
		{
			if (g_Object2[nCount2].bUse == true)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				// テクスチャの設定
				pDevice->SetTexture(0, g_Object2[nCount2].pMeshTextures[nCntMat]);
				// モデル(パーツ)の描画
				g_Object2[nCount2].pMeshObject->DrawSubset(nCntMat);
			}
		}
		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
}
//=============================================================================
// セットモデル
//=============================================================================
void SetObject2(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntObject;

	for (nCntObject = 0; nCntObject < MAX_OBJECT2; nCntObject++)
	{
		if (g_Object2[nCntObject].bUse == false)
		{
			g_Object2[nCntObject].pos = pos;
			g_Object2[nCntObject].rot = rot;
			g_Object2[nCntObject].bUse = true;
			break;
		}
	}
}
//=============================================================================
// モデルの当たり判定
//=============================================================================
bool CollisionObject2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove)
{
	int nCount;
	bool bLand = false;
	Player * pPlayer = GetPlayer();

	for (nCount = 0; nCount < MAX_OBJECT2; nCount++)
	{
		if (pPos->x > g_Object2[nCount].pos.x + g_Object2[nCount].VtxMinObject.x - PULS2_X && pPos->x < g_Object2[nCount].pos.x + g_Object2[nCount].VtxMaxObject.x + PULS2_X)
		{	//Xの範囲
			if (pPosold->z >= g_Object2[nCount].pos.z + g_Object2[nCount].VtxMaxObject.z + PULS2_X && pPos->z < g_Object2[nCount].pos.z + g_Object2[nCount].VtxMaxObject.z + PULS2_X)
			{
				pPos->z = g_Object2[nCount].pos.z + g_Object2[nCount].VtxMaxObject.z + PULS2_X;
				pPos->z = pPosold->z;
			}
			else if (pPosold->z <= g_Object2[nCount].pos.z + g_Object2[nCount].VtxMinObject.z - PULS2_X && pPos->z > g_Object2[nCount].pos.z + g_Object2[nCount].VtxMinObject.z - PULS2_X)
			{
				pPos->z = g_Object2[nCount].pos.z + g_Object2[nCount].VtxMinObject.z - PULS2_X;
				pPos->z = pPosold->z;
			}
		}
		if (pPos->z > g_Object2[nCount].pos.z + g_Object2[nCount].VtxMinObject.z - PULS2 && pPos->z < g_Object2[nCount].pos.z + g_Object2[nCount].VtxMaxObject.z + PULS2)
		{	//Zの範囲
			if (pPosold->x >= g_Object2[nCount].pos.x + g_Object2[nCount].VtxMaxObject.x + PULS2 && pPos->x < g_Object2[nCount].pos.x + g_Object2[nCount].VtxMaxObject.x + PULS2)
			{
				pPos->x = g_Object2[nCount].pos.x + g_Object2[nCount].VtxMaxObject.x + PULS2;
				pPos->x = pPosold->x;
			}
			else if (pPosold->x <= g_Object2[nCount].pos.x + g_Object2[nCount].VtxMinObject.x - PULS2 && pPos->x > g_Object2[nCount].pos.x + g_Object2[nCount].VtxMinObject.x - PULS2)
			{
				pPos->x = g_Object2[nCount].pos.x + g_Object2[nCount].VtxMinObject.x - PULS2;
				pPos->x = pPosold->x;
			}
		}
	}
	return bLand;
}
//=========================================================================================================================
// オブジェクトの取得処理
//=========================================================================================================================
Object2 *GetObject2(void)
{
	return &g_Object2[0];
}
//
//
//
void DeleteObject2(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT2; nCntObject++)
	{
		//g_Object2[nCntObject].VtxMinObject = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		//g_Object2[nCntObject].VtxMaxObject = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_Object2[nCntObject].pos = D3DXVECTOR3(10000, 0, 0);
		g_Object2[nCntObject].rot = D3DXVECTOR3(0, 0, 0);
		g_Object2[nCntObject].bUse = false;
	}
}