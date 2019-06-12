//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// エフェクトの処理 [effect.cpp]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_EFFECT			(4096 * 3)		// エフェクトの最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureEffect[EFFECTTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;			// 頂点バッファへのポインタ

// ----- テクスチャ情報  ----- //
EffectTexture g_aEffectTexture[EFFECTTYPE_MAX] =
{
	{ "data/TEXTURE/effect/effect000.png" },	// 弱攻撃
	{ "data/TEXTURE/effect/effect000.png" },	// 強攻撃
	{ "data/TEXTURE/effect/effect000.png" },	// 妖刀『A・K・R』
	{ "data/TEXTURE/effect/effect000.png" },	// 投てき武器
};

EFFECT g_aEffect[MAX_EFFECT];		// エフェクトの情報

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 初期化処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 初期化
	for(int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].nType = 0;
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fRadius = 0.0f;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	// テクスチャの読み込み
	for (int nTex = 0 ;nTex < EFFECTTYPE_MAX; nTex++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aEffectTexture[nTex].pFilename, &g_apTextureEffect[nTex]);
	}

	// 頂点情報の作成
	MakeVertexEffect(pDevice);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 終了処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UninitEffect(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}

	// テクスチャの開放
	for (int nTex = 0; nTex < EFFECTTYPE_MAX; nTex++)
	{
		if (g_apTextureEffect[nTex] != NULL)
		{
			g_apTextureEffect[nTex]->Release();
			g_apTextureEffect[nTex] = NULL;
		}
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 更新処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UpdateEffect(void)
{
	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
	{
		if (g_aEffect[nCntEffect].bUse == true && g_aEffect[nCntEffect].nLife >= 0)
		{
			// 位置の更新
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			// 寿命の減少
			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].nLife <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}
			// 半径の縮小
			if (g_aEffect[nCntEffect].fRadius >= 0.0f)
			{
				g_aEffect[nCntEffect].fRadius -= 0.5f;
			}
			if (g_aEffect[nCntEffect].fRadius <= 0.0f)
			{
				g_aEffect[nCntEffect].bUse = false;
			}

			//====================
			// 種類別処理
			//====================
			switch (g_aEffect[nCntEffect].nType)
			{
				//--- 弱攻撃 ---//
			case EFFECTTYPE_WEAK_ATTACK:
				break;

				//--- 強攻撃 ---//
			case EFFECTTYPE_STRONG_ATTACK:
				break;

				//--- 妖刀『A・K・R』 ---//
			case EFFECTTYPE_AKR:
				// 下に落ちる移動をする
				g_aEffect[nCntEffect].move.y = -0.8f;

				break;

				//--- 弾[3D] ---//
			case EFFECTTYPE_3DBULLET:
				break;
			}

			//====================
			// 頂点情報の更新
			//====================
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), +(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+(g_aEffect[nCntEffect].fRadius / 2), +(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+(g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 描画処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// 計算用マトリックス

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);		// アルファテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// 描画の基準 [α値 = 0]
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 描画の条件 [～より大きい]

	// ライティングを無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				// ライティングモード無効

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)		// 使用されているものだけ計算 & 描画
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			// -----  ビルボードの設定  ----- //
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

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aEffect[nCntEffect].rot.y, g_aEffect[nCntEffect].rot.x, g_aEffect[nCntEffect].rot.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld,
				&g_aEffect[nCntEffect].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld,
				&g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEffect[g_aEffect[nCntEffect].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	// レンダーステートの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);		// アルファテストを無効化

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ライティングを元に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);				// ライティングモード有効
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 頂点の作成
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,	//確保するバッファのサイズ
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,			//頂点フォーマット
								D3DPOOL_MANAGED,
								&g_pVtxBuffEffect,
								NULL);

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
	{
		//頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 法線の設定
		for (int nNor = 0; nNor < 4; nNor++)
		{
			pVtx[nNor].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);		// ※固定　[法線zは手前を向いている]
		}
		// 頂点カラーの設定
		for (int nCol = 0; nCol < 4; nCol++)
		{
			pVtx[nCol].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// エフェクト設定
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife,int nType)
{
	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].nType = nType;

			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), +(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+(g_aEffect[nCntEffect].fRadius / 2), +(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+(g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);

			// 頂点カラーの設定
			for (int nCol = 0; nCol < 4; nCol++)
			{
				pVtx[nCol].col = g_aEffect[nCntEffect].col;
			}

			// 使用状態
			g_aEffect[nCntEffect].bUse = true;

			break;		//　重要！
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}
