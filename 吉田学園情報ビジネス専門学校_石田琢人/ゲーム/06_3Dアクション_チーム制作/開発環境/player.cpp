//*****************************************************************************
//
//     プレイヤーの処理[player.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"

#include "model.h"
#include "motion.h"
#include "field.h"
#include "water.h"
#include "object.h"
#include "shadow.h"
#include "particle.h"

#include "game.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
// テキストから値を読み取るパス名
#define NUM_MODEL        "NUM_MODEL"           // 読み込むモデルの数
#define MODEL_FILENAME   "MODEL_FILENAME"      // モデルのファイル名
#define PARTSSET         "PARTSSET"            // パーツセット読み込み開始の合図
#define END_PARTSSET     "END_PARTSSET"        // パーツセット読み込み終了の合図
#define CHARACTERSET     "CHARACTERSET"        // キャラクターセット読み込み開始の合図
#define END_CHARACTERSET "END_CHARACTERSET"    // パーツセット読み込み終了の合図

// キャラクターセットパス名
#define MOVE             "MOVE"                // プレイヤーの加速度
#define MAX_WALKSPEED    "MAX_WALKSPEED"       // プレイヤーの歩くスピードの最大値
#define JUMP             "JUMP"                // プレイヤーのジャンプ力
#define GRAVITY          "GRAVITY"             // プレイヤーの重力
#define INERTIA          "INERTIA"             // プレイヤーの慣性
#define RIVISION_ANGLE   "RIVISION_ANGLE"      // プレイヤーの向きを補正する倍率
#define NUM_PARTS        "NUM_PARTS"           // プレイヤーが使用するパーツ数
#define COL_HEIGHT       "COL_HEIGHT"          // プレイヤーの当たり判定をとる高さ

// パーツセットパス名
#define INDEX            "INDEX"               // 割り当てるパーツのモデル番号
#define PARENT           "PARENT"              // 親パーツの番号
#define POS              "POS"                 // 親からのオフセット位置
#define ROT              "ROT"                 // 親からのオフセット向き


// 初期値代入用マクロ
#define PLAYER_MAX_WALKSPEED_INI (2.0f)      // プレイヤーが歩くスピードの最大値
#define PLAYER_ACCELERATION_INI  (3.0f)      // プレイヤーの加速度
#define PLAYER_GRAVITY_INI       (-0.7f)     // プレイヤーの重力
#define PLAYER_JUMPPOWER_INI     (11.65f)    // プレイヤーのジャンプ力
#define PLAYER_INERTIA_INI       (0.423f)    // プレイヤーの慣性
#define PLAYER_ANGLE_REVISION    (0.1f)      // プレイヤーの向きを補正する倍率
#define PLAYER_COLHEIGHT_INI     (80.0f)     // プレイヤーの当たり判定をとる高さ

// 判定用マクロ
#define PLAYER_COL_RANGE         (15.0f)     // 当たり判定を取る範囲
#define PLAYER_MOVE_SPEED        (0.5f)      // プレイヤーが移動していると判定される数値の最大値
#define PLAYER_MAX_WALKSPEED     (2.0f)      // プレイヤーが歩いていると判定される数値の最大値
#define PLAYER_PANCH_COLINDEX    (3)         // パンチの当たり判定をとるモデルの番号
#define PLAYER_PANCH_COLRANGE    (5.0f)      // パンチの当たり判定をとる範囲
#define PLAYER_KICK_COLINDEX     (7)         // キックの当たり判定をとるモデルの番号
#define PLAYER_KICK_COLRANGE     (14.0f)     // キックの当たり判定をとる範囲

//*****************************************************************************
//     CPlayerの処理
//*****************************************************************************
//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************
LPD3DXMESH     *CPlayer::m_apMesh = NULL;      // メッシュ情報へのポインタ
LPD3DXBUFFER   *CPlayer::m_apBuffMat = NULL;   // マテリアル情報へのポインタ
DWORD          *CPlayer::m_anNumMat = NULL;    // マテリアル情報の数
D3DXVECTOR3    *CPlayer::m_aVtxMin = NULL;     // 頂点座標の最小値
D3DXVECTOR3    *CPlayer::m_aVtxMax = NULL;     // 頂点座標の最大値
int            CPlayer::m_nNumModel = 0;       // プレイヤーの使用するパーツ数
char           CPlayer::m_aFileName[256] = ""; // プレイヤーデータの外部ファイル名

//=============================================================================
//    コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_apModel = NULL;                                  // モデルクラスへのポインタ
	m_nNumPart = 0;                                    // 使用するパーツ数
	m_pMotionManager = NULL;                           // モーションクラスへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);             // 現在の座標
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 前回の座標
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);            // 移動量
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);             // 現在の向き
	m_DiffAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 現在の向きと目的の向きの差分
	m_DestAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 目的の向き
	D3DXMatrixIdentity(&m_MtxWorld);                   // ワールドマトリックス
	m_State = STATE_NORMAL;                            // 状態
	m_fGravity = 0.0f;                                 // 重力
	m_fMaxWalkSpeed = 0.0f;                            // 歩くスピードの最大値
	m_fJumpPower = 0.0f;                               // ジャンプ力
	m_bAction = false;                                 // アクション状態であるかどうか
	m_bJump = false;                                   // ジャンプしたかどうか
	m_fAccel = 0.0f;                                   // 加速度
	m_fInertia = 0.0f;                                 // 慣性
	m_fRivisionRot = 0.0f;                             // 向きを補正する倍率
	m_bPush = false;                                   // オブジェクトを押しているかどうか
	m_pLandScene = NULL;                               // プレイヤーが乗っているクラスへのポインタ
	m_pShadow = NULL;                                  // 影クラスへのポインタ
	m_bLand = false;                                   // 何かに乗っているかどうか
	m_bCol = false;                                    // オブジェクトに当たっているかどうか
	m_OffsetPosStd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 乗った時の配置物とのオフセット位置
	m_OffsetRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 乗った時のプレイヤーのオフセット向き
	m_OffsetRotStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 現在の向きと目的の向きの差分
	m_OffsetPosAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 現在のオフセット位置に加える値
	m_OffsetPosAddOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 前回のオフセット位置に加える値
}

