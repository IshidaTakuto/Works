//=============================================================================
//
// オブジェクトの処理 [object.cpp]
// Author : 佐藤亮太
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS		// ファイル読み込みで必要
#include <stdio.h>					// ファイル読み込みで必要
#include <string.h>					// ファイル読み込みで必要

#include "object.h"					// オブジェクト情報のヘッダー
//#include "shadow.h"				// 影情報のヘッダー
//#include "explosion.h"			// 爆発情報のヘッダー
//#include "particle.h"				// パーティクル情報のヘッダー
#include "game.h"					// ゲーム情報のヘッダー
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OPEN_FILE_NAME						("data/TEXTFILE/model.txt")		// Xファイルのアドレス
#define MAX_OBJECT_MODEL					(40)						// モデルの種類の最大数
//#define MAX_CHAR							(256)						// 一度に読み込める文字数
#define OBJECT_FLOWING_SPEED				(WATER_SPEED)						// 流れるスピード
#define OBJECT_INERTIA_XZ					(0.15f)						// 慣性の係数
#define OBJECT_MAX_DEPTH					(4000.0f)					// 一番奥の座標

//*****************************************************************************
// グローバル変数
//*****************************************************************************
ObjectModel			g_ObjectModel[MAX_OBJECT_MODEL];		// モデルのポインタやアドレスの情報
Object				g_Object[MAX_OBJECT];					// オブジェクトの情報
int					g_nMaxModel;							// モデルの数
int					g_nMaxTexture;							// テクスチャの数

