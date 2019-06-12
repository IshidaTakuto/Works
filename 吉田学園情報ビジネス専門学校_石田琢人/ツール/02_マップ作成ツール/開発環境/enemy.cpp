//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "enemy.h"
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "meshField.h"
#include "modelObject.h"
#include "shadow.h"
#include "setPlayer.h"

#include <stdio.h>

//==================================
// マクロ定義
//==================================
#define ENEMY_MODEL_FILE	"data/TEXT/ENEMY/model.txt"
#define ENEMY_SPEED		(0.4f)	// 移動の速度
#define ENEMY_LIFE		(2)		// ライフ
#define ENEMY_SHOOT		(120)	// 撃つ間隔

//==================================
// 静的メンバ変数宣言
//==================================
int				*CEnemy::m_pNumParts = {};				// パーツ数
char			***CEnemy::m_pModelName = NULL;			// モデル名
CEnemy::DATA	**CEnemy::m_pFileData = NULL;			// ファイル読み取り

//==================================
// 生成処理
//==================================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, int nType)
{
	CEnemy *pEnemy;

	pEnemy = new CEnemy;	// メモリを確保

	if (pEnemy != NULL)
	{// メモリ確保成功
		pEnemy->m_pos = pos;	// 位置
		pEnemy->m_nType = nType;	// 種類
		pEnemy->Init();			// 初期化処理
	}

	return pEnemy;
}

//=========================================
// モデルの読み込み
//=========================================
HRESULT CEnemy::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;
	int nCntParts = 0;		// パーツ数のカウンタ
	char **pModelFileName = CModelObjectManager::GetModelFileName(CModelObjectManager::TYPE_ENEMY);	// モデル名の取得
	int nNumType = CModelObjectManager::GetNumType(CModelObjectManager::TYPE_ENEMY);
	int nCntModel = 0;

	// モデル数分メモリ確保
	m_pNumParts = new int[nNumType];
	m_pModelName = new char**[nNumType];
	m_pFileData = new DATA*[nNumType];

	for (int nCntType = 0; nCntType < nNumType; nCntType++)
	{
		// ファイルを開く
		pFile = fopen(pModelFileName[nCntType], "r");

		if (NULL != pFile)
		{// ファイルがあった
			char aStr[128];

			while (1)
			{// スクリプトを読み終わるまでループ
				fscanf(pFile, "%s", aStr);

				if (strcmp(aStr, "SCRIPT") == 0)
				{// スクリプト開始
					while (strcmp(aStr, "END_SCRIPT") != 0)
					{// スクリプト終了メッセージまでループ
						fscanf(pFile, "%s", aStr);

						if (strcmp(aStr, "NUM_MODEL") == 0)
						{// モデル数
							int nNumModel;
							fscanf(pFile, " = %d", &nNumModel);

							m_pModelName[nCntType] = new char*[nNumModel];		// メモリ確保
						}
						if (strcmp(aStr, "MODEL_FILENAME") == 0)
						{// キャラクター情報
							m_pModelName[nCntType][nCntModel] = new char[64];		// メモリ確保
							fscanf(pFile, " = %s", &m_pModelName[nCntType][nCntModel][0]);

							nCntModel++;
						}
						if (strcmp(aStr, "CHARACTERSET") == 0)
						{// キャラクター情報
							while (strcmp(aStr, "END_CHARACTERSET") != 0)
							{// キャラクター情報終了メッセージまでループ
								fscanf(pFile, "%s", aStr);

								if (strcmp(aStr, "NUM_PARTS") == 0)
								{// パーツ数
									fscanf(pFile, " = %d", &m_pNumParts[nCntType]);
									m_pFileData[nCntType] = new DATA[m_pNumParts[nCntType]];
								}
								else if (strcmp(aStr, "PARTSSET") == 0)
								{// パーツの設定
									while (strcmp(aStr, "END_PARTSSET") != 0)
									{// パーツの設定終了メッセージまでループ
										fscanf(pFile, "%s", &aStr);

										if (strcmp(aStr, "INDEX") == 0)
										{// モデル番号
											fscanf(pFile, " = %d", &m_pFileData[nCntType][nCntParts].nIdx);
										}
										else if (strcmp(aStr, "PARENT") == 0)
										{// 親を決める
											fscanf(pFile, " = %d", &m_pFileData[nCntType][nCntParts].nParent);
										}
										else if (strcmp(aStr, "POS") == 0)
										{// 位置の設定
											fscanf(pFile, " = %f %f %f", &m_pFileData[nCntType][nCntParts].pos.x, &m_pFileData[nCntType][nCntParts].pos.y, &m_pFileData[nCntType][nCntParts].pos.z);
										}
										else if (strcmp(aStr, "ROT") == 0)
										{// 向きの設定
											fscanf(pFile, " = %f %f %f", &m_pFileData[nCntType][nCntParts].rot.x, &m_pFileData[nCntType][nCntParts].rot.y, &m_pFileData[nCntType][nCntParts].rot.z);
										}
									}
									nCntParts++;	// パーツ数のカウント
								}
							}
						}
					}
					break;
				}
			}
			fclose(pFile);	// ファイルを閉じる
		}
		else
		{// ファイルがないとき
			MessageBox(0, "テキストファイルがありません。", "ENEMY/model.txt", MB_YESNO);
		}
	}

	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CEnemy::Unload(void)
{
	if (m_pFileData != NULL)
	{
		for (int nCntType = 0; nCntType < CModelObjectManager::GetNumType(CModelObjectManager::TYPE_ENEMY); nCntType++)
		{// 種類分ループ
			if (m_pFileData[nCntType] != NULL)
			{// メモリの開放
				delete[] m_pFileData[nCntType];
				m_pFileData[nCntType] = NULL;
			}
		}

		// メモリの開放
		delete m_pFileData;
		m_pFileData = NULL;
	}
}

