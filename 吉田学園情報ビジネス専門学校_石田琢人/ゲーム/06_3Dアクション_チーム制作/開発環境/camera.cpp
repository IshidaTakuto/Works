//*****************************************************************************
//
//     カメラの処理[camera.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debuglog.h"

#include "scene.h"
#include "player.h"
#include "title.h"
#include "game.h"
#include "map.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define CAMERA_ANGEL_VIEW  (45.0f)               // カメラの画角
#define CAMERA_DEPTH_MIN   (10.0f)               // カメラの描画範囲(手前)
#define CAMERA_DEPTH_MAX   (40000.0f)            // カメラの描画範囲(最奥)
#define CAMERA_ANGLE_SPEED (0.01f)               // カメラをマウスで回転させるスピード倍率
#define CAMERA_MOVE_SPEED  (1.3f)                // カメラをマウスで移動させるスピード倍率
#define CAMERA_LENGTH_MIN  (40.0f)               // 視点注視点の距離の最小値
#define CAMERA_LENGTH_MAX  (400.0f)              // 視点注視点の距離の最大値

#define CAMERA_LENGTH_INI  (200.0f)              // カメラ距離の初期値

#define CAMERA_POSY_UP     (70.0f)               // カメラの高さを上げる補正値
#define CAMERA_POSY_RANGE  (300.0f)              // カメラY座標の範囲
#define CAMERA_POSY_MIN    (50.0f)               // カメラY座標の最小値

#define CAMERA_ANGLEY_MIN (-0.3f)                // カメラ縦移動の角度最小値
#define CAMERA_ANGLEY_MAX (D3DX_PI * 0.5f)       // カメラ縦移動の角度最大値

#define CAMERA_ANGLE_MOVERIVISION_X (0.00004f)   // カメラ横移動の倍率
#define CAMERA_ANGLE_MOVERIVISION_Y (0.000037f)  // カメラ縦移動の倍率

#define CAMERA_LENGTH	  (500.0f)

//=============================================================================
//    コンストラクタ
//=============================================================================
CCamera::CCamera()
{
	// 各種値のクリア
	m_PosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 現在の視点
	m_PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 現在の注視点
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 現在の向き
	m_DestRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 目的の向き
	m_VecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 上方向ベクトル
	D3DXMatrixIdentity(&m_MtxProjection);       // プロジェクションマトリックス
	D3DXMatrixIdentity(&m_MtxView);             // ビューマトリックス
	m_fLength = 0.0f;                           // 距離
	m_ViewPort.X = 0;                           // 描画領域左上X座標
	m_ViewPort.Y = 0;                           // 描画領域左上Y座標
	m_ViewPort.Width = 0;                       // 描画領域の幅
	m_ViewPort.Height = 0;                      // 描画領域の高さ
	m_ViewPort.MinZ = 0.0f;                     // 描画領域深度バッファ最小値
	m_ViewPort.MaxZ = 0.0f;                     // 描画領域深度バッファ最大値
}

//=============================================================================
//    デストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	// 各種値の初期化
	m_PosV = D3DXVECTOR3(0.0f, 300.0f, -200.0f); // 視点
	m_PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 注視点
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 向き
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);      // 上方向ベクトル

	// 視点と注視点との距離を計算
	m_fLength = CAMERA_LENGTH_INI;
	m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
	m_PosV.y = sinf(m_Rot.z) * CAMERA_POSY_RANGE + CAMERA_POSY_MIN;
	m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;

	// ビューポート設定
	m_ViewPort.X = 0;                           // 描画領域左上X座標
	m_ViewPort.Y = 0;                           // 描画領域左上Y座標
	m_ViewPort.Width = SCREEN_WIDTH;            // 描画領域の幅
	m_ViewPort.Height = SCREEN_HEIGHT;          // 描画領域の高さ
	m_ViewPort.MinZ = 0.0f;                     // 描画領域深度バッファ最小値
	m_ViewPort.MaxZ = 1.0f;                     // 描画領域深度バッファ最大値

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CCamera::Uninit(void)
{
	// 各種値のクリア
	m_PosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 視点
	m_PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 注視点
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 向き
	m_VecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 上方向ベクトル
	D3DXMatrixIdentity(&m_MtxProjection);    // プロジェクションマトリックス
	D3DXMatrixIdentity(&m_MtxView);          // ビューマトリックス
	m_ViewPort.X = 0;                        // 描画領域左上X座標
	m_ViewPort.Y = 0;                        // 描画領域左上Y座標
	m_ViewPort.Width = 0;                    // 描画領域の幅
	m_ViewPort.Height = 0;                   // 描画領域の高さ
	m_ViewPort.MinZ = 0.0f;                  // 描画領域深度バッファ最小値
	m_ViewPort.MaxZ = 0.0f;                  // 描画領域深度バッファ最大値
}

