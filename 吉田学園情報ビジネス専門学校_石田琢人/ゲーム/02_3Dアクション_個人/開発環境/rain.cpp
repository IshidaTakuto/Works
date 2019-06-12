//=============================================================================
//
// 雨処理 [rain.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "rain.h"
#include "effect.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RAIN		"data/TEXTURE/shadow000.jpg"	// 読み込むテクスチャファイル名
#define MAX_RAIN	(300)	// 雨の最大数
#define RAIN_FIELD	(2500 / 2)	//
#define TITLE_FIELD	(150)	//

#define RAIN_APPEAR	(10)	//
#define RAIN_SIZE	(0.3f)	// 雨の大きさ
#define RAIN_SPEED	(2.0f)	// 雨の速さ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureRain = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRain = NULL;	// 頂点バッファへのポインタ
RAIN g_aRain[MAX_RAIN];
int g_nCntRain;

//=============================================================================
// ポリゴン初期化処理
//=============================================================================
void InitRain(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// ジェネレータの情報を初期化
	for (int nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{
		g_aRain[nCntRain].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRain[nCntRain].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aRain[nCntRain].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRain[nCntRain].bUse = false;
	}

	g_nCntRain = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RAIN, &g_pTextureRain);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_RAIN,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRain,
		NULL);

	VERTEX_3D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffRain->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	for (int nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{
		pVtx[0].pos = D3DXVECTOR3(-RAIN_SIZE, RAIN_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(RAIN_SIZE, RAIN_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-RAIN_SIZE, -RAIN_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(RAIN_SIZE, -RAIN_SIZE, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
	g_pVtxBuffRain->Unlock();
}

//=============================================================================
// ポリゴン終了処理
//=============================================================================
void UninitRain(void)
{
	// テクスチャの破棄
	if (g_pTextureRain != NULL)
	{
		g_pTextureRain->Release();
		g_pTextureRain = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffRain != NULL)
	{
		g_pVtxBuffRain->Release();
		g_pVtxBuffRain = NULL;
	}
}

//=============================================================================
// ポリゴン更新処理
//=============================================================================
void UpdateRain(void)
{
	int nCntRain;
	VERTEX_3D*pVtx;	// 頂点情報へのポインタ

	Player *pPlayer = GetPlayer();
	MODE mode = GetMode();

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffRain->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{
		g_nCntRain++;	// 雨を出す間隔のカウンタ

		switch (mode)
		{
		case MODE_TITLE:
			if (g_aRain[nCntRain].bUse == true)
			{// 使われている
				if (g_aRain[nCntRain].pos.y <= 0)
				{
					g_aRain[nCntRain].bUse = false;
				}

				g_aRain[nCntRain].pos += g_aRain[nCntRain].move;
			}
			else
			{
				if (g_nCntRain % RAIN_APPEAR == 0)
				{// 一定時間経過で雨を降らす
					if (g_aRain[nCntRain].bUse == false)
					{
						g_aRain[nCntRain].pos = D3DXVECTOR3(-TITLE_FIELD + (rand() % (TITLE_FIELD * 2)) * 1.0f, 200.0f, ((-100 / 2) + (rand() % 100)) * 1.0f);
						g_aRain[nCntRain].move = D3DXVECTOR3(0.0f, -RAIN_SPEED * (rand() % 3 + 2.0f), 0.0f);
						g_aRain[nCntRain].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						g_aRain[nCntRain].bUse = true;

						//頂点座標の設定
						pVtx[0].pos = D3DXVECTOR3(-RAIN_SIZE, RAIN_SIZE + (rand() % 20 + 20.0f), 0.0f);
						pVtx[1].pos = D3DXVECTOR3(RAIN_SIZE, RAIN_SIZE + (rand() % 20 + 20.0f), 0.0f);
						pVtx[2].pos = D3DXVECTOR3(-RAIN_SIZE, -RAIN_SIZE, 0.0f);
						pVtx[3].pos = D3DXVECTOR3(RAIN_SIZE, -RAIN_SIZE, 0.0f);

						// 頂点カラー
						pVtx[0].col = g_aRain[nCntRain].col;
						pVtx[1].col = g_aRain[nCntRain].col;
						pVtx[2].col = g_aRain[nCntRain].col;
						pVtx[3].col = g_aRain[nCntRain].col;

						break;
					}
				}
			}
			break;

		case MODE_GAME:
			if (g_aRain[nCntRain].bUse == true)
			{// 使われている
				if (g_aRain[nCntRain].pos.y <= 0)
				{
					g_aRain[nCntRain].bUse = false;
				}

				g_aRain[nCntRain].pos += g_aRain[nCntRain].move;
			}
			else
			{
				if (g_nCntRain % RAIN_APPEAR == 0 && pPlayer->pos.x > 0 && pPlayer->pos.x < RAIN_FIELD * 4)
				{// 一定時間経過で雨を降らす
					if (g_aRain[nCntRain].bUse == false)
					{
						g_aRain[nCntRain].pos = D3DXVECTOR3((RAIN_FIELD + (rand() % (RAIN_FIELD * 2))) * 1.0f, 150.0f, -200.0f + (rand() % (200 * 2)) * 1.0f);
						g_aRain[nCntRain].move = D3DXVECTOR3(0.0f, -RAIN_SPEED * (rand() % 3 + 2.0f), 0.0f);
						g_aRain[nCntRain].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						g_aRain[nCntRain].bUse = true;

						//頂点座標の設定
						pVtx[0].pos = D3DXVECTOR3(-RAIN_SIZE, RAIN_SIZE + (rand() % 20 + 20.0f), 0.0f);
						pVtx[1].pos = D3DXVECTOR3(RAIN_SIZE, RAIN_SIZE + (rand() % 20 + 20.0f), 0.0f);
						pVtx[2].pos = D3DXVECTOR3(-RAIN_SIZE, -RAIN_SIZE, 0.0f);
						pVtx[3].pos = D3DXVECTOR3(RAIN_SIZE, -RAIN_SIZE, 0.0f);

						// 頂点カラー
						pVtx[0].col = g_aRain[nCntRain].col;
						pVtx[1].col = g_aRain[nCntRain].col;
						pVtx[2].col = g_aRain[nCntRain].col;
						pVtx[3].col = g_aRain[nCntRain].col;

						break;
					}
				}
			}
			break;
		}
		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロック
	g_pVtxBuffRain->Unlock();
}

//=============================================================================
// ポリゴン描画処理
//=============================================================================
void DrawRain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// 計算用マトリックス
	int nCntRain;

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (nCntRain = 0; nCntRain < MAX_RAIN; nCntRain++)
	{
		if (g_aRain[nCntRain].bUse == true)
		{// 雨が使われている間
		 // ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aRain[nCntRain].mtxWorld);

			// 回転を反映
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			g_aRain[nCntRain].mtxWorld._11 = mtxView._11;
			g_aRain[nCntRain].mtxWorld._12 = mtxView._21;
			g_aRain[nCntRain].mtxWorld._13 = mtxView._31;
			//g_aRain[nCntRain].mtxWorld._21 = mtxView._12;
			//g_aRain[nCntRain].mtxWorld._22 = mtxView._22;
			//g_aRain[nCntRain].mtxWorld._23 = mtxView._32;
			g_aRain[nCntRain].mtxWorld._31 = mtxView._13;
			g_aRain[nCntRain].mtxWorld._32 = mtxView._23;
			g_aRain[nCntRain].mtxWorld._33 = mtxView._33;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aRain[nCntRain].pos.x, g_aRain[nCntRain].pos.y, g_aRain[nCntRain].pos.z);
			D3DXMatrixMultiply(&g_aRain[nCntRain].mtxWorld, &g_aRain[nCntRain].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aRain[nCntRain].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffRain, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureRain);

			// 影の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRain, 2);
		}
	}

	// αテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
