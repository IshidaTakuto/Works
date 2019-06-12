//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "meshField.h"
#include "camera.h"
#include "model.h"
#include "motion.h"
#include "shadow.h"
#include "game.h"
#include "enemy.h"
#include "stage.h"
#include "modelSet.h"
#include "fade.h"
#include "life.h"
#include "particle.h"
#include "sound.h"

#include <stdio.h>

//==================================
// マクロ定義
//==================================
#define MODEL_FILE "data/TEXT/PLAYER/model.txt"
#define MOVE_SPEED			(4.0f)		// 移動の速度
#define PLAYER_SLIP_ANGLE	(0.8f)		// 滑るようになる角度
#define JUMP_SPEED			(4.0f)		// ジャンプの速度
#define SLIP_ENABLE_SPEED	(1.0f)		// 滑るようになる速度
#define PLAYER_LIFE			(100)		// 体力
#define	PLAYER_LIFE_WIDTH	(500.0f)	// 体力バーの幅
#define PLAYER_LIFE_HEIGHT	(50.0f)		// 体力バーの高さ
#define PLAYER_DAMAGE_TIME	(60)		// ダメージを受けた時の無敵時間
#define PLAYER_DAMAGE_MOVE	(40)		// ダメージを受けてから動けるようになるまでの時間
#define PLAYER_CLIMB_SPEED	(1.1f)		// 登るときの速度の減衰量
#define PLAYER_SLIP_SPEED	(1.7f)		// 滑る速度
#define PLAYER_SLIP_JUMP	(0.45f)		// 吹っ飛びジャンプ速度補正
#define PLAYER_JUMP_DASH	(25.0f)		// ジャンプ中の空中ダッシュの速度補正

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// 生成処理
//==================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer;

	pPlayer = new CPlayer;	// メモリを確保

	if (pPlayer != NULL)
	{// メモリ確保成功
		pPlayer->Init();
		pPlayer->m_pos = pos;
	}

	return pPlayer;
}

//=========================================
// コンストラクタ
//=========================================
CPlayer::CPlayer(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntState = 0;
	m_rotDest = 0.0f;
	m_fSpeed = 0.0f;
	m_fSlipSpeed = 0.0f;
	m_bJump = false;
	m_bSlip = false;
	m_fJumpSpeed = 0.0f;
	m_pModel = NULL;
	m_nNumParts = 0;
	m_fAngleSlip = 0.0f;
	m_pMotion = NULL;
	m_pLife = NULL;
	m_bDamage = false;
	m_bMove = false;
	m_nCntParticle = 0;
	m_bDash = false;
}