//=============================================================================
//    デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
//    プレイヤーの生成処理
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority)
{
	CPlayer *pPlayer = NULL;   // プレイヤークラス型のポインタ
	if (pPlayer == NULL)
	{// メモリが空になっている
		pPlayer = new CPlayer(nPriority);
		if (pPlayer != NULL)
		{// インスタンスを生成できた
			pPlayer->SetPos(pos);
			pPlayer->SetRot(rot);
			if (FAILED(pPlayer->Init()))
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

	return pPlayer;   // インスタンスのアドレスを返す
}

//=============================================================================
//    プレイヤーのパーツ読み込み処理
//=============================================================================
HRESULT CPlayer::Load(char *pFileName)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			FILE *pFile = NULL;             // プレイヤーデータへの外部ファイルポインタ
			char aString[256];              // テキスト格納用変数(基本的に要らない文字を格納する)
			char aXFileName[256];           // モデルのXファイル名
			int  nCntModel = 0;             // パーツを読み込んだ回数
			strcpy(m_aFileName,pFileName);  // ファイル名を保存

			if (pFile == NULL)
			{// ファイルポインタを確保できる状態にある
				pFile = fopen(m_aFileName, "r");
				if (pFile != NULL)
				{// ファイルポインタが確保できた
					while (fscanf(pFile,"%s",&aString[0]) != EOF)
					{// テキストが最後の行に到達するまで読み込みを続ける
						if (strcmp(&aString[0], NUM_MODEL) == 0)
						{// 読み込むモデル数のパスがある
							fscanf(pFile, "%s %d", &aString[0], &m_nNumModel);
							if (m_nNumModel >= 1)
							{// 読み込むモデルが１つ以上ある
								// 必要な変数を動的確保する
								// メッシュ情報へのポインタ
								if (m_apMesh == NULL)
								{// メモリが確保できる状態にある
									m_apMesh = new LPD3DXMESH[m_nNumModel];
								}

								// マテリアル情報へのポインタ
								if (m_apBuffMat == NULL)
								{// メモリが確保できる状態にある
									m_apBuffMat = new LPD3DXBUFFER[m_nNumModel];
								}

								// マテリアルの数情報へのポインタ
								if (m_anNumMat == NULL)
								{// メモリが確保できる状態にある
									m_anNumMat = new DWORD[m_nNumModel];
								}

								// メッシュの頂点最小値情報へのポインタ
								if (m_aVtxMin == NULL)
								{// メモリが確保できる状態にある
									m_aVtxMin = new D3DXVECTOR3[m_nNumModel];
								}

								// メッシュの頂点最大値情報へのポインタ
								if (m_aVtxMax == NULL)
								{// メモリが確保できる状態にある
									m_aVtxMax = new D3DXVECTOR3[m_nNumModel];
								}
							}
						}
						if (strcmp(&aString[0], MODEL_FILENAME) == 0)
						{// 読み込むモデルのファイルパスがある
							if (nCntModel <= m_nNumModel)
							{// 確保したメモリよりオーバーしていない
								if (fscanf(pFile, "%s %s", &aString[0], &aXFileName[0]) == 2)
								{// モデルのファイルパス名が読み取れた
									// xファイルの読み込み
									D3DXLoadMeshFromX(aXFileName,
										D3DXMESH_SYSTEMMEM,
										pDevice,
										NULL,
										&m_apBuffMat[nCntModel],
										NULL,
										&m_anNumMat[nCntModel],
										&m_apMesh[nCntModel]);

									if (m_apMesh[nCntModel] != NULL)
									{// モデルが読み込めた
										// 最小値と最大値を初期化
										m_aVtxMin[nCntModel] = D3DXVECTOR3(10000000.0f, 10000000.0f, 10000000.0f);
										m_aVtxMax[nCntModel] = D3DXVECTOR3(-10000000.0f, -10000000.0f, -10000000.0f);

										int nNumVtx;     // 頂点数
										DWORD sizeFVF;   // 頂点フォーマットのサイズ
										BYTE *pVtxBuff;  // 頂点バッファへのポインタ

										// 頂点数を獲得
										nNumVtx = m_apMesh[nCntModel]->GetNumVertices();

										// 頂点フォーマットのサイズを取得
										sizeFVF = D3DXGetFVFVertexSize(m_apMesh[nCntModel]->GetFVF());

										// 頂点バッファをロック
										m_apMesh[nCntModel]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

										for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
										{// すべての頂点を比較してモデルの最小値,最大値を抜き出す
											D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // 頂点座標の代入

											if (Vtx.x < m_aVtxMin[nCntModel].x)
											{// 取り出したX座標の最小値が今までのよりも小さい
												m_aVtxMin[nCntModel].x = Vtx.x;  // 最小値更新
											}
											if (Vtx.y < m_aVtxMin[nCntModel].y)
											{// 取り出したY座標の最小値が今までのよりも小さい
												m_aVtxMin[nCntModel].y = Vtx.y;  // 最小値更新
											}
											if (Vtx.z < m_aVtxMin[nCntModel].z)
											{// 取り出したZ座標の最小値が今までのよりも小さい
												m_aVtxMin[nCntModel].z = Vtx.z;  // 最小値更新
											}
											if (Vtx.x > m_aVtxMax[nCntModel].x)
											{// 取り出したX座標の最大値が今までのよりも大きい
												m_aVtxMax[nCntModel].x = Vtx.x;  // 最大値更新
											}
											if (Vtx.y > m_aVtxMax[nCntModel].y)
											{// 取り出したY座標の最大値が今までのよりも大きい
												m_aVtxMax[nCntModel].y = Vtx.y;  // 最大値更新
											}
											if (Vtx.z > m_aVtxMax[nCntModel].z)
											{// 取り出したZ座標の最大値が今までのよりも大きい
												m_aVtxMax[nCntModel].z = Vtx.z;  // 最大値更新
											}

											pVtxBuff += sizeFVF;  // サイズ分ポインタを進める
										}
										// 頂点バッファをアンロック
										m_apMesh[nCntModel]->UnlockVertexBuffer();
									}

									nCntModel++;   // 読み込んだモデルの数を進める
								}
							}
						}
					}
					fclose(pFile);    // ファイルを閉じる
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    プレイヤーの開放処理
//=============================================================================
void CPlayer::UnLoad(void)
{
	// メッシュの破棄
	if (m_apMesh != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			if (m_apMesh[nCntModel] != NULL)
			{
				m_apMesh[nCntModel]->Release();
				m_apMesh[nCntModel] = NULL;
			}
		}

		// メモリの開放
		delete[] m_apMesh;
		m_apMesh = NULL;
	}

	// マテリアルの破棄
	if (m_apBuffMat != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			if (m_apBuffMat[nCntModel] != NULL)
			{
				m_apBuffMat[nCntModel]->Release();
				m_apBuffMat[nCntModel] = NULL;
			}
		}

		// メモリの開放
		delete[] m_apBuffMat;
		m_apBuffMat = NULL;
	}

	// マテリアルの数情報の破棄
	if (m_anNumMat != NULL)
	{
		// メモリの開放
		delete[] m_anNumMat;
		m_anNumMat = NULL;
	}

	// 頂点の最小値情報の破棄
	if (m_aVtxMin != NULL)
	{
		// メモリの開放
		delete[] m_aVtxMin;
		m_aVtxMin = NULL;
	}

	// 頂点の最大値情報の破棄
	if (m_aVtxMax != NULL)
	{
		// メモリの開放
		delete[] m_aVtxMax;
		m_aVtxMax = NULL;
	}
}

//=============================================================================
//    プレイヤーの初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	// 各種値の初期設定
	m_fGravity = PLAYER_GRAVITY_INI;             // 重力
	m_fMaxWalkSpeed = PLAYER_MAX_WALKSPEED_INI;  // 歩くスピードの最大値
	m_fJumpPower = PLAYER_JUMPPOWER_INI;         // ジャンプ力
	m_fAccel = PLAYER_ACCELERATION_INI;          // 加速度
	m_fInertia = PLAYER_INERTIA_INI;             // 慣性
	m_fRivisionRot = PLAYER_ANGLE_REVISION;      // 向きを補正する倍率
	m_fColHeight = PLAYER_COLHEIGHT_INI;         // 当たり判定を取る高さ

	// モデルの生成
	FILE *pFile = NULL;                                          // プレイヤーデータへの外部ファイルポインタ
	char aString[256];                                           // テキスト格納用変数(基本的に要らない文字を格納する)
	int nIdx = 0;                                                // 使用するパーツ番号
	int nIdxParent = -1;                                         // 親パーツへの番号
	D3DXVECTOR3 ModelOffsetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 親パーツからのオフセット位置
	D3DXVECTOR3 ModelOffsetRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 親パーツからのオフセット向き

	// 外部ファイルからプレイヤーデータを読み込む
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタが確保できた
			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// テキストが最後の行に到達するまで読み込みを続ける
				if (strcmp(&aString[0], CHARACTERSET) == 0)
				{// キャラクター情報のパスがある
					while (strcmp(&aString[0], END_CHARACTERSET) != 0)
					{// キャラクター情報を読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストファイルを読み進める
						if (strcmp(&aString[0], MOVE) == 0)
						{// 移動量の情報がある
							fscanf(pFile, "%s %f", &aString[0], &m_fAccel);
						}
						if (strcmp(&aString[0], MAX_WALKSPEED) == 0)
						{// 歩くスピードの最大値の情報がある
							fscanf(pFile, "%s %f", &aString[0], &m_fMaxWalkSpeed);
						}
						if (strcmp(&aString[0], JUMP) == 0)
						{// ジャンプ力の情報がある
							fscanf(pFile, "%s %f", &aString[0], &m_fJumpPower);
						}
						if (strcmp(&aString[0], GRAVITY) == 0)
						{// 重力の情報がある
							fscanf(pFile, "%s %f", &aString[0], &m_fGravity);
						}
						if (strcmp(&aString[0], INERTIA) == 0)
						{// 慣性の情報がある
							fscanf(pFile, "%s %f", &aString[0], &m_fInertia);
						}
						if (strcmp(&aString[0], RIVISION_ANGLE) == 0)
						{// 向きを補正する倍率の情報がある
							fscanf(pFile, "%s %f", &aString[0], &m_fRivisionRot);
						}
						if (strcmp(&aString[0], COL_HEIGHT) == 0)
						{// 当たり判定を取る高さの情報がある
							fscanf(pFile, "%s %f", &aString[0], &m_fColHeight);
						}
						if (strcmp(&aString[0], NUM_PARTS) == 0)
						{// プレイヤーが使用するパーツ数の情報がある
							fscanf(pFile, "%s %d", &aString[0], &m_nNumPart);
							if (m_apModel == NULL)
							{// メモリが確保できる状態にある
								m_apModel = new CModel*[m_nNumPart];
							}
						}
						if (strcmp(&aString[0], PARTSSET) == 0)
						{// パーツ情報のパスがある
							while (strcmp(&aString[0], END_PARTSSET) != 0)
							{// 情報を読み込み終えるまで繰り返し
								fscanf(pFile, "%s", &aString[0]);  // テキストファイルを読み進める
								if (strcmp(&aString[0], INDEX) == 0)
								{// パーツ番号の情報がある
									fscanf(pFile, "%s %d", &aString[0], &nIdx);
								}
								if (strcmp(&aString[0], PARENT) == 0)
								{// 親のパーツ番号の情報がある
									fscanf(pFile, "%s %d", &aString[0], &nIdxParent);
								}
								if (strcmp(&aString[0], POS) == 0)
								{// 親パーツからのオフセット位置の情報がある
									fscanf(pFile, "%s %f %f %f", &aString[0], &ModelOffsetPos.x, &ModelOffsetPos.y, &ModelOffsetPos.z);
								}
								if (strcmp(&aString[0], ROT) == 0)
								{// 親パーツからのオフセット向きの情報がある
									fscanf(pFile, "%s %f %f %f", &aString[0], &ModelOffsetRot.x, &ModelOffsetRot.y, &ModelOffsetRot.z);
								}
							}
							if (m_apModel != NULL)
							{// メモリが確保できた
								m_apModel[nIdx] = CModel::Create(ModelOffsetPos, ModelOffsetRot, m_apMesh[nIdx], m_apBuffMat[nIdx], m_anNumMat[nIdx], m_aVtxMin[nIdx], m_aVtxMax[nIdx]);
								if (nIdxParent > -1)
								{// 親パーツがある
									if (m_apModel[nIdxParent] != NULL)
									{// 親パーツへのポインタが確保されている
										m_apModel[nIdx]->SetParent(m_apModel[nIdxParent]);
									}
								}
							}
						}
					}
				}
			}
			fclose(pFile);    // ファイルを閉じる
		}
	}

	// モーションの生成
	if (m_pMotionManager == NULL)
	{// メモリが確保できる状態にある
		m_pMotionManager = CMotionManager::Create(STATE_MAX, m_nNumPart, m_aFileName);
	}


	// モデルのオフセット向きの設定
	if (m_apModel != NULL && m_pMotionManager != NULL)
	{// メモリが確保できた
		for (int nCntModel = 0; nCntModel < m_nNumPart; nCntModel++)
		{// モデルを生成する数だけ繰り返し
			m_apModel[nCntModel]->SetRot(m_pMotionManager->GetMotion()[m_State]->GetKeyframe()[0][nCntModel]->GetDestRot());   // 向きを反映
		}
	}

	// 影の生成
	if (m_pShadow == NULL)
	{// メモリが確保できる状態である
		m_pShadow = CShadow::Create(D3DXVECTOR3(m_Pos.x, 0.0f, m_Pos.z), m_Rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f), 20.0f, 5);
	}

	return S_OK;
}

//=============================================================================
//    プレイヤーの終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// モデルの破棄
	if (m_apModel != NULL)
	{// メモリが確保されている
		for (int nCntModel = 0; nCntModel < m_nNumPart; nCntModel++)
		{// モデルを生成する数だけ繰り返し
			if (m_apModel[nCntModel] != NULL)
			{// メモリが確保されている
				// モデルの終了処理
				m_apModel[nCntModel]->Uninit();

				// メモリの開放
				delete m_apModel[nCntModel];
				m_apModel[nCntModel] = NULL;
			}
		}

		// メモリの開放
		delete[] m_apModel;
		m_apModel = NULL;
	}

	// モーションの破棄
	if (m_pMotionManager != NULL)
	{// メモリが確保されている
		// モーションの終了処理
		m_pMotionManager->Uninit();

		// メモリの開放
		delete m_pMotionManager;
		m_pMotionManager = NULL;
	}

	// 影の破棄
	if (m_pShadow != NULL)
	{// メモリが確保されている
		// 影の終了処理
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	// プレイヤーオブジェクトの削除
	Release();
}

//=============================================================================
//    プレイヤーの更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// デバッグ情報表示
	CDebugProc::Print("\n( プレイヤー操作方法 )\n");
	CDebugProc::Print("[ 移動 ]       : [ W ] [ S ] [ A ] [ D ] [ 左アナログスティック ] [ 十字キー ]\n");
	CDebugProc::Print("[ ジャンプ ]   : [ ENTER ] [ Aボタン ]\n");
	CDebugProc::Print("[ アクション ] : [ SPACE ] [ Xボタン ]\n\n");

	// ポインタを空にしておく
	if (m_pLandScene != NULL)
	{// アドレスが存在する
		m_pLandScene = NULL;
	}

	// 前回の座標更新
	m_PosOld = m_Pos;
	m_OffsetPosAddOld = m_OffsetPosAdd;

	if (m_State != STATE_PANCH && m_State != STATE_ZIPLINE)
	{// パンチ状態ではないかつジップラインにつかまっている状態ではない
		// アクション処理
		Action();

		// 移動処理
		Movement();
	}

	if (m_State == STATE_PANCH)
	{
		// 現在の座標に移動量を加える
		m_Pos += m_Move;

		// 重力を加える
		m_Move.y += m_fGravity;

		// 移動量に慣性を加える
		m_Move.x += (0.0f - m_Move.x) * m_fInertia;
		m_Move.z += (0.0f - m_Move.z) * m_fInertia;
	}

	if (m_State != STATE_ZIPLINE)
	{// ジップラインにつかまっている状態ではない
		// 当たり判定処理
		Collision();
	}

	// 状態遷移処理
	Statement();

	// モーションの更新処理
	if (m_pMotionManager != NULL)
	{// メモリが確保されている
		m_pMotionManager->Update(m_apModel);
	}

	// エフェクトの生成処理
	CreateEffect();

	// 音の再生
	Sound();

	// プレイヤーのデバッグ情報表示
	CDebugProc::Print("プレイヤーの現在の座標 : ( %.1f %.1f %.1f )\n", m_Pos.x, m_Pos.y, m_Pos.z);
	CDebugProc::Print("プレイヤーの前回の座標 : ( %.1f %.1f %.1f )\n", m_PosOld.x, m_PosOld.y, m_PosOld.z);
	CDebugProc::Print("プレイヤーの向き       : ( %.2f %.2f %.2f )\n", m_Rot.x, m_Rot.y, m_Rot.z);
	CDebugProc::Print("プレイヤーの移動量     : ( %.1f  %.1f  %.1f )\n", m_Move.x, m_Move.y, m_Move.z);

	if (m_bAction == true)
	{
		CDebugProc::Print("アクションしている\n");
	}
	else
	{
		CDebugProc::Print("アクションしていない\n");
	}

	CDebugProc::Print("プレイヤーの状態       : ");
	switch (m_State)
	{// 状態によって処理わけ
	case STATE_NORMAL:
		CDebugProc::Print("NORMAL\n");
		break;
	case STATE_MOVE_WALK:
		CDebugProc::Print("MOVE_WALK\n");
		break;
	case STATE_MOVE_RUN:
		CDebugProc::Print("MOVE_RUN\n");
		break;
	case STATE_JUMP:
		CDebugProc::Print("JUMP\n");
		break;
	case STATE_LANDING:
		CDebugProc::Print("LANDING\n");
		break;
	case STATE_PUSH:
		CDebugProc::Print("PUSH\n");
		break;
	case STATE_PANCH:
		CDebugProc::Print("PANCH\n");
		break;
	case STATE_KICK:
		CDebugProc::Print("KICK\n");
		break;
	case STATE_ZIPLINE:
		CDebugProc::Print("ZIPLINE\n");
		break;
	}
}

