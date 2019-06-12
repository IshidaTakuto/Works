//*****************************************************************************
//
//     配置物の処理[object.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "input.h"
#include "debuglog.h"

#include "player.h"
#include "shadow.h"
#include "debuglog.h"
#include "balloon.h"
#include "particle.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
// 動く配置物用
#define OBJECT_MOVE_TIMING            (180)

// 吹き出しポリゴンを出す位置
#define OBJECT_BALLOON_POLYGON_WIDTH  (300.0f)
#define OBJECT_BALLOON_POLYGON_HEIGHT (130.0f)
#define OBJECT_BALLOON_POLYGON_POS    (D3DXVECTOR3((SCREEN_WIDTH / 2),OBJECT_BALLOON_POLYGON_HEIGHT,0.0f))
#define OBJECT_BALLOON_RANGE          (100.0f)

#define OBJECT_MODEL_FILENAME   "data/MODEL/object/object.x"  // 配置物モデルのファイル名

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************
LPD3DXMESH     CObject::m_apMesh = NULL;                                                     // メッシュ情報へのポインタ
LPD3DXBUFFER   CObject::m_apBuffMat = NULL;                                                  // マテリアル情報へのポインタ
DWORD          CObject::m_anNumMat = NULL;                                                   // マテリアル情報の数
D3DXVECTOR3    CObject::m_aVtxMin = D3DXVECTOR3(100000.0f, 1000000.0f, 100000.0f);           // 頂点座標の最小値
D3DXVECTOR3    CObject::m_aVtxMax = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f);        // 頂点座標の最小値


LPD3DXMESH     CObjectMove::m_apMesh = NULL;                                                 // メッシュ情報へのポインタ
LPD3DXBUFFER   CObjectMove::m_apBuffMat = NULL;                                              // マテリアル情報へのポインタ
DWORD          CObjectMove::m_anNumMat = NULL;                                               // マテリアル情報の数
D3DXVECTOR3    CObjectMove::m_aVtxMin = D3DXVECTOR3(100000.0f, 1000000.0f, 100000.0f);       // 頂点座標の最小値
D3DXVECTOR3    CObjectMove::m_aVtxMax = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f);    // 頂点座標の最小値

LPD3DXMESH     CObjectBalloon::m_apMesh = NULL;                                              // メッシュ情報へのポインタ
LPD3DXBUFFER   CObjectBalloon::m_apBuffMat = NULL;                                           // マテリアル情報へのポインタ
DWORD          CObjectBalloon::m_anNumMat = NULL;                                            // マテリアル情報の数
D3DXVECTOR3    CObjectBalloon::m_aVtxMin = D3DXVECTOR3(100000.0f, 1000000.0f, 100000.0f);    // 頂点座標の最小値
D3DXVECTOR3    CObjectBalloon::m_aVtxMax = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f); // 頂点座標の最小値

LPD3DXMESH     CObjectGoal::m_apMesh = NULL;                                                 // メッシュ情報へのポインタ
LPD3DXBUFFER   CObjectGoal::m_apBuffMat = NULL;                                              // マテリアル情報へのポインタ
DWORD          CObjectGoal::m_anNumMat = NULL;                                               // マテリアル情報の数
D3DXVECTOR3    CObjectGoal::m_aVtxMin = D3DXVECTOR3(100000.0f, 1000000.0f, 100000.0f);       // 頂点座標の最小値
D3DXVECTOR3    CObjectGoal::m_aVtxMax = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f);    // 頂点座標の最小値

LPD3DXMESH     CObjectTarget::m_apMesh = NULL;                                               // メッシュ情報へのポインタ
LPD3DXBUFFER   CObjectTarget::m_apBuffMat = NULL;                                            // マテリアル情報へのポインタ
DWORD          CObjectTarget::m_anNumMat = NULL;                                             // マテリアル情報の数
D3DXVECTOR3    CObjectTarget::m_aVtxMin = D3DXVECTOR3(100000.0f, 1000000.0f, 100000.0f);     // 頂点座標の最小値
D3DXVECTOR3    CObjectTarget::m_aVtxMax = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f);  // 頂点座標の最小値

//*****************************************************************************
//     CObjecctの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CObject::CObject(int nPriority, OBJTYPE objType) : CSceneX(nPriority, objType)
{
	// 各種値のクリア
	m_pShadow = NULL;    // 影クラスへのポインタ
	m_bCollision = true; // 当たり判定を取るかどうか
	m_bBreak = false;    // 壊せるかどうか
	m_bPush = false;     // 押せるかどうか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CObject::~CObject()
{

}

//=============================================================================
//    配置物の生成処理
//=============================================================================
CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, bool bPush, bool bBreak, int nPriority)
{
	CObject *pObject = NULL;   // 配置物クラス型のポインタ
	if (pObject == NULL)
	{// メモリが空になっている
		pObject = new CObject(nPriority);
		if (pObject != NULL)
		{// インスタンスを生成できた
			if (FAILED(pObject->Init(pos, rot, RollSpeed, bCollision, bPush, bBreak)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pObject;   // インスタンスのアドレスを返す
}

//=============================================================================
//    配置物の初期化処理
//=============================================================================
HRESULT CObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, bool bPush, bool bBreak)
{
	// 各種値の設定
	SetPos(pos);                // 現在の座標
	SetRot(rot);                // 現在の向き
	SetVtxMin(m_aVtxMin);       // 頂点座標の最小値
	SetVtxMax(m_aVtxMax);       // 頂点座標の最大値
	m_RollSpeed = RollSpeed;    // 回転するスピード
	m_bCollision = bCollision;  // 当たり判定を取るかどうか
	m_bPush = bPush;            // 押せるかどうか
	m_bBreak = bBreak;          // 壊せるかどうか

	// 共通の初期化処理
	CSceneX::Init();

	// 影の生成
	if (m_pShadow == NULL)
	{// メモリが確保できる状態である
		m_pShadow = CShadow::Create(D3DXVECTOR3(pos.x, 0.0f, pos.z), rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f), 70.0f, 4);
	}

	return S_OK;
}

//=============================================================================
//    配置物の終了処理
//=============================================================================
void CObject::Uninit(void)
{
	// 影の終了処理
	if (m_pShadow != NULL)
	{// メモリが確保されている
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	// 共通の終了処理
	CSceneX::Uninit();
}

//=============================================================================
//    配置物の更新処理
//=============================================================================
void CObject::Update(void)
{
	D3DXVECTOR3 rot = GetRot();

	rot.y += m_RollSpeed.y;
	if (rot.y > D3DX_PI)
	{// 向きが円周率を超えた
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{// 向きが円周率を超えた
		rot.y += D3DX_PI * 2.0f;
	}

	SetRot(rot);
}

//=============================================================================
//    配置物の描画処理
//=============================================================================
void CObject::Draw(void)
{
	// 共通の描画処理
	CSceneX::Draw();
}

//=============================================================================
//    配置物の当たり判定処理
//=============================================================================
bool CObject::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, CShadow *pShadow, float fHeight, float fRange)
{
	bool bLand = false;   // オブジェクトに乗っているかどうか

	// 当たり判定に必要な値を配置物から取得
	D3DXVECTOR3 pos = GetPos();          // 現在の座標
	D3DXVECTOR3 posOld = GetPosOld();    // 前回の座標
	D3DXVECTOR3 Rot = GetRot();          // 現在の向き
	D3DXVECTOR3 VtxMin = GetVtxMin();    // 頂点の座標最小値
	D3DXVECTOR3 VtxMax = GetVtxMax();    // 頂点の座標最大値

	// 外積の判定に必要な値を用意
	D3DXVECTOR3 ObjectCollisionPos[4] =
	{// 当たり判定用の四角(ローカル座標)
		D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMax.z + fRange),
		D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMax.z + fRange),
		D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMin.z - fRange),
		D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMin.z - fRange),
	};

	D3DXVECTOR3 ObjectCollisionWorldPos[4];  // 当たり判定用の四角(ワールド座標)
	D3DXMATRIX ObjectCollisionMtxWorld;      // 計算用マトリックス

	for (int nCntCol = 0; nCntCol < 4; nCntCol++)
	{// 判定する四角の座標分繰り返し
		D3DXMATRIX ObjectCollisionMtxWorld, mtxRot, mtxTrans;  // 計算用マトリックス

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&ObjectCollisionMtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, ObjectCollisionPos[nCntCol].x, ObjectCollisionPos[nCntCol].y, ObjectCollisionPos[nCntCol].z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxTrans);

		// 親の情報を掛け合わせる
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &GetMtxWorld());

		// ワールド座標を抜き出す
		ObjectCollisionWorldPos[nCntCol] = D3DXVECTOR3(ObjectCollisionMtxWorld._41, ObjectCollisionMtxWorld._42, ObjectCollisionMtxWorld._43);
	}

	D3DXVECTOR3 vecA, vecB;        // 判定用ベクトル
	int nCntVecTrue = 0;           // ベクトルの判定を通った回数
	bool bObjectRand = false;      // オブジェクトの中にいるかどうか
	for (int nCntVec = 0; nCntVec < 4; nCntVec++)
	{// 判定するベクトルの数だけ繰り返し
		vecA = ObjectCollisionWorldPos[(nCntVec + 1) % 4] - ObjectCollisionWorldPos[nCntVec];
		vecB = *pPos - ObjectCollisionWorldPos[nCntVec];

		if ((vecA.x * vecB.z) - (vecA.z * vecB.x) <= 0)
		{// ベクトルの右側にいる
			nCntVecTrue++;   // 判定を通った回数を加算
			if (nCntVecTrue == 4)
			{// 全てのベクトルの右側にいる
				bObjectRand = true;
			}
		}
	}

	if (bObjectRand == true)
	{// 配置物の中にいる
	    // 影の位置をずらす
		if (pShadow != NULL)
		{// 影が生成されている
			if (pPos->y >= GetPos().y + VtxMax.y || pPosOld->y >= GetPos().y + VtxMax.y)
			{// 配置物の上にいる
				// 位置をずらす
				pShadow->SetPos(D3DXVECTOR3(pPos->x, pos.y + VtxMax.y + 1.0f, pPos->z));

				// 頂点情報の設定
				VERTEX_3D *pVtx;
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = pShadow->GetVtxBuff();
				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				for (int nCntVer = 0; nCntVer < 4; nCntVer++)
				{// 影の頂点数分繰り返し
					pVtx[nCntVer].pos.y = 0.0f; // 配置物の上に影を乗せるのでオフセット高さは0に
				}

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();

				// 頂点バッファの設定
				pShadow->SetVtxBuff(pVtxBuff);
			}
		}

		// 乗っているかどうかの判定開始
		if (pPosOld->y >= pos.y + VtxMax.y && pPos->y <= pos.y + VtxMax.y)
		{// 配置物に上から入った
			pPos->y = pos.y + VtxMax.y;
			pMove->y = 0.0f;
			bLand = true;
		}
		else if (pPosOld->y + fHeight <= pos.y + VtxMin.y && pPos->y + fHeight > pos.y + VtxMin.y)
		{// 配置物に下から入った
			pPos->y = pos.y + VtxMin.y - fHeight;
			pMove->y = 0.0f;
		}
	}

	// 各種値の設定
	SetPos(pos);   // 現在の座標

	return bLand;   // 乗っているかどうかを返す
}

