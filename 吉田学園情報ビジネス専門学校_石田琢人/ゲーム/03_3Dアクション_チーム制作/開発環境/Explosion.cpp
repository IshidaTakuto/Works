//=========================================================================================================//
//
// [弾発射]->弾移動:爆発処理 [bullet.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "game.h"
#include "camera.h"

//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define EXPLOSION_VERTEX_NUM		(4)					// 頂点の数
#define EXPLOSION_PRIMITIVE_NUM		(2)					// プリミティブの数
#define EXPLOSION_MAX_TEX			(6)
#define EXPLOSION_TextureName		"data/TEXTURE/pipo-btleffect008.png"
#define EXPLOSION_TextureName1		"data/TEXTURE/bulleteffect000.png"
#define EXPLOSION_TextureName2		"data/TEXTURE/starburst_640x480.png"
#define EXPLOSION_TextureName3		"data/TEXTURE/EXP_GAGE.jpg"
#define EXPLOSION_TextureName4		"data/TEXTURE/frameeffects002m.png"
#define EXPLOSION_TextureName5		"data/TEXTURE/pipo-btleffect128.png"
#define	MAX_EXPLOSION				(80)		// 弾の最大数
#define HALFEXPLOSION				(30)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 Interval;	//間隔
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;	//色
	EXPLOSIONSTATE state;
	int nLengthX;		//Xの長さ
	int nLengthY;		//Yの長さ
	int nCntAnim;		//Animationのカウント
	int nPtnAnim;		//指定された座標で表示
	int nPtnHeight;		//画像の縦の分割数
	int nPtnWide;		//画像の横の分割数
	int nPtnSpeed;		//Animation速度
	int nType;			//画像の指定
	float steep;		//ポリゴンの角度
	ANIMATION anim;		//アニメーションの使い方
	int	nView;			//描画方法
	bool bUse;
}Explosin;

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9			g_pTextureExplosion[EXPLOSION_MAX_TEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffExplosion = NULL;						// 頂点バッファへのポインタ
Explosin					g_aExplosion[MAX_EXPLOSION];					//最大のAnimation数

//=========================================================================================================//
// * 背景の描画処理1	初期化処理
//=========================================================================================================//
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntBullet;

	// 弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		//g_aExplosion[nCntBullet].pos = { 0.0f, 0.0f, 0.0f };
		//g_aExplosion[nCntBullet].col = { 1.0f, 1.0f, 1.0f, };
		//g_aExplosion[nCntBullet].nCntAnim = -1;
		//g_aExplosion[nCntBullet].nPtnAnim = -1;
		//g_aExplosion[nCntBullet].nType = 0;
		g_aExplosion[nCntBullet].bUse = false;
	}

	//TEXTURE読み込み
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName, &g_pTextureExplosion[0]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName1, &g_pTextureExplosion[1]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName2, &g_pTextureExplosion[2]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName3, &g_pTextureExplosion[3]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName4, &g_pTextureExplosion[4]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName5, &g_pTextureExplosion[5]);

	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * EXPLOSION_VERTEX_NUM * MAX_EXPLOSION,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
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
		//pVtx[nCntBullet * 4].col = D3DCOLOR_RGBA(0,0,0, 255);	// A = 透明度
		//pVtx[nCntBullet * 4 + 1].col = D3DCOLOR_RGBA(0,0,0, 255);
		//pVtx[nCntBullet * 4 + 2].col = D3DCOLOR_RGBA(0,0,0, 255);
		//pVtx[nCntBullet * 4 + 3].col = D3DCOLOR_RGBA(0,0,0, 255);

		//テクスチャ座標設定
		//pVtx[nCntBullet * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		//pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		//pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		//pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点BUFFERをアンロックする
	g_pVtxBuffExplosion->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理2	終了処理
