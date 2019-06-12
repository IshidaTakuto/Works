//=============================================================================
//
// 雪処理 [snow.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "snow.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SNOW_TEXTURENAME	"data/TEXTURE/shadow000.jpg"
#define MAX_SNOW		(1020)	// 雪の最大数
#define SNOW_FIELD		(1250)	// 中心を0とした範囲
#define TITLE_SNOW_FIELD	(150)
#define SNOW_APPEAR		(3)	// 出現する間隔

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSnow = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSnow = NULL;	// 頂点バッファへのポインタ
SNOW g_aSnow[MAX_SNOW];
int g_nCntSnow;

//=============================================================================
// ポリゴン初期化処理
//=============================================================================
void InitSnow(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// ジェネレータの情報を初期化
	for (int nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{
		g_aSnow[nCntSnow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSnow[nCntSnow].fRadius = 0;
		g_aSnow[nCntSnow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aSnow[nCntSnow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSnow[nCntSnow].bUse = false;
	}

	g_nCntSnow = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, SNOW_TEXTURENAME, &g_pTextureSnow);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SNOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSnow,
		NULL);

	VERTEX_3D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSnow->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	for (int nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{
		pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);

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
	g_pVtxBuffSnow->Unlock();
}

//=============================================================================
// ポリゴン終了処理
//=============================================================================
void UninitSnow(void)
{
	// テクスチャの破棄
	if (g_pTextureSnow != NULL)
	{
		g_pTextureSnow->Release();
		g_pTextureSnow = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffSnow != NULL)
	{
		g_pVtxBuffSnow->Release();
		g_pVtxBuffSnow = NULL;
	}
}

//=============================================================================
// ポリゴン更新処理
//=============================================================================
void UpdateSnow(void)
{
	int nCntSnow;
	VERTEX_3D*pVtx;	// 頂点情報へのポインタ

	Player *pPlayer = GetPlayer();
	MODE mode = GetMode();

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSnow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{
		switch (mode)
		{
		case MODE_TITLE:
			g_nCntSnow = (g_nCntSnow + 1) % SNOW_APPEAR;	// 雪を出す間隔のカウンタ

			if (g_aSnow[nCntSnow].bUse == true)
			{// 使われている
				if (g_aSnow[nCntSnow].pos.y <= 50)
				{
					g_aSnow[nCntSnow].bUse = false;
				}

				g_aSnow[nCntSnow].pos += g_aSnow[nCntSnow].move;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
			}
			else
			{
				if (g_nCntSnow == 0)
				{// 一定時間経過で雪を降らす
					g_aSnow[nCntSnow].pos = D3DXVECTOR3(-TITLE_SNOW_FIELD + (rand() % (TITLE_SNOW_FIELD * 2)) * 1.0f, 200.0f, ((-50 / 2) + (rand() % 50)) * 1.0f);
					g_aSnow[nCntSnow].move = D3DXVECTOR3(rand() % 3 * 0.1f, -rand() % 10 * 0.1f, rand() % 3 * 0.1f);
					g_aSnow[nCntSnow].fRadius = 0.7f;
					g_aSnow[nCntSnow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
					g_aSnow[nCntSnow].bUse = true;

					//頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);

					// 頂点カラー
					pVtx[0].col = g_aSnow[nCntSnow].col;
					pVtx[1].col = g_aSnow[nCntSnow].col;
					pVtx[2].col = g_aSnow[nCntSnow].col;
					pVtx[3].col = g_aSnow[nCntSnow].col;

					//break;
				}
			}
			break;

		case MODE_GAME:
			g_nCntSnow = (g_nCntSnow + 1) % SNOW_APPEAR;	// 雪を出す間隔のカウンタ

			if (g_aSnow[nCntSnow].bUse == true)
			{// 使われている
				if (g_aSnow[nCntSnow].pos.y <= 0)
				{
					g_aSnow[nCntSnow].bUse = false;
				}

				g_aSnow[nCntSnow].pos += g_aSnow[nCntSnow].move;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
			}
			else
			{
				if (g_nCntSnow == 0 && pPlayer->pos.x > SNOW_FIELD * 4)
				{// 一定時間経過で雪を降らす
					g_aSnow[nCntSnow].pos = D3DXVECTOR3((SNOW_FIELD * 5 + rand() % (SNOW_FIELD * 2)) * 1.0f, 150.0f, (-200.0f + (rand() % 400)) * 1.0f);
					g_aSnow[nCntSnow].move = D3DXVECTOR3(rand() % 3 * 0.1f, -rand() % 10 * 0.1f, rand() % 3 * 0.1f);
					g_aSnow[nCntSnow].fRadius = 2.0f;
					g_aSnow[nCntSnow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
					g_aSnow[nCntSnow].bUse = true;

					//頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, g_aSnow[nCntSnow].fRadius, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(-g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aSnow[nCntSnow].fRadius, -g_aSnow[nCntSnow].fRadius, 0.0f);

					// 頂点カラー
					pVtx[0].col = g_aSnow[nCntSnow].col;
					pVtx[1].col = g_aSnow[nCntSnow].col;
					pVtx[2].col = g_aSnow[nCntSnow].col;
					pVtx[3].col = g_aSnow[nCntSnow].col;

					break;
				}
			}
			break;
		}
		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロック
	g_pVtxBuffSnow->Unlock();
}

//=============================================================================
// ポリゴン描画処理
//=============================================================================
void DrawSnow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// 計算用マトリックス
	int nCntSnow;

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{
		if (g_aSnow[nCntSnow].bUse == true)
		{// 雪が使われている間
		 // ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSnow[nCntSnow].mtxWorld);

			// 回転を反映
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			g_aSnow[nCntSnow].mtxWorld._11 = mtxView._11;
			g_aSnow[nCntSnow].mtxWorld._12 = mtxView._21;
			g_aSnow[nCntSnow].mtxWorld._13 = mtxView._31;
			g_aSnow[nCntSnow].mtxWorld._21 = mtxView._12;
			g_aSnow[nCntSnow].mtxWorld._22 = mtxView._22;
			g_aSnow[nCntSnow].mtxWorld._23 = mtxView._32;
			g_aSnow[nCntSnow].mtxWorld._31 = mtxView._13;
			g_aSnow[nCntSnow].mtxWorld._32 = mtxView._23;
			g_aSnow[nCntSnow].mtxWorld._33 = mtxView._33;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aSnow[nCntSnow].pos.x, g_aSnow[nCntSnow].pos.y, g_aSnow[nCntSnow].pos.z);
			D3DXMatrixMultiply(&g_aSnow[nCntSnow].mtxWorld, &g_aSnow[nCntSnow].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSnow[nCntSnow].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSnow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureSnow);

			// 影の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntSnow, 2);
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