//=============================================================================
// 初期化処理
//=============================================================================
void InitObject(void)
{
	// オブジェクトの初期化
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{// オブジェクトの総数分ループ
		g_Object[nCntObject].pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置の初期化
		g_Object[nCntObject].posOld			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 前回の位置の初期化
		g_Object[nCntObject].move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置の移動量の初期化
		g_Object[nCntObject].rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向きの初期化
		g_Object[nCntObject].rotDest		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 目的の向きを初期化
		g_Object[nCntObject].nCollisionType = 0;											// 当たり判定のタイプを初期化
		g_Object[nCntObject].fRadius		= 0.0f;											// 半径を初期化
		g_Object[nCntObject].bUse			= false;										// 使ってない状態に初期化

		// 当たり判定の変数の初期化
		g_Object[nCntObject].Collision.nIdxParent = -1;										// オフセットの位置の親を設定
		g_Object[nCntObject].Collision.pos.x = 0.0f;										// オフセットのからの位置を設定
		g_Object[nCntObject].Collision.pos.y = 20.0f;										// オフセットのからの位置を設定
		g_Object[nCntObject].Collision.pos.z = 0.0f;										// オフセットのからの位置を設定
		g_Object[nCntObject].Collision.fRadius = 20.0f;										// 半径
	}

	// オブジェクトの読み込み
	TextLoadObject();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitObject(void)
{
	for (int nCntModel = 0; nCntModel < MAX_OBJECT_MODEL; nCntModel++)
	{// モデルの数分ループ

		// メッシュの開放
		if (g_ObjectModel[nCntModel].pMesh != NULL)
		{// 中身が入ってたら
			g_ObjectModel[nCntModel].pMesh->Release();
			g_ObjectModel[nCntModel].pMesh = NULL;
		}

		// マテリアルの開放
		if (g_ObjectModel[nCntModel].pBuffMat != NULL)
		{// 中身が入ってたら
			g_ObjectModel[nCntModel].pBuffMat->Release();
			g_ObjectModel[nCntModel].pBuffMat = NULL;
		}

		// テクスチャの開放
		for (int nCntMat = 0; nCntMat < (int)g_ObjectModel[nCntModel].nNumMat; nCntMat++)
		{// マテリアルの数分ループ
			if (g_ObjectModel[nCntModel].pTexture[nCntMat] != NULL)
			{
				g_ObjectModel[nCntModel].pTexture[nCntMat]->Release();
				g_ObjectModel[nCntModel].pTexture[nCntMat] = NULL;
			}
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateObject(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{// オブジェクトの総数分ループ
		if (g_Object[nCntObject].bUse == true)
		{// オブジェクトが使われていたら
			// 流れていく処理
			FlowingObject(&g_Object[nCntObject]);

			// ループ判定
			LoopObject(&g_Object[nCntObject]);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの初期化
	D3DXMATRIX mtxRot, mtxTrans;					// 計算用マトリックス
	D3DXMATRIX mtxParent;							// 階層構造用のマトリックス
	D3DMATERIAL9 matDef;							// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;								// マテリアルデータへのポインタ

	//Zバッファを有効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// 距離で描画順を整頓する
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// 整頓する順の基準を設定


	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{// モデルの数分ループ

		if (g_Object[nCntObject].bUse == true)
		{// オブジェクトが使われているとき

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Object[nCntObject].mtxWorld);

			// 回転(向き)を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_Object[nCntObject].rot.y,
				g_Object[nCntObject].rot.x,
				g_Object[nCntObject].rot.z);								// ゲームを制作するときは一番使うＹ軸から入れる

			D3DXMatrixMultiply(&g_Object[nCntObject].mtxWorld,				// ２番目と３番目の引数を掛け算された値を一番目の引数に代入している
				&g_Object[nCntObject].mtxWorld,
				&mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_Object[nCntObject].pos.x,									// 位置はX座標から入れていく
				g_Object[nCntObject].pos.y,
				g_Object[nCntObject].pos.z);

			D3DXMatrixMultiply(&g_Object[nCntObject].mtxWorld,				// ２番目と３番目の引数を掛け算された値を一番目の引数に代入している
				&g_Object[nCntObject].mtxWorld,
				&mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD,
				&g_Object[nCntObject].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_ObjectModel[g_Object[nCntObject].nIdxModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_ObjectModel[g_Object[nCntObject].nIdxModel].nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				if (g_ObjectModel[g_Object[nCntObject].nIdxModel].pTexture[nCntMat] != (LPDIRECT3DTEXTURE9)0xcdcdcdcd)
				{// アドレス先にテクスチャが存在するとき
					pDevice->SetTexture(0, g_ObjectModel[g_Object[nCntObject].nIdxModel].pTexture[nCntMat]);
				}
				// オブジェクトの描画
				g_ObjectModel[g_Object[nCntObject].nIdxModel].pMesh->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);


			//------------------------------------------------------------
			// 当たり判定のワールドマトリックスの設定
			//------------------------------------------------------------
			mtxParent = g_Object[nCntObject].mtxWorld;		// 親のワールドマトリックスを代入

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Object[nCntObject].Collision.mtxWorld);

			// 回転(向き)を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_Object[nCntObject].Collision.rot.y,
				g_Object[nCntObject].Collision.rot.x,
				g_Object[nCntObject].Collision.rot.z);								// ゲームを制作するときは一番使うＹ軸から入れる

			D3DXMatrixMultiply(&g_Object[nCntObject].Collision.mtxWorld,			// ２番目と３番目の引数を掛け算された値を一番目の引数に代入している
							   &g_Object[nCntObject].Collision.mtxWorld,
							   &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTrans,
								  g_Object[nCntObject].Collision.pos.x,				// 位置はX座標から入れていく
								  g_Object[nCntObject].Collision.pos.y,
								  g_Object[nCntObject].Collision.pos.z);

			D3DXMatrixMultiply(&g_Object[nCntObject].Collision.mtxWorld,			// ２番目と３番目の引数を掛け算された値を一番目の引数に代入している
							   &g_Object[nCntObject].Collision.mtxWorld,
							   &mtxTrans);

			D3DXMatrixMultiply(&g_Object[nCntObject].Collision.mtxWorld,			// 親と子のマトリックスをかけ合わせる
							   &g_Object[nCntObject].Collision.mtxWorld,
							   &mtxParent);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD,&g_Object[nCntObject].Collision.mtxWorld);

		}
	}

	////Zバッファを有効にする
	//pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);			// 距離関係なく強制的に手前に描画する
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// 距離で描画順を整頓する

}

//=============================================================================
// オブジェクトの流れる処理
//=============================================================================
void FlowingObject(Object *pObject)
{
	// 流れる移動量
	pObject->move.x += sinf(D3DX_PI * 1.0f) * -OBJECT_FLOWING_SPEED;
	pObject->move.z += cosf(D3DX_PI * 1.0f) * -OBJECT_FLOWING_SPEED;

	// 慣性
	pObject->move.x += (0.0f - pObject->move.x) * 0.1f;
	pObject->move.z += (0.0f - pObject->move.z) * 0.1f;

	// オブジェクトの移動
	pObject->pos += pObject->move;
}

//=============================================================================
// 最後まで流れたら戻る処理
//=============================================================================
void LoopObject(Object *pObject)
{
	if (pObject->pos.z < 0.0f)
	{// 最後まで流れたら
		pObject->pos.z	  = OBJECT_MAX_DEPTH;		// 一番奥に戻す
		pObject->posOld.z = OBJECT_MAX_DEPTH;		// 一番奥に戻す
	}
}

//=============================================================================
// オブジェクトの当たり判定の処理
//=============================================================================
bool CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadius)
{
	bool bHit = false;					// オブジェクトに当たったかどうか

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{// モデルの数分ループ
		if (g_Object[nCntObject].bUse == true)
		{// オブジェクトが使われているとき
			//----------------------------------------------------------
			// それぞれの当たり判定
			//----------------------------------------------------------
			if (g_Object[nCntObject].nCollisionType == 0)
			{// 矩形の当たり判定
				 CubeCollisionObject(pPos, pPosOld, pMove, fRadius, nCntObject,&bHit);
			}
			//----------------------------------------------------------
			// 半径(球体)の当たり判定
			//----------------------------------------------------------
			else if(g_Object[nCntObject].nCollisionType == 1)
			{// 半径の当たり判定
				bHit = SphiaCollisionObject(pPos, fRadius, nCntObject);
			}
		}
	}
	return bHit;			// 着地したかどうかを返す
}

//=============================================================================
// オブジェクトの矩形の当たり判定
//=============================================================================
void CubeCollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadius, int nIdxObject, bool *bHit)
{
	if (g_Object[nIdxObject].pos.y + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.y > pPos->y + fRadius &&
		g_Object[nIdxObject].pos.y + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMin.y <= pPos->y + fRadius)
	{// プレイヤーがオブジェクトの範囲内なら

		// ブロックの当たり判定
		if (g_Object[nIdxObject].posOld.x + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMin.x < pPosOld->x + fRadius &&
			g_Object[nIdxObject].posOld.x + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.x > pPosOld->x - fRadius)
		{// オブジェクトのX軸の範囲内に入ったら

			// オブジェクトを基準に
			if (g_Object[nIdxObject].posOld.z + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMin.z >= pPos->z + fRadius &&
				g_Object[nIdxObject].pos.z	  + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMin.z <  pPos->z + fRadius)
			{// オブジェクトが奥からの判定
				pPos->z = g_Object[nIdxObject].pos.z + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMin.z - fRadius;			// オブジェクトの位置を戻す
				*bHit = true;		// 当たったフラグを返す
			}
			else if (g_Object[nIdxObject].posOld.z + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.z <= pPos->z - fRadius &&
				g_Object[nIdxObject].pos.z		   + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.z >  pPos->z - fRadius)
			{// オブジェクトが手前からの判定
				pPos->z = g_Object[nIdxObject].pos.z + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.z + fRadius;			// オブジェクトの位置を戻す
				*bHit = true;		// 当たったフラグを返す
			}

			if (g_Object[nIdxObject].posOld.z + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMin.z >= pPosOld->z + fRadius &&
				g_Object[nIdxObject].pos.z	  + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMin.z < pPos->z	 + fRadius)
			{// オブジェクトが奥からの判定
				pPos->z = g_Object[nIdxObject].pos.z + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMin.z - fRadius;			// オブジェクトの位置を戻す
				*bHit = true;		// 当たったフラグを返す
			}
			else if (g_Object[nIdxObject].posOld.z + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.z <= pPosOld->z - fRadius &&
					 g_Object[nIdxObject].pos.z	   + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.z > pPos->z	  - fRadius)
			{// オブジェクトが手前からの判定
				pPos->z = g_Object[nIdxObject].pos.z + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.z + fRadius;			// オブジェクトの位置を戻す
				*bHit = true;		// 当たったフラグを返す
			}

		}
		if (g_Object[nIdxObject].posOld.z + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMin.z < pPosOld->z + fRadius &&
			g_Object[nIdxObject].posOld.z + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.z > pPosOld->z - fRadius)
		{// Z軸がオブジェクトの範囲内だったら
			if (g_Object[nIdxObject].posOld.x + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMin.x >= pPosOld->x + fRadius &&
				g_Object[nIdxObject].pos.x	  + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMin.x < pPos->x	 + fRadius)
			{// 左からの判定
				pPos->x = g_Object[nIdxObject].pos.x + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMin.x - fRadius;			// オブジェクトの位置を戻す
				*bHit = true;		// 当たったフラグを返す
			}
			else if (g_Object[nIdxObject].posOld.x + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.x <= pPosOld->x - fRadius &&
					 g_Object[nIdxObject].pos.x	   + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.x > pPos->x	  - fRadius)
			{// 右からの判定
				pPos->x = g_Object[nIdxObject].pos.x + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.x + fRadius;			// オブジェクトの位置を戻す
				*bHit = true;		// 当たったフラグを返す
			}
		}
	}
	if (g_Object[nIdxObject].pos.x + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMin.x < pPos->x + fRadius &&
		g_Object[nIdxObject].pos.x + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.x > pPos->x - fRadius &&
		g_Object[nIdxObject].pos.z + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMin.z < pPos->z + fRadius &&
		g_Object[nIdxObject].pos.z + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.z > pPos->z - fRadius)
	{// オブジェクトの範囲内だったら
		if (g_Object[nIdxObject].pos.y + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.y <= pPosOld->y &&
			g_Object[nIdxObject].pos.y + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.y > pPos->y)
		{// 上からの判定
			pPos->y = g_Object[nIdxObject].pos.y + g_ObjectModel[g_Object[nIdxObject].nIdxModel].vtxMax.y;							// オブジェクトの位置を戻す
			pMove->y = 0.0f;																										// 落下速度を0.0に戻す
			*bHit = true;		// 当たったフラグを返す
		}
	}
}

