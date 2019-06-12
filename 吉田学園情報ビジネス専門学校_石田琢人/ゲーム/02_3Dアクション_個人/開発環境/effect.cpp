//=============================================================================
//
// エフェクトの処理 [effect.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EFFECT0		"data/TEXTURE/shadow000.jpg"	// 読み込むテクスチャファイル名
#define	TEXTURE_EFFECT1		"data/TEXTURE/effect001.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_EFFECT2		"data/TEXTURE/moon.jpg"			// 読み込むテクスチャファイル名
#define TEXTURE_EFFECT3		"data/TEXTURE/spark.jpg"		// 読み込むテクスチャファイル名
#define TEXTURE_EFFECT4		"data/TEXTURE/flower.jpg"		// 読み込むテクスチャファイル名
#define TEXTURE_EFFECT5		"data/TEXTURE/ripples.jpg"		// 読み込むテクスチャファイル名

#define	MAX_EFFECT		(5096)							// エフェクトの最大数
#define MAX_TEXTURE		(6)

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureEffect[MAX_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// 頂点バッファへのポインタ
EFFECT					g_aEffect[MAX_EFFECT];		// エフェクトの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEffect;

	for(nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fRadius = 0.0f;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
		g_aEffect[nCntEffect].nType = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT0, &g_pTextureEffect[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT1, &g_pTextureEffect[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT2, &g_pTextureEffect[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT3, &g_pTextureEffect[3]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT4, &g_pTextureEffect[4]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT5, &g_pTextureEffect[5]);

	// 頂点情報の作成
	MakeVertexEffect(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	int nCntEffect;

	for (nCntEffect = 0; nCntEffect < MAX_TEXTURE; nCntEffect++)
	{
		// テクスチャの開放
		if (g_pTextureEffect[nCntEffect] != NULL)
		{
			g_pTextureEffect[nCntEffect]->Release();
			g_pTextureEffect[nCntEffect] = NULL;
		}
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
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		switch (g_aEffect[nCntEffect].state)
		{
		case EFFECTSTATE_NONE:
			g_aEffect[nCntEffect].nType = 0;
			break;

		case EFFECTSTATE_BULLET:
			g_aEffect[nCntEffect].nLife--;
			g_aEffect[nCntEffect].fRadius -= 0.3f;
			g_aEffect[nCntEffect].nType = 0;

			if (g_aEffect[nCntEffect].nLife <= 0 || g_aEffect[nCntEffect].fRadius <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			break;

		case EFFECTSTATE_RAIN:
			g_aEffect[nCntEffect].nType = 0;
			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			break;

		case EFFECTSTATE_HIPFIRE:
			g_aEffect[nCntEffect].fRadius -= 0.3f;
			g_aEffect[nCntEffect].nType = 0;

			if (g_aEffect[nCntEffect].fRadius <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			break;

		case EFFECTSTATE_SPARK:
			g_aEffect[nCntEffect].fRadius -= 0.05f;
			g_aEffect[nCntEffect].nType = 1;

			if (g_aEffect[nCntEffect].fRadius <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			break;

		case EFFECTSTATE_BLOOD:
			g_aEffect[nCntEffect].nLife--;
				g_aEffect[nCntEffect].fRadius -= 0.08f;

			g_aEffect[nCntEffect].nType = 3;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			break;

		case EFFECTSTATE_MOON:
			g_aEffect[nCntEffect].nType = 2;
			break;

		case EFFECTSTATE_GATHER:
			g_aEffect[nCntEffect].fRadius -= 0.1f;
			g_aEffect[nCntEffect].nLife--;
			g_aEffect[nCntEffect].nType = 0;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			break;

		case EFFECTSTATE_FOUNTAIN:
			g_aEffect[nCntEffect].fRadius -= 0.05f;
			g_aEffect[nCntEffect].nType = 0;

			if (g_aEffect[nCntEffect].fRadius <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

			break;

		case EFFECTSTATE_RIPPLES:
			g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aEffect[nCntEffect].fRadius += 2.0f;
			g_aEffect[nCntEffect].col.a -= 0.05f;
			g_aEffect[nCntEffect].nType = 5;

			if (g_aEffect[nCntEffect].col.a <= 0)
			{
				g_aEffect[nCntEffect].state = EFFECTSTATE_NONE;
				g_aEffect[nCntEffect].bUse = false;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, 0.0f, g_aEffect[nCntEffect].fRadius);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, 0.0f, g_aEffect[nCntEffect].fRadius);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, 0.0f, -g_aEffect[nCntEffect].fRadius);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, 0.0f, -g_aEffect[nCntEffect].fRadius);

			//頂点カラー
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

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
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// 計算用マトリックス

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 被らないようにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)	//弾が使用されている
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			if (g_aEffect[nCntEffect].state != EFFECTSTATE_RIPPLES)
			{
				// 回転を反映
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);
				g_aEffect[nCntEffect].mtxWorld._11 = mtxView._11;
				g_aEffect[nCntEffect].mtxWorld._12 = mtxView._21;
				g_aEffect[nCntEffect].mtxWorld._13 = mtxView._31;
				g_aEffect[nCntEffect].mtxWorld._21 = mtxView._12;
				g_aEffect[nCntEffect].mtxWorld._22 = mtxView._22;
				g_aEffect[nCntEffect].mtxWorld._23 = mtxView._32;
				g_aEffect[nCntEffect].mtxWorld._31 = mtxView._13;
				g_aEffect[nCntEffect].mtxWorld._32 = mtxView._23;
				g_aEffect[nCntEffect].mtxWorld._33 = mtxView._33;
			}
			else
			{
				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEffect[nCntEffect].rot.y, g_aEffect[nCntEffect].rot.x, g_aEffect[nCntEffect].rot.z);
				D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxRot);
			}

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCntEffect].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEffect, 2);
		}
	}

	// Zバッファを元に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// αテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

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
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	// 頂点バッファの情報を設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
	VERTEX_3D *pVtx;

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
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);

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
