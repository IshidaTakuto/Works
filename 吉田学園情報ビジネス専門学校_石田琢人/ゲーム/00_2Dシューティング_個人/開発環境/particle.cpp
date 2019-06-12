//=============================================================================
//
// エフェクトの処理 [Particle.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "Particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_PARTICLE		"data/TEXTURE/effect000.jpg"	// 読み込むテクスチャファイル名
#define	MAX_PARTICLE			(5096)							// エフェクトの最大数
#define PARTICLE_MOVE	(50)

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureParticle = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	// 頂点バッファへのポインタ
PARTICLE				g_aParticle[MAX_PARTICLE];	// エフェクトの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fRadius = 0.0f;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].nParticles = 0;
		g_aParticle[nCntParticle].state = PARTICLESTATE_NONE;
		g_aParticle[nCntParticle].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PARTICLE, &g_pTextureParticle);

	// 頂点情報の作成
	MakeVertexParticle(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	// テクスチャの開放
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{
	int nCntParticle;
	int nCntParticle2;
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		switch (g_aParticle[nCntParticle].state)
		{
		case PARTICLESTATE_NONE:
			break;

		case PARTICLESTATE_BULLET:
			//g_aParticle[nCntParticle].nLife--;
			//g_aParticle[nCntParticle].fRadius -= 0.3f;

			//if (g_aParticle[nCntParticle].nLife <= 0 || g_aParticle[nCntParticle].fRadius <= 0)
			//{
			//	g_aParticle[nCntParticle].state = PARTICLESTATE_NONE;
			//	g_aParticle[nCntParticle].bUse = false;
			//}

			////頂点座標の設定
			//pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
			//pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
			//pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
			//pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;

			break;

		case PARTICLESTATE_EXPLOSION:
			for (nCntParticle2 = 0; nCntParticle2 <= g_aParticle[nCntParticle].nParticles; nCntParticle2++)
			{
				g_aParticle[nCntParticle2].nLife--;
				g_aParticle[nCntParticle2].fRadius -= 0.1f;

				g_aParticle[nCntParticle2].pos.x += g_aParticle[nCntParticle2].move.x;
				g_aParticle[nCntParticle2].pos.y += g_aParticle[nCntParticle2].move.y;

				if (g_aParticle[nCntParticle2].nLife <= 0 || g_aParticle[nCntParticle2].fRadius <= 0)
				{
					g_aParticle[nCntParticle2].state = PARTICLESTATE_NONE;
					g_aParticle[nCntParticle2].bUse = false;
				}

			}
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;

			break;
		}

		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void)
{
	int nCntParticle;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureParticle);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ポリゴンの描画
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)	//使用されている
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntParticle, 2);
		}
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntParticle;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	// 頂点バッファの情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;

		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}


//=============================================================================
// パーティクル設定
//=============================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nParticles, PARTICLESTATE state)
{
	int nCntParticle;
	int nCntParticle2;

	// 頂点バッファの情報を設定
	//VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	//g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle2 = 0; nCntParticle2 < nParticles; nCntParticle2++)
	{
		for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
		{
			if (g_aParticle[nCntParticle].bUse == false)
			{
				g_aParticle[nCntParticle2].pos = pos;
				g_aParticle[nCntParticle2].move = move;
				g_aParticle[nCntParticle2].col = col;
				g_aParticle[nCntParticle2].fRadius = fRadius;
				g_aParticle[nCntParticle2].nLife = nLife;
				g_aParticle[nCntParticle2].nParticles = nParticles;
				g_aParticle[nCntParticle2].state = state;

				//頂点座標の設定
				//pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
				//pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
				//pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;
				//pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f) + g_aParticle[nCntParticle].pos;

				//頂点カラー
				//pVtx[0].col = col;
				//pVtx[1].col = col;
				//pVtx[2].col = col;
				//pVtx[3].col = col;

				g_aParticle[nCntParticle].bUse = true;
				break;
			}
			//pVtx += 4;	//頂点データのポインタを4つ進める
		}
	}
	// 頂点バッファをアンロックする
	//g_pVtxBuffParticle->Unlock();
}
