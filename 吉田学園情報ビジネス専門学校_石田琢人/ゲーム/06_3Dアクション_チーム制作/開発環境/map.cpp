//*****************************************************************************
//
//     マップの処理[map.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "map.h"
#include "object.h"
#include "dome.h"
#include "cylinder.h"
#include "field.h"
#include "water.h"
#include "modelManager.h"
#include "player.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
// スクリプトファイルの読み込み用のパス名
#define MAPSET               "MAPSET"                // マップセット開始の合図
#define END_MAPSET           "END_MAPSET"            // マップセット終了の合図
#define FIELDSET             "FIELDSET"              // 地面セット開始の合図
#define END_FIELDSET         "END_FIELDSET"          // 地面セット終了の合図
#define WATERSET             "WATERSET"              // 水セット開始の合図
#define END_WATERSET         "END_WATERSET"          // 水セット終了の合図
#define OBJECTSET            "OBJECTSET"             // 配置物セット開始の合図
#define END_OBJECTSET        "END_OBJECTSET"         // 配置物セット終了の合図
#define OBJECTMOVESET        "OBJECTMOVESET"         // 動く配置物セット開始の合図
#define END_OBJECTMOVESET    "END_OBJECTMOVESET"     // 動く配置物セット終了の合図
#define OBJECTBALLOONSET     "OBJECTBALLOONSET"      // 吹き出し用の配置物セット開始の合図
#define END_OBJECTBALLOONSET "END_OBJECTBALLOONSET"  // 吹き出し用の配置物セット終了の合図
#define OBJECTGOALSET        "OBJECTGOALSET"         // ゴール用の配置物セット開始の合図
#define END_OBJECTGOALSET    "END_OBJECTGOALSET"     // ゴール用の配置物セット終了の合図
#define OBJECTTARGETSET      "OBJECTTARGETSET"       // ジップライン用の配置物セット開始の合図
#define END_OBJECTTARGETSET  "END_OBJECTTARGETSET"   // ジップライン用の配置物セット終了の合図
#define MOUNTAINSET          "MOUNTAINSET"           // 山セット開始の合図
#define END_MOUNTAINSET      "END_MOUNTAINSET"       // 山セット終了の合図
#define SKYSET               "SKYSET"                // 空セット開始の合図
#define END_SKYSET           "END_SKYSET"            // 空セット終了の合図

#define FIELD_FILENAME       "FIELD_FILENAME"        // 地面のスクリプトファイル名
#define WATER_FILENAME       "WATER_FILENAME"        // 水のスクリプトファイル名
#define OBJECT_FILENAME      "OBJECT_FILENAME"       // 配置物のスクリプトファイル名
#define MOUNTAIN_FILENAME    "MOUNTAIN_FILENAME"     // 山のスクリプトファイル名
#define SKY_FILENAME         "SKY_FILENAME"          // 空のスクリプトファイル名
#define PLAYER_DEFAULT_POS   "PLAYER_DEFAULT_POS"    // プレイヤーの座標の初期値
#define PLAYER_DEFAULT_ROT   "PLAYER_DEFAULT_ROT"    // プレイヤーの向きの初期値
#define CAMERA_DEFAULT_ROT   "CAMERA_DEFAULT_ROT"    // カメラの向きの初期値

#define NUM_MODEL            "NUM_MODEL"             // 読み込むモデル数
#define MODEL_FILENAME       "MODEL_FILENAME"        // 読み込むモデルファイル名

#define TYPE                 "TYPE"                  // 種類情報
#define POS                  "POS"                   // 座標情報
#define POS_TARGET           "POS_TARGET"            // 目的の位置
#define ROT                  "ROT"                   // 向き情報
#define MOVE                 "MOVE"                  // 移動量情報
#define PUSH                 "PUSH"                  // 押せるかどうか
#define BREAK                "BREAK"                 // 壊せるかどうか
#define ROLL_SPEED           "ROLL_SPEED"            // 回転するスピード量情報
#define CARRY_TIME           "CARRY_TIME"            // プレイヤーを運ぶのにかける時間
#define COLLISION            "COLLISION"             // 当たり判定をするかどうか
#define WIDTH                "WIDTH"                 // 1ブロック分の幅情報
#define DEPTH                "DEPTH"                 // 1ブロック分奥行情報
#define RADIUS               "RADIUS"                // 半径情報
#define HEIGHT               "HEIGHT"                // 高さ情報
#define XBLOCK               "XBLOCK"                // 横の分割数情報
#define YBLOCK               "YBLOCK"                // 縦の分割数情報
#define ZBLOCK               "ZBLOCK"                // 奥行の分割数情報
#define VERTEX_FILENAME      "VERTEX_FILENAME"       // 頂点情報のバイナリファイル名
#define NEXT_MAPNAME         "NEXT_MAPNAME"          // 次のマップデータのファイル名