//=============================================================================
//    プレイヤーの描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
	        // プレイヤーのワールドマトリックス計算処理
			CalcMtxWorld(pDevice);

			if (m_apModel != NULL)
			{// メモリが確保できた
			    // モデルの描画
				for (int nCntModel = 0; nCntModel < m_nNumPart; nCntModel++)
				{// モデルを生成する数だけ繰り返し
					// モデルの描画処理
					m_apModel[nCntModel]->Draw();
				}
			}
		}
	}
}

//=============================================================================
//    プレイヤーのワールドマトリックス計算処理
//=============================================================================
void CPlayer::CalcMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    プレイヤーの移動処理
//=============================================================================
void CPlayer::Movement(void)
{
	// 移動量計算処理
	CGame *pGame = CManager::GetGame();
	if (pGame != NULL)
	{// ゲームクラスが取得できた
		if (pGame->GetState() != CGame::STATE_PLAYERFALL
			&& pGame->GetState() != CGame::STATE_MAPCLEAR
			&& pGame->GetState() != CGame::STATE_MAPCHANGE
			&& pGame->GetState() != CGame::STATE_END)
		{// プレイヤーが死んだ状態ではないかつマップクリア状態ではないかつマップ切り替え状態ではない
			AddMovement();
		}
	}

	// 現在の座標に移動量を加える
	m_Pos += m_Move;

	// 重力を加える
	m_Move.y += m_fGravity;

	// 移動量に慣性を加える
	m_Move.x += (0.0f - m_Move.x) * m_fInertia;
	m_Move.z += (0.0f - m_Move.z) * m_fInertia;

	// 角度の修正
	m_DiffAngle.y = m_DestAngle.y - m_Rot.y;   // 現在の向きと目的の向きの差分を計算

	if (m_DiffAngle.y > D3DX_PI)
	{// 差分がD3DX_PIを超えた
		m_DiffAngle.y -= D3DX_PI * 2.0f;
	}
	if (m_DiffAngle.y < -D3DX_PI)
	{// 差分が-D3DX_PIを超えた
		m_DiffAngle.y += D3DX_PI * 2.0f;
	}

	// 現在の向きに目的の向きとの差分を倍率で補正する
	m_Rot.y += m_DiffAngle.y * m_fRivisionRot;

	if (m_Rot.y > D3DX_PI)
	{// 現在の向きがD3DX_PIを超えた
		m_Rot.y -= D3DX_PI * 2.0f;
	}
	if (m_Rot.y < -D3DX_PI)
	{// 現在の向きが-D3DX_PIを超えた
		m_Rot.y += D3DX_PI * 2.0f;
	}
}

//=============================================================================
//    プレイヤーの移動量計算処理
//=============================================================================
void CPlayer::AddMovement(void)
{
	// キーボードを取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// ジョイパッドを取得
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

	// カメラを取得
	CCamera *pCamera = CManager::GetCamera();

	if (pKeyboard != NULL)
	{// キーボードが取得できた
		if (pKeyboard->GetPress(DIK_A) == true)
		{// 左方向の入力がされた
			if (pKeyboard->GetPress(DIK_W) == true)
			{// 同時に上方向の入力がされた
			    // 移動量計算処理
				m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * (m_fAccel);
				m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * (m_fAccel);

				// 目的の角度変更
				m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);
				return;
			}
			else if (pKeyboard->GetPress(DIK_S) == true)
			{// 同時に下方向の入力がされた
			    // 移動量計算処理
				m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * (m_fAccel);
				m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * (m_fAccel);

				// 目的の角度変更
				m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);
				return;
			}
			else
			{// 何も押されてない
			    // 移動量計算処理
				m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * (m_fAccel);
				m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * (m_fAccel);

				// 目的の角度変更
				m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
				return;
			}
		}
		else if (pKeyboard->GetPress(DIK_D) == true)
		{// 右方向の入力がされた
			if (pKeyboard->GetPress(DIK_W) == true)
			{// 同時に上方向の入力がされた
			    // 移動量計算処理
				m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * (m_fAccel);
				m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * (m_fAccel);

				// 目的の角度変更
				m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * -0.75f);
				return;
			}
			else if (pKeyboard->GetPress(DIK_S) == true)
			{// 同時に下方向の入力がされた
			    // 移動量計算処理
				m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * (m_fAccel);
				m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * (m_fAccel);

				// 目的の角度変更
				m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * -0.25f);
				return;
			}
			else
			{// 何も押されてない
			    // 移動量計算処理
				m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * (m_fAccel);
				m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * (m_fAccel);

				// 目的の角度変更
				m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * -0.5f);
				return;
			}
		}
		else if (pKeyboard->GetPress(DIK_W) == true)
		{// 上方向の入力がされた
		    // 移動量計算処理
			m_Move.x += sinf(pCamera->GetRot().y) * (m_fAccel);
			m_Move.z += cosf(pCamera->GetRot().y) * (m_fAccel);

			// 目的の角度変更
			m_DestAngle.y = pCamera->GetRot().y + D3DX_PI;
			return;
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// 下方向の入力がされた
	     	// 移動量計算処理
			m_Move.x += sinf(pCamera->GetRot().y + D3DX_PI) * (m_fAccel);
			m_Move.z += cosf(pCamera->GetRot().y + D3DX_PI) * (m_fAccel);

			// 目的の角度変更
			m_DestAngle.y = pCamera->GetRot().y;
			return;
		}
	}
	if (pJoyStick != NULL)
	{// ジョイスティックが取得できた
		if (pJoyStick->GetLeftAxisX() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisX() <= -DI_STICK_AXIS_MIN
			|| pJoyStick->GetLeftAxisY() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisY() <= -DI_STICK_AXIS_MIN)
		{// 左アナログ入力がされている
			// スティックの倒された長さの比率を計算
			float fLengthX = (float)(sqrt(pJoyStick->GetLeftAxisX() * pJoyStick->GetLeftAxisX() + pJoyStick->GetLeftAxisY() * pJoyStick->GetLeftAxisY()) / DI_JOY_STICK_INPUT);

			// コサインカーブで比率を補正
			float fRivisionX = cosf(sqrtf(2) - fLengthX);
			if (fRivisionX < 1.0)
			{// 倍率が1を超えていない
				fRivisionX += 0.09f;  // 足りない分を補正
				if (fRivisionX >= 1.0f)
				{// 倍率が1を超えてしまった
					fRivisionX = 1.0f;
				}
			}

		    // 移動量計算処理
			m_Move.x += sinf(pJoyStick->GetLeftAxis() - pCamera->GetRot().y) * (m_fAccel * fRivisionX);
			m_Move.z -= cosf(pJoyStick->GetLeftAxis() - pCamera->GetRot().y) * (m_fAccel * fRivisionX);

			// 目的の角度変更
			m_DestAngle.y = pCamera->GetRot().y - pJoyStick->GetLeftAxis();
			return;
		}
		else if (pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_28) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_29) == true
			|| pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_30) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_31) == true)
		{// 十字キー入力がされている
		    // 移動量計算処理
			m_Move.x += sinf(pCamera->GetRot().y + pJoyStick->GetRadian()) * (m_fAccel);
			m_Move.z += cosf(pCamera->GetRot().y + pJoyStick->GetRadian()) * (m_fAccel);

			// 目的の角度変更
			m_DestAngle.y = pCamera->GetRot().y + (pJoyStick->GetRadian() - D3DX_PI);
			return;
		}
	}
}