//=============================================================================
// オブジェクトの半径の当たり判定
//=============================================================================
bool SphiaCollisionObject(D3DXVECTOR3 *pPos, float fRadius, int nIdxObject)
{
	bool bHit = false;

	// 当たり判定の座標の距離の計算
	float posX = (g_Object[nIdxObject].pos.x - pPos->x);	// (敵の体の当たり判定の座標－攻撃の当たり判定の座標)を2乗する
	float posZ = (g_Object[nIdxObject].pos.z - pPos->z);	// (敵の体の当たり判定の座標－攻撃の当たり判定の座標)を2乗する
	float fPosLenght = (posX * posX) + (posZ * posZ);		// それぞれを2乗して距離を計算

	// 当たり判定の半径の距離の計算
	float fRadiusLenght = (g_Object[nIdxObject].fRadius + fRadius) * (g_Object[nIdxObject].fRadius + fRadius);			// (ボックスの半径－攻撃の半径)を2乗して距離を計算

	// 座標の距離と半径の距離の比較
	if (fPosLenght  <  fRadiusLenght &&
		fPosLenght  > -fRadiusLenght )
	{// 半径の距離より、座標の距離のほうが小さかったら
		float fPosAngle = atan2f(pPos->x - g_Object[nIdxObject].pos.x,										// 敵とキャラの位置の角度を計算
								 pPos->z - g_Object[nIdxObject].pos.z);

		pPos->x = g_Object[nIdxObject].pos.x + (sinf(fPosAngle)*(g_Object[nIdxObject].fRadius + fRadius));	// キャラの位置をずらす
		pPos->z = g_Object[nIdxObject].pos.z + (cosf(fPosAngle)*(g_Object[nIdxObject].fRadius + fRadius));	// キャラの位置をずらす

		bHit = true;		// 当たったフラグを立てる
	}

	return bHit;
}