//=============================================================================
//    コンストラクタ
//=============================================================================
CMap::CMap()
{
	// 各種値の初期化
	m_pModelManager = NULL;  // モデル管轄クラスへのポインタ
	for (int nCntWater = 0; nCntWater < MAX_WATER; nCntWater++)
	{// 配置できる水の数だけ繰り返し
		m_pWater[nCntWater] = NULL;         // 水クラスへのポインタ
	}
	m_pField = NULL;         // 地面クラスへのポインタ
	m_pMoutain = NULL;       // 円筒クラスへのポインタ
	m_pSky = NULL;           // 空クラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMap::~CMap()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMap *CMap::Create(char *aFileName)
{
	CMap *pMap = NULL;  // マップクラス型のポインタ
	if (pMap == NULL)
	{// メモリが確保できる状態にある
		pMap = new CMap;
		if (pMap != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pMap->Init(aFileName);
		}
		else
		{// メモリが確保できなかった
			return NULL;
		}
	}
	else
	{// メモリが確保できる状態にない
		return NULL;
	}

	// インスタンスを返す
	return pMap;
}

//=============================================================================
//    初期化処理
//=============================================================================
void CMap::Init(char *aFileName)
{
	FILE *pFile = NULL;      // ファイルポインタ
	char aString[256];       // テキスト格納用文字列
	char aFileString[256];   // ファイル名格納用文字列

	// 読み込むマップのスクリプトファイル名を保存
	strcpy(m_aFileName, aFileName);

	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// テキストが最後の行に到達するまで読み込みを続ける
				if (strcmp(&aString[0], MAPSET) == 0)
				{// マップセット読み込み開始の合図がある
					while (strcmp(&aString[0], END_MAPSET) != 0)
					{// マップセットを読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストを読み進める
						if (strcmp(&aString[0], FIELD_FILENAME) == 0)
						{// 地面のスクリプトファイル名がある
							if (fscanf(pFile, "%s %s", &aString[0], &aFileString[0]) == 2)
							{// 地面のスクリプトファイル名を読み取れた
								FieldLoad(aFileString);
							}
						}
						if (strcmp(&aString[0], WATER_FILENAME) == 0)
						{// 水のスクリプトファイル名がある
							if (fscanf(pFile, "%s %s", &aString[0], &aFileString[0]) == 2)
							{// 水のスクリプトファイル名を読み取れた
								WaterLoad(aFileString);
							}
						}
						if (strcmp(&aString[0], OBJECT_FILENAME) == 0)
						{// 配置物のスクリプトファイル名がある
							if (fscanf(pFile, "%s %s", &aString[0], &aFileString[0]) == 2)
							{// 配置物のスクリプトファイル名を読み取れた
								ObjectLoad(aFileString);
							}
						}
						if (strcmp(&aString[0], MOUNTAIN_FILENAME) == 0)
						{// 山のスクリプトファイル名がある
							if (fscanf(pFile, "%s %s", &aString[0], &aFileString[0]) == 2)
							{// 山のスクリプトファイル名を読み取れた
								MountainLoad(aFileString);
							}
						}
						if (strcmp(&aString[0], SKY_FILENAME) == 0)
						{// 空のスクリプトファイル名がある
							if (fscanf(pFile, "%s %s", &aString[0], &aFileString[0]) == 2)
							{// 空のスクリプトファイル名を読み取れた
								SkyLoad(aFileString);
							}
						}

						if (strcmp(&aString[0], PLAYER_DEFAULT_POS) == 0)
						{// プレイヤーの初期座標がある
							if (fscanf(pFile, "%s %f %f %f", &aString[0], &m_DefaultPlayerPos.x, &m_DefaultPlayerPos.y, &m_DefaultPlayerPos.z) == 4)
							{// プレイヤーの初期座標を読み取れた
							    // プレイヤークラスへのポインタを取得する
								CPlayer *pPlayer = NULL;
								CScene *pScene = CScene::GetScene(PLAYER_PRIORITY);
								if (pScene != NULL)
								{// ポインタが取得できた
									CScene *pSceneNext;  // 次のポインタ保存用
									while (pScene != NULL)
									{// 次のポインタがなくなるまでループ
										pSceneNext = pScene->GetNext();
										if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
										{// プレイヤークラスのポインタだった
											pPlayer = (CPlayer*)pScene;
											break;
										}
										pScene = pSceneNext;
									}
								}
								if (pPlayer != NULL)
								{// プレイヤーが取得できた
									pPlayer->SetPos(m_DefaultPlayerPos);
									pPlayer->SetPosOld(m_DefaultPlayerPos);
								}
							}
						}
						if (strcmp(&aString[0], PLAYER_DEFAULT_ROT) == 0)
						{// プレイヤーの初期向きがある
							if (fscanf(pFile, "%s %f %f %f", &aString[0], &m_DefaultPlayerRot.x, &m_DefaultPlayerRot.y, &m_DefaultPlayerRot.z) == 4)
							{// プレイヤーの初期座標を読み取れた
							    // プレイヤークラスへのポインタを取得する
								CPlayer *pPlayer = NULL;
								CScene *pScene = CScene::GetScene(PLAYER_PRIORITY);
								if (pScene != NULL)
								{// ポインタが取得できた
									CScene *pSceneNext;  // 次のポインタ保存用
									while (pScene != NULL)
									{// 次のポインタがなくなるまでループ
										pSceneNext = pScene->GetNext();
										if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
										{// プレイヤークラスのポインタだった
											pPlayer = (CPlayer*)pScene;
											break;
										}
										pScene = pSceneNext;
									}
								}
								if (pPlayer != NULL)
								{// プレイヤーが取得できた
									pPlayer->SetRot(m_DefaultPlayerRot);
								}
							}
						}
						if (strcmp(&aString[0], CAMERA_DEFAULT_ROT) == 0)
						{// カメラの初期向きがある
							if (fscanf(pFile, "%s %f %f %f", &aString[0], &m_DefaultCameraRot.x, &m_DefaultCameraRot.y, &m_DefaultCameraRot.z) == 4)
							{// カメラの初期向き
								CManager::GetCamera()->SetDefaultCamera(m_DefaultCameraRot);
							}
						}
					}
				}
			}
			fclose(pFile);
		}
	}
}

