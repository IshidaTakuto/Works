//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "manager.h"
#include "bullet.h"
#include "renderer.h"
#include "life.h"
#include "stock.h"
#include "effect.h"
#include "game.h"
#include "camera.h"
#include "field.h"
#include "gauge.h"
#include "scene2D.h"
#include "tutorial.h"
#include "particle.h"
#include "scene3DIndex.h"
#include "meshCylinder.h"
#include "formTime.h"
#include "sound.h"

//==================================
// マクロ定義
//==================================
#define PLAYER_MODEL_AIRCRAFT		"data/MODEL/aircraft.x"
#define PLAYER_MODEL_SHIP			"data/MODEL/ship.x"
#define PLAYER_MODEL_TANK			"data/MODEL/tank.x"
#define MOVE_SPEED					(5.0f)	// 移動の速度
#define TRANSFORM_TIME				(120)	// 変形できるまでの時間
#define NUM_SHIP_SPBULLET			(20)	// 戦艦の特殊弾の数
#define BULLET_INTERVAL_PLAYER		(20)	// 弾の発射間隔

//==================================
// 静的メンバ変数宣言
//==================================
LPDIRECT3DTEXTURE9	CPlayer::m_pTexture[TYPE_MAX] = {};	// テクスチャ情報へのポインタ
LPD3DXMESH			CPlayer::m_pMesh[TYPE_MAX] = {};	// メッシュ情報へのポインタ
LPD3DXBUFFER		CPlayer::m_pBuffMat[TYPE_MAX] = {};	// マテリアル情報へのポインタ
DWORD				CPlayer::m_nNumMat[TYPE_MAX] = {};	// マテリアル情報の数

//==================================
// 生成処理
//==================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, TYPE type)
{
	CPlayer *pPlayer = NULL;

	switch (type)
	{
	case TYPE_AIRCRAFT:
		pPlayer = new CPlayerAircraft;	// メモリを確保
		break;

	case TYPE_SHIP:
		pPlayer = new CPlayerShip;	// メモリを確保
		break;

	case TYPE_TANK:
		pPlayer = new CPlayerTank;	// メモリを確保
		break;
	}

	// モデル情報の設定
	pPlayer->BindInfo(m_pTexture[type], m_pMesh[type], m_pBuffMat[type], m_nNumMat[type]);

	// 初期化処理
	pPlayer->Init(pos);

	// 現在の形態の設定
	pPlayer->m_type = type;

	// プレイヤー情報の設定
	CGame::SetPlayer(pPlayer);

	return pPlayer;
}

//=========================================
// テクスチャの読み込み
//=========================================
HRESULT CPlayer::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(PLAYER_MODEL_AIRCRAFT,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[0],
		NULL,
		&m_nNumMat[0],
		&m_pMesh[0]);

	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	// テクスチャの生成
	pMat = (D3DXMATERIAL*)m_pBuffMat[0]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[0]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(PLAYER_MODEL_SHIP,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[1],
		NULL,
		&m_nNumMat[1],
		&m_pMesh[1]);

	// テクスチャの生成
	pMat = (D3DXMATERIAL*)m_pBuffMat[1]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[1]);


	// Xファイルの読み込み
	D3DXLoadMeshFromX(PLAYER_MODEL_TANK,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[2],
		NULL,
		&m_nNumMat[2],
		&m_pMesh[2]);

	// テクスチャの生成
	pMat = (D3DXMATERIAL*)m_pBuffMat[2]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[2]);

	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CPlayer::Unload(void)
{
	for (int nCntInfo = 0; nCntInfo < TYPE_MAX; nCntInfo++)
	{// 形態分ループ
		if (NULL != m_pTexture)
		{// テクスチャの破棄
			m_pTexture[nCntInfo]->Release();
			m_pTexture[nCntInfo] = NULL;
		}

		if (NULL != m_pMesh[nCntInfo])
		{// メッシュ情報の破棄
			m_pMesh[nCntInfo]->Release();
			m_pMesh[nCntInfo] = NULL;
		}

		if (NULL != m_pBuffMat[nCntInfo])
		{// マテリアル情報の破棄
			m_pBuffMat[nCntInfo]->Release();
			m_pBuffMat[nCntInfo] = NULL;
		}

		if (NULL != m_nNumMat[nCntInfo])
		{// マテリアル数を初期化する
			m_nNumMat[nCntInfo] = 0;
		}
	}
}

