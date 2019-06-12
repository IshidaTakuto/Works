//=============================================================================
//
// 弾処理 [bullet.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "enemy.h"
#include "effect.h"
#include "life.h"
#include "gauge.h"
#include "camera.h"
#include "particle.h"
#include "sound.h"

//==================================
// マクロ定義
//==================================
#define TEXTURE_NAME			"data/TEXTURE/bullet000.png"
#define BULLET_SPEED			(6.0f)	// 移動の速度
#define BULLET_LIFE				(120)	// 寿命
#define BULLET_LENGTH			(3.0f)	// 大きさ
#define BULLET_EFFECT_SIZE		(10.0f)	// 弾のエフェクトの大きさ
#define BULLET_EFFECT_LIFE		(10)	// 弾のエフェクトの寿命
#define AIRCRAFT_SPECIAL_SIZE	(10.0f)	// 戦闘機の特殊弾の大きさ
#define SHIP_SPECIAL_SIZE		(20.0f)	// 戦艦の特殊弾の大きさ
#define TANK_SPECIAL_SIZE		(15.0f)	// 戦車の特殊弾の大きさ

//==================================
// 静的メンバ変数宣言
//==================================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;
int CBulletPlayerSpecial::m_nCntAngle = 0;

//==================================
// 生成処理
//==================================
CBullet* CBullet::Create(D3DXVECTOR3 pos, TYPE type)
{
	CBullet *pBullet = NULL;

	switch (type)
	{
	case TYPE_PLAYERNORMAL:			// プレイヤーの通常弾
		pBullet = new CBulletPlayerNormal;	// メモリを確保
		break;

	case TYPE_PLAYERSPECIAL:		// プレイヤーの特殊弾
		pBullet = new CBulletPlayerSpecial;	// メモリを確保
		break;

	case TYPE_ENEMY:				// 敵の弾
		pBullet = new CBulletEnemy;	// メモリを確保
		break;
	}

	pBullet->Init(pos);
	pBullet->BindTexture(m_pTexture);

	return pBullet;
}

