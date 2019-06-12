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
#include "player.h"
#include "meshField.h"
#include "game.h"
#include "modelSet.h"
#include "shadow.h"
#include "life.h"
#include "sound.h"

#include <stdio.h>

//==================================
// マクロ定義
//==================================
#define ENEMY_MODEL_FILE	"data/TEXT/ENEMY/model.txt"
#define ENEMY_SPEED		(0.4f)	// 移動の速度
#define ENEMY_LIFE		(10)		// ライフ
#define ENEMY_SHOOT		(120)	// 攻撃間隔

#define ENEMY_LIFE_WIDTH	(50.0f)		// 体力バーの幅
#define ENEMY_LIFE_HEIGHT	(10.0f)		// 体力バーの高さ

#define ENEMY_WIDTH			(30.0f)		// 当たり判定の幅
#define ENEMY_DAMAGE_TIME	(30)		// ダメージ状態の時間

// モーション番号
#define ENEMY_NEUTRAL		(0)		// ニュートラル
#define ENEMY_WALK			(1)		// 歩行
#define ENEMY_JUMP			(2)		// ジャンプ
#define ENEMY_LAND			(3)		// 着地
#define ENEMY_DAMAGE		(4)		// ダメージ
#define ENEMY_ATTACK		(5)		// 攻撃

//==================================
// 静的メンバ変数宣言
//==================================
int				*CEnemy::m_pNumParts = {};				// パーツ数
char			***CEnemy::m_pModelName = NULL;			// モデル名
CEnemy::DATA	**CEnemy::m_pFileData = NULL;			// ファイル読み取り
int				CEnemy::m_nNumType = 0;					// 種類の数

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
	char **pModelFileName = CModelSetManager::GetModelFileName(CModelSetManager::TYPE_ENEMY);	// モデル名の取得
	int nCntModel = 0;
	m_nNumType = CModelSetManager::GetNumType(CModelSetManager::TYPE_ENEMY);

	// モデル数分メモリ確保
	m_pNumParts = new int[m_nNumType];
	m_pModelName = new char**[m_nNumType];
	m_pFileData = new DATA*[m_nNumType];

	for (int nCntType = 0; nCntType < m_nNumType; nCntType++)
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
// 読み込んだデータの解放
//=========================================
void CEnemy::Unload(void)
{
	if (NULL != m_pFileData)
	{
		for (int nCntType = 0; nCntType < CModelSetManager::GetNumType(CModelSetManager::TYPE_ENEMY); nCntType++)
		{// 種類分ループ
			if (NULL != m_pFileData[nCntType])
			{// メモリの開放
				delete[] m_pFileData[nCntType];
				m_pFileData[nCntType] = NULL;
			}
		}

		if (NULL != m_pFileData)
		{// メモリの開放
			delete[] m_pFileData;
			m_pFileData = NULL;
		}
	}

	if (NULL != m_pModelName)
	{// パーツ数の破棄
		for (int nCntType = 0; nCntType < CModelSetManager::GetNumType(CModelSetManager::TYPE_ENEMY); nCntType++)
		{// 種類分ループ
			for (int nCntParts = 0; nCntParts < m_pNumParts[nCntType]; nCntParts++)
			{
				if (NULL != m_pModelName[nCntType][nCntParts])
				{// 破棄
					delete[] m_pModelName[nCntType][nCntParts];
					m_pModelName[nCntType][nCntParts] = NULL;
				}
			}

			if (NULL != m_pModelName[nCntType])
			{// 破棄
				delete[] m_pModelName[nCntType];
				m_pModelName[nCntType] = NULL;
			}
		}
		if (NULL != m_pModelName)
		{// 破棄
			delete[] m_pModelName;
			m_pModelName = NULL;
		}
	}

	if (NULL != m_pNumParts)
	{// パーツ数の破棄
		delete[] m_pNumParts;
		m_pNumParts = NULL;
	}
}

