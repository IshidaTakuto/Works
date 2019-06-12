//=============================================================================
//
// 弾処理 [bullet.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "bullet.h"
#include "shadow.h"
#include "input.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_BULLET		"data/TEXTURE/bullet000.png"	// 読み込むテクスチャファイル名
#define MAX_BULLET		(128)
#define BULLET_LIMIT	(150)
#define BULLET_SPEED	(1.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	// 頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];						// 壁の情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitBullet(void)
{
	int nCntBullet;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET, &g_pTextureBullet);

	// 頂点情報の作成
	MakeVertexBullet(pDevice);

	// 壁の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
		g_aBullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きの初期化
		g_aBullet[nCntBullet].fHeight = 0.0f;
		g_aBullet[nCntBullet].fWidth = 0.0f;
		g_aBullet[nCntBullet].bUse = false;
	}

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	int nCntBullet;

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

			//エフェクト
			SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.4f, 0.0f, 0.7f), 5.0f, 60, EFFECTSTATE_BULLET);

			// ビルボードの移動
			g_aBullet[nCntBullet].pos.x -= sinf((D3DX_PI * 0.0f) + g_aBullet[nCntBullet].rot.y) * BULLET_SPEED;
			g_aBullet[nCntBullet].pos.z -= cosf((D3DX_PI * 0.0f) + g_aBullet[nCntBullet].rot.y) * BULLET_SPEED;

			if (g_aBullet[nCntBullet].pos.x < -BULLET_LIMIT
				|| g_aBullet[nCntBullet].pos.x > BULLET_LIMIT
				|| g_aBullet[nCntBullet].pos.z < -BULLET_LIMIT
				|| g_aBullet[nCntBullet].pos.z > BULLET_LIMIT)
			{// ビルボード削除
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0));	// 爆発させる
				//SetParticle(g_aBullet[nCntBullet].pos,
				//	D3DXCOLOR((rand() % 10) * 0.1f, (rand() % 10) * 0.1f, (rand() % 10) * 0.1f, 1.0f),
				//	(rand() % 3) * 1.5f,
				//	15,
				//	PARTICLETYPE_SPARK);				
				g_aBullet[nCntBullet].bUse = false;					// 使用していない状態にする
				DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);		// 影を消す
			}


			// 影の頂点座標
			SetVtxShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos, 0.7f);

			// 影の位置
			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX	mtxRot, mtxTrans, mtxView;		// 計算用マトリックス
	int nCntBullet;

	// αテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

		// 回転を反映
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		g_aBullet[nCntBullet].mtxWorld._11 = mtxView._11;
		g_aBullet[nCntBullet].mtxWorld._12 = mtxView._21;
		g_aBullet[nCntBullet].mtxWorld._13 = mtxView._31;
		g_aBullet[nCntBullet].mtxWorld._21 = mtxView._12;
		g_aBullet[nCntBullet].mtxWorld._22 = mtxView._22;
		g_aBullet[nCntBullet].mtxWorld._23 = mtxView._32;
		g_aBullet[nCntBullet].mtxWorld._31 = mtxView._13;
		g_aBullet[nCntBullet].mtxWorld._32 = mtxView._23;
		g_aBullet[nCntBullet].mtxWorld._33 = mtxView._33;

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
		D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBullet);

		if (g_aBullet[nCntBullet].bUse == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBullet, 2);
		}
	}

	// αテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntBullet;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// 頂点の座標
		pVtx[0].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidth, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fWidth, 0.0f, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	// 頂点データのポインタを4つ進める
	}
	// 頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// 弾の設定処理
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXVECTOR3 rot)
{
	int nCntBullet;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].fWidth = fWidth;
			g_aBullet[nCntBullet].fHeight = fHeight;
			g_aBullet[nCntBullet].rot = rot;

			// 頂点の座標
			pVtx[0].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidth, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fWidth, 0.0f, 0.0f);

			g_aBullet[nCntBullet].bUse = true;

			// 影をつける
			g_aBullet[nCntBullet].nIdxShadow = SetShadow(g_aBullet[nCntBullet].pos, 
															 g_aBullet[nCntBullet].rot, 
															 g_aBullet[nCntBullet].fWidth, 
															 g_aBullet[nCntBullet].fHeight);

			break;
		}
		pVtx += 4;	// 頂点データのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// 当たり判定の処理
//=============================================================================
bool CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax)
{
	bool bHit = false;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			if (pPos->x + vtxMin.x < g_aBullet[nCntBullet].fWidth + g_aBullet[nCntBullet].pos.x && pPos->x + vtxMax.x > g_aBullet[nCntBullet].pos.x  - g_aBullet[nCntBullet].fWidth &&
				pPos->y + vtxMin.y < g_aBullet[nCntBullet].fHeight + g_aBullet[nCntBullet].pos.y && pPos->y + vtxMax.y > g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight &&
				pPos->z + vtxMin.z < g_aBullet[nCntBullet].fHeight + g_aBullet[nCntBullet].pos.z && pPos->z + vtxMax.z > g_aBullet[nCntBullet].pos.z - g_aBullet[nCntBullet].fHeight)
			{// 弾の範囲
				bHit = true;
			}
		}
	}
	return bHit;
}