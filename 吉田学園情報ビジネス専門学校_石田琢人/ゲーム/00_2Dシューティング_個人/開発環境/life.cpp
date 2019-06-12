//=============================================================================
//
// 体力処理 [life.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "life.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_LIFE		"data/TEXTURE/life000.png"	// 読み込むテクスチャファイル名
#define MAX_LIFE	(5)		//体力の最大桁数
#define LIFE_SIZE	(25)	//体力の大きさ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureLife = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;		// 頂点バッファへのポインタ
LIFE					g_Life[MAX_LIFE];					// 体力

//=============================================================================
// 初期化処理
//=============================================================================
void InitLife(void)
{
	int nCntLife;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{// 体力の初期化
		g_Life[nCntLife].pos = D3DXVECTOR3(40, 40, 0.0f);	// 体力の位置
		g_Life[nCntLife].nCntLife = MAX_LIFE;
		g_Life[nCntLife].bUse = true;
	}


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_LIFE,		// ファイルの名前
								&g_pTextureLife);	// テクスチャへのポインタ

	// 頂点情報の作成
	MakeVertexLife(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLife(void)
{
	// テクスチャの開放
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLife(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureLife);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		if (g_Life[nCntLife].bUse == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntLife, 2);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexLife(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntLife;
	int nCntLife2;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x - LIFE_SIZE, g_Life[nCntLife].pos.y - LIFE_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x + LIFE_SIZE, g_Life[nCntLife].pos.y - LIFE_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x - LIFE_SIZE, g_Life[nCntLife].pos.y + LIFE_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Life[nCntLife].pos.x + LIFE_SIZE, g_Life[nCntLife].pos.y + LIFE_SIZE, 0.0f);

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
		for (nCntLife2 = 0; nCntLife2 < MAX_LIFE; nCntLife2++)
		{
			g_Life[nCntLife2].pos.x += 50;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffLife->Unlock();
}

//=============================================================================
// 体力の減算
//=============================================================================
void SubtractionLife(int nValue)
{
	if (g_Life[nValue - 1].bUse == true)
	{
		g_Life[nValue - 1].bUse = false;
	}
}

//=============================================================================
// 体力の取得
//=============================================================================
LIFE *GetLife(void)
{
	return &g_Life[0];
}