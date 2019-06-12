//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "particle.h"
#include "string.h"
#include "input.h"
#include "operation.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PARTICLE_FILENAME	"data/TEXT/EFFECT/particle.txt"				// パーティクルデータ
#define TEXTURE_FILENAME	"data/TEXT/EFFECT/particle_texture.txt"		// テクスチャ名
#define PARTICLE_SAVEFILE	"data/TEXT/EFFECT/save_particle.txt"

#define MAX_EMITTER_TYPE	(32)							// パーティクルの種類の最大数
#define MAX_PARTICLE		(1024)							// パーティクルの最大数

#define MAX_TEXTURE			(32)								// テクスチャの最大数
#define MAX_TYPE			(32)								// 項目の最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);

void EditParticle(void);

void FileInputParticle(void);
void FileOutputParticle(void);
void TextureInputParticle(void);

void StringParticle(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureParticle[MAX_TEXTURE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	// 頂点バッファへのポインタ
Particle g_aParticle[MAX_PARTICLE];				// パーティクル
ParEmitter g_aEmitter[MAX_EMITTER];				// 表示用のエミッタ
ParEmitter g_aEditEmitter[MAX_EMITTER_TYPE];		// エディット用のエミッタ

int g_nCntParticle;		// 出現させる間隔のカウンタ

int g_nSelectParticleEmitter;				// 選択中のエミッタ
int g_nSelectParticleType;					// 選択中の項目

int g_nNumParticleData;						// パーティクルの種類の数
int g_nNumParticleTexture;					// テクスチャの数

int g_nCntAngle;							// パーティクルの角度計算用カウンタ

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
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fRadius = 0;
		g_aParticle[nCntParticle].nType = 0;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].fAngle = 0.0f;
		g_aParticle[nCntParticle].fGravity = 0.0f;
		g_aParticle[nCntParticle].fBouncing = 0.0f;
		g_aParticle[nCntParticle].nIdx = 0;
		g_aParticle[nCntParticle].bUse = false;
		g_aParticle[nCntParticle].bDraw = false;
	}

	// エミッタの情報を初期化
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		g_aEmitter[nCntEmitter].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEmitter[nCntEmitter].nWidth[0] = 0;
		g_aEmitter[nCntEmitter].nWidth[1] = 0;
		g_aEmitter[nCntEmitter].nHeight[0] = 0;
		g_aEmitter[nCntEmitter].nHeight[1] = 0;
		g_aEmitter[nCntEmitter].nDepth[0] = 0;
		g_aEmitter[nCntEmitter].nDepth[1] = 0;
		g_aEmitter[nCntEmitter].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEmitter[nCntEmitter].flameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEmitter[nCntEmitter].nRadius[0] = 0;
		g_aEmitter[nCntEmitter].nRadius[1] = 0;
		g_aEmitter[nCntEmitter].fFlameRadius = 0;
		g_aEmitter[nCntEmitter].fSpread = 0.0f;
		g_aEmitter[nCntEmitter].nAppear = 1;
		g_aEmitter[nCntEmitter].nParLife[0] = 0;
		g_aEmitter[nCntEmitter].nParLife[1] = 0;
		g_aEmitter[nCntEmitter].nLife = 0;
		g_aEmitter[nCntEmitter].nType = 0;
		g_aEmitter[nCntEmitter].nLap = 0;
		g_aEmitter[nCntEmitter].fGravity = 0.0f;
		g_aEmitter[nCntEmitter].nRot = 0;
		g_aEmitter[nCntEmitter].fRotSpeed = 0.0f;
		g_aEmitter[nCntEmitter].bLoop = false;
		g_aEmitter[nCntEmitter].bBouncing = false;
		g_aEmitter[nCntEmitter].bUse = false;
		g_aEmitter[nCntEmitter].bDraw = false;
	}
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER_TYPE; nCntEmitter++)
	{
		g_aEditEmitter[nCntEmitter].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEditEmitter[nCntEmitter].nWidth[0] = 0;
		g_aEditEmitter[nCntEmitter].nWidth[1] = 0;
		g_aEditEmitter[nCntEmitter].nHeight[0] = 0;
		g_aEditEmitter[nCntEmitter].nHeight[1] = 0;
		g_aEditEmitter[nCntEmitter].nDepth[0] = 0;
		g_aEditEmitter[nCntEmitter].nDepth[1] = 0;
		g_aEditEmitter[nCntEmitter].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEditEmitter[nCntEmitter].flameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEditEmitter[nCntEmitter].nRadius[0] = 0;
		g_aEditEmitter[nCntEmitter].nRadius[1] = 0;
		g_aEditEmitter[nCntEmitter].fFlameRadius = 0;
		g_aEditEmitter[nCntEmitter].fSpread = 0.0f;
		g_aEditEmitter[nCntEmitter].nAppear = 1;
		g_aEditEmitter[nCntEmitter].nParLife[0] = 0;
		g_aEditEmitter[nCntEmitter].nParLife[1] = 0;
		g_aEditEmitter[nCntEmitter].nLife = 0;
		g_aEditEmitter[nCntEmitter].nLap = 1;
		g_aEditEmitter[nCntEmitter].fGravity = 0.0f;
		g_aEditEmitter[nCntEmitter].nRot = 0;
		g_aEditEmitter[nCntEmitter].fRotSpeed = 0.0f;
		g_aEditEmitter[nCntEmitter].nType = 0;
		g_aEditEmitter[nCntEmitter].bLoop = false;
		g_aEditEmitter[nCntEmitter].bBouncing = false;
		g_aEditEmitter[nCntEmitter].bUse = false;
		g_aEditEmitter[nCntEmitter].bDraw = false;
	}

	g_nCntParticle = 0;
	g_nNumParticleTexture = 1;
	g_nCntAngle = 0;

	FileInputParticle();
	TextureInputParticle();

	// パーティクルの頂点情報の作成
	MakeVertexParticle(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
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
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{
	int nCntParticle;	// for分用のカウンタ

	EditParticle();

	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		if (g_aEmitter[nCntEmitter].bUse == true)
		{// 使用されているエミッタ

			g_aEmitter[nCntEmitter].nLife--;	// エミッタの表示時間をカウント

			if (g_aEmitter[nCntEmitter].nLife <= 0 && g_aEmitter[nCntEmitter].bLoop == false)
			{// 一定時間経過でエミッタを消す
				g_aEmitter[nCntEmitter].bUse = false;
			}

			// 出現させる間隔のカウント
			g_nCntParticle = (g_nCntParticle + 1) % g_aEmitter[nCntEmitter].nAppear;

			if (g_nCntParticle == 0)
			{// 出現させる
				for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
				{// パーティクルをつける
					if (g_aParticle[nCntParticle].bUse == false)
					{// 使われていない
						g_aParticle[nCntParticle].fAngle = 1.0f / (g_aEmitter[nCntEmitter].nLap / 2);
						g_aParticle[nCntParticle].pos = g_aEmitter[nCntEmitter].pos;
						//g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntAngle))) * ((g_aEmitter[nCntEmitter].nWidth[0] + (rand() % (1 + (g_aEmitter[nCntEmitter].nWidth[1] * 2))) / 2) + ((rand() % 10) * 0.1f)) * g_aEmitter[nCntEmitter].fSpread,
						//	(rand() % g_aEmitter[nCntEmitter].nHeight[1]) + (rand() % g_aEmitter[nCntEmitter].nHeight[0]),
						//	cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntAngle))) * ((g_aEmitter[nCntEmitter].nDepth[0] + (rand() % (1 + (g_aEmitter[nCntEmitter].nDepth[1] * 2))) / 2) + ((rand() % 10) * 0.1f)) * g_aEmitter[nCntEmitter].fSpread);
						if(0 != g_aEmitter[nCntEmitter].nHeight[1] && 0 != g_aEmitter[nCntEmitter].nHeight[0])
						{
							g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntAngle))) * ((g_aEmitter[nCntEmitter].nWidth[0] + (rand() % (1 + g_aEmitter[nCntEmitter].nWidth[1])) - 1) - ((rand() % 10) * 0.1f)) * g_aEmitter[nCntEmitter].fSpread,
								((rand() % g_aEmitter[nCntEmitter].nHeight[1]) + (rand() % g_aEmitter[nCntEmitter].nHeight[0])) * g_aEmitter[nCntEmitter].fSpread,
								cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntAngle))) * ((g_aEmitter[nCntEmitter].nDepth[0] + (rand() % (1 + g_aEmitter[nCntEmitter].nDepth[1]) - 1)) - ((rand() % 10) * 0.1f)) * g_aEmitter[nCntEmitter].fSpread);
						}
						else
						{
							g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntAngle))) * ((g_aEmitter[nCntEmitter].nWidth[0] + (rand() % (1 + g_aEmitter[nCntEmitter].nWidth[1])) - 1) - ((rand() % 10) * 0.1f)) * g_aEmitter[nCntEmitter].fSpread,
								(g_aEmitter[nCntEmitter].nHeight[1] + g_aEmitter[nCntEmitter].nHeight[0]) * g_aEmitter[nCntEmitter].fSpread,
								cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntAngle))) * ((g_aEmitter[nCntEmitter].nDepth[0] + (rand() % (1 + g_aEmitter[nCntEmitter].nDepth[1]) - 1)) - ((rand() % 10) * 0.1f)) * g_aEmitter[nCntEmitter].fSpread);
						}

						g_aParticle[nCntParticle].col = g_aEmitter[nCntEmitter].col;
						g_aParticle[nCntParticle].fRadius = g_aEmitter[nCntEmitter].nRadius[0] + (rand() % (1 + (g_aEmitter[nCntEmitter].nRadius[1] - g_aEmitter[nCntEmitter].nRadius[0])) - 1) + ((rand() % 10) * 0.1f);
						g_aParticle[nCntParticle].nLife = g_aEmitter[nCntEmitter].nParLife[0] + (rand() % (1 + (g_aEmitter[nCntEmitter].nParLife[1] - g_aEmitter[nCntEmitter].nParLife[0])) - 1);
						g_aParticle[nCntParticle].nType = g_aEmitter[nCntEmitter].nType;
						g_aParticle[nCntParticle].fGravity = g_aEmitter[nCntEmitter].fGravity;
						g_aParticle[nCntParticle].nIdx = nCntEmitter;
						g_aParticle[nCntParticle].bDraw = g_aEmitter[nCntEmitter].bDraw;
						g_aParticle[nCntParticle].bUse = true;

						g_nCntAngle = (g_nCntAngle + 1) % g_aEmitter[nCntEmitter].nLap;
						break;
					}
				}
			}
		}
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// パーティクルの更新
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	VERTEX_3D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{// 使われている
			g_aParticle[nCntParticle].nLife--;		// 表示時間をカウント

			// 表示時間が0以下になった
			if (g_aParticle[nCntParticle].nLife <= 0)
			{// 使用していない状態にする
				g_aParticle[nCntParticle].bUse = false;
			}

			g_aParticle[nCntParticle].move.y -= g_aParticle[nCntParticle].fGravity;		// 重力を加える

			g_aParticle[nCntParticle].fBouncing = g_aParticle[nCntParticle].pos.y;		// 値を記憶

			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;								// 位置の更新
			g_aParticle[nCntParticle].col += g_aEmitter[g_aParticle[nCntParticle].nIdx].flameCol;			// 色の更新
			g_aParticle[nCntParticle].fRadius += g_aEmitter[g_aParticle[nCntParticle].nIdx].fFlameRadius;	// 大きさの更新

			if (g_aParticle[nCntParticle].pos.y <= 0 && g_aEmitter[g_aParticle[nCntParticle].nIdx].bBouncing == true)
			{// バウンド
				g_aParticle[nCntParticle].move.y += (g_aParticle[nCntParticle].fBouncing - g_aParticle[nCntParticle].pos.y) * 2.0f;
			}

			// 頂点の角度
			g_aParticle[nCntParticle].fRotAngle = atan2f(g_aParticle[nCntParticle].pos.x - (g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius), g_aParticle[nCntParticle].pos.y - (g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius));

			// 中心から頂点までの距離
			g_aParticle[nCntParticle].fLength = sqrtf((g_aParticle[nCntParticle].fRadius * g_aParticle[nCntParticle].fRadius) + (g_aParticle[nCntParticle].fRadius * g_aParticle[nCntParticle].fRadius));

			if (g_aEmitter[g_aParticle[nCntParticle].nIdx].nRot == 1)
			{// 時計回り
				g_aParticle[nCntParticle].fCntRot -= g_aEmitter[g_aParticle[nCntParticle].nIdx].fRotSpeed;			// 回転のカウンタ
			}
			else if (g_aEmitter[g_aParticle[nCntParticle].nIdx].nRot == 2)
			{// 反時計回り
				g_aParticle[nCntParticle].fCntRot += g_aEmitter[g_aParticle[nCntParticle].nIdx].fRotSpeed;			// 回転のカウンタ
			}
			else if (g_aEmitter[g_aParticle[nCntParticle].nIdx].nRot == 3)
			{// ランダム
				if (nCntParticle % 2 == 0)
				{// 時計回り
					g_aParticle[nCntParticle].fCntRot += g_aEmitter[g_aParticle[nCntParticle].nIdx].fRotSpeed;		// 回転のカウンタ
				}
				else
				{// 反時計回り
					g_aParticle[nCntParticle].fCntRot -= g_aEmitter[g_aParticle[nCntParticle].nIdx].fRotSpeed;		// 回転のカウンタ
				}
			}

			//頂点座標の設定
			pVtx[0].pos.x = sinf(g_aParticle[nCntParticle].fRotAngle + g_aParticle[nCntParticle].fCntRot) * -(g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);
			pVtx[0].pos.y = cosf(g_aParticle[nCntParticle].fRotAngle + g_aParticle[nCntParticle].fCntRot) * (g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);
			pVtx[1].pos.x = sinf(g_aParticle[nCntParticle].fRotAngle - g_aParticle[nCntParticle].fCntRot) * (g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);
			pVtx[1].pos.y = cosf(g_aParticle[nCntParticle].fRotAngle - g_aParticle[nCntParticle].fCntRot) * (g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);
			pVtx[2].pos.x = sinf(g_aParticle[nCntParticle].fRotAngle - g_aParticle[nCntParticle].fCntRot) * -(g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);
			pVtx[2].pos.y = cosf(g_aParticle[nCntParticle].fRotAngle - g_aParticle[nCntParticle].fCntRot) * -(g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);
			pVtx[3].pos.x = sinf(g_aParticle[nCntParticle].fRotAngle + g_aParticle[nCntParticle].fCntRot) * (g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);
			pVtx[3].pos.y = cosf(g_aParticle[nCntParticle].fRotAngle + g_aParticle[nCntParticle].fCntRot) * -(g_aParticle[nCntParticle].fLength + g_aParticle[nCntParticle].fRadius);

			//頂点座標の設定
			//pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
			//pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
			//pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
			//pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

			//頂点カラー
			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;
		}
		pVtx += 4;	//頂点データのポインタを4つ進める
	}


	// 頂点バッファをアンロック
	g_pVtxBuffParticle->Unlock();

	// ファイルに出力
	if (GetKeyboardTrigger(DIK_F9) == true)
	{
		FileOutputParticle();
	}
}

