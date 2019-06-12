//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "shadow.h"
#include "explosion.h"
#include "particle.h"
#include "bullet.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMYNAME		"data/MODEL/airplane000.x"		// 読み込むxファイル名
#define MAX_ENEMY		(64)
#define DAMAGE_TIME		(60)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH g_pMeshEnemy = NULL;				// メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatEnemy = NULL;		// マテリアル情報へのポインタ
DWORD g_nNumMatEnemy = 0;					// マテリアル情報の数
Enemy g_aEnemy[MAX_ENEMY];					// 敵の情報
int g_nNumEnemy;							// 敵の総数
D3DXVECTOR3 g_vtxMinEnemy, g_vtxMaxEnemy;	// 頂点の最小値、最大値

//=============================================================================
// 初期化処理
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEnemy;

	// Xファイルの読み込み
	D3DXLoadMeshFromX(ENEMYNAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatEnemy,
		NULL,
		&g_nNumMatEnemy,
		&g_pMeshEnemy);

	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = g_pMeshEnemy->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshEnemy->GetFVF());

	// 頂点バッファをロック
	g_pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{// 頂点座標の比較
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

		// Xの値の比較
		if (g_vtxMinEnemy.x > vtx.x)
		{// 最小値と比較
			g_vtxMinEnemy.x = vtx.x;	// 最小値より小さければ代入
		}

		if (g_vtxMaxEnemy.x < vtx.x)
		{// 最大値と比較
			g_vtxMaxEnemy.x = vtx.x;	// 最大値より大きければ代入
		}

		// Yの値の比較
		if (g_vtxMinEnemy.y > vtx.y)
		{// 最小値と比較
			g_vtxMinEnemy.y = vtx.y;	// 最小値より小さければ代入
		}

		if (g_vtxMaxEnemy.y < vtx.y)
		{// 最大値と比較
			g_vtxMaxEnemy.y = vtx.y;	// 最大値より大きければ代入
		}

		// Zの値の比較
		if (g_vtxMinEnemy.z > vtx.z)
		{// 最小値と比較
			g_vtxMinEnemy.z = vtx.z;	// 最小値より小さければ代入
		}

		if (g_vtxMaxEnemy.z < vtx.z)
		{// 最大値と比較
			g_vtxMaxEnemy.z = vtx.z;	// 最大値より大きければ代入
		}

		pVtxBuff += sizeFVF;	// サイズ分ポインタを進める
	}

	// 頂点バッファをアンロック
	g_pMeshEnemy->UnlockVertexBuffer();

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵の初期設定
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].fDestAngle = 0.0f;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nLife = 0;
		g_aEnemy[nCntEnemy].bUse = false;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	// メッシュの開放
	if (g_pMeshEnemy != NULL)
	{
		g_pMeshEnemy->Release();
		g_pMeshEnemy = NULL;
	}

	// マテリアルの開放
	if (g_pBuffMatEnemy != NULL)
	{
		g_pBuffMatEnemy->Release();
		g_pBuffMatEnemy = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:
				break;

			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;
				if (g_aEnemy[nCntEnemy].nCounterState >= DAMAGE_TIME)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
				}
				break;
			}

			if (CollisionBullet(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, g_vtxMinEnemy, g_vtxMaxEnemy) == true)
			{
				HitEnemy(nCntEnemy, 1);
			}
		}
	}


	//// 徐々に方向を変える
	//g_fAngle = g_aEnemy.fDestAngle - g_aEnemy.rot.y;

	//if (g_fAngle < -D3DX_PI)
	//{
	//	g_fAngle += D3DX_PI * 2;
	//}
	//else if (g_fAngle > D3DX_PI)
	//{
	//	g_fAngle -= D3DX_PI * 2;
	//}

	//g_aEnemy.rot.y += g_fAngle * 0.1f;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			// 高さによる影の頂点座標
			SetVtxShadow(g_aEnemy[nCntEnemy].nIdxShadow, g_aEnemy[nCntEnemy].pos,0.7f);

			// 影の位置
			SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow, g_aEnemy[nCntEnemy].pos);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatEnemy; nCntMat++)
			{

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, NULL);

				// 敵(パーツ)の描画
				g_pMeshEnemy->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// 敵の設定処理
//=============================================================================
void SetEnenmy(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].rot = rot;
			g_aEnemy[nCntEnemy].nLife = 10;

			// 影をつける
			g_aEnemy[nCntEnemy].nIdxShadow = SetShadow(g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot, 15.0f, 15.0f);
			g_aEnemy[nCntEnemy].bUse = true;
			break;
		}
	}
	g_nNumEnemy++;
}

//=============================================================================
// 敵のダメージ処理
//=============================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	g_aEnemy[nCntEnemy].nLife -= nDamage;
	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		//SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(5.0f, 0.0f, 0.0f, 10.0f), (rand() % 3) * 1.5f, 60, PARTICLETYPE_BLOOD);
		g_aEnemy[nCntEnemy].bUse = false;
		g_nNumEnemy--;
		DeleteShadow(g_aEnemy[nCntEnemy].nIdxShadow);
		if (g_nNumEnemy <= 0)
		{//敵が全滅したら
			SetGameState(GAMESTATE_CLEAR);
		}
	}
	else
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
	}
}

//=============================================================================
// 敵の情報の取得
//=============================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}