//=========================================
// コンストラクタ
//=========================================
CPlayer::CPlayer(int nPriority) : CScene3DModel(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	m_state = STATE_APPEAR;					// 状態
	m_nCntState = 0;						// 状態が変わるまでのカウンタ
	m_type = TYPE_AIRCRAFT;					// 形態
	m_bDisp = true;							// 描画するかどうか
	m_nCntBullet = 0;						// 弾の発射間隔
	m_pLife = NULL;							// ライフバー
	m_pFormTime = NULL;						// 変形バー
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
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	// オブジェクトタイプの設定
	CScene::SetObjType(CScene::OBJTYPE_PLAYER);

	// 初期化処理
	CScene3DModel::Init();

	// 位置を設定
	CScene3DModel::SetPos(pos);
	CScene3DModel::SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

	if (NULL == m_pLife)
	{// ライフゲージの生成
		m_pLife = CLife::Create();
	}

	if (NULL == m_pFormTime)
	{// 変形ゲージの生成
		m_pFormTime = CFormTime::Create();
	}

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CPlayer::Uninit(void)
{
	if (NULL != m_pFormTime)
	{// 変形ゲージの破棄
		m_pFormTime->Uninit();
	}

	CScene3DModel::Uninit();
	CGame::NullPlayer();
}

//=========================================
// 更新処理
//=========================================
void CPlayer::Update(void)
{
	int nLife = m_pLife->GetLife();		// ライフの取得
	int nStock = CStock::GetStock();	// ストックの取得
	bool bDelete = false;
	CManager::MODE mode = CManager::GetMode();

	// 状態
	switch (m_state)
	{
	case STATE_APPEAR:		// 出現状態
		m_nCntState++;		// ステート時間をカウントする

		if (m_nCntState % 120 == 0)
		{// 通常時に戻す
			m_state = STATE_NORMAL;
			CScene3DModel::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 色をもとに戻す
			m_nCntState = 0;
		}
		break;

	case STATE_NORMAL:		// 通常状態
		break;

	case STATE_DAMAGE:		// ダメージ状態
		m_nCntState++;		// ステート時間をカウントする
		CScene3DModel::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	// 赤くする

		if (m_nCntState % 40 == 0 && nLife > 0)
		{// 通常時に戻す
			m_state = STATE_NORMAL;
			CScene3DModel::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 色をもとに戻す
			m_nCntState = 0;
		}
		else if (nLife <= 0)
		{// ライフがなくなったとき
			m_state = STATE_DEATH;	// 死亡状態にする
		}
		break;

	case STATE_DEATH:		// 死亡状態
		if (nStock <= 0)
		{// ストックが0以下のとき
			if (mode == CManager::MODE_TUTORIAL)
			{// チュートリアルの場合ストックを増やす
				CStock::AddSubtract(1);
			}
			else
			{
				bDelete = true;		// フラグを立てる
				CGame::SetGameState(CGame::STATE_END);
			}
		}
		else
		{// ストックがまだあるとき
			m_bDisp = false;	// 描画しない状態にする

			m_nCntState++;		// 復活までのカウンタ

			if (m_nCntState % 50 == 0)
			{// 一定時間経過
				m_bDisp = true;	// 描画する状態にする
				CScene3DModel::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));	// 半透明にする
				m_state = STATE_APPEAR;			// 出現状態にする
				CStock::AddSubtract(-1);		// ストックを一つ減らす
				m_pLife->AddSubtract(MAX_LIFE);	// ライフを最大値にする
				m_nCntState = 0;				// ステートカウンタを初期化
			}
		}

		break;
	}

	D3DXVECTOR3 pos = CScene3DModel::GetPos();					// 位置の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// ジョイパッドの取得
	CSound *pSound = CManager::GetSound();						// サウンドの情報を取得

	// 弾を撃つ
	if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_2) || pKeyboard->GetPress(DIK_SPACE))
	{// 通常弾
		if (m_nCntBullet == 0)
		{// 一定間隔で発射
			CBullet::Create(pos, CBullet::TYPE_PLAYERNORMAL);
			pSound->PlaySound(CSound::SOUND_LABEL_BULLET000);
		}

		// 発射間隔のカウント
		m_nCntBullet = (1 + m_nCntBullet) % BULLET_INTERVAL_PLAYER;
	}

	// カメラと同じ速度で進む
	pos.x -= sinf(D3DX_PI * -0.5f) * 1.1f;

	if (pos.z < -400.0f)
	{// 一定の範囲を超えると位置を戻す
		pos.z = -400.0f;
	}
	else if (pos.z > 400.0f)
	{// 一定の範囲を超えると位置を戻す
		pos.z = 400.0f;
	}

	// 位置の更新
	CScene3DModel::SetPos(pos);

	// 変形できるかどうか
	bool bTransform = m_pFormTime->GetFormSuccess();

	if (bTransform == true)
	{// 変形できる
		bool bTransForm = false;	// 変形するかどうかのフラグ

		if (bTransForm == false)
		{// 変形中でないとき
			// 変形
			if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_21) || pKeyboard->GetTrigger(DIK_LEFT) == true)
			{// 戦闘機
				m_type = TYPE_AIRCRAFT;
				bTransForm = true;
			}
			else if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_19) || pKeyboard->GetTrigger(DIK_DOWN) == true)
			{// 戦車
				m_type = TYPE_TANK;
				bTransForm = true;
			}
			else if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_20) || pKeyboard->GetTrigger(DIK_RIGHT) == true)
			{// 戦艦
				m_type = TYPE_SHIP;
				bTransForm = true;
			}
		}

		if (bTransForm == true)
		{// 変形
			m_pFormTime->FormTimeReset();	// 変形ゲージの値をリセット

			D3DXVECTOR3 pos = CScene3DModel::GetPos();
			TYPE type = m_type;

			// 今のモデルを消す
			Uninit();
			bDelete = false;

			if (TYPE_AIRCRAFT == type)
			{// 戦闘機になる場合
				pos.y = 180;
				pos.z = 0;
			}

			// 指定した種類を生成
			Create(pos, type);
			CParEmitter::Create(pos, 9);	// パーティクルを出す
		}

		if (bDelete == true)
		{// 消す
			Uninit();	// 終了処理
		}
	}
}

