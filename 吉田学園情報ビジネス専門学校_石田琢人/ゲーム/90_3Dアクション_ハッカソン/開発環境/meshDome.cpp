//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// メッシュドーム処理 [meshDome.cpp]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "meshDome.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexmeshDome(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexmeshDome(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTexturemeshDome[MESHDOMETYPE_MAX] = {};	// テクスチャへのポインタ
meshDome g_ameshDome[MAX_MESHDOME];								// メッシュドームの情報

// -----　テクスチャ情報　----- //
meshDomeTexture g_apTexturemeshDomeInfo[MESHDOMETYPE_MAX] =
{
	{ "data/TEXTURE/sky001.jpg" },
	{ "data/TEXTURE/wall004.jpg" }
};

// -----　配置情報　----- //
meshDomeInfo g_ameshDomeInfo[] =
{// 位置(pos),							向き(rot),						半径,		V,		H,		テクスチャの種類,	テクスチャ移動速度,				描画する面
	{ D3DXVECTOR3(0.0f,0.0f,0.0f),		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	5000.0f,	100,	100,	MESHDOMETYPE_000,	D3DXVECTOR2(0.00005f,0.0f),	DRAWSIDE_IN }
};

int g_NumMAXmeshDomeInfo;		// 配置情報の最大数

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 初期化処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void InitmeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 配置情報の最大数
	g_NumMAXmeshDomeInfo = sizeof g_ameshDomeInfo / sizeof(meshDomeInfo);

	// テクスチャの読み込み
	for (int nCntmeshDomeTex = 0; nCntmeshDomeTex < MESHDOMETYPE_MAX; nCntmeshDomeTex++)
	{
		D3DXCreateTextureFromFile(pDevice, g_apTexturemeshDomeInfo[nCntmeshDomeTex].pFilename, &g_apTexturemeshDome[nCntmeshDomeTex]);
	}

	// 初期化
	for (int nCntmeshDome = 0; nCntmeshDome < MAX_MESHDOME; nCntmeshDome++)
	{
		g_ameshDome[nCntmeshDome].pVtxBuff = NULL;								// 頂点バッファへのポインタ
		g_ameshDome[nCntmeshDome].pIdxBuff = NULL;								// インデックスバッファのポインタ
		g_ameshDome[nCntmeshDome].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
		g_ameshDome[nCntmeshDome].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
		g_ameshDome[nCntmeshDome].fRadius = 0.0f;								// 半径
		g_ameshDome[nCntmeshDome].nNumSplitV = 0;								// 縦方向のブロック数
		g_ameshDome[nCntmeshDome].nNumSplitH = 0;								// 横方向のブロック数
		g_ameshDome[nCntmeshDome].nNumVertex = 0;								// 頂点数
		g_ameshDome[nCntmeshDome].nNumIndex = 0;								// インデックス数
		g_ameshDome[nCntmeshDome].nNumPolygon = 0;								// ポリゴン数
		g_ameshDome[nCntmeshDome].TexType = MESHDOMETYPE_000;					// テクスチャの種類
		g_ameshDome[nCntmeshDome].TexMoveSpeed = D3DXVECTOR2(0.0f, 0.0f);		// テクスチャ移動速度
		g_ameshDome[nCntmeshDome].DrawSide = DRAWSIDE_BOTH;						// 描画する面
	}

	// 配置情報
	for (int nCntmeshDome = 0; nCntmeshDome < g_NumMAXmeshDomeInfo; nCntmeshDome++)
	{
		g_ameshDome[nCntmeshDome].pos = g_ameshDomeInfo[nCntmeshDome].pos;
		g_ameshDome[nCntmeshDome].rot = g_ameshDomeInfo[nCntmeshDome].rot;
		g_ameshDome[nCntmeshDome].fRadius = g_ameshDomeInfo[nCntmeshDome].fRadius;
		g_ameshDome[nCntmeshDome].nNumSplitV = g_ameshDomeInfo[nCntmeshDome].nNumSplitV;
		g_ameshDome[nCntmeshDome].nNumSplitH = g_ameshDomeInfo[nCntmeshDome].nNumSplitH;
		g_ameshDome[nCntmeshDome].TexType = g_ameshDomeInfo[nCntmeshDome].TexType;
		g_ameshDome[nCntmeshDome].TexMoveSpeed = g_ameshDomeInfo[nCntmeshDome].TexMoveSpeed;
		g_ameshDome[nCntmeshDome].DrawSide = g_ameshDomeInfo[nCntmeshDome].DrawSide;

		// 頂点数 = (横分割数 + 1) * (縦分割数 + 1)
		g_ameshDome[nCntmeshDome].nNumVertex = (g_ameshDome[nCntmeshDome].nNumSplitH + 1) * (g_ameshDome[nCntmeshDome].nNumSplitV + 1);
		// インデックス数 = 横分割数 * 縦分割数 * 2 + (縦分割数 - 1) * 4 + 2
		g_ameshDome[nCntmeshDome].nNumIndex = g_ameshDome[nCntmeshDome].nNumSplitH * g_ameshDome[nCntmeshDome].nNumSplitV * 2 + (g_ameshDome[nCntmeshDome].nNumSplitV - 1) * 4 + 2;
		// ポリゴン数 = インデックス数 - 2
		g_ameshDome[nCntmeshDome].nNumPolygon = g_ameshDome[nCntmeshDome].nNumIndex - 2;
	}

	// 頂点情報の作成
	MakeVertexmeshDome(pDevice);

	// インデックス情報の作成
	MakeIndexmeshDome(pDevice);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 終了処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UninitmeshDome(void)
{
	// テクスチャの開放
	for (int nCntTex = 0; nCntTex < MESHDOMETYPE_MAX; nCntTex++)
	{
		if (g_apTexturemeshDome[nCntTex] != NULL)
		{
			g_apTexturemeshDome[nCntTex]->Release();
			g_apTexturemeshDome[nCntTex] = NULL;
		}
	}

	for (int nCntmeshDome = 0; nCntmeshDome < MAX_MESHDOME; nCntmeshDome++)
	{
		// 頂点バッファの開放
		if (g_ameshDome[nCntmeshDome].pVtxBuff != NULL)
		{
			g_ameshDome[nCntmeshDome].pVtxBuff->Release();
			g_ameshDome[nCntmeshDome].pVtxBuff = NULL;
		}

		// インデックスバッファの開放
		if (g_ameshDome[nCntmeshDome].pIdxBuff != NULL)
		{
			g_ameshDome[nCntmeshDome].pIdxBuff->Release();
			g_ameshDome[nCntmeshDome].pIdxBuff = NULL;
		}
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 更新処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UpdatemeshDome(void)
{
	for (int nCntmeshDome = 0; nCntmeshDome < g_NumMAXmeshDomeInfo; nCntmeshDome++)
	{
		VERTEX_3D *pVtx;		// 頂点情報へのポインタ

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_ameshDome[nCntmeshDome].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点情報の設定
		for (int nCntPos = 0; nCntPos < g_ameshDome[nCntmeshDome].nNumVertex; nCntPos++,pVtx++)
		{
			// テクスチャ座標
			pVtx[0].tex += g_ameshDome[nCntmeshDome].TexMoveSpeed;
		}
		// 頂点バッファをアンロックする
		g_ameshDome[nCntmeshDome].pVtxBuff->Unlock();
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 描画処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void DrawmeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot,mtxTrans;					// 計算用マトリックス距離

	// ライトオフ
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntmeshDome = 0; nCntmeshDome < g_NumMAXmeshDomeInfo; nCntmeshDome++)
	{
		// 描画する面を設定
		switch (g_ameshDome[nCntmeshDome].DrawSide)
		{
		case DRAWSIDE_BOTH:
			// カリングオフ
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			break;
		case DRAWSIDE_IN:
			// 表面カリング
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			break;
		case DRAWSIDE_OUT:
			// 裏面カリング
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			break;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_ameshDome[nCntmeshDome].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ameshDome[nCntmeshDome].rot.y, g_ameshDome[nCntmeshDome].rot.x, g_ameshDome[nCntmeshDome].rot.z);
		D3DXMatrixMultiply(&g_ameshDome[nCntmeshDome].mtxWorld, &g_ameshDome[nCntmeshDome].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_ameshDome[nCntmeshDome].pos.x, g_ameshDome[nCntmeshDome].pos.y, g_ameshDome[nCntmeshDome].pos.z);
		D3DXMatrixMultiply(&g_ameshDome[nCntmeshDome].mtxWorld, &g_ameshDome[nCntmeshDome].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_ameshDome[nCntmeshDome].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_ameshDome[nCntmeshDome].pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_ameshDome[nCntmeshDome].pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTexturemeshDome[g_ameshDome[nCntmeshDome].TexType]);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_ameshDome[nCntmeshDome].nNumVertex, 0, g_ameshDome[nCntmeshDome].nNumPolygon);
	}

	// ライトオン
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// カリング設定を戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 頂点情報の作成
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void MakeVertexmeshDome(LPDIRECT3DDEVICE9 pDevice)
{
	for (int nCntmeshDome = 0; nCntmeshDome < g_NumMAXmeshDomeInfo; nCntmeshDome++)
	{
		// 頂点バッファを生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_ameshDome[nCntmeshDome].nNumVertex,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_ameshDome[nCntmeshDome].pVtxBuff,
			NULL);

		VERTEX_3D *pVtx;		// 頂点情報へのポインタ

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_ameshDome[nCntmeshDome].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点[H]の角度を計算
		float fAngleH = D3DX_PI * 2 / g_ameshDome[nCntmeshDome].nNumSplitH;
		float fDestAngleH = 0.0f;		// 目的の角度を格納する関数
		// 頂点[V]の角度を計算
		float fAngleV = D3DX_PI / g_ameshDome[nCntmeshDome].nNumSplitV;
		float fDestAngleV = D3DX_PI;		// 目的の角度を格納する関数

		// 頂点座標の設定
		for (int nCnt1 = 0; nCnt1 < g_ameshDome[nCntmeshDome].nNumSplitV + 1; nCnt1++)
		{// 縦の軸
			 // 角度[H]のリセット
			fDestAngleH = 0.0f;

			for (int nCnt2 = 0; nCnt2 < g_ameshDome[nCntmeshDome].nNumSplitH + 1; nCnt2++)
			{// 横の軸
				// 角度[H]チェック
				if (fDestAngleH > D3DX_PI)
				{
					fDestAngleH -= D3DX_PI * 2;
				}

				pVtx[nCnt1 * (g_ameshDome[nCntmeshDome].nNumSplitH + 1) + nCnt2].pos =
					D3DXVECTOR3(sinf(fDestAngleH) * g_ameshDome[nCntmeshDome].fRadius * sinf(fDestAngleV),
						cosf(fDestAngleV) * g_ameshDome[nCntmeshDome].fRadius,
						cosf(fDestAngleH) * g_ameshDome[nCntmeshDome].fRadius * sinf(fDestAngleV)) + g_ameshDome[nCntmeshDome].pos;

				// 角度[H]を進める
				fDestAngleH += fAngleH;
			}
			// 角度[V]を進める
			fDestAngleV -= fAngleV;
		}
		// 法線の設定　
		for (int nCntNor = 0; nCntNor < g_ameshDome[nCntmeshDome].nNumVertex; nCntNor++)
		{
			D3DXVec3Normalize(&pVtx[nCntNor].nor, &pVtx[nCntNor].pos);		// 正規化する
		}

		// 頂点カラーの設定
		for (int nCntCol = 0; nCntCol < g_ameshDome[nCntmeshDome].nNumVertex; nCntCol++)
		{
			pVtx[nCntCol].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// テクスチャの設定
		for (int nCntTex = 0; nCntTex < g_ameshDome[nCntmeshDome].nNumVertex; nCntTex++)
		{
			pVtx[nCntTex].tex = D3DXVECTOR2(1.0f / 2.0f / g_ameshDome[nCntmeshDome].nNumSplitH * (nCntTex % (g_ameshDome[nCntmeshDome].nNumSplitH + 1)),
				1.0f / 2.0f / g_ameshDome[nCntmeshDome].nNumSplitV * (nCntTex / (g_ameshDome[nCntmeshDome].nNumSplitH + 1)));
		}

		// 頂点バッファをアンロックする
		g_ameshDome[nCntmeshDome].pVtxBuff->Unlock();
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// インデックス情報の作成
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void MakeIndexmeshDome(LPDIRECT3DDEVICE9 pDevice)
{
	for (int nCntmeshDome = 0; nCntmeshDome < g_NumMAXmeshDomeInfo; nCntmeshDome++)
	{
		// インデックスバッファを生成
		// インデックス情報の作成
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_ameshDome[nCntmeshDome].nNumIndex,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,			// インデックスフォーマット [16bit (2byte)]
			D3DPOOL_MANAGED,
			&g_ameshDome[nCntmeshDome].pIdxBuff,
			NULL);

		WORD *pIdx;				// インデックスデータへのポインタ

		// インデックスバッファをロックし、インデックスデータへのポインタを取得
		g_ameshDome[nCntmeshDome].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		// インデックスの設定
		for (int nCnt1 = 0, nCnt2 = 0; nCnt2 < g_ameshDome[nCntmeshDome].nNumIndex; nCnt1++, nCnt2 += 2)
		{
			if ((nCnt2 + 2) % (g_ameshDome[nCntmeshDome].nNumSplitH * 2 + 4) == 0 && nCnt2 != 0)
			{// 縮退ポリゴン
				pIdx[nCnt2] = pIdx[nCnt2 - 1];
				pIdx[nCnt2 + 1] = nCnt1 + g_ameshDome[nCntmeshDome].nNumSplitH + 1;
				nCnt1--;
			}
			else
			{// 通常のポリゴン
				pIdx[nCnt2] = nCnt1 + g_ameshDome[nCntmeshDome].nNumSplitH + 1;
				pIdx[nCnt2 + 1] = nCnt1;
			}
		}

		// 頂点バッファをアンロックする
		g_ameshDome[nCntmeshDome].pIdxBuff->Unlock();
	}
}
