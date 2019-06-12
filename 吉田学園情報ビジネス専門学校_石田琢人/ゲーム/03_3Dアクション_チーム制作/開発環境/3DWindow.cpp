//=========================================================================================================//
//
// [弾発射]->弾移動:爆発処理 [bullet.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "input.h"

//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define EXPLOSION_VERTEX_NUM		(4)					// 頂点の数
#define EXPLOSION_PRIMITIVE_NUM		(2)					// プリミティブの数
#define EXPLOSION_MAX_TEX			(5)
#define EXPLOSION_TextureName		"data/TEXTURE/GAGE000.jpg"
#define EXPLOSION_TextureName1		"data/TEXTURE/Lock_On001.png"
#define EXPLOSION_TextureName2		"data/TEXTURE/Lock_On100.png"
#define EXPLOSION_TextureName3		"data/TEXTURE/maxresdefault.jpg"
#define EXPLOSION_TextureName4		"data/TEXTURE/explosion.jpg"
#define	MAX_EXPLOSION				(1280)		// 弾の最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//位置
	D3DXCOLOR col;	//色
	WINDOWSTATE state;
	float		fCntState;
	int nType;			//画像の指定
	int nView;
	float fLengthX;		//Xの長さ
	float fLengthY;		//Yの長さ
	float fAngle;		//Z軸の角度
	D3DXMATRIX mtxWorldBillBoord;	//ワールドマトリックス
	bool bUse;

}ZWindow;

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9			g_pTextureZWindow[EXPLOSION_MAX_TEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffZWindow = NULL;						// 頂点バッファへのポインタ
ZWindow						g_aZWindow[MAX_EXPLOSION];					//最大のAnimation数

//=========================================================================================================//
// * 背景の描画処理1	初期化処理
//=========================================================================================================//
void InitZWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntBullet;

	// 弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		//g_aExplosion[nCntBullet].pos = { 0.0f, 0.0f, 0.0f };
		//g_aExplosion[nCntBullet].col = { 1.0f, 1.0f, 1.0f, };
		//g_aExplosion[nCntBullet].nCntAnim = -1;
		//g_aExplosion[nCntBullet].nPtnAnim = -1;
		//g_aExplosion[nCntBullet].nType = 0;
		g_aZWindow[nCntBullet].bUse = false;
	}

	//TEXTURE読み込み
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName, &g_pTextureZWindow[0]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName1, &g_pTextureZWindow[1]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName2, &g_pTextureZWindow[2]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName3, &g_pTextureZWindow[3]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName4, &g_pTextureZWindow[4]);

	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * EXPLOSION_VERTEX_NUM * MAX_EXPLOSION,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffZWindow,
		NULL);

	VERTEX_3D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffZWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{

		pVtx[nCntBullet * 4].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[nCntBullet * 4 + 1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[nCntBullet * 4 + 2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[nCntBullet * 4 + 3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//テクスチャ座標設定
		pVtx[nCntBullet * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	//頂点BUFFERをアンロックする
	g_pVtxBuffZWindow->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理2	終了処理
//=========================================================================================================//
void UninitZWindow(void)
{//テクスチャの破棄
	int nCntTexture;

	BreakZWindow();

	for (nCntTexture = 0; nCntTexture < EXPLOSION_MAX_TEX; nCntTexture++)
	{
		if (g_pTextureZWindow[nCntTexture] != NULL)
		{
			g_pTextureZWindow[nCntTexture]->Release();
			g_pTextureZWindow[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffZWindow != NULL)
	{
		g_pVtxBuffZWindow->Release();
		g_pVtxBuffZWindow = NULL;
	}
}
//=========================================================================================================//
// * 背景の描画処理3	更新処理
//=========================================================================================================//
void UpdateZWindow(void)
{
	int nCntBullet;

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aZWindow[nCntBullet].bUse == true)
		{
			g_aZWindow[nCntBullet].pos += g_aZWindow[nCntBullet].move;

			switch (g_aZWindow[nCntBullet].state)
			{
			case WINDOWSTATE_Delete0:
				g_aZWindow[nCntBullet].bUse = false; break;

			case WINDOWSTATE_STAMINAGAGE:
			case WINDOWSTATE_MAXSTAMINAGAGE:
				if (g_aZWindow[nCntBullet].fCntState > 0.0f)
				{//スタミナゲージが使用されているなら
					if (g_aZWindow[nCntBullet].col.a == 1.0f) { g_aZWindow[nCntBullet].fCntState -= 1.0f; }
					else
					{
						g_aZWindow[nCntBullet].col.a += 0.08f;
						if (g_aZWindow[nCntBullet].col.a >= 1.0f) { g_aZWindow[nCntBullet].col.a = 1.0f; }
					}
				}
				else
				{//だんだん消える
					if (g_aZWindow[nCntBullet].col.a != 0.0f)
					{
						g_aZWindow[nCntBullet].col.a -= 0.08f;
						if (g_aZWindow[nCntBullet].col.a <= 0.0f) { g_aZWindow[nCntBullet].col.a = 0.0f; }
					}
				}
				break;

			case WINDOWSTATE_FADE_Z:
				g_aZWindow[nCntBullet].fLengthX *= 1.01f;
				g_aZWindow[nCntBullet].fLengthY *= 1.01f;
			case WINDOWSTATE_FADEOUT:
				if (g_aZWindow[nCntBullet].col.a != 0.0f)
				{
					g_aZWindow[nCntBullet].col.a -= g_aZWindow[nCntBullet].fCntState;
					if (g_aZWindow[nCntBullet].col.a <= 0.0f) { g_aZWindow[nCntBullet].bUse = false; }
				}
				break;
			}//swich
		}//bUse
	}//for
}
//=========================================================================================================//
// * 背景の描画処理4	描画処理
//=========================================================================================================//
void DrawZWindow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		//計算用マトリックス
	VERTEX_3D *pVtx;		//頂点情報へのポインタ
	int nView = 0;

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffZWindow->Lock(0, 0, (void**)&pVtx, 0);
	//ポリゴンの描画
	for (int nCntAnim = 0; nCntAnim < MAX_EXPLOSION; nCntAnim++)
	{
		if (g_aZWindow[nCntAnim].bUse == true)
		{
			if (nView != g_aZWindow[nCntAnim].nView)
			{
				switch (g_aZWindow[nCntAnim].nView)
				{
				case 0: //通常
					pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
					//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
					break;
				case 1:	//加算合成
					pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
					//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					pDevice->SetRenderState(D3DRS_DESTBLEND, 2);
					break;
				case 2:	//減算合成
					pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
					//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
					pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

				}
				nView = g_aZWindow[nCntAnim].nView;
			}

			//Zテストの確認
			if(g_aZWindow[nCntAnim].nType != 0){ pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE); }
			if (g_aZWindow[nCntAnim].state >= WINDOWSTATE_STAMINAGAGE && g_aZWindow[nCntAnim].state <= WINDOWSTATE_AUTOLIFEGAGE)
			{
				pVtx[nCntAnim * 4].pos = D3DXVECTOR3(0.0f, g_aZWindow[nCntAnim].fLengthY * 1.0f, 0.0f);
				pVtx[nCntAnim * 4 + 1].pos = D3DXVECTOR3(g_aZWindow[nCntAnim].fLengthX * 1.0f, g_aZWindow[nCntAnim].fLengthY * 1.0f, 0.0f);
				pVtx[nCntAnim * 4 + 2].pos = D3DXVECTOR3(0.0f, -g_aZWindow[nCntAnim].fLengthY * 1.0f, 0.0f);
				pVtx[nCntAnim * 4 + 3].pos = D3DXVECTOR3(g_aZWindow[nCntAnim].fLengthX * 1.0f, -g_aZWindow[nCntAnim].fLengthY * 1.0f, 0.0f);

				pVtx[nCntAnim * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntAnim * 4 + 1].tex = D3DXVECTOR2((g_aZWindow[nCntAnim].fLengthX / 100.0f) * 1.0f, 0.0f);
				pVtx[nCntAnim * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[nCntAnim * 4 + 3].tex = D3DXVECTOR2((g_aZWindow[nCntAnim].fLengthX / 100.0f) * 1.0f, 1.0f);
			}

				pVtx[nCntAnim * 4].col = g_aZWindow[nCntAnim].col;	// A = 透明度
				pVtx[nCntAnim * 4 + 1].col = g_aZWindow[nCntAnim].col;
				pVtx[nCntAnim * 4 + 2].col = g_aZWindow[nCntAnim].col;
				pVtx[nCntAnim * 4 + 3].col = g_aZWindow[nCntAnim].col;

		 // ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aZWindow[nCntAnim].mtxWorldBillBoord);

			//ビルボード
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//逆行列を設定
			g_aZWindow[nCntAnim].mtxWorldBillBoord._11 = mtxView._11;
			g_aZWindow[nCntAnim].mtxWorldBillBoord._12 = mtxView._21;
			g_aZWindow[nCntAnim].mtxWorldBillBoord._13 = mtxView._31;
			g_aZWindow[nCntAnim].mtxWorldBillBoord._21 = mtxView._12;
			g_aZWindow[nCntAnim].mtxWorldBillBoord._22 = mtxView._22;
			g_aZWindow[nCntAnim].mtxWorldBillBoord._23 = mtxView._32;
			g_aZWindow[nCntAnim].mtxWorldBillBoord._31 = mtxView._13;
			g_aZWindow[nCntAnim].mtxWorldBillBoord._32 = mtxView._23;
			g_aZWindow[nCntAnim].mtxWorldBillBoord._33 = mtxView._33;

			////if (g_aZWindow[nCntAnim].nType == 3)
			//{// 回転を反映
			//	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, g_aZWindow[nCntAnim].fAngle);
			//	D3DXMatrixMultiply(&g_aZWindow[nCntAnim].mtxWorldBillBoord, &g_aZWindow[nCntAnim].mtxWorldBillBoord, &mtxRot);
			//}

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aZWindow[nCntAnim].pos.x, g_aZWindow[nCntAnim].pos.y, g_aZWindow[nCntAnim].pos.z);

			D3DXMatrixMultiply(&g_aZWindow[nCntAnim].mtxWorldBillBoord, &g_aZWindow[nCntAnim].mtxWorldBillBoord, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aZWindow[nCntAnim].mtxWorldBillBoord);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffZWindow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureZWindow[g_aZWindow[nCntAnim].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAnim * 4, 2);

			//Zテストを戻す
			if (g_aZWindow[nCntAnim].nType != 0) { pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE); }
		}
	}
	pDevice->SetTexture(0, NULL);

	//頂点BUFFERをアンロックする
	g_pVtxBuffZWindow->Unlock();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}
//=============================================================================
// 弾の設定処理
//=============================================================================
int SetZWindow(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col , float nLengthX, float nLengthY, int nType, int nView, WINDOWSTATE state, float fCntState)
{
	int nCntBullet;
	VERTEX_3D *pVtx;		//頂点情報へのポインタ

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffZWindow->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aZWindow[nCntBullet].bUse == false)
		{
			g_aZWindow[nCntBullet].pos = pos;
			g_aZWindow[nCntBullet].move = move;
			g_aZWindow[nCntBullet].col = col;
			g_aZWindow[nCntBullet].fLengthX = nLengthX;
			g_aZWindow[nCntBullet].fLengthY = nLengthY;
			g_aZWindow[nCntBullet].nType = nType;
			g_aZWindow[nCntBullet].nView = nView;
			g_aZWindow[nCntBullet].state = state;
			g_aZWindow[nCntBullet].fCntState = fCntState;
			g_aZWindow[nCntBullet].fAngle = (rand() % 314) * 0.01f;
			g_aZWindow[nCntBullet].bUse = true;

			if (nType == 0)
			{
				pVtx[nCntBullet * 4].pos = D3DXVECTOR3(0.0f, g_aZWindow[nCntBullet].fLengthY * 1.0f, 0.0f);
				pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aZWindow[nCntBullet].fLengthX * 1.0f, g_aZWindow[nCntBullet].fLengthY * 1.0f, 0.0f);
				pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(0.0f, -g_aZWindow[nCntBullet].fLengthY * 1.0f, 0.0f);
				pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aZWindow[nCntBullet].fLengthX * 1.0f, -g_aZWindow[nCntBullet].fLengthY * 1.0f, 0.0f);
			}
			else
			{
				pVtx[nCntBullet * 4].pos = D3DXVECTOR3(-g_aZWindow[nCntBullet].fLengthX * 1.0f, g_aZWindow[nCntBullet].fLengthY * 1.0f, 0.0f);
				pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aZWindow[nCntBullet].fLengthX * 1.0f, g_aZWindow[nCntBullet].fLengthY * 1.0f, 0.0f);
				pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(-g_aZWindow[nCntBullet].fLengthX * 1.0f, -g_aZWindow[nCntBullet].fLengthY * 1.0f, 0.0f);
				pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aZWindow[nCntBullet].fLengthX * 1.0f, -g_aZWindow[nCntBullet].fLengthY * 1.0f, 0.0f);
			}
			pVtx[nCntBullet * 4].col = col;	// A = 透明度
			pVtx[nCntBullet * 4 + 1].col = col;
			pVtx[nCntBullet * 4 + 2].col = col;
			pVtx[nCntBullet * 4 + 3].col = col;

			//テクスチャ座標設定
			pVtx[nCntBullet * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffZWindow->Unlock();

	return nCntBullet;
}
//=========================================================================================================//
// * 全Animation破棄処理
//=========================================================================================================//
void BreakZWindow(void)
{
	int nCntAnim;

	for (nCntAnim = 0; nCntAnim < MAX_EXPLOSION; nCntAnim++)
	{
		g_aZWindow[nCntAnim].bUse = false;
	}
}
//=========================================================================================================//
// * 番号でゲージの更新
//=========================================================================================================//
void ChangeZWindow(int nCount, float fMax, float fMin, D3DXVECTOR3 pos, bool bUse)
{
	float fPlus;

	if (g_aZWindow[nCount].bUse == true)
	{
		float fPercent;
		if (fMin < 0.0f) { fMin = 0.0f; }
		//最大値から%を求める
		fPercent = fMin / fMax;
		if (fPercent > 1.0f) { fPercent = 1.0f; }
		if (g_aZWindow[nCount].state == WINDOWSTATE_LIFEGAGE) 
		{ 
			if (g_aZWindow[nCount].fLengthX > 50.0f * fPercent){ g_aZWindow[nCount].fLengthX = 50.0f * fPercent; }
			else
			{
				fPlus = (50.0f * fPercent - g_aZWindow[nCount].fLengthX) * 0.03f;
				if (fPlus < 50.0f * 0.002f) { fPlus = 50.0f * 0.002f; }
				g_aZWindow[nCount].fLengthX += fPlus;
				if (g_aZWindow[nCount].fLengthX > 50.0f * fPercent) { g_aZWindow[nCount].fLengthX = 50.0f * fPercent; }
			}
		}
		else if (g_aZWindow[nCount].state == WINDOWSTATE_AUTOLIFEGAGE)
		{ 
			if (g_aZWindow[nCount].fLengthX > 50.0f * fPercent)
			{
				g_aZWindow[nCount].fLengthX -= 50.0f * 0.002f;
				g_aZWindow[nCount].col = D3DXCOLOR(0.76f, 0.1f, 0.0f, 1.0f);
				if (g_aZWindow[nCount].fLengthX < 50.0f * fPercent) { g_aZWindow[nCount].fLengthX = 50.0f * fPercent; }
			}
			else if (g_aZWindow[nCount].fLengthX < 50.0f * fPercent)
			{ 
				g_aZWindow[nCount].fLengthX = 50.0f * fPercent;
				g_aZWindow[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else{ g_aZWindow[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
		}
		else if (g_aZWindow[nCount].state == WINDOWSTATE_STAMINAGAGE)
		{
			g_aZWindow[nCount].fLengthX = 50.0f * fPercent;
			if(fPercent != 1.0f){ g_aZWindow[nCount].fCntState = 30.0f; }
		}
		else if (g_aZWindow[nCount].state == WINDOWSTATE_MAXSTAMINAGAGE)
		{
			if (fPercent != 1.0f) { g_aZWindow[nCount].fCntState = 30.0f; }
		}
		g_aZWindow[nCount].pos = pos;
		g_aZWindow[nCount].bUse = bUse;
	}
}