//=========================================
// 描画処理
//=========================================
void CPlayer::Draw(void)
{
	if (m_bDisp == true)
	{
		CScene3DModel::Draw();
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 戦闘機
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=========================================
// 戦闘機のコンストラクタ
//=========================================
CPlayerAircraft::CPlayerAircraft(int nPriority) : CPlayer(nPriority)
{
}

//=========================================
// 戦闘機のデストラクタ
//=========================================
CPlayerAircraft::~CPlayerAircraft()
{
}

//=========================================
// 戦闘機の初期化処理
//=========================================
HRESULT CPlayerAircraft::Init(D3DXVECTOR3 pos)
{
	CPlayer::Init(pos);

	return S_OK;
}

//=========================================
// 戦闘機の終了処理
//=========================================
void CPlayerAircraft::Uninit(void)
{
	CPlayer::Uninit();
}

//=========================================
// 戦闘機の更新処理
//=========================================
void CPlayerAircraft::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// ジョイパッドの取得

	D3DXVECTOR3 pos = CScene3DModel::GetPos();	// 位置の取得
	D3DXVECTOR3 posOld = pos;

	if (m_state != STATE_DEATH)
	{// 死亡状態のとき以外
		// キー入力による移動---------------------------------------------------------------------------------------------
		if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_13) || pKeyboard->GetPress(DIK_A))
		{// ←方向へ移動
			if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))
			{// 左上に移動
				m_move.x += sinf(-D3DX_PI * 0.25) * MOVE_SPEED;
				m_move.y += cosf(-D3DX_PI * 0.25) * MOVE_SPEED;
			}
			else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pKeyboard->GetPress(DIK_S))
			{// 左下に移動
				m_move.x += sinf(-D3DX_PI * 0.75f) * MOVE_SPEED;
				m_move.y += cosf(-D3DX_PI * 0.75f) * MOVE_SPEED;
			}
			else
			{// 左に移動
				m_move.x -= MOVE_SPEED;	//ポリゴンを左に移動
			}
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_12) || pKeyboard->GetPress(DIK_D))	//-------------------------- → ------------------------------
		{// →方向へ移動
			if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))
			{// 右上に移動
				m_move.x += sinf(D3DX_PI * 0.25) * MOVE_SPEED;
				m_move.y += cosf(D3DX_PI * 0.25) * MOVE_SPEED;
			}
			else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pKeyboard->GetPress(DIK_S))
			{// 右下に移動
				m_move.x += sinf(D3DX_PI * 0.75f) * MOVE_SPEED;
				m_move.y += cosf(D3DX_PI * 0.75f) * MOVE_SPEED;
			}
			else
			{//右に移動
				m_move.x += MOVE_SPEED;	//ポリゴンを右に移動
			}
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))	//-------------------------- ↑ ------------------------------
		{// 上に移動
			m_move.y += MOVE_SPEED;
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pKeyboard->GetPress(DIK_S))	//-------------------------- ↓ ------------------------------
		{// 下に移動
			m_move.y -= MOVE_SPEED;
		}

		// 移動の慣性
		m_move.x = m_move.x * 0.5f;
		m_move.y = m_move.y * 0.5f;

		if (m_move.x > 0.1f || m_move.x < -0.1f ||
			m_move.y > 0.1f || m_move.y < -0.1f)
		{// 移動量が一定以上のときエフェクトをつける
			CParticle::Create(D3DXVECTOR3(pos.x - 20.0f, pos.y, pos.z), 2);
		}
		else
		{// 移動量がない時
			CParticle::Create(D3DXVECTOR3(pos.x - 20.0f, pos.y, pos.z), 7);
		}

		// 移動させる
		pos += m_move;

		// カメラの範囲外に出ないようにする
		CCamera *pCamera = CManager::GetCamera();
		D3DXVECTOR3 posCamera = pCamera->GetPosV();

		if (pos.x < posCamera.x - 310 || pos.x > posCamera.x + 310)
		{// 左右の画面端にいるとき
			pos.x = posOld.x;	// 位置を戻す
			m_move.x = 0;		// 移動量を0にする
		}
		if (pos.y > 380 || pos.y < 90)
		{// 上下の画面端にいるとき
			pos.y = posOld.y;	// 位置を戻す
			m_move.y = 0;		// 移動量を0にする
		}
	}

	if ((pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_3) || pKeyboard->GetTrigger(DIK_B)) && MAX_GAUGE <= CGauge::Get())
	{// 特殊弾
		CBullet::Create(pos, CBullet::TYPE_PLAYERSPECIAL);
		CGauge::AddSubtract(-MAX_GAUGE);
	}

	// 位置の更新
	CScene3DModel::SetPos(pos);

	// プレイヤーの更新処理
	CPlayer::Update();
}

