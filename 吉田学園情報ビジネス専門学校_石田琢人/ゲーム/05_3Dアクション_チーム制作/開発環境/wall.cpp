//=============================================================================
//
// ポリゴン処理 [wall.cpp]
// Author : 有馬　武志
//
//=============================================================================
#include "wall.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define WALL_TEXTURE_NAME		"data\\TEXTURE\\通行止め.jpg"		//読み込むテクスチャファイル
#define MAX_WALL				(4)									//壁の最大数
#define	WALL_SPEED				(0.0003f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexWall(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	//頂点バッファ
LPDIRECT3DTEXTURE9		g_pTextureWall = NULL;	//テクスチャへのポインタ
Wall g_Wall[MAX_WALL];							//壁の情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCount = 0; nCount < MAX_WALL; nCount++)
	{//全カウントを回す
		g_Wall[nCount].rotWall = D3DXVECTOR3(0, 0, 0);	//向き
		g_Wall[nCount].posWall = D3DXVECTOR3(0, 0, 0);	//位置

		g_Wall[nCount].moveWall = D3DXVECTOR3(WALL_SPEED, 0.0f, 0.0f);		//ポリゴン移動値
		g_Wall[nCount].bUse = false;					//使用しているかどうか
	}

	//頂点情報
	MakeVertexWall(pDevice);
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitWall(void)
{
	for (int nCount = 0; nCount < MAX_WALL; nCount++)
	{//全テクスチャのカウント
		//テクスチャーの破棄
		if (g_pTextureWall != NULL)
		{
			g_pTextureWall->Release();
			g_pTextureWall = NULL;
		}
	}
	// 頂点バッファの開放
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateWall(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_WALL; nCount++)
	{//全カウントを回す

		g_Wall[nCount].posWall += g_Wall[nCount].moveWall;	//位置の更新

		//テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(g_Wall[nCount].posWall.x, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_Wall[nCount].posWall.x + 1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_Wall[nCount].posWall.x, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_Wall[nCount].posWall.x + 1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffWall->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	//ポリゴンの描画
	for (int nCount = 0; nCount < MAX_WALL; nCount++)
	{//ブロックをカウント
		if (g_Wall[nCount].bUse == true)
		{//ブロックがある場合

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Wall[nCount].mtxWorldWall);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCount].rotWall.y, g_Wall[nCount].rotWall.x, g_Wall[nCount].rotWall.z);

			D3DXMatrixMultiply(&g_Wall[nCount].mtxWorldWall, &g_Wall[nCount].mtxWorldWall, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Wall[nCount].posWall.x, g_Wall[nCount].posWall.y, g_Wall[nCount].posWall.z);

			D3DXMatrixMultiply(&g_Wall[nCount].mtxWorldWall, &g_Wall[nCount].mtxWorldWall, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Wall[nCount].mtxWorldWall);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexWall(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		WALL_TEXTURE_NAME,
		&g_pTextureWall
	);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL
	);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点データへのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_WALL; nCount++)
	{//全カウントを回す
	 //頂点座
		pVtx[0].pos = D3DXVECTOR3(-870.0f, 150.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(870.0f, 150.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-870.0f, 30.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(870.0f, 30.0f, 0.0f);

		//法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファのアンロックする
	g_pVtxBuffWall->Unlock();
}
//=============================================================================
// 壁の作成
//=============================================================================
void SetWall(D3DXVECTOR3 pos,D3DXVECTOR3 rot)
{
	for (int nCount = 0; nCount < MAX_WALL; nCount++)
	{//全テクスチャのカウント
		if (g_Wall[nCount].bUse == false)
		{
			g_Wall[nCount].posWall = pos;			//pos値代入
			g_Wall[nCount].rotWall = rot;			//rot値代入
			g_Wall[nCount].bUse = true;				//使用している状態にする

			break;
		}
	}
}