//=============================================================================
//    プレイヤーの当たり判定処理処理
//=============================================================================
bool CPlayer::Collision(void)
{
	bool bLand = false;    // 乗っているかどうか
	bool bPush = false;    // すでに押しているかどうか
	bool bCol = false;     // オブジェクトに当たっているかどうか

	// フィールドと配置物を取得する
	CScene *pScene = NULL;               // シーンクラスへのポインタ
	CScene *pSceneNext = NULL;           // 次のシーンクラスへのポインタ
	CField *pField = NULL;               // 地面クラスへのポインタ
	CWater *pWater = NULL;               // 水クラスへのポインタ
	CObject *pObject = NULL;             // 配置物クラスへのポインタ
	CObjectMove *pObjectMove = NULL;     // 動く配置物クラスへのポインタ
	CObjectGoal *pObjectGoal = NULL;     // ゴール配置物クラスへのポインタ
	CObjectTarget *pObjectTarget = NULL; // ジップライン配置物クラスへのポインタ

	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 描画優先順位の数だけ繰り返し
		pScene = CScene::GetScene(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_FIELD)
			{// フィールドだったら
				pField = (CField*)pScene;
				if (pField != NULL)
				{// フィールドが取得できた
					D3DXVECTOR3 PolygonNormal;
					float fFieldHeight = pField->GetPolyHeight(m_Pos, &PolygonNormal);  // ポリゴンの高さを取得
					if (m_Pos.y <= fFieldHeight)
					{// ポリゴンの高さより低くなった
						m_Pos.y = fFieldHeight;
						m_Move.y = 0.0f;
						if (m_bJump == true)
						{// ジャンプしていたら
							m_bJump = false;

							// 着地したときの処理
							Landing();
						}
					}
					if (m_pShadow != NULL)
					{// 影が生成されている
						if (m_pShadow->GetPos() == m_pShadow->GetPosOld())
						{// 影の位置更新がされている
							m_pShadow->SetPos(D3DXVECTOR3(m_Pos.x, fFieldHeight + 1.0f, m_Pos.z));
							// 頂点情報の設定
							VERTEX_3D *pVtx;
							LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pShadow->GetVtxBuff();
							// 頂点バッファをロックし,頂点データへのポインタを取得
							pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

							for (int nCntVer = 0; nCntVer < 4; nCntVer++)
							{// 影の頂点数分繰り返し
								float DotX = (PolygonNormal.x * pVtx[nCntVer].pos.x);       // X成分の内積を計算
								float DotZ = (PolygonNormal.z * pVtx[nCntVer].pos.z);       // Z成分の内積を計算
								float Dot = -DotX - DotZ;                                   // X成分とZ成分の内積同士を引く
								pVtx[nCntVer].pos.y = (Dot / PolygonNormal.y);              // 引いたものをY成分の法線で割る
							}

							// 頂点バッファをアンロックする
							pVtxBuff->Unlock();

							// 頂点バッファの設定
							m_pShadow->SetVtxBuff(pVtxBuff);
						}
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_WATER)
			{// 水だったら
				pWater = (CWater*)pScene;
				if (pWater != NULL)
				{// 水が取得できた
					D3DXVECTOR3 PolygonNormal;
					float fWaterHeight = pWater->GetPolyHeight(m_Pos, &PolygonNormal);  // ポリゴンの高さを取得
					if (m_Pos.y + m_fColHeight <= fWaterHeight)
					{// ポリゴンの高さより低くなった
						CGame *pGame = CManager::GetGame();
						if (pGame != NULL)
						{// ゲームクラスが取得できた
							if (pGame->GetState() != CGame::STATE_PLAYERFALL)
							{// プレイヤーが落ちた状態ではない
								pWater->SetState(CWater::STATE_RIPPLE);
								pWater->SetRipplesHeight(80.0f);
								pWater->SetRipplePos(D3DXVECTOR3(m_Pos.x, m_Pos.y + m_fColHeight, m_Pos.z));
								pGame->SetState(CGame::STATE_PLAYERFALL);
								m_pShadow->SetPos(D3DXVECTOR3(m_Pos.x, -400.0f + 1.0f, m_Pos.z));
								CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WATER);
							}
						}
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// 配置物だったら
				pObject = (CObject*)pScene;

				// ぶっ壊し判定
				if (pObject != NULL)
				{// ポインタを確保できている
					if (pObject->GetBreak() == true)
					{// 壊せるオブジェクトだったら
						if (AttackCollision(pObject->GetPos(), pObject->GetVtxMin(), pObject->GetVtxMax()) == true)
						{// 攻撃がヒットした
							// 配置物を破棄する
							pObject->Uninit();
							pObject = NULL;

							D3DXVECTOR3 EffectOffSetPos = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
							D3DXMATRIX EffectMtxWorld;

							D3DXMATRIX mtxRot, mtxTrans; // 計算用マトリックス

							// ワールドマトリックスの初期化
							D3DXMatrixIdentity(&EffectMtxWorld);

							// 回転を反映
							D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
							D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &mtxRot);

							// 位置を反映
							D3DXMatrixTranslation(&mtxTrans, EffectOffSetPos.x, EffectOffSetPos.y, EffectOffSetPos.z);
							D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &mtxTrans);

							// 親の情報を反映
							D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &m_apModel[PLAYER_PANCH_COLINDEX]->GetMtxWorld());

							CParEmitter::Create(D3DXVECTOR3(EffectMtxWorld._41, EffectMtxWorld._42, EffectMtxWorld._43), 1, 6);


							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BREAK);
						}
					}
				}

				if (pObject != NULL)
				{// 配置物が取得できた
					if (pObject->GetCollision() == true)
					{// 配置物と当たり判定を取るならば
						// 配置物との判定開始
						if (pObject->Collision(&m_Pos, &m_PosOld, &m_Move, m_pShadow, m_fColHeight, PLAYER_COL_RANGE) == true)
						{// 配置物の上に乗っている
							if (m_pLandScene == NULL && m_bLand == true && m_bJump == false)
							{// 何かに乗っていた
								m_Pos.y = pObject->GetPos().y + pObject->GetVtxMax().y;

								// 影の位置をずらす
								if (m_pShadow != NULL)
								{// 影が生成されている
									m_pShadow->SetPos(D3DXVECTOR3(m_Pos.x, pObject->GetPos().y + pObject->GetVtxMax().y + 1.0f, m_Pos.z));
								}
							}
							m_pLandScene = pObject;
							if (m_bLand == false)
							{// 前回乗っていなかった
								if (m_bJump == true)
								{// ジャンプしていたら
									m_bJump = false;

									// 着地したときの処理
									Landing();
								}
								D3DXVECTOR3 ObjRot = pObject->GetRot();                                                           // 向きを保存
								m_OffsetPosStd = D3DXVECTOR3(m_Pos.x - pObject->GetPos().x, 0.0f, m_Pos.z - pObject->GetPos().z); // オフセット座標を保存
								m_OffsetRot = m_Rot - ObjRot;                                                                     // プレイヤーのオフセット向きを保存
								if (m_OffsetRot.y > D3DX_PI)
								{// 円周率を超えた
									m_OffsetRot.y -= D3DX_PI * 2.0f;
								}
								if (m_OffsetRot.y < -D3DX_PI)
								{// 円周率を超えた
									m_OffsetRot.y += D3DX_PI * 2.0f;
								}
								m_OffsetRotStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // オブジェクトのスタート向きを初期化
								m_OffsetPosAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // オフセットに加える値を初期化
								m_bLand = true;                                      // オブジェクトに乗っている判定にする
							}
							if (m_pLandScene != NULL)
							{// 配置物クラスが取得できた
								// 向きを修正する
								if (m_bMove == false)
								{// 移動していなかったら
									D3DXVECTOR3 ObjRot = pObject->GetRot();
									m_Rot = m_OffsetRot + ObjRot;    // 向きを補正
									if (m_Rot.y > D3DX_PI)
									{// 円周率を超えた
										m_Rot.y -= D3DX_PI * 2.0f;
									}
									if (m_Rot.y < -D3DX_PI)
									{// 円周率を超えた
										m_Rot.y += D3DX_PI * 2.0f;
									}
									m_DestAngle.y = m_Rot.y;  // 目的の向きも補正
								}

								// オフセットの向きに配置物の回転スピードを加算する
								m_OffsetRotStart.y += pObject->GetRollSpeed().y;  // オブジェクトが回転するスピードに合わせて補正
								if (m_OffsetRotStart.y > D3DX_PI)
								{// 円周率を超えた
									m_OffsetRotStart.y -= D3DX_PI * 2.0f;
								}
								else if (m_OffsetRotStart.y < -D3DX_PI)
								{// 円周率を超えた
									m_OffsetRotStart.y += D3DX_PI * 2.0f;
								}
								// オフセット位置にY軸回転に沿わせた移動量を計算する
								if (m_State != STATE_PANCH && m_State != STATE_LANDING)
								{
									m_OffsetPosAdd.x += cosf(0.0f) * (cosf(m_OffsetRotStart.y) * m_Move.x + -sinf(m_OffsetRotStart.y) * m_Move.z) + sinf(0.0f) * (sinf(m_OffsetRotStart.y) * m_Move.x + cosf(m_OffsetRotStart.y) * m_Move.z);
									m_OffsetPosAdd.z += -sinf(0.0f) * (cosf(m_OffsetRotStart.y) * m_Move.x + -sinf(m_OffsetRotStart.y) * m_Move.z) + cosf(0.0f) * (sinf(m_OffsetRotStart.y) * m_Move.x + cosf(m_OffsetRotStart.y) * m_Move.z);
								}

								// 位置をY軸回転に沿わせて移動させる
								float fMtxX = cosf(m_OffsetRotStart.y) * (m_OffsetPosStd.x + (m_OffsetPosAdd.x * 1.5f)) + sinf(m_OffsetRotStart.y) * (m_OffsetPosStd.z + (m_OffsetPosAdd.z * 1.5f));
								float fMtxZ = -sinf(m_OffsetRotStart.y) * (m_OffsetPosStd.x + (m_OffsetPosAdd.x * 1.5f)) + cosf(m_OffsetRotStart.y) * (m_OffsetPosStd.z + (m_OffsetPosAdd.z * 1.5f));
								m_Pos.x = fMtxX + pObject->GetPos().x;
								m_Pos.z = fMtxZ + pObject->GetPos().z;

								// デバッグ情報表示
								CDebugProc::Print("オフセットの基準位置   : ( %.1f  %.1f  %.1f )\n", m_OffsetPosStd.x, m_OffsetPosStd.y, m_OffsetPosStd.z);
								CDebugProc::Print("オフセットに加える位置 : ( %.1f  %.1f  %.1f )\n", m_OffsetPosAdd.x, m_OffsetPosAdd.y, m_OffsetPosAdd.z);
								CDebugProc::Print("オフセットの向き       : ( %.1f  %.1f  %.1f )\n", m_OffsetRot.x, m_OffsetRot.y, m_OffsetRot.z);
								CDebugProc::Print("乗ってからの回転値     : ( %.1f  %.1f  %.1f )\n", m_OffsetRotStart.x, m_OffsetRotStart.y, m_OffsetRotStart.z);
								bLand = true;
							}
							CDebugProc::Print("配置物に乗っています\n\n");
						}
						else
						{// オブジェクトに乗っていない
							if (pObject->Collision(&m_Pos, &m_PosOld, &m_Move, m_fColHeight, PLAYER_COL_RANGE) == true)
							{// オブジェクトに当たっている
								bCol = true;
								D3DXVECTOR3 ObjPos;
								if (m_pLandScene != NULL)
								{// 乗っているオブジェクトがある
									if (m_pLandScene->GetObjType() == OBJTYPE_OBJECT)
									{// 配置物のクラスだったら
										CObject *pLandObject = (CObject*)m_pLandScene;
										ObjPos = pLandObject->GetPos();
									}
									if (m_pLandScene->GetObjType() == OBJTYPE_OBJECTMOVE)
									{// 動く配置物のクラスだったら
										CObjectMove *pLandObject = (CObjectMove*)m_pLandScene;
										ObjPos = pLandObject->GetPos();
									}
									m_OffsetPosStd = D3DXVECTOR3(m_Pos.x - ObjPos.x, 0.0f, m_Pos.z - ObjPos.z); // オフセット座標を保存
									m_OffsetPosAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									m_OffsetPosAddOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								}
								CDebugProc::Print("配置物に当たっています\n");
								if (m_bJump != true)
								{// ジャンプしていない
									if (pObject->GetPush() == true)
									{// 押せるオブジェクトである
										if (m_State != STATE_PUSH)
										{// 押すモーションになっていない
											// 何かを押している状態に
											m_State = STATE_PUSH;

											// モーション切り替え処理
											m_pMotionManager->SwitchMotion(m_apModel, m_State);
											bPush = true;
										}
									}
								}
							}
						}
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_OBJECTMOVE)
			{// 動く配置物だったら
				pObjectMove = (CObjectMove*)pScene;
				if (pObjectMove != NULL)
				{// 動く配置物が取得できた
					if (pObjectMove->GetCollision() == true)
					{// 動く配置物と当たり判定を取るならば
						// 動く配置物の判定開始
						if (pObjectMove->Collision(&m_Pos, &m_PosOld, &m_Move, m_pShadow, m_fColHeight, PLAYER_COL_RANGE) == true)
						{// 動く配置物の中にいる
							if (m_pLandScene == NULL && m_bLand == true && m_bJump == false)
							{// 何かに乗っていた
								m_Pos.y = pObjectMove->GetPos().y + pObjectMove->GetVtxMax().y;
								// 影の位置をずらす
								if (m_pShadow != NULL)
								{// 影が生成されている
									m_pShadow->SetPos(D3DXVECTOR3(m_Pos.x, m_Pos.y + 1.0f, m_Pos.z));
								}
							}
							m_pLandScene = pObjectMove;
							if (m_bLand == false)
							{// 前回乗っていなかった
								if (m_bJump == true)
								{// ジャンプしていたら
									m_bJump = false;

									// 着地したときの処理
									Landing();
								}
								D3DXVECTOR3 ObjRot = pObjectMove->GetRot();                                                                // 向きを保存
								m_OffsetPosStd = D3DXVECTOR3(m_Pos.x - pObjectMove->GetPos().x, 0.0f, m_Pos.z - pObjectMove->GetPos().z);  // オフセット座標を保存
								m_OffsetRot = m_Rot - ObjRot;                                                                              // プレイヤーのオフセット向きを保存
								if (m_OffsetRot.y > D3DX_PI)
								{// 円周率を超えた
									m_OffsetRot.y -= D3DX_PI * 2.0f;
								}
								if (m_OffsetRot.y < -D3DX_PI)
								{// 円周率を超えた
									m_OffsetRot.y += D3DX_PI * 2.0f;
								}
								m_OffsetRotStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // オブジェクトのスタート向きを初期化
								m_OffsetPosAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // オフセットに加える値を初期化
								m_bLand = true;                                      // オブジェクトに乗っている判定にする
								m_Pos.y = pObjectMove->GetPos().y + pObjectMove->GetVtxMax().y;
							}

							if (m_pLandScene != NULL)
							{// 動く配置物クラスが取得できた
								// 向きを修正する
								if (m_bMove == false)
								{// 移動していなかったら
									D3DXVECTOR3 ObjRot = pObjectMove->GetRot();
									m_Rot = m_OffsetRot + ObjRot;    // 向きを補正
									if (m_Rot.y > D3DX_PI)
									{// 円周率を超えた
										m_Rot.y -= D3DX_PI * 2.0f;
									}
									if (m_Rot.y < -D3DX_PI)
									{// 円周率を超えた
										m_Rot.y += D3DX_PI * 2.0f;
									}
									m_DestAngle.y = m_Rot.y;  // 目的の向きも補正
								}

								// オフセットの向きに配置物の回転スピードを加算する
								m_OffsetRotStart.y += pObjectMove->GetRollSpeed().y;  // オブジェクトが回転するスピードに合わせて補正
								if (m_OffsetRotStart.y > D3DX_PI)
								{// 円周率を超えた
									m_OffsetRotStart.y -= D3DX_PI * 2.0f;
								}
								else if (m_OffsetRotStart.y < -D3DX_PI)
								{// 円周率を超えた
									m_OffsetRotStart.y += D3DX_PI * 2.0f;
								}
								// オフセット位置にY軸回転に沿わせた移動量を計算する
								if (m_State != STATE_PANCH && m_State != STATE_LANDING)
								{
									m_OffsetPosAdd.x += cosf(0.0f) * (cosf(m_OffsetRotStart.y) * m_Move.x + -sinf(m_OffsetRotStart.y) * m_Move.z) + sinf(0.0f) * (sinf(m_OffsetRotStart.y) * m_Move.x + cosf(m_OffsetRotStart.y) * m_Move.z);
									m_OffsetPosAdd.z += -sinf(0.0f) * (cosf(m_OffsetRotStart.y) * m_Move.x + -sinf(m_OffsetRotStart.y) * m_Move.z) + cosf(0.0f) * (sinf(m_OffsetRotStart.y) * m_Move.x + cosf(m_OffsetRotStart.y) * m_Move.z);
								}

								// 位置をY軸回転に沿わせて移動させる
								float fMtxX = cosf(m_OffsetRotStart.y) * (m_OffsetPosStd.x + (m_OffsetPosAdd.x * 1.5f)) + sinf(m_OffsetRotStart.y) * (m_OffsetPosStd.z + (m_OffsetPosAdd.z * 1.5f));
								float fMtxZ = -sinf(m_OffsetRotStart.y) * (m_OffsetPosStd.x + (m_OffsetPosAdd.x * 1.5f)) + cosf(m_OffsetRotStart.y) * (m_OffsetPosStd.z + (m_OffsetPosAdd.z * 1.5f));
								m_Pos.x = fMtxX + pObjectMove->GetPos().x;
								m_Pos.z = fMtxZ + pObjectMove->GetPos().z;

								// デバッグ情報表示
								CDebugProc::Print("オフセットの基準位置   : ( %.1f  %.1f  %.1f )\n", m_OffsetPosStd.x, m_OffsetPosStd.y, m_OffsetPosStd.z);
								CDebugProc::Print("オフセットに加える位置 : ( %.1f  %.1f  %.1f )\n", m_OffsetPosAdd.x, m_OffsetPosAdd.y, m_OffsetPosAdd.z);
								CDebugProc::Print("オフセットの向き       : ( %.1f  %.1f  %.1f )\n", m_OffsetRot.x, m_OffsetRot.y, m_OffsetRot.z);
								CDebugProc::Print("乗ってからの回転値     : ( %.1f  %.1f  %.1f )\n", m_OffsetRotStart.x, m_OffsetRotStart.y, m_OffsetRotStart.z);
								bLand = true;
							}
							CDebugProc::Print("動く配置物に乗っています\n\n");
						}
					}
				}
			}

			// ゴール用配置物
			else if (pScene->GetObjType() == OBJTYPE_OBJECTGOAL)
			{// ゴール用配置物だったら
				pObjectGoal = (CObjectGoal*)pScene;
				if (pObjectGoal != NULL)
				{// ゴール用の配置物が取得できた
					if (pObjectGoal->GetCollision() == true)
					{// ゴール用の配置物と当たり判定を取るならば
						if (pObjectGoal->Collision(&m_Pos, &m_PosOld, &m_Move, m_fColHeight, PLAYER_COL_RANGE) == true)
						{// ゴールに触れた
							D3DXVECTOR3 ObjPos;
							if (m_pLandScene != NULL)
							{// 乗っているオブジェクトがある
								if (m_pLandScene->GetObjType() == OBJTYPE_OBJECT)
								{// 配置物のクラスだったら
									CObject *pLandObject = (CObject*)m_pLandScene;
									ObjPos = pLandObject->GetPos();
								}
								if (m_pLandScene->GetObjType() == OBJTYPE_OBJECTMOVE)
								{// 動く配置物のクラスだったら
									CObjectMove *pLandObject = (CObjectMove*)m_pLandScene;
									ObjPos = pLandObject->GetPos();
								}
								m_OffsetPosStd = D3DXVECTOR3(m_Pos.x - ObjPos.x, 0.0f, m_Pos.z - ObjPos.z); // オフセット座標を保存
								m_OffsetPosAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								m_OffsetPosAddOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}

							CGame *pGame = CManager::GetGame();
							if (pGame != NULL)
							{// ゲームクラスが取得できた
								if (pGame->GetMapClear() == false)
								{// マップ未クリア状態である
									if (pGame->GetState() != CGame::STATE_MAPCLEAR)
									{// マップクリア状態になっていない
										pGame->SetMapClear(true);
										pGame->SetState(CGame::STATE_MAPCLEAR);
									}
								}
							}
						}
					}
				}
			}


			// ジップライン用配置物
			else if (pScene->GetObjType() == OBJTYPE_OBJECTTARGET)
			{// ゴール用配置物だったら
				pObjectTarget = (CObjectTarget*)pScene;
				if (pObjectTarget != NULL)
				{// ポインタが取得できている
					if (pObjectTarget->GetCollision() == true)
					{// 当たり判定を取るならば
						if (m_State != STATE_ZIPLINE)
						{// ジップラインにつかまっているモーションになっていない
							if (pObjectTarget->Collision(&m_Pos, &m_PosOld, &m_Move, m_pShadow, m_fColHeight, PLAYER_COL_RANGE, this) == true)
							{// つかまっている
								// ジップラインにつかまっている状態に
								m_State = STATE_ZIPLINE;

								// モーション切り替え処理
								m_pMotionManager->SwitchMotion(m_apModel, m_State);
								bPush = true;
							}
						}
					}
				}
			}
			pScene = pSceneNext;  // 次のポインタを設定
		}
	}

	if (bLand == false)
	{// 何も乗っていなかった
		m_bLand = false;  // 乗っていない判定にする
	}
	m_bPush = bPush;
	m_bCol = bCol;

	return bLand;   // 乗ったかどうかを返す
}

