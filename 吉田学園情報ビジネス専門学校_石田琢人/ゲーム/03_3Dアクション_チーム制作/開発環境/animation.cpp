//=========================================================================================================//
//
// [弾発射]->弾移動:爆発処理 [bullet.cpp]
// Author : Ryou Sugimoto
//
//=========================================================================================================//
#include "main.h"
#include "camera.h"

//*********************************************************************************************************//
// マクロ定義
//*********************************************************************************************************//
#define EXPLOSION_VERTEX_NUM		(4)					// 頂点の数
#define EXPLOSION_PRIMITIVE_NUM		(2)					// プリミティブの数
#define EXPLOSION_MAX_TEX			(13)
#define EXPLOSION_TextureName		"data/TEXTURE/Effect_DASH000.jpg"
#define EXPLOSION_TextureName1		"data/TEXTURE/pipo-btleffect107a.png"
#define EXPLOSION_TextureName2		"data/TEXTURE/pipo-btleffect161.png"
#define EXPLOSION_TextureName3		"data/TEXTURE/pipo-btleffect021.png"
#define EXPLOSION_TextureName4		"data/TEXTURE/pipo-btleffect159.png"
#define EXPLOSION_TextureName5		"data/TEXTURE/kaenbeam.png"
#define EXPLOSION_TextureName6		"data/TEXTURE/pipo-btleffect022.png"
#define EXPLOSION_TextureName7		"data/TEXTURE/pipo-btleffect008.png"
#define EXPLOSION_TextureName8		"data/TEXTURE/fireball00011.png"
#define EXPLOSION_TextureName9		"data/TEXTURE/pipo-btleffect041.png"
#define EXPLOSION_TextureName10		"data/TEXTURE/pipo-btleffect009.png"
#define EXPLOSION_TextureName11		"data/TEXTURE/heal.png"
#define EXPLOSION_TextureName12		"data/TEXTURE/powUP.jpg"
#define	MAX_EXPLOSION				(1280)		// 弾の最大数
#define HALFEXPLOSION				(30)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;	//色
	int nLengthX;		//Xの長さ
	int nLengthY;		//Yの長さ
	int nCntAnim;		//Animationのカウント
	int nPtnAnim;		//指定された座標で表示
	int nPtnHeight;		//画像の縦の分割数
	int nPtnWide;		//画像の横の分割数
	int nPtnSpeed;		//Animation速度
	int nType;			//画像の指定
	int nView;			//合成方法
	float fCntState;
	ANIMATION anim;		//アニメーションの使い方
	D3DXMATRIX mtxWorldBillBoord;	//ワールドマトリックス
	bool bUse;
}Animation;

