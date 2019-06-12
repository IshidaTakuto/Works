//=========================================================================================================//
//
// [弾発射]->弾移動処理 [bullet.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "game.h"				//爆発を呼ぶため

//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define BULLET_VERTEX_NUM			(4)					// 頂点の数
#define BULLET_PRIMITIVE_NUM		(2)					// プリミティブの数
#define BULLET_MAX_Texture			(1)					//画像の種類
#define BULLET_TextureName		"data/TEXTURE/Digital000.png"
#define	MAX_BULLET					(256)				// 弾の最大数

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9		g_pTextureDigital[BULLET_MAX_Texture] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDigital = NULL;	// 頂点バッファへのポインタ		

//=========================================================================================================//
// * 背景の描画処理1	初期化処理
//=========================================================================================================//
void InitDigital(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntBullet;

	DIGITAL *g_aDigital = GetDigital();

	// 弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//g_aBullet[nCntBullet].pos = { 0.0f, 0.0f, 0.0f };
		//g_aBullet[nCntBullet].steep = 0.0f;
		//g_aBullet[nCntBullet].nType = 0;
		//g_aBullet[nCntBullet].nAtkType = 0;
		//g_aBullet[nCntBullet].nShot = 0;
		//g_aBullet[nCntBullet].nLengthX = 0;
		//g_aBullet[nCntBullet].nLengthY = 0;
		g_aDigital[nCntBullet].bUse = false;
	}

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, BULLET_TextureName, &g_pTextureDigital[0]);

	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * BULLET_VERTEX_NUM * MAX_BULLET,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffDigital,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDigital->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
		pVtx[nCntBullet * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);

		pVtx[nCntBullet * 4].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 1].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 2].rhw = 1.0f;
		pVtx[nCntBullet * 4 + 3].rhw = 1.0f;

		////カラー設定
		pVtx[nCntBullet * 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// A = 透明度
		pVtx[nCntBullet * 4 + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCntBullet * 4 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCntBullet * 4 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標設定
		pVtx[nCntBullet * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点BUFFERをアンロックする
	g_pVtxBuffDigital->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理2	終了処理
//=========================================================================================================//
void UninitDigital(void)
{//テクスチャの破棄
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < BULLET_MAX_Texture; nCntTexture++)
	{
		if (g_pTextureDigital[nCntTexture] != NULL)
		{
			g_pTextureDigital[nCntTexture]->Release();
			g_pTextureDigital[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffDigital != NULL)
	{
		g_pVtxBuffDigital->Release();
		g_pVtxBuffDigital = NULL;
	}
}
//=========================================================================================================//
// * 背景の描画処理3	更新処理
//=========================================================================================================//
void UpdateDigital(void)
{
	int nCntDigital;
	DIGITAL *g_aDigital = GetDigital();
	for (nCntDigital = 0; nCntDigital < MAX_BULLET; nCntDigital++)
	{
		if (g_aDigital[nCntDigital].bUse == true)
		{
			switch (g_aDigital[nCntDigital].state)
			{
			case DIGITALSTATE_NONE:
				g_aDigital[nCntDigital].col.a -= 0.0035f;
				if (g_aDigital[nCntDigital].col.a < 0.0f) { g_aDigital[nCntDigital].bUse = false; }
				break;
			case DIGITALSTATE_WAVE:
				if (g_aDigital[nCntDigital].nCntstate == 0)
				{//ジャンプ+半透明
					g_aDigital[nCntDigital].col.a = 0.5f;
					g_aDigital[nCntDigital].move.y = -7.0f;
				}
				if(g_aDigital[nCntDigital].nCntstate <= 0)
				{//FADE+重力
					if (g_aDigital[nCntDigital].col.a != 1.0f)
					{//超えたら
						g_aDigital[nCntDigital].col.a += 0.01f;
						if (g_aDigital[nCntDigital].col.a > 1.0f) { g_aDigital[nCntDigital].col.a = 1.0f; }
					}
					g_aDigital[nCntDigital].move.y += 0.35f;
					g_aDigital[nCntDigital].pos += g_aDigital[nCntDigital].move;
					if (g_aDigital[nCntDigital].pos.y > g_aDigital[nCntDigital].fHeight)
					{ 
						g_aDigital[nCntDigital].pos.y = g_aDigital[nCntDigital].fHeight;
						g_aDigital[nCntDigital].col.a -= 0.025f;
						if (g_aDigital[nCntDigital].col.a < 0.0f) { g_aDigital[nCntDigital].bUse = false; }
					}
				}
				g_aDigital[nCntDigital].nCntstate--;
				break;
			case DIGITALSTATE_WAVE0:
				if (g_aDigital[nCntDigital].nCntstate == 0)
				{//ジャンプ+半透明
					g_aDigital[nCntDigital].col.a = 0.5f;
					g_aDigital[nCntDigital].move.y = -7.0f;
				}
				if (g_aDigital[nCntDigital].nCntstate <= 0)
				{//FADE+重力
					if (g_aDigital[nCntDigital].col.a != 1.0f)
					{//超えたら
						g_aDigital[nCntDigital].col.a += 0.01f;
						if (g_aDigital[nCntDigital].col.a > 1.0f) { g_aDigital[nCntDigital].col.a = 1.0f; }
					}
					g_aDigital[nCntDigital].move.y += 0.35f;
					g_aDigital[nCntDigital].pos += g_aDigital[nCntDigital].move;
					if (g_aDigital[nCntDigital].pos.y > g_aDigital[nCntDigital].fHeight)
					{
						g_aDigital[nCntDigital].pos.y = g_aDigital[nCntDigital].fHeight;
						g_aDigital[nCntDigital].nCntstate = 60;
						//if (g_aDigital[nCntDigital].col.a < 0.0f) { g_aDigital[nCntDigital].bUse = false; }
					}
				}
				g_aDigital[nCntDigital].nCntstate--;
				break;
			case DIGITALSTATE_DAMAGE:
				if (g_aDigital[nCntDigital].nCntstate <= 0)
				{//移動
					if (g_aDigital[nCntDigital].col.a != 1.0f)
					{//超えたら
						g_aDigital[nCntDigital].col.a += 0.06f;
						g_aDigital[nCntDigital].pos.y += 14.0f;
						g_aDigital[nCntDigital].pos.x += 20.0f;

						if (g_aDigital[nCntDigital].col.a > 1.0f)
						{ 
							g_aDigital[nCntDigital].col.a = 1.0f;
							g_aDigital[nCntDigital].nCntstate = 9999;
						}
					}
				}
				if(g_aDigital[nCntDigital].nCntstate != 9999){ g_aDigital[nCntDigital].nCntstate--; }
				else
				{//止まった後
					g_aDigital[nCntDigital].col.a -= 0.01f;
					if (g_aDigital[nCntDigital].col.a < 0.7f)
					{ 
						g_aDigital[nCntDigital].pos.y += 2.8f;
						g_aDigital[nCntDigital].pos.x += 4.0f;
					}
					if (g_aDigital[nCntDigital].col.a < 0.0f) { g_aDigital[nCntDigital].bUse = false; }
				}
				break;
			case DIGITALSTATE_HEEL:
				if (g_aDigital[nCntDigital].nCntstate <= 0)
				{//移動
					if (g_aDigital[nCntDigital].col.a != 1.0f)
					{//超えたら
						g_aDigital[nCntDigital].col.a += 0.06f;
						g_aDigital[nCntDigital].pos.y -= 14.0f;
						g_aDigital[nCntDigital].pos.x -= 20.0f;

						if (g_aDigital[nCntDigital].col.a > 1.0f)
						{
							g_aDigital[nCntDigital].col.a = 1.0f;
							g_aDigital[nCntDigital].nCntstate = 9999;
						}
					}
				}
				if (g_aDigital[nCntDigital].nCntstate != 9999) { g_aDigital[nCntDigital].nCntstate--; }
				else
				{//止まった後
					g_aDigital[nCntDigital].col.a -= 0.01f;
					if (g_aDigital[nCntDigital].col.a < 0.7f)
					{
						g_aDigital[nCntDigital].pos.y -= 2.8f;
						g_aDigital[nCntDigital].pos.x -= 4.0f;
					}
					if (g_aDigital[nCntDigital].col.a < 0.0f) { g_aDigital[nCntDigital].bUse = false; }
				}
				break;
			}
		}
	}
}

//=========================================================================================================//
// * 弾の描画処理4	描画処理
//=========================================================================================================//
void DrawDigital(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	DIGITAL *g_aDigital = GetDigital();

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffDigital, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);
	// ポリゴンの描画

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDigital->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aDigital[nCntBullet].bUse == true)
		{
			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aDigital[nCntBullet].pos.x - g_aDigital[nCntBullet].nLengthX, g_aDigital[nCntBullet].pos.y - g_aDigital[nCntBullet].nLengthY, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aDigital[nCntBullet].pos.x + g_aDigital[nCntBullet].nLengthX, g_aDigital[nCntBullet].pos.y - g_aDigital[nCntBullet].nLengthY, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aDigital[nCntBullet].pos.x - g_aDigital[nCntBullet].nLengthX, g_aDigital[nCntBullet].pos.y + g_aDigital[nCntBullet].nLengthY, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aDigital[nCntBullet].pos.x + g_aDigital[nCntBullet].nLengthX, g_aDigital[nCntBullet].pos.y + g_aDigital[nCntBullet].nLengthY, 0.0f);

			pVtx[nCntBullet * 4].tex = D3DXVECTOR2((g_aDigital[nCntBullet].nDigital % 5) * (1.0f / 5), (g_aDigital[nCntBullet].nDigital / 5) * (1.0f / 2));
			pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2((g_aDigital[nCntBullet].nDigital % 5) * (1.0f / 5) + (1.0f / 5), (g_aDigital[nCntBullet].nDigital / 5) * (1.0f / 2));
			pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2((g_aDigital[nCntBullet].nDigital % 5) * (1.0f / 5), (g_aDigital[nCntBullet].nDigital / 5) * (1.0f / 2) + (1.0f / 2));
			pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2((g_aDigital[nCntBullet].nDigital % 5) * (1.0f / 5) + (1.0f / 5), (g_aDigital[nCntBullet].nDigital / 5) * (1.0f / 2) + (1.0f / 2));

			//カラー設定
			pVtx[nCntBullet * 4].col = g_aDigital[nCntBullet].col;	// A = 透明度
			pVtx[nCntBullet * 4 + 1].col = g_aDigital[nCntBullet].col;
			pVtx[nCntBullet * 4 + 2].col = g_aDigital[nCntBullet].col;
			pVtx[nCntBullet * 4 + 3].col = g_aDigital[nCntBullet].col;

			pDevice->SetTexture(0, g_pTextureDigital[0]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, BULLET_PRIMITIVE_NUM);
		}
	}

	//頂点BUFFERをアンロックする
	g_pVtxBuffDigital->Unlock();
}
//=============================================================================
// 弾の設定処理
//=============================================================================
void SetDigital(D3DXVECTOR3 pos, D3DXCOLOR col, DIGITALSTATE state, int nLengthX, int nLengthY, int nDigital)
{
	int nCntDigital;			//弾数えるの～
	int nCntBit = 0;
	int nCenter;
	int nCntCenter;
	int nPow;
	int nBit;
	DIGITAL *g_aDigital = GetDigital();

	for (nCntCenter = 0, nCenter = nDigital; nCenter > 0; nCenter /= 10, nCntCenter++) {}
	nPow = nCntCenter - 1;
	nBit = (int)pow(10.0f, nCntCenter - 1);
	if (nBit == 0) { nBit = 1; }
	do
	{//一つずつポリゴンを出す
		for (nCntDigital = 0; nCntDigital < MAX_BULLET; nCntDigital++)
		{//代入
			if (g_aDigital[nCntDigital].bUse == false)
			{
				g_aDigital[nCntDigital].pos = D3DXVECTOR3(pos.x + (nLengthX * 2 * nCntBit) - (nLengthX * nCntCenter),pos.y, 0.0f );
				g_aDigital[nCntDigital].fHeight = pos.y;
				g_aDigital[nCntDigital].state = state;
				g_aDigital[nCntDigital].nCntstate = nCntBit * 5;
				g_aDigital[nCntDigital].move = D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
				g_aDigital[nCntDigital].col = col;
				g_aDigital[nCntDigital].nDigital = nDigital / nBit % 10;
				g_aDigital[nCntDigital].nLengthX = nLengthX;
				g_aDigital[nCntDigital].nLengthY = nLengthY;
				g_aDigital[nCntDigital].bUse = true;
				nBit /= 10;
				nCntBit++;
				break;
			}
		}
	} while (nBit > 0);
}

//=========================================================================================================//
// * 全Animationスクロール処理
//=========================================================================================================//
void DigitalScroll(D3DXVECTOR3 move)
{
	int nCntBullet;
	DIGITAL *g_aBullet = GetDigital();

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true && g_aBullet[nCntBullet].state == DIGITALSTATE_WAVE)
		{//スクロール
			g_aBullet[nCntBullet].pos += move;
			g_aBullet[nCntBullet].fHeight += move.y;
		}
	}
}

//=============================================================================
// 弾の設定処理
//=============================================================================
void BreakDigital(void)
{
	int nCntBullet;
	DIGITAL *g_aBullet = GetDigital();

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//g_aBullet[nCntBullet].bUse = false;
	}
}

//=============================================================================
// 弾の取得
//=============================================================================
DIGITAL *GetDigital(void)
{
	static DIGITAL digital[MAX_BULLET];//弾の最大数

	return &digital[0];
}