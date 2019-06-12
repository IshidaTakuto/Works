//=============================================================================
//
// ランキング処理 [ranking.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "ranking.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "meshField.h"
#include "joypad.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RANKING			"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_RANKING_LOGO	"data/TEXTURE/ranking_logo.png"		// 読み込むテクスチャファイル名

#define RANKING_TIME	(300)	// タイトル画面までの時間

#define MAX_TEXTURE		(2)		// テクスチャ数
#define RANKING_DIGIT	(7)		// 桁数
#define MAX_RANKING		(5)		// ランキング数

#define RANKING_X (60.0f)		//数字の大きさ_X
#define RANKING_Y (80.0f)		//数字の大きさ_Y
#define RANK_CHARACTER_X	(250)
#define RANK_CHARACTER_Y	(100)

#define RANKING_POS			(D3DXVECTOR3(SCREEN_WIDTH / 2 + (RANKING_X * (RANKING_DIGIT / 2.0f)) - RANKING_X / 2, 125.0f + RANKING_Y, 0.0f))	// ランキングの位置
#define RANKING_LOGO_POS	(D3DXVECTOR3(SCREEN_WIDTH / 2, 80.0f , 0.0f))										// ロゴの位置

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRanking[MAX_TEXTURE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;						// 頂点バッファへのポインタ
static int				g_nRanking[MAX_RANKING] = { 7000, 6000, 5000, 0000, 0000 };	// スコア
D3DXCOLOR				g_nScoreCol[MAX_RANKING + 1];								// 今のスコアの色
int g_nYourScore;

bool bScore;
int nCntHighScore;
int g_nCntFade;		// フェードするまでの時間

//=============================================================================
// 初期化処理
//=============================================================================
void InitRanking(void)
{
	InitMeshField();

	g_nCntFade = 0;
	nCntHighScore = 0;
	for (int nCnt = 0; nCnt < MAX_RANKING + 1; nCnt++)
	{
		g_nScoreCol[nCnt] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//bScore = false;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING, &g_pTextureRanking[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING_LOGO, &g_pTextureRanking[1]);

	// 頂点情報の作成
	MakeVertexRanking(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRanking(void)
{
	UninitMeshField();

	for (int nCntRankingTex = 0; nCntRankingTex < MAX_TEXTURE; nCntRankingTex++)
	{
		// テクスチャの開放
		if (g_pTextureRanking[nCntRankingTex] != NULL)
		{
			g_pTextureRanking[nCntRankingTex]->Release();
			g_pTextureRanking[nCntRankingTex] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRanking(void)
{
	UpdateMeshField();

	XINPUT_STATE state;
	state = GetJoyPad();

	// 頂点情報の作成
	VERTEX_2D *pVtx;
	FADE pFade;
	pFade = *GetFade();

	// 頂点情報の作成
	//VERTEX_2D *pVtx;

	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			PlaySound(SOUND_LABEL_DECIDE);
			//モード選択
			SetFade(MODE_TITLE);
			bScore = false;
		}
		if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			PlaySound(SOUND_LABEL_DECIDE);
			//モード選択
			SetFade(MODE_TITLE);
			bScore = false;
		}

	}
	g_nCntFade++;
	if (g_nCntFade >= RANKING_TIME)
	{
		if (pFade == FADE_NONE)
		{
			//モード選択
			SetFade(MODE_TITLE);
			bScore = false;
		}
	}

	nCntHighScore++;
	nCntHighScore = nCntHighScore % 4;

	// 頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);
	if (bScore == true)
	{
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
		{
			if (g_nRanking[nCntRanking] == g_nYourScore)
			{
				for (int nCnt = 0; nCnt < RANKING_DIGIT; nCnt++, pVtx += 4)
				{
					if (nCntHighScore < 2)
					{
						g_nScoreCol[nCntRanking] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					}
					else if (nCntHighScore >= 2)
					{
						g_nScoreCol[nCntRanking] = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
					}

					// 頂点カラーの設定
					pVtx[0].col = g_nScoreCol[nCntRanking];
					pVtx[1].col = g_nScoreCol[nCntRanking];
					pVtx[2].col = g_nScoreCol[nCntRanking];
					pVtx[3].col = g_nScoreCol[nCntRanking];
				}
				break;
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRanking(void)
{
	DrawMeshField();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ランキング文字--------------------------------------------------------
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRanking[1]);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (RANKING_DIGIT * MAX_RANKING), 2);

	// ランキング数字--------------------------------------------------------
	// テクスチャの設定
	for (int nCntRanking = 0; nCntRanking < (RANKING_DIGIT * MAX_RANKING); nCntRanking++)
	{
		pDevice->SetTexture(0, g_pTextureRanking[0]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntRanking, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RANKING_DIGIT * (MAX_RANKING + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// ランキングのスコア(数字)
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		for (int nCntDigit = 0; nCntDigit < RANKING_DIGIT; nCntDigit++)
		{
			pVtx[0].pos = D3DXVECTOR3(-(RANKING_X / 2) - (RANKING_X * nCntDigit), -(RANKING_Y / 2) + ((30 + RANKING_Y) * nCntRanking), 0.0f) + RANKING_POS;
			pVtx[1].pos = D3DXVECTOR3((RANKING_X / 2) - (RANKING_X * nCntDigit), -(RANKING_Y / 2) + ((30 + RANKING_Y) * nCntRanking), 0.0f) + RANKING_POS;
			pVtx[2].pos = D3DXVECTOR3(-(RANKING_X / 2) - (RANKING_X * nCntDigit), (RANKING_Y / 2) + ((30 + RANKING_Y) * nCntRanking), 0.0f) + RANKING_POS;
			pVtx[3].pos = D3DXVECTOR3((RANKING_X / 2) - (RANKING_X * nCntDigit), (RANKING_Y / 2) + ((30 + RANKING_Y) * nCntRanking), 0.0f) + RANKING_POS;

			//1.0fで固定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			if (nCntRanking == 0)
			{
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			}
			else if (nCntRanking == 1)
			{
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
			}
			else if (nCntRanking == 2)
			{
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(0.7f, 0.2f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.7f, 0.2f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.7f, 0.2f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.7f, 0.2f, 0.0f, 1.0f);
			}
			else
			{
				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			int nData = 1;

			for (int nCntData = nCntDigit; nCntData != 0; nCntData--)
			{
				nData *= 10;
			}

			pVtx[0].tex = D3DXVECTOR2((g_nRanking[nCntRanking] % (10 * nData) / nData)* 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_nRanking[nCntRanking] % (10 * nData) / nData)* 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_nRanking[nCntRanking] % (10 * nData) / nData)* 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_nRanking[nCntRanking] % (10 * nData) / nData)* 0.1f + 0.1f, 1.0f);

			pVtx += 4;
		}
	}

	// ランキングの文字-------------------------------------------------------------------------------------------											    	   
	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(-RANK_CHARACTER_X / 2, -RANK_CHARACTER_Y / 2, 0.0f) + RANKING_LOGO_POS;
	pVtx[1].pos = D3DXVECTOR3(RANK_CHARACTER_X / 2, -RANK_CHARACTER_Y / 2, 0.0f) + RANKING_LOGO_POS;
	pVtx[2].pos = D3DXVECTOR3(-RANK_CHARACTER_X / 2, RANK_CHARACTER_Y / 2, 0.0f) + RANKING_LOGO_POS;
	pVtx[3].pos = D3DXVECTOR3(RANK_CHARACTER_X / 2, RANK_CHARACTER_Y / 2, 0.0f) + RANKING_LOGO_POS;

	// rhwの設定
	for (int nCntrhw = 0; nCntrhw < 4; nCntrhw++)
	{
		pVtx[nCntrhw].rhw = 1.0f;
	}

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);
}


//=============================================================================
// スコアのセット
//=============================================================================
void SetRanking(int nScore)
{
	int nRank;
	int aEmptyData[MAX_RANKING + 1];
	aEmptyData[MAX_RANKING] = nScore;

	for (int nCntNum = 0; nCntNum < MAX_RANKING; nCntNum++)
	{

		aEmptyData[nCntNum] = g_nRanking[nCntNum];

	}

	for (int nCntNum = 1; nCntNum < MAX_RANKING + 1; nCntNum++)
	{
		for (int nCntNum1 = nCntNum; nCntNum1 > 0; nCntNum1--)
		{
			if (aEmptyData[nCntNum1 - 1] <= aEmptyData[nCntNum1])
			{
				nRank = aEmptyData[nCntNum1];
				aEmptyData[nCntNum1] = aEmptyData[nCntNum1 - 1];
				aEmptyData[nCntNum1 - 1] = nRank;
			}
		}
	}

	for (int nCntNum = 0; nCntNum < MAX_RANKING; nCntNum++)
	{
		g_nRanking[nCntNum] = aEmptyData[nCntNum];

	}
	bScore = true;

	g_nYourScore = nScore;
}