//*********************************************************************************************************//
// グローバル変数
//*********************************************************************************************************//
LPDIRECT3DTEXTURE9			g_pTextureAnimation[EXPLOSION_MAX_TEX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffAnimation = NULL;						// 頂点バッファへのポインタ
Animation					g_aAnimation[MAX_EXPLOSION];					//最大のAnimation数

//=========================================================================================================//
// * 背景の描画処理1	初期化処理
//=========================================================================================================//
void InitAnimation(void)
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
		g_aAnimation[nCntBullet].bUse = false;
	}

	//TEXTURE読み込み
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName, &g_pTextureAnimation[0]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName1, &g_pTextureAnimation[1]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName2, &g_pTextureAnimation[2]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName3, &g_pTextureAnimation[3]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName4, &g_pTextureAnimation[4]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName5, &g_pTextureAnimation[5]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName6, &g_pTextureAnimation[6]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName7, &g_pTextureAnimation[7]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName8, &g_pTextureAnimation[8]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName9, &g_pTextureAnimation[9]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName10, &g_pTextureAnimation[10]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName11, &g_pTextureAnimation[11]);
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TextureName12, &g_pTextureAnimation[12]);

	//頂点BUFFERの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * EXPLOSION_VERTEX_NUM * MAX_EXPLOSION,		//確保するBUFFERのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,				//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffAnimation,
		NULL);

	VERTEX_3D *pVtx;		//頂点情報へのポインタ
							//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAnimation->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		//頂点座標の設定
		//pVtx[nCntBullet * 4].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(0, 0, 0.0f);
		//pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(0, 0, 0.0f);

		pVtx[nCntBullet * 4].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[nCntBullet * 4 + 1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[nCntBullet * 4 + 2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[nCntBullet * 4 + 3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	g_pVtxBuffAnimation->Unlock();
}
//=========================================================================================================//
// * 背景の描画処理2	終了処理
//=========================================================================================================//
void UninitAnimation(void)
{//テクスチャの破棄
	int nCntTexture;

	BreakAnimation();

	for (nCntTexture = 0; nCntTexture < EXPLOSION_MAX_TEX; nCntTexture++)
	{
		if (g_pTextureAnimation[nCntTexture] != NULL)
		{
			g_pTextureAnimation[nCntTexture]->Release();
			g_pTextureAnimation[nCntTexture] = NULL;
		}
	}
	if (g_pVtxBuffAnimation != NULL)
	{
		g_pVtxBuffAnimation->Release();
		g_pVtxBuffAnimation = NULL;
	}
}
//=========================================================================================================//
// * 背景の描画処理3	更新処理
//=========================================================================================================//
void UpdateAnimation(void)
{
	int nCntBullet;
	VERTEX_3D *pVtx;		//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスを取得する
	pDevice = GetDevice();

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAnimation->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{

		if (g_aAnimation[nCntBullet].bUse == true)
		{//弾を前に飛ばす
		 //MessageBox(0, "本当すか？", "終了", MB_YESNO);
			if (g_aAnimation[nCntBullet].anim == ANIMATION_FADE)
			{ 
				g_aAnimation[nCntBullet].col.a += g_aAnimation[nCntBullet].fCntState;
				if (g_aAnimation[nCntBullet].col.a >= 1.0f)
				{ 
					g_aAnimation[nCntBullet].col.a = 1.0f;
					//g_aAnimation[nCntBullet].fCntState *= -1;
				}
				pVtx[nCntBullet * 4].col = g_aAnimation[nCntBullet].col;	// A = 透明度
				pVtx[nCntBullet * 4 + 1].col = g_aAnimation[nCntBullet].col;
				pVtx[nCntBullet * 4 + 2].col = g_aAnimation[nCntBullet].col;
				pVtx[nCntBullet * 4 + 3].col = g_aAnimation[nCntBullet].col;
			}
			if (g_aAnimation[nCntBullet].anim == ANIMATION_UP)
			{
				g_aAnimation[nCntBullet].nLengthX -= 5;
				g_aAnimation[nCntBullet].nLengthY -= 5;
				if (g_aAnimation[nCntBullet].nLengthX < 0) { g_aAnimation[nCntBullet].nLengthX = 0; g_aAnimation[nCntBullet].bUse = false; }
				if (g_aAnimation[nCntBullet].nLengthY < 0) { g_aAnimation[nCntBullet].nLengthY = 0;  g_aAnimation[nCntBullet].bUse = false; }

				pVtx[nCntBullet * 4].pos = D3DXVECTOR3(-g_aAnimation[nCntBullet].nLengthX * 1.0f, g_aAnimation[nCntBullet].nLengthY * 1.0f, 0.0f);
				pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].nLengthX * 1.0f, g_aAnimation[nCntBullet].nLengthY * 1.0f, 0.0f);
				pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(-g_aAnimation[nCntBullet].nLengthX * 1.0f, -g_aAnimation[nCntBullet].nLengthY * 1.0f, 0.0f);
				pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].nLengthX * 1.0f, -g_aAnimation[nCntBullet].nLengthY * 1.0f, 0.0f);
				SetAnimation(g_aAnimation[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), g_aAnimation[nCntBullet].nLengthX, g_aAnimation[nCntBullet].nLengthY, 8, 1, 3, 6, 1, ANIMATION_NORMAL);
					//SetAnimation(g_aAnimation[nCntBullet].pos + D3DXVECTOR3(rand() % 1000 * 0.1f - 50.0f, 0.0f, rand() % 1000 * 0.1f - 50.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 60, 8, 1, 4, 6, ANIMATION_NORMAL);
				g_aAnimation[nCntBullet].pos.y += 10.0f;
			}
			g_aAnimation[nCntBullet].nCntAnim++;

			pVtx[nCntBullet * 4].tex = D3DXVECTOR2((g_aAnimation[nCntBullet].nPtnAnim % g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnWide), (g_aAnimation[nCntBullet].nPtnAnim / g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnHeight));
			pVtx[nCntBullet * 4 + 1].tex = D3DXVECTOR2((g_aAnimation[nCntBullet].nPtnAnim % g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnWide) + (1.0f / g_aAnimation[nCntBullet].nPtnWide), (g_aAnimation[nCntBullet].nPtnAnim / g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnHeight));
			pVtx[nCntBullet * 4 + 2].tex = D3DXVECTOR2((g_aAnimation[nCntBullet].nPtnAnim % g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnWide), (g_aAnimation[nCntBullet].nPtnAnim / g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnHeight) + (1.0f / g_aAnimation[nCntBullet].nPtnHeight));
			pVtx[nCntBullet * 4 + 3].tex = D3DXVECTOR2((g_aAnimation[nCntBullet].nPtnAnim % g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnWide) + (1.0f / g_aAnimation[nCntBullet].nPtnWide), (g_aAnimation[nCntBullet].nPtnAnim / g_aAnimation[nCntBullet].nPtnWide) * (1.0f / g_aAnimation[nCntBullet].nPtnHeight) + (1.0f / g_aAnimation[nCntBullet].nPtnHeight));
			if (g_aAnimation[nCntBullet].nCntAnim % g_aAnimation[nCntBullet].nPtnSpeed == 0)
			{
				g_aAnimation[nCntBullet].nPtnAnim++;
				if (g_aAnimation[nCntBullet].nPtnAnim + 1 > g_aAnimation[nCntBullet].nPtnHeight * g_aAnimation[nCntBullet].nPtnWide && g_aAnimation[nCntBullet].anim != ANIMATION_LOOP)
				{//全部1回表示したら消える
					g_aAnimation[nCntBullet].bUse = false;
				}
			}
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffAnimation->Unlock();

}
//=========================================================================================================//
// * 背景の描画処理4	描画処理
//=========================================================================================================//
void DrawAnimation(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		//計算用マトリックス
	Camera *pCamera = GetCamera();
	int nView = 0;
	int nZView = 1;

	//アルファテスト処理(透明化による画像の切り取り現象を無効にする)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 220);//REF = Reference:参照
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);//FUNC = Function:機能 //GREATER = ～より大きい
	//ポリゴンの描画
	for (int nCntAnim = 0; nCntAnim < MAX_EXPLOSION; nCntAnim++)
	{
		if (g_aAnimation[nCntAnim].bUse == true)
		{
			if (nView != g_aAnimation[nCntAnim].nView % 10)
			{
				switch (g_aAnimation[nCntAnim].nView % 10)
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
				nView = g_aAnimation[nCntAnim].nView % 10;
			}
			if (nZView != g_aAnimation[nCntAnim].nView / 10)
			{
					switch (g_aAnimation[nCntAnim].nView / 10)
					{
					case 0: //Z解放
						pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
						//切り取りを戻す
						pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
						break;
					case 1:	//通常
						pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
						//アルファテスト処理(透明化による画像の切り取り現象を無効にする)
						pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
						pDevice->SetRenderState(D3DRS_ALPHAREF, 220);//REF = Reference:参照
						pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);//FUNC = Function:機能 //GREATER = ～より大きい
						break;
					}
					nZView = g_aAnimation[nCntAnim].nView / 10;
			}

		 // ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aAnimation[nCntAnim].mtxWorldBillBoord);

			//ビルボード
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//逆行列を設定
			g_aAnimation[nCntAnim].mtxWorldBillBoord._11 = mtxView._11;
			g_aAnimation[nCntAnim].mtxWorldBillBoord._12 = mtxView._21;
			g_aAnimation[nCntAnim].mtxWorldBillBoord._13 = mtxView._31;
			g_aAnimation[nCntAnim].mtxWorldBillBoord._21 = mtxView._12;
			g_aAnimation[nCntAnim].mtxWorldBillBoord._22 = mtxView._22;
			g_aAnimation[nCntAnim].mtxWorldBillBoord._23 = mtxView._32;
			g_aAnimation[nCntAnim].mtxWorldBillBoord._31 = mtxView._13;
			g_aAnimation[nCntAnim].mtxWorldBillBoord._32 = mtxView._23;
			g_aAnimation[nCntAnim].mtxWorldBillBoord._33 = mtxView._33;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aAnimation[nCntAnim].pos.x, g_aAnimation[nCntAnim].pos.y, g_aAnimation[nCntAnim].pos.z);

			D3DXMatrixMultiply(&g_aAnimation[nCntAnim].mtxWorldBillBoord, &g_aAnimation[nCntAnim].mtxWorldBillBoord, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aAnimation[nCntAnim].mtxWorldBillBoord);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffAnimation, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureAnimation[g_aAnimation[nCntAnim].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAnim * 4, 2);
		}
	}
	pDevice->SetTexture(0, NULL);

	// Zテスト戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//切り取りを戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}
