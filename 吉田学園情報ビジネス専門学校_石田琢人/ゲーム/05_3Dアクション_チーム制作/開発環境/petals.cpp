//=============================================================================
//
// 花びら処理 [petals.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "petals.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PETALS_TEXTURENAME00	"data/TEXTURE/petal000.png"
#define PETALS_TEXTURENAME01	"data/TEXTURE/petal01.jpg"
#define MAX_PETALS			(256)		// 花びらの最大数
#define PETALS_FIELD		(2500 / 2)	// 中心を0とした範囲
#define PETALS_APPEAR		(8)			// 出現する間隔
#define MAX_TEXTURE			(1)			// テクスチャの数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePetals[MAX_TEXTURE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPetals = NULL;	// 頂点バッファへのポインタ
PETALS g_aPetals[MAX_PETALS];
int g_nCntPetals;	// 花びらを出す間隔のカウンタ
float g_fAnglePetals;
int g_nCntAnglePetals;

//=============================================================================
// ポリゴン初期化処理
//=============================================================================
void InitPetals(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// ジェネレータの情報を初期化
	for (int nCntPetals = 0; nCntPetals < MAX_PETALS; nCntPetals++)
	{
		g_aPetals[nCntPetals].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPetals[nCntPetals].fRadius = 0;
		g_aPetals[nCntPetals].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aPetals[nCntPetals].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPetals[nCntPetals].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPetals[nCntPetals].angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPetals[nCntPetals].bUse = false;
	}

	g_nCntPetals = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PETALS_TEXTURENAME00, &g_pTexturePetals[0]);
	D3DXCreateTextureFromFile(pDevice, PETALS_TEXTURENAME01, &g_pTexturePetals[1]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PETALS,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPetals,
		NULL);

	VERTEX_3D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPetals->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	for (int nCntPetals = 0; nCntPetals < MAX_PETALS; nCntPetals++)
	{
		pVtx[0].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[2].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[3].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

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

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffPetals->Unlock();
}

//=============================================================================
// ポリゴン終了処理
//=============================================================================
void UninitPetals(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		// テクスチャの破棄
		if (g_pTexturePetals[nCntTex] != NULL)
		{
			g_pTexturePetals[nCntTex]->Release();
			g_pTexturePetals[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffPetals != NULL)
	{
		g_pVtxBuffPetals->Release();
		g_pVtxBuffPetals = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePetals(void)
{
	int nCntPetals;

	VERTEX_3D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPetals->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPetals = 0; nCntPetals < MAX_PETALS; nCntPetals++)
	{
		if (g_aPetals[nCntPetals].bUse == true)
		{// 使われている
			if (g_aPetals[nCntPetals].pos.y <= 0)
			{// 地面についたら消す
				g_aPetals[nCntPetals].bUse = false;
			}

			// 回転させる
			g_aPetals[nCntPetals].rot += g_aPetals[nCntPetals].angle;

			g_aPetals[nCntPetals].move.x = g_aPetals[nCntPetals].move.x * 0.8f;
			//g_aPetals[nCntPetals].move.y = g_aPetals[nCntPetals].move.y * 0.8f;
			g_aPetals[nCntPetals].move.z = g_aPetals[nCntPetals].move.z * 0.8f;

			g_aPetals[nCntPetals].pos += g_aPetals[nCntPetals].move;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
		}

		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロック
	g_pVtxBuffPetals->Unlock();
}

//=============================================================================
// ポリゴン描画処理
//=============================================================================
void DrawPetals(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// 計算用マトリックス
	int nCntPetals;

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);		// カリングを無効化

	// 被らないようにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	for (nCntPetals = 0; nCntPetals < MAX_PETALS; nCntPetals++)
	{
		if (g_aPetals[nCntPetals].bUse == true)
		{// 花びらが使われている間
		 // ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPetals[nCntPetals].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPetals[nCntPetals].rot.y, g_aPetals[nCntPetals].rot.x, g_aPetals[nCntPetals].rot.z);
			D3DXMatrixMultiply(&g_aPetals[nCntPetals].mtxWorld, &g_aPetals[nCntPetals].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aPetals[nCntPetals].pos.x, g_aPetals[nCntPetals].pos.y, g_aPetals[nCntPetals].pos.z);
			D3DXMatrixMultiply(&g_aPetals[nCntPetals].mtxWorld, &g_aPetals[nCntPetals].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPetals[nCntPetals].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffPetals, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePetals[g_aPetals[nCntPetals].nType]);

			// 描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntPetals, 2);
		}
	}

	// Zバッファを元に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		// 裏面をカリング
}

//=============================================================================
// 花びらの設定
//=============================================================================
void SetPetals(D3DXVECTOR3 pos, int nNumber)
{
	VERTEX_3D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPetals->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntNumber = 0; nCntNumber < nNumber; nCntNumber++)
	{
		for (int nCntPetals = 0; nCntPetals < MAX_PETALS; nCntPetals++)
		{
			if (g_aPetals[nCntPetals].bUse == false)
			{// 使用されていないもの
				g_nCntPetals = 0;
				g_fAnglePetals = 1.0f / (10.0f / 2.0f);

				g_aPetals[nCntPetals].pos = D3DXVECTOR3(pos.x + ((rand() % 5) * 1.0f), (rand() % 50) + 150.0f, pos.z);
				g_aPetals[nCntPetals].move = D3DXVECTOR3(-15 + (rand() % 30) + ((rand() % 10) * 0.1f),
					-(((rand() % 2) + 1) * 0.5f + ((rand() % 5) * 0.1f)),
					-15 + (rand() % 30) + ((rand() % 10) * 0.1f));
				g_aPetals[nCntPetals].fRadius = rand() % 10 * 1.0f;
				g_aPetals[nCntPetals].col = D3DXCOLOR(1.0f, 0.5f, 0.8f, 1.0f);
				g_aPetals[nCntPetals].angle = D3DXVECTOR3((-5 + (rand() % 10)) * 0.01f, (-5 + (rand() % 10)) * 0.01f, (-5 + (rand() % 10)) * 0.01f);
				g_aPetals[nCntPetals].bUse = true;
				g_aPetals[nCntPetals].nType = 0;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);

				g_nCntAnglePetals = (g_nCntAnglePetals + 1) % 10;
				break;
			}
		}
	}
}
//
//
//
void DeletePetals(void)
{
	for (int nCntPetals = 0; nCntPetals < MAX_PETALS; nCntPetals++)
	{
		g_aPetals[nCntPetals].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPetals[nCntPetals].fRadius = 0;
		g_aPetals[nCntPetals].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aPetals[nCntPetals].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPetals[nCntPetals].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPetals[nCntPetals].angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPetals[nCntPetals].bUse = false;
	}
}