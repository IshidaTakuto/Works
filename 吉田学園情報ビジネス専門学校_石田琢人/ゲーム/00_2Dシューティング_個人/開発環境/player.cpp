//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "game.h"
#include "sound.h"
#include "life.h"
#include "beam.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_TEXTURENAME	"data/TEXTURE/player000.png"	//テクスチャ名
#define PLAYER_POS_X	(50)	//左上X座標
#define PLAYER_POS_Y	(50)	//左上Y座標
#define PLAYER_WIDTH	(100)	//幅
#define PLAYER_HEIGHT	(100)	//高さ
#define PLAYER_SPEED	(1.5f)		//移動速度
#define PLAYER_MOVE_X	(350.0f)	//移動X座標
#define PLAYER_MOVE_Y	(250.0f)	//移動Y座標
#define PLAYER_SLOWING	(0.1f)		//減速
#define DAMAGE_TIME		(5)	//ダメージ状態の時間
#define PLAYER_LIFE		(5)	//プレイヤーの体力
#define PLAYER_STOCK	(3)	//プレイヤーの残機
#define MAX_GAUGE		(200)	//ビームを撃つのに必要な値

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//頂点バッファへのポインタ
D3DXVECTOR3 g_posPlayer;	//ポリゴンの位置
D3DXVECTOR3 g_movePlayer;	//ポリゴンの移動量
Player g_player;
int g_nMoveX;	//速度に必要
int g_nMoveY;	//速度に必要

