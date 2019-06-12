//=============================================================================
//
// モデル処理 [ObjectNot.cpp]
// Author : 有馬 武志
//
//=============================================================================
#include "objectnot.h"		//オブジェクト
#include "input.h"			//キーボード
#include "camera.h"			//カメラ
#include "shadow.h"			//影
#include "player.h"			//プレイヤー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OBJECT_NAME			"data\\MODEL\\木(上)最強.x"	//読み込むテクスチャファイル
#define MAX_TEX				(1)							//オブジェクトのテクスチャ最大数
#define MAX_OBJECT			(92)						//オブジェクトの最大数
#define MAX_VARIATION		(1)							//オブジェクトの種類

//*****************************************************************************
// グローバル変数
//*****************************************************************************
ObjectNot g_ObjectNot[MAX_OBJECT];
int g_nCntLeg = 0;

//=============================================================================
// 初期化処理
//=============================================================================
void InitObjectNot(void)
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
		&g_ObjectNot[0].pBuffMatObjectNot,
		NULL, &g_ObjectNot[0].nNumMatObjectNot,
		&g_ObjectNot[0].pMeshObjectNot
	);
	
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 *pMatDef;

	//テクスチャをつける
	for (int nCount = 0; nCount < MAX_VARIATION; nCount++)
	{
		pMat = (D3DXMATERIAL*)g_ObjectNot[nCount].pBuffMatObjectNot->GetBufferPointer();
		pMatDef = new D3DMATERIAL9[g_ObjectNot[nCount].nNumMatObjectNot];
		g_ObjectNot[nCount].pMeshTextures = new LPDIRECT3DTEXTURE9[g_ObjectNot[nCount].nNumMatObjectNot];

		for (DWORD tex = 0; tex < g_ObjectNot[nCount].nNumMatObjectNot; tex++)
		{
			pMatDef[tex] = pMat[tex].MatD3D;
			pMatDef[tex].Ambient = pMatDef[tex].Diffuse;
			g_ObjectNot[nCount].pMeshTextures[tex] = NULL;
			if (pMat[tex].pTextureFilename != NULL &&
				lstrlen(pMat[tex].pTextureFilename) > 0)
			{
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pMat[tex].pTextureFilename,
					&g_ObjectNot[nCount].pMeshTextures[tex])))
				{
					//MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
				}
			}
		}
	}

	for (int nCntObjectNot = 0; nCntObjectNot < MAX_OBJECT; nCntObjectNot++)
	{
		g_ObjectNot[nCntObjectNot].VtxMinObjectNot = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		g_ObjectNot[nCntObjectNot].VtxMaxObjectNot = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_ObjectNot[nCntObjectNot].pos = D3DXVECTOR3(0, 0, 0);
		g_ObjectNot[nCntObjectNot].rot = D3DXVECTOR3(0, 0, 0);
		g_ObjectNot[nCntObjectNot].bUse = false;
	}

	int nNumVtx;			//頂点数
	DWORD sizeFVF;			//頂点フォーマット7のサイズ
	BYTE *pVtxBuff;			//頂点バッファへのポインタ

	for (int nCount = 0; nCount < MAX_VARIATION; nCount++)
	{
		//頂点数を取得
		nNumVtx = g_ObjectNot[nCount].pMeshObjectNot->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_ObjectNot[nCount].pMeshObjectNot->GetFVF());

		//頂点バッファをロック
		g_ObjectNot[nCount].pMeshObjectNot->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標の代入

			//範囲指定
			//最大値
			if (vtx.x < g_ObjectNot[nCount].VtxMinObjectNot.x)
			{
				g_ObjectNot[nCount].VtxMinObjectNot.x = vtx.x;
			}
			if (vtx.z < g_ObjectNot[nCount].VtxMinObjectNot.z)
			{
				g_ObjectNot[nCount].VtxMinObjectNot.z = vtx.z;
			}
			if (vtx.y < g_ObjectNot[nCount].VtxMinObjectNot.y)
			{
				g_ObjectNot[nCount].VtxMinObjectNot.y = vtx.y;
			}
			//最小値
			if (vtx.x > g_ObjectNot[nCount].VtxMaxObjectNot.x)
			{
				g_ObjectNot[nCount].VtxMaxObjectNot.x = vtx.x;
			}
			if (vtx.z > g_ObjectNot[nCount].VtxMaxObjectNot.z)
			{
				g_ObjectNot[nCount].VtxMaxObjectNot.z = vtx.z;
			}
			if (vtx.y > g_ObjectNot[nCount].VtxMaxObjectNot.y)
			{
				g_ObjectNot[nCount].VtxMaxObjectNot.y = vtx.y;
			}

			pVtxBuff += sizeFVF;		//サイズ分ポインタを進める
		}
		//頂点バッファをアンロック
		g_ObjectNot[nCount].pMeshObjectNot->UnlockVertexBuffer();
	}

	//オブジェクトの代入
	for (int nPotaCountObjectNot = 1; nPotaCountObjectNot < MAX_OBJECT; nPotaCountObjectNot++)
	{
		if (nPotaCountObjectNot < MAX_OBJECT + MAX_VARIATION)
		{
			g_ObjectNot[nPotaCountObjectNot] = g_ObjectNot[0];
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitObjectNot(void)
{
	for (int nCntObjectNot = 0; nCntObjectNot < MAX_VARIATION; nCntObjectNot++)
	{
		// メッシュの開放
		if (g_ObjectNot[nCntObjectNot].pMeshObjectNot != NULL)
		{
			g_ObjectNot[nCntObjectNot].pMeshObjectNot->Release();
			g_ObjectNot[nCntObjectNot].pMeshObjectNot = NULL;
		}
		// マテリアルの開放
		if (g_ObjectNot[nCntObjectNot].pBuffMatObjectNot != NULL)
		{
			g_ObjectNot[nCntObjectNot].pBuffMatObjectNot->Release();
			g_ObjectNot[nCntObjectNot].pBuffMatObjectNot = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateObjectNot(void)
{
	g_nCntLeg++;

	if (g_nCntLeg == 119)
	{//カウントを０に戻す
		g_nCntLeg = 0;
	}
	else if (g_nCntLeg >= 60)
	{//下と左に揺れる
		g_ObjectNot->pos.x += 0.02f;
		g_ObjectNot->pos.y += 0.01f;
	}
	else if (g_nCntLeg <= 59)
	{//下と左に揺れる
		g_ObjectNot->pos.x -= 0.02f;
		g_ObjectNot->pos.y -= 0.01f;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawObjectNot(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	for (int nCount2 = 0; nCount2 < MAX_OBJECT; nCount2++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_ObjectNot[nCount2].mtxWorldObjectNot);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ObjectNot[nCount2].rot.y, g_ObjectNot[nCount2].rot.x, g_ObjectNot[nCount2].rot.z);

		D3DXMatrixMultiply(&g_ObjectNot[nCount2].mtxWorldObjectNot, &g_ObjectNot[nCount2].mtxWorldObjectNot, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTrans, g_ObjectNot[nCount2].pos.x, g_ObjectNot[nCount2].pos.y, g_ObjectNot[nCount2].pos.z);

		D3DXMatrixMultiply(&g_ObjectNot[nCount2].mtxWorldObjectNot, &g_ObjectNot[nCount2].mtxWorldObjectNot, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_ObjectNot[nCount2].mtxWorldObjectNot);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_ObjectNot[nCount2].pBuffMatObjectNot->GetBufferPointer();

		for (int nCntMat = 0; nCntMat<(int)g_ObjectNot[nCount2].nNumMatObjectNot; nCntMat++)
		{
			if (g_ObjectNot[nCount2].bUse == true)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				// テクスチャの設定
				pDevice->SetTexture(0, g_ObjectNot[nCount2].pMeshTextures[nCntMat]);
				// モデル(パーツ)の描画
				g_ObjectNot[nCount2].pMeshObjectNot->DrawSubset(nCntMat);
			}
		}
		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
}
//=============================================================================
// セットモデル
//=============================================================================
void SetObjectNot(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntObjectNot;

	for (nCntObjectNot = 0; nCntObjectNot < MAX_OBJECT; nCntObjectNot++)
	{
		if (g_ObjectNot[nCntObjectNot].bUse == false)
		{
			g_ObjectNot[nCntObjectNot].pos = pos;
			g_ObjectNot[nCntObjectNot].rot = rot;
			g_ObjectNot[nCntObjectNot].bUse = true;
			break;
		}
	}
}
//=============================================================================
// モデルの当たり判定
//=============================================================================

//=========================================================================================================================
// オブジェクトの取得処理
//=========================================================================================================================
ObjectNot *GetObjectNot(void)
{
	return &g_ObjectNot[0];
}
//
//
//
void DeleteNotObject(void)
{
	for (int nCntObjectNot = 0; nCntObjectNot < MAX_OBJECT; nCntObjectNot++)
	{
		g_ObjectNot[nCntObjectNot].VtxMinObjectNot = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		g_ObjectNot[nCntObjectNot].VtxMaxObjectNot = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_ObjectNot[nCntObjectNot].pos = D3DXVECTOR3(0, 0, 0);
		g_ObjectNot[nCntObjectNot].rot = D3DXVECTOR3(0, 0, 0);
		g_ObjectNot[nCntObjectNot].bUse = false;
	}
}