//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "particle.h"
#include "string.h"
#include "input.h"
#include "dragon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PARTICLE_FILENAME	"data/TEXT/particle.txt"				// パーティクルデータ
#define TEXTURE_FILENAME	"data/TEXT/particle_texture.txt"		// テクスチャ名

#define MAX_EMITTER_TYPE	(32)							// パーティクルの種類の最大数
#define MAX_PARTICLE		(1024)							// パーティクルの最大数

#define MAX_TEXTURE			(32)								// テクスチャの最大数
#define MAX_TYPE			(32)								// 項目の最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);

void FileInputParticle(void);
void TextureInputParticle(void);

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

int g_nCntAngle;						// パーティクルの角度計算用カウンタ

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
		g_aParticle[nCntParticle].fBouncing = 0.0f;
		g_aParticle[nCntParticle].nIdx = 0;
		g_aParticle[nCntParticle].bUse = false;
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
		g_aEmitter[nCntEmitter].bLoop = false;
		g_aEmitter[nCntEmitter].bBouncing = false;
		g_aEmitter[nCntEmitter].bUse = false;
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
		g_aEditEmitter[nCntEmitter].nType = 0;
		g_aEditEmitter[nCntEmitter].bLoop = false;
		g_aEditEmitter[nCntEmitter].bBouncing = false;
		g_aEditEmitter[nCntEmitter].bUse = false;
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
						g_aParticle[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntAngle))) * ((g_aEmitter[nCntEmitter].nWidth[0] + (rand() % (1 + (g_aEmitter[nCntEmitter].nWidth[1] * 2)) - 1)) + ((rand() % 10) * 0.1f)) * g_aEmitter[nCntEmitter].fSpread,
							((rand() % (1 + g_aEmitter[nCntEmitter].nHeight[1])) + g_aEmitter[nCntEmitter].nHeight[0]) + ((rand() % 10) * 0.1f),
							cosf(D3DX_PI * (0.0f + (g_aParticle[nCntParticle].fAngle * g_nCntAngle))) * ((g_aEmitter[nCntEmitter].nDepth[0] + (rand() % (1 + (g_aEmitter[nCntEmitter].nDepth[1] * 2)) - 1)) + ((rand() % 10) * 0.1f)) * g_aEmitter[nCntEmitter].fSpread);
						g_aParticle[nCntParticle].col = g_aEmitter[nCntEmitter].col;
						g_aParticle[nCntParticle].fRadius = g_aEmitter[nCntEmitter].nRadius[0] + (rand() % (1 + (g_aEmitter[nCntEmitter].nRadius[1] - g_aEmitter[nCntEmitter].nRadius[0])) - 1) + ((rand() % 10) * 0.1f);
						g_aParticle[nCntParticle].nLife = g_aEmitter[nCntEmitter].nParLife[0] + (rand() % (1 + (g_aEmitter[nCntEmitter].nParLife[1] - g_aEmitter[nCntEmitter].nParLife[0])) - 1);
						g_aParticle[nCntParticle].nType = g_aEmitter[nCntEmitter].nType;
						g_aParticle[nCntParticle].nIdx = nCntEmitter;
						g_aParticle[nCntParticle].fGravity = g_aEmitter[nCntEmitter].fGravity;
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

			if (6 == g_aParticle[nCntParticle].nType)
			{// ドラゴンの火
				D3DXVECTOR3 rot = GetNeckRot();

				g_aParticle[nCntParticle].move -= D3DXVECTOR3(sinf(rot.y) * 0.3f, 0.0f, cosf(rot.y) * 0.3f);
			}

			g_aParticle[nCntParticle].move.y -= g_aParticle[nCntParticle].fGravity;		// 重力を加える

			g_aParticle[nCntParticle].fBouncing = g_aParticle[nCntParticle].pos.y;		// 値を記憶

			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;							// 位置の更新
			g_aParticle[nCntParticle].col += g_aEmitter[g_aParticle[nCntParticle].nIdx].flameCol;				// 色の更新
			g_aParticle[nCntParticle].fRadius += g_aEmitter[g_aParticle[nCntParticle].nIdx].fFlameRadius;		// 大きさの更新

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
				g_aParticle[nCntParticle].fCntRot += 0.01f;		// 回転のカウンタ
			}
			else if (g_aEmitter[g_aParticle[nCntParticle].nIdx].nRot == 2)
			{// 反時計回り
				g_aParticle[nCntParticle].fCntRot -= 0.01f;		// 回転のカウンタ
			}
			else if (g_aEmitter[g_aParticle[nCntParticle].nIdx].nRot == 3)
			{// ランダム
				if (nCntParticle % 2 == 0)
				{// 時計回り
					g_aParticle[nCntParticle].fCntRot += 0.01f;		// 回転のカウンタ
				}
				else
				{// 反時計回り
					g_aParticle[nCntParticle].fCntRot -= 0.01f;		// 回転のカウンタ
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
}

//=============================================================================
// ポリゴン描画処理
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// 計算用マトリックス
	int nCntParticle;

	//フォグを使用しない
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

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

	//フォグを使用する
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
}