//=============================================================================
//    配置物に当たっているかどうか判定する処理
//=============================================================================
bool CObject::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange)
{
	bool bCol = false;  // 当たっているかどうか

	// 当たり判定に必要な値を配置物から取得
	D3DXVECTOR3 pos = GetPos();          // 現在の座標
	D3DXVECTOR3 posOld = GetPosOld();    // 前回の座標
	D3DXVECTOR3 Rot = GetRot();          // 現在の向き
	D3DXVECTOR3 VtxMin = GetVtxMin();    // 頂点の座標最小値
	D3DXVECTOR3 VtxMax = GetVtxMax();    // 頂点の座標最大値

	// 外積の判定に必要な値を用意
	D3DXVECTOR3 ObjectCollisionPos[4] =
	{// 当たり判定用の四角(ローカル座標)
		D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMax.z + fRange),
		D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMax.z + fRange),
		D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMin.z - fRange),
		D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMin.z - fRange),
	};

	D3DXVECTOR3 ObjectCollisionWorldPos[4];  // 当たり判定用の四角(ワールド座標)
	D3DXMATRIX ObjectCollisionMtxWorld;      // 計算用マトリックス

	for (int nCntCol = 0; nCntCol < 4; nCntCol++)
	{// 判定する四角の座標分繰り返し
		D3DXMATRIX ObjectCollisionMtxWorld, mtxRot, mtxTrans;  // 計算用マトリックス

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&ObjectCollisionMtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, ObjectCollisionPos[nCntCol].x, ObjectCollisionPos[nCntCol].y, ObjectCollisionPos[nCntCol].z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxTrans);

		// 親の情報を掛け合わせる
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &GetMtxWorld());

		// ワールド座標を抜き出す
		ObjectCollisionWorldPos[nCntCol] = D3DXVECTOR3(ObjectCollisionMtxWorld._41, ObjectCollisionMtxWorld._42, ObjectCollisionMtxWorld._43);
	}


	D3DXVECTOR3 vecA, vecB;        // 判定用ベクトル
	int nCntVecTrue = 0;           // ベクトルの判定を通った回数
	bool bObjectRand = false;      // オブジェクトの中にいるかどうか
	for (int nCntVec = 0; nCntVec < 4; nCntVec++)
	{// 判定するベクトルの数だけ繰り返し
		vecA = ObjectCollisionWorldPos[(nCntVec + 1) % 4] - ObjectCollisionWorldPos[nCntVec];
		vecB = *pPos - ObjectCollisionWorldPos[nCntVec];

		if ((vecA.x * vecB.z) - (vecA.z * vecB.x) <= 0)
		{// ベクトルの右側にいる
			nCntVecTrue++;   // 判定を通った回数を加算
			if (nCntVecTrue == 4)
			{// 全てのベクトルの右側にいる
				bObjectRand = true;
			}
		}
	}

	if (bObjectRand == true)
	{// ベクトルの範囲内にいる
		if (pPos->y <= pos.y + VtxMax.y && pPos->y + fHeight >= pos.y + VtxMin.y)
		{// オブジェクトのY軸の間にいる
			if (Rot.y != 0.0f)
			{// 向きが回転していない(矩形で判定を取る)
				for (int nCntVec = 0; nCntVec < 4; nCntVec++)
				{// 判定するベクトルの数だけ繰り返し
					vecA = ObjectCollisionWorldPos[(nCntVec + 1) % 4] - ObjectCollisionWorldPos[nCntVec];
					vecB = *pPosOld - ObjectCollisionWorldPos[nCntVec];

					if ((vecA.x * vecB.z) - (vecA.z * vecB.x) >= -1)
					{// ベクトルの左側にいる
						if (m_bPush == true)
						{// 押せるオブジェクトである
							pos.x += pMove->x;
							pos.z += pMove->z;
						}
						// 前回の座標を利用して配置物とのベクトルをしき、左にいるベクトルの交点を求める
						D3DXVECTOR3 Pos1 = *pPosOld;                                    // プレイヤーの前回の座標
						D3DXVECTOR3 Pos2 = pos;                                         // 配置物の座標
						D3DXVECTOR3 Pos3 = ObjectCollisionWorldPos[(nCntVec + 1) % 4];  // ベクトル判定に引っかかった頂点座標(1つ目)
						D3DXVECTOR3 Pos4 = ObjectCollisionWorldPos[nCntVec];            // ベクトル判定に引っかかった頂点座標(2つ目)

						// 直線の方程式から連立方程式を組み、XZ平面上の交点を求める
						float VecLine1 = (Pos2.z - Pos1.z) / (Pos2.x - Pos1.x);
						float VecLine2 = (Pos4.z - Pos3.z) / (Pos4.x - Pos3.x);
						float XPos = (VecLine1 * Pos1.x - Pos1.z - VecLine2 * Pos3.x + Pos3.z) / (VecLine1 - VecLine2);
						float ZPos = (Pos2.z - Pos1.z) / (Pos2.x - Pos1.x) * (XPos - Pos1.x) + Pos1.z;
						pPos->x = XPos;
						pPos->z = ZPos;
						bCol = true;
					}
				}
			}

			if (Rot.y == 0.0f)
			{// 向きが回転していない(矩形で判定を取る)
				if (pPos->z - fRange <= pos.z + VtxMax.z && pPos->z + fRange >= pos.z + VtxMin.z)
				{// オブジェクトのZ軸の間にいる
					if (pPosOld->x + fRange <= pos.x + VtxMin.x && pPos->x + fRange >= pos.x + VtxMin.x)
					{// 左からあたった
						if (m_bPush == true)
						{// 押せるオブジェクトである
							float Move = 0.0f;
							if (pMove->y <= 0.0f)
							{// ジャンプしていない
								Move = pMove->x;
							}
							pPos->x = pos.x + VtxMin.x - fRange + Move;
							if (pMove->y <= 0.0f)
							{// ジャンプしていない
								Move = pMove->x * 1.1f;
							}
							pos.x += Move;
						}
						else
						{// 押せないオブジェクトである
							pPos->x = pos.x + VtxMin.x - fRange;
						}
						bCol = true;
					}
					else if (pPosOld->x - fRange >= pos.x + VtxMax.x && pPos->x - fRange <= pos.x + VtxMax.x)
					{// 右からあたった
						if (m_bPush == true)
						{// 押せるオブジェクトである
							float Move = 0.0f;
							if (pMove->y <= 0.0f)
							{// ジャンプしていない
								Move = pMove->x;
							}
							pPos->x = pos.x + VtxMax.x + fRange + Move;
							if (pMove->y <= 0.0f)
							{// ジャンプしていない
								Move = pMove->x * 1.1f;
							}
							pos.x += Move;
						}
						else
						{// 押せないオブジェクトである
							pPos->x = pos.x + VtxMax.x + fRange;
						}
						bCol = true;
					}
				}
				if (pPos->x - fRange <= pos.x + VtxMax.x && pPos->x + fRange >= pos.x + VtxMin.x)
				{// オブジェクトのX軸の間にいる
					if (pPosOld->z + fRange <= pos.z + VtxMin.z && pPos->z + fRange >= pos.z + VtxMin.z)
					{// 後ろからあたった
						if (m_bPush == true)
						{// 押せるオブジェクトである
							float Move = 0.0f;
							if (pMove->y <= 0.0f)
							{// ジャンプしていない
								Move = pMove->z;
							}
							pPos->z = pos.z + VtxMin.z - fRange + Move;
							if (pMove->y <= 0.0f)
							{// ジャンプしていない
								Move = pMove->z * 1.1f;
							}
							pos.z += Move;
						}
						else
						{// 押せないオブジェクトである
							pPos->z = pos.z + VtxMin.z - fRange;
						}
						bCol = true;
					}
					else if (pPosOld->z - fRange >= pos.z + VtxMax.z && pPos->z - fRange <= pos.z + VtxMax.z)
					{// 前からあたった
						if (m_bPush == true)
						{// 押せるオブジェクトである
							float Move = 0.0f;
							if (pMove->y <= 0.0f)
							{// ジャンプしていない
								Move = pMove->z;
							}
							pPos->z = pos.z + VtxMax.z + fRange + Move;
							if (pMove->y <= 0.0f)
							{// ジャンプしていない
								Move = pMove->z * 1.1f;
							}
							pos.z += Move;
						}
						else
						{// 押せないオブジェクトである
							pPos->z = pos.z + VtxMax.z + fRange;
						}
						bCol = true;
					}
				}
			}
		}
	}

	// 値の設定
	SetPos(pos);

	return bCol;
}

