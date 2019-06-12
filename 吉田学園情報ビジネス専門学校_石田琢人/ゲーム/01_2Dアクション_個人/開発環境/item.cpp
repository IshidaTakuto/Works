//=============================================================================
//
// アイテム処理 [item.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "item.h"
#include "input.h"
#include "score.h"
#include "player.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ITEM_TEXTURENAME_0	"data/TEXTURE/egg.png"
#define ITEM_TEXTURENAME_1	"data/TEXTURE/explosion000.png"
#define TEX_PATTERN	(4)	// テクスチャのパターン数
#define MAX_TEXTURE	(2)	// テクスチャの数
#define TEX_UPDATE_SPEED	(8)	// テクスチャの更新速度
#define EGG_SIZE	(30.0f)	// 卵の大きさ
#define EGG_SIZE_Y	(35.0f)	// 卵の大きさ

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureItem[MAX_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;	// 頂点バッファへのポインタ
ITEM g_aItem[MAX_ITEM];	//アイテムの情報
int g_nTotalEGG;

//=============================================================================
// 初期化処理
//=============================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntItem;

	// デバイスの取得
	pDevice = GetDevice();

	// アイテムの情報の初期化
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].fWidth = 0.0f;
		g_aItem[nCntItem].fHeight = 0.0f;
		g_aItem[nCntItem].fAngle = 0.0f;
		g_aItem[nCntItem].fLength = 0.0f;
		g_aItem[nCntItem].nCounterAnim = 0;
		g_aItem[nCntItem].nPatternAnim = 0;
		g_aItem[nCntItem].type = ITEMTYPE_NONE;
		g_aItem[nCntItem].state = ITEMSTATE_NORMAL;
		g_aItem[nCntItem].nCntState = 0;
		g_aItem[nCntItem].bUse = false;
	}

	g_nTotalEGG = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_0, &g_pTextureItem[0]);
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_1, &g_pTextureItem[1]);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE_Y, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める
	}

	//頂点バッファをアンロック
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItem(void)
{
	int nCntItem;

	for (nCntItem = 0; nCntItem < MAX_TEXTURE; nCntItem++)
	{
		//テクスチャの破棄
		if (g_pTextureItem[nCntItem] != NULL)
		{
			g_pTextureItem[nCntItem]->Release();
			g_pTextureItem[nCntItem] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItem(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	PLAYER Player;

	Player = GetPlayer();

	int nCntItem;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		switch (g_aItem[nCntItem].type)
		{
		case ITEMTYPE_NONE:
			break;

		case ITEMTYPE_EGG:
			if (g_aItem[nCntItem].state == ITEMSTATE_APPEAR)
			{
				PlaySound(SOUND_LABEL_SE_EGG);
				g_aItem[nCntItem].type = ITEMTYPE_NONE;
				AddScore(5000);
			}
			break;

		case ITEMTYPE_DIRTY:
			if (g_aItem[nCntItem].state == ITEMSTATE_APPEAR)
			{
				PlaySound(SOUND_LABEL_SE_DRESSING);
				PlaySound(SOUND_LABEL_SE_SCREAM001);
				g_aItem[nCntItem].type = ITEMTYPE_NONE;
				SetEffect(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 600, 120, EFFECTSTATE_DIRTY);
			}
			break;
		}

		switch (g_aItem[nCntItem].state)
		{
		case ITEMSTATE_APPEAR:
			break;

		case ITEMSTATE_NORMAL:
			break;

		case ITEMSTATE_GET:
			g_aItem[nCntItem].nCntState++;
			g_aItem[nCntItem].pos.y -= 15.0f;
			if (g_aItem[nCntItem].nCntState >= 10)
			{
				g_aItem[nCntItem].state = ITEMSTATE_APPEAR;
				g_aItem[nCntItem].bUse = false;
			}
			break;

		case ITEMSTATE_MOVE:
			g_aItem[nCntItem].pos.x -= SCROLL_SPEED;
			break;
		}

		if (Player.pos.x >= SCREEN_WIDTH / 3 && g_aItem[nCntItem].state == ITEMSTATE_NORMAL)
		{
			g_aItem[nCntItem].state = ITEMSTATE_MOVE;
		}
		


		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE_Y, 0.0f);

		pVtx += 4;	// 頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロック
	g_pVtxBuffItem->Unlock();

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].nCounterAnim++;

			if (g_aItem[nCntItem].bUse == true)
			{// アイテムが使用されている
				if (g_aItem[nCntItem].type == ITEMTYPE_EGG)
				{// 卵
					//if ((g_aItem[nCntItem].nCounterAnim % TEX_UPDATE_SPEED) == 0)
					//{
						//// パターンNo.の更新
						//g_aItem[nCntItem].nPatternAnim = (g_aItem[nCntItem].nPatternAnim + 1) % 4;

						//g_aItem[nCntItem].fAngle = atan2f(-EGG_SIZE, -EGG_SIZE_Y);
						//g_aItem[nCntItem].fLength = sqrtf(EGG_SIZE * EGG_SIZE + EGG_SIZE_Y * EGG_SIZE_Y);

						//// 頂点の座標
						//pVtx[0].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;
						//pVtx[0].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;

						//g_aItem[nCntItem].fAngle = atan2f(EGG_SIZE, -EGG_SIZE_Y);

						//pVtx[1].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;
						//pVtx[1].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;

						//g_aItem[nCntItem].fAngle = atan2f(-EGG_SIZE, EGG_SIZE_Y);

						//pVtx[2].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;
						//pVtx[2].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;

						//g_aItem[nCntItem].fAngle = atan2f(EGG_SIZE, EGG_SIZE_Y);

						//pVtx[3].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;
						//pVtx[3].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;

						//// テクスチャ座標を更新
						//pVtx[0].tex = D3DXVECTOR2(0.0f + (0.25f * g_aItem[nCntItem].nPatternAnim), 0.0f);
						//pVtx[1].tex = D3DXVECTOR2(0.25f + (0.25f * g_aItem[nCntItem].nPatternAnim), 0.0f);
						//pVtx[2].tex = D3DXVECTOR2(0.0f + (0.25f * g_aItem[nCntItem].nPatternAnim), 1.0f);
						//pVtx[3].tex = D3DXVECTOR2(0.25f + (0.25f * g_aItem[nCntItem].nPatternAnim), 1.0f);
					//}
				}
				else if (g_aItem[nCntItem].type == ITEMTYPE_DIRTY)
				{// 汚れ
					if ((g_aItem[nCntItem].nCounterAnim % TEX_UPDATE_SPEED - 6) == 0)
					{
						// パターンNo.の更新
						g_aItem[nCntItem].nPatternAnim = (g_aItem[nCntItem].nPatternAnim + 1) % 8;

						//頂点カラー
						pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.3f, 1.0f);
						pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.3f, 1.0f);
						pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.3f, 1.0f);
						pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.3f, 1.0f);

						// テクスチャ座標を更新
						pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f * g_aItem[nCntItem].nPatternAnim), 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.125f + (0.125f * g_aItem[nCntItem].nPatternAnim), 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f + (0.125f * g_aItem[nCntItem].nPatternAnim), 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.125f + (0.125f * g_aItem[nCntItem].nPatternAnim), 1.0f);
					}
				}
			}
		pVtx += 4;	// 頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロック
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntItem;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)	// 使用されている
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureItem[g_aItem[nCntItem].type - 1]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntItem, 2);
		}
	}
}