//=============================================================================
//    プレイヤーのアクション処理
//=============================================================================
void CPlayer::Action(void)
{
	CGame *pGame = CManager::GetGame();
	if (pGame != NULL)
	{// ゲームクラスが取得できた
		if (pGame->GetState() != CGame::STATE_PLAYERFALL
			&& pGame->GetState() != CGame::STATE_MAPCLEAR
			&& pGame->GetState() != CGame::STATE_MAPCHANGE
			&& pGame->GetState() != CGame::STATE_END)
		{// プレイヤーが死んだ状態ではないかつマップクリア状態ではないかつマップ切り替え状態ではない
	        // キーボードを取得
			CInputKeyboard *pKeyboard = CManager::GetKeyboard();

			// ジョイパッドを取得
			CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

			// XInputを取得
			CXInput *pXInput = CManager::GetXInput();

			if (pKeyboard->GetTrigger(DIK_RETURN) == true
				|| pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == true)
			{// ジャンプボタンが入力された
				if (m_bJump == false && m_Move.y >= 0.0f)
				{// ジャンプしていない
					m_Move.y += m_fJumpPower;
					m_bJump = true;

					// 状態をジャンプ状態に
					m_State = STATE_JUMP;

					// モーション切り替え処理
					m_pMotionManager->SwitchMotion(m_apModel, m_State);

					// 音を再生する
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_JUMP);
				}
			}

			if (pKeyboard->GetTrigger(DIK_SPACE) == true
				|| pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_0) == true)
			{// 攻撃ボタンが入力された
				if (m_bAction == false)
				{// アクション状態ではない
					m_bAction = true;
					if (m_bJump == false)
					{// ジャンプしていない
						if (m_State != STATE_PANCH)
						{// パンチ状態ではない
							// 移動量をなくす
							m_Move.x = 0.0f;
							m_Move.z = 0.0f;

							// 状態をパンチ状態に
							m_State = STATE_PANCH;

							// モーション切り替え処理
							m_pMotionManager->SwitchMotion(m_apModel, m_State);
						}
					}
					else
					{// ジャンプしている
						if (m_State != STATE_KICK)
						{// キック状態ではない
							// 状態をキック状態に
							m_State = STATE_KICK;

							// モーション切り替え処理
							m_pMotionManager->SwitchMotion(m_apModel, m_State);
						}
					}
				}
			}
		}
	}
}

