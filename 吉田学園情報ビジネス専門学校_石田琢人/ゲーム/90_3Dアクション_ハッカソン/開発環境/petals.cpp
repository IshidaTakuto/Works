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
#define MAX_PETALS			(128)		// 花びらの最大数
#define MAX_TEXTURE			(1)			// テクスチャの数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePetals[MAX_TEXTURE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPetals = NULL;	// 頂点バッファへのポインタ
PETALS g_aPetals[MAX_PETALS];

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
		g_aPetals[nCntPetals].bMoveWidth = false;
		g_aPetals[nCntPetals].bMoveAngle = false;
		g_aPetals[nCntPetals].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PETALS_TEXTURENAME00, &g_pTexturePetals[0]);

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

			// 下降
			if (g_aPetals[nCntPetals].bMoveAngle)
			{// 緩やか
				g_aPetals[nCntPetals].move.y += 0.01f;

				if (g_aPetals[nCntPetals].move.y >= 0.3f)
				{// 一定まで上昇した
					g_aPetals[nCntPetals].bMoveAngle = false;
				}
			}
			else if (!g_aPetals[nCntPetals].bMoveAngle)
			{// 急
				g_aPetals[nCntPetals].move.y -= 0.01f;

				if (g_aPetals[nCntPetals].move.y <= -0.8f)
				{// 一定まで下降した
					g_aPetals[nCntPetals].bMoveAngle = true;
				}
			}

			// 横の移動
			if (g_aPetals[nCntPetals].bMoveWidth)
			{// ＋方向
				g_aPetals[nCntPetals].move.x += 0.01f;

				if (g_aPetals[nCntPetals].move.x >= 0.8f)
				{// 一定まで速度が落ちた
					g_aPetals[nCntPetals].bMoveWidth = false;
				}
			}
			else if (!g_aPetals[nCntPetals].bMoveWidth)
			{// -方向
				g_aPetals[nCntPetals].move.x -= 0.01f;

				if (g_aPetals[nCntPetals].move.x <= -0.8f)
				{// 一定まで速度が上がった
					g_aPetals[nCntPetals].bMoveWidth = true;
				}
			}

			// 移動量を加算
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
				g_aPetals[nCntPetals].pos = pos;
				g_aPetals[nCntPetals].move = D3DXVECTOR3(-15 + (rand() % 30) + ((rand() % 10) * 0.1f) * 0.008f,
					-(((rand() % 2) + 1) * 0.5f + ((rand() % 5) * 0.1f)),
					-15 + (rand() % 30) + ((rand() % 10) * 0.1f)) * 0.008f;
				g_aPetals[nCntPetals].fRadius = 10;
				g_aPetals[nCntPetals].col = D3DXCOLOR(1.0f, 0.5f, 0.8f, 1.0f);
				g_aPetals[nCntPetals].angle = D3DXVECTOR3((-5 + (rand() % 10)) * 0.01f, (-5 + (rand() % 10)) * 0.01f, (-5 + (rand() % 10)) * 0.01f);
				g_aPetals[nCntPetals].bUse = true;
				g_aPetals[nCntPetals].nType = 0;
				// 横に揺らす方向
				if(rand() % 2 == 0){ g_aPetals[nCntPetals].bMoveWidth = false; }
				else{ g_aPetals[nCntPetals].bMoveWidth = true; }

				// 縦に揺らす方向
				if (rand() % 2 == 0) { g_aPetals[nCntPetals].bMoveAngle = false; }
				else { g_aPetals[nCntPetals].bMoveAngle = true; }

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, g_aPetals[nCntPetals].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aPetals[nCntPetals].fRadius, -g_aPetals[nCntPetals].fRadius, 0.0f);
				break;
			}
		}
	}
}