//=========================================
// コンストラクタ
//=========================================
CEnemy::CEnemy(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pShadow = NULL;
	m_pModel = NULL;
	m_pMotion = NULL;
}

//=========================================
// デストラクタ
//=========================================
CEnemy::~CEnemy()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CEnemy::Init(void)
{
	if (m_pModel == NULL)
	{// モデルクラスの生成
		m_pModel = new CModel*[m_pNumParts[m_nType]];	// パーツ分のメモリを確保

		for (int nCntModel = 0; nCntModel < m_pNumParts[m_nType]; nCntModel++)
		{// メモリの確保
			m_pModel[nCntModel] = CModel::Create();

			// モデル情報の受け渡し
			m_pModel[nCntModel]->SetModel(m_pModelName[m_nType][nCntModel]);

			if (m_pFileData[m_nType][nCntModel].nParent >= 0)
			{// 親のポインタを設定
				m_pModel[nCntModel]->SetParent(m_pModel[m_pFileData[m_nType][nCntModel].nParent]);
			}
			else
			{// 親がいない場合
				m_pModel[nCntModel]->SetParent(NULL);
			}

			m_pModel[nCntModel]->SetPos(m_pFileData[m_nType][nCntModel].pos);
			m_pModel[nCntModel]->SetRot(m_pFileData[m_nType][nCntModel].rot);
		}
	}

	if (m_pMotion == NULL)
	{// モーションクラスの生成
		m_pMotion = CMotion::Create(1, m_pNumParts[m_nType], m_pModel);
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// ニュートラルモーションを設定
	}

	if (m_pShadow == NULL)
	{// 影の生成
		m_pShadow = CShadow::Create();
	}

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CEnemy::Uninit(void)
{
	if (m_pShadow != NULL)
	{// 影の破棄
		m_pShadow = NULL;
	}

	if (m_pMotion != NULL)
	{// モーションクラスの破棄
		m_pMotion->Uninit();
	}

	// 自分を破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CEnemy::Update(void)
{
	float fHeight = 0.0f;	// 起伏の高さを求める

	// 地面オブジェクトを探す
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// 地面だったとき
			CMeshField *pMeshField = (CMeshField*)pScene;	// 地面のポインタを取得

			fHeight = pMeshField->GetHeight(m_pos);			// 地面の高さの取得
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	// 地面の高さに合わせる
	m_pos.y = fHeight;

	// 範囲内かどうか
	CSetPlayer *pSetPlayer = CManager::GetSetPlayer();
	m_bRange = pSetPlayer->Range(m_pos);
}

//=========================================
// 描画処理
//=========================================
void CEnemy::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pShadow != NULL)
	{// 使われている
		for (int nCntModel = 0; nCntModel < m_pNumParts[m_nType]; nCntModel++)
		{// 影の描画
			m_pShadow->Set(m_pModel[nCntModel]->GetMtxWorld(), m_pModel[nCntModel]->GetMesh(), m_pos);
		}
	}

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntModel = 0; nCntModel < m_pNumParts[0]; nCntModel++)
	{// パーツモデルの描画
		if (m_bRange)
		{
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, -0.3f));
		}

		m_pModel[nCntModel]->Draw();

		if (m_bRange)
		{
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(-1.0f, 0.0f, 0.0f, 0.3f));
		}
	}
}

//=========================================
// 当たり判定
//=========================================
bool CEnemy::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// 相手のXYの範囲
	float fLength = powf(fRange, 2);

	float fLengthX = pos.x - m_mtxWorld._41;						// Xの差
	float fLengthY = pos.y - m_mtxWorld._42;						// Yの差
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2);		// XYの差の二乗

	if (fLength >= fLengthTotal)
	{// 相手と自分の判定
		if (fLengthX > 0)
		{// プレイヤーの右側にいる
			m_pos.x += fLengthX - fRange;
		}
		else
		{// プレイヤーの左側にいる
			m_pos.x -= fabsf(fLengthX) - fRange;
		}
		bHit = true;	// 判定を有効にする
	}

	return bHit;	// 判定結果を返す
}

//=========================================
// モデル情報の設定
//=========================================
void CEnemy::GetInfo(int nType, CModel **pModel)
{
	for (int nCntModel = 0; nCntModel < m_pNumParts[nType]; nCntModel++)
	{// メモリの確保
		// モデル情報の受け渡し
		pModel[nCntModel]->SetModel(m_pModelName[nType][nCntModel]);

		if (m_pFileData[nType][nCntModel].nParent >= 0)
		{// 親のポインタを設定
			pModel[nCntModel]->SetParent(pModel[m_pFileData[nType][nCntModel].nParent]);
		}
		else
		{// 親がいない場合
			pModel[nCntModel]->SetParent(NULL);
		}

		pModel[nCntModel]->SetPos(m_pFileData[nType][nCntModel].pos);
		pModel[nCntModel]->SetRot(m_pFileData[nType][nCntModel].rot);
	}
}

//=========================================
// 範囲内の敵を削除
//=========================================
void CEnemy::Range(void)
{
	if (m_bRange)
	{// 範囲内のとき削除
		Uninit();
	}
}