//=============================================================================
//    プレイヤーの状態遷移処理
//=============================================================================
void CPlayer::Statement(void)
{
	if (m_bJump != true && m_State != STATE_LANDING && m_State != STATE_PUSH && m_bAction != true && m_bPush != true)
	{// ジャンプしていないかつ着地状態ではないかつアクション状態ではない
		if (m_Move.x <= 0.1f && m_Move.x >= -0.1f
			&& m_Move.z <= 0.1f && m_Move.z >= -0.1f)
		{// 移動していない
			m_bMove = false;
			if (m_State != STATE_NORMAL)
			{// 通常状態ではない
				// 状態を通常の状態に
				m_State = STATE_NORMAL;

				if (m_bLand == true)
				{// オフセット位置をワールドマトリックスを使って割り出す
					if (m_pLandScene != NULL)
					{// プレイヤーが乗っているクラスへのポインタが取得できている
						if (m_pLandScene->GetObjType() == OBJTYPE_OBJECT)
						{// 配置物クラスだったら
							CObject *pObject = (CObject*)m_pLandScene;
							D3DXVECTOR3 ObjRot = pObject->GetRot();
							m_OffsetRot = m_Rot - ObjRot;
							if (m_OffsetRot.y > D3DX_PI)
							{// 円周率を超えた
								m_OffsetRot.y -= D3DX_PI * 2.0f;
							}
							if (m_OffsetRot.y < -D3DX_PI)
							{// 円周率を超えた
								m_OffsetRot.y += D3DX_PI * 2.0f;
							}
						}
						else if (m_pLandScene->GetObjType() == OBJTYPE_OBJECTMOVE)
						{// 動く配置物クラスだったら
							CObjectMove *pObjectMove = (CObjectMove*)m_pLandScene;
							D3DXVECTOR3 ObjRot = pObjectMove->GetRot();
							m_OffsetRot = m_Rot - ObjRot;
							if (m_OffsetRot.y > D3DX_PI)
							{// 円周率を超えた
								m_OffsetRot.y -= D3DX_PI * 2.0f;
							}
							if (m_OffsetRot.y < -D3DX_PI)
							{// 円周率を超えた
								m_OffsetRot.y += D3DX_PI * 2.0f;
							}
						}
					}
				}

				// モーション切り替え処理
				m_pMotionManager->SwitchMotion(m_apModel, m_State);
			}
		}
		else
		{// 移動している
		    // 移動量の長さを計算
			float fMoveLength = sqrtf((m_Move.x * m_Move.x) + (m_Move.z * m_Move.z));

			if (fMoveLength >= PLAYER_MAX_WALKSPEED)
			{// 移動量が既定の値より大きい
				if (m_State != STATE_MOVE_RUN)
				{// 走る状態ではない
					m_State = STATE_MOVE_RUN;

					// モーション切り替え処理
					m_pMotionManager->SwitchMotion(m_apModel, m_State);
				}
			}
			else
			{// スティックの軸が既定の値より小さい
				if (m_State != STATE_MOVE_WALK)
				{// 歩く状態ではない
					m_State = STATE_MOVE_WALK;

					// モーション切り替え処理
					m_pMotionManager->SwitchMotion(m_apModel, m_State);
				}
			}

			if (m_bMove == false)
			{// 移動している状態ではない
				// 移動している状態に
				m_bMove = true;
			}
		}
	}
	else
	{// ジャンプしているか、着地状態である
		if (m_bLand == true)
		{// オフセット位置をワールドマトリックスを使って割り出す
			if (m_pLandScene != NULL)
			{// プレイヤーが乗っているクラスへのポインタが取得できている
				if (m_pLandScene->GetObjType() == OBJTYPE_OBJECT)
				{// 配置物クラスだったら
					CObject *pObject = (CObject*)m_pLandScene;
					D3DXVECTOR3 ObjRot = pObject->GetRot();
					m_OffsetRot = m_Rot - ObjRot;
					if (m_OffsetRot.y > D3DX_PI)
					{// 円周率を超えた
						m_OffsetRot.y -= D3DX_PI * 2.0f;
					}
					if (m_OffsetRot.y < -D3DX_PI)
					{// 円周率を超えた
						m_OffsetRot.y += D3DX_PI * 2.0f;
					}
				}
				else if (m_pLandScene->GetObjType() == OBJTYPE_OBJECTMOVE)
				{// 動く配置物クラスだったら
					CObjectMove *pObjectMove = (CObjectMove*)m_pLandScene;
					D3DXVECTOR3 ObjRot = pObjectMove->GetRot();
					m_OffsetRot = m_Rot - ObjRot;
					if (m_OffsetRot.y > D3DX_PI)
					{// 円周率を超えた
						m_OffsetRot.y -= D3DX_PI * 2.0f;
					}
					if (m_OffsetRot.y < -D3DX_PI)
					{// 円周率を超えた
						m_OffsetRot.y += D3DX_PI * 2.0f;
					}
				}
			}
		}

		if (m_State == STATE_LANDING && m_pMotionManager->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP
			|| m_State == STATE_LANDING && sqrtf((m_Move.x * m_Move.x) + (m_Move.z * m_Move.z)) >= PLAYER_MAX_WALKSPEED)
		{// 着地のモーション再生が終わったか移動している
			// 状態を通常の状態に
			m_State = STATE_NORMAL;

			// モーション切り替え処理
			m_pMotionManager->SwitchMotion(m_apModel, m_State);
		}

		if (m_bAction == true && m_State != STATE_PANCH)
		{// アクションのモーション再生が終わった
			// アクションをしていない状態にする
			m_bAction = false;
		}

		if (m_State == STATE_PANCH && m_pMotionManager->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
		{// アクションのモーション再生が終わった
		    // アクションをしていない状態にする
			m_bAction = false;

			// 状態を通常の状態に
			m_State = STATE_NORMAL;

			// モーション切り替え処理
			m_pMotionManager->SwitchMotion(m_apModel, m_State);
		}

		if (m_State == STATE_PUSH)
		{// 押している状態ではない
			if (m_bPush == false)
			{// 押している状態である
				if (m_bCol == false)
				{// オブジェクトに当たっていない
					if (m_State != STATE_NORMAL)
					{// 通常状態ではない
						// 状態を通常の状態に
						m_State = STATE_NORMAL;

						// モーション切り替え処理
						m_pMotionManager->SwitchMotion(m_apModel, m_State);
					}
				}
			}
		}
	}
}

//=============================================================================
//    プレイヤーが着地したときの処理
//=============================================================================
void CPlayer::Landing(void)
{
	// 移動量の長さを計算
	float fMoveLength = sqrtf((m_Move.x * m_Move.x) + (m_Move.z * m_Move.z));

	if (fMoveLength < PLAYER_MAX_WALKSPEED)
	{// 移動していない
	    // 移動量をなくす
		m_Move.x = 0.0f;
		m_Move.z = 0.0f;

	    // 着地状態に
		m_State = STATE_LANDING;

		// モーション切り替え処理
		m_pMotionManager->SwitchMotion(m_apModel, m_State);
	}
	else
	{// 移動している
		if (fMoveLength >= PLAYER_MAX_WALKSPEED)
		{// 移動量が既定の値より大きい
			if (m_State != STATE_MOVE_RUN)
			{// 走る状態ではない
				m_State = STATE_MOVE_RUN;

				// モーション切り替え処理
				m_pMotionManager->SwitchMotion(m_apModel, m_State);
			}
		}
		else
		{// スティックの軸が既定の値より小さい
			if (m_State != STATE_MOVE_WALK)
			{// 歩く状態ではない
				m_State = STATE_MOVE_WALK;

				// モーション切り替え処理
				m_pMotionManager->SwitchMotion(m_apModel, m_State);
			}
		}
	}

	m_bJump = false;  // ジャンプしていない状態に
}

//=============================================================================
//    プレイヤーの攻撃判定処理
//=============================================================================
bool CPlayer::AttackCollision(D3DXVECTOR3 pos, D3DXVECTOR3 VtxMin, D3DXVECTOR3 VtxMax)
{
	bool bCol = false;   // 当たったかどうか

	if (m_State == STATE_PANCH)
	{// パンチのモーション中ならば
		// 当たり判定を取る場所を設定
		D3DXVECTOR3 ColPosition = D3DXVECTOR3(m_apModel[PLAYER_PANCH_COLINDEX]->GetMtxWorld()._41, m_apModel[PLAYER_PANCH_COLINDEX]->GetMtxWorld()._42, m_apModel[PLAYER_PANCH_COLINDEX]->GetMtxWorld()._43);
		float fColRange = PLAYER_PANCH_COLRANGE;

		if (ColPosition.x + fColRange >= pos.x + VtxMin.x && ColPosition.x - fColRange <= pos.x + VtxMax.x)
		{// X軸の判定内に入っている
			if (ColPosition.y + fColRange >= pos.y && ColPosition.y - fColRange <= pos.y + VtxMax.y)
			{// Y軸の判定内に入っている
				if (ColPosition.z + fColRange >= pos.z + VtxMin.z && ColPosition.z - fColRange <= pos.z + VtxMax.z)
				{// Z軸の判定内に入っている
					bCol = true;
					CDebugProc::Print("\nパンチがヒットしました\n\n");
				}
			}
		}
	}
	else if (m_State == STATE_KICK)
	{// キックのモーション中ならば
	    // 当たり判定を取る場所を設定
		D3DXVECTOR3 ColPosition = D3DXVECTOR3(m_apModel[PLAYER_KICK_COLINDEX]->GetMtxWorld()._41, m_apModel[PLAYER_KICK_COLINDEX]->GetMtxWorld()._42, m_apModel[PLAYER_KICK_COLINDEX]->GetMtxWorld()._43);
		float fColRange = PLAYER_KICK_COLRANGE;

		if (ColPosition.x + fColRange >= pos.x + VtxMin.x && ColPosition.x - fColRange <= pos.x + VtxMax.x)
		{// X軸の判定内に入っている
			if (ColPosition.y + fColRange >= pos.y && ColPosition.y - fColRange <= pos.y + VtxMax.y)
			{// Y軸の判定内に入っている
				if (ColPosition.z + fColRange >= pos.z + VtxMin.z && ColPosition.z - fColRange <= pos.z + VtxMax.z)
				{// Z軸の判定内に入っている
					bCol = true;
					CDebugProc::Print("\nキックがヒットしました\n\n");
				}
			}
		}
	}

	return bCol;   // 当たったかどうかを返す
}

//=============================================================================
//    プレイヤーのエフェクト生成処理
//=============================================================================
void CPlayer::CreateEffect(void)
{
	if (m_State == STATE_PANCH)
	{// パンチをしていたら
	    // エフェクトの生成
		if (m_pMotionManager != NULL)
		{// モーション管理クラスが生成されている
			if (m_pMotionManager->GetKey() == 3 && m_pMotionManager->GetCounter() == 6)
			{
				D3DXVECTOR3 EffectOffSetPos = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
				D3DXMATRIX EffectMtxWorld;

				D3DXMATRIX mtxRot, mtxTrans; // 計算用マトリックス

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&EffectMtxWorld);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
				D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &mtxRot);

				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, EffectOffSetPos.x, EffectOffSetPos.y, EffectOffSetPos.z);
				D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &mtxTrans);

				// 親の情報を反映
				D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &m_apModel[PLAYER_PANCH_COLINDEX]->GetMtxWorld());

				CParEmitter::Create(D3DXVECTOR3(EffectMtxWorld._41, EffectMtxWorld._42, EffectMtxWorld._43), 1, 6);
			}
		}
	}

	if (m_State == STATE_KICK)
	{// キックをしていたら
	    // エフェクトの生成
		if (m_pMotionManager != NULL)
		{// モーション管理クラスが生成されている
			if (m_pMotionManager->GetKey() == 1 && m_pMotionManager->GetCounter() == 0)
			{
				D3DXVECTOR3 EffectOffSetPos = D3DXVECTOR3(0.0f, -3.0f, 0.0f);
				D3DXMATRIX EffectMtxWorld;

				D3DXMATRIX mtxRot, mtxTrans; // 計算用マトリックス

				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&EffectMtxWorld);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
				D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &mtxRot);

				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, EffectOffSetPos.x, EffectOffSetPos.y, EffectOffSetPos.z);
				D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &mtxTrans);

				// 親の情報を反映
				D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &m_apModel[PLAYER_KICK_COLINDEX]->GetMtxWorld());

				CParEmitter::Create(D3DXVECTOR3(EffectMtxWorld._41, EffectMtxWorld._42, EffectMtxWorld._43), 2, 6);
			}
		}
	}
}