//=============================================================================
//    更新処理
//=============================================================================
void CCamera::Update(void)
{

#if _DEBUG  // デバッグ時の処理
	if (GetUpdate() == false)
	{// 更新中だったら
		CInputMouse *pMouse = CManager::GetMouse();

		if (pMouse != NULL)
		{// マウスが取得できた
			if (pMouse->GetMouseAxisZ() != 0)
			{// マウスのホイールボタンがスクロールされた
				m_fLength += 0.1f * pMouse->GetMouseAxisZ();
				if (m_fLength < CAMERA_LENGTH_MIN)
				{// 距離が一定の値より近くなった
					m_fLength = CAMERA_LENGTH_MIN;
				}
				else if (m_fLength > CAMERA_LENGTH_MAX)
				{// 距離が一定の値より遠くなった
					m_fLength = CAMERA_LENGTH_MAX;
				}

				// 視点の計算
				m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
				m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
				m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
			}

			if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE && pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
			{// マウスの左ボタンと右ボタンが同時に押された
				if (pMouse->GetMouseAxisX() != 0 || pMouse->GetMouseAxisY() != 0)
				{// マウスが動かされている
					// 同時に押されたときの位置から今のマウスカーソルの位置から視点注視点を移動させる向きを計算
					float fAngle = atan2f(((float)pMouse->GetPoint().x - (float)pMouse->GetPointOld().x), ((float)pMouse->GetPoint().y - (float)pMouse->GetPointOld().y));
					fAngle -= m_Rot.y;
					if (fAngle < -D3DX_PI)
					{// 向きが円周率を超えた
						fAngle += D3DX_PI * 2.0f;
					}
					if (fAngle > D3DX_PI)
					{// 向きが円周率を超えた
						fAngle -= D3DX_PI * 2.0f;
					}

					// マウスのX軸,Y軸から移動量を計算
					float fMovement = sqrtf((float)pMouse->GetMouseAxisX() * (float)pMouse->GetMouseAxisX() + (float)pMouse->GetMouseAxisY() * (float)pMouse->GetMouseAxisY());
					fMovement *= CAMERA_MOVE_SPEED;

					// 視点を移動させる
					m_PosR.x += sinf(fAngle) * fMovement;
					m_PosR.z -= cosf(fAngle) * fMovement;

					// 注視点を移動させる
					m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
					m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
				}
			}
			else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE)
			{// マウスの左クリックのみが押された
				// 向きをマウスの速度に合わせて回転させる
				// 縦軸
				m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
				if (m_Rot.y > D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.y -= D3DX_PI * 2.0f;
				}
				if (m_Rot.y < -D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.y += D3DX_PI * 2.0f;
				}

				// 横軸
				m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
				if (m_Rot.z < -D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.z = -D3DX_PI;
				}
				if (m_Rot.z > 0.0f)
				{// 向きが円周率を超えた
					m_Rot.z = 0.0f;
				}

				// 視点の位置を計算する
				m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
				m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
				m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
			}
			else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
			{// マウスの右クリックのみが押された
				// 向きをマウスの速度に合わせて回転させる
				// 縦軸
				m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
				if (m_Rot.y > D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.y -= D3DX_PI * 2.0f;
				}
				if (m_Rot.y < -D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.y += D3DX_PI * 2.0f;
				}

				// 横軸
				m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
				if (m_Rot.z < -D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.z = -D3DX_PI;
				}
				if (m_Rot.z > 0.0f)
				{// 向きが既定の値を超えた
					m_Rot.z = 0.0f;
				}

				// 注視点の位置を計算する
				m_PosR.x = m_PosV.x + sinf(m_Rot.y) * m_fLength;
				m_PosR.y = m_PosV.y - cosf(m_Rot.z) * m_fLength;
				m_PosR.z = m_PosV.z + cosf(m_Rot.y) * m_fLength;
			}
		}

		// 操作方法表示
		CDebugProc::Print("\n( カメラの操作方法 )\n");
		CDebugProc::Print("[ カメラの視点移動   ]   : マウス左クリック\n");
		CDebugProc::Print("[ カメラの注視点移動 ]   : マウス右クリック\n");
		CDebugProc::Print("[ カメラの移動       ]   : マウス左右同時クリック\n");
		CDebugProc::Print("[ カメラの拡大縮小   ]   : マウスホイール\n");
	}
	else
	{// 更新中だったら
		CManager::MODE mode = CManager::GetMode();  // 現在のモードを取得
		if (mode == CManager::MODE_GAME)
		{// ゲームモード中である
			GameCamera();
		}
		else if (mode == CManager::MODE_TITLE)
		{// タイトルモード中である
			TitleCamera();
		}
		else if (mode == CManager::MODE_RESULT)
		{// リザルトモード中である
			ResultCamera();
		}
		else if (mode == CManager::MODE_RANKING)
		{// ランキングモード中である
			RankingCamera();
		}
	}

	CDebugProc::Print("\nカメラの視点   : ( %.1f %.1f %.1f )\n", m_PosV.x, m_PosV.y, m_PosV.z);
	CDebugProc::Print("カメラの注視点 : ( %.1f %.1f %.1f )\n", m_PosR.x, m_PosR.y, m_PosR.z);
	CDebugProc::Print("カメラの向き   : ( %.2f %.2f %.2f )\n\n", m_Rot.x, m_Rot.y, m_Rot.z);

