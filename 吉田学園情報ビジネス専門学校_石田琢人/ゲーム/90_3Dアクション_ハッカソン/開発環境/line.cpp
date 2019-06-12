//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// ライン処理 [line.cpp]
// Author : Toshiharu Ueno
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "line.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LINE	(12 * 512)		// ラインの最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexLine(LPDIRECT3DDEVICE9 pDevice);		// 頂点情報の生成

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLine;	// 頂点バッファへのポインタ
Line g_aLine[MAX_LINE];					// ラインの情報
int g_nIdxLine = 0;						// ラインの使用番号
bool g_bLine = true;					// ラインを描画するかどうか

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 初期化処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void InitLine(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 初期化
	for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{
		g_aLine[nCntLine].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_aLine[nCntLine].posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aLine[nCntLine].posEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aLine[nCntLine].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aLine[nCntLine].bUse = false;
		g_aLine[nCntLine].nIdxLine = 0;
	}

	// 頂点情報の作成
	MakeVertexLine(pDevice);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 終了処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UninitLine(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffLine != NULL)
	{
		g_pVtxBuffLine->Release();
		g_pVtxBuffLine = NULL;
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 更新処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void UpdateLine(void)
{
	// 描画切り替え
	if (GetKeyboardTrigger(DIK_0) == true)
	{
		g_bLine = g_bLine ? false : true;
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 描画処理
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void DrawLine(void)
{
	if (g_bLine == true)
	{// 描画する場合
		LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
		D3DXMATRIX mtxTrans, mtxView;				// 計算用マトリックス

		// ライトオフ
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
		{
			if (g_aLine[nCntLine].bUse == true)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aLine[nCntLine].mtxWorld);

				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans,
					g_aLine[nCntLine].pos.x, g_aLine[nCntLine].pos.y, g_aLine[nCntLine].pos.z);
				D3DXMatrixMultiply(&g_aLine[nCntLine].mtxWorld,
					&g_aLine[nCntLine].mtxWorld, &mtxTrans);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aLine[nCntLine].mtxWorld);

				// 頂点バッファをデータストリームに設定
				pDevice->SetStreamSource(0, g_pVtxBuffLine, 0, sizeof(VERTEX_3D));

				// 頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				// ポリゴン[線]の描画
				pDevice->DrawPrimitive(D3DPT_LINELIST, 2 * nCntLine, 1);
			}
		}

		// ライトオン
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 頂点情報の作成
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void MakeVertexLine(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;		// 頂点情報へのポインタ

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2 * MAX_LINE,		// 確保するバッファのサイズ
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,				// 頂点フォーマット
								D3DPOOL_MANAGED,
								&g_pVtxBuffLine,
								NULL);

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffLine->Unlock();
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ラインの設定
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void SetLine(D3DXVECTOR3 pos, D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd, D3DXCOLOR col,int nIdxLine)
{
	VERTEX_3D *pVtx;		// 頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++, pVtx += 2)
	{
		if (g_aLine[nCntLine].bUse == false)
		{ // 影が使用されていない場合
			g_aLine[nCntLine].pos = pos;
			g_aLine[nCntLine].posStart = posStart;
			g_aLine[nCntLine].posEnd = posEnd;
			g_aLine[nCntLine].col = col;
			g_aLine[nCntLine].nIdxLine = nIdxLine;

			// 頂点座標の設定
			pVtx[0].pos = g_aLine[nCntLine].posStart;
			pVtx[1].pos = g_aLine[nCntLine].posEnd;

			// 頂点カラーの設定
			pVtx[0].col = g_aLine[nCntLine].col;
			pVtx[1].col = g_aLine[nCntLine].col;

			g_aLine[nCntLine].bUse = true; // 使用している状態にする

			break;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffLine->Unlock();
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 当たり判定用ラインセットの設定
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int SetColisionLine(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	// インデックスを進める
	g_nIdxLine++;

	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z), D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z), D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z), D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z), D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), g_nIdxLine);

	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z), D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z), D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z), D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z), D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), g_nIdxLine);

	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMin.z), D3DXVECTOR3(vtxMin.x, vtxMax.y, vtxMax.z), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMin.z), D3DXVECTOR3(vtxMax.x, vtxMax.y, vtxMax.z), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMin.z), D3DXVECTOR3(vtxMin.x, vtxMin.y, vtxMax.z), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMin.z), D3DXVECTOR3(vtxMax.x, vtxMin.y, vtxMax.z), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), g_nIdxLine);

	return g_nIdxLine;		// インデックスを返す
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 当たり判定用ラインセットの設定
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int SetColisionLine2(D3DXVECTOR3 pos, float fRadius, float fHeight)
{
	// インデックスを進める
	g_nIdxLine++;

	//--- XZ [上] ---//
	SetLine(pos, D3DXVECTOR3(-fRadius,	fHeight,	-fRadius),	D3DXVECTOR3(fRadius,	fHeight,	-fRadius),	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(fRadius,	fHeight,	-fRadius),	D3DXVECTOR3(fRadius,	fHeight,	fRadius),	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(fRadius,	fHeight,	fRadius),	D3DXVECTOR3(-fRadius,	fHeight,	fRadius),	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(-fRadius,	fHeight,	fRadius),	D3DXVECTOR3(-fRadius,	fHeight,	-fRadius),	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_nIdxLine);
	//--- XZ [下] ---//
	SetLine(pos, D3DXVECTOR3(-fRadius,	0.0f,		-fRadius),	D3DXVECTOR3(fRadius,	0.0f,		-fRadius),	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(fRadius,	0.0f,		-fRadius),	D3DXVECTOR3(fRadius,	0.0f,		fRadius),	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(fRadius,	0.0f,		fRadius),	D3DXVECTOR3(-fRadius,	0.0f,		fRadius),	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(-fRadius,	0.0f,		fRadius),	D3DXVECTOR3(-fRadius,	0.0f,		-fRadius),	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_nIdxLine);
	//--- Y [上→下へ]---//
	SetLine(pos, D3DXVECTOR3(-fRadius,	fHeight,	-fRadius),	D3DXVECTOR3(-fRadius,	0.0f,		-fRadius),	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(fRadius,	fHeight,	-fRadius),	D3DXVECTOR3(fRadius,	0.0f,		-fRadius),	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(fRadius,	fHeight,	fRadius),	D3DXVECTOR3(fRadius,	0.0f,		fRadius),	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_nIdxLine);
	SetLine(pos, D3DXVECTOR3(-fRadius,	fHeight,	fRadius),	D3DXVECTOR3(-fRadius,	0.0f,		fRadius),	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), g_nIdxLine);

	return g_nIdxLine;		// インデックスを返す
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ラインの位置更新
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void SetPositionLine(int nIdxLine, D3DXVECTOR3 pos)
{
	for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{// 使用状態 & 同じインデックスの場合
		if (g_aLine[nCntLine].bUse == true && g_aLine[nCntLine].nIdxLine == nIdxLine)
		{
			g_aLine[nCntLine].pos = pos;		// 位置の更新
		}
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ラインの削除
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void DeleteLine(int nIdxLine)
{
	for (int nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{// 使用状態 & 同じインデックスの場合
		if (g_aLine[nCntLine].bUse == true && g_aLine[nCntLine].nIdxLine == nIdxLine)
		{
			g_aLine[nCntLine].bUse = false;		// 未使用に戻す
		}
	}
}