//=========================================
// コンストラクタ
//=========================================
CEnemy::CEnemy(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntBullet = 0;
	m_pShadow = NULL;
	m_fAttackRange = 0.0f;
	m_fWalkRange = 0.0f;
	m_bAttack = false;
	m_nIntervalAttack = 0;
	m_fRotDest = 0.0f;
	m_pLife = NULL;
	m_fAlpha = 0.0f;

	for (int nCntDamage = 0; nCntDamage < ATTACK_MOTION; nCntDamage++)
	{// 攻撃モーション分ループ
		m_nCntDamage[nCntDamage] = 0;
		m_bDamage[nCntDamage] = false;
	}
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
	if (NULL == m_pModel)
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

			m_pModel[nCntModel]->Init();
			m_pModel[nCntModel]->SetPos(m_pFileData[m_nType][nCntModel].pos);
			m_pModel[nCntModel]->SetRot(m_pFileData[m_nType][nCntModel].rot);
		}
	}

	if (NULL == m_pMotion)
	{// モーションクラスの生成
		m_pMotion = CMotion::Create(1, m_pNumParts[m_nType], &m_pModel[m_nType]);
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);		// ニュートラルモーションを設定
	}

	if (NULL == m_pShadow)
	{// 影の生成
		m_pShadow = CShadow::Create(&m_pos);
	}

	if (NULL == m_pLife)
	{// 体力の生成
		float fHeight = 0.0f;
		int nMostHeight = 0;
		for (int nCntParts = 0; nCntParts < m_pNumParts[m_nType]; nCntParts++)
		{
			if (fHeight < m_pFileData[m_nType][nCntParts].pos.y)
			{// 一番Y軸が高いパーツを見つける
				fHeight = m_pFileData[m_nType][nCntParts].pos.y;
				nMostHeight = nCntParts;
			}
		}

		float fModelHeight = m_pModel[nMostHeight]->GetVtxMax().y;
		m_pLife = CLife3D::Create(ENEMY_LIFE, D3DXVECTOR3(-ENEMY_LIFE_WIDTH * 0.5f, fHeight + fModelHeight + ENEMY_LIFE_HEIGHT + 10.0f, 0.0f), ENEMY_LIFE_WIDTH, ENEMY_LIFE_HEIGHT, &m_pos);
	}

	// 攻撃判定の範囲
	m_fAttackRange = 60.0f;
	m_fWalkRange = 400.0f;

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CEnemy::Uninit(void)
{
	if (m_pShadow != NULL)
	{// 影の破棄
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	if (NULL != m_pLife)
	{// 体力クラスの破棄
		m_pLife->Uninit();
		m_pLife = NULL;
	}

	if (m_pMotion != NULL)
	{// モーションクラスの破棄
		m_pMotion->Uninit();
		m_pMotion = NULL;
	}

	for (int nCntType = 0; nCntType < m_nNumType; nCntType++)
	{
		for (int nCntModel = 0; nCntModel < m_pNumParts[nCntType]; nCntModel++)
		{// モデルの破棄
			if (NULL != m_pModel)
			{
				m_pModel[nCntModel]->Uninit();
				delete m_pModel[nCntModel];
				m_pModel[nCntModel] = NULL;
			}
		}
	}

	if (NULL != m_pModel)
	{// メモリの開放
		delete m_pModel;
		m_pModel = NULL;
	}

	// 自分を破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CEnemy::Update(void)
{
	CMotionManager::TYPE type = CMotionManager::TYPE_NEUTRAL;		// モーションの種類
	float fDiffAngle;												// 角度の差分
	CScene *pScene = NULL;											// オブジェクト管理クラスのポインタ変数

	if (m_pMotion && !m_bDeath)
	{// 使われている
		type = m_pMotion->GetType();	// モーションの種類

		if (type != CMotionManager::TYPE_ATTACK01 && type != CMotionManager::TYPE_DAMAGE)
		{// 攻撃モーションでない時
			if (m_move.x > 0.1f || m_move.x < -0.1f)
			{// 移動モーションを設定
				m_pMotion->SetMotion(CMotionManager::TYPE_WALK);		// モーションの設定
			}
			else
			{// ニュートラルモーションを設定
				m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);		// モーションの設定
			}
		}

		// プレイヤーオブジェクトを探す
		pScene = CScene::GetSceneTop(PLAYER_PRIORITY);

		while (pScene != NULL)
		{// NULLになるまでループ
			CScene *pSceneNext = pScene->GetSceneNext();		// 次のオブジェクトを保管
			CScene::OBJTYPE objType = pScene->GetObjType();		// オブジェクトの種類を取得

			if (objType == CScene::OBJTYPE_PLAYER)
			{// プレイヤーだったとき
				CPlayer *pPlayer = (CPlayer*)pScene;
				D3DXVECTOR3 posPlayer = pPlayer->GetPos();		// プレイヤーの座標の取得

				if (m_pModel[0]->Collision(posPlayer, m_fAttackRange) && !m_bAttack)
				{// 攻撃範囲にプレイヤーがいる
					m_pMotion->SetMotion(CMotionManager::TYPE_ATTACK01);	// モーションの設定
					m_bAttack = true;
				}
				else if (m_pModel[0]->Collision(posPlayer, m_fWalkRange) && CMotionManager::TYPE_ATTACK01 != type && !m_pModel[0]->Collision(posPlayer, m_fAttackRange))
				{// プレイヤーに近づく
					float fAngle = atan2f(posPlayer.x - m_pos.x, posPlayer.y - m_pos.y);
					m_move.x = sinf(fAngle) * 2.0f;
				}

				if (CMotionManager::TYPE_ATTACK01 == m_pMotion->GetType())
				{
					for (int nCntHit = 0; nCntHit < 5; nCntHit++)
					{// 当たり判定を団子状にする
						if (NULL != m_pMotion)
						{// モーションクラスが使われているときのみ
							if (m_pMotion->Collision(D3DXVECTOR3(posPlayer.x, posPlayer.y + (nCntHit * 10), posPlayer.z)))
							{// 当たり判定
								pPlayer->Damage(-10);
								break;
							}
						}
					}
				}

				if (Collision(posPlayer, ENEMY_WIDTH))
				{// プレイヤーとぶつかった
					m_move.x = 0.0f;

					// プレイヤーの移動量を減らす
					D3DXVECTOR3 move = pPlayer->GetMove();
					move *= 0.3f;
					pPlayer->SetMove(move);
				}

				// プレイヤーの方向を向く
				if (posPlayer.x - m_pos.x <= 0)
				{
					m_fRotDest = D3DX_PI * -0.5f;
				}
				else
				{
					m_fRotDest = D3DX_PI * 0.5f;
				}
			}

			// 次のオブジェクトを見る
			pScene = pSceneNext;
		}
	}

	// 一回転しないように角度を調節
	if (m_fRotDest > D3DX_PI)
	{
		m_fRotDest -= D3DX_PI * 2.0f;
	}
	if (m_fRotDest < -D3DX_PI)
	{
		m_fRotDest += D3DX_PI * 2.0f;
	}

	// 差分を求める
	fDiffAngle = (m_fRotDest + D3DX_PI) - m_rot.y;

	// 一回転しないように角度を調節
	if (fDiffAngle > D3DX_PI)
	{
		fDiffAngle -= D3DX_PI * 2.0f;
	}
	if (fDiffAngle < -D3DX_PI)
	{
		fDiffAngle += D3DX_PI * 2.0f;
	}

	// 回転
	m_rot.y += fDiffAngle * 0.2f;

	// 一回転しないように角度を調節
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	if (m_bAttack)
	{// 攻撃する間隔
		m_nIntervalAttack = (1 + m_nIntervalAttack) % 120;

		if (0 == m_nIntervalAttack)
		{// 一定時間経過で攻撃フラグをfalseにする
			m_bAttack = false;
		}
	}

	// 重力
	m_move.y -= GRAVITY;

	// 移動量の慣性
	m_move.x += (0 - m_move.x) * 0.4f;
	m_pos += m_move;

	float fHeight = 0.0f;	// 起伏の高さを求める

	// 地面オブジェクトを探す
	pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

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

	if (m_pos.y <= fHeight)
	{// 地面以下になったとき
		m_pos.y = fHeight;	// 高さを座標に入れる
		m_move.y = 0.0f;	// Y軸の移動量を無くす
	}

	// 敵オブジェクトを探す
	pScene = CScene::GetSceneTop(ENEMY_PRIORITY);

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_ENEMY)
		{// 敵だったとき
			CEnemy *pEnemy = (CEnemy*)pScene;	// ポインタを取得

			// 敵同士の当たり判定
			pEnemy->Collision(m_pos, ENEMY_WIDTH);
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	for (int nCntDamage = 0; nCntDamage < ATTACK_MOTION; nCntDamage++)
	{// 攻撃モーション分ループ
		if (m_bDamage[nCntDamage] && !m_bDeath)
		{// ダメージ状態
			m_nCntDamage[nCntDamage] = (1 + m_nCntDamage[nCntDamage]) % ENEMY_DAMAGE_TIME;

			if (m_nCntDamage[nCntDamage] == 0)
			{// 一定時間経過
				m_bDamage[nCntDamage] = false;
			}
		}
	}

	if (m_bDeath)
	{// 死亡状態
		if (m_pos.y == fHeight)
		{// 地面と同じ高さ
			if (m_pMotion)
			{// 死亡モーション
				m_pMotion->SetMotion(CMotionManager::TYPE_WALK);
			}

			m_move.z = 5.0f;
			m_rot.y = D3DX_PI;

			m_fAlpha += 0.01f;
			if (m_fAlpha > 1.0f)
			{// 透明になったとき削除する
				Uninit();
			}
		}
	}
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

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntModel = 0; nCntModel < m_pNumParts[0]; nCntModel++)
	{// パーツモデルの描画
		if (m_bDeath)
		{// 死亡状態
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, -m_fAlpha));
		}
		for (int nCntDamage = 0; nCntDamage < ATTACK_MOTION; nCntDamage++)
		{// 攻撃モーション分ループ
			if (m_bDamage[nCntDamage])
			{// ダメージ状態
				m_pModel[nCntModel]->AddColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
				break;
			}
		}

		// モデルの描画
		m_pModel[nCntModel]->Draw();

		if (m_bDeath)
		{// 死亡状態
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fAlpha));
		}
		for (int nCntDamage = 0; nCntDamage < ATTACK_MOTION; nCntDamage++)
		{// 攻撃モーション分ループ
			if (m_bDamage[nCntDamage])
			{// ダメージ状態
				m_pModel[nCntModel]->AddColor(D3DXCOLOR(-1.0f, 0.0f, 0.0f, 0.0f));
				break;
			}
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

	float fLengthX = pos.x - m_pos.x;						// Xの差
	float fLengthY = pos.y - m_pos.y;						// Yの差
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2);		// XYの差の二乗

	if (fLength >= fLengthTotal)
	{// 相手と自分の判定
		if (fLengthX > 0)
		{// 右側にいる
			m_pos.x += fLengthX - fRange;
		}
		else if(fLengthX < 0)
		{// 左側にいる
			m_pos.x -= fabsf(fLengthX) - fRange;
		}
		bHit = true;	// 判定を有効にする
	}

	return bHit;	// 判定結果を返す
}