//=============================================================================
//    配置物の向きを回転させるスピード取得処理
//=============================================================================
D3DXVECTOR3 CObject::GetRollSpeed(void)
{
	return m_RollSpeed;
}

//=============================================================================
//    配置物の影取得処理
//=============================================================================
CShadow *CObject::GetShadow(void)
{
	return m_pShadow;
}

//=============================================================================
//    配置物と当たり判定を取るかどうか取得処理
//=============================================================================
bool CObject::GetCollision(void)
{
	return m_bCollision;
}

//=============================================================================
//    配置物が壊れるかどうか取得処理
//=============================================================================
bool CObject::GetBreak(void)
{
	return m_bBreak;
}

//=============================================================================
//    配置物が押せるかどうか取得処理
//=============================================================================
bool CObject::GetPush(void)
{
	return m_bPush;
}


//*****************************************************************************
//     CObjecctMoveの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CObjectMove::CObjectMove(int nPriority, OBJTYPE objType) : CObject(nPriority, objType)
{
	// 各種値のクリア
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 移動量
	m_MoveCurrent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 現在の移動量
	m_nMoveCounter = 0;                             // 移動量管理カウンター
}

//=============================================================================
//    デストラクタ
//=============================================================================
CObjectMove::~CObjectMove()
{

}

//=============================================================================
//    動くオブジェクトの生成処理
//=============================================================================
CObjectMove *CObjectMove::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 move, int nPriority)
{
	CObjectMove *pObjectMove = NULL;   // 動く配置物クラス型のポインタ
	if (pObjectMove == NULL)
	{// メモリが空になっている
		pObjectMove = new CObjectMove(nPriority);
		if (pObjectMove != NULL)
		{// インスタンスを生成できた
			if (FAILED(pObjectMove->Init(pos, rot, RollSpeed, bCollision, move)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pObjectMove;   // インスタンスのアドレスを返す
}

//=============================================================================
//    動く配置物の読み込み処理
//=============================================================================
HRESULT CObjectMove::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // xファイルの読み込み
			D3DXLoadMeshFromX(OBJECT_MODEL_FILENAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_apBuffMat,
				NULL,
				&m_anNumMat,
				&m_apMesh);

			int nNumVtx;     // 頂点数
			DWORD sizeFVF;   // 頂点フォーマットのサイズ
			BYTE *pVtxBuff;  // 頂点バッファへのポインタ

			// 頂点数を獲得
			nNumVtx = m_apMesh->GetNumVertices();

			// 頂点フォーマットのサイズを取得
			sizeFVF = D3DXGetFVFVertexSize(m_apMesh->GetFVF());

			// 頂点バッファをロック
			m_apMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// すべての頂点を比較してモデルの最小値,最大値を抜き出す
				D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // 頂点座標の代入

				if (Vtx.x < m_aVtxMin.x)
				{// 取り出したX座標の最小値が今までのよりも小さい
					m_aVtxMin.x = Vtx.x;  // 最小値更新
				}
				if (Vtx.y < m_aVtxMin.y)
				{// 取り出したY座標の最小値が今までのよりも小さい
					m_aVtxMin.y = Vtx.y;  // 最小値更新
				}
				if (Vtx.z < m_aVtxMin.z)
				{// 取り出したZ座標の最小値が今までのよりも小さい
					m_aVtxMin.z = Vtx.z;  // 最小値更新
				}
				if (Vtx.x > m_aVtxMax.x)
				{// 取り出したX座標の最大値が今までのよりも大きい
					m_aVtxMax.x = Vtx.x;  // 最大値更新
				}
				if (Vtx.y > m_aVtxMax.y)
				{// 取り出したY座標の最大値が今までのよりも大きい
					m_aVtxMax.y = Vtx.y;  // 最大値更新
				}
				if (Vtx.z > m_aVtxMax.z)
				{// 取り出したZ座標の最大値が今までのよりも大きい
					m_aVtxMax.z = Vtx.z;  // 最大値更新
				}

				pVtxBuff += sizeFVF;  // サイズ分ポインタを進める
			}
			// 頂点バッファをアンロック
			m_apMesh->UnlockVertexBuffer();
		}
	}
	return S_OK;
}