//=============================================================================
// オブジェクトの攻撃の当たり判定の処理
//=============================================================================
void CollisionObjectAttack(D3DXVECTOR3 *pPos, float fRadius)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{// モデルの数分ループ
		if (g_Object[nCntObject].bUse == true)
		{// オブジェクトが使われているとき

			// 距離の計算
			float posX = (g_Object[nCntObject].Collision.mtxWorld._41 - pPos->x);		// (オブジェクトの位置－攻撃と位置)を2乗する
			float posY = (g_Object[nCntObject].Collision.mtxWorld._42 - pPos->y);		// (オブジェクトの位置－攻撃と位置)を2乗する
			float posZ = (g_Object[nCntObject].Collision.mtxWorld._43 - pPos->z);		// (オブジェクトの位置－攻撃と位置)を2乗する
			float fPosLenght = (posX * posX) + (posY * posY) + (posZ * posZ);			// オブジェクトと攻撃の距離を計算

			// 半径の計算
			float fRadiusLenght = (g_Object[nCntObject].Collision.fRadius + fRadius) * (g_Object[nCntObject].Collision.fRadius + fRadius);			// (オブジェクトの半径－攻撃の半径)を2乗して距離を計算

			// 攻撃とオブジェクトの当たり判定
			if (fPosLenght  < fRadiusLenght &&
				fPosLenght  > -fRadiusLenght)
			{// オブジェクトと攻撃の距離よりも、オブジェクトの半径+弾の半径のほうが大きかったら
				g_Object[nCntObject].bUse = false;											// オブジェクトを削除
				//SetExplosion(D3DXVECTOR3(g_Object[nCntObject].Collision.mtxWorld._41,
				//						 g_Object[nCntObject].Collision.mtxWorld._42,
				//						 g_Object[nCntObject].Collision.mtxWorld._43));		// 爆発を設置

				//DeleteShadow(g_Object[nCntObject].nIdyShadow);								// 影を削除
			}
		}
	}
}