//=============================================================================
//    プレイヤーの音の処理
//=============================================================================
void CPlayer::Sound(void)
{
	if (m_State == STATE_MOVE_WALK)
	{// 歩くモーションだったら
		if (m_pMotionManager != NULL)
		{// モーション管理クラスが生成されている
			if (m_pMotionManager->GetKey() == 0 && m_pMotionManager->GetCounter() == 20)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WALK000);
			}
			else if (m_pMotionManager->GetKey() == 1 && m_pMotionManager->GetCounter() == 20)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WALK001);
			}
		}
	}
	else if (m_State == STATE_MOVE_RUN)
	{// 走るモーションだったら
		if (m_pMotionManager != NULL)
		{// モーション管理クラスが生成されている
			if (m_pMotionManager->GetKey() == 0 && m_pMotionManager->GetCounter() == 3)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WALK000);
			}
			else if (m_pMotionManager->GetKey() == 1 && m_pMotionManager->GetCounter() == 3)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WALK001);
			}
		}
	}
	else if (m_State == STATE_PANCH)
	{// パンチモーションだったら
		if (m_pMotionManager != NULL)
		{// モーション管理クラスが生成されている
			if (m_pMotionManager->GetKey() == 3 && m_pMotionManager->GetCounter() == 0)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PANCH);
			}
		}
	}
	else if (m_State == STATE_KICK)
	{// キックモーションだったら
		if (m_pMotionManager != NULL)
		{// モーション管理クラスが生成されている
			if (m_pMotionManager->GetKey() == 1 && m_pMotionManager->GetCounter() == 3)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_KICK);
			}
		}
	}
	else if (m_State == STATE_LANDING)
	{// 着地モーションだったら
		if (m_pMotionManager != NULL)
		{// モーション管理クラスが生成されている
			if (m_pMotionManager->GetKey() == 0 && m_pMotionManager->GetCounter() == 0)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_LANDING);
			}
		}
	}
	else if (m_State == STATE_PUSH)
	{// 押すモーションだったら
		if (m_pMotionManager != NULL)
		{// モーション管理クラスが生成されている
			if (m_pMotionManager->GetKey() == 0 && m_pMotionManager->GetCounter() == 0)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PUSH);
			}
		}
	}
}

//=============================================================================
//    プレイヤーの位置取得処理
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    プレイヤーの向き取得処理
//=============================================================================
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    プレイヤーの移動量取得処理
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    プレイヤーの当たり判定を取る高さ取得処理
//=============================================================================
float CPlayer::GetColHeight(void)
{
	return m_fColHeight;
}

//=============================================================================
//    プレイヤーのジャンプしているかどうか取得処理
//=============================================================================
bool CPlayer::GetJump(void)
{
	return m_bJump;
}

//=============================================================================
//    プレイヤーの状態設定処理
//=============================================================================
void CPlayer::SetState(const STATE state)
{
	m_State = state;
	m_pMotionManager->SwitchMotion(m_apModel, m_State);
}

//=============================================================================
//    プレイヤーの現在の座標設定処理
//=============================================================================
void CPlayer::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    プレイヤーの前回の座標設定処理
//=============================================================================
void CPlayer::SetPosOld(const D3DXVECTOR3 posOld)
{
	m_PosOld = posOld;
}

//=============================================================================
//    プレイヤーの向き設定処理
//=============================================================================
void CPlayer::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
	m_DestAngle = rot;
}

//=============================================================================
//    プレイヤーの移動量設定処理
//=============================================================================
void CPlayer::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    プレイヤーのジャンプしているかどうか設定処理
//=============================================================================
void CPlayer::SetJump(const bool bJump)
{
	m_bJump = bJump;
}

//=============================================================================
//    プレイヤーのデフォルト設定処理
//=============================================================================
void CPlayer::SetDefault(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 座標を戻す
	m_Pos = pos;
	m_PosOld = pos;

	// 向きを戻す
	m_Rot = rot;
	m_DestAngle = rot;

	// モデルのオフセット向きの設定
	if (m_apModel != NULL && m_pMotionManager != NULL)
	{// メモリが確保できた
		for (int nCntModel = 0; nCntModel < m_nNumPart; nCntModel++)
		{// モデルを生成する数だけ繰り返し
			m_apModel[nCntModel]->SetRot(m_pMotionManager->GetMotion()[m_State]->GetKeyframe()[0][nCntModel]->GetDestRot());   // 向きを反映
		}
	}

	// モーションを戻す
	if (m_pMotionManager != NULL)
	{// モージョンクラスのポインタが確保できている
		// 状態を戻す
		m_State = STATE_NORMAL;

		// モーション切り替え処理
		m_pMotionManager->SwitchMotion(m_apModel, m_State);
	}
}


// モデル単体のプレイヤー処理なので今回これは使わないです
#if 0
//*****************************************************************************
//     CSceneXPlayerの処理
//*****************************************************************************
//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define PLAYER_MODEL_FILENAME   "data/MODEL/01_head.x"   // プレイヤーモデルのファイル名

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************
LPD3DXMESH     CSceneXPlayer::m_apMesh = NULL;      // メッシュ情報へのポインタ
LPD3DXBUFFER   CSceneXPlayer::m_apBuffMat = NULL;   // マテリアル情報へのポインタ
DWORD          CSceneXPlayer::m_anNumMat = NULL;    // マテリアル情報の数

//=============================================================================
//    コンストラクタ
//=============================================================================
CSceneXPlayer::CSceneXPlayer(int nPriority, OBJTYPE objType) : CSceneX(nPriority, objType)
{
	// 各種値のクリア
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 移動量
	m_DiffAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 現在の向きと目的の向きの差分
	m_DestAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 目的の向き
	m_fGravity = 0.0f;                            // 重力
	m_fJumpPower = 0.0f;                          // ジャンプ力
	m_bJump = false;                              // ジャンプしたかどうか
	m_fAccel = 0.0f;                              // 加速度
	m_fInertia = 0.0f;                            // 慣性
}

//=============================================================================
//    デストラクタ
//=============================================================================
CSceneXPlayer::~CSceneXPlayer()
{

}

//=============================================================================
//    プレイヤー(モデル単体)の生成処理
//=============================================================================
CSceneXPlayer *CSceneXPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority)
{
	CSceneXPlayer *pSceneXPlayer = NULL;   // プレイヤー(モデル単体)クラス型のポインタ
	if (pSceneXPlayer == NULL)
	{// メモリが空になっている
		pSceneXPlayer = new CSceneXPlayer(nPriority);
		if (pSceneXPlayer != NULL)
		{// インスタンスを生成できた
			if (FAILED(pSceneXPlayer->Init(pos, rot)))
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

	return pSceneXPlayer;   // インスタンスのアドレスを返す
}

//=============================================================================
//    プレイヤー(モデル単体)の読み込み処理
//=============================================================================
HRESULT CSceneXPlayer::Load(void)
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
			D3DXLoadMeshFromX(PLAYER_MODEL_FILENAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_apBuffMat,
				NULL,
				&m_anNumMat,
				&m_apMesh);
		}
	}

	return S_OK;
}

//=============================================================================
//    プレイヤー(モデル単体)の開放処理
//=============================================================================
void CSceneXPlayer::UnLoad(void)
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
//    プレイヤー(モデル単体)の初期化処理
//=============================================================================
HRESULT CSceneXPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 各種値の設定
	SetPos(pos);                         // 現在の座標
	SetRot(rot);                         // 現在の向き
	m_fGravity = PLAYER_GRAVITY_INI;     // 重力
	m_fJumpPower = PLAYER_JUMPPOWER_INI; // ジャンプ力
	m_fAccel = PLAYER_ACCELERATION_INI;  // 加速度
	m_fInertia = PLAYER_INERTIA_INI;     // 慣性

	// 共通の初期化処理
	CSceneX::Init();

	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // モデルの割り当て
			BindModel(pDevice, m_apMesh, m_apBuffMat, m_anNumMat);
		}
	}

	// 影の生成
	if (m_pShadow == NULL)
	{// メモリが確保できる状態である
		m_pShadow = CShadow::Create(D3DXVECTOR3(pos.x, 0.0f, pos.z), rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f), 70.0f, 4);
	}

	return S_OK;
}

//=============================================================================
//    プレイヤー(モデル単体)の終了処理
//=============================================================================
void CSceneXPlayer::Uninit(void)
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
//    プレイヤー(モデル単体)の更新処理
//=============================================================================
void CSceneXPlayer::Update(void)
{
	// アクション処理
	Action();

	// 移動処理
	Movement();

	// 当たり判定処理
	Collision();
}

//=============================================================================
//    プレイヤー(モデル単体)の描画処理
//=============================================================================
void CSceneXPlayer::Draw(void)
{
	// 共通の描画処理
	CSceneX::Draw();
}

//=============================================================================
//    プレイヤー(モデル単体)のワールドマトリックス計算処理
//=============================================================================
void CSceneXPlayer::CalcMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXVECTOR3 pos = GetPos();           // 現在の位置
	D3DXVECTOR3 rot = GetRot();           // 現在の向き
	D3DXMATRIX MtxWorld = GetMtxWorld();  // ワールドマトリックス
	D3DXMATRIX mtxRot, mtxTrans;          // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&MtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &MtxWorld);
	SetMtxWorld(MtxWorld);
}