//=============================================================================
//    動く配置物の開放処理
//=============================================================================
void CObjectMove::UnLoad(void)
{
	// メッシュの破棄
	if (m_apMesh != NULL)
	{
		m_apMesh->Release();
		m_apMesh = NULL;
	}

	// マテリアルの破棄
	if (m_apBuffMat != NULL)
	{
		m_apBuffMat->Release();
		m_apBuffMat = NULL;
	}
}

//=============================================================================
//    動く配置物の初期化処理
//=============================================================================
HRESULT CObjectMove::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 move)
{
	// 共通の初期化処理
	CObject::Init(pos, rot, RollSpeed, bCollision, false, false);

	// 各種値の設定
	m_Move = move;  // 移動量

	return S_OK;
}

//=============================================================================
//    動く配置物の終了処理
//=============================================================================
void CObjectMove::Uninit(void)
{
	// 共通の終了処理
	CObject::Uninit();
}

//=============================================================================
//    動く配置物の更新処理
//=============================================================================
void CObjectMove::Update(void)
{
	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();        // 現在の座標
	D3DXVECTOR3 posOld = GetPosOld();  // 前回の座標
	D3DXVECTOR3 rot = GetRot();        // 現在の向き

	// 前回の座標更新
	posOld = pos;

	// 移動量を反転
	m_nMoveCounter++;
	if (m_nMoveCounter % OBJECT_MOVE_TIMING == 0)
	{
		m_fMoveRivision = 0.0f;
		m_Move *= -1;
		m_nMoveCounter = 0;
	}

	// 移動させる
	m_fMoveRivision += D3DX_PI / OBJECT_MOVE_TIMING;
	if(m_fMoveRivision > D3DX_PI)
	{
		m_fMoveRivision -= D3DX_PI * 2.0f;
	}
	m_MoveCurrent.x = sinf(m_fMoveRivision) * m_Move.x;
	m_MoveCurrent.y = sinf(m_fMoveRivision) * m_Move.y;
	m_MoveCurrent.z = sinf(m_fMoveRivision) * m_Move.z;
	pos += m_MoveCurrent;

	// 向きを回転させる
	rot.y += GetRollSpeed().y;
	if (rot.y > D3DX_PI)
	{// 向きが円周率を超えた
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{// 向きが円周率を超えた
		rot.y += D3DX_PI * 2.0f;
	}

	// 各種値の設定
	SetPos(pos);        // 現在の座標
	SetPosOld(posOld);  // 前回の座標
	SetRot(rot);
}

//=============================================================================
//    動く配置物の描画処理
//=============================================================================
void CObjectMove::Draw(void)
{
	// 共通の描画処理
	CObject::Draw();
}

//=============================================================================
//    動く配置物の当たり判定処理
//=============================================================================
bool CObjectMove::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, CShadow *pShadow, float fHeight, float fRange)
{
	bool bLand = false;                // オブジェクトに乗っているかどうか
	D3DXVECTOR3 pos = GetPos();        // 現在の座標
	D3DXVECTOR3 posOld = GetPosOld();  // 前回の座標
	D3DXVECTOR3 rot = GetRot();        // 現在の向き
	D3DXVECTOR3 VtxMin = GetVtxMin();  // 最小の頂点座標
	D3DXVECTOR3 VtxMax = GetVtxMax();  // 最大の頂点座標

	// 外積の判定に必要な値を用意
	D3DXVECTOR3 ObjectCollisionPos[4] =
	{// 当たり判定用の四角(ローカル座標)
		D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMax.z + fRange),
		D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMax.z + fRange),
		D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMin.z - fRange),
		D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMin.z - fRange),
	};
	D3DXVECTOR3 ObjectCollisionWorldPos[4];  // 当たり判定用の四角(ワールド座標)
	D3DXMATRIX ObjectCollisionMtxWorld;      // 計算用マトリックス

	for (int nCntCol = 0; nCntCol < 4; nCntCol++)
	{// 判定する四角の座標分繰り返し
		D3DXMATRIX ObjectCollisionMtxWorld, mtxRot, mtxTrans;  // 計算用マトリックス

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&ObjectCollisionMtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, ObjectCollisionPos[nCntCol].x, ObjectCollisionPos[nCntCol].y, ObjectCollisionPos[nCntCol].z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxTrans);

		// 親の情報を掛け合わせる
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &GetMtxWorld());

		// ワールド座標を抜き出す
		ObjectCollisionWorldPos[nCntCol] = D3DXVECTOR3(ObjectCollisionMtxWorld._41, ObjectCollisionMtxWorld._42, ObjectCollisionMtxWorld._43);
	}

	D3DXVECTOR3 vecA, vecB;  // 判定用ベクトル
	int nCntVecTrue = 0;     // ベクトルの判定を通った回数
	bool bObject = false;    // オブジェクトの中にいるかどうか
	for (int nCntVec = 0; nCntVec < 4; nCntVec++)
	{// 判定するベクトルの数だけ繰り返し
		vecA = ObjectCollisionWorldPos[(nCntVec + 1) % 4] - ObjectCollisionWorldPos[nCntVec];
		vecB = *pPos - ObjectCollisionWorldPos[nCntVec];

		if ((vecA.x * vecB.z) - (vecA.z * vecB.x) <= 0)
		{// ベクトルの右側にいる
			nCntVecTrue++;   // 判定を通った回数を加算
			if (nCntVecTrue == 4)
			{// 全てのベクトルの右側にいる
				bObject = true;
				CDebugProc::Print("動く配置物の中にいます\n");
			}
		}
	}

	if (bObject == true)
	{// 配置物の中にいる
	    // 影の位置をずらす
		if (pShadow != NULL)
		{// 影が生成されている
			if (pPos->y >= GetPos().y + VtxMax.y - m_Move.y || pPosOld->y >= GetPos().y + VtxMax.y - m_Move.y
				|| pPos->y >= GetPos().y + VtxMax.y + m_Move.y || pPosOld->y >= GetPos().y + VtxMax.y + m_Move.y
				|| pPos->y >= GetPos().y + VtxMax.y || pPosOld->y >= GetPos().y + VtxMax.y)
			{// 配置物の上にいる
				// 位置をずらす
				pShadow->SetPos(D3DXVECTOR3(pPos->x, GetPos().y + VtxMax.y + 1.0f, pPos->z));

				// 頂点情報の設定
				VERTEX_3D *pVtx;
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = pShadow->GetVtxBuff();
				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				for (int nCntVer = 0; nCntVer < 4; nCntVer++)
				{// 影の頂点数分繰り返し
					pVtx[nCntVer].pos.y = 0.0f; // 動く配置物の上に影を乗せるのでオフセット高さは0に
				}

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();

				// 頂点バッファの設定
				pShadow->SetVtxBuff(pVtxBuff);
			}
		}

		// 乗っているかどうかの判定開始
		if (pPosOld->y >= pos.y + VtxMax.y && pPos->y <= pos.y + VtxMax.y
			|| m_Move.y >= 0.0f && pPosOld->y >= pos.y + VtxMax.y && pPos->y <= pos.y + VtxMax.y
			|| pPosOld->y >= posOld.y + VtxMax.y && pPos->y <= posOld.y + VtxMax.y
			|| m_Move.y >= 0.0f && pPosOld->y >= posOld.y + VtxMax.y && pPos->y <= posOld.y + VtxMax.y
			|| posOld.y + VtxMax.y <= pPos->y && pos.y + VtxMax.y >= pPos->y)
		{// 配置物に上から入った
			pPos->y = pos.y + VtxMax.y + m_MoveCurrent.y;
			pMove->y = 0.0f;
			bLand = true;
		}
		else if (pPosOld->y + fHeight <= pos.y + VtxMin.y && pPos->y + fHeight > pos.y + VtxMin.y
			|| pPosOld->y + fHeight <= posOld.y + VtxMin.y && pPos->y + fHeight > posOld.y + VtxMin.y)
		{// 配置物に下から入った
			pPos->y = pos.y + VtxMin.y - fHeight;
			pMove->y = 0.0f;
		}
	}

	if (pPos->y <= pos.y + VtxMax.y && pPos->y + fHeight >= pos.y + VtxMin.y)
	{// オブジェクトのY軸の間にいる
		if (bObject == true)
		{// オブジェクトの範囲内にいる
			if (GetRot().y != 0.0f)
			{// 少しでも向きが回転している(ベクトル同士が = になってしまうので方程式が成り立たない)
				for (int nCntVec = 0; nCntVec < 4; nCntVec++)
				{// 判定するベクトルの数だけ繰り返し
					vecA = ObjectCollisionWorldPos[(nCntVec + 1) % 4] - ObjectCollisionWorldPos[nCntVec];
					vecB = *pPosOld - ObjectCollisionWorldPos[nCntVec];

					if ((vecA.x * vecB.z) - (vecA.z * vecB.x) >= -1)
					{// ベクトルの左側にいる
					    // 前回の座標を利用して配置物とのベクトルをしき、左にいるベクトルの交点を求める
						D3DXVECTOR3 Pos1 = *pPosOld;                                    // プレイヤーの前回の座標
						D3DXVECTOR3 Pos2 = pos;                                         // 配置物の座標
						D3DXVECTOR3 Pos3 = ObjectCollisionWorldPos[(nCntVec + 1) % 4];  // ベクトル判定に引っかかった頂点座標(1つ目)
						D3DXVECTOR3 Pos4 = ObjectCollisionWorldPos[nCntVec];            // ベクトル判定に引っかかった頂点座標(2つ目)

						// 直線の方程式から連立方程式を組み、XZ平面上の交点を求める
						float VecLine1 = (Pos2.z - Pos1.z) / (Pos2.x - Pos1.x);
						float VecLine2 = (Pos4.z - Pos3.z) / (Pos4.x - Pos3.x);
						float XPos = (VecLine1 * Pos1.x - Pos1.z - VecLine2 * Pos3.x + Pos3.z) / (VecLine1 - VecLine2);
						float ZPos = (Pos2.z - Pos1.z) / (Pos2.x - Pos1.x) * (XPos - Pos1.x) + Pos1.z;
						pPos->x = XPos;
						pPos->z = ZPos;
						CDebugProc::Print("%.1f\n", XPos);
						CDebugProc::Print("%.1f\n\n", ZPos);
					}
				}
			}
		}
		if (GetRot().y == 0.0f)
		{// 向きが回転していない(矩形で判定を取る)
			if (pPos->z <= pos.z + VtxMax.z && pPos->z >= pos.z + VtxMin.z)
			{// オブジェクトのZ軸の間にいる
				if (pPosOld->x <= pos.x + VtxMin.x && pPos->x >= pos.x + VtxMin.x
					|| posOld.x + VtxMin.x >= pPosOld->x && pos.x + VtxMin.x <= pPos->x)
				{// 左からあたった
					pPos->x = pos.x + VtxMin.x;
				}
				else if (pPosOld->x >= pos.x + VtxMax.x && pPos->x <= pos.x + VtxMax.x
					|| posOld.x + VtxMax.x <= pPosOld->x && pos.x + VtxMax.x >= pPos->x)
				{// 右からあたった
					pPos->x = pos.x + VtxMax.x;
				}
			}
			if (pPos->x <= pos.x + VtxMax.x && pPos->x >= pos.x + VtxMin.x)
			{// オブジェクトのX軸の間にいる
				if (pPosOld->z <= pos.z + VtxMin.z && pPos->z >= pos.z + VtxMin.z
					|| posOld.z + VtxMin.z >= pPosOld->z && pos.z + VtxMin.z <= pPos->z)
				{// 後ろからあたった
					pPos->z = pos.z + VtxMin.z;
				}
				else if (pPosOld->z >= pos.z + VtxMax.z && pPos->z <= pos.z + VtxMax.z
					|| posOld.z + VtxMax.z <= pPosOld->z && pos.z + VtxMax.z >= pPos->z)
				{// 前からあたった
					pPos->z = pos.z + VtxMax.z;
				}
			}
		}
	}

	return bLand;    // オブジェクトに乗っているかどうかを返す
}

