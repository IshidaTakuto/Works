//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "bullet.h"
#include "player.h"
#include "game.h"
#include "camera.h"
#include "gauge.h"
#include "score.h"
#include "field.h"
#include "tutorial.h"
#include "result.h"
#include "title.h"
#include "particle.h"
#include "shadow.h"
#include "sound.h"

//==================================
// マクロ定義
//==================================
#define ENEMY_MODEL_AIRCRAFT	"data/MODEL/aircraft.x"
#define ENEMY_MODEL_SHIP		"data/MODEL/ship.x"
#define ENEMY_MODEL_TANK		"data/MODEL/tank.x"
#define ENEMY_SPEED		(0.4f)	// 移動の速度
#define ENEMY_LIFE		(2)		// ライフ
#define ENEMY_SHOOT		(120)	// 撃つ間隔

//==================================
// 静的メンバ変数宣言
//==================================
LPDIRECT3DTEXTURE9	CEnemy::m_pTexture[TYPE_MAX] = {};
LPD3DXMESH			CEnemy::m_pMesh[TYPE_MAX] = {};	// メッシュ情報へのポインタ
LPD3DXBUFFER		CEnemy::m_pBuffMat[TYPE_MAX] = {};	// マテリアル情報へのポインタ
DWORD				CEnemy::m_nNumMat[TYPE_MAX] = {};	// マテリアル情報の数
int					CEnemy::m_nNumEnemy = 0;

//==================================
// 生成処理
//==================================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, TYPE type)
{
	CEnemy *pEnemy;

	pEnemy = new CEnemy;	// メモリを確保
	pEnemy->BindInfo(m_pTexture[type], m_pMesh[type], m_pBuffMat[type], m_nNumMat[type]);	// モデル情報を渡す
	pEnemy->m_type = type;
	pEnemy->Init(pos);

	return pEnemy;
}

//=========================================
// テクスチャの読み込み
//=========================================
HRESULT CEnemy::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATERIAL *pMat;					// マテリアルデータへのポインタ

	// Xファイルの読み込み
	D3DXLoadMeshFromX(ENEMY_MODEL_AIRCRAFT,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[TYPE_AIRCRAFT],
		NULL,
		&m_nNumMat[TYPE_AIRCRAFT],
		&m_pMesh[TYPE_AIRCRAFT]);

	pMat = (D3DXMATERIAL*)m_pBuffMat[TYPE_AIRCRAFT]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[TYPE_AIRCRAFT]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(ENEMY_MODEL_SHIP,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[TYPE_SHIP],
		NULL,
		&m_nNumMat[TYPE_SHIP],
		&m_pMesh[TYPE_SHIP]);

	// テクスチャの生成
	pMat = (D3DXMATERIAL*)m_pBuffMat[TYPE_SHIP]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[TYPE_SHIP]);


	// Xファイルの読み込み
	D3DXLoadMeshFromX(ENEMY_MODEL_TANK,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[TYPE_TANK],
		NULL,
		&m_nNumMat[TYPE_TANK],
		&m_pMesh[TYPE_TANK]);

	// テクスチャの生成
	pMat = (D3DXMATERIAL*)m_pBuffMat[TYPE_TANK]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[TYPE_TANK]);

	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CEnemy::Unload(void)
{
	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		if (NULL != m_pTexture[nCntModel])
		{// テクスチャの破棄
			m_pTexture[nCntModel]->Release();
			m_pTexture[nCntModel] = NULL;
		}

		if (NULL != m_pMesh[nCntModel])
		{// メッシュ情報の破棄
			m_pMesh[nCntModel]->Release();
			m_pMesh[nCntModel] = NULL;
		}

		if (NULL != m_pBuffMat[nCntModel])
		{// マテリアル情報の破棄
			m_pBuffMat[nCntModel]->Release();
			m_pBuffMat[nCntModel] = NULL;
		}

		if (NULL != m_nNumMat[nCntModel])
		{// マテリアル数の初期化
			m_nNumMat[nCntModel] = 0;
		}
	}

	// 敵の総数を初期化
	m_nNumEnemy = 0;
}

