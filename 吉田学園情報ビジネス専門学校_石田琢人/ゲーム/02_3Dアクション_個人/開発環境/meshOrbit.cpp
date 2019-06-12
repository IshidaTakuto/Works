//=============================================================================
//
// メッシュオービット処理 [meshOrbit.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "meshOrbit.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MESHORBIT		"data/TEXTURE/bg000.jpg"	// 読み込むテクスチャファイル名
#define MAX_ORBIT	(8)		// オービットの最大数
#define MAX_VERTEX	(256)	// 頂点の最大数
#define MAX_TEX		(1)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexMeshOrbit(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshOrbit[MAX_TEX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshOrbit = NULL;	// 頂点バッファへのポインタ
MeshOrbit g_aMeshOrbit[MAX_ORBIT];
int g_nCntUseVertex;

//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshOrbit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MESHORBIT, &g_pTextureMeshOrbit[0]);

	int nCntOrbit;

	for (nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		g_aMeshOrbit[nCntOrbit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshOrbit[nCntOrbit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshOrbit[nCntOrbit].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aMeshOrbit[nCntOrbit].bUse = false;
	}

	g_nCntUseVertex = 0;

	// 頂点情報の作成
	MakeVertexMeshOrbit(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshOrbit(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureMeshOrbit[nCntTex] != NULL)
		{
			g_pTextureMeshOrbit[nCntTex]->Release();
			g_pTextureMeshOrbit[nCntTex] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffMeshOrbit != NULL)
	{
		g_pVtxBuffMeshOrbit->Release();
		g_pVtxBuffMeshOrbit = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshOrbit(void)
{
	int nCntOrbit;
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMeshOrbit->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		if (g_aMeshOrbit[nCntOrbit].bUse == false)
		{
			// 小さい方にデータを移す
			g_aMeshOrbit[nCntOrbit].pos = g_aMeshOrbit[MAX_ORBIT - 1 - nCntOrbit].pos;
			g_aMeshOrbit[nCntOrbit].start = g_aMeshOrbit[MAX_ORBIT - 1 - nCntOrbit].start;
			g_aMeshOrbit[nCntOrbit].end = g_aMeshOrbit[MAX_ORBIT - 1 - nCntOrbit].end;
			g_aMeshOrbit[nCntOrbit].col = g_aMeshOrbit[MAX_ORBIT - 1 - nCntOrbit].col;
			g_aMeshOrbit[nCntOrbit].nTime = g_aMeshOrbit[MAX_ORBIT - 1 - nCntOrbit].nTime;
			g_aMeshOrbit[nCntOrbit].nType = g_aMeshOrbit[MAX_ORBIT - 1 - nCntOrbit].nType;
			g_aMeshOrbit[nCntOrbit].bUse = true;

			if (nCntOrbit % 2 == 0)
			{
				// 頂点座標の指定
				pVtx[nCntOrbit - 1].pos = g_aMeshOrbit[nCntOrbit - 1].start;
				pVtx[nCntOrbit].pos = g_aMeshOrbit[nCntOrbit].end;

				pVtx[nCntOrbit - 1].col = g_aMeshOrbit[nCntOrbit - 1].col;
				pVtx[nCntOrbit].col = g_aMeshOrbit[nCntOrbit].col;

				break;
			}
		}
	}

	for (nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		if (g_aMeshOrbit[nCntOrbit].bUse == true)
		{
			g_aMeshOrbit[nCntOrbit].nTime--;	// 時間を減らす

			if (g_aMeshOrbit[nCntOrbit].nTime <= 0)
			{// 時間が0以下になった
				g_aMeshOrbit[nCntOrbit].col.a -= 0.02f;	// 徐々に透明化

				if (g_aMeshOrbit[nCntOrbit].col.a <= 0)
				{// 透明になったらfalse
					g_aMeshOrbit[nCntOrbit].bUse = false;
				}
				pVtx[nCntOrbit].col = g_aMeshOrbit[nCntOrbit].col;
			}
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffMeshOrbit->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshOrbit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans;				// 計算用マトリックス
	int nCntMeshOrbit;

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);		// カリングを無効化

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (nCntMeshOrbit = 0; nCntMeshOrbit < MAX_ORBIT; nCntMeshOrbit++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMeshOrbit[nCntMeshOrbit].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshOrbit[nCntMeshOrbit].rot.y, g_aMeshOrbit[nCntMeshOrbit].rot.x, g_aMeshOrbit[nCntMeshOrbit].rot.z);
		D3DXMatrixMultiply(&g_aMeshOrbit[nCntMeshOrbit].mtxWorld, &g_aMeshOrbit[nCntMeshOrbit].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aMeshOrbit[nCntMeshOrbit].pos.x, g_aMeshOrbit[nCntMeshOrbit].pos.y, g_aMeshOrbit[nCntMeshOrbit].pos.z);
		D3DXMatrixMultiply(&g_aMeshOrbit[nCntMeshOrbit].mtxWorld, &g_aMeshOrbit[nCntMeshOrbit].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aMeshOrbit[nCntMeshOrbit].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffMeshOrbit, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMeshOrbit[g_aMeshOrbit[nCntMeshOrbit].nType]);

		if (g_aMeshOrbit[nCntMeshOrbit].bUse == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, MAX_VERTEX - 2);
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

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		// 裏面をカリング
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMeshOrbit(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshOrbit,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMeshOrbit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOrbit = 0; nCntOrbit < MAX_VERTEX / 2; nCntOrbit++)
	{
		// 頂点の座標
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(nCntOrbit * 1.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(nCntOrbit * 1.0f, 0.0f);

		pVtx += 2;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffMeshOrbit->Unlock();
}

//=============================================================================
// オービットの設定
//=============================================================================
void SetOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col, int nTime, int nType)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMeshOrbit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		g_aMeshOrbit[nCntOrbit].pos = pos;
		g_aMeshOrbit[nCntOrbit].start = start;
		g_aMeshOrbit[nCntOrbit].end = end;
		g_aMeshOrbit[nCntOrbit].col = col;
		g_aMeshOrbit[nCntOrbit].nTime = nTime;
		g_aMeshOrbit[nCntOrbit].nType = nType;
	}

	// 頂点の座標
	pVtx[MAX_ORBIT - 1].pos = start;
	pVtx[MAX_ORBIT - 2].pos = end;

	pVtx[MAX_ORBIT - 1].col = col;
	pVtx[MAX_ORBIT - 2].col = col;

	// 頂点バッファをアンロック
	g_pVtxBuffMeshOrbit->Unlock();
}