//=============================================================================
//    動く配置物の移動量取得処理
//=============================================================================
D3DXVECTOR3 CObjectMove::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    動く配置物の現在の移動量取得処理
//=============================================================================
D3DXVECTOR3 CObjectMove::GetMoveCurrent(void)
{
	return m_MoveCurrent;
}

//*****************************************************************************
//     CObjecctBalloonの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CObjectBalloon::CObjectBalloon(int nPriority, OBJTYPE objType) : CObject(nPriority, objType)
{
	// 各種値のクリア
	m_nCount = 0;
	m_nType = 0;
	m_MapBalloon = BALLOON_TYPE_0;
	for (int nCntBalloon = 0; nCntBalloon < BALLOON_TYPE_MAX; nCntBalloon++)
	{
		m_pBalloon[nCntBalloon] = NULL;
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CObjectBalloon::~CObjectBalloon()
{

}

//=============================================================================
//    吹き出し用オブジェクトの生成処理
//=============================================================================
CObjectBalloon *CObjectBalloon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, BALLOON MapBalloon, char *pNextMapName, int nPriority)
{
	CObjectBalloon *pObjectBalloon = NULL;   // 吹き出し用配置物クラス型のポインタ
	if (pObjectBalloon == NULL)
	{// メモリが空になっている
		pObjectBalloon = new CObjectBalloon(nPriority);
		if (pObjectBalloon != NULL)
		{// インスタンスを生成できた
			if (FAILED(pObjectBalloon->Init(pos, rot, RollSpeed, bCollision, pNextMapName)))
			{// 初期化に失敗した
				return NULL;
			}
			pObjectBalloon->m_MapBalloon = MapBalloon;
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pObjectBalloon;   // インスタンスのアドレスを返す
}

//=============================================================================
//    吹き出し用配置物の読み込み処理
//=============================================================================
HRESULT CObjectBalloon::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // xファイルの読み込み
			D3DXLoadMeshFromX(OBJECT_MODEL_FILENAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_apBuffMat,
				NULL,
				&m_anNumMat,
				&m_apMesh);

			int nNumVtx;     // 頂点数
			DWORD sizeFVF;   // 頂点フォーマットのサイズ
			BYTE *pVtxBuff;  // 頂点バッファへのポインタ

			// 頂点数を獲得
			nNumVtx = m_apMesh->GetNumVertices();

			// 頂点フォーマットのサイズを取得
			sizeFVF = D3DXGetFVFVertexSize(m_apMesh->GetFVF());

			// 頂点バッファをロック
			m_apMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// すべての頂点を比較してモデルの最小値,最大値を抜き出す
				D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // 頂点座標の代入

				if (Vtx.x < m_aVtxMin.x)
				{// 取り出したX座標の最小値が今までのよりも小さい
					m_aVtxMin.x = Vtx.x;  // 最小値更新
				}
				if (Vtx.y < m_aVtxMin.y)
				{// 取り出したY座標の最小値が今までのよりも小さい
					m_aVtxMin.y = Vtx.y;  // 最小値更新
				}
				if (Vtx.z < m_aVtxMin.z)
				{// 取り出したZ座標の最小値が今までのよりも小さい
					m_aVtxMin.z = Vtx.z;  // 最小値更新
				}
				if (Vtx.x > m_aVtxMax.x)
				{// 取り出したX座標の最大値が今までのよりも大きい
					m_aVtxMax.x = Vtx.x;  // 最大値更新
				}
				if (Vtx.y > m_aVtxMax.y)
				{// 取り出したY座標の最大値が今までのよりも大きい
					m_aVtxMax.y = Vtx.y;  // 最大値更新
				}
				if (Vtx.z > m_aVtxMax.z)
				{// 取り出したZ座標の最大値が今までのよりも大きい
					m_aVtxMax.z = Vtx.z;  // 最大値更新
				}

				pVtxBuff += sizeFVF;  // サイズ分ポインタを進める
			}
			// 頂点バッファをアンロック
			m_apMesh->UnlockVertexBuffer();
		}
	}
	return S_OK;
}

