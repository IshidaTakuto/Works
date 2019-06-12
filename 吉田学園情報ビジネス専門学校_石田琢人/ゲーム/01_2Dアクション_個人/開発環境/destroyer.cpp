//=============================================================================
//
// サメの処理 [destroyer.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "destroyer.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_DESTROYER		"data/TEXTURE/shark.png"	// 読み込むテクスチャファイル名
#define DESTROYER_SIZE_X	(200)
#define DESTROYER_SIZE_Y	(150)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureDestroyer = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDestroyer = NULL;	//頂点バッファへのポインタ
DESTROYER g_Destroyer;
int g_nCntDestroyer;
float g_fSpeed;

//=============================================================================
// 初期化処理
//=============================================================================
void InitDestroyer(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//背景の情報の初期化
	g_Destroyer.pos = D3DXVECTOR3(-DESTROYER_SIZE_X, SCREEN_HEIGHT + DESTROYER_SIZE_Y, 0.0f);
	g_Destroyer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Destroyer.state = DESTROYERSTATE_NONE;

	g_nCntDestroyer = 0;
	g_fSpeed = 1;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_DESTROYER, &g_pTextureDestroyer);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDestroyer,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffDestroyer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_Destroyer.pos.x - DESTROYER_SIZE_X, g_Destroyer.pos.y - DESTROYER_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Destroyer.pos.x + DESTROYER_SIZE_X, g_Destroyer.pos.y - DESTROYER_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Destroyer.pos.x - DESTROYER_SIZE_X, g_Destroyer.pos.y + DESTROYER_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Destroyer.pos.x + DESTROYER_SIZE_X, g_Destroyer.pos.y + DESTROYER_SIZE_Y, 0.0f);

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

	//頂点バッファをアンロック
	g_pVtxBuffDestroyer->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDestroyer(void)
{
	//テクスチャの破棄
	if (g_pTextureDestroyer != NULL)
	{
		g_pTextureDestroyer->Release();
		g_pTextureDestroyer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffDestroyer != NULL)
	{
		g_pVtxBuffDestroyer->Release();
		g_pVtxBuffDestroyer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDestroyer(void)
{
	PLAYER pPlayer;
	pPlayer = GetPlayer();

	g_nCntDestroyer++;

	switch (g_Destroyer.state)
	{
	case DESTROYERSTATE_NORMAL:
		g_Destroyer.pos.y += 0.6f * g_fSpeed;
		if (g_nCntDestroyer % 90 == 0)
		{
			g_fSpeed = g_fSpeed * -1;
		}

		break;

	case DESTROYERSTATE_MOVE:
		if(g_Destroyer.pos.x - DESTROYER_SIZE_X < 0 && g_Destroyer.pos.y + DESTROYER_SIZE_Y > SCREEN_HEIGHT + 10)
		{
			g_Destroyer.move.x += 0.06f;
			g_Destroyer.move.y -= 0.04f;

			g_Destroyer.pos.y += g_Destroyer.move.y;
			g_Destroyer.pos.x += g_Destroyer.move.x;

		}
		else
		{// 移動終了
 			g_Destroyer.state = DESTROYERSTATE_NORMAL;
		}
		break;
	}

	if (pPlayer.pos.x >= SCREEN_WIDTH / 3 && g_Destroyer.state == DESTROYERSTATE_NONE)
	{
 		g_Destroyer.state = DESTROYERSTATE_MOVE;
	}

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffDestroyer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_Destroyer.pos.x - DESTROYER_SIZE_X, g_Destroyer.pos.y - DESTROYER_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Destroyer.pos.x + DESTROYER_SIZE_X, g_Destroyer.pos.y - DESTROYER_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Destroyer.pos.x - DESTROYER_SIZE_X, g_Destroyer.pos.y + DESTROYER_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Destroyer.pos.x + DESTROYER_SIZE_X, g_Destroyer.pos.y + DESTROYER_SIZE_Y, 0.0f);

	//頂点バッファをアンロック
	g_pVtxBuffDestroyer->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDestroyer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffDestroyer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureDestroyer);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// デストロイヤーの当たり判定処理
//=============================================================================
bool CollisionDestroyer(D3DXVECTOR3 *pPos, float pHeight, float pWidth)
{
	bool bTouch = false;

	if (pPos->x>= g_Destroyer.pos.x - DESTROYER_SIZE_X && pPos->x <= g_Destroyer.pos.x + DESTROYER_SIZE_X)
	{// 幅の範囲内
		if (pPos->y > g_Destroyer.pos.y - DESTROYER_SIZE_Y && pPos->y <= g_Destroyer.pos.y - DESTROYER_SIZE_Y ||
			pPos->y > g_Destroyer.pos.y - DESTROYER_SIZE_Y && pPos->y <= g_Destroyer.pos.y + DESTROYER_SIZE_Y ||
			pPos->y - pHeight >= g_Destroyer.pos.y - DESTROYER_SIZE_Y && pPos->y - pHeight < g_Destroyer.pos.y + DESTROYER_SIZE_Y ||
			pPos->y - pHeight <= g_Destroyer.pos.y - DESTROYER_SIZE_Y && pPos->y > g_Destroyer.pos.y + DESTROYER_SIZE_Y
			)
		{// 上と横の判定
			bTouch = true;
		}
	}
	return bTouch;
}


DESTROYER GetDestroyer(void)
{
	return g_Destroyer;
}