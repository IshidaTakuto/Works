//=============================================================================
//
// 弾処理 [bullet.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "bullet.h"
#include "input.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BULLET	(500)	// 弾の最大数
#define BULLET_TEXTURENAME	"data/TEXTURE/bullet000.png"
#define BULLET_DAMAGE	(1)	//弾の攻撃力
#define BULLET_LIFE	(40)	//弾の体力

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	// 頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];	//弾の情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	// デバイスの取得
	pDevice = GetDevice();

	// 弾の情報の初期化
	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURENAME, &g_pTextureBullet);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);

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

	//頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntBullet;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)	
		{//弾が使用されている
			//弾の位置を移動させる
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;

			//頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 5.0f, g_aBullet[nCntBullet].pos.y - 15.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 5.0f, g_aBullet[nCntBullet].pos.y - 15.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 5.0f, g_aBullet[nCntBullet].pos.y - 5.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 5.0f, g_aBullet[nCntBullet].pos.y - 5.0f, 0.0f);

			if (g_aBullet[nCntBullet].pos.y < 0 || g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT ||
				g_aBullet[nCntBullet].pos.x < 0 || g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH)
			{
				g_aBullet[nCntBullet].bUse = false;	//使用していない状態にする
			}

			//一定時間経過
			//g_aBullet[nCntBullet].nLife--;
			//if (g_aBullet[nCntBullet].nLife <= 0)
			//{
			//	//SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			//	//g_aBullet[nCntBullet].bUse = false;
			//}

			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{//プレイヤーの弾
				int nCntEnemy;
				Enemy *pEnemy;

				//敵の取得
				pEnemy = GetEnemy();

				//エフェクト
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 0.4f, 1.0f, 1.0f), 15.0f, 60, EFFECTSTATE_BULLET);

				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている
						if (pEnemy->pos.x - 20 < g_aBullet[nCntBullet].pos.x
							&& pEnemy->pos.x + 20 > g_aBullet[nCntBullet].pos.x
							&& pEnemy->pos.y - 20 < g_aBullet[nCntBullet].pos.y
							&& pEnemy->pos.y + 20 > g_aBullet[nCntBullet].pos.y)
						{
							HitEnemy(nCntEnemy, BULLET_DAMAGE);
							g_aBullet[nCntBullet].bUse = false;	//使用していない状態にする
						}
					}
				}
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{//敵の弾
				Player *pPlayer;
				Enemy *pEnemy;

				//プレイヤーの取得
				pPlayer = GetPlayer();

				//敵の取得
				pEnemy = GetEnemy();

				//エフェクト
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f), 15.0f, 60, EFFECTSTATE_BULLET);
				
				//float fDiffAngle;	//差分
				//float fDestAngle;	//目的の角度
				//float fMoveAngle;	//現在の角度
				//static float fAngle;

				//fDestAngle = atan2f(pPlayer->pos.x - g_aBullet[nCntBullet].pos.x, pPlayer->pos.y - g_aBullet[nCntBullet].pos.y);
				//fMoveAngle = ;

				//fDiffAngle = fDestAngle - fMoveAngle;
				//if (fDiffAngle > D3DX_PI)
				//{
				//	fDiffAngle -= D3DX_PI * 2.0f;
				//}
				//if (fDiffAngle < -D3DX_PI)
				//{
				//	fDiffAngle += D3DX_PI * 2;
				//}

				//fMoveAngle += fDiffAngle * 0.4f;	//係数(0.0～1.0)寄る強さ
				//if (fMoveAngle > D3DX_PI)
				//{
				//	fMoveAngle -= D3DX_PI * 2.0f;
				//}
				//if (fMoveAngle < -D3DX_PI)
				//{
				//	fMoveAngle += D3DX_PI * 2;
				//}

				if (pPlayer->bDisp == true)
				{
					if (pPlayer->pos.x - 15 < g_aBullet[nCntBullet].pos.x
						&& pPlayer->pos.x + 15 > g_aBullet[nCntBullet].pos.x
						&& pPlayer->pos.y - 15 < g_aBullet[nCntBullet].pos.y
						&& pPlayer->pos.y + 15 > g_aBullet[nCntBullet].pos.y)
					{
						HitPlayer(BULLET_DAMAGE);
						g_aBullet[nCntBullet].bUse = false;	//使用していない状態にする
					}
				}
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER_BEAM)
			{
				//エフェクト
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.2f, 0.7f, 1.0f, 1.0f), 25.0f, 150, EFFECTSTATE_BEAM);
			}
		}
		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);

	// ポリゴンの描画
	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)	//弾が使用されている
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBullet, 2);
		}
	}
}

//=============================================================================
// 弾の設定処理
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type)
{
	int nCntBullet;
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

		if (g_aBullet[nCntBullet].bUse == false)	//弾が使用されていない
		{
			pVtx += (nCntBullet * 4);
			if (type == BULLETTYPE_PLAYER)
			{//プレイヤーの弾
				g_aBullet[nCntBullet].pos = pos;
				g_aBullet[nCntBullet].move = move;
				g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;

				//頂点の座標
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

				g_aBullet[nCntBullet].bUse = true;	//使用している状態にする
				g_aBullet[nCntBullet].nLife = BULLET_LIFE;
				break;
			}
			else if (type == BULLETTYPE_ENEMY)
			{//敵の弾
				g_aBullet[nCntBullet].pos = pos;
				g_aBullet[nCntBullet].move = move;
				g_aBullet[nCntBullet].type = BULLETTYPE_ENEMY;

				//頂点の座標
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				g_aBullet[nCntBullet].bUse = true;	//使用している状態にする
				g_aBullet[nCntBullet].nLife = BULLET_LIFE;
				break;
			}
			else if (type == BULLETTYPE_PLAYER_BEAM)
			{//プレイヤーのビーム
				g_aBullet[nCntBullet].pos = pos;
				g_aBullet[nCntBullet].move = move;
				g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER_BEAM;

				//頂点の座標
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y - 10.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 10.0f, g_aBullet[nCntBullet].pos.y + 10.0f, 0.0f);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

				g_aBullet[nCntBullet].bUse = true;	//使用している状態にする
				g_aBullet[nCntBullet].nLife = BULLET_LIFE;
				break;
			}
		}
		//頂点バッファをアンロック
		g_pVtxBuffBullet->Unlock();
	}
}

//=============================================================================
// 弾の取得
//=============================================================================
Bullet *GetBullet(void)
{
	return &g_aBullet[0];
}