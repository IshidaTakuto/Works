//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "main.h"
#include "title.h"
#include "fade.h"
#include "sound.h"
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_TEXTURENAME	"DATA/TEXTURE/title000.png"	//タイトルのテクスチャ名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;	//頂点バッファへのポインタ

//=============================================================================
// タイトル初期化処理
//=============================================================================
void InitTitle(void)
{
	//背景初期化処理
	InitBg();

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURENAME, &g_pTextureTitle);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBuffTitle->Unlock();
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UninitTitle(void)
{
	//背景終了処理
	UninitBg();

	//テクスチャの破棄
	if (g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateTitle(void)
{
	//背景更新処理
	UpdateBg();

	FADE pFade;

	//フェードの取得
	pFade = *GetFade();
	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);
			//モード選択
			SetFade(MODE_TUTORIAL);
		}
		else if (GetKeyboardTrigger(DIK_R) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);
			//モード選択
			SetFade(MODE_RANKING);
		}
	}
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawTitle(void)
{
	//背景描画処理
	DrawBg();

	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitle);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