//=============================================================================
// ポリゴン描画処理
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// 計算用マトリックス
	int nCntParticle;

	// パーティクルのデータを文字列にする
	StringParticle();

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
		if (g_aParticle[nCntParticle].bDraw == true)
		{ // αブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
		else
		{// αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

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
// エフェクトのデータを文字列にする
//=============================================================================
void StringParticle(void)
{
	char aStrParticle[PARTICLE_MAX][MAX_STRING];			// 表示する文字列　数値
	char aStrParticleType[PARTICLE_MAX][MAX_STRING];		// 表示する文字列　項目

	// 項目
	sprintf(&aStrParticleType[PARTICLE_SELEMITTER][0], "[ エミッタ番号 ]");			// 選択中のエミッタ
	sprintf(&aStrParticleType[PARTICLE_MAXMOVE][0], "[ 移動量の最大 ]");			// 移動量の最大値
	sprintf(&aStrParticleType[PARTICLE_MINMOVE][0], "[ 移動量の最小 ]");			// 移動量の最小値
	sprintf(&aStrParticleType[PARTICLE_COLOR][0], "[ カラー ]");					// 色
	sprintf(&aStrParticleType[PARTICLE_FLAMECOLOR][0], "[ カラーの変化 ]");			// 色の変化
	sprintf(&aStrParticleType[PARTICLE_RADIUS][0], "[ サイズ ]");					// サイズ
	sprintf(&aStrParticleType[PARTICLE_FLAMERADIUS][0], "[ サイズの拡縮 ]");		// 毎フレームのサイズの拡縮
	sprintf(&aStrParticleType[PARTICLE_LIFE][0], "[ エミッタの寿命 ]");				// エミッタの表示時間
	sprintf(&aStrParticleType[PARTICLE_PARLIFE][0], "[ 表示フレーム ]");			// パーティクルの表示時間
	sprintf(&aStrParticleType[PARTICLE_APPEAR][0], "[ 出る間隔 ]");					// 出る間隔
	sprintf(&aStrParticleType[PARTICLE_SPREAD][0], "[ 拡散 ]");						// 拡散する度合
	sprintf(&aStrParticleType[PARTICLE_LAP][0], "[ 1周の個数 ]");					// 1周に出す個数
	sprintf(&aStrParticleType[PARTICLE_LOOP][0], "[ ループの有無 ]");				// ループの有無
	sprintf(&aStrParticleType[PARTICLE_GRAVITY][0], "[ 重力 ]");					// 重力
	sprintf(&aStrParticleType[PARTICLE_BOUNCING][0], "[ バウンドの有無 ]");			// バウンドの有無
	sprintf(&aStrParticleType[PARTICLE_ROTATE][0], "[ 回転のパターン ]");			// 回転のパターン
	sprintf(&aStrParticleType[PARTICLE_ROTATE_SPEED][0], "[ 回転の速度 ]");			// 回転の速度
	sprintf(&aStrParticleType[PARTICLE_TEXTURE][0], "[ テクスチャ ]");				// テクスチャ
	sprintf(&aStrParticleType[PARTICLE_DRAW][0], "[ 加算合成の有無 ]");				// 加算合成の有無

	// 数値
	sprintf(&aStrParticle[PARTICLE_SELEMITTER][0], "[ %d ]", g_nSelectParticleEmitter);								// 選択中のエミッタ
	sprintf(&aStrParticle[PARTICLE_MAXMOVE][0], "[ %d %d %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nWidth[1], g_aEditEmitter[g_nSelectParticleEmitter].nHeight[1], g_aEditEmitter[g_nSelectParticleEmitter].nDepth[1]);	// 移動量の最大
	sprintf(&aStrParticle[PARTICLE_MINMOVE][0], "[ %d %d %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0], g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0], g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0]);	// 移動量の最小
	sprintf(&aStrParticle[PARTICLE_COLOR][0], "[ %.2f %.2f %.2f %.2f ]", g_aEditEmitter[g_nSelectParticleEmitter].col.r, g_aEditEmitter[g_nSelectParticleEmitter].col.g, g_aEditEmitter[g_nSelectParticleEmitter].col.b, g_aEditEmitter[g_nSelectParticleEmitter].col.a);	// 色
	sprintf(&aStrParticle[PARTICLE_FLAMECOLOR][0], "[ %.2f %.2f %.2f %.2f ]", g_aEditEmitter[g_nSelectParticleEmitter].flameCol.r, g_aEditEmitter[g_nSelectParticleEmitter].flameCol.g, g_aEditEmitter[g_nSelectParticleEmitter].flameCol.b, g_aEditEmitter[g_nSelectParticleEmitter].flameCol.a);	// 色
	sprintf(&aStrParticle[PARTICLE_RADIUS][0], "[ %d %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0], g_aEditEmitter[g_nSelectParticleEmitter].nRadius[1]);			// サイズ
	sprintf(&aStrParticle[PARTICLE_FLAMERADIUS][0], "[ %.2f ]", g_aEditEmitter[g_nSelectParticleEmitter].fFlameRadius);		// 毎フレームのサイズの拡縮
	sprintf(&aStrParticle[PARTICLE_LIFE][0], "[ %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nLife);						// 寿命
	sprintf(&aStrParticle[PARTICLE_PARLIFE][0], "[ %d %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0], g_aEditEmitter[g_nSelectParticleEmitter].nParLife[1]);			// 表示時間
	sprintf(&aStrParticle[PARTICLE_APPEAR][0], "[ %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nAppear);					// 出る間隔
	sprintf(&aStrParticle[PARTICLE_SPREAD][0], "[ %.2f ]", g_aEditEmitter[g_nSelectParticleEmitter].fSpread);					// 拡散する度合
	sprintf(&aStrParticle[PARTICLE_LAP][0], "[ %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nLap);						// 1周に出す個数
	sprintf(&aStrParticle[PARTICLE_GRAVITY][0], "[ %.2f ]", g_aEditEmitter[g_nSelectParticleEmitter].fGravity);					// 重力の度合
	sprintf(&aStrParticle[PARTICLE_LOOP][0], "[ %s ]", g_aEditEmitter[g_nSelectParticleEmitter].bLoop ? "あり" : "なし");		// ループさせるか
	sprintf(&aStrParticle[PARTICLE_BOUNCING][0], "[ %s ]", g_aEditEmitter[g_nSelectParticleEmitter].bBouncing ? "あり" : "なし");	// バウンドさせるか
	sprintf(&aStrParticle[PARTICLE_ROTATE][0], "[ %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nRot);		// 回転のパターン
	sprintf(&aStrParticle[PARTICLE_ROTATE_SPEED][0], "[ %.2f ]", g_aEditEmitter[g_nSelectParticleEmitter].fRotSpeed);	// 回転の速度
	sprintf(&aStrParticle[PARTICLE_TEXTURE][0], "[ %d ]", g_aEditEmitter[g_nSelectParticleEmitter].nType);					// テクスチャ
	sprintf(&aStrParticle[PARTICLE_DRAW][0], "[ %s ]", g_aEditEmitter[g_nSelectParticleEmitter].bDraw ? "加算合成" : "普通");		// 加算合成の有無

	// 表示する
	RECT rect;

	for (int nCntString = 0; nCntString < PARTICLE_MAX; nCntString++)
	{	// 項目の描画
		rect = { 10,230 + (nCntString * 20),0,0 };
		SetString(&aStrParticleType[nCntString][0], rect, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// ：を入れる
		rect = { 170,230 + (nCntString * 20),0,0 };
		SetString(":", rect, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// 数値の描画
		if (nCntString == g_nSelectParticleType)
		{// 選択中
			rect = { 200,230 + (nCntString * 20),0,0 };
			SetString(&aStrParticle[nCntString][0], rect, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{// 選択中でない
			rect = { 190,230 + (nCntString * 20),0,0 };
			SetString(&aStrParticle[nCntString][0], rect, 0, D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
		}
	}
}

//=============================================================================
// パーティクルの作成
//=============================================================================
void EditParticle(void)
{
	// 項目の選択
	if (GetKeyboardRepeat(SELECT_UP.nDIK) == true)
	{// カーソルを上げる
		g_nSelectParticleType--;

		if (g_nSelectParticleType < 0)
		{// ループさせる
			g_nSelectParticleType = PARTICLE_MAX - 1;
		}
	}
	else if (GetKeyboardRepeat(SELECT_DOWN.nDIK) == true)
	{// カーソルを下げる
		g_nSelectParticleType = (g_nSelectParticleType + 1) % PARTICLE_MAX;
	}

	// 数値の設定
	switch (g_nSelectParticleType)
	{
	case PARTICLE_SELEMITTER:		// 選択中のエミッタ------------------------------------------------------------------------------
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_nSelectParticleEmitter = 0;
		}

		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// 加算する
			if (g_nSelectParticleEmitter < MAX_EMITTER_TYPE)
			{// 最大を越さない
				g_nSelectParticleEmitter++;
			}
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// 減算する
			if (g_nSelectParticleEmitter > 0)
			{// 0より下げない
				g_nSelectParticleEmitter--;
			}
		}

		break;

	case PARTICLE_MAXMOVE:				// 移動量の最大-------------------------------------------------------------------------------
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_aEditEmitter[g_nSelectParticleEmitter].nWidth[1] = g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0];
			g_aEditEmitter[g_nSelectParticleEmitter].nHeight[1] = g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0];
			g_aEditEmitter[g_nSelectParticleEmitter].nDepth[1] = g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0];
		}

		// 数値変更
		if (GetKeyboardPress(DIK_X) == true)
		{// X軸
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				g_aEditEmitter[g_nSelectParticleEmitter].nWidth[1]++;
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				if (g_aEditEmitter[g_nSelectParticleEmitter].nWidth[1] > g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0])
				{// 最小値より小さくしない
					g_aEditEmitter[g_nSelectParticleEmitter].nWidth[1]--;
				}
			}
		}
		if (GetKeyboardPress(DIK_Y) == true)
		{// Y軸
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				g_aEditEmitter[g_nSelectParticleEmitter].nHeight[1]++;
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				if (g_aEditEmitter[g_nSelectParticleEmitter].nHeight[1] > g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0])
				{// 最小値より小さくしない
					g_aEditEmitter[g_nSelectParticleEmitter].nHeight[1]--;
				}
			}
		}
		if (GetKeyboardPress(DIK_Z) == true)
		{// Z軸
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				g_aEditEmitter[g_nSelectParticleEmitter].nDepth[1]++;
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				if (g_aEditEmitter[g_nSelectParticleEmitter].nDepth[1] > g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0])
				{// 最小値より小さくしない
					g_aEditEmitter[g_nSelectParticleEmitter].nDepth[1]--;
				}
			}
		}
		break;

	case PARTICLE_MINMOVE:				// 移動量の最小-------------------------------------------------------------------------------
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0] = 0;
			g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0] = 0;
			g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0] = 0;
		}

		// 数値変更
		if (GetKeyboardPress(DIK_X) == true)
		{// X軸
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				if (g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0] < g_aEditEmitter[g_nSelectParticleEmitter].nWidth[1])
				{// 最大値より大きくしない
					g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0]++;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0]--;
			}
		}
		if (GetKeyboardPress(DIK_Y) == true)
		{// Y軸
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				if (g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0] < g_aEditEmitter[g_nSelectParticleEmitter].nHeight[1])
				{// 最大値より大きくしない
					g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0]++;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0]--;
			}
		}
		if (GetKeyboardPress(DIK_Z) == true)
		{// Z軸
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				if (g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0] < g_aEditEmitter[g_nSelectParticleEmitter].nDepth[1])
				{// 最大値より大きくしない
					g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0]++;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0]--;
			}
		}
		break;

	case PARTICLE_COLOR:			// 色-------------------------------------------------------------------------------
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_aEditEmitter[g_nSelectParticleEmitter].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		}

		// 数値変更
		if (GetKeyboardPress(DIK_R) == true)
		{// 赤
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				g_aEditEmitter[g_nSelectParticleEmitter].col.r += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.r > 1.0f)
				{// 最大を越すと0にする
					g_aEditEmitter[g_nSelectParticleEmitter].col.r = 0.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				g_aEditEmitter[g_nSelectParticleEmitter].col.r -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.r < 0.0f)
				{// 0を越すと最大にする
					g_aEditEmitter[g_nSelectParticleEmitter].col.r = 1.0f;
				}
			}
		}
		else if (GetKeyboardPress(DIK_G) == true)
		{// 緑
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				g_aEditEmitter[g_nSelectParticleEmitter].col.g += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.g > 1.0f)
				{// 最大を越すと0にする
					g_aEditEmitter[g_nSelectParticleEmitter].col.g = 0.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				g_aEditEmitter[g_nSelectParticleEmitter].col.g -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.g < 0.0f)
				{// 0を越すと最大にする
					g_aEditEmitter[g_nSelectParticleEmitter].col.g = 1.0f;
				}
			}
		}
		else if (GetKeyboardPress(DIK_B) == true)
		{// 青
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				g_aEditEmitter[g_nSelectParticleEmitter].col.b += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.b > 1.0f)
				{// 最大を越すと0にする
					g_aEditEmitter[g_nSelectParticleEmitter].col.b = 0.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				g_aEditEmitter[g_nSelectParticleEmitter].col.b -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.b < 0.0f)
				{// 0を越すと最大にする
					g_aEditEmitter[g_nSelectParticleEmitter].col.b = 1.0f;
				}
			}
		}
		if (GetKeyboardPress(DIK_T) == true)
		{// 透明度
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				g_aEditEmitter[g_nSelectParticleEmitter].col.a += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.a > 1.0f)
				{// 最大を越すと0にする
					g_aEditEmitter[g_nSelectParticleEmitter].col.a = 0.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				g_aEditEmitter[g_nSelectParticleEmitter].col.a -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].col.a < 0.0f)
				{// 0を越すと最大にする
					g_aEditEmitter[g_nSelectParticleEmitter].col.a = 1.0f;
				}
			}
		}
		break;

	case PARTICLE_FLAMECOLOR:			// 色の加算-------------------------------------------------------------------------------
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_aEditEmitter[g_nSelectParticleEmitter].flameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		}

		// 数値変更
		if (GetKeyboardPress(DIK_R) == true)
		{// 赤
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.r += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.r > 1.0f)
				{// 最大を越すと-1にする
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.r = -1.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.r -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.r < -1.0f)
				{// -1を越すと最大にする
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.r = 1.0f;
				}
			}
		}
		else if (GetKeyboardPress(DIK_G) == true)
		{// 緑
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.g += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.g > 1.0f)
				{// 最大を越すと-1にする
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.g = -1.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.g -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.g < -1.0f)
				{// -1を越すと最大にする
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.g = 1.0f;
				}
			}
		}
		else if (GetKeyboardPress(DIK_B) == true)
		{// 青
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.b += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.b > 1.0f)
				{// 最大を越すと-1にする
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.b = -1.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.b -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.b < -1.0f)
				{// -1を越すと最大にする
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.b = 1.0f;
				}
			}
		}
		else if (GetKeyboardPress(DIK_T) == true)
		{// 透明度
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.a += 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.a > 1.0f)
				{// 最大を越すと-1にする
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.a = -1.0f;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				g_aEditEmitter[g_nSelectParticleEmitter].flameCol.a -= 0.01f;

				if (g_aEditEmitter[g_nSelectParticleEmitter].flameCol.a < -1.0f)
				{// -1を越すと最大にする
					g_aEditEmitter[g_nSelectParticleEmitter].flameCol.a = 1.0f;
				}
			}
		}
		break;

	case PARTICLE_RADIUS:				// サイズ-------------------------------------------------------------------------------
		if (GetKeyboardPress(DIK_G) == true)
		{// 最小値
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				if (g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0] < g_aEditEmitter[g_nSelectParticleEmitter].nRadius[1])
				{
					g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0]++;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				if (g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0] > 0)
				{
					g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0]--;
				}
			}
		}
		if (GetKeyboardPress(DIK_T) == true)
		{// 最大値
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				g_aEditEmitter[g_nSelectParticleEmitter].nRadius[1]++;
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				if (g_aEditEmitter[g_nSelectParticleEmitter].nRadius[1] > g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0])
				{
					g_aEditEmitter[g_nSelectParticleEmitter].nRadius[1]--;
				}
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0] = 5;
			g_aEditEmitter[g_nSelectParticleEmitter].nRadius[1] = 10;
		}

		break;

	case PARTICLE_FLAMERADIUS:		// 大きさの加算-------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// 加算する
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// 加算する量を増やす
				g_aEditEmitter[g_nSelectParticleEmitter].fFlameRadius += 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fFlameRadius += 0.01f;
			}
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// 減算する
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// 減算する量を増やす
				g_aEditEmitter[g_nSelectParticleEmitter].fFlameRadius -= 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fFlameRadius -= 0.01f;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_aEditEmitter[g_nSelectParticleEmitter].fFlameRadius = 5.0f;
		}

		break;

	case PARTICLE_LIFE:				// 寿命-------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// 加算する
			g_aEditEmitter[g_nSelectParticleEmitter].nLife++;
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// 減算する
			if (g_aEditEmitter[g_nSelectParticleEmitter].nLife > 1)
			{// 0より下げない
				g_aEditEmitter[g_nSelectParticleEmitter].nLife--;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_aEditEmitter[g_nSelectParticleEmitter].nLife = 0;
		}

		break;

	case PARTICLE_PARLIFE:			// 表示時間-------------------------------------------------------------------------------
		if (GetKeyboardPress(DIK_G) == true)
		{
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				if (g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0] < g_aEditEmitter[g_nSelectParticleEmitter].nParLife[1])
				{// 最大値より大きくしない
					g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0]++;
				}
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				if (g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0] > 0)
				{// 0より下げない
					g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0]--;
				}
			}
		}
		if (GetKeyboardPress(DIK_T) == true)
		{
			if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
			{// 加算する
				g_aEditEmitter[g_nSelectParticleEmitter].nParLife[1]++;
			}
			else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
			{// 減算する
				if (g_aEditEmitter[g_nSelectParticleEmitter].nParLife[1] > g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0])
				{// 最小値より下げない
					g_aEditEmitter[g_nSelectParticleEmitter].nParLife[1]--;
				}
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0] = 0;
			g_aEditEmitter[g_nSelectParticleEmitter].nParLife[1] = 0;
		}

		break;

	case PARTICLE_APPEAR:			// 出る間隔-------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// 加算する
			g_aEditEmitter[g_nSelectParticleEmitter].nAppear++;
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// 減算する
			if (g_aEditEmitter[g_nSelectParticleEmitter].nAppear > 1)
			{// 0より下げない
				g_aEditEmitter[g_nSelectParticleEmitter].nAppear--;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_aEditEmitter[g_nSelectParticleEmitter].nAppear = 0;
		}

		break;

	case PARTICLE_SPREAD:			// 広がり-------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// 加算する
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// 加算する量を増やす
				g_aEditEmitter[g_nSelectParticleEmitter].fSpread += 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fSpread += 0.01f;
			}
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// 減算する
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// 減算する量を増やす
				g_aEditEmitter[g_nSelectParticleEmitter].fSpread -= 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fSpread -= 0.01f;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_aEditEmitter[g_nSelectParticleEmitter].fSpread = 0;
		}

		break;

	case PARTICLE_LAP:				// 1周に出す個数-------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// 加算する
			g_aEditEmitter[g_nSelectParticleEmitter].nLap++;
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// 減算する
			if (g_aEditEmitter[g_nSelectParticleEmitter].nLap > 1)
			{// 0より下げない
				g_aEditEmitter[g_nSelectParticleEmitter].nLap--;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_aEditEmitter[g_nSelectParticleEmitter].nLap = 0;
		}

		break;

	case PARTICLE_GRAVITY:			// 重力---------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// 加算する
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// 加算する量を増やす
				g_aEditEmitter[g_nSelectParticleEmitter].fGravity += 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fGravity += 0.01f;
			}
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// 減算する
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// 減算する量を増やす
				g_aEditEmitter[g_nSelectParticleEmitter].fGravity -= 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fGravity -= 0.01f;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_aEditEmitter[g_nSelectParticleEmitter].fGravity = 0;
		}

		break;

	case PARTICLE_ROTATE:			// 回転のパターン-------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// 加算する
			g_aEditEmitter[g_nSelectParticleEmitter].nRot = (g_aEditEmitter[g_nSelectParticleEmitter].nRot + 1) % 4;
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// 減算する
			g_aEditEmitter[g_nSelectParticleEmitter].nRot--;

			if (g_aEditEmitter[g_nSelectParticleEmitter].nRot < 0)
			{// 0より下げない
				g_aEditEmitter[g_nSelectParticleEmitter].nRot = 3;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_aEditEmitter[g_nSelectParticleEmitter].nRot = 0;
		}

		break;

	case PARTICLE_ROTATE_SPEED:		// 回転する速度-----------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// 加算する
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// 加算する量を増やす
				g_aEditEmitter[g_nSelectParticleEmitter].fRotSpeed += 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fRotSpeed += 0.01f;
			}
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// 減算する
			if (GetKeyboardPress(DIK_LSHIFT) == true)
			{// 減算する量を増やす
				g_aEditEmitter[g_nSelectParticleEmitter].fRotSpeed -= 1.0f;
			}
			else
			{
				g_aEditEmitter[g_nSelectParticleEmitter].fRotSpeed -= 0.01f;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_aEditEmitter[g_nSelectParticleEmitter].fRotSpeed = 0;
		}

		break;

	case PARTICLE_LOOP:				// ループするかどうか-------------------------------------------------------------------------------
		if (GetKeyboardTrigger(NUM_UP.nDIK) == true || GetKeyboardTrigger(NUM_DOWN.nDIK) == true)
		{
			g_aEditEmitter[g_nSelectParticleEmitter].bLoop = g_aEditEmitter[g_nSelectParticleEmitter].bLoop ? false : true;
		}

		break;

	case PARTICLE_BOUNCING:			// バウンドするかどうか-------------------------------------------------------------------------------
		if (GetKeyboardTrigger(NUM_UP.nDIK) == true || GetKeyboardTrigger(NUM_DOWN.nDIK) == true)
		{
			g_aEditEmitter[g_nSelectParticleEmitter].bBouncing = g_aEditEmitter[g_nSelectParticleEmitter].bBouncing ? false : true;
		}

		break;

	case PARTICLE_TEXTURE:			// テクスチャ-------------------------------------------------------------------------------
		if (GetKeyboardRepeat(NUM_UP.nDIK) == true)
		{// 加算する
			g_aEditEmitter[g_nSelectParticleEmitter].nType = (g_aEditEmitter[g_nSelectParticleEmitter].nType + 1) % g_nNumParticleTexture;
		}
		else if (GetKeyboardRepeat(NUM_DOWN.nDIK) == true)
		{// 減算する
			g_aEditEmitter[g_nSelectParticleEmitter].nType--;

			if (g_aEditEmitter[g_nSelectParticleEmitter].nType < 0)
			{// 0を越すと最大にする
				g_aEditEmitter[g_nSelectParticleEmitter].nType = g_nNumParticleTexture - 1;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// リセット
			g_aEditEmitter[g_nSelectParticleEmitter].nType = 0;
		}

		break;

	case PARTICLE_DRAW:			// 加算合成するかしないか-------------------------------------------------------------------------------
		if (GetKeyboardTrigger(NUM_UP.nDIK) == true || GetKeyboardTrigger(NUM_DOWN.nDIK) == true)
		{
			g_aEditEmitter[g_nSelectParticleEmitter].bDraw = g_aEditEmitter[g_nSelectParticleEmitter].bDraw ? false : true;
		}

		break;

	}
}