//=========================================
// モデル情報の受け渡し
//=========================================
void CEnemy::GetModelData(CModel**& pModel)
{
	m_pModel = pModel;
}

//=========================================
// ダメージを受けたときの処理
//=========================================
void CEnemy::Damage(int nDamage, int nMotion)
{
	if (0 < m_pLife->GetLife())
	{
		if (nMotion < ATTACK_MOTION && nMotion >= 0)
		{// 配列を越さないようにする
			if (NULL != m_pLife && !m_bDamage[nMotion])
			{// ライフクラスが使われている
				m_pLife->AddSubtract(nDamage);

				// ダメージ状態にする
				m_pMotion->SetMotion(CMotionManager::TYPE_DAMAGE);
				m_bDamage[nMotion] = true;
				CSound *pSound = CManager::GetSound();			// サウンドの取得
				pSound->PlaySound(CSound::SOUND_LABEL_HIT);		// 攻撃ヒット音の再生

				if (0 >= m_pLife->GetLife())
				{// 体力が0以下になった
					m_bDeath = true;
					m_nCntDamage[nMotion] = 0;
					pSound->PlaySound(CSound::SOUND_LABEL_ENEMYDEATH);	// 敵死亡音の再生
					m_move.y = 10.0f;

					if (m_pMotion)
					{// 死亡モーション
						m_pMotion->SetMotion(CMotionManager::TYPE_DEATH);
					}
				}
			}
		}
		else if (-1 == nMotion)
		{// モーションが-1のとき
			CSound *pSound = CManager::GetSound();			// サウンドの取得
			pSound->PlaySound(CSound::SOUND_LABEL_HIT);		// 攻撃ヒット音の再生

			m_pLife->AddSubtract(nDamage);	// ライフを0にする

			if (0 >= m_pLife->GetLife())
			{// 体力が0以下になった
				m_bDeath = true;
				pSound->PlaySound(CSound::SOUND_LABEL_ENEMYDEATH);	// 敵死亡音の再生

				m_move.y = 15.0f;

				if (m_pMotion)
				{// 死亡モーション
					m_pMotion->SetMotion(CMotionManager::TYPE_DEATH);
				}
			}
		}
	}
}