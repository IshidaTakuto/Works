//=============================================================================
//
// クレジット処理 [Credit.cpp]
// Author : 
//
//=============================================================================
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "credit.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Credit_TEXTURENAME1	"data/TEXTURE/CREDIT/credit.png"	// 読み込むテクスチャの名前１
#define Credit_TEXTURENAME2	"data/TEXTURE/CREDIT/credit.jpg"	// 読み込むテクスチャの名前２
#define Credit_TEXTURENAME3	"data/TEXTURE/CREDIT/PressAnyBotton000.png"		// 読み込むテクスチャの名前３
#define Credit_LOGO_POS_X		(140.0f)							// ロゴのX座標
#define Credit_LOGO_POS_Y		(35.0f)								// ロゴのY座標
#define Credit_LOGO_WIDTH		(280.0f)							// ロゴの幅
#define Credit_LOGO_HEIGHT	(70.0f)								// ロゴの高さ
#define Credit_ENTER_POS_X	(SCREEN_WIDTH  - 180.0f)			// エンターのX座標
#define Credit_ENTER_POS_Y	(SCREEN_HEIGHT - 70.0f)				// エンターのY座標
#define Credit_ENTER_WIDTH	(300.0f)							// エンターの幅
#define Credit_ENTER_HEIGHT	(90.0f)								// エンターの高さ
#define Credit_FLASH_ENTER	(60)								// エンターの点滅間隔

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	CreditTEX_BG = 0,	// 背景
	CreditTEX_ENTER,	// エンター
	CreditTEX_MAX		// テクスチャの総数
} CreditTEX;

typedef struct
{
	int nCntFps;		// 経過時間
	bool bPush;			// 押されたかどうか
} Credit;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureCredit[CreditTEX_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCredit = NULL;
Credit g_Credit;
bool g_bDrawCredit[CreditTEX_MAX];	// 描画するかどうか

//=============================================================================
// チュートリアル初期化処理
//=============================================================================
void InitCredit(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	// デバイスを取得する
	pDevice = GetDevice();

	// 設定の初期化
	for (int nCntTex = 0; nCntTex < CreditTEX_MAX; nCntTex++)
	{
		g_bDrawCredit[nCntTex] = true;
		if (nCntTex == CreditTEX_ENTER)
		{// エンターの場合
			g_bDrawCredit[nCntTex] = false;
		}
	}
	g_Credit.nCntFps = 0;
	g_Credit.bPush = false;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, Credit_TEXTURENAME1, &g_apTextureCredit[CreditTEX_BG]);
	D3DXCreateTextureFromFile(pDevice, Credit_TEXTURENAME3, &g_apTextureCredit[CreditTEX_ENTER]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * CreditTEX_MAX
		, D3DUSAGE_WRITEONLY
		, FVF_VERTEX_2D
		, D3DPOOL_MANAGED
		, &g_pVtxBuffCredit
		, NULL);

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffCredit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTex = 0; nCntTex < CreditTEX_MAX; nCntTex++, pVtx += 4)
	{
		// 頂点座標の設定
		if (nCntTex == CreditTEX_BG)
		{
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		}
		
		else if (nCntTex == CreditTEX_ENTER)
		{
			pVtx[0].pos = D3DXVECTOR3(Credit_ENTER_POS_X - Credit_ENTER_WIDTH / 2, Credit_ENTER_POS_Y - Credit_ENTER_HEIGHT / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(Credit_ENTER_POS_X + Credit_ENTER_WIDTH / 2, Credit_ENTER_POS_Y - Credit_ENTER_HEIGHT / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(Credit_ENTER_POS_X - Credit_ENTER_WIDTH / 2, Credit_ENTER_POS_Y + Credit_ENTER_HEIGHT / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(Credit_ENTER_POS_X + Credit_ENTER_WIDTH / 2, Credit_ENTER_POS_Y + Credit_ENTER_HEIGHT / 2, 0.0f);
		}

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		if (nCntTex == CreditTEX_ENTER)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffCredit->Unlock();

	
}

//=============================================================================
// チュートリアル終了処理
//=============================================================================
void UninitCredit(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < CreditTEX_MAX; nCntTex++)
	{
		if (g_apTextureCredit[nCntTex] != NULL)
		{
			g_apTextureCredit[nCntTex]->Release();
			g_apTextureCredit[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffCredit != NULL)
	{
		g_pVtxBuffCredit->Release();
		g_pVtxBuffCredit = NULL;
	}

}

//=============================================================================
// チュートリアル更新処理
//=============================================================================
void UpdateCredit(void)
{
	FADE pFade = *GetFade(); // フェードの取得
	PLAYER *pPlayer = GetPlayer();


	if (GetKeyboardTrigger(DIK_RETURN) == true || pPlayer[0].bButton[BUTTON_ENTER] == true)
	{// 決定キー(ENTERキー)が押された場合
		if (pFade == FADE_NONE)
		{// フェードが何もしていない状態の場合
			PlaySound(SOUND_LABEL_ENTER);
			g_Credit.bPush = true;
			SetFade(MODE_SELCT,0,0.0,0);
		}
	}
	

	g_Credit.nCntFps++;
	if (g_Credit.bPush == true)
	{// 決定キー(ENTERキー)が押された場合
		if (g_Credit.nCntFps % 2 == 0)
		{
			g_bDrawCredit[CreditTEX_ENTER] = g_bDrawCredit[CreditTEX_ENTER] ? false : true;
		}
	}
	else
	{// 決定キー(ENTERキー)が押されていない場合
		if (g_Credit.nCntFps % Credit_FLASH_ENTER == 0)
		{// １秒経過した場合
			g_bDrawCredit[CreditTEX_ENTER] = g_bDrawCredit[CreditTEX_ENTER] ? false : true;
		}
	}
}

//=============================================================================
// チュートリアル描画処理
//=============================================================================
void DrawCredit(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCredit, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTex = 0; nCntTex < 3; nCntTex++)
	{
		if (g_bDrawCredit[nCntTex] == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureCredit[nCntTex]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP	// プリミティブの種類
				, nCntTex * 4		// 開始する頂点のインデックス
				, 2);				// 描画するプリミティブの数
		}
	}
}
