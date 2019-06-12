//=============================================================================
//
// ポリゴン処理 [meshbg.cpp]
// Author : 有馬　武志
//
//=============================================================================
#include "meshbg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULOCK_Z					(1.0f)												//ブロックの横行
#define BULOCK_X					(1.0f)												//ブロックの奥行
#define BULOCK_Y					(1.0f)												//ブロックの縦
#define VERTICAL					(1)													//縦
#define CORSS						(8)													//横
#define VERTEX						((VERTICAL + 1) * (CORSS + 1))						//辺と辺の計算
#define ANGLE						(1.0f / (CORSS/2))									//指定した数文のPIを出す式
#define IDX							(((2 + CORSS) * 2 * VERTICAL) + (VERTICAL - 1) * 4)	//インデックスの計算
#define POLYGON						((VERTICAL * CORSS) * 2 + (VERTICAL - 1) * 4)		//ポリゴンの計算

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexMeshBg(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshBg = NULL;	//頂点バッファ
LPDIRECT3DTEXTURE9		g_pTextureMeshBg = NULL;	//テクスチャへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshBg = NULL;	//インデックスバッファのポインタ
MeshBg g_MeshBg;								//メッシュウォールの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshBg(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	//初期値
	g_MeshBg.posMeshBg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	g_MeshBg.rotMeshBg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	g_MeshBg.nNumVertexMeshBg = VERTEX;						//頂点数
	g_MeshBg.nNumIdxMeshBg = IDX;							//インデックス数
	g_MeshBg.nNumPolygonMeshBg = POLYGON;					//ポリゴン数

	//頂点情報
	MakeVertexMeshBg(pDevice);
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshBg(void)
{
	//テクスチャーの破棄
	if (g_pTextureMeshBg != NULL)
	{
		g_pTextureMeshBg->Release();
		g_pTextureMeshBg = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffMeshBg != NULL)
	{
		g_pVtxBuffMeshBg->Release();
		g_pVtxBuffMeshBg = NULL;
	}
	//インデックスバッファの解放
	if (g_pIdxBuffMeshBg != NULL)
	{
		g_pIdxBuffMeshBg-> Release();
		g_pIdxBuffMeshBg = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshBg(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshBg(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_MeshBg.mtxWorldMeshBg);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshBg.rotMeshBg.y, g_MeshBg.rotMeshBg.x, g_MeshBg.rotMeshBg.z);

	D3DXMatrixMultiply(&g_MeshBg.mtxWorldMeshBg, &g_MeshBg.mtxWorldMeshBg, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_MeshBg.posMeshBg.x, g_MeshBg.posMeshBg.y, g_MeshBg.posMeshBg.z);

	D3DXMatrixMultiply(&g_MeshBg.mtxWorldMeshBg, &g_MeshBg.mtxWorldMeshBg, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_MeshBg.mtxWorldMeshBg);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshBg, 0, sizeof(VERTEX_3D));

	//インデックスバッファデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshBg);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshBg);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0,0 , POLYGON,0, g_MeshBg.nNumPolygonMeshBg);

	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMeshBg(LPDIRECT3DDEVICE9 pDevice)
{
	//変数宣言
	int nCntCorss;
	int nCntVertical;
	int nCntIdx;

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * g_MeshBg.nNumVertexMeshBg,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshBg,
		NULL
	);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * g_MeshBg.nNumIdxMeshBg,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshBg,
		NULL
	);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//インデックスデータへのポインタ
	WORD * pIdx;

	//頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffMeshBg->Lock(0, 0, (void**)&pVtx, 0);

	//旧頂点設定
#if 0
	////頂点情報
	//pVtx[0].pos = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
	//pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 100.0f);
	//pVtx[2].pos = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
	//pVtx[3].pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);
	//pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//pVtx[5].pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
	//pVtx[6].pos = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);
	//pVtx[7].pos = D3DXVECTOR3(0.0f, 0.0f, -100.0f);
	//pVtx[8].pos = D3DXVECTOR3(100.0f, 0.0f, -100.0f);

	////頂点のインデックス情報
	//pIdx[0] = 3;
	//pIdx[1] = 0;
	//pIdx[2] = 4;
	//pIdx[3] = 1;
	//pIdx[4] = 5;
	//pIdx[5] = 2;
	//pIdx[6] = 2;
	//pIdx[7] = 6;
	//pIdx[8] = 6;
	//pIdx[9] = 3;
	//pIdx[10] = 7;
	//pIdx[11] = 4;
	//pIdx[12] = 8;
	//pIdx[13] = 5;
#endif

	//頂点、法線、カラー、テクスチャの頂点設定
	for (nCntVertical = 0; nCntVertical <= VERTICAL; nCntVertical++)
	{//縦軸
		for (nCntCorss = 0; nCntCorss < CORSS + 1; nCntCorss++)
		{//横軸
		 //円柱
			//pVtx[頂点番号].x,,z = sinf((3.14の円周　これに-をつけると、カリング方向が逆になる　* (カウントの回数　* 指定した数を計算して角度調整)))* 半径;
			pVtx[0].pos.x = sinf((-D3DX_PI * (nCntCorss * ANGLE)))* 1200.0f;
			//pVtx[頂点番号].y = 縦のカウント回数　* 縦の幅
			pVtx[0].pos.y = nCntVertical * 900.0f;
			pVtx[0].pos.z = cosf((-D3DX_PI * (nCntCorss * ANGLE)))* 1200.0f;

			//法線
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ
			pVtx[0].tex = D3DXVECTOR2(nCntCorss * 1.0f, nCntVertical * 1.0f);

			//ずらし
			pVtx += 1;
		}
	}

	//旧インデックス文
#if 0
	//頂点バッファのアンロックする
	g_pVtxBuffMeshBg->Unlock();

	//for (nCntVertical = 0; nCntVertical < VERTICAL; nCntVertical++)
	//{//縦軸
	//	for (nCntCorss = 0; nCntCorss < CORSS + 1; nCntCorss++)
	//	{//横軸
	//	    //pVtx[たてのカウント数　* (横のブロック数 * ポリゴン数) 折り返し分【+ カウント数 * ポリゴン数】+ 横のカウント * ポリゴン数 + 頂点番号]
	//		//                                                   .pos = D3DXVECTOR3(横カウント * ブロックの幅,Y軸は０,縦のカウント * ブロックの高さ - ブロックの高さ)
	//		//                                                  .pos = D3DXVECTOR3(横カウント * ブロックの幅,Y軸は０,縦のカウント * ブロックの高さ)
	//		pVtx[nCntVertical * (CORSS * 2) + nCntVertical * 2 + nCntCorss * 2].pos = D3DXVECTOR3(nCntCorss * BULOCK_X, 0.0f,nCntVertical * BULOCK_Z - BULOCK_Z);	//0
	//		pVtx[nCntVertical * (CORSS * 2) + nCntVertical * 2 + nCntCorss * 2 + 1].pos = D3DXVECTOR3(nCntCorss * BULOCK_X, 0, nCntVertical * BULOCK_Z);			//1


	//		//最後の頂点に行ったら
	//		//横のカウントがブロック数-1に行った場合と縦が空白の場所を-1して縦のブロック数が足りなくない場合
	//		if (nCntCorss == CORSS - 1 && nCntVertical - 1 != VERTICAL)
	//		{
	//			//pVtx[たてのカウント数　* (横のブロック数 * ポリゴン数) 折り返し分【+ カウント数 * ポリゴン数】+ 横のカウント * ポリゴン数 + 頂点番号]
	//			//                                                       .pos = D3DXVECTOR3(横カウント * ブロックの幅,Y軸は０,縦のカウント * ブロックの高さ - ブロックの高さ)
	//			//                                                       .pos = D3DXVECTOR3(X軸は動かないから０,Y軸は０,縦のカウント * ブロックの高さ + ブロックの高さ)
	//			pVtx[nCntVertical * (CORSS * 2) + nCntVertical * 2 + nCntCorss * 2 + 2].pos = D3DXVECTOR3(nCntCorss * BULOCK_X, 0, nCntCorss * BULOCK_Z);	//2
	//			pVtx[nCntVertical * (CORSS * 2) + nCntVertical * 2 + nCntCorss * 2 + 3].pos = D3DXVECTOR3(0, 0, nCntVertical * BULOCK_Z + BULOCK_Z);				//3
	//		}
	//	}
	//}

	//for (nCntNorCol = 0; nCntNorCol < g_g_MeshBg.nNumVertexMeshBg; nCntNorCol++)
	//{//法線ベクトルと頂点カラーのfor文
	//	//法線ベクトル
	//	pVtx[nCntNorCol].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//	//頂点カラー
	//	pVtx[nCntNorCol].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//}
	////テクスチャ設定
	////                         X     Z
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
	//pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.5f, 0.5f);
	//pVtx[3].tex = D3DXVECTOR2(0.5f, 0.0f);
	//pVtx[4].tex = D3DXVECTOR2(1.0f, 0.5f);
	//pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[6].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[7].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[8].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[9].tex = D3DXVECTOR2(0.0f, 0.5f);
	//pVtx[10].tex = D3DXVECTOR2(0.5f, 1.0f);
	//pVtx[11].tex = D3DXVECTOR2(0.5f, 0.5f);
	//pVtx[12].tex = D3DXVECTOR2(1.0f, 1.0f);
	//pVtx[13].tex = D3DXVECTOR2(1.0f, 0.5f);
#endif

	//インデックスバッファのロックし、インデックスデータへのポインタを取得
	g_pIdxBuffMeshBg->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスの式
	for (nCntVertical = 0, nCntIdx = 0; nCntVertical < VERTICAL; nCntVertical++)
	{//縦軸カウント
		for (nCntCorss = 0; nCntCorss < CORSS + 1; nCntCorss++, nCntIdx++)
		{//横軸カウント
		 //pIdx[番号] = (指定した横 + １（この１は０からスタートした時に１足りないから） + インデックスの回数)
			pIdx[0] = (CORSS + 1) + nCntIdx;
			pIdx[1] = 0 + nCntIdx;

			//２頂点ずらし
			pIdx += 2;

			if (nCntVertical < VERTICAL - 1 && nCntCorss == CORSS)
			{//指定した縦-1に行ったときと横が指定した数まで行ったとき
				pIdx[0] = 0 + nCntIdx;
				pIdx[1] = (CORSS + 1) + nCntIdx + 1;

				//２頂点文ずらす
				pIdx += 2;
			}
		}
	}

	//インデックスのバッファのアンロック
	g_pIdxBuffMeshBg->Unlock();

}