//=============================================================================
// 弾の設定処理
//=============================================================================
void SetAnimation(D3DXVECTOR3 pos, D3DXCOLOR col , int nLengthX, int nLengthY, int nPtnWide, int nPtnHeight, int nPtnSpeed, int nType, int nView, ANIMATION anim)
{
	int nCntBullet;
	VERTEX_3D *pVtx;		//頂点情報へのポインタ

	//頂点BUFFERをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAnimation->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aAnimation[nCntBullet].bUse == false)
		{
			g_aAnimation[nCntBullet].pos = pos;
			g_aAnimation[nCntBullet].nCntAnim = -1;
			g_aAnimation[nCntBullet].nPtnAnim = -1;
			g_aAnimation[nCntBullet].nPtnHeight = nPtnHeight;
			g_aAnimation[nCntBullet].nPtnWide = nPtnWide;
			g_aAnimation[nCntBullet].nPtnSpeed = nPtnSpeed;
			g_aAnimation[nCntBullet].nLengthX = nLengthX;
			g_aAnimation[nCntBullet].nLengthY = nLengthY;
			g_aAnimation[nCntBullet].nType = nType;
			g_aAnimation[nCntBullet].nView = nView;
			g_aAnimation[nCntBullet].anim = anim;
			g_aAnimation[nCntBullet].fCntState = 0.0f;
			g_aAnimation[nCntBullet].bUse = true;

			if(anim == ANIMATION_FADE)
			{
				g_aAnimation[nCntBullet].fCntState = 2.0f / (nPtnHeight * nPtnWide);
			}

			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(-g_aAnimation[nCntBullet].nLengthX * 1.0f, g_aAnimation[nCntBullet].nLengthY * 1.0f, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].nLengthX * 1.0f, g_aAnimation[nCntBullet].nLengthY * 1.0f, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(-g_aAnimation[nCntBullet].nLengthX * 1.0f, -g_aAnimation[nCntBullet].nLengthY * 1.0f, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aAnimation[nCntBullet].nLengthX * 1.0f, -g_aAnimation[nCntBullet].nLengthY * 1.0f, 0.0f);

			pVtx[nCntBullet * 4].col = col;	// A = 透明度
			pVtx[nCntBullet * 4 + 1].col = col;
			pVtx[nCntBullet * 4 + 2].col = col;
			pVtx[nCntBullet * 4 + 3].col = col;
			break;
		}
	}
	//頂点BUFFERをアンロックする
	g_pVtxBuffAnimation->Unlock();
}
//=========================================================================================================//
// * 全Animation破棄処理
//=========================================================================================================//
void BreakAnimation(void)
{
	int nCntAnim;

	for (nCntAnim = 0; nCntAnim < MAX_EXPLOSION; nCntAnim++)
	{
		g_aAnimation[nCntAnim].bUse = false;
	}
}
