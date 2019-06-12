//=============================================================================
//
// 影の処理 [shadow.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_SHADOW		"data/TEXTURE/shadow000.jpg"	// 読み込むテクスチャファイル名
#define SHADOW_WIDTH		(10.0f)							// x軸の大きさ
#define SHADOW_DEPTH		(10.0f)							// z軸の大きさ
#define MAX_SHADOW			(900)							// 影の最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	// 頂点バッファへのポインタ
Shadow g_aShadow[MAX_SHADOW];						// 影の情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitShadow(void)
{
	int nCntShadow;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SHADOW, &g_pTextureShadow);

	// 頂点情報の作成
	MakeVertexShadow(pDevice);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きの初期化
		g_aShadow[nCntShadow].fWidth = 0.0f;
		g_aShadow[nCntShadow].fDepth = 0.0f;
		g_aShadow[nCntShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aShadow[nCntShadow].bUse = false;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitShadow(void)
{
	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateShadow(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans;				// 計算用マトリックス
	int nCntShadow;

	//フォグを使用しない
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 被らないようにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{// 影が使われている間
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			// 影の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntShadow, 2);
		}
	}

	// Zバッファを元に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//フォグを使用する
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntShadow;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		// 頂点の座標
		pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].fDepth);
		pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].fDepth);
		pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidth, 0.0f, -g_aShadow[nCntShadow].fDepth);
		pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fWidth, 0.0f, -g_aShadow[nCntShadow].fDepth);

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

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	// 頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// 影の設定処理
//=============================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fDepth)
{
	int nCntShadow;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{
			g_aShadow[nCntShadow].pos.x = pos.x;
			g_aShadow[nCntShadow].pos.z = pos.z;
			g_aShadow[nCntShadow].rot = rot;
			g_aShadow[nCntShadow].fWidth = fWidth;
			g_aShadow[nCntShadow].fDepth = fDepth;

			// 頂点の座標
			pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].fDepth);
			pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fWidth, 0.0f, g_aShadow[nCntShadow].fDepth);
			pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fWidth, 0.0f, -g_aShadow[nCntShadow].fDepth);
			pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fWidth, 0.0f, -g_aShadow[nCntShadow].fDepth);

			g_aShadow[nCntShadow].bUse = true;
			break;
		}
		pVtx += 4;	// 頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;
}

//=============================================================================
// 影の位置を設定
//=============================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nIdxShadow].pos.x = pos.x;
	g_aShadow[nIdxShadow].pos.z = pos.z;
}

//=============================================================================
// 影の高さによる頂点を設定
//=============================================================================
void SetVtxShadow(int nIdxShadow, D3DXVECTOR3 pos, float fAlpha)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aShadow[nIdxShadow].bUse == true)
	{
		g_aShadow[nIdxShadow].fHeightWidth = 0.08f * pos.y;
		g_aShadow[nIdxShadow].fHeightDepth = 0.08f * pos.y;

		g_aShadow[nIdxShadow].col.a = fAlpha - (0.005f * pos.y);
	}

	// 頂点の座標
	pVtx[0 + (nIdxShadow * 4)].pos = D3DXVECTOR3(-g_aShadow[nIdxShadow].fWidth - g_aShadow[nIdxShadow].fHeightWidth, 0.0f, g_aShadow[nIdxShadow].fDepth + g_aShadow[nIdxShadow].fHeightDepth);
	pVtx[1 + (nIdxShadow * 4)].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].fWidth + g_aShadow[nIdxShadow].fHeightWidth, 0.0f, g_aShadow[nIdxShadow].fDepth + g_aShadow[nIdxShadow].fHeightDepth);
	pVtx[2 + (nIdxShadow * 4)].pos = D3DXVECTOR3(-g_aShadow[nIdxShadow].fWidth - g_aShadow[nIdxShadow].fHeightWidth, 0.0f, -g_aShadow[nIdxShadow].fDepth - g_aShadow[nIdxShadow].fHeightDepth);
	pVtx[3 + (nIdxShadow * 4)].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].fWidth + g_aShadow[nIdxShadow].fHeightWidth, 0.0f, -g_aShadow[nIdxShadow].fDepth - g_aShadow[nIdxShadow].fHeightDepth);

	// 頂点カラー
	pVtx[0 + (nIdxShadow * 4)].col = g_aShadow[nIdxShadow].col;
	pVtx[1 + (nIdxShadow * 4)].col = g_aShadow[nIdxShadow].col;
	pVtx[2 + (nIdxShadow * 4)].col = g_aShadow[nIdxShadow].col;
	pVtx[3 + (nIdxShadow * 4)].col = g_aShadow[nIdxShadow].col;

	// 頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// 影の削除
//=============================================================================
void DeleteShadow(int nIdxShadow)
{
	if (g_aShadow[nIdxShadow].bUse == true)
	{
		g_aShadow[nIdxShadow].bUse = false;
	}
}

//=============================================================================
// 影の取得
//=============================================================================
Shadow *GetShadow(void)
{
	return &g_aShadow[0];
}