//=============================================================================
// エミッタの設定
//=============================================================================
void SetParticleEmitter(D3DXVECTOR3 pos)
{
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		if (g_aEmitter[nCntEmitter].bUse == false)
		{// 使用されている状態にする
			g_aEmitter[nCntEmitter].pos = pos;
			g_aEmitter[nCntEmitter].nWidth[0] = g_aEditEmitter[g_nSelectParticleEmitter].nWidth[0];
			g_aEmitter[nCntEmitter].nWidth[1] = g_aEditEmitter[g_nSelectParticleEmitter].nWidth[1];
			g_aEmitter[nCntEmitter].nHeight[0] = g_aEditEmitter[g_nSelectParticleEmitter].nHeight[0];
			g_aEmitter[nCntEmitter].nHeight[1] = g_aEditEmitter[g_nSelectParticleEmitter].nHeight[1];
			g_aEmitter[nCntEmitter].nDepth[0] = g_aEditEmitter[g_nSelectParticleEmitter].nDepth[0];
			g_aEmitter[nCntEmitter].nDepth[1] = g_aEditEmitter[g_nSelectParticleEmitter].nDepth[1];
			g_aEmitter[nCntEmitter].col = g_aEditEmitter[g_nSelectParticleEmitter].col;
			g_aEmitter[nCntEmitter].flameCol = g_aEditEmitter[g_nSelectParticleEmitter].flameCol;
			g_aEmitter[nCntEmitter].nRadius[0] = g_aEditEmitter[g_nSelectParticleEmitter].nRadius[0];
			g_aEmitter[nCntEmitter].nRadius[1] = g_aEditEmitter[g_nSelectParticleEmitter].nRadius[1];
			g_aEmitter[nCntEmitter].fFlameRadius = g_aEditEmitter[g_nSelectParticleEmitter].fFlameRadius;
			g_aEmitter[nCntEmitter].fSpread = g_aEditEmitter[g_nSelectParticleEmitter].fSpread;
			g_aEmitter[nCntEmitter].nAppear = g_aEditEmitter[g_nSelectParticleEmitter].nAppear;
			g_aEmitter[nCntEmitter].nParLife[0] = g_aEditEmitter[g_nSelectParticleEmitter].nParLife[0];
			g_aEmitter[nCntEmitter].nParLife[1] = g_aEditEmitter[g_nSelectParticleEmitter].nParLife[1];
			g_aEmitter[nCntEmitter].nLap = g_aEditEmitter[g_nSelectParticleEmitter].nLap;
			g_aEmitter[nCntEmitter].nLife = g_aEditEmitter[g_nSelectParticleEmitter].nLife;
			g_aEmitter[nCntEmitter].nRot = g_aEditEmitter[g_nSelectParticleEmitter].nRot;
			g_aEmitter[nCntEmitter].fRotSpeed = g_aEditEmitter[g_nSelectParticleEmitter].fRotSpeed;
			g_aEmitter[nCntEmitter].nType = g_aEditEmitter[g_nSelectParticleEmitter].nType;
			g_aEmitter[nCntEmitter].bLoop = g_aEditEmitter[g_nSelectParticleEmitter].bLoop;
			g_aEmitter[nCntEmitter].bBouncing = g_aEditEmitter[g_nSelectParticleEmitter].bBouncing;
			g_aEmitter[nCntEmitter].fGravity = g_aEditEmitter[g_nSelectParticleEmitter].fGravity;
			g_aEmitter[nCntEmitter].bDraw = g_aEditEmitter[g_nSelectParticleEmitter].bDraw;

			g_aEmitter[nCntEmitter].bUse = true;
			break;
		}
	}
}