//=========================================
// コンストラクタ
//=========================================
CEnemy::CEnemy(int nPriority) : CScene3DModel(nPriority)
{
	m_posOriginalZ = 0.0f;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntBullet = 0;
	m_state = STATE_NORMAL;
	m_nCntState = 0;
	m_bMoveAngle = true;
	m_nNumEnemy++;
	m_pShadow = NULL;
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
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	CScene::SetObjType(CScene::OBJTYPE_ENEMY);
	CScene3DModel::Init();

	CScene3DModel::SetPos(pos);		// ポリゴンの位置を設定
	CScene3DModel::SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	CScene3DModel::SetLife(ENEMY_LIFE);
	CScene3DModel::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	if (m_pShadow == NULL && m_type == TYPE_AIRCRAFT)
	{// 影をつける
		m_pShadow = CShadow::Create(pos);
	}

	m_posOriginalZ = pos.z;
	m_move.x = -ENEMY_SPEED;			// 常にプレイヤーの反対方向に進ませる

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CEnemy::Uninit(void)
{
	CScene3DModel::Uninit();
}

//=========================================
// 更新処理
//=========================================
void CEnemy::Update(void)
{
	D3DXVECTOR3 thisPos = CScene3DModel::GetPos();	// 位置の取得
	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 posCamera = pCamera->GetPosV();
	float vtxMinY = CScene3DModel::GetMinVtxY();

	CManager::MODE mode = CManager::GetMode();
	if (mode == CManager::MODE_TITLE)
	{// タイトルのとき
		m_move.x = -(rand() % 3 + 0.5f);

		CField *pField[MAX_FIELD];		// フィールドのポインタ
		for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
		{
			pField[nCntField] = CTitle::GetField(nCntField);

			if (pField[nCntField] != NULL)
			{
				CField::TYPE type = pField[nCntField]->GetType();		// フィールドの種類の取得

				if (pField[nCntField]->Collision(&thisPos, vtxMinY) && type == CField::TYPE_GROUND && m_type == TYPE_SHIP)
				{// 陸上にいる
					m_state = STATE_DEATH;	// 死亡状態にする
				}
				else if (pField[nCntField]->Collision(&thisPos, -vtxMinY) && type == CField::TYPE_SEA && m_type == TYPE_TANK)
				{// 海上にいる
					m_state = STATE_DEATH;	// 死亡状態にする
				}
			}
		}
		if (m_pShadow != NULL)
		{// 影の位置の設定
			m_pShadow->SetPos(D3DXVECTOR3(thisPos.x, 0.0f, thisPos.z));
		}
	}
	else
	{// タイトル以外
		CField *pField[MAX_FIELD];		// フィールドのポインタ

		for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
		{// 敵を動かす
			if (CManager::GetMode() == CManager::MODE_GAME)
			{// モードがゲームのとき
				pField[nCntField] = CGame::GetField(nCntField);		// フィールドの情報を取得
			}
			else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
			{// モードがチュートリアルのとき
				pField[nCntField] = CTutorial::GetField(nCntField);	// フィールドの情報を取得
			}

			if (NULL != pField[nCntField])
			{// 情報があるもののみ
				CField::TYPE type = pField[nCntField]->GetType();	// フィールドの種類の取得
				D3DXVECTOR3 length;
				switch (m_type)
				{// 種類によって変わる処理
				case TYPE_AIRCRAFT:		// 戦闘機
					if (m_move.y < 0.8f && m_bMoveAngle)
					{// 上昇
						m_move.y += 0.002f;

						if (m_move.y >= 0.8f)
						{// 一定まで上昇した
							m_bMoveAngle = false;
						}
					}
					else if (m_move.y > -0.8f && !m_bMoveAngle)
					{// 下降
						m_move.y -= 0.002f;

						if (m_move.y <= -0.8f)
						{// 一定まで下降した
							m_bMoveAngle = true;
						}
					}

					if (m_pShadow != NULL)
					{// 影の大きさを高さで変える
						length = m_pShadow->GetLength();
						length.x = (0.5f * thisPos.y) - length.x;
						length.z = (0.5f * thisPos.y) - length.z;

						m_pShadow->SetLength(length);	// 大きさの設定

						m_pShadow->SetPos(D3DXVECTOR3(thisPos.x, 0.0f, thisPos.z));		// 影の位置の設定
					}

					break;

				case TYPE_SHIP:			// 戦艦
					if (pField[nCntField]->Collision(&thisPos, vtxMinY) && type == CField::TYPE_GROUND)
					{// 陸上にいる
						m_state = STATE_DEATH;
					}
					break;

				case TYPE_TANK:			// 戦車
					if (pField[nCntField]->Collision(&thisPos, -vtxMinY) && type == CField::TYPE_SEA)
					{// 海上にいる
						m_state = STATE_DEATH;
					}
					break;
				}
			}
		}

		CPlayer *pPlayer = CGame::GetPlayer();				// プレイヤーの情報を取得
		if (pPlayer != NULL)
		{
			// プレイヤーの種類によって変わる処理
			D3DXVECTOR3 posPlayer = pPlayer->GetPos();			// プレイヤーの位置を取得
			CPlayer::TYPE typePlayer = pPlayer->GetType();		// プレイヤーの種類を取得
			m_nCntBullet = (m_nCntBullet + 1) % ENEMY_SHOOT;	// 弾を撃つ間隔
			CSound *pSound = CManager::GetSound();				// サウンドの情報を取得

			if (typePlayer == CPlayer::TYPE_AIRCRAFT)
			{// プレイヤーが戦闘機
				if (posPlayer.x + 50.0f < thisPos.x)
				{// 後ろへ弾を撃たない
					if (thisPos.x > posCamera.x - 310 && thisPos.x < posCamera.x + 310)
					{// 画面内にいるとき
						if (m_nCntBullet == 0)
						{// 弾を撃つ
							pSound->PlaySound(CSound::SOUND_LABEL_BULLET000);
							CBullet::Create(thisPos, CBullet::TYPE_ENEMY);
						}
					}
				}

				thisPos.z = 0;
				m_move.z = 0;
			}
			else if (typePlayer == CPlayer::TYPE_SHIP)
			{// プレイヤーが戦艦
				if (thisPos.x > posCamera.x - 250 && thisPos.x < posCamera.x + 250)
				{// 画面内にいるとき
					if (m_nCntBullet == 0)
					{// 弾を撃つ
						pSound->PlaySound(CSound::SOUND_LABEL_BULLET000);
						CBullet::Create(thisPos, CBullet::TYPE_ENEMY);
					}
				}

				thisPos.z = m_posOriginalZ;		// Z軸を元の位置に戻す
			}
			else if (typePlayer == CPlayer::TYPE_TANK)
			{// プレイヤーが戦車
				if (posPlayer.x + 50.0f < thisPos.x)
				{// 後ろへ弾を撃たない
					if (m_nCntBullet == 0)
					{// 弾を撃つ
						pSound->PlaySound(CSound::SOUND_LABEL_BULLET000);
						CBullet::Create(D3DXVECTOR3(thisPos.x, thisPos.y + 20.0f, thisPos.z), CBullet::TYPE_ENEMY);
					}
				}

				thisPos.z = m_posOriginalZ;		// Z軸を元の位置に戻す
			}
		}
	}

	// 敵の位置の設定
	thisPos += m_move;
	CScene3DModel::SetPos(thisPos);
	CSound *pSound = CManager::GetSound();				// サウンドの情報を取得

	if (thisPos.x < posCamera.x - 400)
	{// 画面から消えた時
		m_state = STATE_DEATH;	// 死亡状態にする
	}

	int nLife = CScene3DModel::GetLife();		// ライフを取得

	switch (m_state)
	{
	case STATE_NORMAL:		// 通常状態
		break;

	case STATE_DAMAGE:		// ダメージ状態
		m_nCntState++;		// ステート時間をカウントする
		if (m_nCntState % 10 == 0 && nLife > 0)
		{// 通常時に戻す
			if (mode == CManager::MODE_GAME)
			{// モードがゲームのときのみ
				CScore::AddScore(300);		// スコア獲得
			}
			m_state = STATE_NORMAL;
			m_nCntState = 0;
		}
		else if (nLife <= 0)
		{// ライフがなくなったとき
			if (mode == CManager::MODE_GAME)
			{// モードがゲームのときのみ
				CScore::AddScore(1000);		// スコア獲得
				CResult::AddKillEnemy();	// リザルトの倒した敵数を加算
			}

			// 爆発と黒煙
			CParticle::Create(thisPos, 6);
			CParticle::Create(thisPos, 1);
			pSound->PlaySound(CSound::SOUND_LABEL_EXPLOSION);

			CGauge::AddSubtract(10);
			m_state = STATE_DEATH;	// 死亡状態にする
		}
		break;

	case STATE_DEATH:		// 死亡状態
		if (m_pShadow != NULL)
		{
			m_pShadow->Uninit();
		}
		Uninit();					// 終了処理
		m_nNumEnemy--;
		break;
	}
}

//=========================================
// 描画処理
//=========================================
void CEnemy::Draw(void)
{
	CScene3DModel::Draw();
}
