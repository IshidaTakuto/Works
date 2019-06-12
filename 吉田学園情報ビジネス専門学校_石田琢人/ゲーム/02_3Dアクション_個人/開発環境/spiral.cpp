//=============================================================================
//
// 螺旋の処理 [spiral.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "spiral.h"
#include "effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SPIRAL		"data/TEXTURE/shadow000.jpg"	// 読み込むテクスチャファイル名

#define SORT_TEXTURE		(1)
#define	MAX_SPIRAL			(256)							// エフェクトの最大数

#define ANGLE_RING			(1.0f / (SPIRAL_RING / 2))
#define ANGLE_POWERUP		(1.0f / (SPIRAL_POWERUP / 2))
#define ANGLE_MAGIC			(1.0f / (SPIRAL_MAGIC / 2))

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureSpiral = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSpiral = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSpiralEmitter = NULL;	// 頂点バッファへのポインタ
Spiral					g_aSpiral[MAX_SPIRAL];		// エフェクトの情報
int g_nCntMoveAngle;
float g_fSpiralHeight;

//=============================================================================
// 初期化処理
//=============================================================================
void InitSpiral(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntSpiral;

	for (nCntSpiral = 0; nCntSpiral < MAX_SPIRAL; nCntSpiral++)
	{
		g_aSpiral[nCntSpiral].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSpiral[nCntSpiral].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aSpiral[nCntSpiral].type = SPIRALTYPE_NONE;
		g_aSpiral[nCntSpiral].fRadius = 0.0f;
		g_aSpiral[nCntSpiral].bUse = false;
	}

	g_nCntMoveAngle = 0;
	g_aSpiral[nCntSpiral].fRadius = 20.0f;
	g_fSpiralHeight = 0.0f;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SPIRAL, &g_pTextureSpiral);

	// 頂点情報の作成
	MakeVertexSpiral(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSpiral(void)
{
		// テクスチャの開放
		if (g_pTextureSpiral != NULL)
		{
			g_pTextureSpiral->Release();
			g_pTextureSpiral = NULL;
		}

	// 頂点バッファの開放
	if (g_pVtxBuffSpiral != NULL)
	{
		g_pVtxBuffSpiral->Release();
		g_pVtxBuffSpiral = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSpiral(void)
{
	int nCntSpiral;
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSpiral->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSpiral = 0; nCntSpiral < MAX_SPIRAL; nCntSpiral++)
	{
		if (g_aSpiral[nCntSpiral].bUse == true)
		{
			switch (g_aSpiral[nCntSpiral].type)
			{
			case SPIRALTYPE_RING:
				g_aSpiral[nCntSpiral].move = D3DXVECTOR3(0.0f, sinf(D3DX_PI * (0.0f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * 2.0f, cosf(D3DX_PI * (0.0f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * 2.0f);
				g_aSpiral[nCntSpiral].nCntAngle = (g_aSpiral[nCntSpiral].nCntAngle + 1) % SPIRAL_RING;

				g_aSpiral[nCntSpiral].pos += g_aSpiral[nCntSpiral].move;

				SetEffect(g_aSpiral[nCntSpiral].pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.1f), 3.0f, 5, EFFECTSTATE_GATHER);
				break;

			case SPIRALTYPE_POWERUP:
				if (nCntSpiral % 2 == 0)
				{
					g_aSpiral[nCntSpiral].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].PlayerPos.x + sinf(D3DX_PI * (0.5f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * g_aSpiral[nCntSpiral].fRadius, 20.0f, g_aSpiral[nCntSpiral].PlayerPos.z + cosf(D3DX_PI * (0.5f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * g_aSpiral[nCntSpiral].fRadius);
				}
				else
				{
					g_aSpiral[nCntSpiral].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].PlayerPos.x + -sinf(D3DX_PI * (0.5f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * g_aSpiral[nCntSpiral].fRadius, 20.0f, g_aSpiral[nCntSpiral].PlayerPos.z + -cosf(D3DX_PI * (0.5f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * g_aSpiral[nCntSpiral].fRadius);
				}
				//g_aSpiral[nCntSpiral].move.y += 0.5f;
				g_aSpiral[nCntSpiral].nCntAngle = (g_aSpiral[nCntSpiral].nCntAngle + 1) % SPIRAL_RING;

				g_aSpiral[nCntSpiral].pos += g_aSpiral[nCntSpiral].move;

				//g_aSpiral[nCntSpiral].fRadius -= 0.25f;

				SetEffect(g_aSpiral[nCntSpiral].pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 2.0f, 10, EFFECTSTATE_GATHER);

				if (g_aSpiral[nCntSpiral].fRadius <= 0)
				{
					g_aSpiral[nCntSpiral].bUse = false;
				}
				break;

			case SPIRALTYPE_MAGIC:
				g_aSpiral[nCntSpiral].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].PlayerPos.x + sinf(D3DX_PI * (0.0f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * g_aSpiral[nCntSpiral].fRadius, g_aSpiral[nCntSpiral].fHeight, g_aSpiral[nCntSpiral].PlayerPos.z + cosf(D3DX_PI * (0.0f + (ANGLE_RING * g_aSpiral[nCntSpiral].nCntAngle))) * g_aSpiral[nCntSpiral].fRadius);
				g_aSpiral[nCntSpiral].nCntAngle = (g_aSpiral[nCntSpiral].nCntAngle + 1) % SPIRAL_RING;
				g_aSpiral[nCntSpiral].fRadius += 0.2f;
				g_aSpiral[nCntSpiral].fSize += 0.02f;
				g_aSpiral[nCntSpiral].fHeight += 0.2f;

				SetEffect(g_aSpiral[nCntSpiral].pos + D3DXVECTOR3(0.0f,2.0f,0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.1f), 2.0f, 20, EFFECTSTATE_GATHER);

				if (g_aSpiral[nCntSpiral].fHeight >= 30)
				{
					g_aSpiral[nCntSpiral].bUse = false;
				}

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);

				break;

			}
		}
		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffSpiral->Unlock();
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawSpiral(void)
{
	int nCntSpiral;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// 計算用マトリックス

												// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 被らないようにする
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	for (nCntSpiral = 0; nCntSpiral < MAX_SPIRAL; nCntSpiral++)
	{
		if (g_aSpiral[nCntSpiral].bUse == true)	//弾が使用されている
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSpiral[nCntSpiral].mtxWorld);

			// 回転を反映
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			g_aSpiral[nCntSpiral].mtxWorld._11 = mtxView._11;
			g_aSpiral[nCntSpiral].mtxWorld._12 = mtxView._21;
			g_aSpiral[nCntSpiral].mtxWorld._13 = mtxView._31;
			g_aSpiral[nCntSpiral].mtxWorld._21 = mtxView._12;
			g_aSpiral[nCntSpiral].mtxWorld._22 = mtxView._22;
			g_aSpiral[nCntSpiral].mtxWorld._23 = mtxView._32;
			g_aSpiral[nCntSpiral].mtxWorld._31 = mtxView._13;
			g_aSpiral[nCntSpiral].mtxWorld._32 = mtxView._23;
			g_aSpiral[nCntSpiral].mtxWorld._33 = mtxView._33;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aSpiral[nCntSpiral].pos.x, g_aSpiral[nCntSpiral].pos.y, g_aSpiral[nCntSpiral].pos.z);
			D3DXMatrixMultiply(&g_aSpiral[nCntSpiral].mtxWorld, &g_aSpiral[nCntSpiral].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSpiral[nCntSpiral].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSpiral, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureSpiral);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntSpiral, 2);
		}
	}

	// Zバッファを元に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// αテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexSpiral(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntSpiral;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SPIRAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSpiral,
		NULL);

	// 頂点バッファの情報を設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSpiral->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSpiral = 0; nCntSpiral < MAX_SPIRAL; nCntSpiral++)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-1, 1, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1, 1, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-1, -1, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1, -1, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
	// 頂点バッファをアンロックする
	g_pVtxBuffSpiral->Unlock();
}


//=============================================================================
// 設定
//=============================================================================
void SetSpiral(D3DXVECTOR3 pos, D3DXCOLOR col, SPIRALTYPE type)
{
	int nCntSpiral;

	// 頂点バッファの情報を設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSpiral->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSpiral = 0; nCntSpiral < MAX_SPIRAL; nCntSpiral++)
	{
		if (g_aSpiral[nCntSpiral].bUse == false)
		{
			switch (type)
			{
			case SPIRALTYPE_RING:
				g_aSpiral[nCntSpiral].pos = D3DXVECTOR3(pos.x, pos.y - sinf(D3DX_PI * (0.5f + (ANGLE_RING * g_nCntMoveAngle))) * 20.0f, pos.z - cosf(D3DX_PI * (0.5f + (ANGLE_RING * g_nCntMoveAngle))) * 20.0f);
				g_aSpiral[nCntSpiral].col = col;
				g_aSpiral[nCntSpiral].PlayerPos = pos;
				g_aSpiral[nCntSpiral].type = type;
				g_aSpiral[nCntSpiral].fSize = 2.0f;

				g_aSpiral[nCntSpiral].nCntAngle = (g_nCntMoveAngle + 1) % SPIRAL_RING;
				g_nCntMoveAngle = g_aSpiral[nCntSpiral].nCntAngle;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);

				break;

			case SPIRALTYPE_POWERUP:
				//g_aSpiral[nCntSpiral].pos = D3DXVECTOR3(pos.x - sinf(D3DX_PI * (0.5f + (ANGLE_RING * g_nCntMoveAngle))) * 20.0f, 0.0f, pos.z - cosf(D3DX_PI * (0.5f + (ANGLE_RING * g_nCntMoveAngle))) * 20.0f);
				g_aSpiral[nCntSpiral].pos = pos;
				g_aSpiral[nCntSpiral].move = D3DXVECTOR3(0.0f,0.0f,0.0f);
				g_aSpiral[nCntSpiral].col = col;
				g_aSpiral[nCntSpiral].PlayerPos = pos;
				g_aSpiral[nCntSpiral].type = type;
				g_aSpiral[nCntSpiral].fRadius = 20.0f;
				g_aSpiral[nCntSpiral].fSize = 1.5f;
				g_aSpiral[nCntSpiral].nCntAngle = (g_nCntMoveAngle + 1 * (SPIRAL_RING / SPIRAL_POWERUP)) % SPIRAL_RING;
				g_nCntMoveAngle = g_aSpiral[nCntSpiral].nCntAngle;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);

				break;

			case SPIRALTYPE_MAGIC:
				g_aSpiral[nCntSpiral].pos = pos;
				g_aSpiral[nCntSpiral].col = col;
				g_aSpiral[nCntSpiral].PlayerPos = pos;
				g_aSpiral[nCntSpiral].type = type;
				g_aSpiral[nCntSpiral].fRadius = 0.0f;
				g_aSpiral[nCntSpiral].fSize = 1.5f;
				g_aSpiral[nCntSpiral].fHeight = 0.0f;
				g_aSpiral[nCntSpiral].nCntAngle = (g_nCntMoveAngle + 1 * (SPIRAL_RING / SPIRAL_MAGIC)) % SPIRAL_RING;
				g_nCntMoveAngle = g_aSpiral[nCntSpiral].nCntAngle;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aSpiral[nCntSpiral].fSize, -g_aSpiral[nCntSpiral].fSize, 0.0f);

				break;

			}

			//頂点カラー
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aSpiral[nCntSpiral].bUse = true;
			break;
		}
		pVtx += 4;	//頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffSpiral->Unlock();
}