//=========================================
// デストラクタ
//=========================================
CPlayer::~CPlayer()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CPlayer::Init(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;
	int nCntParts = 0;		// パーツ数のカウンタ
	int nCntModel = 0;
	float fX, fY, fZ;
	int nId;
	int nParent;
	char **pModelName = NULL;

	// ファイルを開く
	pFile = fopen(MODEL_FILE, "r");

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

						pModelName = new char*[nNumModel];		// メモリ確保
					}
					if (strcmp(aStr, "MODEL_FILENAME") == 0)
					{// キャラクター情報
						fscanf(pFile, " = %s", &aStr);

						pModelName[nCntModel] = new char[64];		// メモリ確保
						strcpy(pModelName[nCntModel], aStr);
						nCntModel++;
					}
					if (strcmp(aStr, "CHARACTERSET") == 0)
					{// キャラクター情報
						while (strcmp(aStr, "END_CHARACTERSET") != 0)
						{// キャラクター情報終了メッセージまでループ
							fscanf(pFile, "%s", aStr);

							if (strcmp(aStr, "NUM_PARTS") == 0)
							{// パーツ数
								fscanf(pFile, " = %d", &m_nNumParts);
								m_pModel = new CModel*[m_nNumParts];

								for (int nCntModelParts = 0; nCntModelParts < m_nNumParts; nCntModelParts++)
								{// パーツ数分ループ
									m_pModel[nCntModelParts] = CModel::Create();
								}
							}
							else if (strcmp(aStr, "PARTSSET") == 0)
							{// パーツの設定
								while (strcmp(aStr, "END_PARTSSET") != 0)
								{// パーツの設定終了メッセージまでループ
									if (m_pModel[nCntParts] != NULL)
									{// メモリ確保成功
										fscanf(pFile, "%s", &aStr);

										if (strcmp(aStr, "INDEX") == 0)
										{// モデル番号
											fscanf(pFile, " = %d", &nId);

											// 使うモデルを指定
											m_pModel[nCntParts]->SetModel(pModelName[nId]);
										}
										else if (strcmp(aStr, "PARENT") == 0)
										{// 親を決める
											fscanf(pFile, " = %d", &nParent);

											if (nParent >= 0)
											{// 親のポインタを設定
												m_pModel[nCntParts]->SetParent(m_pModel[nParent]);
											}
											else
											{// 親がいない場合
												m_pModel[nCntParts]->SetParent(NULL);
											}
										}
										else if (strcmp(aStr, "POS") == 0)
										{// 位置の設定
											fscanf(pFile, " = %f %f %f", &fX, &fY, &fZ);

											// 位置の設定
											m_pModel[nCntParts]->SetPos(D3DXVECTOR3(fX, fY, fZ));
										}
										else if (strcmp(aStr, "ROT") == 0)
										{// 向きの設定
											fscanf(pFile, " = %f %f %f", &fX, &fY, &fZ);

											// 向きの設定
											m_pModel[nCntParts]->SetRot(D3DXVECTOR3(fX, fY, fZ));
										}
									}
								}
								nCntParts++;
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
		MessageBox(0, "ファイルがありません", "player/model.txt", MB_YESNO);
	}

	if (NULL != pModelName)
	{// メモリの開放
		for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
		{// メモリの開放
			delete[] pModelName[nCnt];
		}

		// メモリの開放
		delete[] pModelName;
	}

	// ポリゴンの位置を設定
	m_fSpeed = MOVE_SPEED;	// 速度の設定

	if (NULL == m_pMotion)
	{// モーションクラスの生成
		m_pMotion = CMotion::Create(0, m_nNumParts, m_pModel);
		m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);	// ニュートラルモーションを設定
	}

	if (NULL == m_pShadow)
	{// 影の生成
		m_pShadow = CShadow::Create(&m_pos);
	}

	if (NULL == m_pLife && CManager::MODE_GAME == CManager::GetMode())
	{// 体力の生成
		m_pLife = CLife2D::Create(PLAYER_LIFE, D3DXVECTOR3(10.0f, 0.0f, 0.0f), PLAYER_LIFE_WIDTH, PLAYER_LIFE_HEIGHT);
	}

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CPlayer::Uninit(void)
{
	if (NULL != m_pMotion)
	{// モーションクラスの破棄
		m_pMotion->Uninit();
		m_pMotion = NULL;
	}

	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{// モデルの破棄
		if (NULL != m_pModel[nCntModel])
		{
			m_pModel[nCntModel]->Uninit();
			delete m_pModel[nCntModel];
			m_pModel[nCntModel] = NULL;
		}
	}

	if (NULL != m_pModel)
	{// メモリの開放
		delete[] m_pModel;
		m_pModel = NULL;
	}

	if (NULL != m_pLife)
	{// ライフの破棄
		m_pLife->Uninit();
		m_pLife = NULL;
	}

	// オブジェクトの破棄
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CPlayer::Update(void)
{
	CSound *pSound = CManager::GetSound();						// サウンドの取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの入力を取得
	CXInput *pXInput = CManager::GetXInput();					// XInputの入力を取得

	D3DXVECTOR3 posOld = m_pos;		// 位置の保存
	float fDiffAngle = 0.0f;		// 回転の差分
	bool bJumpOld = m_bJump;		// ジャンプフラグの記憶
	bool bMoveKey = false;			// 移動キー押下フラグ
	float fAngleOld = m_fAngle;		// 地面との交点の角度を保存

	// 速度を入れる
	m_fSpeed = MOVE_SPEED;

	// 傾斜の処理
	Tilt();

	if (m_bDamage)
	{// 無敵時間
		m_nCntState = (1 + m_nCntState) % PLAYER_DAMAGE_TIME;
		m_bMove = false;

		if (m_nCntState == 0)
		{// 一定時間経過
			m_bDamage = false;	// 無敵時間を解除
		}
	}

	if (m_nCntState > PLAYER_DAMAGE_MOVE || m_nCntState == 0)
	{// 一定時間経過
		m_bMove = true;		// 動けるようにする
	}

	// カメラの角度を取得
	float rotCamera = CManager::GetCamera()->GetRotY();
	if ((!m_bSlip && m_fSlipSpeed > -3.0f && m_fSlipSpeed < 3.0f) && m_nCntState == 0 || m_nCntState > PLAYER_DAMAGE_TIME * 0.5f)
	{// キー入力による移動
		if (pKeyboard->GetPress(DIK_A) || pXInput->GetThumbLX(0) <= -MIN_GAMEPAD_LEFT_THUMB_X)
		{// ←方向へ移動
			m_rotDest = D3DX_PI * 0.5f + rotCamera;

			// 移動フラグを立てる
			bMoveKey = true;
		}
		else if (pKeyboard->GetPress(DIK_D) || pXInput->GetThumbLX(0) >= MIN_GAMEPAD_LEFT_THUMB_X)
		{// →方向へ移動
			m_rotDest = D3DX_PI * -0.5f + rotCamera;

			// 移動フラグを立てる
			bMoveKey = true;
		}

		if (bMoveKey)
		{// 移動キーが押された
			if (m_bJump)
			{// ジャンプ中
				m_move += D3DXVECTOR3(sinf(m_rotDest), 0.0f, 0.0f) * m_fJumpSpeed;
			}
			else if (m_fSlipSpeed == 0)
			{// 向いている角度への移動量
				m_move += D3DXVECTOR3(sinf(m_rotDest), 0.0f, 0.0f) * m_fSpeed;
			}
			else
			{// 0になるまで減らす
				m_fSlipSpeed += (0 - m_fSlipSpeed) * 0.3f;
			}

			if (((0 == m_pMotion->GetCurKey() && 0 == m_pMotion->GetCurFlame()) || (1 == m_pMotion->GetCurKey() && 0 == m_pMotion->GetCurFlame())) && !m_bJump)
			{// 歩いたときのパーティクル
				CParticle::Create(m_pos, 1);
			}
		}
	}
	else if (m_bSlip && !m_bJump)
	{// 滑っている
		float fRot = 0;				// 移動量を足す角度
		float fMoveX = m_move.x;	// 移動量の保存
		bool bBefore = false;		// キー入力前の移動方向
		bool bAfter = false;		// キー入力後の移動方向

		m_pMotion->SetMotion(CMotionManager::TYPE_SLIP);
		if (m_move.x > 0)
		{// Xの移動量が正
			bBefore = true;
		}

		// キー入力による移動---------------------------------------------------------------------------------------------
		if (pKeyboard->GetPress(DIK_A) || pXInput->GetThumbLX(0) <= -MIN_GAMEPAD_LEFT_THUMB_X)
		{// ←方向へ移動
			fRot = D3DX_PI * 0.5f + rotCamera;

			// 向いている角度への移動量
			m_move += D3DXVECTOR3(sinf(fRot), 0.0f, 0.0f) * (m_fSlipSpeed * 0.2f);
		}
		else if (pKeyboard->GetPress(DIK_D) || pXInput->GetThumbLX(0) >= MIN_GAMEPAD_LEFT_THUMB_X)
		{// →方向へ移動
			fRot = D3DX_PI * -0.5f + rotCamera;

			// 向いている角度への移動量
			m_move += D3DXVECTOR3(sinf(fRot), 0.0f, 0.0f) * (m_fSlipSpeed * 0.2f);
		}

		if (m_move.x > 0)
		{// Xの移動量が正
			bAfter = true;
		}

		if (bAfter != bBefore)
		{// 符号が変わっていた
			m_move.x = fMoveX * 0.5f;	// 摩擦の値を調整
		}
	}

	CDebugProc::Print("速度：%.2f", m_fSpeed);
	if (m_pMotion)
	{// モーションクラスが使われている
		if (bMoveKey && !m_bJump && m_bMove)
		{// 移動モーション
			m_pMotion->SetMotion(CMotionManager::TYPE_WALK);
		}
		else if (!bMoveKey && m_bMove && (CMotionManager::TYPE_WALK == m_pMotion->GetType() || CMotionManager::TYPE_SLIP == m_pMotion->GetType() && !m_bSlip))
		{// ニュートラルモーション
			m_pMotion->SetMotion(CMotionManager::TYPE_NEUTRAL);
		}
	}

	// 一回転しないように角度を調節
	if (m_rotDest > D3DX_PI)
	{
		m_rotDest -= D3DX_PI * 2.0f;
	}
	if (m_rotDest < -D3DX_PI)
	{
		m_rotDest += D3DX_PI * 2.0f;
	}

	// 差分を求める
	fDiffAngle = (m_rotDest + D3DX_PI) - m_rot.y;

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
	m_rot.y += fDiffAngle * (fabsf(((float)m_bSlip * -0.97f + 1.0f)) * 0.1f + 0.1f);

	// 一回転しないように角度を調節
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	// 重力
	m_move.y -= GRAVITY;

	CDebugProc::Print("スリップ : %s", m_bSlip ? "してる" : "してない");

	// 移動量の慣性
	if (!m_bJump) { m_move.x += (0 - m_move.x) * 0.6f; }
	else { m_move.x += (0 - m_move.x) * 0.005f; }

	CGame::MoveMountain(m_move.x);

	float fHeight = 0.0f;	// 起伏の高さを求める

	if (m_fAngle < 0.0f && fAngleOld >= 0.0f && !m_bJump && m_bSlip)
	{// 上っているときで地面についていないとき
		m_move.y = fAngleOld * m_fSlipSpeed * PLAYER_SLIP_JUMP;
		if (m_move.y > 0.0f)
		{// 上に飛んだ時ジャンプ状態にする
			m_bJump = true;
			m_bDash = false;
			pSound->PlaySound(CSound::SOUND_LABEL_JUMP);
		}
		m_move.x = sinf(m_rotDest) * m_fSlipSpeed * PLAYER_SLIP_JUMP;
		m_pMotion->SetMotion(CMotionManager::TYPE_JUMP);
	}

	if (m_bJump && !m_bDash)
	{// モーションクラスが使われている
		if ((pKeyboard->GetTrigger(DIK_SPACE) || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_10) || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_11) || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_12) || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_13)) && m_bMove)
		{// 空中ダッシュ
			m_move.x = sinf(m_rotDest) * PLAYER_JUMP_DASH;
			m_move.y += PLAYER_JUMP_DASH * 0.1f;
			m_bDash = true;
		}
	}

	if (m_bDash)
	{// 空中ダッシュ中
		CParticle::Create(m_pos, 4);
	}

	// 移動させる
	m_pos += m_move;

	// 地面オブジェクトを探す
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);
	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// 地面だったとき
			CMeshField *pMeshField = (CMeshField*)pScene;

			fHeight = pMeshField->GetHeight(m_pos);		// 地面の高さの取得
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	// オブジェクトモデルを探す
	pScene = CScene::GetSceneTop(MODEL_PRIORITY);
	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_MODELOBJECT)
		{// 配置物だったとき
			CModelObject *pModelObject = (CModelObject*)pScene;

			for (int nCntHeight = 0; nCntHeight < 10; nCntHeight++)
			{
				if (pModelObject->Collision(&m_pos, 45.0f))
				{// 当たり判定
					m_move.x = 0;
					m_fJumpSpeed = 0.0f;
					m_fSlipSpeed = 0.0f;
					m_fSpeed = 0.0f;
					break;
				}
			}
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	if (m_pos.y <= fHeight || !m_bJump)
	{// 地面以下になったとき
		m_pos.y = fHeight;	// 高さを座標に入れる
		m_bJump = false;	// 着地状態にする
		m_move.y = 0.0f;	// Y軸の移動量を無くす
		m_bDash = false;	// 空中ダッシュフラグを負にする
	}

	if (!m_bJump && bJumpOld)
	{// 着地したとき
		m_fSlipSpeed *= 0.2f;	// 滑る移動量を減らす

		if (m_pMotion)
		{// モーションクラスが使われている
			m_pMotion->SetMotion(CMotionManager::TYPE_LANDING);
		}
	}

	if (m_pMotion)
	{// モーションクラスが使われている
		pScene = CScene::GetSceneTop(ENEMY_PRIORITY);		// 敵オブジェクトを探す

		while (pScene != NULL)
		{// NULLになるまでループ
			CScene *pSceneNext = pScene->GetSceneNext();
			CScene::OBJTYPE objType = pScene->GetObjType();

			if (objType == CScene::OBJTYPE_ENEMY)
			{// 敵だったとき
				CEnemy *pEnemy = (CEnemy*)pScene;
				D3DXVECTOR3 posEnemy = pEnemy->GetPos();

				if (m_bSlip)
				{// 滑っている
					if (m_pModel[6]->Collision(posEnemy, 50.0f))
					{// 当たり判定
						pEnemy->Damage(-10, -1);

						// ヒットエフェクト
						CParticle::Create(posEnemy, 3);
					}
				}
			}
			// 次のオブジェクトを見る
			pScene = pSceneNext;
		}
	}

	if (Collision(CModelSetManager::GetGoalPos(), 70.0f) && CManager::MODE_GAME == CManager::GetMode() && CFade::FADE_NONE == CFade::GetFade())
	{// ゴールしたとき
		m_bMove = false;
		m_bSlip = false;
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_fSlipSpeed = 0.0f;

		if (!m_bStageFade)
		{// ステージ移動の設定
			CGame::NextStage();
		}
		m_bStageFade = true;
	}
	else if(m_bStageFade)
	{// ステージ遷移が終わった
		m_bStageFade = false;
	}

	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{// パーツモデルの更新
		m_pModel[nCntModel]->Update();
	}
}

