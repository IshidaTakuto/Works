//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// メッシュ(インデックスバッファ使用)の処理 [meshField.cpp]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "meshField.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexmeshField(LPDIRECT3DDEVICE9 pDevice);		// 頂点情報の生成
void MakeIndexmeshField(LPDIRECT3DDEVICE9 pDevice);			// インデックスの生成

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTexturemeshField[MESHFIELDTYPE_MAX] = {};	// テクスチャへのポインタ
meshField g_ameshField[MAX_MESHFIELD];								// メッシュの情報

// -----　テクスチャ情報　----- //
meshFieldTexture g_ameshFieldTexture[MESHFIELDTYPE_MAX] =
{
	{"data/TEXTURE/field000.jpg"}		// 000
};

// -----　フィールド配置情報　----- //
meshFieldInfo g_ameshFieldInfo[] =
{// 位置(pos),							向き(rot),					分割数	X,	Z
	{ D3DXVECTOR3(0.0f,-100.0f,0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	5,	10			,3000.0f, 6000.0f },
};

int g_NumMAXmeshFieldInfo;		// 配置情報の最大数

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 初期化処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void InitmeshField(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 配置情報の最大数
	g_NumMAXmeshFieldInfo = sizeof g_ameshFieldInfo / sizeof(meshFieldInfo);

	// テクスチャの読み込み
	for (int nTex = 0; nTex < MESHFIELDTYPE_MAX; nTex++)
	{
		D3DXCreateTextureFromFile(pDevice, g_ameshFieldTexture[nTex].pFilename, &g_apTexturemeshField[nTex]);
	}

	// 初期化
	for (int nCntmeshField = 0; nCntmeshField < MAX_MESHFIELD; nCntmeshField++)
	{
		g_ameshField[nCntmeshField].pVtxBuff = NULL;
		g_ameshField[nCntmeshField].pIdxBuff = NULL;
		g_ameshField[nCntmeshField].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_ameshField[nCntmeshField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ameshField[nCntmeshField].nNumSplitX = 0;
		g_ameshField[nCntmeshField].nNumSplitZ = 0;
		g_ameshField[nCntmeshField].fWidth = 0.0f;
		g_ameshField[nCntmeshField].fDepth = 0.0f;
		g_ameshField[nCntmeshField].nNumVertex = 0;
		g_ameshField[nCntmeshField].nNumIndex = 0;
		g_ameshField[nCntmeshField].nNumPolygon = 0;
	}

	// 配置情報の代入
	for (int nCntmeshField = 0; nCntmeshField < g_NumMAXmeshFieldInfo; nCntmeshField++)
	{
		g_ameshField[nCntmeshField].pos = g_ameshFieldInfo[nCntmeshField].pos;
		g_ameshField[nCntmeshField].rot = g_ameshFieldInfo[nCntmeshField].rot;
		g_ameshField[nCntmeshField].nNumSplitX = g_ameshFieldInfo[nCntmeshField].nNumSplitX;
		g_ameshField[nCntmeshField].nNumSplitZ = g_ameshFieldInfo[nCntmeshField].nNumSplitZ;
		g_ameshField[nCntmeshField].fWidth = g_ameshFieldInfo[nCntmeshField].fWidth;
		g_ameshField[nCntmeshField].fDepth = g_ameshFieldInfo[nCntmeshField].fDepth;
		// 頂点数 = (X分割数 + 1) * (Z分割数 + 1)
		g_ameshField[nCntmeshField].nNumVertex = (g_ameshField[nCntmeshField].nNumSplitX + 1) * (g_ameshField[nCntmeshField].nNumSplitZ + 1);
		// インデックス数 = X分割数 * Z分割数 * 2 + (Z分割数 - 1) * 4 + 2
		g_ameshField[nCntmeshField].nNumIndex = g_ameshField[nCntmeshField].nNumSplitX * g_ameshField[nCntmeshField].nNumSplitZ * 2 + (g_ameshField[nCntmeshField].nNumSplitZ - 1) * 4 + 2;
		// ポリゴン数 = インデックス数 - 2
		g_ameshField[nCntmeshField].nNumPolygon = g_ameshField[nCntmeshField].nNumIndex - 2;
	}

	// 頂点情報の作成
	MakeVertexmeshField(pDevice);

	// インデックスの生成
	MakeIndexmeshField(pDevice);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 終了処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UninitmeshField(void)
{
	for (int nCntmeshField = 0; nCntmeshField < MAX_MESHFIELD; nCntmeshField++)
	{
		// 頂点バッファの開放
		if (g_ameshField[nCntmeshField].pVtxBuff != NULL)
		{
			g_ameshField[nCntmeshField].pVtxBuff->Release();
			g_ameshField[nCntmeshField].pVtxBuff = NULL;
		}
		// インデックスの破棄
		if (g_ameshField[nCntmeshField].pIdxBuff != NULL)
		{
			g_ameshField[nCntmeshField].pIdxBuff->Release();
			g_ameshField[nCntmeshField].pIdxBuff = NULL;
		}
	}
	// テクスチャの破棄
	for (int nTex = 0; nTex < MESHFIELDTYPE_MAX; nTex++)
	{
		if (g_apTexturemeshField[nTex] != NULL)
		{
			g_apTexturemeshField[nTex]->Release();
			g_apTexturemeshField[nTex] = NULL;
		}
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 更新処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UpdatemeshField(void)
{
	//SetParticleEmitter(D3DXVECTOR3(-100.0f, 50.0f, -620.0f), 3);
	//SetParticleEmitter(D3DXVECTOR3(0.0f, 50.0f, -620.0f), 3);
	//SetParticleEmitter(D3DXVECTOR3(100.0f, 50.0f, -620.0f), 3);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 描画処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void DrawmeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	for (int nCntmeshField = 0; nCntmeshField < g_NumMAXmeshFieldInfo; nCntmeshField++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_ameshField[nCntmeshField].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_ameshField[nCntmeshField].rot.y, g_ameshField[nCntmeshField].rot.x, g_ameshField[nCntmeshField].rot.z);
		D3DXMatrixMultiply(&g_ameshField[nCntmeshField].mtxWorld,
			&g_ameshField[nCntmeshField].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			g_ameshField[nCntmeshField].pos.x, g_ameshField[nCntmeshField].pos.y, g_ameshField[nCntmeshField].pos.z);
		D3DXMatrixMultiply(&g_ameshField[nCntmeshField].mtxWorld,
			&g_ameshField[nCntmeshField].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_ameshField[nCntmeshField].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_ameshField[nCntmeshField].pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_ameshField[nCntmeshField].pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTexturemeshField[ MESHFIELDTYPE_000]);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_ameshField[nCntmeshField].nNumVertex, 0, g_ameshField[nCntmeshField].nNumPolygon);
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 頂点情報の作成
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void MakeVertexmeshField(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;		// 頂点情報へのポインタ

	for (int nCntmeshField = 0; nCntmeshField < g_NumMAXmeshFieldInfo; nCntmeshField++)
	{
		// 頂点バッファを生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_ameshField[nCntmeshField].nNumVertex,		// 確保するバッファのサイズ
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,				// 頂点フォーマット
			D3DPOOL_MANAGED,
			&g_ameshField[nCntmeshField].pVtxBuff,
			NULL);

		//--- 頂点情報の設定 ---//
		// 頂点バッファをロックし、頂点データへのポインタを取得
		g_ameshField[nCntmeshField].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		for (int nCnt1 = 0; nCnt1 < g_ameshField[nCntmeshField].nNumSplitZ + 1; nCnt1++)
		{// 奥行の軸
			for (int nCnt2 = 0; nCnt2 < g_ameshField[nCntmeshField].nNumSplitX + 1; nCnt2++)
			{// 幅の軸
				pVtx[nCnt1 * (g_ameshField[nCntmeshField].nNumSplitX + 1) + nCnt2].pos =
					D3DXVECTOR3(-g_ameshField[nCntmeshField].fWidth / 2 + g_ameshField[nCntmeshField].fWidth / g_ameshField[nCntmeshField].nNumSplitX * nCnt2,
						0.0f,
						g_ameshField[nCntmeshField].fDepth / 2 - g_ameshField[nCntmeshField].fDepth / g_ameshField[nCntmeshField].nNumSplitZ * nCnt1);
			}
		}
		// 法線
		for (int nNor = 0; nNor < g_ameshField[nCntmeshField].nNumVertex; nNor++)
		{
			pVtx[nNor].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
		// 頂点カラー
		for (int nCol = 0; nCol < g_ameshField[nCntmeshField].nNumVertex; nCol++)
		{
			pVtx[nCol].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		// テクスチャ座標
		for (int nCnt1 = 0; nCnt1 < g_ameshField[nCntmeshField].nNumSplitZ + 1; nCnt1++)
		{// 奥行の軸
			for (int nCnt2 = 0; nCnt2 < g_ameshField[nCntmeshField].nNumSplitX + 1; nCnt2++)
			{// 幅の軸
				//// 一枚
				//pVtx[nCnt1 * (g_ameshField[nCntmeshField].nNumSplitX + 1) + nCnt2].tex = D3DXVECTOR2(1.0f / (g_ameshField[nCntmeshField].nNumSplitX) * nCnt2, 1.0f / (g_ameshField[nCntmeshField].nNumSplitZ) * nCnt1);
				// 分割数に合わせる
				pVtx[nCnt1 * (g_ameshField[nCntmeshField].nNumSplitX + 1) + nCnt2].tex = D3DXVECTOR2(0.5f * nCnt2, 0.5f * nCnt1);
			}
		}

		// 頂点バッファをアンロックする
		g_ameshField[nCntmeshField].pVtxBuff->Unlock();
	}
}


//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// インデックスの作成
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void MakeIndexmeshField(LPDIRECT3DDEVICE9 pDevice)
{
	WORD *pIdx;		// インデックスデータへのポインタ

	for (int nCntmeshField = 0; nCntmeshField < g_NumMAXmeshFieldInfo; nCntmeshField++)
	{
		// インデックスバッファの生成
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_ameshField[nCntmeshField].nNumIndex,		// 確保するバッファのサイズ
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,				// インデックスフォーマット [16bit (2byte)]
			D3DPOOL_MANAGED,
			&g_ameshField[nCntmeshField].pIdxBuff,
			NULL);

		// インデックスバッファをロックし、インデックスデータへのポインタを取得
		g_ameshField[nCntmeshField].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		// インデックスの設定
		for (int nCnt1 = 0, nCnt2 = 0; nCnt2 < g_ameshField[nCntmeshField].nNumIndex; nCnt1++, nCnt2 += 2)
		{
			if ((nCnt2 + 2) % (g_ameshField[nCntmeshField].nNumSplitX * 2 + 4) == 0 && nCnt2 != 0)
			{// 縮退ポリゴン
				pIdx[nCnt2] = pIdx[nCnt2 - 1];											// 右上インデックス
				pIdx[nCnt2 + 1] = nCnt1 + g_ameshField[nCntmeshField].nNumSplitX + 1;	// 左下インデックス
				nCnt1--;
			}
			else
			{// 通常のポリゴン
				pIdx[nCnt2] = nCnt1 + g_ameshField[nCntmeshField].nNumSplitX + 1;
				pIdx[nCnt2 + 1] = nCnt1;
			}
		}
		// インデックスをアンロックする
		g_ameshField[nCntmeshField].pIdxBuff->Unlock();
	}
}
