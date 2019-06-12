//=============================================================================
//
// ビーム処理 [Beam.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "beam.h"
#include "player.h"
#include "effect.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BEAM		"data/TEXTURE/bar.png"	// 読み込むテクスチャファイル名
#define MAX_BEAM	(2)	//チャージの最大数
#define BEAM_GAUGE_SIZE	(100)
#define MAX_GAUGE	(210)	//ゲージの最大値

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexBeam(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBeam[MAX_BEAM] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBeam = NULL;				// 頂点バッファへのポインタ
BEAM					g_aBeam[MAX_BEAM];					// ビーム

//=============================================================================
// 初期化処理
//=============================================================================
void InitBeam(void)
{
	int nCntBeam;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// ビームの初期化
	for (nCntBeam = 0; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		g_aBeam[nCntBeam].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aBeam[nCntBeam].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 30, 0.0f);
		g_aBeam[nCntBeam].nCntGauge = 0;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BEAM, &g_pTextureBeam[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BEAM, &g_pTextureBeam[1]);

	// 頂点情報の作成
	MakeVertexBeam(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBeam(void)
{
	int nCntBeam;

	for (nCntBeam = 0; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		// テクスチャの開放
		if (g_pTextureBeam[nCntBeam] != NULL)
		{
			g_pTextureBeam[nCntBeam]->Release();
			g_pTextureBeam[nCntBeam] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBeam != NULL)
	{
		g_pVtxBuffBeam->Release();
		g_pVtxBuffBeam = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBeam(void)
{
	Player *pPlayer;
	pPlayer = GetPlayer();

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBeam->Lock(0, 0, (void**)&pVtx, 0);

	if (pPlayer->state == PLAYERSTATE_BEAM)
	{
		g_aBeam[1].nCntGauge = 0;

		//頂点カラー
		pVtx[4].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	}

	if (g_aBeam[1].nCntGauge <= MAX_GAUGE)
	{
		//頂点座標の設定
		pVtx[4].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE, g_aBeam[1].pos.y - BEAM_GAUGE_SIZE, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE + g_aBeam[1].nCntGauge, g_aBeam[1].pos.y - BEAM_GAUGE_SIZE, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE, g_aBeam[1].pos.y + BEAM_GAUGE_SIZE, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE + g_aBeam[1].nCntGauge, g_aBeam[1].pos.y + BEAM_GAUGE_SIZE, 0.0f);
	}
	if (g_aBeam[1].nCntGauge == MAX_GAUGE)
	{
		//頂点カラー
		pVtx[4].col = D3DXCOLOR(0.3f, 0.5f, 1.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(0.3f, 0.5f, 1.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(0.3f, 0.5f, 1.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(0.3f, 0.5f, 1.0f, 1.0f);
	}


	// 頂点バッファをアンロックする
	g_pVtxBuffBeam->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBeam(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBeam, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBeam = 0; nCntBeam < MAX_BEAM; nCntBeam++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBeam[nCntBeam]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBeam, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexBeam(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BEAM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBeam,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBeam->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aBeam[0].pos.x - BEAM_GAUGE_SIZE, g_aBeam[0].pos.y - BEAM_GAUGE_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBeam[0].pos.x + BEAM_GAUGE_SIZE, g_aBeam[0].pos.y - BEAM_GAUGE_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBeam[0].pos.x - BEAM_GAUGE_SIZE, g_aBeam[0].pos.y + BEAM_GAUGE_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBeam[0].pos.x + BEAM_GAUGE_SIZE, g_aBeam[0].pos.y + BEAM_GAUGE_SIZE, 0.0f);

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

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE, g_aBeam[1].pos.y - BEAM_GAUGE_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE + 1, g_aBeam[1].pos.y - BEAM_GAUGE_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE, g_aBeam[1].pos.y + BEAM_GAUGE_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBeam[1].pos.x - BEAM_GAUGE_SIZE + 1, g_aBeam[1].pos.y + BEAM_GAUGE_SIZE, 0.0f);

	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//pVtx += 4;	//頂点データのポインタを4つ進める

	// 頂点バッファをアンロックする
	g_pVtxBuffBeam->Unlock();
}

//=============================================================================
// ビームの加算
//=============================================================================
void AddBeam(int nValue)
{
	g_aBeam[1].nCntGauge += nValue;
}

//=============================================================================
// ビームの取得
//=============================================================================
BEAM *GetBeam(void)
{
	return &g_aBeam[1];
}