#else
	CManager::MODE mode = CManager::GetMode();  // 現在のモードを取得
	if (mode == CManager::MODE_GAME)
	{// ゲームモード中である
		GameCamera();
	}
	else if (mode == CManager::MODE_TITLE)
	{// タイトルモード中である
		TitleCamera();
	}
	else if (mode == CManager::MODE_RESULT)
	{// リザルトモード中である
		ResultCamera();
	}
	else if (mode == CManager::MODE_RANKING)
	{// ランキングモード中である
		RankingCamera();
	}
#endif
}

//=============================================================================
//    カメラの設定
//=============================================================================
void CCamera::SetCamera(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// デバイスの取得
		if (pDevice != NULL)
		{// デバイスが取得できた
			// ビューポート(描画領域)の設定
			pDevice->SetViewport(&m_ViewPort);

		    // プロジェクションマトリックスの初期化
			D3DXMatrixIdentity(&m_MtxProjection);

			// プロジェクションマトリックスを作成
			D3DXMatrixPerspectiveFovLH(&m_MtxProjection,
				D3DXToRadian(CAMERA_ANGEL_VIEW),
				(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
				CAMERA_DEPTH_MIN,
				CAMERA_DEPTH_MAX);

			// プロジェクションマトリックスの設定
			pDevice->SetTransform(D3DTS_PROJECTION, &m_MtxProjection);

			// ビューマトリックスの初期化
			D3DXMatrixIdentity(&m_MtxView);

			// ビューマトリックスを作成
			D3DXMatrixLookAtLH(&m_MtxView,
				&m_PosV,
				&m_PosR,
				&m_VecU);

			// ビューマトリックスの設定
			pDevice->SetTransform(D3DTS_VIEW, &m_MtxView);
		}
	}
}

//=============================================================================
//    ゲーム中のデフォルトのカメラ位置を設定する処理
//=============================================================================
void CCamera::SetDefaultCamera(D3DXVECTOR3 rot)
{
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

	// カメラのデフォルト向きを設定
	m_Rot = rot;

	if (pPlayer != NULL)
	{// プレイヤーが取得できた
		// プレイヤーの情報を保存
		D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
		D3DXVECTOR3 PlayerRot = pPlayer->GetRot();

		// 注視点の座標を設定
		m_PosR = PlayerPos;
	}

	m_PosR.y += CAMERA_POSY_UP;

	// 視点の位置を計算する
	m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
	m_PosV.y = m_PosR.y + sinf(m_Rot.z) * CAMERA_POSY_RANGE + CAMERA_POSY_MIN;
	m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
}