//=============================================================================
//    プレイヤー(モデル単体)の移動処理
//=============================================================================
void CSceneXPlayer::Movement(void)
{
	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();        // 現在の座標
	D3DXVECTOR3 posOld = GetPosOld();  // 前回の座標
	D3DXVECTOR3 rot = GetRot();        // 現在の向き

	// 前回の座標更新
	posOld = pos;

	// キーボードを取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// カメラを取得
	CCamera *pCamera = CManager::GetCamera();

	if (pKeyboard->GetPress(DIK_A) == true)
	{// 左方向の入力がされた
		if (pKeyboard->GetPress(DIK_W) == true)
		{// 同時に上方向の入力がされた
		    // 移動処理
			m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;
			m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;

			// 目的の角度変更
			m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// 同時に下方向の入力がされた
		    // 移動処理
			m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;
			m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;

			// 目的の角度変更
			m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);
		}
		else
		{// 何も押されてない
		    // 移動処理
			m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;
			m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;

			// 目的の角度変更
			m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
		}
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{// 右方向の入力がされた
		if (pKeyboard->GetPress(DIK_W) == true)
		{// 同時に上方向の入力がされた
		    // 移動処理
			m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;
			m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;

			// 目的の角度変更
			m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * -0.75f);
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// 同時に下方向の入力がされた
		    // 移動処理
			m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;
			m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;

			// 目的の角度変更
			m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * -0.25f);
		}
		else
		{// 何も押されてない
		    // 移動処理
			m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;
			m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;

			// 目的の角度変更
			m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * -0.5f);
		}
	}
	else if (pKeyboard->GetPress(DIK_W) == true)
	{// 上方向の入力がされた
	    // 移動処理
		m_Move.x += sinf(pCamera->GetRot().y) * m_fAccel;
		m_Move.z += cosf(pCamera->GetRot().y) * m_fAccel;

		// 目的の角度変更
		m_DestAngle.y = pCamera->GetRot().y + D3DX_PI;
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{// 下方向の入力がされた
	    // 移動処理
		m_Move.x += sinf(pCamera->GetRot().y + D3DX_PI) * m_fAccel;
		m_Move.z += cosf(pCamera->GetRot().y + D3DX_PI) * m_fAccel;

		// 目的の角度変更
		m_DestAngle.y = pCamera->GetRot().y;
	}

	// 現在の座標に移動量を加える
	pos += m_Move;

	// 重力を加える
	m_Move.y += m_fGravity;

	// 移動量に慣性を加える
	m_Move.x += (0.0f - m_Move.x) * m_fInertia;
	m_Move.z += (0.0f - m_Move.z) * m_fInertia;

	// 角度の修正
	m_DiffAngle.y = m_DestAngle.y - rot.y;   // 現在の向きと目的の向きの差分を計算

	if (m_DiffAngle.y > D3DX_PI)
	{// 差分がD3DX_PIを超えた
		m_DiffAngle.y -= D3DX_PI * 2.0f;
	}
	if (m_DiffAngle.y < -D3DX_PI)
	{// 差分が-D3DX_PIを超えた
		m_DiffAngle.y += D3DX_PI * 2.0f;
	}

	// 現在の向きに目的の向きとの差分を倍率で補正する
	rot.y += m_DiffAngle.y * PLAYER_ANGLE_REVISION;

	if (rot.y > D3DX_PI)
	{// 現在の向きがD3DX_PIを超えた
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{// 現在の向きが-D3DX_PIを超えた
		rot.y += D3DX_PI * 2.0f;
	}

	// 各種値の設定
	SetPos(pos);        // 現在の座標
	SetPosOld(posOld);  // 前回の座標
	SetRot(rot);        // 現在の向き
}

//=============================================================================
//    プレイヤー(モデル単体)の当たり判定処理処理
//=============================================================================
bool CSceneXPlayer::Collision(void)
{
	bool bCol = false;                 // 当たったかどうか
	D3DXVECTOR3 pos = GetPos();        // プレイヤーの現在の座標
	D3DXVECTOR3 posOld = GetPosOld();  // プレイヤーの前回の座標

	// フィールドと配置物を取得する
	CScene *pScene = NULL;               // シーンクラスへのポインタ
	CScene *pSceneNext = NULL;           // 次のシーンクラスへのポインタ
	CField *pField = NULL;               // 地面クラスへのポインタ
	CObject *pObject = NULL;             // 配置物クラスへのポインタ
	CObjectMove *pObjectMove = NULL;     // 動く配置物クラスへのポインタ
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 描画優先順位の数だけ繰り返し
		pScene = CScene::GetScene(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_FIELD)
			{// フィールドだったら
				pField = (CField*)pScene;
			}
			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// 配置物だったら
				pObject = (CObject*)pScene;
			}
			if (pScene->GetObjType() == OBJTYPE_OBJECTMOVE)
			{// 配置物だったら
				pObjectMove = (CObjectMove*)pScene;
			}
			pScene = pSceneNext;
		}
	}

	if (pObject != NULL)
	{// 配置物が取得できた
	    // 当たり判定に必要な値を配置物から取得
		D3DXVECTOR3 ObjectPos = pObject->GetPos();          // 現在の座標
		D3DXVECTOR3 ObjectPosOld = pObject->GetPosOld();    // 前回の座標
		D3DXVECTOR3 ObjectVtxMin = pObject->GetVtxMin();    // 頂点の座標最小値
		D3DXVECTOR3 ObjectVtxMax = pObject->GetVtxMax();    // 頂点の座標最大値

		// 配置物との判定開始
		if (pos.x > ObjectPos.x + ObjectVtxMin.x && pos.x < ObjectPos.x + ObjectVtxMax.x
			&& pos.z > ObjectPos.z + ObjectVtxMin.z && pos.z < ObjectPos.z + ObjectVtxMax.z)
		{// 配置物の中にいる
			CDebugProc::Print("配置物の中にいます\n");
			if (posOld.y >= ObjectPos.y + ObjectVtxMax.y && pos.y <= ObjectPos.y + ObjectVtxMax.y)
			{// 配置物に上から入った
				pos.y = ObjectPos.y + ObjectVtxMax.y;
				m_Move.y = 0.0f;
				if (m_bJump == true)
				{// ジャンプしていたら
					m_bJump = false;
				}
				CDebugProc::Print("配置物に乗っています\n");
			}
			if (posOld.y + 15.0f <= ObjectPos.y + ObjectVtxMin.y && pos.y + 15.0f > ObjectPos.y + ObjectVtxMin.y)
			{// 配置物に下から入った
				pos.y = ObjectPos.y + ObjectVtxMin.y - 15.0f;
				m_Move.y = 0.0f;
			}

			// 影の位置をずらす
			if (m_pShadow != NULL)
			{// 影が生成されている
				if (pos.y >= ObjectPos.y + ObjectVtxMax.y)
				{// 配置物の上にいる
					m_pShadow->SetPos(D3DXVECTOR3(pos.x, ObjectPos.y + ObjectVtxMax.y + 1.0f, pos.z));
				}
			}
		}
	}

	if (pObjectMove != NULL)
	{// 動く配置物が取得できた
	    // 当たり判定に必要な値を動く配置物から取得
		D3DXVECTOR3 ObjectPos = pObjectMove->GetPos();                  // 現在の座標
		D3DXVECTOR3 ObjectPosOld = pObjectMove->GetPosOld();            // 前回の座標
		D3DXVECTOR3 ObjectMove = pObjectMove->GetMove();                // 移動量
		D3DXVECTOR3 ObjectVtxMin = pObjectMove->GetVtxMin();            // 頂点の座標最小値
		D3DXVECTOR3 ObjectVtxMax = pObjectMove->GetVtxMax();            // 頂点の座標最大値
		D3DXVECTOR3 ObjectPosNext = pObjectMove->GetPos() + ObjectMove; // 次回の座標

		// 動く配置物の判定開始
		if (pos.x > ObjectPos.x + ObjectVtxMin.x && pos.x < ObjectPos.x + ObjectVtxMax.x
			&& pos.z > ObjectPos.z + ObjectVtxMin.z && pos.z < ObjectPos.z + ObjectVtxMax.z)
		{// 配置物の中にいる
			CDebugProc::Print("配置物の中にいます\n");
			if (posOld.y >= ObjectPos.y + ObjectVtxMax.y && pos.y <= ObjectPos.y + ObjectVtxMax.y
				|| ObjectMove.y >= 0.0f && posOld.y >= ObjectPos.y + ObjectVtxMax.y && pos.y <= ObjectPos.y + ObjectVtxMax.y
				|| posOld.y >= ObjectPosOld.y + ObjectVtxMax.y && pos.y <= ObjectPosOld.y + ObjectVtxMax.y
				|| ObjectMove.y >= 0.0f && posOld.y >= ObjectPosOld.y + ObjectVtxMax.y && pos.y <= ObjectPosOld.y + ObjectVtxMax.y)
			{// 配置物に上から入った
				pos.y = ObjectPos.y + ObjectVtxMax.y + ObjectMove.y;
				pos.x += ObjectMove.x;
				pos.z += ObjectMove.z;
				m_Move.y = 0.0f;
				if (m_bJump == true)
				{// ジャンプしていたら
					m_bJump = false;
				}
				CDebugProc::Print("配置物に乗っています\n");
			}
			if (posOld.y + 15.0f <= ObjectPos.y + ObjectVtxMin.y && pos.y + 15.0f > ObjectPos.y + ObjectVtxMin.y
				|| posOld.y + 15.0f <= ObjectPosOld.y + ObjectVtxMin.y && pos.y + 15.0f > ObjectPosOld.y + ObjectVtxMin.y)
			{// 配置物に下から入った
				pos.y = ObjectPos.y + ObjectVtxMin.y - 15.0f;
				m_Move.y = 0.0f;
			}

			// 影の位置をずらす
			if (m_pShadow != NULL)
			{// 影が生成されている
				if (pos.y >= ObjectPos.y + ObjectVtxMax.y || posOld.y >= ObjectPos.y + ObjectVtxMax.y)
				{// 配置物の上にいる
					m_pShadow->SetPos(D3DXVECTOR3(pos.x, ObjectPos.y + ObjectVtxMax.y + 1.0f + ObjectMove.y, pos.z));
				}
			}
		}
	}


	if (pField != NULL)
	{// フィールドが取得できた
		D3DXVECTOR3 PolygonNormal;
		float fFieldHeight = pField->GetPolyHeight(pos, &PolygonNormal);  // ポリゴンの高さを取得
		if (pos.y < fFieldHeight)
		{// ポリゴンの高さより低くなった
			pos.y = fFieldHeight;
			m_Move.y = 0.0f;
			if (m_bJump == true)
			{// ジャンプしていたら
				m_bJump = false;
			}
		}
		if (m_pShadow != NULL)
		{// 影が生成されている
			m_pShadow->SetPos(D3DXVECTOR3(pos.x, fFieldHeight + 1.0f, pos.z));
			// 頂点情報の設定
			VERTEX_3D *pVtx;
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pShadow->GetVtxBuff();
			// 頂点バッファをロックし,頂点データへのポインタを取得
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCntVer = 0; nCntVer < 4; nCntVer++)
			{// 影の頂点数分繰り返し
				float DotX = (PolygonNormal.x * pVtx[nCntVer].pos.x);       // X成分の内積を計算
				float DotZ = (PolygonNormal.z * pVtx[nCntVer].pos.z);       // Z成分の内積を計算
				float Dot = -DotX - DotZ;                                   // X成分とZ成分の内積同士を引く
				pVtx[nCntVer].pos.y = (Dot / PolygonNormal.y);              // 引いたものをY成分の法線で割る
			}

			// 頂点バッファをアンロックする
			pVtxBuff->Unlock();

			// 頂点バッファの設定
			m_pShadow->SetVtxBuff(pVtxBuff);
		}
	}

	// 各種値の設定
	SetPos(pos);       // 現在の座標

	CDebugProc::Print("\nプレイヤーの現在の座標 : ( %.1f %.1f %.1f )\n", pos.x, pos.y, pos.z);
	CDebugProc::Print("プレイヤーの前回の座標 : ( %.1f %.1f %.1f )\n\n", posOld.x, posOld.y, posOld.z);

	return bCol;   // 当たったかどうかを返す
}

//=============================================================================
//    プレイヤー(モデル単体)のアクション処理
//=============================================================================
void CSceneXPlayer::Action(void)
{
	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();  // 座標
	D3DXVECTOR3 rot = GetRot();  // 向き

	// キーボードを取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_RETURN) == true)
	{// ENTERキーが押された
		if (m_bJump == false)
		{// ジャンプしていない
			m_Move.y += m_fJumpPower;
			m_bJump = true;
		}
	}
}

//=============================================================================
//    プレイヤー(モデル単体)の移動量取得処理
//=============================================================================
D3DXVECTOR3 CSceneXPlayer::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    プレイヤー(モデル単体)のジャンプしているかどうか取得処理
//=============================================================================
bool CSceneXPlayer::GetJump(void)
{
	return m_bJump;
}

//=============================================================================
//    プレイヤー(モデル単体)の移動量設定処理
//=============================================================================
void CSceneXPlayer::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    プレイヤー(モデル単体)のジャンプしているかどうか設定処理
//=============================================================================
void CSceneXPlayer::SetJump(const bool bJump)
{
	m_bJump = bJump;
}

#endif