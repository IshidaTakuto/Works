//=============================================================================
//
// パーティクル処理 [particle2D.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "particle2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PARTICLE_TEXTURENAME	"data/TEXTURE/blood0.jpg"

#define MAX_EMITTER		(16)		// エミッタの最大数
#define MAX_PARTICLE	(500)		// パーティクルの最大数
#define PARTICLE_APPEAR	(2)			// パーティクルの出る間隔
#define NORMAL_TIME		(15)

#define WIDTH_MOVE		(1)		// X軸移動量
#define HEIGHT_MOVE		(1)		// Y軸移動量
#define SPREAD			(0.7f)	// 広がり

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexParticle2D(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureParticle2D = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle2D = NULL;	// 頂点バッファへのポインタ
Particle2D g_aParticle2D[MAX_PARTICLE];
Emitter g_aEmitter2D[MAX_EMITTER];
int g_nCntParticle2D;
int g_nCntAngle2D;

//=============================================================================
// ポリゴン初期化処理
//=============================================================================
void InitParticle2D(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// パーティクルの情報を初期化
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle2D[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle2D[nCntParticle].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle2D[nCntParticle].fRadius = 0;
		g_aParticle2D[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle2D[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle2D[nCntParticle].type = PARTICLETYPE_NONE;
		g_aParticle2D[nCntParticle].nLife = 0;
		g_aParticle2D[nCntParticle].bUse = false;
	}

	// エミッタの情報を初期化
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		g_aEmitter2D[nCntEmitter].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEmitter2D[nCntEmitter].fRadius = 0;
		g_aEmitter2D[nCntEmitter].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEmitter2D[nCntEmitter].type = PARTICLETYPE_NONE;
		g_aEmitter2D[nCntEmitter].bUse = false;
	}

	g_nCntParticle2D = 0;
	g_nCntAngle2D = 0;

	// パーティクルの頂点情報の作成
	MakeVertexParticle2D(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PARTICLE_TEXTURENAME, &g_pTextureParticle2D);
}

//=============================================================================
// ポリゴン終了処理
//=============================================================================
void UninitParticle2D(void)
{
	// テクスチャの破棄
	if (g_pTextureParticle2D != NULL)
	{
		g_pTextureParticle2D->Release();
		g_pTextureParticle2D = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffParticle2D != NULL)
	{
		g_pVtxBuffParticle2D->Release();
		g_pVtxBuffParticle2D = NULL;
	}
}

//=============================================================================
// ポリゴン更新処理
//=============================================================================
void UpdateParticle2D(void)
{
	int nCntParticle;	// for分用のカウンタ

	// 出現させる間隔のカウント
	g_nCntParticle2D = (g_nCntParticle2D + 1) % PARTICLE_APPEAR;

	if (g_nCntParticle2D == 0)
	{// 出現させる
		for (int nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
		{
			if (g_aEmitter2D[nCntEmitter].bUse == true)
			{// 使用されているエミッタ

				g_aEmitter2D[nCntEmitter].nLife--;	// エミッタの表示時間をカウント

				if (g_aEmitter2D[nCntEmitter].nLife <= 0)
				{// 一定時間経過でエミッタを消す
					g_aEmitter2D[nCntEmitter].bUse = false;
				}

				for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
				{// パーティクルをつける
					if (g_aParticle2D[nCntParticle].bUse == false)
					{// 使われていない
						g_aParticle2D[nCntParticle].pos = g_aEmitter2D[nCntEmitter].pos;
						g_aParticle2D[nCntParticle].fAngle = 1.0f / (13 / 2);
						if (nCntParticle % 2 == 0)
						{
							g_aParticle2D[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (0.0f + (g_aParticle2D[nCntParticle].fAngle * g_nCntAngle2D))) * (WIDTH_MOVE + ((rand() % 10) * 0.1f)) * SPREAD,
								cosf(D3DX_PI * (0.0f + (g_aParticle2D[nCntParticle].fAngle * g_nCntAngle2D))) * (HEIGHT_MOVE + ((rand() % 10) * 0.1f)) * SPREAD,
								0.0f);
						}
						else
						{
							g_aParticle2D[nCntParticle].move = D3DXVECTOR3(sinf(D3DX_PI * (1.0f + (g_aParticle2D[nCntParticle].fAngle * g_nCntAngle2D))) * (WIDTH_MOVE + ((rand() % 10) * 0.1f)) * SPREAD,
								cosf(D3DX_PI * (1.0f + (g_aParticle2D[nCntParticle].fAngle * g_nCntAngle2D))) * (HEIGHT_MOVE + ((rand() % 10) * 0.1f)) * SPREAD,
								0.0f);
						}

						g_aParticle2D[nCntParticle].nIdx = nCntEmitter;
						g_aParticle2D[nCntParticle].bUse = true;
						g_aParticle2D[nCntParticle].nLife = NORMAL_TIME;
						g_aParticle2D[nCntParticle].col = g_aEmitter2D[nCntEmitter].col;
						g_aParticle2D[nCntParticle].fRadius = g_aEmitter2D[nCntEmitter].fRadius;

						g_nCntAngle2D = (g_nCntAngle2D + 1) % 13;
						break;
					}
				}
			}
		}
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// パーティクルの更新
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffParticle2D->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle2D[nCntParticle].bUse == true)
		{// 使われている
			g_aParticle2D[nCntParticle].nLife--;		// 表示時間をカウント

			// 表示時間が0以下になった
			if (g_aParticle2D[nCntParticle].nLife <= 0)
			{// 使用していない状態にする
				g_aParticle2D[nCntParticle].bUse = false;
			}

			g_aParticle2D[nCntParticle].pos += g_aParticle2D[nCntParticle].move;					// 位置の更新
			g_aParticle2D[nCntParticle].col += D3DXCOLOR(0.01f, 0.01f, 0.01f, 0.0f);				// 色の更新
			g_aParticle2D[nCntParticle].fRadius += 0.01f;		// 大きさの更新

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aParticle2D[nCntParticle].fRadius, -g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aParticle2D[nCntParticle].fRadius, -g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aParticle2D[nCntParticle].fRadius, g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aParticle2D[nCntParticle].fRadius, g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;

			//頂点カラー
			pVtx[0].col = g_aParticle2D[nCntParticle].col;
			pVtx[1].col = g_aParticle2D[nCntParticle].col;
			pVtx[2].col = g_aParticle2D[nCntParticle].col;
			pVtx[3].col = g_aParticle2D[nCntParticle].col;
		}
		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffParticle2D->Unlock();
}

//=============================================================================
// ポリゴン描画処理
//=============================================================================
void DrawParticle2D(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// 計算用マトリックス
	int nCntParticle;

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffParticle2D, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureParticle2D);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ポリゴンの描画
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle2D[nCntParticle].bUse == true)	//使用されている
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntParticle, 2);
		}
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// パーティクルの設定
//=============================================================================
void SetParticle2D(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius,int nLife, PARTICLETYPE type)
{
	int nCntEmitter;

	for (nCntEmitter = 0; nCntEmitter < MAX_EMITTER; nCntEmitter++)
	{
		if (g_aEmitter2D[nCntEmitter].bUse == false)
		{
			g_aEmitter2D[nCntEmitter].pos = pos;
			g_aEmitter2D[nCntEmitter].col = col;
			g_aEmitter2D[nCntEmitter].fRadius = fRadius;
			g_aEmitter2D[nCntEmitter].nLife = nLife;
			g_aEmitter2D[nCntEmitter].type = type;

			g_aEmitter2D[nCntEmitter].bUse = true;
			break;
		}
	}
}

//=============================================================================
// パーティクルの頂点情報の作成
//=============================================================================
void MakeVertexParticle2D(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle2D,
		NULL);

	VERTEX_2D*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffParticle2D->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の座標
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		pVtx[0].pos = D3DXVECTOR3(-g_aParticle2D[nCntParticle].fRadius, -g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;
		pVtx[1].pos = D3DXVECTOR3(g_aParticle2D[nCntParticle].fRadius, -g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;
		pVtx[2].pos = D3DXVECTOR3(-g_aParticle2D[nCntParticle].fRadius, g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;
		pVtx[3].pos = D3DXVECTOR3(g_aParticle2D[nCntParticle].fRadius, g_aParticle2D[nCntParticle].fRadius, 0.0f) + g_aParticle2D[nCntParticle].pos;

		// 法線の設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

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
	g_pVtxBuffParticle2D->Unlock();
}