//=============================================================================
//    ゲーム中のカメラの動き処理
//=============================================================================
void CCamera::GameCamera(void)
{
	if (CManager::GetRenderer()->GetPause() == false)
	{// ポーズ中ではない
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

		// カメラの注視点をプレイヤーに移動させる
		if (pPlayer != NULL)
		{// プレイヤークラスへのポインタを確保できた
			CGame *pGame = CManager::GetGame();
			if (pGame != NULL)
			{// ゲームクラスが取得できた
				if (pGame->GetState() != CGame::STATE_PLAYERFALL)
				{// プレイヤーが死んだ状態ではない
					// ジョイパッドを取得
					CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);
					if (pJoyStick != NULL)
					{// ジョイスティックを取得できた
						if (pJoyStick->GetRightAxisX() >= DI_STICK_AXIS_MIN || pJoyStick->GetRightAxisX() <= -DI_STICK_AXIS_MIN
							|| pJoyStick->GetRightAxisY() >= DI_STICK_AXIS_MIN || pJoyStick->GetRightAxisY() <= -DI_STICK_AXIS_MIN)
						{// 右アナログ入力がされている
							float RightAxisX = pJoyStick->GetRightAxisX();
							float RightAxisY = pJoyStick->GetRightAxisY();

							m_Rot.y += RightAxisX * CAMERA_ANGLE_MOVERIVISION_X;
							m_Rot.z += RightAxisY * CAMERA_ANGLE_MOVERIVISION_Y;

							if (m_Rot.y > D3DX_PI)
							{// 向きが円周率を超えた
								m_Rot.y -= D3DX_PI * 2.0f;
							}
							if (m_Rot.y < -D3DX_PI)
							{// 向きが円周率を超えた
								m_Rot.y += D3DX_PI * 2.0f;
							}

							if (m_Rot.z < CAMERA_ANGLEY_MIN)
							{// 向きが最大値を超えた
								m_Rot.z = CAMERA_ANGLEY_MIN;
							}
							if (m_Rot.z > CAMERA_ANGLEY_MAX)
							{// 向きが最小値を超えた
								m_Rot.z = CAMERA_ANGLEY_MAX;
							}
						}
					}
					if (CManager::GetKeyboard()->GetPress(DIK_LEFT) == true)
					{
						m_Rot.y -= 0.025f;
						if (m_Rot.y > D3DX_PI)
						{// 向きが円周率を超えた
							m_Rot.y -= D3DX_PI * 2.0f;
						}
						if (m_Rot.y < -D3DX_PI)
						{// 向きが円周率を超えた
							m_Rot.y += D3DX_PI * 2.0f;
						}
					}
					else if (CManager::GetKeyboard()->GetPress(DIK_RIGHT) == true)
					{
						m_Rot.y += 0.025f;
						if (m_Rot.y > D3DX_PI)
						{// 向きが円周率を超えた
							m_Rot.y -= D3DX_PI * 2.0f;
						}
						if (m_Rot.y < -D3DX_PI)
						{// 向きが円周率を超えた
							m_Rot.y += D3DX_PI * 2.0f;
						}
					}
					if (CManager::GetKeyboard()->GetPress(DIK_UP) == true)
					{
						m_Rot.z -= 0.025f;
						if (m_Rot.z < CAMERA_ANGLEY_MIN)
						{// 向きが最大値を超えた
							m_Rot.z = CAMERA_ANGLEY_MIN;
						}
						if (m_Rot.z > CAMERA_ANGLEY_MAX)
						{// 向きが最小値を超えた
							m_Rot.z = CAMERA_ANGLEY_MAX;
						}
					}
					else if (CManager::GetKeyboard()->GetPress(DIK_DOWN) == true)
					{
						m_Rot.z += 0.025f;
						if (m_Rot.z < CAMERA_ANGLEY_MIN)
						{// 向きが最大値を超えた
							m_Rot.z = CAMERA_ANGLEY_MIN;
						}
						if (m_Rot.z > CAMERA_ANGLEY_MAX)
						{// 向きが最小値を超えた
							m_Rot.z = CAMERA_ANGLEY_MAX;
						}
					}

					// プレイヤーの情報を保存
					D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
					D3DXVECTOR3 PlayerRot = pPlayer->GetRot();

					// 注視点の座標を設定
					m_PosR = PlayerPos;

					m_PosR.y += CAMERA_POSY_UP;
				}
			}

			// 視点の位置を計算する
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + sinf(m_Rot.z) * CAMERA_POSY_RANGE + CAMERA_POSY_MIN;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;

			if (m_PosV.y <= 60.0f)
			{// 水の高さを下回っている
				m_PosV.y = 60.0f;
			}
		}
	}
}

