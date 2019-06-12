//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "game.h"
#include "sound.h"
#include "stock.h"
#include "destroyer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_TEXTURENAME	"data/TEXTURE/player001.png"	// テクスチャ名
#define PLAYER_SIZE		(30)	// プレイヤーの大きさ
#define JUMP_HEIGHT		(-30)	// ジャンプの高さ
#define GRAVITY			(2)		// 重力
#define ANIM_SPEED		(6)		// アニメーションの移動速度
#define TEX_UPDATE		((g_Player.nCounterAnim % 4) * 0.25f)	// テクスチャX座標の更新
#define TEX_PATTERN		(4)	// テクスチャのパターン数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//頂点バッファへのポインタ
PLAYER g_Player;
int g_nCntStock;

//=============================================================================
// プレイヤー初期化処理
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// プレイヤーの情報の初期化
	g_Player.pos = D3DXVECTOR3(60, 400, 0.0f);
	g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.fHeight = PLAYER_SIZE * 2;
	g_Player.fWidth = PLAYER_SIZE / 2;
	g_Player.nCounterAnim = 0;
	g_Player.nPatternAnim = 0;
	g_Player.nDirectionMove = 1;
	g_Player.bJump = false;
	g_Player.bDisp = true;
	g_Player.nCounterState = 0;
	g_Player.BlockOld.type = BLOCKTYPE_NORMAL;
	g_Player.state = PLAYERSTATE_NORMAL;
	g_Player.pBlock = NULL;

	g_nCntStock = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEXTURENAME, &g_pTexturePlayer);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_SIZE, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_SIZE, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_SIZE, g_Player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_SIZE, g_Player.pos.y, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);

	//頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================