//=========================================
// テクスチャの読み込み
//=========================================
HRESULT CBullet::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);

	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CBullet::Unload(void)
{
	if (NULL != m_pTexture)
	{// テクスチャの破棄
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=========================================
// コンストラクタ
//=========================================
CBullet::CBullet(int nPriority) : CScene3DBill(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
}

//=========================================
// デストラクタ
//=========================================
CBullet::~CBullet()
{
}

//=========================================
// 弾の初期化処理
//=========================================
HRESULT CBullet::Init(D3DXVECTOR3 pos)
{
	CScene3DBill::Init();

	// ポリゴンの位置を設定
	CScene3DBill::SetPos(pos);

	// ポリゴンのサイズの設定
	CScene3DBill::SetLength(BULLET_LENGTH, BULLET_LENGTH);

	// ライフの設定
	CScene3DBill::SetLife(BULLET_LIFE);

	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_BULLET);

	return S_OK;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// プレイヤーの弾
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=========================================
// コンストラクタ
//=========================================
CBulletPlayer::CBulletPlayer()
{
}

//=========================================
// デストラクタ
//=========================================
CBulletPlayer::~CBulletPlayer()
{
}

//=========================================
// プレイヤーの弾の初期化処理
//=========================================
HRESULT CBulletPlayer::Init(D3DXVECTOR3 pos)
{
	CBullet::Init(pos);
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_move.x = BULLET_SPEED;

	return S_OK;
}

//=========================================
// プレイヤーの弾の終了処理
//=========================================
void CBulletPlayer::Uninit(void)
{
	CScene3DBill::Uninit();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		プレイヤー通常弾
//
//=========================================
// コンストラクタ
//=========================================
CBulletPlayerNormal::CBulletPlayerNormal()
{
}

//=========================================
// デストラクタ
//=========================================
CBulletPlayerNormal::~CBulletPlayerNormal()
{
}

//=========================================
// プレイヤーの通常弾の初期化処理
//=========================================
HRESULT CBulletPlayerNormal::Init(D3DXVECTOR3 pos)
{
	CBulletPlayer::Init(pos);
	return S_OK;
}

//=========================================
// プレイヤーの通常弾の終了処理
//=========================================
void CBulletPlayerNormal::Uninit(void)
{
	CBulletPlayer::Uninit();
}

//=========================================
// プレイヤーの通常弾の更新処理
//=========================================
void CBulletPlayerNormal::Update(void)
{
	D3DXVECTOR3 thisPos = CScene3DBill::GetPos();	// 弾の位置

	thisPos += m_move;		// 移動量を位置に加算する

	// 位置の更新
	CScene3DBill::SetPos(thisPos);

	int nLife;						// 弾のライフを格納
	nLife = CScene3DBill::GetLife();	// 弾のライフを取得
	nLife--;
	CScene3DBill::SetLife(nLife);

	if (nLife <= 0)
	{// 寿命が尽きた
		CBulletPlayer::Uninit();
	}
	else
	{// 生きてる
		// エフェクトをつける
		CEffect::Create(thisPos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), BULLET_EFFECT_SIZE, BULLET_EFFECT_LIFE);

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			CScene3DModel *pScene3DModel;								// 3Dオブジェクトのポインタ変数
			pScene3DModel = (CScene3DModel*)GetScene(ENEMY_PRIORITY, nCntScene);		// インデックス順に取得

			if (pScene3DModel != NULL)
			{// nullチェック
				CScene::OBJTYPE objType;					// オブジェクトの種類判別用の変数
				objType = pScene3DModel->GetObjType();			// オブジェクトの種類を取得

				if (objType == CScene::OBJTYPE_ENEMY)
				{// オブジェクトの種類が敵のとき
					int nEnemyLife;								// 敵のライフを格納
					nEnemyLife = pScene3DModel->GetLife();		// 敵のライフを取得

					CEnemy *pEnemy;												// 敵の情報を格納
					pEnemy = (CEnemy*)pScene3DModel->GetScene(ENEMY_PRIORITY, nCntScene);	// 敵の情報を取得

					D3DXVECTOR3 pos = pEnemy->GetPos();
					float fLength = pEnemy->GetLength();
					bool bHit = false;			// 当たり判定の確認

					for (int nCntScene2 = 0; nCntScene2 < MAX_OBJECT; nCntScene2++)
					{
						CScene3DModel *pScene3DModel2;								// 3Dオブジェクトのポインタ変数
						pScene3DModel2 = (CScene3DModel*)GetScene(PLAYER_PRIORITY, nCntScene2);		// インデックス順に取得

						if (pScene3DModel2 != NULL)
						{// nullチェック
							CScene::OBJTYPE objType;					// オブジェクトの種類判別用の変数
							objType = pScene3DModel2->GetObjType();			// オブジェクトの種類を取得

							if (objType == CScene::OBJTYPE_PLAYER)
							{// オブジェクトの種類がプレイヤーのとき
								CPlayer *pPlayer;															// プレイヤーの情報を格納
								pPlayer = (CPlayer*)pScene3DModel2->GetScene(PLAYER_PRIORITY, nCntScene2);	// プレイヤーの情報を取得

								CPlayer::TYPE type;			// プレイヤーの現在の形態を格納
								type = pPlayer->GetType();	// プレイヤーの現在の形態を取得

								// カメラの情報を取得
								CCamera *pCamera = CManager::GetCamera();
								D3DXVECTOR3 posCamera = pCamera->GetPosV();

								if (CPlayer::TYPE_AIRCRAFT == type)
								{// プレイヤーが戦闘機の場合
									bHit = CScene3DBill::CollisionXY(pos, (fLength * 2.0f));	// Z軸を抜いた判定

									if (thisPos.x < posCamera.x - 370 || thisPos.x > posCamera.x + 370)
									{// 左右の画面外にいるとき
										bHit = false;				// 当たり判定チェックを無効にする
										CBulletPlayer::Uninit();	// 弾の終了処理
									}
								}
								else if (CPlayer::TYPE_SHIP == type)
								{// プレイヤーが戦艦の場合
									bHit = CScene3DBill::CollisionXZ(pos, (fLength * 2.5f));	// Y軸を抜いた判定

									if (thisPos.x < posCamera.x - 450)
									{// 下の画面外にいるとき
										bHit = false;				// 当たり判定チェックを無効にする
										CBulletPlayer::Uninit();	// 弾の終了処理
									}
									else if (thisPos.x > posCamera.x + 450)
									{// 上の画面外にいるとき
										bHit = false;				// 当たり判定チェックを無効にする
										CBulletPlayer::Uninit();	// 弾の終了処理
									}
								}
								else if (CPlayer::TYPE_TANK == type)
								{// プレイヤーが戦艦の場合
									bHit = CScene3DBill::Collision(pos, (fLength * 2.0f));		// 全軸の当たり判定
								}
							}
						}
					}

					if (true == bHit)
					{// 当たり判定が有効の場合
						if (pEnemy->GetState() == CEnemy::STATE_NORMAL)
						{// 通常時のみダメージ
							CParticle::Create(pos, 5);
							nEnemyLife = pEnemy->SubtractLife(1);		// 敵のライフを減らす
							CGauge::AddSubtract(5);
							pEnemy->SetState(CEnemy::STATE_DAMAGE);		// 敵をダメージ状態にする

							CSound *pSound = CManager::GetSound();		// サウンドの情報を取得
							pSound->PlaySound(CSound::SOUND_LABEL_HIT);
						}

 						CBulletPlayer::Uninit();	// 弾の終了処理
					}
				}
			}
		}
	}
}

//=========================================
// プレイヤーの通常弾の描画処理
//=========================================
void CBulletPlayerNormal::Draw(void)
{
	CScene3DBill::Draw();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		プレイヤー特殊弾
//
//=========================================
// コンストラクタ
//=========================================
CBulletPlayerSpecial::CBulletPlayerSpecial()
{
}

//=========================================
// デストラクタ
//=========================================
CBulletPlayerSpecial::~CBulletPlayerSpecial()
{
}

//=========================================
// プレイヤーの特殊弾の初期化処理
//=========================================
HRESULT CBulletPlayerSpecial::Init(D3DXVECTOR3 pos)
{
	CBulletPlayer::Init(pos);

	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{// 弾の設定
		CScene3DModel *pScene3DModel;					// オブジェクトのポインタ変数
		pScene3DModel = (CScene3DModel*)GetScene(PLAYER_PRIORITY, nCntScene);		// インデックス順に取得

		if (pScene3DModel != NULL)
		{// 使われているかのチェック
			CScene::OBJTYPE objType;					// オブジェクトの種類判別用の変数
			objType = pScene3DModel->GetObjType();		// オブジェクトの種類を取得

			if (objType == CScene::OBJTYPE_PLAYER)
			{// オブジェクトの種類がプレイヤーのとき
				CPlayer *pPlayer = (CPlayer*)pScene3DModel->GetScene(PLAYER_PRIORITY, nCntScene);	// プレイヤーの情報を格納
				CPlayer::TYPE type = pPlayer->GetType();		// プレイヤーの現在の形体を取得
				D3DXVECTOR3 thisPos = CScene3DBill::GetPos();	// 弾の位置を取得

				int nBasicAngle = 20;						// 半周の個数
				int nNumAngle = 6;							// [nBasicAngle]を[nNumAngle]で割った数の倍の個数を1周に置く	[(2)でnBasicAngleの値分設置]

				float fAngle = 1.0f / (nBasicAngle / nNumAngle);	// 間隔

				switch (type)
				{// フォーム別の特殊弾の設定
				case CPlayer::TYPE_AIRCRAFT:	// 戦闘機
					CScene3DBill::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
					CScene3DBill::SetLife(120);		// ビームを撃ち続ける時間
					m_move.x = 2.0f;
					break;

				case CPlayer::TYPE_SHIP:		// 戦艦
					CScene3DBill::SetLength(SHIP_SPECIAL_SIZE, SHIP_SPECIAL_SIZE);			// 弾のサイズの設定
					CEffect::Create(thisPos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 30.0f, 60);	// エフェクトをつける

					// 弾をバラバラに撃つ
					if (rand() % 2 == 0)
					{// 時計回り
						m_move = D3DXVECTOR3(sinf(D3DX_PI * (1.0f + (fAngle * m_nCntAngle))) * 3.0f,
							0,
							cosf(D3DX_PI * (1.0f + (fAngle * m_nCntAngle))) * 3.0f);
					}
					else
					{// 反時計回り
						m_move = D3DXVECTOR3(-sinf(D3DX_PI * (0.5f + (fAngle * m_nCntAngle))) * 3.0f,
							0,
							-cosf(D3DX_PI * (0.5f + (fAngle * m_nCntAngle))) * 3.0f);
					}
					m_nCntAngle = (m_nCntAngle + 1) % nBasicAngle;
					break;

				case CPlayer::TYPE_TANK:		// 戦車
					CScene3DBill::SetLength(TANK_SPECIAL_SIZE, TANK_SPECIAL_SIZE);			// 弾のサイズの設定
					CEffect::Create(thisPos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 30.0f, 60);	// エフェクトをつける
					m_move = D3DXVECTOR3(10.0f, 3.0f, 0.0f);
					break;
				}

			}
		}
	}
	return S_OK;
}

//=========================================
// プレイヤーの特殊弾の終了処理
//=========================================
void CBulletPlayerSpecial::Uninit(void)
{
	CBulletPlayer::Uninit();
}

//=========================================
// プレイヤーの特殊弾の更新処理
//=========================================
void CBulletPlayerSpecial::Update(void)
{
	D3DXVECTOR3 thisPos = CScene3DBill::GetPos();	// 弾の位置
	int nLife;							// 弾のライフを格納
	nLife = CScene3DBill::GetLife();	// 弾のライフを取得
	nLife--;							// ライフを減らす
	CScene3DBill::SetLife(nLife);		// ライフの適用

	if (nLife <= 0)
	{// 寿命が尽きた
		CBulletPlayer::Uninit();
	}
	else
	{// 生きてる
		thisPos += m_move;

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{// 敵の情報の探索
			CScene3DModel *pScene3DModel;											// 3Dオブジェクトのポインタ変数
			pScene3DModel = (CScene3DModel*)GetScene(ENEMY_PRIORITY, nCntScene);	// インデックス順に取得

			if (pScene3DModel != NULL)
			{// nullチェック
				CScene::OBJTYPE objType;					// オブジェクトの種類判別用の変数
				objType = pScene3DModel->GetObjType();		// オブジェクトの種類を取得

				if (objType == CScene::OBJTYPE_ENEMY)
				{// オブジェクトの種類が敵のとき
					CEnemy *pEnemy;															// 敵の情報を格納
					pEnemy = (CEnemy*)pScene3DModel->GetScene(ENEMY_PRIORITY, nCntScene);	// 敵の情報を取得

					D3DXVECTOR3 pos = pEnemy->GetPos();
					float fLength = pEnemy->GetLength();
					bool bHit = false;			// 当たり判定の確認

					for (int nCntScene2 = 0; nCntScene2 < MAX_OBJECT; nCntScene2++)
					{
						CScene3DModel *pScene3DModel2;								// 3Dオブジェクトのポインタ変数
						pScene3DModel2 = (CScene3DModel*)GetScene(PLAYER_PRIORITY, nCntScene2);		// インデックス順に取得

						if (pScene3DModel2 != NULL)
						{// nullチェック
							CScene::OBJTYPE objType;					// オブジェクトの種類判別用の変数
							objType = pScene3DModel2->GetObjType();		// オブジェクトの種類を取得

							if (objType == CScene::OBJTYPE_PLAYER)
							{// オブジェクトの種類がプレイヤーのとき
								CPlayer *pPlayer;															// プレイヤーの情報を格納
								pPlayer = (CPlayer*)pScene3DModel2->GetScene(PLAYER_PRIORITY, nCntScene2);	// プレイヤーの情報を取得
								D3DXVECTOR3 posPlayer = pPlayer->GetPos();		// プレイヤーの位置を取得

								CPlayer::TYPE type;			// プレイヤーの現在の形態を格納
								type = pPlayer->GetType();	// プレイヤーの現在の形態を取得

								if (CPlayer::TYPE_AIRCRAFT == type)
								{// プレイヤーが戦闘機の場合
									CEffect::Create(posPlayer, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), AIRCRAFT_SPECIAL_SIZE, 120, CEffect::TYPE_BIIM);	// エフェクトをつける

									CSound *pSound = CManager::GetSound();				// サウンドの情報を取得
									pSound->PlaySound(CSound::SOUND_LABEL_BULLET001);
								}
								else if (CPlayer::TYPE_SHIP == type)
								{// プレイヤーが戦艦の場合
									bHit = CScene3DBill::CollisionXZ(pos, (fLength * 1.8f));	// Y軸を抜いた判定
								}
								else if (CPlayer::TYPE_TANK == type)
								{// プレイヤーが戦車の場合
									m_move.y -= 0.3f;
									bHit = CScene3DBill::Collision(pos, (fLength * 1.8f));		// 全軸の当たり判定

									if (nLife <= 1 || thisPos.y <= 0)
									{// ライフがなくなると爆発させる
										CParticle::Create(thisPos, 0);
										CSound *pSound = CManager::GetSound();				// サウンドの情報を取得
										pSound->PlaySound(CSound::SOUND_LABEL_BULLET003);

										if (CScene3DBill::Collision(thisPos, 30))
										{// パーティクルの当たり判定
											if (pEnemy->GetState() == CEnemy::STATE_NORMAL)
											{// 通常時のみダメージ
												pEnemy->SubtractLife(2);					// 敵のライフを減らす

												pEnemy->SetState(CEnemy::STATE_DAMAGE);		// 敵をダメージ状態にする
											}
										}
									}
								}
							}
						}
					}

					if (true == bHit)
					{// 当たり判定が有効の場合
						if (pEnemy->GetState() == CEnemy::STATE_NORMAL)
						{// 通常時のみダメージ
							pEnemy->SubtractLife(2);					// 敵のライフを減らす

							pEnemy->SetState(CEnemy::STATE_DAMAGE);		// 敵をダメージ状態にする

							CSound *pSound = CManager::GetSound();		// サウンドの情報を取得
							pSound->PlaySound(CSound::SOUND_LABEL_HIT);
						}
					}
				}
			}
		}

		// 位置の更新
		CScene3DBill::SetPos(thisPos);
	}
}

//=========================================
// プレイヤーの特殊弾の描画処理
//=========================================
void CBulletPlayerSpecial::Draw(void)
{
	CScene3DBill::Draw();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 敵の弾
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=========================================
// コンストラクタ
//=========================================
CBulletEnemy::CBulletEnemy()
{
	m_nCntAngle = 0;
	m_fAngle = 0;
}

//=========================================
// デストラクタ
//=========================================
CBulletEnemy::~CBulletEnemy()
{
}

//=========================================
// 敵の弾の初期化処理
//=========================================
HRESULT CBulletEnemy::Init(D3DXVECTOR3 pos)
{
	CBullet::Init(pos);
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_move.x = -BULLET_SPEED;

	return S_OK;
}

//=========================================
// 敵の弾の終了処理
//=========================================
void CBulletEnemy::Uninit(void)
{
	CScene3DBill::Uninit();
}

//=========================================
// 敵の弾の更新処理
//=========================================
void CBulletEnemy::Update(void)
{
	bool bDelete = false;				// 削除する条件を満たしたかどうか
	int nLife;							// 弾のライフを格納
	nLife = CScene3DBill::GetLife();	// 弾のライフを取得

	m_nCntAngle++;	// 追尾する時間のカウント

	nLife--;
	CScene3DBill::SetLife(nLife);

	if (nLife <= 0)
	{// 寿命が尽きた
		bDelete = true;
	}
	else
	{// 生きてる
		D3DXVECTOR3 thisPos = CScene3DBill::GetPos();	// 弾の位置を取得
		CEffect::Create(thisPos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), BULLET_EFFECT_SIZE, BULLET_EFFECT_LIFE);	// エフェクトをつける

		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			CScene3DModel *pScene3DModel;					// オブジェクトのポインタ変数
			pScene3DModel = (CScene3DModel*)GetScene(PLAYER_PRIORITY, nCntScene);		// インデックス順に取得

			if (pScene3DModel != NULL)
			{// 使われているかのチェック
				CScene::OBJTYPE objType;					// オブジェクトの種類判別用の変数
				objType = pScene3DModel->GetObjType();			// オブジェクトの種類を取得

				if (objType == CScene::OBJTYPE_PLAYER)
				{// オブジェクトの種類がプレイヤーのとき
					CPlayer *pPlayer;	// プレイヤーの情報を格納
					pPlayer = (CPlayer*)pScene3DModel->GetScene(PLAYER_PRIORITY, nCntScene);	// プレイヤーの情報を取得
					D3DXVECTOR3 posPlayer = pPlayer->GetPos();					// プレイヤーの位置を取得
					CPlayer::STATE statePlayer = pPlayer->GetState();			// プレイヤーの状態を取得
					CPlayer::TYPE typePlayer = pPlayer->GetType();				// プレイヤーの形態を取得
					float fLengthPlayer = pPlayer->GetLength();					// プレイヤーのサイズを取得
					D3DXVECTOR3 BulletMove(0, 0, 0);							// 目的の角度への移動量

					// プレイヤーの形態によって弾を変える
					if (CPlayer::TYPE_AIRCRAFT == typePlayer)
					{// プレイヤーの形態が戦闘機の場合
						if (m_nCntAngle < 5)
						{// プレイヤーへの角度を計算
							m_fAngle = atan2f(posPlayer.x - thisPos.x, posPlayer.y - thisPos.y);
						}

						BulletMove.x = sinf(m_fAngle) * 5.0f;	// X軸の移動量
						BulletMove.y = cosf(m_fAngle) * 5.0f;	// Y軸の移動量

						m_move = D3DXVECTOR3(BulletMove.x, BulletMove.y, 0);		// 弾の移動量

						thisPos += m_move;	// 移動量を位置に加算する

						for (int nCntHit = 0; nCntHit < 5; nCntHit++)
						{// 当たり判定を団子状に置く
							if (CScene3DBill::CollisionXY(D3DXVECTOR3((posPlayer.x - fLengthPlayer * 2.5f) + (nCntHit * fLengthPlayer), posPlayer.y, posPlayer.z), fLengthPlayer * 2.3f) && statePlayer == CPlayer::STATE_NORMAL)
							{// 当たり判定
								bDelete = true;								// 弾を消す
								CLife *pLife = pPlayer->GetLife();			// ライフのポインタを取得
								pLife->AddSubtract(-10);					// プレイヤーのライフを減らす
								pPlayer->SetState(CPlayer::STATE_DAMAGE);	// プレイヤーをダメージ状態にする

								break;
							}
						}
					}
					else if (CPlayer::TYPE_SHIP == typePlayer)
					{// プレイヤーの形態が戦艦の場合
						thisPos.y = 10.0f;
						if (m_nCntAngle < 5)
						{// プレイヤーへの角度を計算
							m_fAngle = atan2f(posPlayer.x - thisPos.x, posPlayer.z - thisPos.z);
						}

						BulletMove.x = sinf(m_fAngle) * 5.0f;	// X軸の移動量
						BulletMove.z = cosf(m_fAngle) * 5.0f;	// Z軸の移動量

						m_move = D3DXVECTOR3(BulletMove.x, 0.0f, BulletMove.z);		// 弾の移動量

						thisPos += m_move;	// 移動量を位置に加算する

						for (int nCntHit = 0; nCntHit < 6; nCntHit++)
						{// 当たり判定を団子状に置く
							if (CScene3DBill::CollisionXZ(D3DXVECTOR3((posPlayer.x - fLengthPlayer * 3) + (nCntHit * fLengthPlayer), posPlayer.y, posPlayer.z), fLengthPlayer) && statePlayer == CPlayer::STATE_NORMAL)
							{// 当たり判定
								bDelete = true;								// 弾を消す
								CLife *pLife = pPlayer->GetLife();			// ライフのポインタを取得
								pLife->AddSubtract(-10);					// プレイヤーのライフを減らす
								pPlayer->SetState(CPlayer::STATE_DAMAGE);	// プレイヤーをダメージ状態にする

								break;
							}
						}
					}
					else if(CPlayer::TYPE_TANK == typePlayer)
					{// プレイヤーの形態が戦車の場合
						for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
						{// 敵の情報の探索
							CScene3DModel *pScene3DModel;											// 3Dオブジェクトのポインタ変数
							pScene3DModel = (CScene3DModel*)GetScene(ENEMY_PRIORITY, nCntScene);	// インデックス順に取得

							if (pScene3DModel != NULL)
							{// nullチェック
								CScene::OBJTYPE objType;					// オブジェクトの種類判別用の変数
								objType = pScene3DModel->GetObjType();		// オブジェクトの種類を取得

								if (objType == CScene::OBJTYPE_ENEMY)
								{// オブジェクトの種類が敵のとき
									CEnemy *pEnemy;															// 敵の情報を格納
									pEnemy = (CEnemy*)pScene3DModel->GetScene(ENEMY_PRIORITY, nCntScene);	// 敵の情報を取得
									if (pEnemy != NULL)
									{
										CEnemy::TYPE type = pEnemy->GetType();		// 敵の種類を取得

										if (CEnemy::TYPE_AIRCRAFT == type)
										{// 敵が戦闘機の場合
											m_move.y = -5.0f;
										}
										else
										{// 敵が戦艦か戦車の場合
											thisPos.y = 10.0f;
											if (m_nCntAngle < 5)
											{// プレイヤーへの角度を計算
												m_fAngle = atan2f(posPlayer.x - thisPos.x, posPlayer.z - thisPos.z);
											}

											BulletMove.x = sinf(m_fAngle) * 3.0f;	// X軸の移動量
											BulletMove.z = cosf(m_fAngle) * 3.0f;	// Z軸の移動量

											m_move = D3DXVECTOR3(BulletMove.x, 0.0f, BulletMove.z);		// 弾の移動量
										}
									}
									thisPos += m_move;	// 移動量を位置に加算する
								}
							}
						}

						for (int nCntHit = 0; nCntHit < 3; nCntHit++)
						{// 当たり判定を団子状に置く
							if (CScene3DBill::Collision(D3DXVECTOR3((posPlayer.x - fLengthPlayer) + (nCntHit * fLengthPlayer), posPlayer.y, posPlayer.z), fLengthPlayer) && statePlayer == CPlayer::STATE_NORMAL && !bDelete)
							{// 当たり判定
								bDelete = true;			// 弾を消す

								CLife *pLife = pPlayer->GetLife();			// ライフのポインタを取得
								pLife->AddSubtract(-10);					// プレイヤーのライフを減らす

								pPlayer->SetState(CPlayer::STATE_DAMAGE);	// プレイヤーをダメージ状態にする

								CSound *pSound = CManager::GetSound();		// サウンドの情報を取得
								pSound->PlaySound(CSound::SOUND_LABEL_HIT1);
							}
						}
					}
				}
			}
		}
		// 位置の更新
		CScene3DBill::SetPos(thisPos);

		if (thisPos.y <= 0)
		{// 地面についた
			bDelete = true;
		}
	}

	if (bDelete)
	{// 弾の終了処理
		Uninit();
	}
}

//=========================================
// 敵の弾の描画処理
//=========================================
void CBulletEnemy::Draw(void)
{
	CScene3DBill::Draw();
}

