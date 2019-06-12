//=============================================================================
//
// ブロック処理 [block.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "block.h"
#include "game.h"
#include "sound.h"
#include "block.h"
#include "destroyer.h"
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BLOCK_TEXTURENAME0	"data/TEXTURE/bar.png"
#define BLOCK_TEXTURENAME1	"data/TEXTURE/bard000.png"
#define BLOCK_TEXTURENAME2	"data/TEXTURE/bar001.png"
#define BLOCK_TEXTURENAME3	"data/TEXTURE/dorsal.png"
#define BLOCK_DRAW		(200)	// ブロックを描画する範囲
#define MAX_TEX			(4)		// テクスチャの数
#define ANIM_SPEED		(7)		// テクスチャアニメーションの速度
#define TEX_PATTERN		(8)		// テクスチャアニメーションのパターン数
#define GOAL_HEIGHT		(25)	// ゴールの鳥の背中の高さ

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBlock[MAX_TEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;		// 頂点バッファへのポインタ
BLOCK g_aBlock[MAX_BLOCK];	//ブロックの情報
int g_nCntMove;		// 鳥の追ってくる時間

//=============================================================================
// 初期化処理
//=============================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;

	// デバイスの取得
	pDevice = GetDevice();

	// ブロックの情報の初期化
	for(nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].des = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].fWidth = 0.0f;
		g_aBlock[nCntBlock].fHeight = 0.0f;
		g_aBlock[nCntBlock].nCounterAnim = 0;
		g_aBlock[nCntBlock].nPatternAnim = 0;
		g_aBlock[nCntBlock].nDirectionMove = 0;
		g_aBlock[nCntBlock].type = BLOCKTYPE_NORMAL;
		g_aBlock[nCntBlock].state = BLOCKSTATE_NORMAL;
		g_aBlock[nCntBlock].nCounterState = 0;
		g_aBlock[nCntBlock].nCntMove = 0;
		g_aBlock[nCntBlock].nType = 0;
		g_aBlock[nCntBlock].nVertical = 0;
		g_aBlock[nCntBlock].bUse = false;
		g_aBlock[nCntBlock].bScroll = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME0, &g_pTextureBlock[0]);
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME1, &g_pTextureBlock[1]);
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME2, &g_pTextureBlock[2]);
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME3, &g_pTextureBlock[3]);

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
									D3DUSAGE_WRITEONLY,
									FVF_VERTEX_2D,
									D3DPOOL_MANAGED,
									&g_pVtxBuffBlock,
									NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);

		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBlock(void)
{
	for (int nCntBlock = 0; nCntBlock < MAX_TEX; nCntBlock++)
	{
		// テクスチャの開放
		if (g_pTextureBlock[nCntBlock] != NULL)
		{
			g_pTextureBlock[nCntBlock]->Release();
			g_pTextureBlock[nCntBlock] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBlock(void)
{
	int nCntBlock;
	PLAYER pPlayer;
	pPlayer = GetPlayer();

	VERTEX_2D *pVtx;		// 頂点情報の作成

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// 位置の保存
		g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			switch (g_aBlock[nCntBlock].state)
			{
			case BLOCKSTATE_NONE:
				break;

			case BLOCKSTATE_NORMAL:
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_ENEMY || g_aBlock[nCntBlock].type == BLOCKTYPE_HORIZONTAL)
				{
					g_aBlock[nCntBlock].bUse = false;
				}
				break;

			case BLOCKSTATE_MOVE:
				if (g_aBlock[nCntBlock].type != BLOCKTYPE_HORIZONTAL)
				{
					g_aBlock[nCntBlock].bScroll = true;
				}
				break;

			case BLOCKSTATE_CLEAR:
				g_aBlock[nCntBlock].nCounterState++;
				for (int nCntBlock2 = 0; nCntBlock2 < MAX_BLOCK; nCntBlock2++)
				{
					if (g_aBlock[nCntBlock2].bUse == true && g_aBlock[nCntBlock2].nType == 1)
					{// 鳥を上げる
						g_aBlock[nCntBlock2].move.x = 0;
						g_aBlock[nCntBlock2].pos.y -= 5.5f;
					}
				}
					PlaySound(SOUND_LABEL_SE_FLY);
				if (g_aBlock[nCntBlock].nCounterState % 60 == 0)
				{// クリア画面へ
					PlaySound(SOUND_LABEL_SE_BIRD);
					g_aBlock[nCntBlock].state = BLOCKSTATE_NONE;
					SetGameState(GAMESTATE_CLEAR);
				}
				break;
			}

			switch (g_aBlock[nCntBlock].type)
			{
			case BLOCKTYPE_NORMAL:		// 通常
				if (g_aBlock[nCntBlock].pos.x < 0 - BLOCK_SIZE)
				{
					g_aBlock[nCntBlock].bUse = false;
				}

				g_aBlock[nCntBlock].nType = 1;	// 鳥

				break;

			case BLOCKTYPE_VERTICAL:	// 垂直移動
				if (g_aBlock[nCntBlock].pos.y >= g_aBlock[nCntBlock].des.y)
				{// 上移動
					g_aBlock[nCntBlock].move.y -= 1 * 0.02f;
				}
				else if (g_aBlock[nCntBlock].pos.y <= g_aBlock[nCntBlock].posOld.y)
				{// 下移動
					g_aBlock[nCntBlock].move.y += 1 * 0.02f;
				}

				if (g_aBlock[nCntBlock].pos.y >= SCREEN_HEIGHT || g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth <= 0)
				{// 画面外に行ったとき
					g_aBlock[nCntBlock].bUse = false;
				}

				g_aBlock[nCntBlock].pos.y += g_aBlock[nCntBlock].move.y;

				g_aBlock[nCntBlock].nType = 1;	// 鳥

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				break;

			case BLOCKTYPE_HORIZONTAL:	// 水平移動
				if (g_aBlock[nCntBlock].pos.x >= g_aBlock[nCntBlock].des.x)
				{// 左移動
					g_aBlock[nCntBlock].move.x -= 1 * 0.02f;
				}
				else if (g_aBlock[nCntBlock].pos.x <= g_aBlock[nCntBlock].des.x)
				{// 右移動
					g_aBlock[nCntBlock].move.x += 1 * 0.02f;
				}

				if (g_aBlock[nCntBlock].pos.x <= -BLOCK_DRAW || g_aBlock[nCntBlock].pos.x >= SCREEN_WIDTH + BLOCK_DRAW)
				{// 画面外に行ったとき
					g_aBlock[nCntBlock].bUse = false;
				}

				g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;

				g_aBlock[nCntBlock].nType = 1;	// 鳥

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				break;

			case BLOCKTYPE_TITLE:
				if (g_aBlock[nCntBlock].pos.x >= g_aBlock[nCntBlock].des.x)
				{// 左移動
					g_aBlock[nCntBlock].move.x -= 1 * 0.02f;
				}
				else if (g_aBlock[nCntBlock].pos.x <= g_aBlock[nCntBlock].des.x)
				{// 右移動
					g_aBlock[nCntBlock].move.x += 1 * 0.02f;
				}

				if (g_aBlock[nCntBlock].pos.x <= -BLOCK_DRAW || g_aBlock[nCntBlock].pos.x >= SCREEN_WIDTH + BLOCK_DRAW)
				{// 画面外に行ったとき
					g_aBlock[nCntBlock].bUse = false;
				}

				g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;

				g_aBlock[nCntBlock].nType = 1;	// 鳥

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				break;

			case BLOCKTYPE_BELT_LEFT:	// ベルトコンベアー(左向き)
					// ブロックの移動
				g_aBlock[nCntBlock].move.x -= (1 + g_aBlock[nCntBlock].move.x) * 0.01f;
				g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;

				if (g_aBlock[nCntBlock].pos.y >= g_aBlock[nCntBlock].des.y)
				{// 上移動
					g_aBlock[nCntBlock].move.y -= 1 * 0.02f;
				}
				else if (g_aBlock[nCntBlock].pos.y <= g_aBlock[nCntBlock].posOld.y)
				{// 下移動
					g_aBlock[nCntBlock].move.y += 1 * 0.02f;
				}

				if (g_aBlock[nCntBlock].pos.y <= 0)
				{// 消す
					g_aBlock[nCntBlock].bUse = false;
				}

				g_aBlock[nCntBlock].pos.y += g_aBlock[nCntBlock].move.y;

				g_aBlock[nCntBlock].nType = 0;	// 木

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;

			case BLOCKTYPE_BELT_RIGHT:	// ベルトコンベアー(右向き)				
					// ブロックの移動
				g_aBlock[nCntBlock].move.x -= (1 + g_aBlock[nCntBlock].move.x) * 0.01f;
				g_aBlock[nCntBlock].pos.x -= g_aBlock[nCntBlock].move.x;

				if (g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth > SCREEN_WIDTH / 3 - 50)
				{// 真ん中あたりについたとき
					g_aBlock[nCntBlock].pos.y += 1;
					if (g_aBlock[nCntBlock].pos.y >= SCREEN_HEIGHT)
					{
						g_aBlock[nCntBlock].bUse = false;
					}
				}

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
				break;

			case BLOCKTYPE_FALL:	// 落下床
				if (g_aBlock[nCntBlock].pos.y >= SCREEN_HEIGHT || g_aBlock[nCntBlock].pos.x < -200)
				{
					g_aBlock[nCntBlock].bUse = false;
				}

				g_aBlock[nCntBlock].nType = 1;	// 鳥

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				break;

			case BLOCKTYPE_GOAL:		// ゴール
				g_aBlock[nCntBlock].nType = 1;	// 鳥
				g_aBlock[nCntBlock].nDirectionMove = 1;

				if (g_aBlock[nCntBlock].pos.x <= 1000)
				{// 一定地点で止まる
					g_aBlock[nCntBlock].bScroll = false;
				}

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.7f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.7f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.7f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.7f, 1.0f, 0.0f, 1.0f);
				break;

			case BLOCKTYPE_SHARK:
				if (g_aBlock[nCntBlock].pos.x >= g_aBlock[nCntBlock].des.x)
				{// 左移動
					g_aBlock[nCntBlock].move.x -= 1 * 0.02f;
				}
				else if (g_aBlock[nCntBlock].pos.x <= g_aBlock[nCntBlock].des.x)
				{// 右移動
					g_aBlock[nCntBlock].move.x += 1 * 0.02f;
				}

				if (g_aBlock[nCntBlock].pos.y >= g_aBlock[nCntBlock].des.y)
				{// 上移動
					g_aBlock[nCntBlock].move.y -= 1 * 0.02f;
				}
				else if (g_aBlock[nCntBlock].pos.y <= g_aBlock[nCntBlock].posOld.y)
				{// 下移動
					g_aBlock[nCntBlock].move.y += 1 * 0.02f;
				}

				if (g_aBlock[nCntBlock].pos.x <= -BLOCK_DRAW || g_aBlock[nCntBlock].pos.x >= SCREEN_WIDTH + BLOCK_DRAW)
				{// 画面外に行ったとき
					g_aBlock[nCntBlock].bUse = false;
				}

				g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;
				g_aBlock[nCntBlock].pos.y += g_aBlock[nCntBlock].move.y;

				g_aBlock[nCntBlock].nType = 3;	// 背びれ

				// テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, g_aBlock[nCntBlock].nDirectionMove * 0.5f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, g_aBlock[nCntBlock].nDirectionMove * 0.5f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, g_aBlock[nCntBlock].nDirectionMove * 0.5f + 0.5f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, g_aBlock[nCntBlock].nDirectionMove * 0.5f + 0.5f);

				break;

			case BLOCKTYPE_ENEMY:	// 突っ込んでくる
				static float fAngle;
				D3DXVECTOR3 BlockMove(0.0f, 0.0f, 0.0f);

				g_aBlock[nCntBlock].nCntMove++;
				if (g_aBlock[nCntBlock].state != BLOCKSTATE_CLEAR)
				{
					if (g_aBlock[nCntBlock].nCntMove <= 60)
					{// 追ってくる時間
						fAngle = atan2f(pPlayer.pos.x - g_aBlock[nCntBlock].pos.x, pPlayer.pos.y - g_aBlock[nCntBlock].pos.y);

						BlockMove.x = sinf(fAngle) * 5.0f;
						BlockMove.y = cosf(fAngle) * 5.0f;

						g_aBlock[nCntBlock].move = BlockMove;
					}
				}
				else
				{
					g_aBlock[nCntBlock].move.y -= 15.0f;
				}

				if (g_aBlock[nCntBlock].pos.x <= -BLOCK_DRAW
					&& g_aBlock[nCntBlock].pos.x >= SCREEN_WIDTH + BLOCK_DRAW
					&& g_aBlock[nCntBlock].pos.y <= -BLOCK_DRAW
					&& g_aBlock[nCntBlock].pos.y >= SCREEN_HEIGHT + BLOCK_DRAW)
				{
					g_aBlock[nCntBlock].bUse = false;
				}

				if (g_aBlock[nCntBlock].pos.x <= pPlayer.pos.x)
				{// プレイヤーより左側で右向き
					g_aBlock[nCntBlock].nDirectionMove = 0;
				}
				else
				{// プレイヤーより右側で左向き
					g_aBlock[nCntBlock].nDirectionMove = 1;
				}
				g_aBlock[nCntBlock].nType = 1;	// 鳥

				g_aBlock[nCntBlock].pos += g_aBlock[nCntBlock].move;

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				break;
			}

			if (pPlayer.pos.x >= SCREEN_WIDTH / 3 && g_aBlock[nCntBlock].state == BLOCKSTATE_NORMAL)
			{// 強制スクロール
				g_aBlock[nCntBlock].state = BLOCKSTATE_MOVE;
			}
			else if (g_aBlock[nCntBlock].type == BLOCKTYPE_GOAL && g_aBlock[nCntBlock].pos.x <= 1000)
			{
				g_aBlock[nCntBlock].state = BLOCKSTATE_NONE;
			}

			if (g_aBlock[nCntBlock].bScroll == true)
			{// 画面スクロール
				g_aBlock[nCntBlock].pos.x -= SCROLL_SPEED;
			}

			g_aBlock[nCntBlock].nCounterAnim++;

			if (g_aBlock[nCntBlock].nType == 1)
			{
				if ((g_aBlock[nCntBlock].nCounterAnim % ANIM_SPEED) == 0)
				{// テクスチャを動かす
					g_aBlock[nCntBlock].nPatternAnim = (g_aBlock[nCntBlock].nPatternAnim + 1) % TEX_PATTERN;	//パターンNo.更新

					// テクスチャ座標
					pVtx[0].tex = D3DXVECTOR2((g_aBlock[nCntBlock].nPatternAnim % 8) * 0.125f, g_aBlock[nCntBlock].nDirectionMove * 0.5f);
					pVtx[1].tex = D3DXVECTOR2((g_aBlock[nCntBlock].nPatternAnim % 8) * 0.125f + 0.125f, g_aBlock[nCntBlock].nDirectionMove * 0.5f);
					pVtx[2].tex = D3DXVECTOR2((g_aBlock[nCntBlock].nPatternAnim % 8) * 0.125f, g_aBlock[nCntBlock].nDirectionMove * 0.5f + 0.5f);
					pVtx[3].tex = D3DXVECTOR2((g_aBlock[nCntBlock].nPatternAnim % 8) * 0.125f + 0.125f, g_aBlock[nCntBlock].nDirectionMove * 0.5f + 0.5f);
				}
			}

			if (CollisionDestroyer(&g_aBlock[nCntBlock].pos, g_aBlock[nCntBlock].fHeight, g_aBlock[nCntBlock].fWidth) == true)
			{// デストロイヤーに触れた時
				if (g_aBlock[nCntBlock].nType == 0)
				{
					g_aBlock[nCntBlock].nType = 2;
				}
				else if (g_aBlock[nCntBlock].nType == 0 && g_aBlock[nCntBlock].type != BLOCKTYPE_HORIZONTAL)
				{
					g_aBlock[nCntBlock].bUse = false;
				}
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
		}
		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	
	// 頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for(int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{//ブロックが使用されている
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBlock[g_aBlock[nCntBlock].nType]);

			if (g_aBlock[nCntBlock].pos.x >= -BLOCK_DRAW
				&& g_aBlock[nCntBlock].pos.x <= SCREEN_WIDTH + BLOCK_DRAW
				&& g_aBlock[nCntBlock].pos.y >= -BLOCK_DRAW
				&& g_aBlock[nCntBlock].pos.y <= SCREEN_HEIGHT + BLOCK_DRAW)
			{
				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBlock, 2);
			}
		}
	}
}