//=============================================================================
//    吹き出し用配置物の開放処理
//=============================================================================
void CObjectBalloon::UnLoad(void)
{
	// メッシュの破棄
	if (m_apMesh != NULL)
	{
		m_apMesh->Release();
		m_apMesh = NULL;
	}

	// マテリアルの破棄
	if (m_apBuffMat != NULL)
	{
		m_apBuffMat->Release();
		m_apBuffMat = NULL;
	}
}

//=============================================================================
//    吹き出し用配置物の初期化処理
//=============================================================================
HRESULT CObjectBalloon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, char *pNextMapName)
{
	// 共通の初期化処理
	CObject::Init(pos, rot, RollSpeed, bCollision, false, false);

	// 各種値の設定
	strcpy(m_aNextMapName, pNextMapName);
	m_nCount = 0;
	m_nType = 0;
	return S_OK;
}

//=============================================================================
//    吹き出し用配置物の終了処理
//=============================================================================
void CObjectBalloon::Uninit(void)
{
	// 共通の終了処理
	CObject::Uninit();
}

//=============================================================================
//    吹き出し用配置物の更新処理
//=============================================================================
void CObjectBalloon::Update(void)
{
	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();        // 現在の座標
	CManager *pManager = NULL;
	CGame *pGame = pManager->GetGame();
	int nTypeBalloon = -1;

	// キーボードを取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// ジョイパッドを取得
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

	// XInputを取得
	CXInput *pXInput = CManager::GetXInput();

	CScene *pScene = NULL;
	CScene *pSceneBalloon = NULL;

	bool bUse = false;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{
		pScene = CScene::GetScene(nCntPriority);

		while (pScene != NULL)
		{
			CScene *pSceneNext = pScene->GetNext();

			CScene::OBJTYPE objType;

			//種類を格納
			objType = pScene->GetObjType();

			if (objType == CScene::OBJTYPE_PLAYER)
			{
				CPlayer *pPlayer = (CPlayer*)pScene;

				//プレイヤーの位置を取得
				D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

				//プレイヤーがモデルの範囲内に入ったら吹き出しを拡大
				if (PlayerPos.x < pos.x + OBJECT_BALLOON_RANGE && PlayerPos.x > pos.x - OBJECT_BALLOON_RANGE &&
					PlayerPos.z < pos.z + OBJECT_BALLOON_RANGE && PlayerPos.z > pos.z - OBJECT_BALLOON_RANGE)
				{
					CDebugProc::Print("\n次のマップデータのファイル名 : %s\n\n", m_aNextMapName);
					//拡大
					m_nType = 0;

					if (m_MapBalloon == CObjectBalloon::BALLOON_TYPE_0)
					{
						nTypeBalloon = BALLOON_TYPE_0;
						if (bUse == false)
						{
							bUse = true;
							m_nCount += 1;
							if (m_nCount == 1)
							{
								//吹き出しの生成
								m_pBalloon[BALLOON_TYPE_0] = CBalloon::Create(OBJECT_BALLOON_POLYGON_POS,
									D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), OBJECT_BALLOON_POLYGON_WIDTH, OBJECT_BALLOON_POLYGON_HEIGHT, CObjectBalloon::BALLOON_TYPE_0);
							}
						}
						if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == true)
						{
							//ENTERを押したら呼ぶ
							pGame->SetMapName(m_aNextMapName);
							pGame->SetState(CGame::STATE_MAPCHANGE);
							CParEmitter::Create(GetPos(), 7, 6);
						}
					}
					else if (m_MapBalloon == CObjectBalloon::BALLOON_TYPE_1)
					{
						nTypeBalloon = BALLOON_TYPE_1;

						if (bUse == false)
						{
							bUse = true;
							m_nCount += 1;
							if (m_nCount == 1)
							{
								//吹き出しの生成
								m_pBalloon[BALLOON_TYPE_1] = CBalloon::Create(OBJECT_BALLOON_POLYGON_POS,
									D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), OBJECT_BALLOON_POLYGON_WIDTH, OBJECT_BALLOON_POLYGON_HEIGHT, CObjectBalloon::BALLOON_TYPE_1);
							}
						}
						if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == true)
						{
							//ENTERを押したら呼ぶ
							pGame->SetMapName(m_aNextMapName);
							pGame->SetState(CGame::STATE_MAPCHANGE);
							CParEmitter::Create(GetPos(), 7, 6);
						}
					}

					else if (m_MapBalloon == CObjectBalloon::BALLOON_TYPE_2)
					{
						nTypeBalloon = BALLOON_TYPE_2;

						if (bUse == false)
						{
							bUse = true;
							m_nCount += 1;
							if (m_nCount == 1)
							{
								//吹き出しの生成
								m_pBalloon[BALLOON_TYPE_2] = CBalloon::Create(OBJECT_BALLOON_POLYGON_POS,
									D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), OBJECT_BALLOON_POLYGON_WIDTH, OBJECT_BALLOON_POLYGON_HEIGHT, CObjectBalloon::BALLOON_TYPE_2);
							}
						}
						if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == true)
						{
							//ENTERを押したら呼ぶ
							pGame->SetMapName(m_aNextMapName);
							pGame->SetState(CGame::STATE_MAPCHANGE);
							CParEmitter::Create(GetPos(), 7, 6);
						}
					}
					else if (m_MapBalloon == CObjectBalloon::BALLOON_TYPE_3)
					{
						nTypeBalloon = BALLOON_TYPE_3;

						if (bUse == false)
						{
							bUse = true;
							m_nCount += 1;
							if (m_nCount == 1)
							{
								//吹き出しの生成
								m_pBalloon[BALLOON_TYPE_3] = CBalloon::Create(OBJECT_BALLOON_POLYGON_POS,
									D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), OBJECT_BALLOON_POLYGON_WIDTH, OBJECT_BALLOON_POLYGON_HEIGHT, CObjectBalloon::BALLOON_TYPE_3);
							}
						}
						if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == true)
						{
							//ENTERを押したら呼ぶ
							pGame->SetMapName(m_aNextMapName);
							pGame->SetState(CGame::STATE_MAPCHANGE);
							CParEmitter::Create(GetPos(), 7, 6);
						}
					}
					else if (m_MapBalloon == CObjectBalloon::BALLOON_TYPE_4)
					{
						nTypeBalloon = BALLOON_TYPE_4;

						if (bUse == false)
						{
							bUse = true;
							m_nCount += 1;
							if (m_nCount == 1)
							{
								//吹き出しの生成
								m_pBalloon[BALLOON_TYPE_4] = CBalloon::Create(OBJECT_BALLOON_POLYGON_POS,
									D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), OBJECT_BALLOON_POLYGON_WIDTH, OBJECT_BALLOON_POLYGON_HEIGHT, CObjectBalloon::BALLOON_TYPE_4);
							}
						}
						if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == true)
						{
							pGame->SetMapName(m_aNextMapName);
							pGame->SetState(CGame::STATE_MAPCHANGE);

							CParEmitter::Create(GetPos(), 7, 6);
						}
					}
				}
				else
				{//プレイヤーがモデルの範囲外だったら吹き出しを縮小
				 //縮小
					m_nType = 1;
					bUse = false;
				}

				for (int nCntBalloon = 0; nCntBalloon < MAX_BALLOON; nCntBalloon++)
				{
					if (m_nType == 0 || m_nType == 1)
					{
						if (nTypeBalloon == nCntBalloon)
						{
							//吹き出しの透明か不透明
							m_pBalloon[nCntBalloon]->SetType(m_nType);
						}
						else
						{
							if (m_pBalloon[nCntBalloon] != NULL)
							{
								m_pBalloon[nCntBalloon]->SetType(1);
							}
						}
					}
				}
			}
			pScene = pSceneNext;
		}
	}
	// 各種値の設定
	SetPos(pos);        // 現在の座標
}

//=============================================================================
//    吹き出し用配置物の描画処理
//=============================================================================
void CObjectBalloon::Draw(void)
{
	// 共通の描画処理
	CObject::Draw();
}