//=========================================
// 戦闘機の描画処理
//=========================================
void CPlayerAircraft::Draw(void)
{
	CPlayer::Draw();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 戦艦
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=========================================
// 戦艦のコンストラクタ
//=========================================
CPlayerShip::CPlayerShip(int nPriority) : CPlayer(nPriority)
{
	m_nCntSpBullet = 0;
	m_bSpShoot = false;
}

//=========================================
// 戦艦のデストラクタ
//=========================================
CPlayerShip::~CPlayerShip()
{
}

//=========================================
// 戦艦の初期化処理
//=========================================
HRESULT CPlayerShip::Init(D3DXVECTOR3 pos)
{
	CPlayer::Init(pos);

	return S_OK;
}

//=========================================
// 戦艦の終了処理
//=========================================
void CPlayerShip::Uninit(void)
{


	CPlayer::Uninit();
}

//=========================================
// 戦艦の更新処理
//=========================================
void CPlayerShip::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// ジョイパッドの取得

	D3DXVECTOR3 pos = CScene3DModel::GetPos();	// 位置の取得
	D3DXVECTOR3 posOld = pos;

	if (m_state != STATE_DEATH)
	{// 死亡状態のとき以外
		// キー入力による移動---------------------------------------------------------------------------------------------
		if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_13) || pKeyboard->GetPress(DIK_A))
		{// ←方向へ移動
			if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))
			{// 左上に移動
				m_move.x += sinf(D3DX_PI * 0.25) * MOVE_SPEED;
				m_move.z += cosf(D3DX_PI * 0.25) * MOVE_SPEED;
			}
			else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pKeyboard->GetPress(DIK_S))
			{// 左下に移動
				m_move.x += sinf(-D3DX_PI * 0.25) * MOVE_SPEED;
				m_move.z += cosf(-D3DX_PI * 0.25) * MOVE_SPEED;
			}
			else
			{//左に移動
				m_move.z += MOVE_SPEED;	//ポリゴンを左に移動
			}
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_12) || pKeyboard->GetPress(DIK_D))	//-------------------------- → ------------------------------
		{// →方向へ移動
			if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))
			{// 右上に移動
				m_move.x += sinf(D3DX_PI * 0.75f) * MOVE_SPEED;
				m_move.z += cosf(D3DX_PI * 0.75f) * MOVE_SPEED;
			}
			else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pKeyboard->GetPress(DIK_S))
			{// 右下に移動
				m_move.x += sinf(-D3DX_PI * 0.75f) * MOVE_SPEED;
				m_move.z += cosf(-D3DX_PI * 0.75f) * MOVE_SPEED;
			}
			else
			{//右に移動
				m_move.z -= MOVE_SPEED;	//ポリゴンを右に移動
			}
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))	//-------------------------- ↑ ------------------------------
		{// 上に移動
			m_move.x += MOVE_SPEED;
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pKeyboard->GetPress(DIK_S))	//-------------------------- ↓ ------------------------------
		{// 下に移動
			m_move.x -= MOVE_SPEED;
		}

		// 移動の慣性
		m_move.x = m_move.x * 0.5f;
		m_move.z = m_move.z * 0.5f;
		m_move.y = -1;

		if (m_move.x > 0.1f || m_move.x < -0.1f ||
			m_move.z > 0.1f || m_move.z < -0.1f)
		{// 移動量が一定以上のときエフェクトをつける
			CParticle::Create(D3DXVECTOR3(pos.x - 20.0f, pos.y, pos.z), 3);
		}

		// 波紋を出す
		CEffect::Create(D3DXVECTOR3(pos.x + 10, pos.y, pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 20, CEffect::TYPE_RIPPLES);

		// 移動させる
		pos += m_move;

		if (pos.z > 450 || pos.z < -450)
		{// 横の移動範囲
			pos.z = posOld.z;
			m_move.z = 0;
		}

		// 地面にめり込まないようにする
		float vtxY = CScene3DModel::GetMinVtxY();	// 一番下の頂点座標

		CField *pField[MAX_FIELD];

		for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
		{
			if (CManager::GetMode() == CManager::MODE_GAME)
			{// モードがゲームのとき
				pField[nCntField] = CGame::GetField(nCntField);		// フィールドの情報を取得
			}
			else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
			{
				pField[nCntField] = CTutorial::GetField(nCntField);		// フィールドの情報を取得
			}

			if (NULL != pField[nCntField])
			{// 情報があるもののみ
				CField::TYPE type = pField[nCntField]->GetType();	// フィールドの種類の取得

				if (pField[nCntField]->Collision(&pos, vtxY) == true && type == CField::TYPE_GROUND && m_state == STATE_NORMAL)
				{// 陸上にいる
					m_pLife->AddSubtract(-1);	// ライフを減らす
					m_state = STATE_DAMAGE;
				}
			}
		}

		// カメラの範囲外に出ないようにする
		CCamera *pCamera = CManager::GetCamera();
		D3DXVECTOR3 posCamera = pCamera->GetPosV();

		if (pos.x < posCamera.x - 150)
		{// 下の画面端にいるとき
			pos.x = posCamera.x - 150;	// 位置を戻す
			m_move.x = 0;		// 移動量を0にする
		}
		else if (pos.x > posCamera.x + 250)
		{// 上の画面端にいるとき
			pos.x = posCamera.x + 250;	// 位置を戻す
			m_move.x = 0;		// 移動量を0にする
		}

		// 位置の更新
		CScene3DModel::SetPos(pos);

		int nGauge = CGauge::Get();	// ゲージ量を取得

		// 弾を撃つ
		if ((pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_3) || pKeyboard->GetTrigger(DIK_B)) && MAX_GAUGE <= nGauge && !m_bSpShoot)
		{// 特殊弾
			m_bSpShoot = true;
			CGauge::AddSubtract(-MAX_GAUGE);
		}

		if (m_nCntSpBullet >= ((NUM_SHIP_SPBULLET + 1) * 5))
		{// 撃ち終わったとき
			m_bSpShoot = false;
			m_nCntSpBullet = 0;
		}

		if (m_bSpShoot == true)
		{// 撃っている間
			if (0 == m_nCntSpBullet % 5)
			{// 一定時間経過
				CBullet::Create(pos, CBullet::TYPE_PLAYERSPECIAL);
			}

			m_nCntSpBullet++;
		}
	}

	CPlayer::Update();	// プレイヤーの更新処理
}

