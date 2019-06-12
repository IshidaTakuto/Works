//=========================================================================================================================
//
// 投影処理 [matrixShadow.cpp]
// Author :
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "matrixShadow.h"
#include "light.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************

//=========================================================================================================================
// 初期化処理
//=========================================================================================================================
void InitmatrixShadow(void)
{

}

//=========================================================================================================================
// 終了処理
//=========================================================================================================================
void UninitmatrixShadow(void)
{

}

//=========================================================================================================================
// 更新処理
//=========================================================================================================================
void UpdatematrixShadow(void)
{

}

//=========================================================================================================================
// 描画処理
//=========================================================================================================================
void DrawmatrixShadow(void)
{

}

//=========================================================================================================================
// 影設定処理
//=========================================================================================================================
void SetmatrixShadow(D3DXMATRIX &mtxWorld, LPD3DXMESH &pMesh, DWORD &nNumMat, D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DLIGHT9 Light = GetLight();
	D3DXVECTOR4 vLightDirection = D3DXVECTOR4(Light.Direction, 0.0f);

	//影用の射影マトリクスを作成
	D3DXMATRIX mmatrixShadow;
	D3DXVECTOR3 vPoint = pos;
	D3DXVECTOR3 vNormal(0.0f, -1.0f, 0.0f);
	D3DXPLANE GroundPlane;
	D3DXPlaneFromPointNormal(&GroundPlane, &vPoint, &vNormal);
	D3DXMatrixShadow(&mmatrixShadow, &vLightDirection, &GroundPlane);

	mmatrixShadow = mtxWorld * mmatrixShadow;

	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//影用マテリアルの設定
	D3DMATERIAL9 Material;
	ZeroMemory(&Material, sizeof(D3DMATERIAL9));
	Material.Emissive.a = 1.0f;
	Material.Emissive.r = 0.1f;
	Material.Emissive.g = 0.1f;
	Material.Emissive.b = 0.1f;

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);			// αブレンドを停止

	//影としてレンダリング
	pDevice->SetTransform(D3DTS_WORLD, &mmatrixShadow);
	pDevice->SetMaterial(&Material);
	for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
	{
		pMesh->DrawSubset(nCntMat);
	}
	pDevice->SetMaterial(&matDef);				// 現在のマテリアル戻す

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// 影を投影する位置を調べる
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
D3DXVECTOR3 CheckShadowPosition(D3DXVECTOR3 pos)
{
	// 一時保存用
	D3DXVECTOR3 position = D3DXVECTOR3(0.0f, -100000.0f, 0.0f);		// 位置
	float fRange = 1000000.0f;										// 距離

#if 0
	// オブジェクト情報を取得
	Stage_objInfo *pStage_objInfo = GetStage_objInfo();
	// オブジェクトを取得
	Stage_obj *pStage_obj = GetStage_obj();
	// オブジェクトの総数を取得
	int nNumStage_obj = GetNumStage_obj();

	for (int nCntObj = 0; nCntObj < nNumStage_obj; nCntObj++, pStage_obj++)
	{// オブジェクトの総数
		if (pos.y >= pStage_obj->pos.y + (pStage_objInfo + pStage_obj->nType)->vtxMax.y)
		{// プレイヤーの位置[Y]よりも低い位置にある場合
		 // プレイヤーの位置[X,Z]により近いオブジェクトを調べる
			float fRangeX = pos.x - pStage_obj->pos.x;
			float fRangeZ = pos.z - pStage_obj->pos.z;
			float fNowRange = fRangeX * fRangeX + fRangeZ * fRangeZ;

			if (fRange > fNowRange)
			{// 最も近い位置のオブジェクトの位置を代入
				position.x = pStage_obj->pos.x;
				position.z = pStage_obj->pos.z;
				position.y = pStage_obj->pos.y + (pStage_objInfo + pStage_obj->nType)->vtxMax.y;
			}
		}
	}
#endif

	return position;
}