//=============================================================================
//
// コンボ処理 [combo.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "combo.h"
#include "score.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_COMBO_00 	"data/TEXTURE/number.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_COMBO_01	"data/TEXTURE/game_combo.png"	// 読み込むテクスチャファイル名
#define NUM_COMBO				(3)							// コンボの最大桁数
#define SIZE_COMBO				(50)						// コンボの大きさ
#define MAX_SIZE_COMBO			(40)						// 大きさの最大値
#define MAX_TEX_COMBO			(2)							// テクスチャの数
#define SIZE_LARGE_COMBO		(20)						// どれくらい大きくするか
#define SIZE_SMALL_COMBO		(0.1f)						// 差分にかける値
#define TRANSPARENT_TIME_COMBO	(120)						// 透明化するまでの時間
#define MAX_COMBO_POLYGON		(NUM_COMBO + 1)				// 表示するポリゴン数
#define LOGO_SIZE_COMBO			(60)						// コンボの文字の大きさ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexCombo(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureCombo[MAX_TEX_COMBO] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCombo = NULL;					// 頂点バッファへのポインタ
Combo					g_aCombo[MAX_COMBO_POLYGON];			// コンボ数を桁ごとに格納
int g_nNumCombo;												// コンボ数
int g_aNumLogCombo[NUM_COMBO];									// 桁ごとのコンボ数を保存
D3DXVECTOR3 g_aDiffSizeComco[NUM_COMBO][4];						// 元のサイズ
int g_nCntComboTime;											// 透明化させるまでのカウンタ
int g_nCntKillCombo;