//=========================================
// 描画処理
//=========================================
void CPlayer::Draw(void)
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

	for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
	{// パーツモデルの描画
		if (m_bDamage)
		{// ダメージを受けている状態
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.7f, 0.0f, 0.0f, -0.5f));
		}

		// モデルの描画処理
		m_pModel[nCntModel]->Draw();

		if (m_bDamage)
		{// ダメージを受けている状態
			m_pModel[nCntModel]->AddColor(D3DXCOLOR(-0.7f, 0.0f, 0.0f, 0.5f));
		}
	}
}

//=========================================
// 傾斜の処理
//=========================================
void CPlayer::Tilt(void)
{
	CSound *pSound = CManager::GetSound();			// サウンドの取得
	float fCosTheta = 0.0f;							// 計算結果
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f,0.0f,0.0f);	// 法線

	// 地面オブジェクトを探す
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// 地面だったとき
			CMeshField *pMeshField = (CMeshField*)pScene;

			m_fAngle = pMeshField->GetAngle(m_pos, m_rot.y);	// 傾斜の角度の取得
			nor = pMeshField->GetNor(m_pos);					// 地面の法線の取得

			if (!m_bJump)
			{// 坂の下り方向を取得
				m_fAngleSlip = pMeshField->GetSlipAngle(m_pos);
			}
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	CDebugProc::Print("地面とのcosθ：%.2f", m_fAngle);

	// カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	if (!m_bSlip)
	{// 滑っていない
		if (!m_bJump && nor.y < PLAYER_SLIP_ANGLE && nor.y != 0.0f && 0 > m_fAngle)
		{// ジャンプしてないかつ角度が一定以下で滑る
			m_bSlip = true;
			pSound->PlaySound(CSound::SOUND_LABEL_SLIP);

			if (m_fAngle < 0.0f)
			{// 滑り下りるとき速度を入れる
				m_fSlipSpeed = m_fSpeed;
			}
		}

		if (!m_bJump)
		{// 地面
			if (m_fAngle < 0.0f)
			{// 下りるときに速度を上げる
				m_fSpeed += MOVE_SPEED * (1.0f - nor.y);
			}
			else if (m_fAngle > 0.0f)
			{// 上るときに速度を下げる
				m_fSpeed -= MOVE_SPEED * (1.0f - nor.y) * PLAYER_CLIMB_SPEED;
			}
		}

		// 距離を離す
		pCamera->SetType(false);
	}
	else
	{// 滑っている
		if(!m_bJump)
		{// 滑る移動量を増やす
			m_fSlipSpeed += PLAYER_SLIP_SPEED * (1.0f - nor.y) * -m_fAngle;

			// 滑り落ちるときのパーティクル
			CParticle::Create(m_pos, 2);
		}
		else
		{// ジャンプ中
			pSound->StopSound(CSound::SOUND_LABEL_SLIP);
		}

		// 距離を縮める
		pCamera->SetType(true);
	}

	if (m_fSlipSpeed > -3.0f && m_fSlipSpeed < 3.0f)
	{// 速度が一定以下で通常へ
		m_bSlip = false;
		pSound->StopSound(CSound::SOUND_LABEL_SLIP);
		m_fSlipSpeed = 0;
	}

	if (!m_bJump)
	{// 滑る移動量
		m_move.x += sinf(m_fAngleSlip) * m_fSlipSpeed;

		if (m_bSlip)
		{// 滑っているとき
			if (0.0 <= m_rot.y && 0 < m_move.x)
			{// 移動量を逆にする
				m_move.x *= -1.0f;
			}
			else if (0.0 >= m_rot.y && 0 > m_move.x)
			{// 移動量を逆にする
				m_move.x *= -1.0f;
			}
		}
	}

	// 移動量のベクトル
	D3DXVECTOR3 vecMove = (m_pos + m_move) - m_pos;

	// 移動量ベクトルと法線のcosθを求める
	fCosTheta = (vecMove.x * nor.x) + (vecMove.y * nor.y) + (vecMove.z * nor.z);
	fCosTheta = fCosTheta / (sqrtf(powf(vecMove.x, 2) + powf(vecMove.y, 2) + powf(vecMove.z, 2)) * (sqrtf(powf(nor.x, 2) + powf(nor.y, 2) + powf(nor.z, 2))));

	if (isnan(fCosTheta))
	{// nanチェック
		fCosTheta = 0.0f;
	}

	// 角度によってパーティクルを変える
	if (nor.y < PLAYER_SLIP_ANGLE && 0 < m_fAngle && (m_move.x >= 0.01f || m_move.x <= -0.01f))
	{// 汗を出す
		m_nCntParticle = (1 + m_nCntParticle) % 15;

		if (m_nCntParticle == 0)
		{// パーティクルを出す
			CParticle::Create(m_pos + D3DXVECTOR3(0.0f, 40.0f, 0.0f), 0);
		}

		// 距離を縮める
		pCamera->SetType(true);
	}

	CDebugProc::Print("滑る速度：%.2f", m_fSlipSpeed);
	CDebugProc::Print("ジャンプ速度：%.2f", m_fJumpSpeed);
}

