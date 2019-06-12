//=============================================================================
//
// ポリゴン処理 [Polygon.cpp]
// Author : 杉本涼
//
//=============================================================================
#include "game.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define		MAX_SHADOW		(128)
#define		MAX_TEXSHADOW	(2)
#define		TEXTURE_EFFECT	""
#define		TEXTURE_EFFECT1	"data/TEXTURE/名称未設定 1 のコピー.jpg"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePolygon[MAX_TEXSHADOW] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;			//頂点バッファへのポインタ
ZPolygon g_Polygon[MAX_SHADOW];

//=============================================================================
// 初期化処理
//=============================================================================
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT, &g_pTexturePolygon[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT1, &g_pTexturePolygon[1]);

	// 頂点情報の作成
	MakeVertexPolygon(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPolygon(void)
{
	BreakPolygon(-1);

	for (int nCntTex = 0; nCntTex < MAX_TEXSHADOW; nCntTex++)
	{// テクスチャの開放
		if (g_pTexturePolygon[nCntTex] != NULL)
		{
			g_pTexturePolygon[nCntTex]->Release();
			g_pTexturePolygon[nCntTex] = NULL;
		}
	}

	// 頂点バッファの開放;
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePolygon(void)
{
	//for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	//{//全影分回る
	//	if (g_Polygon[nCntShadow].bUse == true)
	//	{
	//		CollisionObjectS(&g_Polygon[nCntShadow].pos);
	//	}
	//}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	int nCntShadow;
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	// Zテスト解除
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, 2);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Polygon[nCntShadow].bUse == true)
		{
			//頂点カラーの設定
			pVtx[nCntShadow * 4].col = g_Polygon[nCntShadow].col;  //色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
			pVtx[nCntShadow * 4 + 1].col = g_Polygon[nCntShadow].col;  //色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
			pVtx[nCntShadow * 4 + 2].col = g_Polygon[nCntShadow].col;  //色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）
			pVtx[nCntShadow * 4 + 3].col = g_Polygon[nCntShadow].col;  //色の指定（0.0f ～ 1.0f）&透明度（0.0f　～　1.0f）

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Polygon[nCntShadow].mtxWorldShadow);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Polygon[nCntShadow].rot.y, g_Polygon[nCntShadow].rot.x, g_Polygon[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_Polygon[nCntShadow].mtxWorldShadow, &g_Polygon[nCntShadow].mtxWorldShadow, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Polygon[nCntShadow].pos.x, g_Polygon[nCntShadow].pos.y, g_Polygon[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_Polygon[nCntShadow].mtxWorldShadow, &g_Polygon[nCntShadow].mtxWorldShadow, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Polygon[nCntShadow].mtxWorldShadow);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePolygon[g_Polygon[nCntShadow].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}

	//頂点バッファのアンロック
	g_pVtxBuffPolygon->Unlock();
	pDevice->SetTexture(0, NULL);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Zテスト戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	//頂点バッファのアンロック
	g_pVtxBuffPolygon->Unlock();

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntShadow;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

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
		g_Polygon[nCntShadow].bUse = false;

	}

	//頂点バッファのアンロック
	g_pVtxBuffPolygon->Unlock();
}
//=============================================================================
// 影の作成
//=============================================================================
int SetPolygon(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fLengthX, float fLengthZ, int nType)
{
	int nCntShadow;
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Polygon[nCntShadow].bUse == false)
		{//影の情報を設定してその番号を返す

			g_Polygon[nCntShadow].bUse = true;
			g_Polygon[nCntShadow].nType = nType;
			g_Polygon[nCntShadow].col = col;

			g_Polygon[nCntShadow].pos = pos;
			g_Polygon[nCntShadow].rot = rot;
			g_Polygon[nCntShadow].fLengthX = fLengthX;
			g_Polygon[nCntShadow].fLengthZ = fLengthZ;

			pVtx[nCntShadow * 4].pos = D3DXVECTOR3(-g_Polygon[nCntShadow].fLengthX, 0.1f, g_Polygon[nCntShadow].fLengthZ);
			pVtx[nCntShadow * 4 + 1].pos = D3DXVECTOR3(g_Polygon[nCntShadow].fLengthX, 0.1f, g_Polygon[nCntShadow].fLengthZ);
			pVtx[nCntShadow * 4 + 2].pos = D3DXVECTOR3(-g_Polygon[nCntShadow].fLengthX, 0.1f, -g_Polygon[nCntShadow].fLengthZ);
			pVtx[nCntShadow * 4 + 3].pos = D3DXVECTOR3(g_Polygon[nCntShadow].fLengthX, 0.1f, -g_Polygon[nCntShadow].fLengthZ);
			break;
		}
	}

	//頂点バッファのアンロック
	g_pVtxBuffPolygon->Unlock();
	return nCntShadow;
}
//=============================================================================
// 影の反映
//=============================================================================
void SettingPolygon(D3DXVECTOR3 pos)
{
	PLAYER *pPlayer = GetPlayer();

	//目標までの距離計算
	D3DXVECTOR3 distance = pPlayer[0].target - pos;
	float		fDistance = powf(distance.x, 2) + powf(distance.z, 2);

	if (fDistance > 2000.0f * 2000.0f && pPlayer[0].bTarget == true)
	{//目標が遠いい
		g_Polygon[pPlayer[0].nCntTarget].col.a = 1.0f;
		g_Polygon[pPlayer[0].nCntTarget].rot.y = atan2f(distance.x, distance.z);
		g_Polygon[pPlayer[0].nCntTarget].pos = pos + D3DXVECTOR3(sinf(g_Polygon[pPlayer[0].nCntTarget].rot.y), 0.0f, cosf(g_Polygon[pPlayer[0].nCntTarget].rot.y)) * 1000.0f;
	}
	else if(pPlayer[0].bTarget == true)
	{//だんだん消える
		if (g_Polygon[pPlayer[0].nCntTarget].col.a != 0.0f)
		{
			g_Polygon[pPlayer[0].nCntTarget].col.a -= 0.05f;
		}
	}
	else { g_Polygon[pPlayer[0].nCntTarget].col.a = 0.0f; }
}
//=============================================================================
// 影の削除
//=============================================================================
void BreakPolygon(int nNumber)
{
	if (nNumber > -1) { g_Polygon[nNumber].bUse = false; }
	else if (nNumber == -1)
	{
		for (int nCount = 0; nCount < MAX_SHADOW; nCount++)
		{
			g_Polygon[nCount].bUse = false;
		}
	}
	else
	{//プレイヤーのみ削除しない
		PLAYER *pPlayer = GetPlayer();

		for (int nCount = 0; nCount < MAX_SHADOW; nCount++)
		{
			if (pPlayer[0].nCntTarget != nCount)
			{
				g_Polygon[nCount].bUse = false;
			}
		}
	}

}