//=============================================================================
// ブロックの設定
//=============================================================================
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXVECTOR3 des, BLOCKTYPE type)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntBlock;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{//ブロックが使用されていない
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f,0.0f,0.0f);
			g_aBlock[nCntBlock].fWidth = fWidth;
			g_aBlock[nCntBlock].fHeight = fHeight;
			g_aBlock[nCntBlock].des = des;
			g_aBlock[nCntBlock].type = type;
			g_aBlock[nCntBlock].bUse = true;	//使用している状態にする

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + fWidth, g_aBlock[nCntBlock].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + fWidth, g_aBlock[nCntBlock].pos.y + fHeight, 0.0f);

			if (g_aBlock[nCntBlock].nType == -100)
			{// テクスチャを分割して貼る
				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aBlock[nCntBlock].fWidth / BLOCK_SIZE, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, g_aBlock[nCntBlock].fHeight / BLOCK_SIZE);
				pVtx[3].tex = D3DXVECTOR2(g_aBlock[nCntBlock].fWidth / BLOCK_SIZE, g_aBlock[nCntBlock].fHeight / BLOCK_SIZE);
			}
			else
			{
				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			if (g_aBlock[nCntBlock].pos.x > SCREEN_WIDTH / 2)
			{// 画面半分より右側
				g_aBlock[nCntBlock].nDirectionMove = 1;
			}
			else if (g_aBlock[nCntBlock].pos.x < SCREEN_WIDTH / 2)
			{// 画面半分より左側
				g_aBlock[nCntBlock].nDirectionMove = 0;
			}

				break;
		}
		pVtx += 4;
	}
	
	//頂点バッファをアンロック
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// ブロックの取得
//=============================================================================
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}

