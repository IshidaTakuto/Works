//=============================================================================
//
// エフェクトの処理 [effect.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "effect.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EFFECT		"data/TEXTURE/effect000.jpg"	// 読み込むテクスチャファイル名
#define	MAX_EFFECT			(5096)							// エフェクトの最大数
#define EFFECT_EXPLOSION	(50)
#define BEAM_DAMAGE			(100)

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// 頂点バッファへのポインタ
EFFECT					g_aEffect[MAX_EFFECT];		// エフェクトの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fRadius = 0.0f;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
		g_aEffect[nCntEffect].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT, &g_pTextureEffect);

	// 頂点情報の作成
	MakeVertexEffect(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	// テクスチャの開放
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	int nCntEffect;
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		switch (g_aEffect[nCntEffect].state)
		{
		case EFFECTSTATE_NONE:
			break;

		case EFFECTSTATE_BULLET:
			g_aEffect[nCntEffect].nLife--;
			g_aEffect[nCntEffect].fRadius -= 0.3f;

			if (g_aEffect[nCntEffect].nLife <= 0 || g_aEffect[nCntEffect].fRadius <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;

			break;

		case EFFECTSTATE_EXPLOSION:
			g_aEffect[nCntEffect].nLife--;
			g_aEffect[nCntEffect].fRadius += 1.0f;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius - rand() % EFFECT_EXPLOSION, -g_aEffect[nCntEffect].fRadius - rand() % EFFECT_EXPLOSION, 0.0f) + g_aEffect[nCntEffect].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius + rand() % EFFECT_EXPLOSION, -g_aEffect[nCntEffect].fRadius - rand() % EFFECT_EXPLOSION, 0.0f) + g_aEffect[nCntEffect].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius - rand() % EFFECT_EXPLOSION, g_aEffect[nCntEffect].fRadius + rand() % EFFECT_EXPLOSION, 0.0f) + g_aEffect[nCntEffect].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius + rand() % EFFECT_EXPLOSION, g_aEffect[nCntEffect].fRadius + rand() % EFFECT_EXPLOSION, 0.0f) + g_aEffect[nCntEffect].pos;

			break;

		case EFFECTSTATE_BEAM:
			g_aEffect[nCntEffect].nLife--;
			if (g_aEffect[nCntEffect].fRadius <= 100.0f)
			{
				g_aEffect[nCntEffect].fRadius += 5.0f;
			}
			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].fRadius -= 10.0f;
				if (g_aEffect[nCntEffect].fRadius <= 0)
				{
					g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
					g_aEffect[nCntEffect].bUse = false;
				}
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;

			int nCntEnemy;
			Enemy *pEnemy;

			//敵の取得
			pEnemy = GetEnemy();

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->bUse == true)
				{//敵が使用されている
					if (pEnemy->pos.x - 20 < g_aEffect[nCntEffect].pos.x + 60
						&& pEnemy->pos.x + 20 > g_aEffect[nCntEffect].pos.x - 60
						&& pEnemy->pos.y - 20 < g_aEffect[nCntEffect].pos.y + 60
						&& pEnemy->pos.y + 20 > g_aEffect[nCntEffect].pos.y - 60)
					{
						HitEnemy(nCntEnemy, BEAM_DAMAGE);
					}
				}
			}
			break;

		case EFFECTSTATE_TITLE:
			g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
			break;
		}
	
		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	int nCntEffect;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEffect);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ポリゴンの描画
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)	//弾が使用されている
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEffect, 2);
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
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntEffect;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	// 頂点バッファの情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;

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
	g_pVtxBuffEffect->Unlock();
}


//=============================================================================
// エフェクト設定
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, EFFECTSTATE state)
{
	int nCntEffect;

	// 頂点バッファの情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].state = state;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f) + g_aEffect[nCntEffect].pos;

			//頂点カラー
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aEffect[nCntEffect].bUse = true;
			break;

		}
		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}
