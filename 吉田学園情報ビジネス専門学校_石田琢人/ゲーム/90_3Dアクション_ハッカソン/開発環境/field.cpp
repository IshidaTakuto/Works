//=============================================================================
//
// 水面処理 [field.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "field.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MESHFIELD_01		"data/TEXTURE/river000.png"	// 読み込むテクスチャファイル名
#define TEXTURE_MESHFIELD_00		"data/TEXTURE/river001.png"	// 読み込むテクスチャファイル名

#define FIELD_WIDTH		(1000.0f)		// 幅
#define FIELD_DEPTH		(4500.0f)		// 奥行
#define MAX_TEXTURE		(2)				// テクスチャ数
#define MAX_FIELD		(2)				// フィールド数（地面を増やしたいときに値を変えてください）

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureField[MAX_TEXTURE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;	// 頂点バッファへのポインタ
Field g_Field[MAX_FIELD];

//=============================================================================
// 初期化処理
//=============================================================================
void InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_00, &g_pTextureField[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHFIELD_01, &g_pTextureField[1]);

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		g_Field[nCntField].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
		g_Field[nCntField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 向きの初期化
		g_Field[nCntField].nType = nCntField;										// フィールドのテクスチャ指定
	}

	// 頂点情報の作成
	MakeVertexField(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitField(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureField[nCntTex] != NULL)
		{
			g_pTextureField[nCntTex]->Release();
			g_pTextureField[nCntTex] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffField != NULL)
	{
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateField(void)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex += D3DXVECTOR2(0.000f, -0.0004f);
	pVtx[1].tex += D3DXVECTOR2(0.000f, -0.0004f);
	pVtx[2].tex += D3DXVECTOR2(0.000f, -0.0004f);
	pVtx[3].tex += D3DXVECTOR2(0.000f, -0.0004f);

	pVtx[4].tex += D3DXVECTOR2(0.0003f, 0.0003f);
	pVtx[5].tex += D3DXVECTOR2(0.0003f, 0.0003f);
	pVtx[6].tex += D3DXVECTOR2(0.0003f, 0.0003f);
	pVtx[7].tex += D3DXVECTOR2(0.0003f, 0.0003f);

	// 頂点バッファをアンロック
	g_pVtxBuffField->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans;				// 計算用マトリックス

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Field[nCntField].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Field[nCntField].rot.y, g_Field[nCntField].rot.x, g_Field[nCntField].rot.z);
		D3DXMatrixMultiply(&g_Field[nCntField].mtxWorld, &g_Field[nCntField].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Field[nCntField].pos.x, g_Field[nCntField].pos.y, g_Field[nCntField].pos.z);
		D3DXMatrixMultiply(&g_Field[nCntField].mtxWorld, &g_Field[nCntField].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Field[nCntField].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureField[g_Field[nCntField].nType]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntField, 2);
	}

	// αテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_FIELD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffField,
		NULL);

	// 頂点バッファの情報を設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-FIELD_WIDTH, 0.0f + (nCntField * 0.001f), FIELD_DEPTH);
		pVtx[1].pos = D3DXVECTOR3(FIELD_WIDTH, 0.0f + (nCntField * 0.001f), FIELD_DEPTH);
		pVtx[2].pos = D3DXVECTOR3(-FIELD_WIDTH, 0.0f + (nCntField * 0.001f), -FIELD_DEPTH);
		pVtx[3].pos = D3DXVECTOR3(FIELD_WIDTH, 0.0f + (nCntField * 0.001f), -FIELD_DEPTH);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ進める

	}
	// 頂点バッファをアンロックする
	g_pVtxBuffField->Unlock();
}

//=============================================================================
// 当たり判定の処理
//=============================================================================
//bool CollisionField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
//{
//	bool bLand = false;		// 地面に接着しているかどうか	[true : 接着している	false : 浮いている]
//
//	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
//	{
//		if (pPos->y < g_Field[nCntField].pos.y && pPosOld->y >= g_Field[nCntField].pos.y)
//		{// 上から当たった時
//			bLand = true;							// 地面に接着状態にする
//			pPos->y = g_Field[nCntField].pos.y;	// 埋まっているのを地面の上に戻す
//			pMove->y = 0.0f;						// 落下速度を0にする
//		}
//	}
//
//	return bLand;	// 接着状態を返す
//}