//=============================================================================
// オブジェクトの設定
//=============================================================================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTypeModel, int nCollisionType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの初期化
	D3DXMATRIX mtxRot, mtxTrans;					// 計算用マトリックス
	D3DXMATRIX mtxParent;							// 階層構造用のマトリックス

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{// モデルの数分ループ
		if (g_Object[nCntObject].bUse == false)
		{// モデルが使われていないとき
			g_Object[nCntObject].pos			= pos;								// 位置を設定
			g_Object[nCntObject].posOld			= pos;								// 前回位置を設定
			g_Object[nCntObject].rot			= rot;								// 向きを設定
			g_Object[nCntObject].nIdxModel		= nTypeModel;						// 使用モデルを設定
			g_Object[nCntObject].nCollisionType = nCollisionType;					// 当たり判定のタイプを設定

			// 半径の設定
			if (g_Object[nCntObject].nCollisionType != 0)
			{// 当たり判定のタイプが半径だった時
				g_Object[nCntObject].fRadius = g_ObjectModel[nTypeModel].vtxMax.x;	// Xの最大値を半径として設定
			}
			else
			{// 矩形
				g_Object[nCntObject].fRadius = 0.0f;								// 0.0fで設定
			}

			g_Object[nCntObject].bUse = true;																	// 使っている状態に設定

			//----------------------------------------------------------------------------
			// ワールドマトリクスの設定
			//----------------------------------------------------------------------------

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Object[nCntObject].mtxWorld);

			// 回転(向き)を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_Object[nCntObject].rot.y,
				g_Object[nCntObject].rot.x,
				g_Object[nCntObject].rot.z);								// ゲームを制作するときは一番使うＹ軸から入れる

			D3DXMatrixMultiply(&g_Object[nCntObject].mtxWorld,				// ２番目と３番目の引数を掛け算された値を一番目の引数に代入している
				&g_Object[nCntObject].mtxWorld,
				&mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_Object[nCntObject].pos.x,									// 位置はX座標から入れていく
				g_Object[nCntObject].pos.y,
				g_Object[nCntObject].pos.z);

			D3DXMatrixMultiply(&g_Object[nCntObject].mtxWorld,				// ２番目と３番目の引数を掛け算された値を一番目の引数に代入している
				&g_Object[nCntObject].mtxWorld,
				&mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD,
				&g_Object[nCntObject].mtxWorld);

			mtxParent = g_Object[nCntObject].mtxWorld;						// 親のワールドマトリックスを代入

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Object[nCntObject].Collision.mtxWorld);

			// 回転(向き)を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_Object[nCntObject].Collision.rot.y,
				g_Object[nCntObject].Collision.rot.x,
				g_Object[nCntObject].Collision.rot.z);									// ゲームを制作するときは一番使うＹ軸から入れる

			D3DXMatrixMultiply(&g_Object[nCntObject].Collision.mtxWorld,				// ２番目と３番目の引数を掛け算された値を一番目の引数に代入している
				&g_Object[nCntObject].Collision.mtxWorld,
				&mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_Object[nCntObject].Collision.pos.x,									// 位置はX座標から入れていく
				g_Object[nCntObject].Collision.pos.y,
				g_Object[nCntObject].Collision.pos.z);

			D3DXMatrixMultiply(&g_Object[nCntObject].Collision.mtxWorld,				// ２番目と３番目の引数を掛け算された値を一番目の引数に代入している
				&g_Object[nCntObject].Collision.mtxWorld,
				&mtxTrans);

			D3DXMatrixMultiply(&g_Object[nCntObject].Collision.mtxWorld,				// 親と子のマトリックスをかけ合わせる
				&g_Object[nCntObject].Collision.mtxWorld,
				&mtxParent);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Object[nCntObject].Collision.mtxWorld);

			break;
		}
	}
}

