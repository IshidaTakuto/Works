//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "particle.h"
#include "effect.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PARTICLE_TEXTURENAME0	"data/TEXTURE/shadow000.jpg"
#define PARTICLE_TEXTURENAME1	"data/TEXTURE/spark.jpg"
#define PARTICLE_TEXTURENAME2	"data/TEXTURE/smoke.jpg"
#define PARTICLE_TEXTURENAME3	"data/TEXTURE/fire1.jpg"
#define PARTICLE_TEXTURENAME4	"data/TEXTURE/flower.jpg"
#define PARTICLE_TEXTURENAME5	"data/TEXTURE/elect.jpg"
#define PARTICLE_TEXTURENAME6	"data/TEXTURE/explosion.jpg"

#define NUM_TEXTURE		(7)
#define MAX_EMITTER		(80)		// エミッタの最大数
#define MAX_PARTICLE	(1000)		// パーティクルの最大数
#define PARTICLE_APPEAR	(1)			// パーティクルの出る間隔
#define PARTICLE_SPREAD	(20)		// パーティクルの広がり

#define SMOKE_TIME		(60)		//
#define SPARK_TIME		(60)
#define NORMAL_TIME		(60)
#define FIRE_TIME		(15)
#define FLOWERS_TIME	(100)
#define ELECT_TIME		(60)
#define EXPLOSION_TIME	(90)
#define FIREFLOWER_TIME	(60)
#define BLOOD_TIME		(30)
#define FOUNTAIN_TIME	(90)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);
void MakeVertexEmitter(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureParticle[NUM_TEXTURE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEmitter = NULL;	// 頂点バッファへのポインタ
Particle g_aParticle[MAX_PARTICLE];
Emitter g_aEmitter[MAX_EMITTER];
int g_nCntEmitter;
int g_nCntParticle;
int g_nCntSpread;
int g_nCntFireFlower;

//=============================================================================
// 初期化処理
//=============================================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// パーティクルの情報を初期化
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].fRadius = 0;
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].type = PARTICLETYPE_NONE;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;
	}

	// エミッタの情報を初期化
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		g_aEmitter[nCntEmitter].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEmitter[nCntEmitter].fRadius = 0;
		g_aEmitter[nCntEmitter].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEmitter[nCntEmitter].type = PARTICLETYPE_NONE;
		g_aEmitter[nCntEmitter].bUse = false;
	}

	g_nCntEmitter = 0;
	g_nCntParticle = 0;
	g_nCntSpread = 0;
	g_nCntFireFlower = 0;

	// パーティクルの頂点情報の作成
	MakeVertexParticle(pDevice);

	// エミッタの頂点情報の作成
	MakeVertexEmitter(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURENAME0, &g_pTextureParticle[0]);
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURENAME1, &g_pTextureParticle[1]);
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURENAME2, &g_pTextureParticle[2]);
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURENAME3, &g_pTextureParticle[3]);
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURENAME4, &g_pTextureParticle[4]);
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURENAME5, &g_pTextureParticle[5]);
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURENAME6, &g_pTextureParticle[6]);

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < NUM_TEXTURE; nCntTexture++)
	{
		if (g_pTextureParticle[nCntTexture] != NULL)
		{
			g_pTextureParticle[nCntTexture]->Release();
			g_pTextureParticle[nCntTexture] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEmitter != NULL)
	{
		g_pVtxBuffEmitter->Release();
		g_pVtxBuffEmitter = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{
	int nCntParticle;
	Player *pPlayer = GetPlayer();

	VERTEX_3D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{// 使われている
			g_aParticle[nCntParticle].posOld = g_aParticle[nCntParticle].pos;

			g_aParticle[nCntParticle].nLife--;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}

			switch (g_aParticle[nCntParticle].type)
			{
			case PARTICLETYPE_NONE:
				break;

			case PARTICLETYPE_NORMAL:
				g_aParticle[nCntParticle].fRadius -= 0.05f;
				g_aParticle[nCntParticle].col.a -= 0.05f;

				break;

			case PARTICLETYPE_SPARK:
				g_aParticle[nCntParticle].move += D3DXVECTOR3(0.0f, -0.05f, 0.0f);
				g_aParticle[nCntParticle].fRadius += 0.1f;
				g_aParticle[nCntParticle].col += D3DXCOLOR(0.02f, 0.02f, 0.02f, 0.0f);

				SetEffect(g_aParticle[nCntParticle].pos, g_aParticle[nCntParticle].col, 2.0f, 20, EFFECTSTATE_SPARK);
				break;

			case PARTICLETYPE_SMOKE:
				g_aParticle[nCntParticle].fRadius += 0.3f;
				g_aParticle[nCntParticle].col += D3DXCOLOR(0.02f, 0.02f, 0.02f, -0.01f);

				break;

			case PARTICLETYPE_FIRE:
				g_aParticle[nCntParticle].fRadius -= 0.2f;
				g_aParticle[nCntParticle].col += D3DXCOLOR(0.05f, 0.05f, 0.5f, -0.02f);

				break;

			case PARTICLETYPE_FLOWERS:
				g_aParticle[nCntParticle].fRadius += 0.3f;
				//g_aParticle[nCntParticle].col += D3DXCOLOR(0.02f, 0.02f, 0.02f, -0.01f);
				break;

			case PARTICLETYPE_ELECT:
				g_aParticle[nCntParticle].fRadius += 0.03f;
				g_aParticle[nCntParticle].col += D3DXCOLOR(0.02f, 0.02f, 0.02f, -0.01f);
				break;

			case PARTICLETYPE_BLOOD:
				g_aParticle[nCntParticle].move += D3DXVECTOR3(0.0f, -0.01f, 0.0f);
				g_aParticle[nCntParticle].fRadius += 0.2f;
				g_aParticle[nCntParticle].col.a -= 0.001f;
				SetEffect(g_aParticle[nCntParticle].pos, g_aParticle[nCntParticle].col, g_aParticle[nCntParticle].fRadius * 0.7f, rand() % 10, EFFECTSTATE_BLOOD);

				break;

			case PARTICLETYPE_GATHER:
				g_aParticle[nCntParticle].fRadius -= 0.02f;
				g_aParticle[nCntParticle].col.a -= 0.001f;
				//SetEffect(g_aParticle[nCntParticle].pos, g_aParticle[nCntParticle].col, g_aParticle[nCntParticle].fRadius * 0.7f, rand() % 10, EFFECTSTATE_BLOOD);
				g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle))),
					0.0f,
					cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle))));
				break;

			case PARTICLETYPE_FOUNTAIN:
				g_aParticle[nCntParticle].move += D3DXVECTOR3(0.0f, -0.02f, 0.0f);
				SetEffect(g_aParticle[nCntParticle].pos, g_aParticle[nCntParticle].col, g_aParticle[nCntParticle].fRadius, 0, EFFECTSTATE_FOUNTAIN);
				break;
			}

			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			if (g_aParticle[nCntParticle].pos.y <= 0 && g_aParticle[nCntParticle].type != PARTICLETYPE_BLOOD)
			{
				g_aParticle[nCntParticle].move.y += (g_aParticle[nCntParticle].posOld.y - g_aParticle[nCntParticle].pos.y) * 2.0f;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

			//頂点カラー
			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;
		}
		else
		{
			g_nCntParticle = (g_nCntParticle + 1) % PARTICLE_APPEAR;

			if (g_nCntParticle == 0)
			{
				for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
				{
					g_aEmitter[nCntEmitter].nLife--;
					if (g_aEmitter[nCntEmitter].nLife <= 0 && g_aEmitter[nCntEmitter].type != PARTICLETYPE_FIRE)
					{// 一定時間経過でエミッタを消す
						g_aEmitter[nCntEmitter].bUse = false;
					}

					if (g_aEmitter[g_nCntEmitter].bUse == true)
					{// 使用されているエミッタにパーティクルをつける

						switch (g_aEmitter[g_nCntEmitter].type)
						{
						case PARTICLETYPE_NONE:
							break;

						case PARTICLETYPE_NORMAL:
							g_aParticle[nCntParticle].pos = g_aEmitter[g_nCntEmitter].pos;
							g_aParticle[nCntParticle].fAngle = 1.0f / ((PARTICLE_SPREAD - g_nCntEmitter) / 2);
							g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 1.2f,
								(rand() % 10 + 2) * 0.2f,
								cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 1.2f);
							g_aParticle[nCntParticle].col = g_aEmitter[g_nCntEmitter].col;
							g_aParticle[nCntParticle].nLife = NORMAL_TIME;
							g_aParticle[nCntParticle].fRadius = g_aEmitter[g_nCntEmitter].fRadius + (rand() % 10 * 0.1f);
							g_aParticle[nCntParticle].type = g_aEmitter[g_nCntEmitter].type;
							g_aParticle[nCntParticle].nType = 0;
							g_aParticle[nCntParticle].bUse = true;

							break;

						case PARTICLETYPE_SPARK:
							g_aParticle[nCntParticle].pos = g_aEmitter[g_nCntEmitter].pos;
							g_aParticle[nCntParticle].fAngle = 1.0f / ((PARTICLE_SPREAD - g_nCntEmitter) / 2);
							g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 0.7f,
								(rand() % 10 + 2) * 0.7f,
								cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 0.7f);
							g_aParticle[nCntParticle].col = g_aEmitter[g_nCntEmitter].col;
							g_aParticle[nCntParticle].nLife = SPARK_TIME;
							g_aParticle[nCntParticle].fRadius = g_aEmitter[g_nCntEmitter].fRadius + (rand() % 15 * 0.1f);
							g_aParticle[nCntParticle].type = g_aEmitter[g_nCntEmitter].type;
							g_aParticle[nCntParticle].nType = 1;
							g_aParticle[nCntParticle].bUse = true;

							break;

						case PARTICLETYPE_SMOKE:
							g_aParticle[nCntParticle].pos = g_aEmitter[g_nCntEmitter].pos;
							g_aParticle[nCntParticle].fAngle = 1.0f / ((PARTICLE_SPREAD - g_nCntEmitter) / 2);
							g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 0.3f,
								(rand() % 20 + 2) * 0.2f,
								cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 0.3f);
							g_aParticle[nCntParticle].col = g_aEmitter[g_nCntEmitter].col;
							g_aParticle[nCntParticle].nLife = SMOKE_TIME;
							g_aParticle[nCntParticle].fRadius = g_aEmitter[g_nCntEmitter].fRadius + (rand() % 25 * 0.5f);
							g_aParticle[nCntParticle].type = g_aEmitter[g_nCntEmitter].type;
							g_aParticle[nCntParticle].nType = 2;
							g_aParticle[nCntParticle].bUse = true;

							break;

						case PARTICLETYPE_FIRE:
							g_aParticle[nCntParticle].pos = g_aEmitter[g_nCntEmitter].pos;
							g_aParticle[nCntParticle].fAngle = 1.0f / ((PARTICLE_SPREAD - g_nCntEmitter) / 2);
							g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 1.5f,
								(rand() % 15 + 2) * 0.2f,
								cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 1.5f);
							g_aParticle[nCntParticle].col = g_aEmitter[g_nCntEmitter].col;
							g_aParticle[nCntParticle].nLife = FIRE_TIME;
							g_aParticle[nCntParticle].fRadius = g_aEmitter[g_nCntEmitter].fRadius + (rand() % 25 * 0.5f);
							g_aParticle[nCntParticle].type = g_aEmitter[g_nCntEmitter].type;
							g_aParticle[nCntParticle].nType = 3;
							g_aParticle[nCntParticle].bUse = true;

							break;

						case PARTICLETYPE_FLOWERS:
							g_aParticle[nCntParticle].pos = g_aEmitter[g_nCntEmitter].pos;
							g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(pPlayer->rot.y * D3DX_PI) * 1.3f, (rand() % 2 + 1) * 0.15f, cosf(pPlayer->rot.y * D3DX_PI) * 1.3f);
							//g_aParticle[nCntParticle].fAngle = 1.0f / ((PARTICLE_SPREAD - g_nCntEmitter) / 2);
							//g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 1.3f,
							//	(rand() % 2 + 1) * 0.15f,
							//	cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 1.3f);
							g_aParticle[nCntParticle].col = g_aEmitter[g_nCntEmitter].col;
							g_aParticle[nCntParticle].nLife = FLOWERS_TIME;
							g_aParticle[nCntParticle].fRadius = g_aEmitter[g_nCntEmitter].fRadius + (rand() % 25 * 0.5f);
							g_aParticle[nCntParticle].type = g_aEmitter[g_nCntEmitter].type;
							g_aParticle[nCntParticle].nType = 4;
							g_aParticle[nCntParticle].bUse = true;

							break;

						case PARTICLETYPE_ELECT:
							g_aParticle[nCntParticle].pos = g_aEmitter[g_nCntEmitter].pos;
							g_aParticle[nCntParticle].fAngle = 1.0f / ((PARTICLE_SPREAD - g_nCntEmitter) / 2);
							g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 0.3f,
								(rand() % 2 + 1) * 0.2f,
								cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 0.3f);
							g_aParticle[nCntParticle].col = g_aEmitter[g_nCntEmitter].col;
							g_aParticle[nCntParticle].nLife = ELECT_TIME;
							g_aParticle[nCntParticle].fRadius = g_aEmitter[g_nCntEmitter].fRadius + (rand() % 25 * 0.5f);
							g_aParticle[nCntParticle].type = g_aEmitter[g_nCntEmitter].type;
							g_aParticle[nCntParticle].nType = 5;
							g_aParticle[nCntParticle].bUse = true;

							break;

						case PARTICLETYPE_BLOOD:
							g_aParticle[nCntParticle].pos = g_aEmitter[g_nCntEmitter].pos;
							g_aParticle[nCntParticle].fAngle = 1.0f / ((PARTICLE_SPREAD - g_nCntEmitter) / 2);
							g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * ((rand() % 10) * 0.1f + 4.0f),
								(rand() % 10 * 0.1f + 1) * 1.2f,
								cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * ((rand() % 10) * 0.1f + 4.0f));
							g_aParticle[nCntParticle].col = g_aEmitter[g_nCntEmitter].col;
							g_aParticle[nCntParticle].nLife = BLOOD_TIME;
							g_aParticle[nCntParticle].fRadius = g_aEmitter[g_nCntEmitter].fRadius;
							g_aParticle[nCntParticle].type = g_aEmitter[g_nCntEmitter].type;
							g_aParticle[nCntParticle].nType = 1;
							g_aParticle[nCntParticle].bUse = true;

							break;

						case PARTICLETYPE_GATHER:
							g_aParticle[nCntParticle].pos = D3DXVECTOR3(g_aEmitter[nCntEmitter].pos.x + (sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 50.0f),
								g_aEmitter[nCntEmitter].pos.y,
								g_aEmitter[nCntEmitter].pos.z + (cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 50.0f));
							g_aParticle[nCntParticle].fAngle = 1.0f / ((PARTICLE_SPREAD - g_nCntEmitter) / 2);
							g_aParticle[nCntParticle].col = g_aEmitter[g_nCntEmitter].col;
							g_aParticle[nCntParticle].nLife = BLOOD_TIME;
							g_aParticle[nCntParticle].fRadius = g_aEmitter[g_nCntEmitter].fRadius;
							g_aParticle[nCntParticle].type = g_aEmitter[g_nCntEmitter].type;
							g_aParticle[nCntParticle].nType = 0;
							g_aParticle[nCntParticle].bUse = true;

							break;

						case PARTICLETYPE_FOUNTAIN:
							g_aParticle[nCntParticle].pos = g_aEmitter[g_nCntEmitter].pos;
							g_aParticle[nCntParticle].fAngle = 1.0f / ((PARTICLE_SPREAD - g_nCntEmitter) / 2);
							g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 0.2f,
								(rand() % 10 * 0.01f + 5) * 0.2f,
								cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntSpread))) * 0.2f);
							g_aParticle[nCntParticle].col = g_aEmitter[g_nCntEmitter].col;
							g_aParticle[nCntParticle].nLife = FOUNTAIN_TIME;
							g_aParticle[nCntParticle].fRadius = g_aEmitter[g_nCntEmitter].fRadius + (rand() % 10 * 0.1f);
							g_aParticle[nCntParticle].type = g_aEmitter[g_nCntEmitter].type;
							g_aParticle[nCntParticle].nType = 0;
							g_aParticle[nCntParticle].bUse = true;
							break;
						}
						//頂点座標の設定
						pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
						pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
						pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
						pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

						//頂点カラー
						pVtx[0].col = g_aParticle[nCntParticle].col;
						pVtx[1].col = g_aParticle[nCntParticle].col;
						pVtx[2].col = g_aParticle[nCntParticle].col;
						pVtx[3].col = g_aParticle[nCntParticle].col;

						g_nCntSpread = (g_nCntSpread + 1) % PARTICLE_SPREAD;
						break;
					}
				}
				g_nCntEmitter = (g_nCntEmitter + 1) % MAX_EMITTER;
			}
		}

		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();
}