//=============================================================================
// 初期化処理
//=============================================================================
void InitCombo(D3DXVECTOR3 pos)
{
	/***********************************************************
	*
	*	Initを呼ぶときに表示する座標を引数にいれてください
	*
	************************************************************/

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntScore = 0; nCntScore < NUM_COMBO; nCntScore++)
	{
		// コンボの初期化
		g_aCombo[nCntScore].pos = D3DXVECTOR3(pos.x - (nCntScore * 70.0f), pos.y, 0.0f);
		g_aCombo[nCntScore].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		g_aCombo[nCntScore].nType = 0;

		g_aNumLogCombo[nCntScore] = 0;
	}

	g_aCombo[NUM_COMBO].pos = D3DXVECTOR3(pos.x - 70.0f, pos.y - 60.0f, 0.0f);
	g_aCombo[NUM_COMBO].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	g_aCombo[NUM_COMBO].nType = 1;

	g_nCntComboTime = 0;	// コンボが透明化するまでの時間
	g_nNumCombo = 0;			// コンボ数の初期化
	g_nCntKillCombo = 0;	// キルコンボの初期化

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_COMBO_00, &g_pTextureCombo[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_COMBO_01, &g_pTextureCombo[1]);

	// 頂点情報の作成
	MakeVertexCombo(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCombo(void)
{
	for (int nCntScore = 0; nCntScore < MAX_TEX_COMBO; nCntScore++)
	{
		// テクスチャの開放
		if (g_pTextureCombo[nCntScore] != NULL)
		{
			g_pTextureCombo[nCntScore]->Release();
			g_pTextureCombo[nCntScore] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCombo != NULL)
	{
		g_pVtxBuffCombo->Release();
		g_pVtxBuffCombo = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCombo(void)
{
	D3DXVECTOR3 aDestSize[4];	// サイズの差分を求めるのに必要
	D3DXCOLOR col[4];

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nNumCombo > 0)
	{// コンボが続いているとき
		for (int nCntVtx = 0; nCntVtx < NUM_COMBO; nCntVtx++)
		{
			col[0] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			col[1] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			col[2] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			col[3] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			if (g_nNumCombo >= pow(10, nCntVtx))
			{// 必要な桁数分だけ
				if (pVtx[(nCntVtx * 4) + 0].pos.x < g_aDiffSizeComco[nCntVtx][0].x)
				{// サイズを戻す
					aDestSize[0].x = g_aDiffSizeComco[nCntVtx][0].x - pVtx[(nCntVtx * 4) + 0].pos.x;
					aDestSize[0].y = g_aDiffSizeComco[nCntVtx][0].y - pVtx[(nCntVtx * 4) + 0].pos.y;
					aDestSize[1].x = g_aDiffSizeComco[nCntVtx][1].x - pVtx[(nCntVtx * 4) + 1].pos.x;
					aDestSize[1].y = g_aDiffSizeComco[nCntVtx][1].y - pVtx[(nCntVtx * 4) + 1].pos.y;
					aDestSize[2].x = g_aDiffSizeComco[nCntVtx][2].x - pVtx[(nCntVtx * 4) + 2].pos.x;
					aDestSize[2].y = g_aDiffSizeComco[nCntVtx][2].y - pVtx[(nCntVtx * 4) + 2].pos.y;
					aDestSize[3].x = g_aDiffSizeComco[nCntVtx][3].x - pVtx[(nCntVtx * 4) + 3].pos.x;
					aDestSize[3].y = g_aDiffSizeComco[nCntVtx][3].y - pVtx[(nCntVtx * 4) + 3].pos.y;

					//頂点座標の設定
					pVtx[(nCntVtx * 4) + 0].pos.x += aDestSize[0].x * SIZE_SMALL_COMBO;
					pVtx[(nCntVtx * 4) + 0].pos.y += aDestSize[0].y * SIZE_SMALL_COMBO;
					pVtx[(nCntVtx * 4) + 1].pos.x += aDestSize[1].x * SIZE_SMALL_COMBO;
					pVtx[(nCntVtx * 4) + 1].pos.y += aDestSize[1].y * SIZE_SMALL_COMBO;
					pVtx[(nCntVtx * 4) + 2].pos.x += aDestSize[2].x * SIZE_SMALL_COMBO;
					pVtx[(nCntVtx * 4) + 2].pos.y += aDestSize[2].y * SIZE_SMALL_COMBO;
					pVtx[(nCntVtx * 4) + 3].pos.x += aDestSize[3].x * SIZE_SMALL_COMBO;
					pVtx[(nCntVtx * 4) + 3].pos.y += aDestSize[3].y * SIZE_SMALL_COMBO;

					col[0] -= pVtx[(nCntVtx * 4) + 0].col;
					col[1] -= pVtx[(nCntVtx * 4) + 1].col;
					col[2] -= pVtx[(nCntVtx * 4) + 2].col;
					col[3] -= pVtx[(nCntVtx * 4) + 3].col;
				}

				//頂点カラー
				pVtx[(nCntVtx * 4) + 0].col += col[0] * 0.02f;
				pVtx[(nCntVtx * 4) + 1].col += col[1] * 0.02f;
				pVtx[(nCntVtx * 4) + 2].col += col[2] * 0.02f;
				pVtx[(nCntVtx * 4) + 3].col += col[3] * 0.02f;

				// 透明化させる
				if (g_nCntComboTime > TRANSPARENT_TIME_COMBO)
				{// 一定時間経過
						//頂点カラー
					pVtx[(nCntVtx * 4) + 0].col -= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.03f);
					pVtx[(nCntVtx * 4) + 1].col -= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.03f);
					pVtx[(nCntVtx * 4) + 2].col -= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.03f);
					pVtx[(nCntVtx * 4) + 3].col -= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.03f);
				}
			}
		}
	}

	D3DXCOLOR vtxColor = pVtx[0].col;

	 //頂点カラー
	pVtx[(NUM_COMBO * 4) + 0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, vtxColor.a);
	pVtx[(NUM_COMBO * 4) + 1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, vtxColor.a);
	pVtx[(NUM_COMBO * 4) + 2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, vtxColor.a);
	pVtx[(NUM_COMBO * 4) + 3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, vtxColor.a);

	if (vtxColor.a <= 0.02f)
	{// 透明になったとき
		int nScore;
		float fScore;

		fScore = g_nNumCombo % 100 / 10.0f;
		fScore = fScore * 0.01f + 1.0f;

		// コンボとキル数でスコア割り出し
		nScore = ((g_nCntKillCombo * 1000) + (g_nNumCombo * 100)) * (int)fScore;

		AddScore(nScore);
		g_nNumCombo = 0;
		g_nCntKillCombo = 0;

		if (nScore > 0)
		{// スコア反映時効果音
			PlaySound(SOUND_LABEL_COMBO);
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffCombo->Unlock();

	g_nCntComboTime++;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCombo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = NUM_COMBO; nCntScore >= 0; nCntScore--)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureCombo[g_aCombo[nCntScore].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntScore, 2);
	}
}

//=============================================================================
// キルコンボの加算
//=============================================================================
void AddKillCombo(void)
{
	g_nCntKillCombo++;
}