//=============================================================================
// オブジェクト情報の読み込み
//=============================================================================
void TextLoadObject(void)
{
	FILE *pFile = fopen(OPEN_FILE_NAME, "r");		// 読み込むテキストファイルのアドレス

	// 読み込むための変数宣言
	char acLine[MAX_CHAR];							// 1行丸ごと記憶するための変数
	char acData[MAX_CHAR];							// 一部の分を記憶するための変数

	int nCntModel = 0;								// 読み込むモデルをカウントする変数
	int nTypeModel;									// 使用するモデルの番号を入れる変数
	int nTypeCollision;								// 当たり判定のタイプを入れる変数
	D3DXVECTOR3 pos;								// 位置を入れる変数
	D3DXVECTOR3 rot;								// 向きを入れる変数

	if (pFile != NULL)
	{// ヌルチェック

		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// 文をデータにコピー

			if (strcmp(acData, "NUM_MODEL") == 0)
			{// NUM_MODELがあったら
				sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &g_nMaxModel);	// モデルの数を取得
			}
			else if (strcmp(acData, "MODEL_FILENAME") == 0)
			{// MODEL_FILENAMEがあったら
				sscanf(&acLine[0], "%s %s %s",
									&acData[0],
									&acData[0],
									&g_ObjectModel[nCntModel].cAddressModel[0]);	// モデルのアドレスを取得
				FileLodeObject(nCntModel);		// ファイルを読み込む
				MinMaxSearchObject(nCntModel);	// 最小最大を検索
				nCntModel++;					// カウントを進めて次に備える
			}
			else if (strcmp(acData, "MODELSET") == 0)
			{// MODELSETがあったら
				nTypeCollision = 0;										// 当たり判定のタイプを初期化

				while (strcmp(acData, "END_MODELSET") != 0)
				{// END_MODELSETが見つかるまでループする
					fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
					sscanf(&acLine[0], "%s", &acData);					// データをコピー

					if (strcmp(acData, "TYPE") == 0)
					{// TYPEがあったら
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nTypeModel);					// 使用するモデルの番号を取得
					}
					else if (strcmp(acData, "POS") == 0)
					{// POSがあったら
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &pos.x, &pos.y, &pos.z);	// 位置を取得
					}
					else if (strcmp(acData, "ROT") == 0)
					{// ROTがあったら
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &rot.x, &rot.y, &rot.z);	// 向きを取得
						rot.x = (D3DX_PI * 2 / 360 * rot.x);													// 3.14を2倍して360度で割って1度分の値を出して、テキストの角度にする
						rot.y = (D3DX_PI * 2 / 360 * rot.y);													// 3.14を2倍して360度で割って1度分の値を出して、テキストの角度にする
						rot.z = (D3DX_PI * 2 / 360 * rot.z);													// 3.14を2倍して360度で割って1度分の値を出して、テキストの角度にする
					}
					else if (strcmp(acData, "COLLISION_TYPE") == 0)
					{// ROTがあったら
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nTypeCollision);				// 当たり判定のタイプを取得
					}

				}

				// オブジェクトの設置
				SetObject(pos, rot, nTypeModel, nTypeCollision);
			}

		}
			fclose(pFile);								// 開いたファイルを閉じる
	}
}