//=============================================================================
// ブロックの当たり判定処理
//=============================================================================
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float *pHeight, float *pWidth)
{
	int nCntBlock;
	bool bLand = false;

	// プレイヤーの情報を取得
	PLAYER pPlayer;
	pPlayer = GetPlayer();

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{// 使用されている間
			if (pPos->x + *pWidth >= g_aBlock[nCntBlock].pos.x && pPos->x - *pWidth <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth)
			{// ブロックの幅の範囲内
				if (pPos->y > g_aBlock[nCntBlock].pos.y && pPosOld->y <= g_aBlock[nCntBlock].posOld.y)
				{// 上から当たった時
					bLand = true;
					pPos->y = g_aBlock[nCntBlock].pos.y;
					pMove->y = 0.0f;
					pPlayer.BlockOld = g_aBlock[nCntBlock];	// 最後に乗ったブロックを保存

					if (g_aBlock[nCntBlock].bScroll == true)
					{// スクロール移動
						pPos->x -= SCROLL_SPEED;
					}

					if (g_aBlock[nCntBlock].move.y <= -5.0f)
					{// 縦の移動についていく
						pPos->y = g_aBlock[nCntBlock].pos.y - 5.0f;
					}
					if (g_aBlock[nCntBlock].move.y >= 0.0f)
					{// 縦の移動についていく
						pPos->y = g_aBlock[nCntBlock].pos.y;
					}

					if (g_aBlock[nCntBlock].type == BLOCKTYPE_HORIZONTAL)
					{// 水平移動ブロック
						pPos->x += g_aBlock[nCntBlock].move.x;
					}
					if (g_aBlock[nCntBlock].type == BLOCKTYPE_BELT_LEFT)
					{// ベルトコンベアー(左向き)
						pPos->x += g_aBlock[nCntBlock].move.x;
						if (g_aBlock[nCntBlock].pos.y < g_aBlock[nCntBlock].des.y
							&& g_aBlock[nCntBlock].pos.y < 680)
						{// 沈ませる
							g_aBlock[nCntBlock].move.y += 1 * 0.05f;
						}
					}
					if (g_aBlock[nCntBlock].type == BLOCKTYPE_BELT_RIGHT)
					{// ベルトコンベアー(右向き)
						pPos->x -= g_aBlock[nCntBlock].move.x;
					}
					if (g_aBlock[nCntBlock].type == BLOCKTYPE_FALL)
					{// 落下床
						if (g_aBlock[nCntBlock].pos.y < 700)
						{
							g_aBlock[nCntBlock].pos.y++;
						}
					}
					if (g_aBlock[nCntBlock].type == BLOCKTYPE_GOAL)
					{// ゴール
						pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2;
						pMove->y = 0.0f;
						bLand = false;
						if (g_aBlock[nCntBlock].state == BLOCKSTATE_NONE)
						{// クリア
							g_aBlock[nCntBlock].state = BLOCKSTATE_CLEAR;
						}
					}
					if (g_aBlock[nCntBlock].type == BLOCKTYPE_ENEMY)
					{// 突っ込んでくるやつ
						pMove->y = JUMP_HEIGHT;
						PlaySound(SOUND_LABEL_SE_SCORE_UP);
						AddScore(4500);
						g_aBlock[nCntBlock].pos.y += 25.0f;
					}
				}
				else if (pPos->y - *pHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight && pPosOld->y - *pHeight > g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
				{// 下から当たった時
					pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + *pHeight;
					pMove->y = 0.0f;
				}
				else if (pPos->y > g_aBlock[nCntBlock].pos.y && pPos->y <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight ||					// 足元が範囲内
					pPos->y - *pHeight >= g_aBlock[nCntBlock].pos.y && pPos->y - *pHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight ||	// 頭が範囲内
					pPos->y - *pHeight <= g_aBlock[nCntBlock].pos.y && pPos->y > g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)				// ブロックが頭と足の間にある
				{// 横から当たった時
					if (pPos->x < g_aBlock[nCntBlock].pos.x + 10)
					{// 左から
						pPos->x = g_aBlock[nCntBlock].pos.x - *pWidth;
						pMove->x = 0.0f;
					}
					else if (pPos->x > g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2)
					{// 右から
						pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + *pWidth;
						pMove->x = 0.0f;
					}
				}
			}

			if (bLand == false)
			{// ブロックから離れた時の処理
				if (pPlayer.BlockOld.type == BLOCKTYPE_HORIZONTAL)
				{// 水平移動ブロック
					if (pPlayer.BlockOld.move.x < 0)
					{// 左移動
						pPlayer.pos.x += pPlayer.BlockOld.move.x * 0.015f;
					}
					else if(pPlayer.BlockOld.move.x > 0)
					{// 右移動
						pPlayer.pos.x += pPlayer.BlockOld.move.x * 0.015f;
					}
				}
				else if (pPlayer.BlockOld.type == BLOCKTYPE_BELT_LEFT)
				{// 左向きベルトコンベアー
					pPlayer.pos.x += g_aBlock[nCntBlock].move.x * 0.08f;
				}
				else if (pPlayer.BlockOld.type == BLOCKTYPE_BELT_RIGHT)
				{// 右向きベルトコンベアー
					pPlayer.pos.x -= g_aBlock[nCntBlock].move.x * 0.08f;
				}
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_FALL || 
					pPlayer.BlockOld.type == BLOCKTYPE_BELT_LEFT)
				{
					if (g_aBlock[nCntBlock].pos.y > g_aBlock[nCntBlock].des.y)
					{// 降りると上がる
						g_aBlock[nCntBlock].pos.y -= 5.0f;
					}
				}
			}
		}
	}
	return bLand;
}