//=========================================
// 当たり判定
//=========================================
bool CPlayer::Collision(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// 相手のXYの範囲
	float fLength = powf(fRange, 2);

	float fLengthX = pos.x - m_mtxWorld._41;	// Xの差
	float fLengthY = pos.y - m_mtxWorld._42;	// Yの差
	float fLengthZ = pos.z - m_mtxWorld._43;	// Zの差

	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2) + powf(fLengthZ, 2);		// XYZの差の二乗

	if (fLength >= fLengthTotal)
	{// 相手と自分の判定
		bHit = true;	// 判定を有効にする
	}

	return bHit;	// 判定結果を返す
}

//=========================================
// ダメージを受けたときの処理
//=========================================
void CPlayer::Damage(int nDamage)
{
	if (m_pMotion)
	{// モーションクラスが使われている
		if (NULL != m_pLife && m_pMotion->GetType() != CMotionManager::TYPE_DAMAGE && !m_bDamage)
		{// ライフクラスが使われている
			m_pLife->AddSubtract(nDamage);
			m_pMotion->SetMotion(CMotionManager::TYPE_DAMAGE);	// ダメージモーションを再生
			m_bDamage = true;									// ダメージを受けている状態にする
			CSound *pSound = CManager::GetSound();				// サウンドの取得
			pSound->PlaySound(CSound::SOUND_LABEL_DAMAGE);		// ダメージ音を再生

			if (0 >= m_pLife->GetLife())
			{// 体力が0以下になった
				Uninit();
				CGame::SetGameState(CGame::STATE_END);
			}
		}
	}
}