//=============================================================================
// ポリゴン描画処理
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// 計算用マトリックス
	int nCntParticle;

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 被らないようにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{// パーティクルが使われている間
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aParticle[nCntParticle].mtxWorld);

			// 回転を反映
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			g_aParticle[nCntParticle].mtxWorld._11 = mtxView._11;
			g_aParticle[nCntParticle].mtxWorld._12 = mtxView._21;
			g_aParticle[nCntParticle].mtxWorld._13 = mtxView._31;
			g_aParticle[nCntParticle].mtxWorld._21 = mtxView._12;
			g_aParticle[nCntParticle].mtxWorld._22 = mtxView._22;
			g_aParticle[nCntParticle].mtxWorld._23 = mtxView._32;
			g_aParticle[nCntParticle].mtxWorld._31 = mtxView._13;
			g_aParticle[nCntParticle].mtxWorld._32 = mtxView._23;
			g_aParticle[nCntParticle].mtxWorld._33 = mtxView._33;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			D3DXMatrixMultiply(&g_aParticle[nCntParticle].mtxWorld, &g_aParticle[nCntParticle].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aParticle[nCntParticle].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureParticle[g_aParticle[nCntParticle].nType]);

			// パーティクルの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntParticle, 2);
		}
	}

	// Zバッファを元に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// αテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// パーティクルの設定