//*****************************************************************************
//     CObjecctGoalの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CObjectGoal::CObjectGoal(int nPriority, OBJTYPE objType) : CObject(nPriority, objType)
{
	// 各種値のクリア
}

//=============================================================================
//    デストラクタ
//=============================================================================
CObjectGoal::~CObjectGoal()
{

}

//=============================================================================
//    ゴール用オブジェクトの生成処理
//=============================================================================
CObjectGoal *CObjectGoal::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, int nPriority)
{
	CObjectGoal *pObjectGoal = NULL;   // ゴール用配置物クラス型のポインタ
	if (pObjectGoal == NULL)
	{// メモリが空になっている
		pObjectGoal = new CObjectGoal(nPriority);
		if (pObjectGoal != NULL)
		{// インスタンスを生成できた
			if (FAILED(pObjectGoal->Init(pos, rot, RollSpeed, bCollision)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pObjectGoal;   // インスタンスのアドレスを返す
}

//=============================================================================
//    ゴール用配置物の読み込み処理
//=============================================================================
HRESULT CObjectGoal::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // xファイルの読み込み
			D3DXLoadMeshFromX(OBJECT_MODEL_FILENAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_apBuffMat,
				NULL,
				&m_anNumMat,
				&m_apMesh);

			int nNumVtx;     // 頂点数
			DWORD sizeFVF;   // 頂点フォーマットのサイズ
			BYTE *pVtxBuff;  // 頂点バッファへのポインタ

			// 頂点数を獲得
			nNumVtx = m_apMesh->GetNumVertices();

			// 頂点フォーマットのサイズを取得
			sizeFVF = D3DXGetFVFVertexSize(m_apMesh->GetFVF());

			// 頂点バッファをロック
			m_apMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// すべての頂点を比較してモデルの最小値,最大値を抜き出す
				D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // 頂点座標の代入

				if (Vtx.x < m_aVtxMin.x)
				{// 取り出したX座標の最小値が今までのよりも小さい
					m_aVtxMin.x = Vtx.x;  // 最小値更新
				}
				if (Vtx.y < m_aVtxMin.y)
				{// 取り出したY座標の最小値が今までのよりも小さい
					m_aVtxMin.y = Vtx.y;  // 最小値更新
				}
				if (Vtx.z < m_aVtxMin.z)
				{// 取り出したZ座標の最小値が今までのよりも小さい
					m_aVtxMin.z = Vtx.z;  // 最小値更新
				}
				if (Vtx.x > m_aVtxMax.x)
				{// 取り出したX座標の最大値が今までのよりも大きい
					m_aVtxMax.x = Vtx.x;  // 最大値更新
				}
				if (Vtx.y > m_aVtxMax.y)
				{// 取り出したY座標の最大値が今までのよりも大きい
					m_aVtxMax.y = Vtx.y;  // 最大値更新
				}
				if (Vtx.z > m_aVtxMax.z)
				{// 取り出したZ座標の最大値が今までのよりも大きい
					m_aVtxMax.z = Vtx.z;  // 最大値更新
				}

				pVtxBuff += sizeFVF;  // サイズ分ポインタを進める
			}
			// 頂点バッファをアンロック
			m_apMesh->UnlockVertexBuffer();
		}
	}
	return S_OK;
}

//=============================================================================
//    ゴール用配置物の開放処理
//=============================================================================
void CObjectGoal::UnLoad(void)
{
	// メッシュの破棄
	if (m_apMesh != NULL)
	{
		m_apMesh->Release();
		m_apMesh = NULL;
	}

	// マテリアルの破棄
	if (m_apBuffMat != NULL)
	{
		m_apBuffMat->Release();
		m_apBuffMat = NULL;
	}
}

//=============================================================================
//    ゴール用配置物の初期化処理
//=============================================================================
HRESULT CObjectGoal::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision)
{
	// 共通の初期化処理
	CObject::Init(pos, rot, RollSpeed, bCollision, false, false);

	return S_OK;
}

//=============================================================================
//    ゴール用配置物の終了処理
//=============================================================================
void CObjectGoal::Uninit(void)
{
	// 共通の終了処理
	CObject::Uninit();
}

//=============================================================================
//    ゴール用配置物の更新処理
//=============================================================================
void CObjectGoal::Update(void)
{

}

//=============================================================================
//    ゴール用配置物の描画処理
//=============================================================================
void CObjectGoal::Draw(void)
{
	// 共通の描画処理
	CObject::Draw();
}

//*****************************************************************************
//     CObjectTargetの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CObjectTarget::CObjectTarget(int nPriority, OBJTYPE objType) : CObject(nPriority, objType)
{
	// 各種値のクリア
	m_nMaxCarryTime = 0;                              // プレイヤーを運ぶのにかける時間
	m_nCounter = 0;                                   // 運びを管理するカウンター
	m_State = STATE_NONE;                             // 状態
	m_PosTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 目的の位置
	m_DiffPosTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 目的の位置との差分
	m_pPlayer = NULL;                                 // プレイヤークラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CObjectTarget::~CObjectTarget()
{

}

//=============================================================================
//    ジップライン用オブジェクトの生成処理
//=============================================================================
CObjectTarget *CObjectTarget::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 PosTarget, int nMaxCarry, int nPriority)
{
	CObjectTarget *pObjectTarget = NULL;   // ジップライン用配置物クラス型のポインタ
	if (pObjectTarget == NULL)
	{// メモリが空になっている
		pObjectTarget = new CObjectTarget(nPriority);
		if (pObjectTarget != NULL)
		{// インスタンスを生成できた
			if (FAILED(pObjectTarget->Init(pos, rot, RollSpeed, bCollision, PosTarget, nMaxCarry)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pObjectTarget;   // インスタンスのアドレスを返す
}

//=============================================================================
//    ジップライン用配置物の読み込み処理
//=============================================================================
HRESULT CObjectTarget::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // xファイルの読み込み
			D3DXLoadMeshFromX(OBJECT_MODEL_FILENAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_apBuffMat,
				NULL,
				&m_anNumMat,
				&m_apMesh);

			int nNumVtx;     // 頂点数
			DWORD sizeFVF;   // 頂点フォーマットのサイズ
			BYTE *pVtxBuff;  // 頂点バッファへのポインタ

			// 頂点数を獲得
			nNumVtx = m_apMesh->GetNumVertices();

			// 頂点フォーマットのサイズを取得
			sizeFVF = D3DXGetFVFVertexSize(m_apMesh->GetFVF());

			// 頂点バッファをロック
			m_apMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// すべての頂点を比較してモデルの最小値,最大値を抜き出す
				D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // 頂点座標の代入

				if (Vtx.x < m_aVtxMin.x)
				{// 取り出したX座標の最小値が今までのよりも小さい
					m_aVtxMin.x = Vtx.x;  // 最小値更新
				}
				if (Vtx.y < m_aVtxMin.y)
				{// 取り出したY座標の最小値が今までのよりも小さい
					m_aVtxMin.y = Vtx.y;  // 最小値更新
				}
				if (Vtx.z < m_aVtxMin.z)
				{// 取り出したZ座標の最小値が今までのよりも小さい
					m_aVtxMin.z = Vtx.z;  // 最小値更新
				}
				if (Vtx.x > m_aVtxMax.x)
				{// 取り出したX座標の最大値が今までのよりも大きい
					m_aVtxMax.x = Vtx.x;  // 最大値更新
				}
				if (Vtx.y > m_aVtxMax.y)
				{// 取り出したY座標の最大値が今までのよりも大きい
					m_aVtxMax.y = Vtx.y;  // 最大値更新
				}
				if (Vtx.z > m_aVtxMax.z)
				{// 取り出したZ座標の最大値が今までのよりも大きい
					m_aVtxMax.z = Vtx.z;  // 最大値更新
				}

				pVtxBuff += sizeFVF;  // サイズ分ポインタを進める
			}
			// 頂点バッファをアンロック
			m_apMesh->UnlockVertexBuffer();
		}
	}
	return S_OK;
}