// プレイヤー終了処理
//=============================================================================
void UninitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=============================================================================
// プレイヤー更新処理
//=============================================================================
void UpdatePlayer(void)
{
	g_Player.posOld = g_Player.pos;	// 前回の位置を保存

	switch (g_Player.state)
	{
	case PLAYERSTATE_NONE:
		break;

	case PLAYERSTATE_APPEAR:
		g_Player.nCounterState++;
		if (g_Player.nCounterState >= 60)
		{
			g_Player.nCounterState = 0;
			g_Player.bDisp = true;
			// 水面から出てくる
			g_Player.move.y = JUMP_HEIGHT * 3.15f;
			
			// 一定地点で止まる
			if (g_Player.pos.y <= 350)
			{
				g_Player.move.y = 0;
				g_Player.state = PLAYERSTATE_NORMAL;
			}
		}
		break;

	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_DEATH:
		g_Player.bDisp = false;
		g_Player.state = PLAYERSTATE_NONE;
		PlaySound(SOUND_LABEL_SE_SCREAM000);
		SetGameState(GAMESTATE_END);
		return;
		break;

	case PLAYERSTATE_CLEAR:
		g_Player.state = PLAYERSTATE_NONE;
		SetGameState(GAMESTATE_CLEAR);
		return;
		break;
	}

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	if (GetKeyboardPress(DIK_D) == true && g_Player.pos.y <= SCREEN_HEIGHT)
	{// 右移動
		g_Player.move.x += (g_Player.move.x + MOVE_SPEED) * 0.9f;
		g_Player.nDirectionMove = 1; // 右向き状態
	}

	if (GetKeyboardPress(DIK_A) == true && g_Player.pos.y <= SCREEN_HEIGHT)
	{// 左移動
		g_Player.move.x += (g_Player.move.x - MOVE_SPEED) * 0.9f;
		g_Player.nDirectionMove = 0; // 左向き状態
	}

	if (g_Player.move.x < 0 || g_Player.move.x > 0)
	{// プレイヤー移動時
		VERTEX_2D *pVtx;	//頂点情報へのポインタ

		g_Player.nCounterAnim++;

		if ((g_Player.nCounterAnim % ANIM_SPEED) == 0)
		{// 移動時にテクスチャを動かす
			g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % TEX_PATTERN;	//パターンNo.更新

			//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//テクスチャ座標
			pVtx[0].tex.x = TEX_UPDATE;
			pVtx[1].tex.x = TEX_UPDATE + 0.25f;
			pVtx[2].tex.x = TEX_UPDATE;
			pVtx[3].tex.x = TEX_UPDATE + 0.25f;

			//頂点バッファをアンロック
			g_pVtxBuffPlayer->Unlock();
		}
	}
	else
	{// 直立
		if (g_Player.nPatternAnim == 1 || g_Player.nPatternAnim == 3)
		{// 直立時のテクスチャにそろえる
			g_Player.nCounterAnim++;

			if ((g_Player.nCounterAnim % ANIM_SPEED) == 0)
			{
				g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % TEX_PATTERN;	//パターンNo.更新

				//頂点バッファをロックし、頂点データへのポインタを取得
				g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

				//テクスチャ座標
				pVtx[0].tex.x = TEX_UPDATE;
				pVtx[1].tex.x = TEX_UPDATE + 0.25f;
				pVtx[2].tex.x = TEX_UPDATE;
				pVtx[3].tex.x = TEX_UPDATE + 0.25f;

				//頂点バッファをアンロック
				g_pVtxBuffPlayer->Unlock();
			}
		}
	}

	// ジャンプ
	if (GetKeyboardTrigger(DIK_SPACE) == true && g_Player.bJump == false)
	{
		PlaySound(SOUND_LABEL_SE_JUMP);	// ジャンプ音

		// ジャンプ声をランダムにする
		if (rand() % 2 == 0)
		{// 声１
			PlaySound(SOUND_LABEL_SE_JUMP_VOICE000);
		}
		else if (rand() % 2 == 1)
		{// 声２
			PlaySound(SOUND_LABEL_SE_JUMP_VOICE001);
		}

		g_Player.bJump = true;	// ジャンプ状態に
		g_Player.move.y = JUMP_HEIGHT;
	}

	g_Player.move.y += GRAVITY;	// 重力加算

	g_Player.pos.x += g_Player.move.x;	// 横移動
	g_Player.pos.y += g_Player.move.y;	// 縦移動

	g_Player.move.x = 0;	// 加速を抑える

	// 画面端処理
	if (g_Player.pos.x < -30)
	{// 左端へ行ったとき
		g_Player.pos.x = SCREEN_WIDTH + 30;		// 画面外に行かないようにする
	}
	else if (g_Player.pos.x > SCREEN_WIDTH + 30)
	{// 右端へ行ったとき
		g_Player.pos.x = -30;					// 画面外に行かないようにする
	}

	if (g_Player.pos.y - g_Player.fHeight >= SCREEN_HEIGHT && g_Player.state == PLAYERSTATE_NORMAL)
	{// 通常時のみ落下死
		g_Player.bDisp = false;
		PlaySound(SOUND_LABEL_SE_SPLASH);
		g_Player.move.y = 0.0f;

		g_nCntStock++;
		if (g_nCntStock < NUM_STOCK + 1)
		{// 残機を１減らす
			SubtractionStock(g_nCntStock);
			g_Player.state = PLAYERSTATE_APPEAR;		
		}
		else
		{// ゲームオーバー
			g_Player.state = PLAYERSTATE_DEATH;
		}
	}

	// アイテム取得処理
	CollisionItem();

	g_Player.pBlock = NULL;

	// ブロックの当たり判定
	if (g_Player.state == PLAYERSTATE_NORMAL)
	{
		if (CollisionBlock(&g_Player.pos, &g_Player.posOld, &g_Player.move, &g_Player.fHeight, &g_Player.fWidth) == true)
		{
			g_Player.bJump = false;	// ジャンプフラグを落とす
		}
		else
		{
			g_Player.bJump = true;	// ジャンプフラグを立てる

			if (g_Player.nPatternAnim == 0 || g_Player.nPatternAnim == 2)
			{// 空中時のテクスチャにそろえる
				g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % TEX_PATTERN;	// パターンNo.更新

				// 頂点バッファをロックし、頂点データへのポインタを取得
				g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

				// テクスチャ座標
				pVtx[0].tex.x = TEX_UPDATE;
				pVtx[1].tex.x = TEX_UPDATE + 0.25f;
				pVtx[2].tex.x = TEX_UPDATE;
				pVtx[3].tex.x = TEX_UPDATE + 0.25f;

				//頂点バッファをアンロック
				g_pVtxBuffPlayer->Unlock();
			}
		}
	}

	if (CollisionDestroyer(&g_Player.pos, g_Player.fHeight, g_Player.fWidth) == true && g_Player.state == PLAYERSTATE_NORMAL)
	{// サメとぶつかった
		SetBlood();
		g_Player.state = PLAYERSTATE_DEATH;	
	}

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_SIZE, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_SIZE, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_SIZE, g_Player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_SIZE, g_Player.pos.y, 0.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(g_Player.nPatternAnim * 0.25f, g_Player.nDirectionMove * 0.5f);
	pVtx[1].tex = D3DXVECTOR2(g_Player.nPatternAnim * 0.25f + 0.25f, g_Player.nDirectionMove * 0.5f);
	pVtx[2].tex = D3DXVECTOR2(g_Player.nPatternAnim * 0.25f + 0.0f, (g_Player.nDirectionMove * 0.5f) + 0.5f);
	pVtx[3].tex = D3DXVECTOR2(g_Player.nPatternAnim * 0.25f + 0.25f, (g_Player.nDirectionMove * 0.5f) + 0.5f);

	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================
// プレイヤー描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	if (g_Player.bDisp == true)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
PLAYER GetPlayer(void)
{
	return g_Player;	// プレイヤーのデータを返す
}

//=============================================================================
// アイテムの当たり判定処理
//=============================================================================
void CollisionItem(void)
{
	int nCntItem;

	ITEM *pItem;
	pItem = GetItem();

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == true)
		{// 使用されている
			if (g_Player.pos.x + g_Player.fWidth >= pItem->pos.x - pItem->fWidth
				&& g_Player.pos.x - g_Player.fWidth <= pItem->pos.x + pItem->fWidth 
				&& g_Player.pos.y - g_Player.fHeight / 2 <= pItem->pos.y + pItem->fHeight 
				&& g_Player.pos.y >= pItem->pos.y - pItem->fHeight)
			{// 当たり判定
				pItem->state = ITEMSTATE_GET;
			}
		}
	}
}

