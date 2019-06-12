//=============================================================================
//
// モデル処理 [Object.cpp]
// Author : 有馬 武志
//
//=============================================================================
#include "object.h"		//オブジェクト
#include "input.h"		//キーボード
#include "camera.h"		//カメラ
#include "shadow.h"		//影
#include "player.h"		//プレイヤー
#include "petals.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OBJECT_NAME			"data\\MODEL\\木(下)最強.x"		//読み込むテクスチャファイル
#define OBJECT_NAME1		"data\\MODEL\\岩.x"				//読み込むテクスチャファイル
#define MAX_TEX				(1)								//オブジェクトのテクスチャ最大数
#define MAX_OBJECT			(72)							//オブジェクトの最大数
#define MAX_VARIATION		(1)								//オブジェクトの種類
#define PLUS				(20)							//あたり判定

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Object g_Object[MAX_OBJECT];

//=============================================================================
// 初期化処理
//=============================================================================
void InitObject(void)
{
	//デバイスポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX
	(
		OBJECT_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Object[0].pBuffMatObject,
		NULL, &g_Object[0].nNumMatObject,
		&g_Object[0].pMeshObject
	);
	// Xファイルの読み込み
	D3DXLoadMeshFromX
	(
		OBJECT_NAME1,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Object[1].pBuffMatObject,
		NULL, &g_Object[1].nNumMatObject,
		&g_Object[1].pMeshObject
	);

	D3DXMATERIAL *pMat;
	D3DMATERIAL9 *pMatDef;

	//テクスチャをつける
	for (int nCount = 0; nCount < MAX_VARIATION; nCount++)
	{
		pMat = (D3DXMATERIAL*)g_Object[nCount].pBuffMatObject->GetBufferPointer();
		pMatDef = new D3DMATERIAL9[g_Object[nCount].nNumMatObject];
		g_Object[nCount].pMeshTextures = new LPDIRECT3DTEXTURE9[g_Object[nCount].nNumMatObject];

		for (DWORD tex = 0; tex < g_Object[nCount].nNumMatObject; tex++)
		{
			pMatDef[tex] = pMat[tex].MatD3D;
			pMatDef[tex].Ambient = pMatDef[tex].Diffuse;
			g_Object[nCount].pMeshTextures[tex] = NULL;
			if (pMat[tex].pTextureFilename != NULL &&
				lstrlen(pMat[tex].pTextureFilename) > 0)
			{
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pMat[tex].pTextureFilename,
					&g_Object[nCount].pMeshTextures[tex])))
				{
					//MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
				}
			}
		}
	}

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		g_Object[nCntObject].VtxMinObject = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		g_Object[nCntObject].VtxMaxObject = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_Object[nCntObject].pos = D3DXVECTOR3(0, 0, 0);
		g_Object[nCntObject].rot = D3DXVECTOR3(0, 0, 0);
		g_Object[nCntObject].bUse = false;
	}

	int nNumVtx;			//頂点数
	DWORD sizeFVF;			//頂点フォーマット7のサイズ
	BYTE *pVtxBuff;			//頂点バッファへのポインタ

	for (int nCount = 0; nCount < MAX_VARIATION; nCount++)
	{
		//頂点数を取得
		nNumVtx = g_Object[nCount].pMeshObject->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_Object[nCount].pMeshObject->GetFVF());

		//頂点バッファをロック
		g_Object[nCount].pMeshObject->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標の代入

			//範囲指定
			//最大値
			if (vtx.x < g_Object[nCount].VtxMinObject.x)
			{
				g_Object[nCount].VtxMinObject.x = vtx.x;
			}
			if (vtx.z < g_Object[nCount].VtxMinObject.z)
			{
				g_Object[nCount].VtxMinObject.z = vtx.z;
			}
			if (vtx.y < g_Object[nCount].VtxMinObject.y)
			{
				g_Object[nCount].VtxMinObject.y = vtx.y;
			}
			//最小値
			if (vtx.x > g_Object[nCount].VtxMaxObject.x)
			{
				g_Object[nCount].VtxMaxObject.x = vtx.x;
			}
			if (vtx.z > g_Object[nCount].VtxMaxObject.z)
			{
				g_Object[nCount].VtxMaxObject.z = vtx.z;
			}
			if (vtx.y > g_Object[nCount].VtxMaxObject.y)
			{
				g_Object[nCount].VtxMaxObject.y = vtx.y;
			}
			pVtxBuff += sizeFVF;		//サイズ分ポインタを進める
		}
		//頂点バッファをアンロック
		g_Object[nCount].pMeshObject->UnlockVertexBuffer();
	}

	//オブジェクトの代入
	//for(カウント　= 全体のオブジェクトの数を最初に+する)
	for (int nPotaCountObject = MAX_VARIATION; nPotaCountObject < MAX_OBJECT; nPotaCountObject++)
	{
		if (nPotaCountObject < MAX_OBJECT + MAX_VARIATION)
		{//木（下）
			g_Object[nPotaCountObject] = g_Object[0];
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitObject(void)
{
	for (int nCntObject = 0; nCntObject < MAX_VARIATION; nCntObject++)
	{
		// メッシュの開放
		if (g_Object[nCntObject].pMeshObject != NULL)
		{
			g_Object[nCntObject].pMeshObject->Release();
			g_Object[nCntObject].pMeshObject = NULL;
		}
		// マテリアルの開放
		if (g_Object[nCntObject].pBuffMatObject != NULL)
		{
			g_Object[nCntObject].pBuffMatObject->Release();
			g_Object[nCntObject].pBuffMatObject = NULL;
		}

		//テクスチャ
		for (DWORD nTex = 0; nTex < g_Object[nCntObject].nNumMatObject; nTex++)
		{
			if (g_Object[nCntObject].pMeshTextures[nTex] != NULL)
			{
				g_Object[nCntObject].pMeshTextures[nTex]->Release();
				g_Object[nCntObject].pMeshTextures[nTex] = NULL;
			}
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateObject(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (CollisionAttack(g_Object[nCntObject].pos, g_Object[nCntObject].VtxMaxObject.x))
		{// 攻撃がヒット
			if (g_Object[nCntObject].bUse == true)
			{// 使われている
				//PlaySound(SOUND_LABEL_SWORDHIT);
				SetPetals(g_Object[nCntObject].pos, 3);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	for (int nCount2 = 0; nCount2 < MAX_OBJECT; nCount2++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Object[nCount2].mtxWorldObject);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Object[nCount2].rot.y, g_Object[nCount2].rot.x, g_Object[nCount2].rot.z);

		D3DXMatrixMultiply(&g_Object[nCount2].mtxWorldObject, &g_Object[nCount2].mtxWorldObject, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTrans, g_Object[nCount2].pos.x, g_Object[nCount2].pos.y, g_Object[nCount2].pos.z);

		D3DXMatrixMultiply(&g_Object[nCount2].mtxWorldObject, &g_Object[nCount2].mtxWorldObject, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Object[nCount2].mtxWorldObject);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_Object[nCount2].pBuffMatObject->GetBufferPointer();

		for (int nCntMat = 0; nCntMat<(int)g_Object[nCount2].nNumMatObject; nCntMat++)
		{
			if (g_Object[nCount2].bUse == true)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				// テクスチャの設定
				pDevice->SetTexture(0, g_Object[nCount2].pMeshTextures[nCntMat]);
				// モデル(パーツ)の描画
				g_Object[nCount2].pMeshObject->DrawSubset(nCntMat);
			}
		}
		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
}
//=============================================================================
// セットモデル
//=============================================================================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntObject;

	for (nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_Object[nCntObject].bUse == false)
		{
			g_Object[nCntObject].pos = pos;
			g_Object[nCntObject].rot = rot;
			g_Object[nCntObject].bUse = true;
			break;
		}
	}
}
//=============================================================================
// モデルの当たり判定
//=============================================================================
bool CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove)
{
	int nCount;
	bool bLand = false;
	Player * pPlayer = GetPlayer();

	for (nCount = 0; nCount < MAX_OBJECT; nCount++)
	{
		if (true == g_Object[nCount].bUse)
		{
			if (pPos->x > g_Object[nCount].pos.x + g_Object[nCount].VtxMinObject.x - PLUS && pPos->x < g_Object[nCount].pos.x + g_Object[nCount].VtxMaxObject.x + PLUS)
			{	//Xの範囲
				if (pPosold->z >= g_Object[nCount].pos.z + g_Object[nCount].VtxMaxObject.z + PLUS && pPos->z < g_Object[nCount].pos.z + g_Object[nCount].VtxMaxObject.z + PLUS)
				{
					pPos->z = g_Object[nCount].pos.z + g_Object[nCount].VtxMaxObject.z + PLUS;
					pPos->z = pPosold->z;
				}
				else if (pPosold->z <= g_Object[nCount].pos.z + g_Object[nCount].VtxMinObject.z - PLUS && pPos->z > g_Object[nCount].pos.z + g_Object[nCount].VtxMinObject.z - PLUS)
				{
					pPos->z = g_Object[nCount].pos.z + g_Object[nCount].VtxMinObject.z - PLUS;
					pPos->z = pPosold->z;
				}
			}
			if (pPos->z > g_Object[nCount].pos.z + g_Object[nCount].VtxMinObject.z - PLUS && pPos->z < g_Object[nCount].pos.z + g_Object[nCount].VtxMaxObject.z + PLUS)
			{	//Zの範囲
				if (pPosold->x >= g_Object[nCount].pos.x + g_Object[nCount].VtxMaxObject.x + PLUS && pPos->x < g_Object[nCount].pos.x + g_Object[nCount].VtxMaxObject.x + PLUS)
				{
					pPos->x = g_Object[nCount].pos.x + g_Object[nCount].VtxMaxObject.x + PLUS;
					pPos->x = pPosold->x;
				}
				else if (pPosold->x <= g_Object[nCount].pos.x + g_Object[nCount].VtxMinObject.x - PLUS && pPos->x > g_Object[nCount].pos.x + g_Object[nCount].VtxMinObject.x - PLUS)
				{
					pPos->x = g_Object[nCount].pos.x + g_Object[nCount].VtxMinObject.x - PLUS;
					pPos->x = pPosold->x;
				}
			}
		}
	}
	return bLand;
}
//=========================================================================================================================
// オブジェクトの取得処理
//=========================================================================================================================
Object *GetObject(void)
{
	return &g_Object[0];
}
//
//
//
void DeleteObject(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		//g_Object[nCntObject].VtxMinObject = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		//g_Object[nCntObject].VtxMaxObject = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_Object[nCntObject].pos = D3DXVECTOR3(0, 0, 0);
		g_Object[nCntObject].rot = D3DXVECTOR3(0, 0, 0);
		g_Object[nCntObject].bUse = false;
	}
}