//=============================================================================
//    ジップライン用配置物の開放処理
//=============================================================================
void CObjectTarget::UnLoad(void)
{
	// メッシュの破棄
	if (m_apMesh != NULL)
	{
		m_apMesh->Release();
		m_apMesh = NULL;
	}

	// マテリアルの破棄
	if (m_apBuffMat != NULL)
	{
		m_apBuffMat->Release();
		m_apBuffMat = NULL;
	}
}

//=============================================================================
//    ジップライン用配置物の初期化処理
//=============================================================================
HRESULT CObjectTarget::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 PosTarget, int nMaxCarry)
{
	// 共通の初期化処理
	CObject::Init(pos, rot, RollSpeed, bCollision, false, false);

	// 各種値の設定
	m_PosTarget = PosTarget;       // 目的の位置
	m_nMaxCarryTime = nMaxCarry;   // 運ぶのにかける時間

	// 現在の位置と目的の位置の差分を求める
	m_DiffPosTarget = m_PosTarget - pos;

	return S_OK;
}

//=============================================================================
//    ジップライン用配置物の終了処理
//=============================================================================
void CObjectTarget::Uninit(void)
{
	// 共通の終了処理
	CObject::Uninit();
}

//=============================================================================
//    ジップライン用配置物の更新処理
//=============================================================================
void CObjectTarget::Update(void)
{
	if (m_State == STATE_CARRY)
	{// プレイヤーを運ぶ状態である
		Carry();
	}
}

//=============================================================================
//    ジップライン用配置物の描画処理
//=============================================================================
void CObjectTarget::Draw(void)
{
	// 共通の描画処理
	CObject::Draw();
}

//=============================================================================
//    ジップライン用配置物の当たり判定処理
//=============================================================================
bool CObjectTarget::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, CShadow *pShadow, float fHeight, float fRange, CScene *pScene)
{
	bool bCatch = false;   // オブジェクトにつかまっているかどうか

	// 当たり判定に必要な値を配置物から取得
	D3DXVECTOR3 pos = GetPos();          // 現在の座標
	D3DXVECTOR3 posOld = GetPosOld();    // 前回の座標
	D3DXVECTOR3 Rot = GetRot();          // 現在の向き
	D3DXVECTOR3 VtxMin = GetVtxMin();    // 頂点の座標最小値
	D3DXVECTOR3 VtxMax = GetVtxMax();    // 頂点の座標最大値

	// 外積の判定に必要な値を用意
	D3DXVECTOR3 ObjectCollisionPos[4] =
	{// 当たり判定用の四角(ローカル座標)
		D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMax.z + fRange),
		D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMax.z + fRange),
		D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMin.z - fRange),
		D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMin.z - fRange),
	};

	D3DXVECTOR3 ObjectCollisionWorldPos[4];  // 当たり判定用の四角(ワールド座標)
	D3DXMATRIX ObjectCollisionMtxWorld;      // 計算用マトリックス

	for (int nCntCol = 0; nCntCol < 4; nCntCol++)
	{// 判定する四角の座標分繰り返し
		D3DXMATRIX ObjectCollisionMtxWorld, mtxRot, mtxTrans;  // 計算用マトリックス

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&ObjectCollisionMtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, ObjectCollisionPos[nCntCol].x, ObjectCollisionPos[nCntCol].y, ObjectCollisionPos[nCntCol].z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxTrans);

		// 親の情報を掛け合わせる
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &GetMtxWorld());

		// ワールド座標を抜き出す
		ObjectCollisionWorldPos[nCntCol] = D3DXVECTOR3(ObjectCollisionMtxWorld._41, ObjectCollisionMtxWorld._42, ObjectCollisionMtxWorld._43);
	}

	D3DXVECTOR3 vecA, vecB;        // 判定用ベクトル
	int nCntVecTrue = 0;           // ベクトルの判定を通った回数
	bool bObjectRand = false;      // オブジェクトの中にいるかどうか
	for (int nCntVec = 0; nCntVec < 4; nCntVec++)
	{// 判定するベクトルの数だけ繰り返し
		vecA = ObjectCollisionWorldPos[(nCntVec + 1) % 4] - ObjectCollisionWorldPos[nCntVec];
		vecB = *pPos - ObjectCollisionWorldPos[nCntVec];

		if ((vecA.x * vecB.z) - (vecA.z * vecB.x) <= 0)
		{// ベクトルの右側にいる
			nCntVecTrue++;   // 判定を通った回数を加算
			if (nCntVecTrue == 4)
			{// 全てのベクトルの右側にいる
				bObjectRand = true;
			}
		}
	}

	if (bObjectRand == true)
	{// 配置物の中にいる
	    // 影の位置をずらす
		if (pShadow != NULL)
		{// 影が生成されている
			if (pPos->y >= GetPos().y + VtxMax.y || pPosOld->y >= GetPos().y + VtxMax.y)
			{// 配置物の上にいる
			    // 位置をずらす
				pShadow->SetPos(D3DXVECTOR3(pPos->x, pos.y + VtxMax.y + 1.0f, pPos->z));

				// 頂点情報の設定
				VERTEX_3D *pVtx;
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = pShadow->GetVtxBuff();
				// 頂点バッファをロックし,頂点データへのポインタを取得
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				for (int nCntVer = 0; nCntVer < 4; nCntVer++)
				{// 影の頂点数分繰り返し
					pVtx[nCntVer].pos.y = 0.0f; // 配置物の上に影を乗せるのでオフセット高さは0に
				}

				// 頂点バッファをアンロックする
				pVtxBuff->Unlock();

				// 頂点バッファの設定
				pShadow->SetVtxBuff(pVtxBuff);
			}
		}

		// 乗っているかどうかの判定開始
		if (pPosOld->y >= pos.y + VtxMax.y && pPos->y <= pos.y + VtxMax.y)
		{// 配置物に上から入った
			pPos->y = pos.y + VtxMax.y;
			pMove->y = 0.0f;
		}
		else if (pPosOld->y + fHeight <= pos.y + VtxMin.y && pPos->y + fHeight > pos.y + VtxMin.y)
		{// 配置物に下から入った
			pPos->y = pos.y + VtxMin.y - fHeight;
			if (m_State == STATE_NONE)
			{// プレイヤーを運んでいない状態だったら
				pPos->x = pos.x;
				pPos->z = pos.z;
				pMove->y = 0.0f;
				m_State = STATE_CARRY;  // 運ぶ状態に
				if (pScene != NULL)
				{// プレイヤーのポインタが取得できている
					if (pScene->GetObjType() == OBJTYPE_PLAYER)
					{// プレイヤーのポインタだったら
						m_pPlayer = (CPlayer*)pScene;
						if (m_pPlayer != NULL)
						{// プレイヤークラスへのポインタを取得できた
							m_pPlayer->SetRot(GetRot());
						}
					}
				}
				bCatch = true;
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ZIPLINE);
			}
		}
	}

	// 各種値の設定
	SetPos(pos);   // 現在の座標

	return bCatch;   // つかまったかどうかを返す
}

//=============================================================================
//    プレイヤーを運ぶ処理
//=============================================================================
void CObjectTarget::Carry(void)
{
	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();  // 現在の座標

	// 現在の位置に加える
	pos += m_DiffPosTarget / (float)m_nMaxCarryTime;

	// プレイヤーも運ぶ
	if (m_pPlayer != NULL)
	{// プレイヤークラスが取得できている
		m_pPlayer->SetPos(D3DXVECTOR3(pos.x, pos.y - m_pPlayer->GetColHeight(), pos.z));
	}

	// 現在の位置を設定
	SetPos(pos);

	m_nCounter++;
	if (m_nCounter % m_nMaxCarryTime == 0)
	{// 目的の位置に到達した
		m_State = STATE_FINISH;
		// プレイヤーの状態を戻す
		if (m_pPlayer != NULL)
		{// プレイヤークラスが取得できている
			m_pPlayer->SetState(CPlayer::STATE_NORMAL);
		}
		m_pPlayer = NULL;
		CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_ZIPLINE);
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CARRYFINISH);
	}
}