//=========================================================================================================//
void UninitExplosion(void)
{//テクスチャの破棄
	int nCntTexture;

	for (nCntTexture = 0; nCntTexture < EXPLOSION_MAX_TEX; nCntTexture++)
	{
		if (g_pTextureExplosion[nCntTexture] != NULL)
		{
			g_pTextureExplosion[nCntTexture]->Release();
			g_pTextureExplosion[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}
//=========================================================================================================//
// * 背景の描画処理3	更新処理
//=========================================================================================================//
void UpdateExplosion(void)
{
	int nCntBullet;

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{

		if (g_aExplosion[nCntBullet].bUse == true && g_aExplosion[nCntBullet].state != EXPLOSIONSTATE_LOAD)
		{//弾を前に飛ばす
		 //MessageBox(0, "本当すか？", "終了", MB_YESNO);
			if (g_aExplosion[nCntBullet].state == EXPLOSIONSTATE_DELETE)
			{//消えていく
				g_aExplosion[nCntBullet].col.a -= 0.05f;
				if (g_aExplosion[nCntBullet].col.a <= 0.0f) { g_aExplosion[nCntBullet].bUse = false; }
			}
			if (g_aExplosion[nCntBullet].state == EXPLOSIONSTATE_FADE)
			{//出てくる
				if (g_aExplosion[nCntBullet].col.a < 1.0f) { g_aExplosion[nCntBullet].col.a += 0.01f; }
			}

			g_aExplosion[nCntBullet].nCntAnim++;

			if (g_aExplosion[nCntBullet].nCntAnim % g_aExplosion[nCntBullet].nPtnSpeed == 0)
			{
				g_aExplosion[nCntBullet].nPtnAnim++;
				if (g_aExplosion[nCntBullet].nPtnAnim >= g_aExplosion[nCntBullet].nPtnHeight * g_aExplosion[nCntBullet].nPtnWide && g_aExplosion[nCntBullet].anim != ANIMATION_LOOP)
				{//全部1回表示したら消える
					g_aExplosion[nCntBullet].bUse = false;
				}
			}
		}
	}
}
//=========================================================================================================//
// * 背景の描画処理4	描画処理
//=========================================================================================================//
void DrawExplosion(void)
{
	Camera *pCamera = GetCamera();
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;
	int nView = 0;
	float fPercent;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aExplosion[nCntBullet].bUse == true)
		{
			if (nView != g_aExplosion[nCntBullet].nView)
			{
				switch (g_aExplosion[nCntBullet].nView)
				{
				case 0: //通常
					// αブレンディングを元に戻す
					pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
					pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
					break;
				case 1:	//加算合成
					// αブレンディングを加算合成に設定
					pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
					pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					pDevice->SetRenderState(D3DRS_DESTBLEND, 2);
					break;
				}
				nView = g_aExplosion[nCntBullet].nView;
			}

			if (g_aExplosion[nCntBullet].state == EXPLOSIONSTATE_GAGE)
			{//伸びる分 * (最大長さ / 長さ) 0.0が上 +倍率分
				if (pCamera[0].state == CAMERA_STATE_GAME)
				{
					g_aExplosion[nCntBullet].col.a = 1.0f;
					pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x + (sinf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthY), g_aExplosion[nCntBullet].pos.y + (cosf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthY), 0.0f);
					pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x + (sinf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthY) - (cosf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthX), g_aExplosion[nCntBullet].pos.y + (sinf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthX) + (cosf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthY), 0.0f);
					pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x, g_aExplosion[nCntBullet].pos.y, 0.0f);
					pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x - (cosf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthX), g_aExplosion[nCntBullet].pos.y + (sinf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthX), 0.0f);

					fPercent = g_aExplosion[nCntBullet].nLengthY / 170.0f;
					if (fPercent > 1.0f) { fPercent = 1.0f; }
					fPercent -= 1.0f;
					pVtx[nCntBullet * 4].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight) - ((1.0f / g_aExplosion[nCntBullet].nPtnHeight) * fPercent));
					pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide) + (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight) - ((1.0f / g_aExplosion[nCntBullet].nPtnHeight) * fPercent));
					pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight) + (1.0f / g_aExplosion[nCntBullet].nPtnHeight));
					pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide) + (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight) + (1.0f / g_aExplosion[nCntBullet].nPtnHeight));
				}
				else { g_aExplosion[nCntBullet].col.a = 0.0f; }
			}
			else
			{
				pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x + (sinf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthY) + (cosf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthX), g_aExplosion[nCntBullet].pos.y - (sinf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthX) + (cosf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthY), 0.0f);
				pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x + (sinf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthY) - (cosf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthX), g_aExplosion[nCntBullet].pos.y + (sinf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthX) + (cosf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthY), 0.0f);
				pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x - (sinf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthY) + (cosf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthX), g_aExplosion[nCntBullet].pos.y - (sinf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthX) - (cosf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthY), 0.0f);
				pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aExplosion[nCntBullet].pos.x - (sinf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthY) - (cosf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthX), g_aExplosion[nCntBullet].pos.y + (sinf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthX) - (cosf(g_aExplosion[nCntBullet].steep) * g_aExplosion[nCntBullet].nLengthY), 0.0f);

				pVtx[nCntBullet * 4].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight));
				pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide) + (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight));
				pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight) + (1.0f / g_aExplosion[nCntBullet].nPtnHeight));
				pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2((g_aExplosion[nCntBullet].nPtnAnim % g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnWide) + (1.0f / g_aExplosion[nCntBullet].nPtnWide), (g_aExplosion[nCntBullet].nPtnAnim / g_aExplosion[nCntBullet].nPtnWide) * (1.0f / g_aExplosion[nCntBullet].nPtnHeight) + (1.0f / g_aExplosion[nCntBullet].nPtnHeight));
			}
			//カラー設定
			pVtx[nCntBullet * 4].col = g_aExplosion[nCntBullet].col;	// A = 透明度
			pVtx[nCntBullet * 4 + 1].col = g_aExplosion[nCntBullet].col;
			pVtx[nCntBullet * 4 + 2].col = g_aExplosion[nCntBullet].col;
			pVtx[nCntBullet * 4 + 3].col = g_aExplosion[nCntBullet].col;

			// テクスチャの設定 ポリゴンの描画
			pDevice->SetTexture(0, g_pTextureExplosion[g_aExplosion[nCntBullet].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, EXPLOSION_PRIMITIVE_NUM);
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffExplosion->Unlock();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//=============================================================================
// 弾の設定処理
//=============================================================================
int SetExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 Interval, D3DXCOLOR col , int nLengthX, int nLengthY, int nPtnWide, int nPtnHeight, int nPtnSpeed, int nType, ANIMATION anim, EXPLOSIONSTATE state, float steep, int nView)
{
	int nCntBullet;

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aExplosion[nCntBullet].bUse == false)
		{
			g_aExplosion[nCntBullet].steep = steep;
			g_aExplosion[nCntBullet].pos = pos;
			g_aExplosion[nCntBullet].Interval = Interval;
			g_aExplosion[nCntBullet].col = col;
			g_aExplosion[nCntBullet].state = state;
			g_aExplosion[nCntBullet].nCntAnim = -1;
			g_aExplosion[nCntBullet].nPtnAnim = -1;
			g_aExplosion[nCntBullet].nPtnHeight = nPtnHeight;
			g_aExplosion[nCntBullet].nPtnWide = nPtnWide;
			g_aExplosion[nCntBullet].nPtnSpeed = nPtnSpeed;
			g_aExplosion[nCntBullet].nLengthX = nLengthX;
			g_aExplosion[nCntBullet].nLengthY = nLengthY;
			g_aExplosion[nCntBullet].nType = nType;
			g_aExplosion[nCntBullet].anim = anim;
			g_aExplosion[nCntBullet].nView = nView;
			g_aExplosion[nCntBullet].bUse = true;
			break;
		}
	}
	return nCntBullet;
}
//=========================================================================================================//
// * 全Animationスクロール処理
//=========================================================================================================//
void ExplosionScroll(D3DXVECTOR3 move)
{
	int nCntBullet;

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aExplosion[nCntBullet].bUse == true)
		{//スクロール
			g_aExplosion[nCntBullet].pos += move;
		}
	}
}
//=========================================================================================================//
// * 全Animation破棄処理
//=========================================================================================================//
void BreakExplosion(void)
{
	int nCntAnim;

	for (nCntAnim = 0; nCntAnim < MAX_EXPLOSION; nCntAnim++)
	{
		g_aExplosion[nCntAnim].bUse = false;
	}
}
//=========================================================================================================//
// * 背景の描画処理4	描画処理
//=========================================================================================================//
void CheckEXP(float fEXP, float fMaxEXP, int nCntExplosion)
{
	float fPercent = fEXP / fMaxEXP;

	//経験値のパーセンテージが100％を過ぎていれば戻す
	if (fPercent > 1.0) { fPercent = 1.0f; }
	g_aExplosion[nCntExplosion].nLengthY = (int)(170.0f * fPercent);
}
//=========================================================================================================//
// * 背景の描画処理4	描画処理
//=========================================================================================================//
void ChangeExplosion(int nCntExplosion, EXPLOSIONSTATE state)
{
	g_aExplosion[nCntExplosion].state = state;
}
