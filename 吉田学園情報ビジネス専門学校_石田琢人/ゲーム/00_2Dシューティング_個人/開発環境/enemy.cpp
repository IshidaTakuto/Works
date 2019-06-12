//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "game.h"
#include "score.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TYPE_ENEMY		(4)		// 敵の種類
#define ENEMY_TEXTURENAME0	"data/TEXTURE/enemy000.png"
#define ENEMY_TEXTURENAME1	"data/TEXTURE/enemy001.png"
#define ENEMY_TEXTURENAME2	"data/TEXTURE/enemy002.png"
#define ENEMY_TEXTURENAME3	"data/TEXTURE/enemy003.png"
#define DAMAGE_TIME	(5)	//ダメージ状態の時間
#define ENEMY_LIFE	(3)	//敵の体力
#define TEX_PATTERN	(2)	//テクスチャのパターン数
#define TEX_X_UPDATE	(g_aEnemy[nCntEnemy].nPatternAnim * 0.5f)	//テクスチャX座標の更新
#define ANIM_SPEED		(30)		//アニメーションの移動速度
#define CNT_BEAM_CHAGE	(10)			//プレイヤーのビームが溜まるのに必要な倒す敵の数

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureEnemy[MAX_TYPE_ENEMY];	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				// 頂点バッファへのポインタ
Enemy g_aEnemy[MAX_ENEMY];	//敵の情報
int g_nNumEnemy;			//敵の総数

//=============================================================================
// 初期化処理
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	// デバイスの取得
	pDevice = GetDevice();

	// 敵の情報の初期化
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].nCounterAnim = 0;		//カウンターを初期化する
		g_aEnemy[nCntEnemy].nPatternAnim = 0;		//パターンNo.を初期化する
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NOMAL;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE;
		g_aEnemy[nCntEnemy].bUse = false;
	}

	g_nNumEnemy = 0;	//敵の総数をクリア

	// テクスチャの読み込み(４体分)
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURENAME0, &g_apTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURENAME1, &g_apTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURENAME2, &g_apTextureEnemy[2]);
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURENAME3, &g_apTextureEnemy[3]);

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
									D3DUSAGE_WRITEONLY,
									FVF_VERTEX_2D,
									D3DPOOL_MANAGED,
									&g_pVtxBuffEnemy,
									NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	int nCntTex;

	// テクスチャの開放
	for(nCntTex = 0; nCntTex < MAX_TYPE_ENEMY; nCntTex++)
	{
		if (g_apTextureEnemy[nCntTex] != NULL)
		{
			g_apTextureEnemy[nCntTex]->Release();
			g_apTextureEnemy[nCntTex] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	int nCntEnemy;
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].nCounterAnim++;

		if ((g_aEnemy[nCntEnemy].nCounterAnim % ANIM_SPEED) == 0)
		{
			g_aEnemy[nCntEnemy].nPatternAnim = (g_aEnemy[nCntEnemy].nPatternAnim + 1) % TEX_PATTERN;	//パターンNo.更新

			if (g_aEnemy[nCntEnemy].bUse == true)
			{
				//テクスチャ座標を更新
				pVtx[0].tex = D3DXVECTOR2(0.0f + TEX_X_UPDATE, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.5f + TEX_X_UPDATE, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + TEX_X_UPDATE, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f + TEX_X_UPDATE, 1.0f);
			}
		}
		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();


	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が使用されている
			switch (g_aEnemy[nCntEnemy].state)	
			{
			case ENEMYSTATE_NOMAL:
				if (rand() % 480 == 0)
				{
					D3DXVECTOR3 BulletMove(0.0f, 0.0f, 0.0f);
					Player *pPlayer;
					static float fAngle;

					//プレイヤーの取得
					pPlayer = GetPlayer();

					fAngle = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);

					BulletMove.x = sinf(fAngle) * 5.0f;
					BulletMove.y = cosf(fAngle) * 5.0f;

					SetBullet(g_aEnemy[nCntEnemy].pos, BulletMove, BULLETTYPE_ENEMY);
				}
				break;

			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;
				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NOMAL;

					VERTEX_2D *pVtx;	//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
					g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

					pVtx += (nCntEnemy * 4);	//該当の位置まで進める

					//頂点カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//頂点バッファをアンロック
					g_pVtxBuffEnemy->Unlock();
				}
				break;
			}
		}
		////頂点バッファをロック
		//g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		//g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
		//g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;

		////頂点座標の設定
		//pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].move.y, 0.0f);
		//pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].move.y, 0.0f);
		//pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].move.y, 0.0f);
		//pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].move.y, 0.0f);

		//pVtx += 4;	//頂点データのポインタを4つ進める

		////頂点バッファをアンロック
		//g_pVtxBuffEnemy->Unlock();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が使用されている
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEnemy, 2);
		}
	}
}

//=============================================================================
// 敵の設定
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntEnemy;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//敵が使用されていない
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].nType = nType;
			//g_aEnemy[nCntEnemy].move = move;
			g_aEnemy[nCntEnemy].bUse = true;	//使用している状態にする

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);

			g_nNumEnemy++;	//敵の総数を加算する
			break;
		}
		pVtx += 4;
	}
	
	//頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// 敵の取得
//=============================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=============================================================================
// 敵のダメージ処理
//=============================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	g_aEnemy[nCntEnemy].nLife -= nDamage;
	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		//SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		SetEffect(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f), 15.0f, 30, EFFECTSTATE_EXPLOSION);
		g_aEnemy[nCntEnemy].bUse = false;
		AddScore(1000);
		g_nNumEnemy--;
		AddBeam(15);
		if (g_nNumEnemy <= 0)
		{//敵が全滅したら
			SetGameState(GAMESTATE_END);
		}
	}
	else
	{
		PlaySound(SOUND_LABEL_SE_HIT001);
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = DAMAGE_TIME;	//ダメージ状態の時間

		AddScore(50);

		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nCntEnemy * 4);	//該当の位置まで進める

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//頂点バッファをアンロック
		g_pVtxBuffEnemy->Unlock();
	}
}