//=============================================================================
// プレイヤー初期化処理
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//プレイヤーの情報の初期化
	g_player.bDisp = true;
	g_player.nLife = PLAYER_LIFE;
	g_player.stock = PLAYER_STOCK;
	g_player.state = PLAYERSTATE_NOMAL;
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.nCounterState = 0;
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_posPlayer = D3DXVECTOR3(SCREEN_WIDTH / 2, 400, 0.0f);	//ポリゴンの位置
	g_nMoveX = 0;
	g_nMoveY = 0;


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
	pVtx[0].pos = D3DXVECTOR3(PLAYER_POS_X - 10, PLAYER_POS_Y - 10, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PLAYER_POS_X + 10, PLAYER_POS_Y - 10, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PLAYER_POS_X - 10, PLAYER_POS_Y + 10, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PLAYER_POS_X + 10, PLAYER_POS_Y + 10, 0.0f);

	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

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
	switch (g_player.state)
	{
	case PLAYERSTATE_APPEAR:
		g_player.nCounterState--;
		if (g_player.nCounterState <= 0)
		{
			InitLife();
			g_player.nLife = PLAYER_LIFE;
			g_posPlayer = D3DXVECTOR3(SCREEN_WIDTH / 2, 400, 0.0f);
			g_player.bDisp = true;
			g_player.state = PLAYERSTATE_NOMAL;
			VERTEX_2D *pVtx;	//頂点情報へのポインタ

			//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//頂点バッファをアンロック
			g_pVtxBuffPlayer->Unlock();
		}
		break;

	case PLAYERSTATE_NOMAL:
		break;

	case PLAYERSTATE_DAMAGE:
		g_player.nCounterState--;
		if (g_player.nCounterState <= 0)
		{
			g_player.state = PLAYERSTATE_NOMAL;

			VERTEX_2D *pVtx;	//頂点情報へのポインタ

			//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//頂点バッファをアンロック
			g_pVtxBuffPlayer->Unlock();
		}
		break;

	case PLAYERSTATE_DEATH:
		g_player.state = PLAYERSTATE_NOMAL;
		SetGameState(GAMESTATE_END);
		return;
		break;

	case PLAYERSTATE_BEAM:
		g_player.nCounterState--;
		if (g_player.nCounterState <= 0)
		{
			g_player.state = PLAYERSTATE_NOMAL;
		}
		break;
	}

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロック
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_player.state == PLAYERSTATE_NOMAL || g_player.state == PLAYERSTATE_DAMAGE)
	{
		//任意のキー(Aキー)が押されたかどうか
		if (GetKeyboardPress(DIK_A) == true)
		{
			if (GetKeyboardPress(DIK_W) == true)
			{//Wキーが押された
			 //ポリゴンを左上に移動
				g_movePlayer.x += sinf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
				g_movePlayer.y += cosf(-D3DX_PI * 0.75f) * PLAYER_SPEED;

				MovePlayer();
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//Sキーが押された
			 //ポリゴンを左下に移動
				g_movePlayer.x += sinf(-D3DX_PI * 0.25) * PLAYER_SPEED;
				g_movePlayer.y += cosf(-D3DX_PI * 0.25) * PLAYER_SPEED;

				MovePlayer();
			}
			else
			{//左に移動
				g_movePlayer.x -= PLAYER_SPEED;	//ポリゴンを左に移動

				MovePlayer();
			}
		}
		//任意のキー(Dキー)が押されたかどうか
		else if (GetKeyboardPress(DIK_D) == true)
		{
			if (GetKeyboardPress(DIK_W) == true)
			{//Wキーが押された
			 //ポリゴンを右上に移動
				g_movePlayer.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED;
				g_movePlayer.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED;

				MovePlayer();
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//Sキーが押された
			 //ポリゴンを右下に移動
				g_movePlayer.x += sinf(D3DX_PI * 0.25) * PLAYER_SPEED;
				g_movePlayer.y += cosf(D3DX_PI * 0.25) * PLAYER_SPEED;

				MovePlayer();
			}
			else
			{//右に移動
				g_movePlayer.x += PLAYER_SPEED;	//ポリゴンを右に移動

				MovePlayer();
			}
		}
		//任意のキー(Wキー)が押されたかどうか
		else if (GetKeyboardPress(DIK_W) == true)
		{//上に移動
			g_movePlayer.y -= PLAYER_SPEED;	//ポリゴンを上に移動

			MovePlayer();
		}
		//任意のキー(Sキー)が押されたかどうか
		else if (GetKeyboardPress(DIK_S) == true)
		{//下に移動
			g_movePlayer.y += PLAYER_SPEED;	//ポリゴンを下に移動

			MovePlayer();
		}

		//位置更新(移動)
		g_posPlayer.x += g_movePlayer.x;
		g_posPlayer.y += g_movePlayer.y;

		//減速
		g_movePlayer.x += (0.0f - g_movePlayer.x) * PLAYER_SLOWING;
		g_movePlayer.y += (0.0f - g_movePlayer.y) * PLAYER_SLOWING;
	}
	if (g_player.bDisp == true)
	{//プレイヤーが生きている
		//spaceキーが押された
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{//弾を撃つ
			PlaySound(SOUND_LABEL_SE_SHOT000);

			SetBullet(g_posPlayer, D3DXVECTOR3(0.0f, -10.0f, 0.0f), BULLETTYPE_PLAYER);		//前
			SetBullet(g_posPlayer, D3DXVECTOR3(-2.0f, -10.0f, 0.0f), BULLETTYPE_PLAYER);	//左前
			SetBullet(g_posPlayer, D3DXVECTOR3(2.0f, -10.0f, 0.0f), BULLETTYPE_PLAYER);		//右側
		}

		if (GetKeyboardTrigger(DIK_H) == true)
		{// 追尾弾
			PlaySound(SOUND_LABEL_SE_SHOT000);

			D3DXVECTOR3 BulletMove(0.0f, 0.0f, 0.0f);
			static float fAngle;
			int nCntEnemy;
			Enemy *pEnemy;
			pEnemy = GetEnemy();

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				if (pEnemy->bUse == false)
				{//エネミーが死んでいる
					pEnemy++;
				}

				if (pEnemy->bUse == true)
				{//エネミーが生きている
					fAngle = atan2f(pEnemy->pos.x - g_player.pos.x, pEnemy->pos.y - g_player.pos.y);

					BulletMove.x = sinf(fAngle) * 5.0f;
					BulletMove.y = cosf(fAngle) * 5.0f;
				}
			}
			SetBullet(g_player.pos, BulletMove, BULLETTYPE_PLAYER);
		}

		BEAM *pBeam;
		pBeam = GetBeam();

		if (GetKeyboardTrigger(DIK_V) == true)
		{//ビーム
			if (pBeam->nCntGauge >= MAX_GAUGE)
			{
				PlaySound(SOUND_LABEL_SE_BEAM);

				g_player.nCounterState = 170;
				g_player.state = PLAYERSTATE_BEAM;

				SetEffect(g_player.pos, D3DXCOLOR(0.0f, 0.3f, 1.0f, 1.0f), 15.0f, 170, EFFECTSTATE_EXPLOSION);
				SetBullet(g_player.pos + D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), BULLETTYPE_PLAYER_BEAM);
			}
		}
	}

	if (g_posPlayer.x < 0.0f + 25.0f)
	{// X座標が一定値を超えたら位置を戻す
		g_posPlayer.x = 0.0f + 25.0f;
	}
	else if (g_posPlayer.x > SCREEN_WIDTH - 25.0f)
	{// X座標が一定値を超えたら位置を戻す
		g_posPlayer.x = SCREEN_WIDTH - 25.0f;
	}

	if(g_posPlayer.y < 0.0f + 25.0f)
	{// Y座標が一定値を超えたら位置を戻す
		g_posPlayer.y = 0.0f + 25.0f;
	}
	else if (g_posPlayer.y > SCREEN_HEIGHT - 25.0f)
	{// Y座標が一定値を超えたら位置を戻す
		g_posPlayer.y = SCREEN_HEIGHT - 25.0f;
	}

	//頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(g_posPlayer.x - 25, g_posPlayer.y - 25, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posPlayer.x + 25, g_posPlayer.y - 25, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posPlayer.x - 25, g_posPlayer.y + 25, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posPlayer.x + 25, g_posPlayer.y + 25, 0.0f);

	g_player.pos = g_posPlayer;
	g_player.move = g_movePlayer;

	//頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================
