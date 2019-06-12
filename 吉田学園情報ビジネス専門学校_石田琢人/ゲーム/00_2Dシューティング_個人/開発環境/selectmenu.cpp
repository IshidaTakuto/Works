//=============================================================================
//
// ポーズメニュー処理 [selectmenu.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "selectmenu.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SELECTMENU_TEXTURENAME0	"data/TEXTURE/pause000.png"	//ポーズのテクスチャ名
#define SELECTMENU_TEXTURENAME1	"data/TEXTURE/pause001.png"	//ポーズのテクスチャ名
#define SELECTMENU_TEXTURENAME2	"data/TEXTURE/pause002.png"	//ポーズのテクスチャ名
#define TEX_UPDATE	(g_aSelectmenu.nCountAnim * 0.005f)
#define MAX_MENU	(3)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureSelectmenu[MAX_MENU];		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelectmenu = NULL;	//頂点バッファへのポインタ
SELECTMENU g_aSelectmenu[MAX_MENU];

//=============================================================================
// 初期化処理
//=============================================================================
void InitSelectmenu(void)
{
	int nCntSelectmenu;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	for (nCntSelectmenu = 0; nCntSelectmenu < MAX_MENU; nCntSelectmenu++)
	{
		//情報の初期化
		g_aSelectmenu[nCntSelectmenu].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//背景の位置
		g_aSelectmenu[nCntSelectmenu].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aSelectmenu[nCntSelectmenu].state = SELECTMENUSTATE_CONTINUE;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, SELECTMENU_TEXTURENAME0, &g_apTextureSelectmenu[0]);
	D3DXCreateTextureFromFile(pDevice, SELECTMENU_TEXTURENAME1, &g_apTextureSelectmenu[1]);
	D3DXCreateTextureFromFile(pDevice, SELECTMENU_TEXTURENAME2, &g_apTextureSelectmenu[2]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_MENU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelectmenu,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSelectmenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSelectmenu = 0; nCntSelectmenu < MAX_MENU; nCntSelectmenu++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x - 50, g_aSelectmenu[nCntSelectmenu].pos.y - 50, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x + 50, g_aSelectmenu[nCntSelectmenu].pos.y - 50, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x - 50, g_aSelectmenu[nCntSelectmenu].pos.y + 50, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x + 50, g_aSelectmenu[nCntSelectmenu].pos.y + 50, 0.0f);

		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffSelectmenu->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSelectmenu(void)
{
	int nCntSelectmenu;

	//テクスチャの破棄
	for (nCntSelectmenu = 0; nCntSelectmenu < MAX_MENU; nCntSelectmenu++)
	{
		if (g_apTextureSelectmenu[nCntSelectmenu] != NULL)
		{
			g_apTextureSelectmenu[nCntSelectmenu]->Release();
			g_apTextureSelectmenu[nCntSelectmenu] = NULL;
		}

	}

	//頂点バッファの破棄
	if (g_pVtxBuffSelectmenu != NULL)
	{
		g_pVtxBuffSelectmenu->Release();
		g_pVtxBuffSelectmenu = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSelectmenu(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSelectmenu(void)
{
	int nCntSelectmenu;

	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスを取得する
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSelectmenu, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	//ポリゴンの描画
	for (nCntSelectmenu = 0; nCntSelectmenu < MAX_MENU; nCntSelectmenu++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureSelectmenu[g_aSelectmenu[nCntSelectmenu].state]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntSelectmenu, 2);
	}
}

//=============================================================================
// セレクトメニューの設定
//=============================================================================
void SetSelectmenu(D3DXVECTOR3 pos, D3DXCOLOR col, SELECTMENUSTATE state)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntSelectmenu;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSelectmenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSelectmenu = 0; nCntSelectmenu < MAX_MENU; nCntSelectmenu++)
	{
		g_aSelectmenu[nCntSelectmenu].pos = pos;
		g_aSelectmenu[nCntSelectmenu].col = col;
		g_aSelectmenu[nCntSelectmenu].state = state;

		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x - 50, g_aSelectmenu[nCntSelectmenu].pos.y - 50, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x + 50, g_aSelectmenu[nCntSelectmenu].pos.y - 50, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x - 50, g_aSelectmenu[nCntSelectmenu].pos.y + 50, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aSelectmenu[nCntSelectmenu].pos.x + 50, g_aSelectmenu[nCntSelectmenu].pos.y + 50, 0.0f);

		break;
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffSelectmenu->Unlock();
}