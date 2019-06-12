//=============================================================================
//
// 魔法の処理 [magic.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "magic.h"
#include "effect.h"
#include "particle.h"
#include "spiral.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAGIC		"data/TEXTURE/magic.jpg"	// 読み込むテクスチャファイル名
#define MAX_MAGIC		(64)
#define ANGLE_MAGIC		(1.0f / (MAGIC_PARTICLE / 2))
#define MAGIC_PARTICLE	(10)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexMagic(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMagic = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMagic = NULL;		// 頂点バッファへのポインタ
MAGIC g_aMagic[MAX_MAGIC];							// 壁の情報
int g_nAngleMagic;

//=============================================================================
// 初期化処理
//=============================================================================
void InitMagic(void)
{
	int nCntMagic;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MAGIC, &g_pTextureMagic);

	// 頂点情報の作成
	MakeVertexMagic(pDevice);

	// 壁の情報の初期化
	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		g_aMagic[nCntMagic].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
		g_aMagic[nCntMagic].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きの初期化
		g_aMagic[nCntMagic].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aMagic[nCntMagic].bUse = false;
	}

	g_nAngleMagic = 0;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMagic(void)
{
	//テクスチャの破棄
	if (g_pTextureMagic != NULL)
	{
		g_pTextureMagic->Release();
		g_pTextureMagic = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffMagic != NULL)
	{
		g_pVtxBuffMagic->Release();
		g_pVtxBuffMagic = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMagic(void)
{
	for (int nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		if (g_aMagic[nCntMagic].bUse == true)
		{
			switch (g_aMagic[nCntMagic].type)
			{
			case MAGICTYPE_FIRE:
				g_aMagic[nCntMagic].rot.y += 0.02f;
				g_aMagic[nCntMagic].nLife--;

				
				SetParticle(D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + sinf(D3DX_PI * (0.0f + (ANGLE_MAGIC * g_nAngleMagic))) * g_aMagic[nCntMagic].fRadius,
							0.0f,
							g_aMagic[nCntMagic].pos.z + cosf(D3DX_PI * (0.0f + (ANGLE_MAGIC * g_nAngleMagic))) * g_aMagic[nCntMagic].fRadius), 
							g_aMagic[nCntMagic].col, 
							2.0f, 
							100, 
							PARTICLETYPE_NORMAL);

				//SetSpiral(g_aMagic[nCntMagic].pos, g_aMagic[nCntMagic].col, SPIRALTYPE_MAGIC);

				g_nAngleMagic = (g_nAngleMagic + 1) % MAGIC_PARTICLE;

				if (g_aMagic[nCntMagic].nLife <= 0)
				{
					g_aMagic[nCntMagic].bUse = false;
				}

				break;

			case MAGICTYPE_WATER:

				break;
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMagic(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans;				// 計算用マトリックス
	int nCntMagic;

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMagic[nCntMagic].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMagic[nCntMagic].rot.y, g_aMagic[nCntMagic].rot.x, g_aMagic[nCntMagic].rot.z);
		D3DXMatrixMultiply(&g_aMagic[nCntMagic].mtxWorld, &g_aMagic[nCntMagic].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aMagic[nCntMagic].pos.x, g_aMagic[nCntMagic].pos.y, g_aMagic[nCntMagic].pos.z);
		D3DXMatrixMultiply(&g_aMagic[nCntMagic].mtxWorld, &g_aMagic[nCntMagic].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aMagic[nCntMagic].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffMagic, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMagic);

		if (g_aMagic[nCntMagic].bUse == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntMagic, 2);
		}
	}

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
// 頂点情報の作成
//=============================================================================
void MakeVertexMagic(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntMagic;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_MAGIC,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMagic,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		// 頂点の座標
		pVtx[0].pos = D3DXVECTOR3(-1, 0.0f, 1);
		pVtx[1].pos = D3DXVECTOR3(1, 0.0f, 1);
		pVtx[2].pos = D3DXVECTOR3(-1, 0.0f, 1);
		pVtx[3].pos = D3DXVECTOR3(1, 0.0f, 1);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	// 頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロック
	g_pVtxBuffMagic->Unlock();
}

//=============================================================================
// 設定処理
//=============================================================================
void SetMagic(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, int nCntNumber, MAGICTYPE type)
{
	int nCntMagic, nCntLoop;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntLoop = 0; nCntLoop < nCntNumber; nCntLoop++)
	{
		for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
		{
			if (g_aMagic[nCntMagic].bUse == false)
			{
				g_aMagic[nCntMagic].pos = pos + D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				g_aMagic[nCntMagic].col = col;
				g_aMagic[nCntMagic].fRadius = fRadius;
				g_aMagic[nCntMagic].nLife = nLife;
				g_aMagic[nCntMagic].type = type;

				// 頂点の座標
				pVtx[0].pos = D3DXVECTOR3(-fRadius, 0.0f, fRadius);
				pVtx[1].pos = D3DXVECTOR3(fRadius, 0.0f, fRadius);
				pVtx[2].pos = D3DXVECTOR3(-fRadius, 0.0f, -fRadius);
				pVtx[3].pos = D3DXVECTOR3(fRadius, 0.0f, -fRadius);

				// 頂点カラー
				pVtx[0].col = col;
				pVtx[1].col = col;
				pVtx[2].col = col;
				pVtx[3].col = col;

				for (int nCntGather = 0; nCntGather < SPIRAL_MAGIC; nCntGather++)
				{
					SetSpiral(g_aMagic[nCntMagic].pos, g_aMagic[nCntMagic].col, SPIRALTYPE_MAGIC);
				}

				g_aMagic[nCntMagic].bUse = true;
				break;
			}
			pVtx += 4;	// 頂点データのポインタを4つ進める
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffMagic->Unlock();
}