//=============================================================================
//    終了処理
//=============================================================================
void CMap::Uninit(void)
{
	// 地面の破棄
	if (m_pField != NULL)
	{// メモリが確保されている
		m_pField->Uninit();
		m_pField = NULL;
	}

	// 配置物の破棄
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetScene(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
			{// 配置物のクラスだったら
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTMOVE)
			{// 動く配置物のクラスだったら
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTBALLOON)
			{// 吹き出し用の配置物のクラスだったら
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTGOAL)
			{// ゴール用配置物のクラスだったら
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTTARGET)
			{// ジップライン用配置物のクラスだったら
				pScene->Uninit();
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}

	//// 配置物の破棄
	//for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	//{// 配置できるオブジェクトの数だけ繰り返し
	//	if (m_pObject[nCntObject] != NULL)
	//	{// メモリが確保されている
	//		m_pObject[nCntObject]->Uninit();
	//		m_pObject[nCntObject] = NULL;
	//	}
	//}

	//// 動く配置物の破棄
	//for (int nCntObjectMove = 0; nCntObjectMove < MAX_OBJECT_MOVE; nCntObjectMove++)
	//{// 配置できるオブジェクトの数だけ繰り返し
	//	if (m_pObjectMove[nCntObjectMove] != NULL)
	//	{// メモリが確保されている
	//		m_pObjectMove[nCntObjectMove]->Uninit();
	//		m_pObjectMove[nCntObjectMove] = NULL;
	//	}
	//}

	//// 吹き出し用配置物の破棄
	//for (int nCntObjectBalloon = 0; nCntObjectBalloon < MAX_OBJECT_BALLOON; nCntObjectBalloon++)
	//{// 配置できるオブジェクトの数だけ繰り返し
	//	if (m_pObjectBalloon[nCntObjectBalloon] != NULL)
	//	{// メモリが確保されている
	//		m_pObjectBalloon[nCntObjectBalloon]->Uninit();
	//		m_pObjectBalloon[nCntObjectBalloon] = NULL;
	//	}
	//}

	//// ゴール用配置物の破棄
	//for (int nCntObjectGoal = 0; nCntObjectGoal < MAX_OBJECT_GOAL; nCntObjectGoal++)
	//{// 配置できるオブジェクトの数だけ繰り返し
	//	if (m_pObjectGoal[nCntObjectGoal] != NULL)
	//	{// メモリが確保されている
	//		m_pObjectGoal[nCntObjectGoal]->Uninit();
	//		m_pObjectGoal[nCntObjectGoal] = NULL;
	//	}
	//}

	//// ジップライン用配置物の破棄
	//for (int nCntObjectTarget = 0; nCntObjectTarget < MAX_OBJECT_TARGET; nCntObjectTarget++)
	//{// 配置できるオブジェクトの数だけ繰り返し
	//	if (m_pObjectTarget[nCntObjectTarget] != NULL)
	//	{// メモリが確保されている
	//		m_pObjectTarget[nCntObjectTarget]->Uninit();
	//		m_pObjectTarget[nCntObjectTarget] = NULL;
	//	}
	//}

	// 水の破棄
	for (int nCntWater = 0; nCntWater < MAX_WATER; nCntWater++)
	{// 配置できるオブジェクトの数だけ繰り返し
		if (m_pWater[nCntWater] != NULL)
		{// メモリが確保されている
			m_pWater[nCntWater]->Uninit();
			m_pWater[nCntWater] = NULL;
		}
	}

	// 山の破棄
	if (m_pMoutain != NULL)
	{// メモリが確保されている
		m_pMoutain->Uninit();
		m_pMoutain = NULL;
	}

	// 空の破棄
	if (m_pSky != NULL)
	{// メモリが確保されている
		m_pSky->Uninit();
		m_pSky = NULL;
	}

	// モデル管轄クラスの破棄
	if (m_pModelManager != NULL)
	{// メモリが確保されている
		m_pModelManager->Uninit();

		// メモリの開放
		delete m_pModelManager;
		m_pModelManager = NULL;
	}
}

//=============================================================================
//    地面情報読み込み処理
//=============================================================================
void CMap::FieldLoad(char *aFieldFileName)
{
	FILE *pFile = NULL;  // ファイルポインタ
	char aString[256];   // テキスト格納用文字列
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(aFieldFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // 地面用の格納変数
			D3DXVECTOR3 FieldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 地面の座標
			D3DXVECTOR3 FieldRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 地面の向き
			float FieldWidth = 0.0f;                               // 地面の1ブロック分の幅
			float FieldDepth = 0.0f;                               // 地面の1ブロック分の奥行
			int FieldXblock = 0;                                   // 地面の横のブロック数
			int FieldZblock = 0;                                   // 地面の奥行のブロック数
			char aFieldVertexFile[256];                            // 地面の頂点情報が格納されたファイル名

			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// テキストが最後の行に到達するまで読み込みを続ける
				if (strcmp(&aString[0], FIELDSET) == 0)
				{// 地面セット読み込み開始の合図がある
					while (strcmp(&aString[0], END_FIELDSET) != 0)
					{// 地面セットを読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストを読み進める
						if (strcmp(&aString[0], POS) == 0)
						{// 地面の座標情報がある
							fscanf(pFile, "%s %f %f %f", &aString[0], &FieldPos.x, &FieldPos.y, &FieldPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// 地面の向き情報がある
							fscanf(pFile, "%s %f %f %f", &aString[0], &FieldRot.x, &FieldRot.y, &FieldRot.z);
						}
						if (strcmp(&aString[0], WIDTH) == 0)
						{// 地面の1ブロック分の幅情報がある
							fscanf(pFile, "%s %f", &aString[0], &FieldWidth);
						}
						if (strcmp(&aString[0], DEPTH) == 0)
						{// 地面の1ブロック分の奥行情報がある
							fscanf(pFile, "%s %f", &aString[0], &FieldDepth);
						}
						if (strcmp(&aString[0], XBLOCK) == 0)
						{// 地面の横の分割数情報がある
							fscanf(pFile, "%s %d", &aString[0], &FieldXblock);
						}
						if (strcmp(&aString[0], ZBLOCK) == 0)
						{// 地面の奥行の分割数の奥行情報がある
							fscanf(pFile, "%s %d", &aString[0], &FieldZblock);
						}
						if (strcmp(&aString[0], VERTEX_FILENAME) == 0)
						{// 地面の頂点情報のバイナリファイル名情報がある
							fscanf(pFile, "%s %s", &aString[0], &aFieldVertexFile[0]);
						}
					}
					// 地面の生成
					if (m_pField == NULL)
					{// メモリが確保できる状態にある
						m_pField = CField::Create(FieldPos, FieldRot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FieldWidth, FieldDepth, FieldXblock, FieldZblock, aFieldVertexFile);
					}
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    地面情報読み込み処理
//=============================================================================
void CMap::WaterLoad(char *aFieldFileName)
{
	FILE *pFile = NULL;  // ファイルポインタ
	char aString[256];   // テキスト格納用文字列
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(aFieldFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // 水用の格納変数
			D3DXVECTOR3 WaterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 水の座標
			D3DXVECTOR3 WaterRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 水の向き
			float WaterWidth = 0.0f;                               // 水の1ブロック分の幅
			float WaterDepth = 0.0f;                               // 水の1ブロック分の奥行
			int WaterXblock = 0;                                   // 水の横のブロック数
			int WaterZblock = 0;                                   // 水の奥行のブロック数
			char aWaterVertexFile[256];                            // 水の頂点情報が格納されたファイル名
			int nCntWater = 0;                                     // 水を配置した数

			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// テキストが最後の行に到達するまで読み込みを続ける
				if (strcmp(&aString[0], WATERSET) == 0)
				{// 水セット読み込み開始の合図がある
					while (strcmp(&aString[0], END_WATERSET) != 0)
					{// 水セットを読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストを読み進める
						if (strcmp(&aString[0], POS) == 0)
						{// 水の座標情報がある
							fscanf(pFile, "%s %f %f %f", &aString[0], &WaterPos.x, &WaterPos.y, &WaterPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// 水の向き情報がある
							fscanf(pFile, "%s %f %f %f", &aString[0], &WaterRot.x, &WaterRot.y, &WaterRot.z);
						}
						if (strcmp(&aString[0], WIDTH) == 0)
						{// 水の1ブロック分の幅情報がある
							fscanf(pFile, "%s %f", &aString[0], &WaterWidth);
						}
						if (strcmp(&aString[0], DEPTH) == 0)
						{// 水の1ブロック分の奥行情報がある
							fscanf(pFile, "%s %f", &aString[0], &WaterDepth);
						}
						if (strcmp(&aString[0], XBLOCK) == 0)
						{// 水の横の分割数情報がある
							fscanf(pFile, "%s %d", &aString[0], &WaterXblock);
						}
						if (strcmp(&aString[0], ZBLOCK) == 0)
						{// 水の奥行の分割数の奥行情報がある
							fscanf(pFile, "%s %d", &aString[0], &WaterZblock);
						}
						if (strcmp(&aString[0], VERTEX_FILENAME) == 0)
						{// 水の頂点情報のバイナリファイル名情報がある
							fscanf(pFile, "%s %s", &aString[0], &aWaterVertexFile[0]);
						}
					}
					// 水の生成
					if (m_pWater[nCntWater] == NULL)
					{// メモリが確保できる状態にある
						m_pWater[nCntWater] = CWater::Create(WaterPos, WaterRot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WaterWidth, WaterDepth, WaterXblock, WaterZblock, aWaterVertexFile, nCntWater);
						nCntWater++;
					}
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    配置物情報読み込み処理
//=============================================================================
void CMap::ObjectLoad(char *aObjectFileName)
{
	FILE *pFile = NULL;  // ファイルポインタ
	char aString[256];   // テキスト格納用文字列
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(aObjectFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // 配置物用の格納変数
			CObject *pObject = NULL;
			CObjectMove *pObjectMove = NULL;
			CObjectBalloon *pObjectBalloon = NULL;
			CObjectGoal *pObjectGoal = NULL;
			CObjectTarget *pObjectTarget = NULL;
			int nNumModel;                   // 読み込むモデルの数
			char aXFileName[256];            // Xファイル名格納用
			LPD3DXMESH pMesh = NULL;         // メッシュ情報格納用
			LPD3DXBUFFER pBuffMat = NULL;    // マテリアル情報格納用
			DWORD nNumMat = NULL;            // マテリアルの数情報格納用
			D3DXVECTOR3 VtxMax;              // 頂点の最代値情報格納用
			D3DXVECTOR3 VtxMin;              // 頂点の最小値情報格納用
			int nCntModelManager = 0;        // 管轄クラスへ割り当てる際のモデル番号
			int nObjectIndex = 0;            // 割り当てるモデルの番号
			D3DXVECTOR3 ObjectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 配置物の座標
			D3DXVECTOR3 ObjectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 配置物の向き
			D3DXVECTOR3 ObjectMove = D3DXVECTOR3(0.0f,0.0f,0.0f);          // 配置物の移動量
			D3DXVECTOR3 ObjectRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 配置物の回転するスピード量
			D3DXVECTOR3 ObjectPosTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 配置物のプレイヤーを運ぶ位置
			bool bCollision = true;                                        // 当たり判定を取るかどうか
			bool bPush = false;                                            // 配置物を押せるかどうか
			bool bBreak = false;                                           // 配置物を壊せるかどうか
			int nMaxCarryTime = 0;                                         // プレイヤーを運ぶのにかける時間
			int nCntObject = 0;                                            // 配置物を置いた数を数える変数
			int nCntObjectMove = 0;                                        // 動く配置物を置いた数を数える変数
			int nCntObjectBalloon = 0;                                     // 吹き出し用配置物を置いた数を数える変数
			int nCntObjectGoal = 0;                                        // ゴール用配置物を置いた数を数える変数
			int nCntObjectTarget = 0;                                      // ジップライン用配置物を置いた数を数える変数
			char aNextMapName[256];                                        // 次のマップデータのファイル名

			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// テキストが最後の行に到達するまで読み込みを続ける
				if (strcmp(&aString[0], NUM_MODEL) == 0)
				{// 読み込むモデル数情報がある
					fscanf(pFile, "%s %d", &aString[0], &nNumModel);
					if (nNumModel >= 1)
					{// 読み込むモデルの数が1個以上ある
						m_pModelManager = CModelManager::Create(nNumModel);
					}
				}

				if (strcmp(&aString[0], MODEL_FILENAME) == 0)
				{// 読み込むモデルのファイル名情報が書かれていたら
					fscanf(pFile, "%s %s", &aString[0], &aXFileName[0]);
					// xファイルの読み込み
					D3DXLoadMeshFromX(aXFileName,
						D3DXMESH_SYSTEMMEM,
						CManager::GetRenderer()->GetDevice(),
						NULL,
						&pBuffMat,
						NULL,
						&nNumMat,
						&pMesh);

					// 頂点の最小値, 最大値情報を読み込む
					if (pMesh != NULL)
					{// メッシュ情報が読み込めた
						int nNumVtx;     // 頂点数
						DWORD sizeFVF;   // 頂点フォーマットのサイズ
						BYTE *pVtxBuff;  // 頂点バッファへのポインタ

						// 頂点の最小値, 最大値情報格納用変数を初期化
						VtxMax = D3DXVECTOR3(-100000000.0f, -100000000.0f, -100000000.0f);
						VtxMin = D3DXVECTOR3(100000000.0f, 100000000.0f, 100000000.0f);

						// 頂点数を獲得
					    nNumVtx = pMesh->GetNumVertices();

						// 頂点フォーマットのサイズを取得
						sizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

						// 頂点バッファをロック
						pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

						for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
						{// すべての頂点を比較してモデルの最小値,最大値を抜き出す
							D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // 頂点座標の代入

							if (Vtx.x < VtxMin.x)
							{// 取り出したX座標の最小値が今までのよりも小さい
								VtxMin.x = Vtx.x;  // 最小値更新
							}
							if (Vtx.y < VtxMin.y)
							{// 取り出したY座標の最小値が今までのよりも小さい
								VtxMin.y = Vtx.y;  // 最小値更新
							}
							if (Vtx.z < VtxMin.z)
							{// 取り出したZ座標の最小値が今までのよりも小さい
								VtxMin.z = Vtx.z;  // 最小値更新
							}
							if (Vtx.x > VtxMax.x)
							{// 取り出したX座標の最大値が今までのよりも大きい
								VtxMax.x = Vtx.x;  // 最大値更新
							}
							if (Vtx.y > VtxMax.y)
							{// 取り出したY座標の最大値が今までのよりも大きい
								VtxMax.y = Vtx.y;  // 最大値更新
							}
							if (Vtx.z > VtxMax.z)
							{// 取り出したZ座標の最大値が今までのよりも大きい
								VtxMax.z = Vtx.z;  // 最大値更新
							}

							pVtxBuff += sizeFVF;   // サイズ分ポインタを進める
						}
						// 頂点バッファをアンロック
						pMesh->UnlockVertexBuffer();

						// モデル管轄クラスへの割り当て
						m_pModelManager->SetMesh(pMesh, nCntModelManager);        // メッシュ情報
						m_pModelManager->SetBuffMat(pBuffMat, nCntModelManager);  // マテリアル情報
						m_pModelManager->SetNumMat(nNumMat, nCntModelManager);    // マテリアルの数情報
						m_pModelManager->SetVtxMax(VtxMax, nCntModelManager);     // 頂点の最大値情報
						m_pModelManager->SetVtxMin(VtxMin, nCntModelManager);     // 頂点の最小値情報

						nCntModelManager++;    // モデル番号を進めておく
					}
				}

				// 配置物
				if (strcmp(&aString[0], OBJECTSET) == 0)
				{// 配置物セット読み込み開始の合図がある
					while (strcmp(&aString[0], END_OBJECTSET) != 0)
					{// 配置物セットを読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストを読み進める
						if (strcmp(&aString[0], TYPE) == 0)
						{// 配置するオブジェクトの番号を読み取る
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// 座標を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// 向きを読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// 回転するスピード量を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// 当たり判定を取るかどうかを読み取る
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
						if (strcmp(&aString[0], PUSH) == 0)
						{// 押せるかどうかを読み取る
							int nPush;
							fscanf(pFile, "%s %d", &aString[0], &nPush);
							if (nPush == 0)
							{
								bPush = false;
							}
							else
							{
								bPush = true;
							}
						}
						if (strcmp(&aString[0], BREAK) == 0)
						{// 壊せるかどうかを読み取る
							int nBreak;
							fscanf(pFile, "%s %d", &aString[0], &nBreak);
							if (nBreak == 0)
							{
								bBreak = false;
							}
							else
							{
								bBreak = true;
							}
						}
					}
					pObject = CObject::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, bPush, bBreak);
					if (pObject != NULL)
					{// メモリが確保できた
					 // モデルの割り当て
						pObject->BindModel(CManager::GetRenderer()->GetDevice(),
							m_pModelManager->GetMesh(nObjectIndex),
							m_pModelManager->GetBuffMat(nObjectIndex),
							m_pModelManager->GetNumMat(nObjectIndex));

						// 頂点の最小値、最大値情報の設定
						pObject->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObject->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObject++;  // 配置物を配置した数を進める

									   // 変数を初期化しておく
						bCollision = true;
						bBreak = false;
						bPush = false;
					}
				}

				// 動く配置物
				if (strcmp(&aString[0], OBJECTMOVESET) == 0)
				{// 動く配置物セット読み込み開始の合図がある
					while (strcmp(&aString[0], END_OBJECTMOVESET) != 0)
					{// 動く配置物セットを読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストを読み進める
						if (strcmp(&aString[0], TYPE) == 0)
						{// 配置するオブジェクトの番号を読み取る
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// 座標を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// 向きを読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], MOVE) == 0)
						{// 移動量を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectMove.x, &ObjectMove.y, &ObjectMove.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// 回転するスピード量を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// 当たり判定を取るかどうかを読み取る
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
					}
					pObjectMove = CObjectMove::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, ObjectMove);
					if (pObjectMove != NULL)
					{// メモリが確保できた
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// モデルの割り当て
						pObjectMove->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// 頂点の最小値、最大値情報の設定
						pObjectMove->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectMove->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectMove++;  // 動く配置物を配置した数を進める

										   // 変数を初期化しておく
						bCollision = true;
					}
				}

				// 吹き出し用
				if (strcmp(&aString[0], OBJECTBALLOONSET) == 0)
				{// 吹き出し用配置物セット読み込み開始の合図がある
					while (strcmp(&aString[0], END_OBJECTBALLOONSET) != 0)
					{// 吹き出し用配置物セットを読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストを読み進める
						if (strcmp(&aString[0], TYPE) == 0)
						{// 配置するオブジェクトの番号を読み取る
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// 座標を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// 向きを読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// 回転するスピード量を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// 当たり判定を取るかどうかを読み取る
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
						if (strcmp(&aString[0], NEXT_MAPNAME) == 0)
						{// 次のマップデータのファイル名を読み取る
							fscanf(pFile, "%s %s", &aString[0], &aNextMapName[0]);
						}
					}

					pObjectBalloon = CObjectBalloon::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, (CObjectBalloon::BALLOON)nCntObjectBalloon, aNextMapName, 4);
					if (pObjectBalloon != NULL)
					{// メモリが確保できた
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// モデルの割り当て
						pObjectBalloon->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// 頂点の最小値、最大値情報の設定
						pObjectBalloon->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectBalloon->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectBalloon++;  // 吹き出し用配置物を配置した数を進める

											  // 変数を初期化しておく
						bCollision = true;
					}
				}

				// ゴール用
				if (strcmp(&aString[0], OBJECTGOALSET) == 0)
				{// ゴール用配置物セット読み込み開始の合図がある
					while (strcmp(&aString[0], END_OBJECTGOALSET) != 0)
					{// ゴール用配置物セットを読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストを読み進める
						if (strcmp(&aString[0], TYPE) == 0)
						{// 配置するオブジェクトの番号を読み取る
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// 座標を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// 向きを読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// 回転するスピード量を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// 当たり判定を取るかどうかを読み取る
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
					}
					pObjectGoal = CObjectGoal::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, 4);
					if (pObjectGoal != NULL)
					{// メモリが確保できた
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// モデルの割り当て
						pObjectGoal->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// 頂点の最小値、最大値情報の設定
						pObjectGoal->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectGoal->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectGoal++;  // ゴール用配置物を配置した数を進める

										   // 変数を初期化しておく
						bCollision = true;
					}
				}


				// ジップライン用
				if (strcmp(&aString[0], OBJECTTARGETSET) == 0)
				{// ジップライン用配置物セット読み込み開始の合図がある
					while (strcmp(&aString[0], END_OBJECTTARGETSET) != 0)
					{// ジップライン用配置物セットを読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストを読み進める
						if (strcmp(&aString[0], TYPE) == 0)
						{// 配置するオブジェクトの番号を読み取る
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// 座標を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// 向きを読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// 回転するスピード量を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], POS_TARGET) == 0)
						{// プレイヤーを運ぶ位置を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPosTarget.x, &ObjectPosTarget.y, &ObjectPosTarget.z);
						}
						if (strcmp(&aString[0], CARRY_TIME) == 0)
						{// プレイヤーを運ぶのにかける時間を読み取る
							fscanf(pFile, "%s %d", &aString[0], &nMaxCarryTime);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// 当たり判定を取るかどうかを読み取る
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
					}
					pObjectTarget = CObjectTarget::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, ObjectPosTarget, nMaxCarryTime);
					if (pObjectTarget != NULL)
					{// メモリが確保できた
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// モデルの割り当て
						pObjectTarget->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// 頂点の最小値、最大値情報の設定
						pObjectTarget->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectTarget->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectTarget++;  // ジップライン用配置物を配置した数を進める

											 // 変数を初期化しておく
						bCollision = true;
					}
				}
			}

			// オブジェクトのファイル名をコピーしておく
			strcpy(m_aObjectFileName, aObjectFileName);

			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    山情報読み込み処理
//=============================================================================
void CMap::MountainLoad(char *aMoutainFileName)
{
	FILE *pFile = NULL;  // ファイルポインタ
	char aString[256];   // テキスト格納用文字列
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(aMoutainFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // 山用の格納変数
			D3DXVECTOR3 MountainPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 山の座標
			D3DXVECTOR3 MountainRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 山の向き
			float MountainRadius;                                     // 山の半径
			float MountainHeight;                                     // 山の高さ
			int MountainXblock = 0;                                   // 山の横のブロック数
			int MountainYblock = 0;                                   // 山の縦のブロック数

			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// テキストが最後の行に到達するまで読み込みを続ける
				if (strcmp(&aString[0], MOUNTAINSET) == 0)
				{// 山セット読み込み開始の合図がある
					while (strcmp(&aString[0], END_MOUNTAINSET) != 0)
					{// 山セットを読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストを読み進める
						if (strcmp(&aString[0], POS) == 0)
						{// 山の座標情報がある
							fscanf(pFile, "%s %f %f %f", &aString[0], &MountainPos.x, &MountainPos.y, &MountainPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// 山の向き情報がある
							fscanf(pFile, "%s %f %f %f", &aString[0], &MountainRot.x, &MountainRot.y, &MountainRot.z);
						}
						if (strcmp(&aString[0], RADIUS) == 0)
						{// 山の半径情報がある
							fscanf(pFile, "%s %f", &aString[0], &MountainRadius);
						}
						if (strcmp(&aString[0], HEIGHT) == 0)
						{// 山の高さ情報がある
							fscanf(pFile, "%s %f", &aString[0], &MountainHeight);
						}
						if (strcmp(&aString[0], XBLOCK) == 0)
						{// 山の横の分割数情報がある
							fscanf(pFile, "%s %d", &aString[0], &MountainXblock);
						}
						if (strcmp(&aString[0], YBLOCK) == 0)
						{// 山の縦の分割数の奥行情報がある
							fscanf(pFile, "%s %d", &aString[0], &MountainYblock);
						}
					}
					// 山の生成
					if (m_pMoutain == NULL)
					{// メモリが確保できる状態にある
						m_pMoutain = CCylinder::Create(MountainPos, MountainRot, MountainRadius, MountainHeight, MountainXblock, MountainYblock);
					}
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    空情報読み込み処理
//=============================================================================
void CMap::SkyLoad(char *aSkyFileName)
{
	FILE *pFile = NULL;  // ファイルポインタ
	char aString[256];   // テキスト格納用文字列
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(aSkyFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // 空用の格納変数
			D3DXVECTOR3 SkyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 空の座標
			D3DXVECTOR3 SkyRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 空の向き
			float SkyRadius;                                     // 空の半径
			int SkyXblock = 0;                                   // 空の横のブロック数
			int SkyYblock = 0;                                   // 空の縦のブロック数

			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// テキストが最後の行に到達するまで読み込みを続ける
				if (strcmp(&aString[0], SKYSET) == 0)
				{// 空セット読み込み開始の合図がある
					while (strcmp(&aString[0], END_SKYSET) != 0)
					{// 空セットを読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストを読み進める
						if (strcmp(&aString[0], POS) == 0)
						{// 空の座標情報がある
							fscanf(pFile, "%s %f %f %f", &aString[0], &SkyPos.x, &SkyPos.y, &SkyPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// 空の向き情報がある
							fscanf(pFile, "%s %f %f %f", &aString[0], &SkyRot.x, &SkyRot.y, &SkyRot.z);
						}
						if (strcmp(&aString[0], RADIUS) == 0)
						{// 空の半径情報がある
							fscanf(pFile, "%s %f", &aString[0], &SkyRadius);
						}
						if (strcmp(&aString[0], XBLOCK) == 0)
						{// 空の横の分割数情報がある
							fscanf(pFile, "%s %d", &aString[0], &SkyXblock);
						}
						if (strcmp(&aString[0], YBLOCK) == 0)
						{// 空の縦の分割数の奥行情報がある
							fscanf(pFile, "%s %d", &aString[0], &SkyYblock);
						}
					}
					// 空の生成
					if (m_pSky == NULL)
					{// メモリが確保できる状態にある
						m_pSky = CDome::Create(SkyPos, SkyRot, SkyRadius, SkyXblock, SkyYblock);
					}
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    配置物をデフォルトの位置に戻す処理
//=============================================================================
void CMap::SetObjectDefault(void)
{
	// 配置物の破棄
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetScene(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
			{// 配置物のクラスだったら
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTMOVE)
			{// 動く配置物のクラスだったら
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTBALLOON)
			{// 吹き出し用の配置物のクラスだったら
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTGOAL)
			{// ゴール用配置物のクラスだったら
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTTARGET)
			{// ジップライン用配置物のクラスだったら
				pScene->Uninit();
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}

	FILE *pFile = NULL;  // ファイルポインタ
	char aString[256];   // テキスト格納用文字列
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(m_aObjectFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // 配置物用の格納変数
			CObject *pObject = NULL;
			CObjectMove *pObjectMove = NULL;
			CObjectBalloon *pObjectBalloon = NULL;
			CObjectGoal *pObjectGoal = NULL;
			CObjectTarget *pObjectTarget = NULL;
			LPD3DXMESH pMesh = NULL;         // メッシュ情報格納用
			LPD3DXBUFFER pBuffMat = NULL;    // マテリアル情報格納用
			DWORD nNumMat = NULL;            // マテリアルの数情報格納用
			D3DXVECTOR3 VtxMax;              // 頂点の最代値情報格納用
			D3DXVECTOR3 VtxMin;              // 頂点の最小値情報格納用
			int nCntModelManager = 0;        // 管轄クラスへ割り当てる際のモデル番号
			int nObjectIndex = 0;            // 割り当てるモデルの番号
			D3DXVECTOR3 ObjectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 配置物の座標
			D3DXVECTOR3 ObjectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 配置物の向き
			D3DXVECTOR3 ObjectMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // 配置物の移動量
			D3DXVECTOR3 ObjectRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 配置物の回転するスピード量
			D3DXVECTOR3 ObjectPosTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 配置物のプレイヤーを運ぶ位置
			bool bCollision = true;                                        // 当たり判定を取るかどうか
			bool bPush = false;                                            // 配置物を押せるかどうか
			bool bBreak = false;                                           // 配置物を壊せるかどうか
			int nMaxCarryTime = 0;                                         // プレイヤーを運ぶのにかける時間
			int nCntObject = 0;                                            // 配置物を置いた数を数える変数
			int nCntObjectMove = 0;                                        // 動く配置物を置いた数を数える変数
			int nCntObjectBalloon = 0;                                     // 吹き出し用配置物を置いた数を数える変数
			int nCntObjectGoal = 0;                                        // ゴール用配置物を置いた数を数える変数
			int nCntObjectTarget = 0;                                      // ジップライン用配置物を置いた数を数える変数
			char aNextMapName[256];                                        // 次のマップデータのファイル名

			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// テキストが最後の行に到達するまで読み込みを続ける
				// 配置物
				if (strcmp(&aString[0], OBJECTSET) == 0)
				{// 配置物セット読み込み開始の合図がある
					while (strcmp(&aString[0], END_OBJECTSET) != 0)
					{// 配置物セットを読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストを読み進める
						if (strcmp(&aString[0], TYPE) == 0)
						{// 配置するオブジェクトの番号を読み取る
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// 座標を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// 向きを読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// 回転するスピード量を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// 当たり判定を取るかどうかを読み取る
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
						if (strcmp(&aString[0], PUSH) == 0)
						{// 押せるかどうかを読み取る
							int nPush;
							fscanf(pFile, "%s %d", &aString[0], &nPush);
							if (nPush == 0)
							{
								bPush = false;
							}
							else
							{
								bPush = true;
							}
						}
						if (strcmp(&aString[0], BREAK) == 0)
						{// 壊せるかどうかを読み取る
							int nBreak;
							fscanf(pFile, "%s %d", &aString[0], &nBreak);
							if (nBreak == 0)
							{
								bBreak = false;
							}
							else
							{
								bBreak = true;
							}
						}
					}
					pObject = CObject::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, bPush, bBreak);
					if (pObject != NULL)
					{// メモリが確保できた
						// モデルの割り当て
						pObject->BindModel(CManager::GetRenderer()->GetDevice(),
							m_pModelManager->GetMesh(nObjectIndex),
							m_pModelManager->GetBuffMat(nObjectIndex),
							m_pModelManager->GetNumMat(nObjectIndex));

						// 頂点の最小値、最大値情報の設定
						pObject->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObject->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObject++;  // 配置物を配置した数を進める

						// 変数を初期化しておく
						bCollision = true;
						bBreak = false;
						bPush = false;
					}
				}

				// 動く配置物
				if (strcmp(&aString[0], OBJECTMOVESET) == 0)
				{// 動く配置物セット読み込み開始の合図がある
					while (strcmp(&aString[0], END_OBJECTMOVESET) != 0)
					{// 動く配置物セットを読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストを読み進める
						if (strcmp(&aString[0], TYPE) == 0)
						{// 配置するオブジェクトの番号を読み取る
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// 座標を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// 向きを読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], MOVE) == 0)
						{// 移動量を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectMove.x, &ObjectMove.y, &ObjectMove.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// 回転するスピード量を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// 当たり判定を取るかどうかを読み取る
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
					}

					pObjectMove = CObjectMove::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, ObjectMove);
					if (pObjectMove != NULL)
					{// メモリが確保できた
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// モデルの割り当て
						pObjectMove->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// 頂点の最小値、最大値情報の設定
						pObjectMove->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectMove->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectMove++;  // 動く配置物を配置した数を進める

						// 変数を初期化しておく
						bCollision = true;
					}
				}

				// 吹き出し用
				if (strcmp(&aString[0], OBJECTBALLOONSET) == 0)
				{// 吹き出し用配置物セット読み込み開始の合図がある
					while (strcmp(&aString[0], END_OBJECTBALLOONSET) != 0)
					{// 吹き出し用配置物セットを読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストを読み進める
						if (strcmp(&aString[0], TYPE) == 0)
						{// 配置するオブジェクトの番号を読み取る
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// 座標を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// 向きを読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// 回転するスピード量を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// 当たり判定を取るかどうかを読み取る
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
						if (strcmp(&aString[0], NEXT_MAPNAME) == 0)
						{// 次のマップデータのファイル名を読み取る
							fscanf(pFile, "%s %s", &aString[0], &aNextMapName[0]);
						}
					}

					pObjectBalloon = CObjectBalloon::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, (CObjectBalloon::BALLOON)nCntObjectBalloon, aNextMapName, 4);
					if (pObjectBalloon != NULL)
					{// メモリが確保できた
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// モデルの割り当て
						pObjectBalloon->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// 頂点の最小値、最大値情報の設定
						pObjectBalloon->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectBalloon->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectBalloon++;  // 吹き出し用配置物を配置した数を進める

						// 変数を初期化しておく
						bCollision = true;
					}
				}

				// ゴール用
				if (strcmp(&aString[0], OBJECTGOALSET) == 0)
				{// ゴール用配置物セット読み込み開始の合図がある
					while (strcmp(&aString[0], END_OBJECTGOALSET) != 0)
					{// ゴール用配置物セットを読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストを読み進める
						if (strcmp(&aString[0], TYPE) == 0)
						{// 配置するオブジェクトの番号を読み取る
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// 座標を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// 向きを読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// 回転するスピード量を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// 当たり判定を取るかどうかを読み取る
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
					}
					pObjectGoal = CObjectGoal::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, 4);
					if (pObjectGoal != NULL)
					{// メモリが確保できた
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// モデルの割り当て
						pObjectGoal->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// 頂点の最小値、最大値情報の設定
						pObjectGoal->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectGoal->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectGoal++;  // ゴール用配置物を配置した数を進める

						// 変数を初期化しておく
						bCollision = true;
					}
				}


				// ジップライン用
				if (strcmp(&aString[0], OBJECTTARGETSET) == 0)
				{// ジップライン用配置物セット読み込み開始の合図がある
					while (strcmp(&aString[0], END_OBJECTTARGETSET) != 0)
					{// ジップライン用配置物セットを読み込み終えるまで繰り返し
						fscanf(pFile, "%s", &aString[0]);  // テキストを読み進める
						if (strcmp(&aString[0], TYPE) == 0)
						{// 配置するオブジェクトの番号を読み取る
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// 座標を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// 向きを読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// 回転するスピード量を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], POS_TARGET) == 0)
						{// プレイヤーを運ぶ位置を読み取る
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPosTarget.x, &ObjectPosTarget.y, &ObjectPosTarget.z);
						}
						if (strcmp(&aString[0], CARRY_TIME) == 0)
						{// プレイヤーを運ぶのにかける時間を読み取る
							fscanf(pFile, "%s %d", &aString[0], &nMaxCarryTime);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// 当たり判定を取るかどうかを読み取る
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
					}
					pObjectTarget = CObjectTarget::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, ObjectPosTarget, nMaxCarryTime);
					if (pObjectTarget != NULL)
					{// メモリが確保できた
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// モデルの割り当て
						pObjectTarget->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// 頂点の最小値、最大値情報の設定
						pObjectTarget->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectTarget->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectTarget++;  // ジップライン用配置物を配置した数を進める

						// 変数を初期化しておく
						bCollision = true;
					}
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    プレイヤーの初期座標を取得する処理
//=============================================================================
D3DXVECTOR3 CMap::GetDefaultPlayerPos(void)
{
	return m_DefaultPlayerPos;
}

//=============================================================================
//    プレイヤーの初期向きを取得する処理
//=============================================================================
D3DXVECTOR3 CMap::GetDefaultPlayerRot(void)
{
	return m_DefaultPlayerRot;
}

//=============================================================================
//    カメラの初期向きを取得する処理
//=============================================================================
D3DXVECTOR3 CMap::GetDefaultCameraRot(void)
{
	return m_DefaultCameraRot;
}

//=============================================================================
//    読み込んだマップのファイル名を返す処理
//=============================================================================
char *CMap::GetMapName(void)
{
	return m_aFileName;
}