//=============================================================================
// エミッタの位置の設定
//=============================================================================
void SetPosParticleEmitter(int nIdx, D3DXVECTOR3 pos)
{
	if (g_aEmitter[nIdx].bUse == true)
	{// 位置を更新
		g_aEmitter[nIdx].pos = pos;
	}
}

//=============================================================================
// エミッタの削除
//=============================================================================
void DestroyParticle(int nIdx)
{
	if (g_aEmitter[nIdx].bUse == true)
	{// 使っていない状態にする
		g_aEmitter[nIdx].bUse = false;
	}
}

//=============================================================================
// ファイルからデータを取得
//=============================================================================
void FileInputParticle(void)
{
	FILE *pFile;
	int nCntEffect = 0;
	char aStr[128];

	pFile = fopen(PARTICLE_FILENAME, "r");

	if (NULL != pFile)
	{
		while (strcmp(aStr, "END_SCRIPT") != 0)
		{// 終了メッセージまでループ
			fscanf(pFile, "%s", aStr);
			if (strcmp(aStr, "PARTICLESET") == 0)
			{// エミッタの情報
				while (strcmp(aStr, "END_PARTICLESET") != 0)
				{// 終了メッセージまでループ
					fscanf(pFile, "%s", &aStr);

					if (strcmp(aStr, "MAXMOVE") == 0)
					{// 移動量の最大
						fscanf(pFile, " = %d %d %d", &g_aEditEmitter[nCntEffect].nWidth[1], &g_aEditEmitter[nCntEffect].nHeight[1], &g_aEditEmitter[nCntEffect].nDepth[1]);
					}
					else if (strcmp(aStr, "MINMOVE") == 0)
					{// 移動量の最小
						fscanf(pFile, " = %d %d %d", &g_aEditEmitter[nCntEffect].nWidth[0], &g_aEditEmitter[nCntEffect].nHeight[0], &g_aEditEmitter[nCntEffect].nDepth[0]);
					}
					else if (strcmp(aStr, "INITCOL") == 0)
					{// 初期カラー
						fscanf(pFile, " = %f %f %f %f", &g_aEditEmitter[nCntEffect].col.r, &g_aEditEmitter[nCntEffect].col.g, &g_aEditEmitter[nCntEffect].col.b, &g_aEditEmitter[nCntEffect].col.a);
					}
					else if (strcmp(aStr, "FLAMECOL") == 0)
					{// フレームによるカラーの変化
						fscanf(pFile, " = %f %f %f %f", &g_aEditEmitter[nCntEffect].flameCol.r, &g_aEditEmitter[nCntEffect].flameCol.g, &g_aEditEmitter[nCntEffect].flameCol.b, &g_aEditEmitter[nCntEffect].flameCol.a);
					}
					else if (strcmp(aStr, "INITSIZE") == 0)
					{// 初期サイズ
						fscanf(pFile, " = %d %d", &g_aEditEmitter[nCntEffect].nRadius[0], &g_aEditEmitter[nCntEffect].nRadius[1]);
					}
					else if (strcmp(aStr, "FLAMESIZE") == 0)
					{// フレームによるサイズの変化
						fscanf(pFile, " = %f", &g_aEditEmitter[nCntEffect].fFlameRadius);
					}
					else if (strcmp(aStr, "EMITTERLIFE") == 0)
					{// 表示時間(寿命)
						fscanf(pFile, " = %d", &g_aEditEmitter[nCntEffect].nLife);
					}
					else if (strcmp(aStr, "PARLIFE") == 0)
					{// パーティクルの表示時間
						fscanf(pFile, " = %d %d", &g_aEditEmitter[nCntEffect].nParLife[0], &g_aEditEmitter[nCntEffect].nParLife[1]);
					}
					else if (strcmp(aStr, "APPEAR") == 0)
					{// 出る間隔
						fscanf(pFile, " = %d", &g_aEditEmitter[nCntEffect].nAppear);
					}
					else if (strcmp(aStr, "SPREAD") == 0)
					{// 広がり
						fscanf(pFile, " = %f", &g_aEditEmitter[nCntEffect].fSpread);
					}
					else if (strcmp(aStr, "LAP") == 0)
					{// 1周に出す個数
						fscanf(pFile, " = %d", &g_aEditEmitter[nCntEffect].nLap);
					}
					else if (strcmp(aStr, "GRAVITY") == 0)
					{// 重力
						fscanf(pFile, " = %f", &g_aEditEmitter[nCntEffect].fGravity);
					}
					else if (strcmp(aStr, "ROTPATTERN") == 0)
					{// 回転のパターン
						fscanf(pFile, " = %d", &g_aEditEmitter[nCntEffect].nRot);
					}
					else if (strcmp(aStr, "ROTSPEED") == 0)
					{// 回転の速度
						fscanf(pFile, " = %f", &g_aEditEmitter[nCntEffect].fRotSpeed);
					}
					else if (strcmp(aStr, "LOOP") == 0)
					{// ループするかどうか
						fscanf(pFile, " = %d", (int*)&g_aEditEmitter[nCntEffect].bLoop);
					}
					else if (strcmp(aStr, "BOUND") == 0)
					{// バウンドするかどうか
						fscanf(pFile, " = %d", (int*)&g_aEditEmitter[nCntEffect].bBouncing);
					}
					else if (strcmp(aStr, "TEXTURE") == 0)
					{// テクスチャの種類
						fscanf(pFile, " = %d", &g_aEditEmitter[nCntEffect].nType);
					}
					else if (strcmp(aStr, "DRAW") == 0)
					{// 加算合成の有無
						fscanf(pFile, " = %d", (int*)&g_aEditEmitter[nCntEffect].bDraw);
					}
				}
				nCntEffect++;	// エミッタ番号のカウント
			}
		}

		fclose(pFile);
	}
	else
	{// 失敗
		MessageBox(0, "ファイルが開けませんでした", "effect.txt", MB_OK);
	}
}