//=============================================================================
// アイテムの設定処理
//=============================================================================
void SetItem(D3DXVECTOR3 pos, ITEMTYPE type)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntItem;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if(type == ITEMTYPE_EGG)
		{
			if (g_aItem[nCntItem].bUse == false)	//弾が使用されていない
			{
				g_aItem[nCntItem].pos = pos;
				g_aItem[nCntItem].type = type;
				g_aItem[nCntItem].nCounterAnim = 0;
				g_aItem[nCntItem].nPatternAnim = 0;
				g_aItem[nCntItem].fWidth = EGG_SIZE;
				g_aItem[nCntItem].fHeight = EGG_SIZE_Y;
				g_aItem[nCntItem].bUse = true;	//使用している状態にする

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE_Y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE_Y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE_Y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE_Y, 0.0f);
				g_nTotalEGG++;

				break;
			}
		}
		else if (type == ITEMTYPE_DIRTY)
		{
			if (g_aItem[nCntItem].bUse == false)	//弾が使用されていない
			{
				g_aItem[nCntItem].pos = pos;
				g_aItem[nCntItem].type = type;
				g_aItem[nCntItem].nCounterAnim = 0;
				g_aItem[nCntItem].nPatternAnim = 0;
				g_aItem[nCntItem].fWidth = EGG_SIZE;
				g_aItem[nCntItem].fHeight = EGG_SIZE_Y;
				g_aItem[nCntItem].bUse = true;	//使用している状態にする

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE, 0.0f);
				break;
			}
		}
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// アイテムの取得
//=============================================================================
ITEM *GetItem(void)
{
	return &g_aItem[0];
}
