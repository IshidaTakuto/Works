//=============================================================================
//
// メッシュフィールド処理 [meshField.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "meshField.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESH_X			(6)			// x軸方向のマス数
#define MESH_Z			(6)			// z軸方向のマス数
#define MESH_WIDTH		(400.0f)	// 幅
#define MESH_DEPTH		(400.0f)	// 奥行
#define MAX_POLYGON		((MESH_X * MESH_Z * 2) + (4 * (MESH_Z - 1)))		// ポリゴン数

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//==================================
// 生成処理
//==================================
CMeshField* CMeshField::Create(void)
{
	CMeshField *pMeshField = NULL;

	pMeshField = new CMeshField;	// メモリを確保

	if (NULL != pMeshField)
	{// メモリ確保成功
		pMeshField->Init();
	}

	return pMeshField;
}

//=========================================
// コンストラクタ
//=========================================
CMeshField::CMeshField()
{
	m_nWire = 0;
	m_nType = 0;
}

//=========================================
// デストラクタ
//=========================================
CMeshField::~CMeshField()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshField::Init(void)
{
	CScene3DIndex::SetObjType(CScene::OBJTYPE_FIELD);
	CScene3DIndex::Init();
	CScene3DIndex::MakeVertex();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMeshField::Uninit(void)
{
	CScene3DIndex::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMeshField::Update(void)
{
	CScene3DIndex::Update();

	// キーボードの入力を取得
	CInputKeyboard *pKeyboard = CManager::GetInput();

	if (pKeyboard->GetTrigger(DIK_F8) == true)
	{// ワイヤーフレーム表示
		m_nWire = (2 + m_nWire) % 4;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshField::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ワイヤーフレームを表示
	pDevice->SetRenderState(D3DRS_FILLMODE, m_nWire);

	CScene3DIndex::Draw();

	// ワイヤーフレームを非表示
	pDevice->SetRenderState(D3DRS_FILLMODE, 0);
}

//=========================================================================================================================
// 法線の取得
//=========================================================================================================================
D3DXVECTOR3 CMeshField::GetNor(D3DXVECTOR3 pos)
{
	float fDiff = 0.0f;		// 判定用にY軸下げた分を保存
	int nPosBrock[2];		// プレイヤーのいるポリゴンのブロック
	int nVtxNumber[4];		// ブロックの頂点番号

	// プレイヤーのベクトルを求める
	D3DXVECTOR3 vecPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fWidth, fDepth;	// 頂点間の距離
	int nMeshX, nMeshZ;		// 分割数

	// データの取得
	CScene3DIndex::GetInfo(&fWidth, NULL, &fDepth, &nMeshX, NULL, &nMeshZ);

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

	//頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 vecA, vecB, collisionNor, collisionPos[3];

	// プレイヤーのいるポリゴン判定
	nPosBrock[0] = (int)(pos.x + (fWidth * nMeshX * 0.5f)) / (int)fWidth;		// X軸のブロック割り出し
	nPosBrock[1] = (int)(pos.z - (fDepth * nMeshZ * 0.5f)) / (int)fDepth * -1;	// Z軸のブロック割り出し

	if (nPosBrock[0] >= nMeshX || (int)(pos.x + (fWidth * nMeshX * 0.5f)) < 0 ||
		nPosBrock[1] >= nMeshZ || (int)(pos.z - (fDepth * nMeshZ * 0.5f)) > 0)
	{// ポリゴンに乗っていないとき
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// 頂点番号を計算
	nVtxNumber[0] = nPosBrock[0] + ((nPosBrock[1] + 1) * (nMeshX + 1));			// 自分のブロックの左下番号
	nVtxNumber[1] = nPosBrock[0] + (nPosBrock[1] * (nMeshX + 1));					// 自分のブロックの左上番号
	nVtxNumber[2] = (nPosBrock[0] + 1) + ((nPosBrock[1] + 1) * (nMeshX + 1));		// 自分のブロックの右下番号
	nVtxNumber[3] = (nPosBrock[0] + 1) + (nPosBrock[1] * (nMeshX + 1));			// 自分のブロックの右上番号

																				// 外積で左右どちらのポリゴンか判別
	vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[1]].pos;	// 頂点から目的の頂点への方向
	vecB = pos - pVtx[nVtxNumber[1]].pos;						// プレイヤー　－　vecAの始まりの頂点

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) >= 0)
	{// 左側
	 // 判定用頂点座標
		collisionPos[0] = pVtx[nVtxNumber[0]].pos;
		collisionPos[1] = pVtx[nVtxNumber[1]].pos;
		collisionPos[2] = pVtx[nVtxNumber[2]].pos;
	}
	else
	{// 右側
	 // 判定用頂点座標
		collisionPos[0] = pVtx[nVtxNumber[3]].pos;
		collisionPos[1] = pVtx[nVtxNumber[2]].pos;
		collisionPos[2] = pVtx[nVtxNumber[1]].pos;
	}

	// 判定用法線
	vecA = collisionPos[1] - collisionPos[0];	// 左側
	vecB = collisionPos[2] - collisionPos[0];	// 右側
	D3DXVec3Cross(&collisionNor, &vecA, &vecB);			// 外積を求める
	D3DXVec3Normalize(&collisionNor, &collisionNor);	// 正規化

	// 頂点バッファをアンロック
	vtxBuff->Unlock();

	return collisionNor;
}

//=========================================================================================================================
// 傾斜による移動量の取得
//=========================================================================================================================
float CMeshField::GetAngle(D3DXVECTOR3 pos, float fRot)
{
	float fSpeed = 0.0f;	// 返す速度
	int nPosBrock[2];		// プレイヤーのいるポリゴンのブロック
	int nVtxNumber[4];		// ブロックの頂点番号
	D3DXVECTOR3 vecPlayer;	// プレイヤーの向きのベクトル
	float fAngle;			// プレイヤーの向きと法線のなす角

	float fWidth, fDepth;	// 頂点間の距離
	int nMeshX, nMeshZ;		// 分割数

	// データの取得
	CScene3DIndex::GetInfo(&fWidth, NULL, &fDepth, &nMeshX, NULL, &nMeshZ);

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

	//頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 vecA, vecB, collisionNor, collisionPos[3];

	// プレイヤーのいるポリゴン判定
	nPosBrock[0] = (int)(pos.x + (fWidth * nMeshX * 0.5f)) / (int)fWidth;		// X軸のブロック割り出し
	nPosBrock[1] = (int)(pos.z - (fDepth * nMeshZ * 0.5f)) / (int)fDepth * -1;	// Z軸のブロック割り出し

	if (nPosBrock[0] >= nMeshX || (int)(pos.x + (fWidth * nMeshX * 0.5f)) < 0 ||
		nPosBrock[1] >= nMeshZ || (int)(pos.z - (fDepth * nMeshZ * 0.5f)) > 0)
	{// ポリゴンに乗っていないとき
		return 0;
	}

	// 頂点番号を計算
	nVtxNumber[0] = nPosBrock[0] + ((nPosBrock[1] + 1) * (nMeshX + 1));			// 自分のブロックの左下番号
	nVtxNumber[1] = nPosBrock[0] + (nPosBrock[1] * (nMeshX + 1));					// 自分のブロックの左上番号
	nVtxNumber[2] = (nPosBrock[0] + 1) + ((nPosBrock[1] + 1) * (nMeshX + 1));		// 自分のブロックの右下番号
	nVtxNumber[3] = (nPosBrock[0] + 1) + (nPosBrock[1] * (nMeshX + 1));			// 自分のブロックの右上番号

																				// 外積で左右どちらのポリゴンか判別
	vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[1]].pos;	// 頂点から目的の頂点への方向
	vecB = pos - pVtx[nVtxNumber[1]].pos;						// プレイヤー　－　vecAの始まりの頂点

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) >= 0)
	{// 左側
	 // 判定用頂点座標
		collisionPos[0] = pVtx[nVtxNumber[0]].pos;
		collisionPos[1] = pVtx[nVtxNumber[1]].pos;
		collisionPos[2] = pVtx[nVtxNumber[2]].pos;
	}
	else
	{// 右側
	 // 判定用頂点座標
		collisionPos[0] = pVtx[nVtxNumber[3]].pos;
		collisionPos[1] = pVtx[nVtxNumber[2]].pos;
		collisionPos[2] = pVtx[nVtxNumber[1]].pos;
	}

	// プレイヤーのいるポリゴンの法線を出す
	vecA = collisionPos[1] - collisionPos[0];	// 左側
	vecB = collisionPos[2] - collisionPos[0];	// 右側
	D3DXVec3Cross(&collisionNor, &vecA, &vecB);			// 外積を求める
	D3DXVec3Normalize(&collisionNor, &collisionNor);	// 正規化

														// プレイヤーの向きのベクトルの生成
	vecPlayer = D3DXVECTOR3(pos.x + sinf(fRot), pos.y, pos.z + cosf(fRot)) - pos;

	// プレイヤーの向きと法線のなす角を求める
	fAngle = (vecPlayer.x * collisionNor.x) + (vecPlayer.y * collisionNor.y) + (vecPlayer.z * collisionNor.z);
	fAngle = fAngle / (sqrtf(powf(vecPlayer.x, 2) + powf(vecPlayer.y, 2) + powf(vecPlayer.z, 2)) * (sqrtf(powf(collisionNor.x, 2) + powf(collisionNor.y, 2) + powf(collisionNor.z, 2))));

	return fAngle;
}