//=============================================================================
//    タイトル中のカメラの動き処理
//=============================================================================
void CCamera::TitleCamera(void)
{
	m_PosR = D3DXVECTOR3(0.0f, 200.0f, 0.0f);     // 現在の注視点
	m_Rot.z = D3DX_PI * 0.3f;

	//カメラを回す速さ
	m_Rot.y += 0.005f;
	if (m_Rot.y > D3DX_PI)
	{// 円周率を超えた
		m_Rot.y -= D3DX_PI * 2.0f;
	}
	if (m_Rot.y < -D3DX_PI)
	{// 円周率を超えた
		m_Rot.y += D3DX_PI * 2.0f;
	}

	// 視点の位置を計算する
	m_PosV.x = m_PosR.x - sinf(m_Rot.y) * CAMERA_LENGTH;
	m_PosV.y = m_PosR.y + sinf(m_Rot.z) * CAMERA_POSY_RANGE + CAMERA_POSY_MIN;
	m_PosV.z = m_PosR.z - cosf(m_Rot.y) * CAMERA_LENGTH;
}

//=============================================================================
//    リザルト中のカメラの動き処理
//=============================================================================
void CCamera::ResultCamera(void)
{
	m_PosR = D3DXVECTOR3(0.0f, 200.0f, 0.0f);     // 現在の注視点
	m_Rot.z = D3DX_PI * 0.3f;

	//カメラを回す速さ
	m_Rot.y += 0.005f;
	if (m_Rot.y > D3DX_PI)
	{// 円周率を超えた
		m_Rot.y -= D3DX_PI * 2.0f;
	}
	if (m_Rot.y < -D3DX_PI)
	{// 円周率を超えた
		m_Rot.y += D3DX_PI * 2.0f;
	}

	// 視点の位置を計算する
	m_PosV.x = m_PosR.x - sinf(m_Rot.y) * CAMERA_LENGTH;
	m_PosV.y = m_PosR.y + sinf(m_Rot.z) * CAMERA_POSY_RANGE + CAMERA_POSY_MIN;
	m_PosV.z = m_PosR.z - cosf(m_Rot.y) * CAMERA_LENGTH;
}

//=============================================================================
//    ランキング中のカメラの動き処理
//=============================================================================
void CCamera::RankingCamera(void)
{
	m_PosR = D3DXVECTOR3(0.0f, 200.0f, 0.0f);     // 現在の注視点
	m_Rot.z = D3DX_PI * 0.3f;

	//カメラを回す速さ
	m_Rot.y -= 0.005f;
	if (m_Rot.y > D3DX_PI)
	{// 円周率を超えた
		m_Rot.y -= D3DX_PI * 2.0f;
	}
	if (m_Rot.y < -D3DX_PI)
	{// 円周率を超えた
		m_Rot.y += D3DX_PI * 2.0f;
	}

	// 視点の位置を計算する
	m_PosV.x = m_PosR.x - sinf(m_Rot.y) * CAMERA_LENGTH;
	m_PosV.y = m_PosR.y + sinf(m_Rot.z) * CAMERA_POSY_RANGE + CAMERA_POSY_MIN;
	m_PosV.z = m_PosR.z - cosf(m_Rot.y) * CAMERA_LENGTH;
}

//=============================================================================
//    カメラの向きの取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_Rot;
}