//=============================================================================
// エミッタの設定
//=============================================================================
void SetParticleEmitter(D3DXVECTOR3 pos, int nType)
{
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		if (g_aEmitter[nCntEmitter].bUse == false)
		{// 使用されている状態にする
			g_aEmitter[nCntEmitter].pos = pos;
			g_aEmitter[nCntEmitter].nWidth[0] = g_aEditEmitter[nType].nWidth[0];
			g_aEmitter[nCntEmitter].nWidth[1] = g_aEditEmitter[nType].nWidth[1];
			g_aEmitter[nCntEmitter].nHeight[0] = g_aEditEmitter[nType].nHeight[0];
			g_aEmitter[nCntEmitter].nHeight[1] = g_aEditEmitter[nType].nHeight[1];
			g_aEmitter[nCntEmitter].nDepth[0] = g_aEditEmitter[nType].nDepth[0];
			g_aEmitter[nCntEmitter].nDepth[1] = g_aEditEmitter[nType].nDepth[1];
			g_aEmitter[nCntEmitter].col = g_aEditEmitter[nType].col;
			g_aEmitter[nCntEmitter].flameCol = g_aEditEmitter[nType].flameCol;
			g_aEmitter[nCntEmitter].nRadius[0] = g_aEditEmitter[nType].nRadius[0];
			g_aEmitter[nCntEmitter].nRadius[1] = g_aEditEmitter[nType].nRadius[1];
			g_aEmitter[nCntEmitter].fFlameRadius = g_aEditEmitter[nType].fFlameRadius;
			g_aEmitter[nCntEmitter].fSpread = g_aEditEmitter[nType].fSpread;
			g_aEmitter[nCntEmitter].nAppear = g_aEditEmitter[nType].nAppear;
			g_aEmitter[nCntEmitter].nParLife[0] = g_aEditEmitter[nType].nParLife[0];
			g_aEmitter[nCntEmitter].nParLife[1] = g_aEditEmitter[nType].nParLife[1];
			g_aEmitter[nCntEmitter].nLap = g_aEditEmitter[nType].nLap;
			g_aEmitter[nCntEmitter].nLife = g_aEditEmitter[nType].nLife;
			g_aEmitter[nCntEmitter].nRot = g_aEditEmitter[nType].nRot;
			g_aEmitter[nCntEmitter].nType = g_aEditEmitter[nType].nType;
			g_aEmitter[nCntEmitter].bLoop = g_aEditEmitter[nType].bLoop;
			g_aEmitter[nCntEmitter].bBouncing = g_aEditEmitter[nType].bBouncing;
			g_aEmitter[nCntEmitter].fGravity = g_aEditEmitter[nType].fGravity;
			g_aEmitter[nCntEmitter].bDraw = g_aEditEmitter[nType].bDraw;

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
	int nCntEffect;
	int nBool;

	pFile = fopen(PARTICLE_FILENAME, "r");

	if (NULL != pFile)
	{
		for (nCntEffect = 0; nCntEffect < MAX_EMITTER_TYPE; nCntEffect++)
		{// データ分まわす
			fscanf(pFile, "%d %d %d", &g_aEditEmitter[nCntEffect].nWidth[1], &g_aEditEmitter[nCntEffect].nHeight[1], &g_aEditEmitter[nCntEffect].nDepth[1]);	// 移動量の最大
			fscanf(pFile, "%d %d %d", &g_aEditEmitter[nCntEffect].nWidth[0], &g_aEditEmitter[nCntEffect].nHeight[0], &g_aEditEmitter[nCntEffect].nDepth[0]);	// 移動量の最小
			fscanf(pFile, "%f %f %f %f", &g_aEditEmitter[nCntEffect].col.r, &g_aEditEmitter[nCntEffect].col.g, &g_aEditEmitter[nCntEffect].col.b, &g_aEditEmitter[nCntEffect].col.a);	// 色
			fscanf(pFile, "%f %f %f %f", &g_aEditEmitter[nCntEffect].flameCol.r, &g_aEditEmitter[nCntEffect].flameCol.g, &g_aEditEmitter[nCntEffect].flameCol.b, &g_aEditEmitter[nCntEffect].flameCol.a);	// 色
			fscanf(pFile, "%d %d", &g_aEditEmitter[nCntEffect].nRadius[0], &g_aEditEmitter[nCntEffect].nRadius[1]);			// サイズ
			fscanf(pFile, "%f", &g_aEditEmitter[nCntEffect].fFlameRadius);	// サイズの加算
			fscanf(pFile, "%d", &g_aEditEmitter[nCntEffect].nLife);			// 表示時間(寿命)
			fscanf(pFile, "%d %d", &g_aEditEmitter[nCntEffect].nParLife[0], &g_aEditEmitter[nCntEffect].nParLife[1]);		// パーティクルの表示時間
			fscanf(pFile, "%d", &g_aEditEmitter[nCntEffect].nAppear);		// 出る間隔
			fscanf(pFile, "%f", &g_aEditEmitter[nCntEffect].fSpread);		// 広がり
			fscanf(pFile, "%d", &g_aEditEmitter[nCntEffect].nLap);			// 1周に出す個数
			fscanf(pFile, "%f", &g_aEditEmitter[nCntEffect].fGravity);		// 重力
			fscanf(pFile, "%d", &g_aEditEmitter[nCntEffect].nRot);			// 回転のパターン
			fscanf(pFile, "%d", &nBool);			// ループするかどうか
			if (nBool == 0)
			{// 0のとき偽
				g_aEditEmitter[nCntEffect].bLoop = false;
			}
			else
			{// 0以外で真
				g_aEditEmitter[nCntEffect].bLoop = true;
			}

			fscanf(pFile, "%d", &nBool);		// バウンドするかどうか
			if (nBool == 0)
			{// 0のとき偽
				g_aEditEmitter[nCntEffect].bBouncing = false;
			}
			else
			{// 0以外で真
				g_aEditEmitter[nCntEffect].bBouncing = true;
			}

			fscanf(pFile, "%d", &g_aEditEmitter[nCntEffect].nType);			// テクスチャの種類
			fscanf(pFile, "%d", &nBool);			// 加算合成の有無
			if (nBool == 0)
			{// 0のとき偽
				g_aEditEmitter[nCntEffect].bDraw = false;
			}
			else
			{// 0以外で真
				g_aEditEmitter[nCntEffect].bDraw = true;
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

//=============================================================================
// パーティクルのリセット
//=============================================================================
void DeleteParticle(void)
{
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
		g_aParticle[nCntParticle].fBouncing = 0.0f;
		g_aParticle[nCntParticle].nIdx = 0;
		g_aParticle[nCntParticle].bUse = false;
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
		g_aEmitter[nCntEmitter].bLoop = false;
		g_aEmitter[nCntEmitter].bBouncing = false;
		g_aEmitter[nCntEmitter].bUse = false;
	}
}
