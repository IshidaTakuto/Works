//=============================================================================
//
// ポリゴン処理 [Shadow.cpp]
// Author : 杉本涼
//
//=============================================================================
#include "game.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define		MAX_SHADOW		(128)
#define		MAX_TEXSHADOW	(3)
#define		TEXTURE_EFFECT	"data/TEXTURE/shadow000.jpg"
#define		TEXTURE_EFFECT1	"data/TEXTURE/shadow001.jpg"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureShadow[MAX_TEXSHADOW] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;			//頂点バッファへのポインタ
Shadow g_Shadow[MAX_SHADOW];

//=============================================================================
// 初期化処理
//=============================================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT, &g_pTextureShadow[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT1, &g_pTextureShadow[1]);

	// 頂点情報の作成
	MakeVertexShadow(pDevice);


}

//=============================================================================
// 終了処理
//=============================================================================
void UninitShadow(void)
{
	BreakShadow(-1);

	for (int nCntTex = 0; nCntTex < MAX_TEXSHADOW; nCntTex++)
	{// テクスチャの開放
		if (g_pTextureShadow[nCntTex] != NULL)
		{
			g_pTextureShadow[nCntTex]->Release();
			g_pTextureShadow[nCntTex] = NULL;
		}
	}

	// 頂点バッファの開放;
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateShadow(void)
{
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{//全影分回る
		if (g_Shadow[nCntShadow].bUse == true)
		{
			CollisionObjectS(&g_Shadow[nCntShadow].pos);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	int nCntShadow;

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == true)
		{
			pVtx[nCntShadow * 4].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fLengthX, 0.1f, g_Shadow[nCntShadow].fLengthZ);
			pVtx[nCntShadow * 4 + 1].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fLengthX, 0.1f, g_Shadow[nCntShadow].fLengthZ);
			pVtx[nCntShadow * 4 + 2].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fLengthX, 0.1f, -g_Shadow[nCntShadow].fLengthZ);
			pVtx[nCntShadow * 4 + 3].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fLengthX, 0.1f, -g_Shadow[nCntShadow].fLengthZ);

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Shadow[nCntShadow].mtxWorldShadow);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Shadow[nCntShadow].rot.y, g_Shadow[nCntShadow].rot.x, g_Shadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorldShadow, &g_Shadow[nCntShadow].mtxWorldShadow, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Shadow[nCntShadow].pos.x, g_Shadow[nCntShadow].pos.y, g_Shadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorldShadow, &g_Shadow[nCntShadow].mtxWorldShadow, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Shadow[nCntShadow].mtxWorldShadow);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow[g_Shadow[nCntShadow].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//頂点バッファのアンロック
	g_pVtxBuffShadow->Unlock();

	pDevice->SetTexture(0, NULL);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntShadow;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//法線の設定
		pVtx[nCntShadow * 4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nCntShadow * 4 + 1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nCntShadow * 4 + 2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[nCntShadow * 4 + 3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[nCntShadow * 4].tex = D3DXVECTOR2(0.05f, 0.05f);
		pVtx[nCntShadow * 4 + 1].tex = D3DXVECTOR2(0.95f, 0.05f);
		pVtx[nCntShadow * 4 + 2].tex = D3DXVECTOR2(0.05f, 0.95f);
		pVtx[nCntShadow * 4 + 3].tex = D3DXVECTOR2(0.95f, 0.95f);

		//頂点カラーの設定
		pVtx[nCntShadow * 4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  //色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
		pVtx[nCntShadow * 4 + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  //色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
		pVtx[nCntShadow * 4 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  //色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
		pVtx[nCntShadow * 4 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  //色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）

																		   //g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
																		   //g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Shadow[nCntShadow].bUse = false;

	}

	//頂点バッファのアンロック
	g_pVtxBuffShadow->Unlock();
}
//=============================================================================
// 影の作成
//=============================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLengthX, float fLengthZ, float fHeight, int nShadow)
{
	int nCntShadow;
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == false)
		{//影の情報を設定してその番号を返す

			g_Shadow[nCntShadow].bUse = true;
			g_Shadow[nCntShadow].fLengthZ = fLengthZ;
			g_Shadow[nCntShadow].nType = nShadow;
			g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (nShadow == 0)
			{
				g_Shadow[nCntShadow].pos = pos;
				g_Shadow[nCntShadow].rot.y = rot.y;
				g_Shadow[nCntShadow].fLengthX = fLengthX;
			}
			else
			{
				g_Shadow[nCntShadow].pos = pos + D3DXVECTOR3(fHeight * -0.3f, 0.0f, 0.0f);
				g_Shadow[nCntShadow].fLengthX = fLengthX + fHeight * 0.3f;
			}

			if (nShadow == 2) { g_Shadow[nCntShadow].fLengthX = 0; g_Shadow[nCntShadow].fLengthZ = 0; }
			SettingShadow(g_Shadow[nCntShadow].pos, g_Shadow[nCntShadow].rot, g_Shadow[nCntShadow].fLengthX, g_Shadow[nCntShadow].fLengthZ, nCntShadow);

			pVtx[nCntShadow * 4].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fLengthX, 0.1f, g_Shadow[nCntShadow].fLengthZ);
			pVtx[nCntShadow * 4 + 1].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fLengthX, 0.1f, g_Shadow[nCntShadow].fLengthZ);
			pVtx[nCntShadow * 4 + 2].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fLengthX, 0.1f, -g_Shadow[nCntShadow].fLengthZ);
			pVtx[nCntShadow * 4 + 3].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fLengthX, 0.1f, -g_Shadow[nCntShadow].fLengthZ);
			break;
		}
	}

	//頂点バッファのアンロック
	g_pVtxBuffShadow->Unlock();
	return nCntShadow;
}
//=============================================================================
// 影の反映
//=============================================================================
void SettingShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLengthX, float fLengthZ, int nNumber)
{
	g_Shadow[nNumber].pos = pos;
	g_Shadow[nNumber].rot.y = rot.y;
	g_Shadow[nNumber].fLengthX = fLengthX;
	g_Shadow[nNumber].fLengthZ = fLengthZ;
}
//=============================================================================
// 影の削除
//=============================================================================
void BreakShadow(int nNumber)
{
	if (nNumber > -1) { g_Shadow[nNumber].bUse = false; }
	else if(nNumber == -1)
	{
		for (int nCount = 0; nCount < MAX_SHADOW; nCount++)
		{
			g_Shadow[nCount].bUse = false;
		}
	}
	else
	{//プレイヤーのみ削除しない
		MODEL_INFO *pModel = GetModel_Info();
		for (int nCount = 0; nCount < MAX_SHADOW; nCount++)
		{
			if (pModel[0].nCntShadow != nCount)
			{
				g_Shadow[nCount].bUse = false;
			}
		}
	}

}