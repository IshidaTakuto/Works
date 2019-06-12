//=============================================================================
//
// 血の処理 [Blood.cpp]
// Author : KOJI INAI
//
//=============================================================================
#include "Blood.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BLOOD_MAX			(64)							// 影の最大数
#define	BLOOD_TEX			"data/TEXTURE/Blood.png"		// モデルの名前

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexBlood(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// 影の構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 rot;				// 向
	//float fHeight;					// 高さ
	//float fWide;					// 幅
	D3DXMATRIX mtxWorld;			// ワールドマトリックス
	bool bUse;						// 使用しているかどうか
}Blood;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBlood = NULL;					//テクスチャへポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlood = NULL;			//頂点バッファへのポインタ
Blood g_aBlood[BLOOD_MAX];								// 影の構造体へのポインタ
float g_fHeightBlood;

//=============================================================================
// 初期化処理
//=============================================================================
void InitBlood(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	//g_nWBlood = 0.000001f;
	g_fHeightBlood = 0.0f;
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BLOOD_TEX, &g_pTextureBlood);

	// 頂点情報の作成
	MakeVertexBlood(pDevice);

	for (int nCntBlood = 0; nCntBlood < BLOOD_MAX; nCntBlood++)
	{
		g_aBlood[nCntBlood].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlood[nCntBlood].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlood[nCntBlood].bUse = false;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBlood(void)
{
	// テクスチャの開放;
	if (g_pTextureBlood != NULL)
	{
		g_pTextureBlood->Release();
		g_pTextureBlood = NULL;
	}

	// 頂点バッファの開放;
	if (g_pVtxBuffBlood != NULL)
	{
		g_pVtxBuffBlood->Release();
		g_pVtxBuffBlood = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBlood(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBlood(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCntBlood = 0; nCntBlood < BLOOD_MAX; nCntBlood++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aBlood[nCntBlood].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBlood[nCntBlood].rot.y, g_aBlood[nCntBlood].rot.x, g_aBlood[nCntBlood].rot.z);
		D3DXMatrixMultiply(&g_aBlood[nCntBlood].mtxWorld, &g_aBlood[nCntBlood].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aBlood[nCntBlood].pos.x, g_aBlood[nCntBlood].pos.y, g_aBlood[nCntBlood].pos.z);
		D3DXMatrixMultiply(&g_aBlood[nCntBlood].mtxWorld, &g_aBlood[nCntBlood].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aBlood[nCntBlood].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffBlood, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBlood);

		if (g_aBlood[nCntBlood].bUse == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlood * 4, 2);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexBlood(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * BLOOD_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlood,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得6
	g_pVtxBuffBlood->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlood = 0; nCntBlood < BLOOD_MAX; nCntBlood++)
	{
		//頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(-40.0f,0.0f, 40.0f);		//頂点座標(X座標,Y座標)
		pVtx[1].pos = D3DXVECTOR3(40.0f, 0.0f, 40.0f);		//頂点座標(X座標,Y座標)
		pVtx[2].pos = D3DXVECTOR3(-40.0f,0.0f, -40.0f);		//頂点座標(X座標,Y座標)
		pVtx[3].pos = D3DXVECTOR3(40.0f, 0.0f, -40.0f);		//頂点座標(X座標,Y座標)

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.1f, 0.0f, 1.0f);  //色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
		pVtx[1].col = D3DXCOLOR(0.0f, 0.1f, 0.0f, 1.0f);  //色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f)
		pVtx[2].col = D3DXCOLOR(0.0f, 0.1f, 0.0f, 1.0f);  //色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
		pVtx[3].col = D3DXCOLOR(0.0f, 0.1f, 0.0f, 1.0f);  //色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		//Uが幅、Vが高さ　(0.0f～1.0f）
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);		//Uが幅、Vが高さ　(0.0f～1.0f）
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		//Uが幅、Vが高さ　(0.0f～1.0f）
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);		//Uが幅、Vが高さ　(0.0f～1.0f）

		pVtx += 4;

	}
	//頂点バッファのアンロック
	g_pVtxBuffBlood->Unlock();
}

//=============================================================================
// 血痕の設定
//=============================================================================
int SetBlood(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWide, D3DXCOLOR col)
{
	int nCntBlood;

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得6
	g_pVtxBuffBlood->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlood = 0; nCntBlood < BLOOD_MAX; nCntBlood++)
	{
		if (g_aBlood[nCntBlood].bUse == false)
		{
			g_fHeightBlood += 0.0001f;

			g_aBlood[nCntBlood].pos = pos;
			g_aBlood[nCntBlood].rot = rot;
			g_aBlood[nCntBlood].bUse = true;

			//頂点情報の設定
			//pVtx[0].pos = D3DXVECTOR3(-fWide, g_fHeightBlood, fHeight);		//頂点座標(X座標,Y座標)
			//pVtx[1].pos = D3DXVECTOR3(fWide, g_fHeightBlood, fHeight);		//頂点座標(X座標,Y座標)
			//pVtx[2].pos = D3DXVECTOR3(-fWide, g_fHeightBlood, -fHeight);		//頂点座標(X座標,Y座標)
			//pVtx[3].pos = D3DXVECTOR3(fWide, g_fHeightBlood, -fHeight);		//頂点座標(X座標,Y座標)

			//頂点カラーの設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			break;
		}
		pVtx += 4;

	}
	//頂点バッファのアンロック
	g_pVtxBuffBlood->Unlock();

	//for (nCntBlood = 0; nCntBlood < BLOOD_MAX; nCntBlood++)
	//{
	//	if (g_aBlood[nCntBlood].bUse == true)
	//	{
	//		float nPosBlood = 0.000001f;
	//		g_aBlood[nCntBlood].pos.y += nPosBlood;
	//	}
	//}
	return nCntBlood;
}

////=============================================================================
//// 影の設定
////=============================================================================
//void SetPositionBlood(int nIdyShadow, D3DXVECTOR3 pos)
//{
//	g_aBlood[nIdyShadow].pos = pos;
//}
//=============================================================================
// モデルの取得
//=============================================================================
Blood *GetBlood(void)
{
	return &g_aBlood[0];
}
//
//
//
void DeleteBlood(void)
{
	for (int nCntBlood = 0; nCntBlood < BLOOD_MAX; nCntBlood++)
	{
		g_aBlood[nCntBlood].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlood[nCntBlood].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlood[nCntBlood].bUse = false;
	}
}