//=============================================================================
// ファイルにデータを書き込み
//=============================================================================
void FileOutputParticle(void)
{
	FILE *pFile;
	int nCntEffect;

	pFile = fopen(PARTICLE_SAVEFILE, "w");

	if (NULL != pFile)
	{
		fprintf(pFile, "#********************************************************************\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#    【EffectCreater】スクリプトファイル	[save_particle.txt]\n");
		fprintf(pFile, "#    Auther : Ishida Takuto\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#********************************************************************\n");

		fprintf(pFile, "SCRIPT\n\n");
		fprintf(pFile, "#******************************************\n");
		fprintf(pFile, "#	パーティクルの設定\n");
		fprintf(pFile, "#******************************************\n");

		for (nCntEffect = 0; nCntEffect < MAX_EMITTER_TYPE; nCntEffect++)
		{// データ分まわす
			fprintf(pFile, "PARTICLESET		[ %d ]\n", nCntEffect);
			fprintf(pFile, "	MAXMOVE = %d %d %d					＃移動量の最大\n", g_aEditEmitter[nCntEffect].nWidth[1], g_aEditEmitter[nCntEffect].nHeight[1], g_aEditEmitter[nCntEffect].nDepth[1]);	// 移動量の最大
			fprintf(pFile, "	MINMOVE = %d %d %d					＃移動量の最小\n", g_aEditEmitter[nCntEffect].nWidth[0], g_aEditEmitter[nCntEffect].nHeight[0], g_aEditEmitter[nCntEffect].nDepth[0]);	// 移動量の最小
			fprintf(pFile, "	INITCOL = %.2f %.2f %.2f %.2f	＃初期カラー\n", g_aEditEmitter[nCntEffect].col.r, g_aEditEmitter[nCntEffect].col.g, g_aEditEmitter[nCntEffect].col.b, g_aEditEmitter[nCntEffect].col.a);	// 色
			fprintf(pFile, "	FLAMECOL = %.2f %.2f %.2f %.2f	＃フレーム毎のカラーの変化\n", g_aEditEmitter[nCntEffect].flameCol.r, g_aEditEmitter[nCntEffect].flameCol.g, g_aEditEmitter[nCntEffect].flameCol.b, g_aEditEmitter[nCntEffect].flameCol.a);	// 色
			fprintf(pFile, "	INITSIZE = %d %d					＃初期サイズ\n", g_aEditEmitter[nCntEffect].nRadius[0], g_aEditEmitter[nCntEffect].nRadius[1]);			// サイズ
			fprintf(pFile, "	FLAMESIZE = %f			＃フレーム毎のサイズの変化\n", g_aEditEmitter[nCntEffect].fFlameRadius);	// サイズの加算
			fprintf(pFile, "	EMITTERLIFE = %d					＃エミッタの寿命\n", g_aEditEmitter[nCntEffect].nLife);			// 表示時間(寿命)
			fprintf(pFile, "	PARLIFE = %d %d					＃ポリゴンの寿命\n", g_aEditEmitter[nCntEffect].nParLife[0], g_aEditEmitter[nCntEffect].nParLife[1]);		// パーティクルの表示時間
			fprintf(pFile, "	APPEAR = %d						＃出現間隔\n", g_aEditEmitter[nCntEffect].nAppear);			// 出る間隔
			fprintf(pFile, "	SPREAD = %.2f					＃拡散\n", g_aEditEmitter[nCntEffect].fSpread);		// 広がり
			fprintf(pFile, "	LAP = %d							＃1周に出す個数\n", g_aEditEmitter[nCntEffect].nLap);			// 1周に出す個数
			fprintf(pFile, "	GRAVITY = %f				＃重力\n", g_aEditEmitter[nCntEffect].fGravity);		// 重力
			fprintf(pFile, "	ROTPATTERN = %d					＃回転のパターン\n", g_aEditEmitter[nCntEffect].nRot);			// 回転のパターン
			fprintf(pFile, "	ROTSPEED = %f				＃回転の速度\n", g_aEditEmitter[nCntEffect].fRotSpeed);		// 回転の速度
			fprintf(pFile, "	LOOP = %d						＃ループ[0:なし　1:あり]\n", g_aEditEmitter[nCntEffect].bLoop);			// ループするかどうか
			fprintf(pFile, "	BOUND = %d						＃バウンド[0:なし　1:あり]\n", g_aEditEmitter[nCntEffect].bBouncing);		// バウンドするかどうか
			fprintf(pFile, "	TEXTURE = %d						＃テクスチャの種類\n", g_aEditEmitter[nCntEffect].nType);			// テクスチャの種類
			fprintf(pFile, "	DRAW = %d						＃加算合成[0:なし　1:あり]\n", g_aEditEmitter[nCntEffect].bDraw);			// 加算合成の有無
			fprintf(pFile, "END_PARTICLESET");
			fprintf(pFile, "\n\n");
		}
		fprintf(pFile, "\nEND_SCRIPT\n");

		fclose(pFile);
	}
	else
	{// 失敗
		MessageBox(0, "ファイルが開けませんでした", "particle.txt", MB_OK);
	}
}

//=============================================================================
// ファイルからテクスチャを取得
//=============================================================================
void TextureInputParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	char aFileName[MAX_TEXTURE][64];		// テクスチャの名前
	int nCntTexture;

	pFile = fopen(TEXTURE_FILENAME, "r");

	if (NULL != pFile)
	{// 開けた
		fscanf(pFile, "%d", &g_nNumParticleTexture);		// テクスチャ数

		for (nCntTexture = 0; nCntTexture < g_nNumParticleTexture; nCntTexture++)
		{
			fscanf(pFile, "%s\n", &aFileName[nCntTexture][0]);		// テクスチャ数
		}
		fclose(pFile);
	}
	else
	{// 失敗
		MessageBox(0, "ファイルが開けませんでした", "texture.txt", MB_OK);
	}

	// テクスチャの読み込み
	for (nCntTexture = 0; nCntTexture < g_nNumParticleTexture; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice, &aFileName[nCntTexture][0], &g_pTextureParticle[nCntTexture]);
	}
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