//=============================================================================
// Xファイルの読み込み
//=============================================================================
void FileLodeObject(int nIdxObject)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();				// デバイスの取得
	D3DXMATERIAL *pMat;										// マテリアルデータへのポインタ

	// Xファイルの読み込み
	D3DXLoadMeshFromX(g_ObjectModel[nIdxObject].cAddressModel,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_ObjectModel[nIdxObject].pBuffMat,
		NULL,
		&g_ObjectModel[nIdxObject].nNumMat,
		&g_ObjectModel[nIdxObject].pMesh);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_ObjectModel[nIdxObject].pBuffMat->GetBufferPointer();

	g_ObjectModel[nIdxObject].pTexture = new LPDIRECT3DTEXTURE9[g_ObjectModel[nIdxObject].nNumMat];		// マテリアル分容量を増やす

	for (int nCntMat = 0; nCntMat < (int)g_ObjectModel[nIdxObject].nNumMat; nCntMat++)
	{// マテリアルの数分ループ
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// テクスチャのアドレスが存在していたら
			D3DXCreateTextureFromFile(pDevice,											// 3Dデバイスのアドレス
				pMat[nCntMat].pTextureFilename,					// 読み込むテクスチャのファイルのアドレス
				&g_ObjectModel[nIdxObject].pTexture[nCntMat]);		// テクスチャのポインタにアドレスに設定する
		}
		else
		{// テクスチャのアドレスがないなら
			g_ObjectModel[nIdxObject].pTexture[nCntMat] = NULL;							// NULLに設定する
		}
	}
}

//=============================================================================
// 幅や高さの最小最大検索
//=============================================================================
void MinMaxSearchObject(int nIdxObject)
{
	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = g_ObjectModel[nIdxObject].pMesh->GetNumVertices();

	// 頂点フォーマットを取得
	sizeFVF = D3DXGetFVFVertexSize(g_ObjectModel[nIdxObject].pMesh->GetFVF());

	// 頂点バッファをロック
	g_ObjectModel[nIdxObject].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{// 頂点数分ループ
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// 頂点座標の代入

														// 最小値と比較
		if (g_ObjectModel[nIdxObject].vtxMin.x > vtx.x)
		{// 今持っている最小値のxよりも小さい値だったら
			g_ObjectModel[nIdxObject].vtxMin.x = vtx.x;						// 最小値のxを代入
		}
		if (g_ObjectModel[nIdxObject].vtxMin.y > vtx.y)
		{// 今持っている最小値のyよりも小さい値だったら
			g_ObjectModel[nIdxObject].vtxMin.y = vtx.y;						// 最小値のyを代入
		}
		if (g_ObjectModel[nIdxObject].vtxMin.z > vtx.z)
		{// 今持っている最小値のyよりも小さい値だったら
			g_ObjectModel[nIdxObject].vtxMin.z = vtx.z;						// 最小値のzを代入
		}

		// 最大値と比較
		if (g_ObjectModel[nIdxObject].vtxMax.x < vtx.x)
		{// 今持っている最大値のxよりも大きい値だったら
			g_ObjectModel[nIdxObject].vtxMax.x = vtx.x;						// 最大値のxを代入
		}
		if (g_ObjectModel[nIdxObject].vtxMax.y < vtx.y)
		{// 今持っている最大値のxよりも大きい値だったら
			g_ObjectModel[nIdxObject].vtxMax.y = vtx.y;						// 最大値のyを代入
		}
		if (g_ObjectModel[nIdxObject].vtxMax.z < vtx.z)
		{// 今持っている最大値のxよりも大きい値だったら
			g_ObjectModel[nIdxObject].vtxMax.z = vtx.z;						// 最大値のzを代入
		}

		pVtxBuff += sizeFVF;												// ポインタをサイズ分進める
	}

	// 頂点バッファをアンロック
	g_ObjectModel[nIdxObject].pMesh->UnlockVertexBuffer();

}

//=============================================================================
// オブジェクトの前回位置の保存
//=============================================================================
void SetPosOldObject(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{// オブジェクトの総数分ループ
		if (g_Object[nCntObject].bUse == true)
		{
			g_Object[nCntObject].posOld = g_Object[nCntObject].pos;		// 前回の位置の保存
		}
	}
}

//=============================================================================
// オブジェクト情報の取得
//=============================================================================
Object *GetObjectModel(void)
{
	return &g_Object[0];
}