//=============================================================================
// コンボの加算
//=============================================================================
void AddCombo(void)
{
	/***********************************************************
	*
	*	敵を倒したときに呼んでください
	*
	***********************************************************/

	int nCntScore;
	int aNumber[NUM_COMBO];	// 桁ごとのコンボ数

	g_nNumCombo++;	// コンボ数を増やす

	g_nCntComboTime = 0;	// 透明化するまでの時間を0に戻す

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラー
	pVtx[(NUM_COMBO * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_COMBO * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_COMBO * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[(NUM_COMBO * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	aNumber[0] = g_nNumCombo % 10;				// １桁目
	aNumber[1] = g_nNumCombo % 100 / 10;		// ２桁目
	aNumber[2] = g_nNumCombo % 1000 / 100;		// ３桁目

	for (nCntScore = 0; nCntScore < NUM_COMBO; nCntScore++)
	{
		//テクスチャ座標
		pVtx[(nCntScore * 4) + 0].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 1].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 2].tex = D3DXVECTOR2(0.0f + aNumber[nCntScore] * 0.1f, 1.0f);
		pVtx[(nCntScore * 4) + 3].tex = D3DXVECTOR2(0.1f + aNumber[nCntScore] * 0.1f, 1.0f);

		if (g_nNumCombo < 100)
		{// 100より小さい
			//頂点カラー
			pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{// 100以上
			pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(1.0f, rand() % 3 * 0.1f, rand() % 3 * 0.1f, 1.0f);
			pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(1.0f, rand() % 3 * 0.1f, rand() % 3 * 0.1f, 1.0f);
			pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(1.0f, rand() % 3 * 0.1f, rand() % 3 * 0.1f, 1.0f);
			pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(1.0f, rand() % 3 * 0.1f, rand() % 3 * 0.1f, 1.0f);

			if ((g_aCombo[nCntScore].pos.x - SIZE_COMBO) - (MAX_SIZE_COMBO * 1.1f) < pVtx[(nCntScore * 4) + 0].pos.x)
			{// 大きさの限度
				//頂点座標の設定
				pVtx[(nCntScore * 4) + 0].pos.x -= SIZE_LARGE_COMBO * 1.1f;
				pVtx[(nCntScore * 4) + 0].pos.y -= SIZE_LARGE_COMBO * 1.1f;
				pVtx[(nCntScore * 4) + 1].pos.x += SIZE_LARGE_COMBO * 1.1f;
				pVtx[(nCntScore * 4) + 1].pos.y -= SIZE_LARGE_COMBO * 1.1f;
				pVtx[(nCntScore * 4) + 2].pos.x -= SIZE_LARGE_COMBO * 1.1f;
				pVtx[(nCntScore * 4) + 2].pos.y += SIZE_LARGE_COMBO * 1.1f;
				pVtx[(nCntScore * 4) + 3].pos.x += SIZE_LARGE_COMBO * 1.1f;
				pVtx[(nCntScore * 4) + 3].pos.y += SIZE_LARGE_COMBO * 1.1f;
			}
		}

		if (g_aNumLogCombo[nCntScore] != aNumber[nCntScore])
		{
			if ((g_aCombo[nCntScore].pos.x - SIZE_COMBO) - MAX_SIZE_COMBO < pVtx[(nCntScore * 4) + 0].pos.x)
			{// 大きさの限度
			 //頂点座標の設定
				pVtx[(nCntScore * 4) + 0].pos.x -= SIZE_LARGE_COMBO;
				pVtx[(nCntScore * 4) + 0].pos.y -= SIZE_LARGE_COMBO;
				pVtx[(nCntScore * 4) + 1].pos.x += SIZE_LARGE_COMBO;
				pVtx[(nCntScore * 4) + 1].pos.y -= SIZE_LARGE_COMBO;
				pVtx[(nCntScore * 4) + 2].pos.x -= SIZE_LARGE_COMBO;
				pVtx[(nCntScore * 4) + 2].pos.y += SIZE_LARGE_COMBO;
				pVtx[(nCntScore * 4) + 3].pos.x += SIZE_LARGE_COMBO;
				pVtx[(nCntScore * 4) + 3].pos.y += SIZE_LARGE_COMBO;
			}
		}

		g_aNumLogCombo[nCntScore] = aNumber[nCntScore];	// 新しいデータを保存
	}

	if (g_nNumCombo < 10)
	{// ９以下だと2,3桁目を透明に
	 //頂点カラー
		pVtx[4].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[5].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[6].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[7].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		pVtx[8].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[9].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[10].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[11].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
	}
	else if (g_nNumCombo < 100)
	{// ９９以下だと3桁目を透明に
		pVtx[8].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[9].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[10].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
		pVtx[11].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffCombo->Unlock();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexCombo(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntScore;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_COMBO_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCombo,
		NULL);

	// 頂点情報を設定
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCombo->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_COMBO; nCntScore++)
	{
		//頂点座標の設定
		pVtx[(nCntScore * 4) + 0].pos = D3DXVECTOR3(g_aCombo[nCntScore].pos.x - SIZE_COMBO, g_aCombo[nCntScore].pos.y - SIZE_COMBO, 0.0f);
		pVtx[(nCntScore * 4) + 1].pos = D3DXVECTOR3(g_aCombo[nCntScore].pos.x + SIZE_COMBO, g_aCombo[nCntScore].pos.y - SIZE_COMBO, 0.0f);
		pVtx[(nCntScore * 4) + 2].pos = D3DXVECTOR3(g_aCombo[nCntScore].pos.x - SIZE_COMBO, g_aCombo[nCntScore].pos.y + SIZE_COMBO, 0.0f);
		pVtx[(nCntScore * 4) + 3].pos = D3DXVECTOR3(g_aCombo[nCntScore].pos.x + SIZE_COMBO, g_aCombo[nCntScore].pos.y + SIZE_COMBO, 0.0f);

		// サイズの記憶
		g_aDiffSizeComco[nCntScore][0] = pVtx[(nCntScore * 4) + 0].pos;
		g_aDiffSizeComco[nCntScore][1] = pVtx[(nCntScore * 4) + 1].pos;
		g_aDiffSizeComco[nCntScore][2] = pVtx[(nCntScore * 4) + 2].pos;
		g_aDiffSizeComco[nCntScore][3] = pVtx[(nCntScore * 4) + 3].pos;

		//1.0fで固定
		pVtx[(nCntScore * 4) + 0].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 1].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 2].rhw = 1.0f;
		pVtx[(nCntScore * 4) + 3].rhw = 1.0f;

		//頂点カラー
		pVtx[(nCntScore * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[(nCntScore * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[(nCntScore * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[(nCntScore * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		//テクスチャ座標
		pVtx[(nCntScore * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntScore * 4) + 1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[(nCntScore * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntScore * 4) + 3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//頂点座標の設定
	pVtx[(NUM_COMBO * 4) + 0].pos = D3DXVECTOR3(g_aCombo[NUM_COMBO].pos.x - LOGO_SIZE_COMBO * 1.5f, g_aCombo[NUM_COMBO].pos.y - LOGO_SIZE_COMBO, 0.0f);
	pVtx[(NUM_COMBO * 4) + 1].pos = D3DXVECTOR3(g_aCombo[NUM_COMBO].pos.x + LOGO_SIZE_COMBO * 1.5f, g_aCombo[NUM_COMBO].pos.y - LOGO_SIZE_COMBO, 0.0f);
	pVtx[(NUM_COMBO * 4) + 2].pos = D3DXVECTOR3(g_aCombo[NUM_COMBO].pos.x - LOGO_SIZE_COMBO * 1.5f, g_aCombo[NUM_COMBO].pos.y + LOGO_SIZE_COMBO, 0.0f);
	pVtx[(NUM_COMBO * 4) + 3].pos = D3DXVECTOR3(g_aCombo[NUM_COMBO].pos.x + LOGO_SIZE_COMBO * 1.5f, g_aCombo[NUM_COMBO].pos.y + LOGO_SIZE_COMBO, 0.0f);

	//1.0fで固定
	pVtx[(NUM_COMBO * 4) + 0].rhw = 1.0f;
	pVtx[(NUM_COMBO * 4) + 1].rhw = 1.0f;
	pVtx[(NUM_COMBO * 4) + 2].rhw = 1.0f;
	pVtx[(NUM_COMBO * 4) + 3].rhw = 1.0f;

	//頂点カラー
	pVtx[(NUM_COMBO * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[(NUM_COMBO * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[(NUM_COMBO * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[(NUM_COMBO * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//テクスチャ座標
	pVtx[(NUM_COMBO * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[(NUM_COMBO * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[(NUM_COMBO * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[(NUM_COMBO * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffCombo->Unlock();
}