//=============================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius,int nLife, PARTICLETYPE type)
{
	int nCntEmitter;

	// 頂点バッファの情報を設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEmitter->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		if (g_aEmitter[nCntEmitter].bUse == false)
		{
			g_aEmitter[nCntEmitter].pos = pos;
			g_aEmitter[nCntEmitter].col = col;
			g_aEmitter[nCntEmitter].fRadius = fRadius;
			g_aEmitter[nCntEmitter].nLife = nLife;
			g_aEmitter[nCntEmitter].type = type;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aEmitter[nCntEmitter].fRadius, g_aEmitter[nCntEmitter].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEmitter[nCntEmitter].fRadius, g_aEmitter[nCntEmitter].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEmitter[nCntEmitter].fRadius, -g_aEmitter[nCntEmitter].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEmitter[nCntEmitter].fRadius, -g_aEmitter[nCntEmitter].fRadius, 0.0f);

			// 頂点カラー
			pVtx[0].col = g_aEmitter[nCntEmitter].col;
			pVtx[1].col = g_aEmitter[nCntEmitter].col;
			pVtx[2].col = g_aEmitter[nCntEmitter].col;
			pVtx[3].col = g_aEmitter[nCntEmitter].col;

			g_aEmitter[nCntEmitter].bUse = true;
			break;

		}
		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEmitter->Unlock();
}

//=============================================================================
// パーティクルの頂点情報の作成
//=============================================================================
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	VERTEX_3D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

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

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();
}

//=============================================================================
// エミッタの頂点情報の作成
//=============================================================================
void MakeVertexEmitter(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EMITTER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEmitter,
		NULL);

	VERTEX_3D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEmitter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		// 頂点の座標
		pVtx[0].pos = D3DXVECTOR3(-g_aEmitter[nCntEmitter].fRadius, g_aEmitter[nCntEmitter].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEmitter[nCntEmitter].fRadius, g_aEmitter[nCntEmitter].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aEmitter[nCntEmitter].fRadius, -g_aEmitter[nCntEmitter].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEmitter[nCntEmitter].fRadius, -g_aEmitter[nCntEmitter].fRadius, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEmitter->Unlock();
}