// プレイヤー描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスを取得する
	pDevice = GetDevice();

	if (g_player.bDisp == true)
	{
		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	}
}

//=============================================================================
// プレイヤーのダメージ処理
//=============================================================================
void HitPlayer(int nDamage)
{
	if (g_player.state == PLAYERSTATE_BEAM)
	{
	}
	else
	{
		SubtractionLife(g_player.nLife);		//体力テクスチャを減らす
		g_player.nLife -= nDamage;
		//LIFE *pLife;
		//pLife = GetLife();

		if (g_player.nLife <= 0)
		{
			PlaySound(SOUND_LABEL_SE_EXPLOSION);
			//SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetEffect(g_player.pos, D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f), 15.0f, 30, EFFECTSTATE_EXPLOSION);
			SubtractionStock(g_player.stock);
			g_player.stock--;
			g_player.bDisp = false;
			g_player.nCounterState = 60;
			if (g_player.stock == 0)
			{
				SetExplosion(g_posPlayer, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
				g_player.state = PLAYERSTATE_DEATH;
			}
			else
			{
				g_player.state = PLAYERSTATE_APPEAR;
			}
		}
		else
		{
			PlaySound(SOUND_LABEL_SE_HIT000);
			if (g_player.state == PLAYERSTATE_BEAM)
			{
			}
			else
			{
				g_player.state = PLAYERSTATE_DAMAGE;
				g_player.nCounterState = DAMAGE_TIME;	//ダメージ状態の時間

				VERTEX_2D *pVtx;

				//頂点バッファをロックし、頂点データへのポインタを取得
				g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				//頂点バッファをアンロック
				g_pVtxBuffPlayer->Unlock();
			}
		}
	}
}

//=============================================================================
// プレイヤー移動処理
//=============================================================================
void MovePlayer(void)
{
	VERTEX_2D *pVtx;

	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_posPlayer.y > 0 || g_posPlayer.y <= SCREEN_HEIGHT ||
		g_posPlayer.x > 0 || g_posPlayer.x <= SCREEN_WIDTH)
	{
		g_nMoveX = 0;
		g_nMoveY = 0;
	}

	pVtx[0].pos.x = g_posPlayer.x - g_nMoveX;	//X座標を設定
	pVtx[0].pos.y = g_posPlayer.y - g_nMoveY;	//Y座標を設定
	pVtx[1].pos.x = g_posPlayer.x + g_nMoveX;	//X座標を設定
	pVtx[1].pos.y = g_posPlayer.y - g_nMoveY;	//Y座標を設定
	pVtx[2].pos.x = g_posPlayer.x - g_nMoveX;	//X座標を設定
	pVtx[2].pos.y = g_posPlayer.y + g_nMoveY;	//Y座標を設定
	pVtx[3].pos.x = g_posPlayer.x + g_nMoveX;	//X座標を設定
	pVtx[3].pos.y = g_posPlayer.y + g_nMoveY;	//Y座標を設定

	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================
// デバイスの取得
//=============================================================================
Player *GetPlayer(void)
{
	return &g_player;
}