//=========================================
// 戦艦の描画処理
//=========================================
void CPlayerShip::Draw(void)
{
	CPlayer::Draw();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 戦車
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=========================================
// 戦車のコンストラクタ
//=========================================
CPlayerTank::CPlayerTank(int nPriority) : CPlayer(nPriority)
{
}

//=========================================
// 戦車のデストラクタ
//=========================================
CPlayerTank::~CPlayerTank()
{
}

//=========================================
// 戦車の初期化処理
//=========================================
HRESULT CPlayerTank::Init(D3DXVECTOR3 pos)
{
	CPlayer::Init(pos);

	return S_OK;
}

//=========================================
// 戦車の終了処理
//=========================================
void CPlayerTank::Uninit(void)
{
	CPlayer::Uninit();
}

//=========================================
// 戦車の更新処理
//=========================================
void CPlayerTank::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// ジョイパッドの取得

	D3DXVECTOR3 pos = CScene3DModel::GetPos();	// 位置の取得
	D3DXVECTOR3 posOld = pos;

	if (m_state != STATE_DEATH)
	{// 死亡状態のとき以外
		// キー入力による移動---------------------------------------------------------------------------------------------
		if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_13) || pKeyboard->GetPress(DIK_A))
		{// ←方向へ移動
			if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))
			{// 左上に移動
				m_move.x += sinf(D3DX_PI * 0.25) * (MOVE_SPEED * 0.5f);
				m_move.z += cosf(D3DX_PI * 0.25) * MOVE_SPEED;
			}
			else
			{//左に移動
				m_move.z += MOVE_SPEED;	//ポリゴンを左に移動
			}
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_12) || pKeyboard->GetPress(DIK_D))	//-------------------------- → ------------------------------
		{// →方向へ移動
			if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))
			{// 右上に移動
				m_move.x += sinf(D3DX_PI * 0.75f) * (MOVE_SPEED * 0.5f);
				m_move.z += cosf(D3DX_PI * 0.75f) * MOVE_SPEED;
			}
			else
			{//右に移動
				m_move.z -= MOVE_SPEED;	//ポリゴンを右に移動
			}
		}
		else if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pKeyboard->GetPress(DIK_W))	//-------------------------- ↑ ------------------------------
		{// 前に移動
			m_move.x += MOVE_SPEED * 0.5f;
		}

		if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pKeyboard->GetPress(DIK_S))
		{// 速度を落とす
			pos.x -= sinf(D3DX_PI * -0.5f) * 0.5f;
		}
		else
		{
			pos.x -= sinf(D3DX_PI * -0.5f) * 1.1f;
		}

		// 移動の慣性
		m_move.x = m_move.x * 0.5f;
		m_move.z = m_move.z * 0.5f;
		m_move.y -= 1;

		if (m_move.x > 0.1f || m_move.x < -0.1f ||
			m_move.z > 0.1f || m_move.z < -0.1f)
		{// 移動量が一定以上のときエフェクトをつける
			CParticle::Create(D3DXVECTOR3(pos.x - 20.0f, 0.0f, pos.z), 4);
		}
		else
		{// 移動量がない時
			CParticle::Create(D3DXVECTOR3(pos.x - 20.0f, 0.0f, pos.z), 8);
		}

		// 移動させる
		pos += m_move;

		if (pos.z > 450 || pos.z < -450)
		{// 横の移動範囲
			pos.z = posOld.z;
			m_move.z = 0;
		}

		// 地面にめり込まないようにする
		float vtxY = CScene3DModel::GetMinVtxY();	// 一番下の頂点座標
		CField *pField[MAX_FIELD];

		for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
		{
			if (CManager::GetMode() == CManager::MODE_GAME)
			{// モードがゲームのとき
				pField[nCntField] = CGame::GetField(nCntField);		// フィールドの情報を取得
			}
			else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
			{
				pField[nCntField] = CTutorial::GetField(nCntField);		// フィールドの情報を取得
			}

			if (NULL != pField[nCntField])
			{// 情報があるもののみ
				CField::TYPE type = pField[nCntField]->GetType();	// フィールドの種類の取得

				if (pField[nCntField]->Collision(&pos, -vtxY) == true && type == CField::TYPE_SEA && m_state == STATE_NORMAL)
				{// 海上にいる
					m_pLife->AddSubtract(-1);	// ライフを減らす
					m_state = STATE_DAMAGE;
				}
			}
		}

		int nGauge = CGauge::Get();	// ゲージ量を取得

		// 弾を撃つ
		if ((pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_3) || pKeyboard->GetTrigger(DIK_B)) && MAX_GAUGE <= nGauge)
		{// 特殊弾
			CBullet::Create(D3DXVECTOR3(pos.x,pos.y + 20.f,pos.z), CBullet::TYPE_PLAYERSPECIAL);
			CGauge::Set(0);
		}
	}

	// 位置の更新
	CScene3DModel::SetPos(pos);

	CPlayer::Update();	// プレイヤーの更新処理
}

//=========================================
// 戦車の描画処理
//=========================================
void CPlayerTank::Draw(void)
{
	CPlayer::Draw();
}