//=========================================================================================================================
// 滑る向きの取得
//=========================================================================================================================
float CMeshField::GetSlipAngle(D3DXVECTOR3 pos)
{
	int nPosBrock[2];		// プレイヤーのいるポリゴンのブロック
	int nVtxNumber[4];		// ブロックの頂点番号
	D3DXVECTOR3 vecPlayer;	// プレイヤーの向きのベクトル
	float fAngle;			// プレイヤーの向きと法線のなす角
	D3DXVECTOR3 vecA, vecB, collisionNor;	// 計算用ベクトル

	float fWidth, fDepth;	// 頂点間の距離
	int nMeshX, nMeshZ;		// 分割数

							// データの取得
	CScene3DIndex::GetInfo(&fWidth, NULL, &fDepth, &nMeshX, NULL, &nMeshZ);

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

						// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

	//頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// プレイヤーのいるポリゴン判定
	nPosBrock[0] = (int)(pos.x + (fWidth * nMeshX * 0.5f)) / (int)fWidth;		// X軸のブロック割り出し
	nPosBrock[1] = (int)(pos.z - (fDepth * nMeshZ * 0.5f)) / (int)fDepth * -1;	// Z軸のブロック割り出し

	if (nPosBrock[0] >= nMeshX || (int)(pos.x + (fWidth * nMeshX * 0.5f)) < 0 ||
		nPosBrock[1] >= nMeshZ || (int)(pos.z - (fDepth * nMeshZ * 0.5f)) > 0)
	{// ポリゴンに乗っていないとき
		return 0;
	}

	// 頂点番号を計算
	nVtxNumber[0] = nPosBrock[0] + ((nPosBrock[1] + 1) * (nMeshX + 1));			// 自分のブロックの左下番号
	nVtxNumber[1] = nPosBrock[0] + (nPosBrock[1] * (nMeshX + 1));					// 自分のブロックの左上番号
	nVtxNumber[2] = (nPosBrock[0] + 1) + ((nPosBrock[1] + 1) * (nMeshX + 1));		// 自分のブロックの右下番号
	nVtxNumber[3] = (nPosBrock[0] + 1) + (nPosBrock[1] * (nMeshX + 1));			// 自分のブロックの右上番号

																				// 外積で左右どちらのポリゴンか判別
	vecA = pVtx[nVtxNumber[2]].pos - pVtx[nVtxNumber[1]].pos;	// 頂点から目的の頂点への方向
	vecB = pos - pVtx[nVtxNumber[1]].pos;						// プレイヤー　－　vecAの始まりの頂点

	if ((vecA.z * vecB.x) - (vecA.x * vecB.z) >= 0)
	{// 左側
		collisionNor = pVtx[nVtxNumber[0]].nor;	// 頂点の法線を保存
	}
	else
	{// 右側
		collisionNor = pVtx[nVtxNumber[3]].nor;	// 頂点の法線を保存
	}

	// ベクトルの角度を取得
	fAngle = acosf(collisionNor.x / sqrtf((collisionNor.x * collisionNor.x) + (collisionNor.y * collisionNor.y)));
	if (collisionNor.y < 0)
	{// -だった時
		fAngle = 2.0f * D3DX_PI - fAngle;
	}

	if (isnan(fAngle))
	{// nanチェック
		fAngle = 0.0f;
	}

	// 一回転しないように角度を調節
	if (fAngle > D3DX_PI)
	{
		fAngle -= D3DX_PI * 2.0f;
	}
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	}

	return fAngle;
}

//=========================================================================================================================
// 範囲内の地面の色の設定
//=========================================================================================================================
void CMeshField::RangeColor(D3DXVECTOR3 pos, float fRange)
{
	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = CScene3DIndex::GetVtxBuff();

	//頂点バッファをロックし、頂点データへのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nNumVtx = CScene3DIndex::GetNumVtx();
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{// 全頂点の距離を計算
		float fLength = powf(pVtx[nCntVtx].pos.x - pos.x, 2.0f) + powf(pVtx[nCntVtx].pos.z - pos.z, 2.0f);

		if (fLength <= powf(fRange, 2.0f))
		{// 範囲内にある頂点
			float fCol = 1.0f - cosf((D3DX_PI * 0.4f) / (fRange / sqrtf(fLength)));

			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, fCol, fCol, 1.0f);
		}
		else
		{// 範囲外
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	// 頂点バッファをアンロック
	vtxBuff->Unlock();
}