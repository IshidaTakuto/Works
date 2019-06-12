//=============================================================================
//
// 配置用プレイヤー処理 [setPlayer.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "setPlayer.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "meshField.h"
#include "modelObject.h"
#include "GUIManager.h"
#include "model.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// 生成処理
//=============================================================================
CSetPlayer* CSetPlayer::Create(CModelObjectManager::TYPE type)
{
	CSetPlayer *pPlayer = NULL;

	pPlayer = new CSetPlayer;	// メモリを確保

	if (pPlayer != NULL)
	{// メモリ確保成功
		pPlayer->m_type = type;
		pPlayer->Init();
	}

	return pPlayer;
}

//=============================================================================
// モデルの設定
//=============================================================================
void CSetPlayer::SetModel(char **pModelName, int nNumModel)
{
	int nCntModel = 0;	// ループ用カウンタ

	if (NULL != m_pModel)
	{// 使われている
		for (nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{// メモリの開放
			delete m_pModel[nCntModel];
			m_pModel[nCntModel] = NULL;
		}

		// メモリの開放
		delete m_pModel;
		m_pModel = NULL;
	}

	if (NULL == m_pModel)
	{// 使われていない
		if (CModelObjectManager::TYPE_ENEMY == m_type)
		{// 敵
			m_nNumModel = CEnemy::GetNumParts(m_nType);	// モデル数の取得
			m_pModel = new CModel*[m_nNumModel];		// モデル分のメモリ確保

			for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
			{// メモリ確保
				m_pModel[nCntModel] = CModel::Create();
			}

			// モデル情報を取得
			CEnemy::GetInfo(m_nType, m_pModel);
		}
		else
		{// 敵以外
			// メモリ確保
			m_pModel = new CModel*;
			m_pModel[0] = new CModel;

			m_pModel[0]->SetModel(pModelName[m_nType]);
			m_pModel[0]->SetParent(NULL);
			m_nNumModel = 1;
		}
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CSetPlayer::CSetPlayer(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_fSpeed = 0.0f;		// 移動速度
	m_fRange = 1.0f;		// 影響を及ぼす範囲
	m_fHeight = 0.0f;		// 起伏を作るときの上げる値
	m_nType = 0;			// モデルの種類
	m_type = CModelObjectManager::TYPE_MODEL;
	m_pModel = NULL;
	m_nNumModel = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CSetPlayer::~CSetPlayer()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSetPlayer::Init(void)
{
	m_nNumModel = CModelObjectManager::GetNumType(m_type);

	// モデルファイル名
	char **pModelName = CModelObjectManager::GetModelFileName(m_type);

	SetModel(pModelName, m_nNumModel);

	// 位置を設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 向きを設定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_fSpeed = 2.5f;			// 速度調整

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSetPlayer::Uninit(void)
{
	if (NULL != m_pModel)
	{// 使われている
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{// メモリの開放
			delete m_pModel[nCntModel];
			m_pModel[nCntModel] = NULL;
		}

		// メモリの開放
		delete m_pModel;
		m_pModel = NULL;
	}

	// 自分を破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSetPlayer::Update(void)
{
	// キーボードの入力を取得
	CInputKeyboard *pKeyboard = CManager::GetInput();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	float fDestAngle = 0;								// 目的の角度

	// カメラの角度を取得
	float fAngleCamera = CManager::GetCamera()->GetAngle();

	if (pKeyboard->GetPress(DIK_LSHIFT))
	{// 移動速度を上げる
		m_fSpeed = 5.0f;
	}
	else
	{
		m_fSpeed = 2.5f;
	}

	// キー入力による移動---------------------------------------------------------------------------------------------
	if (pKeyboard->GetPress(DIK_A) == true)
	{// ←方向へ移動
		if (pKeyboard->GetPress(DIK_W) == true)
		{// Wキーが押された
			fDestAngle = D3DX_PI * -0.25f + fAngleCamera;
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// Sキーが押された
			fDestAngle = D3DX_PI * -0.75f + fAngleCamera;
		}
		else
		{// 左に移動
			fDestAngle = D3DX_PI * -0.5f + fAngleCamera;
		}

		// 向いている角度への移動量
		move += D3DXVECTOR3(sinf(fDestAngle), 0.0f, cosf(fDestAngle)) * m_fSpeed;
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{// →方向へ移動
		if (pKeyboard->GetPress(DIK_W) == true)
		{// Wキーが押された
			fDestAngle = D3DX_PI * 0.25f + fAngleCamera;
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// Sキーが押された
			fDestAngle = D3DX_PI * 0.75f + fAngleCamera;
		}
		else
		{// 右に移動
			fDestAngle = D3DX_PI * 0.5f + fAngleCamera;
		}

		// 向いている角度への移動量
		move += D3DXVECTOR3(sinf(fDestAngle), 0.0f, cosf(fDestAngle)) * m_fSpeed;
	}
	else if (pKeyboard->GetPress(DIK_W) == true)	//-------------------------- ↑ ------------------------------
	{// 上に移動
		fDestAngle = D3DX_PI * 0.0f + fAngleCamera;

		// 向いている角度への移動量
		move += D3DXVECTOR3(sinf(fDestAngle), 0.0f, cosf(fDestAngle)) * m_fSpeed;
	}
	else if (pKeyboard->GetPress(DIK_S) == true)	//-------------------------- ↓ ------------------------------
	{// 下に移動
		fDestAngle = D3DX_PI * 1.0f + fAngleCamera;

		// 向いている角度への移動量
		move += D3DXVECTOR3(sinf(fDestAngle), 0.0f, cosf(fDestAngle)) * m_fSpeed;
	}

	//位置更新(移動)
	m_pos += move;

	// 地面オブジェクトを探す
	CScene *pScene = CScene::GetSceneTop(MESHFIELD_PRIORITY);

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_FIELD)
		{// 地面だったとき
			CMeshField *pMeshField = (CMeshField*)pScene;

			m_pos.y = pMeshField->GetHeight(m_pos);
			pMeshField->RangeColor(m_pos, m_fRange);	// 範囲内の地面の色を変える
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}

	// 上げる高さの設定
	m_fHeight = CGuiManager::GetHeight();

	// 選択範囲の設定
	m_fRange = CGuiManager::GetRange();

	// オブジェクトの設置
	SetObject();
}

//=============================================================================
// 描画処理
//=============================================================================
void CSetPlayer::Draw(void)
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

	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		// 半透明にする
		m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.5f));

		// 描画処理
		m_pModel[nCntModel]->Draw();

		// 不透明にする
		m_pModel[nCntModel]->AddColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
	}
}

//=============================================================================
// オブジェクトを置く
//=============================================================================
void CSetPlayer::SetObject(void)
{
	// モデルを変えたかどうか
	CModelObjectManager::TYPE typeOld = m_type;		// オブジェクトの種類の保存
	int nTypeOld = m_nType;		// モデル番号の保存
	int nTypeMax = 0;

	// キーボードの入力を取得
	CInputKeyboard *pKeyboard = CManager::GetInput();

	m_type = CGuiManager::GetObjectType();

	m_nType = CGuiManager::GetType();

	nTypeMax = CModelObjectManager::GetNumType(m_type);

	if (m_nType < 0)
	{// 番号が負の数になったときループさせる
		m_nType = nTypeMax - 1;
	}
	if (m_nType >= nTypeMax)
	{// 番号が最大値を超えたときループさせる
		m_nType = 0;
	}

	if (pKeyboard->GetRepeat(DIK_Q))
	{// 向きを回転させる
		m_rot.y -= 0.1f * D3DX_PI;
	}
	if (pKeyboard->GetRepeat(DIK_E))
	{// 向きを回転させる
		m_rot.y += 0.1f * D3DX_PI;
	}

	if (pKeyboard->GetRepeat(DIK_RETURN))
	{// モデル設置
		if (CModelObjectManager::TYPE_ENEMY == m_type)
		{// 敵
			CEnemy::Create(m_pos, m_nType);
		}
		else
		{// 敵以外
			CModelObject::Create(m_pos, m_rot, m_type, m_nType);
		}
	}

	// モデルを変える
	SetModelInfo(typeOld, nTypeOld);
}

//=============================================================================
// モデル情報を変える
//=============================================================================
const void CSetPlayer::SetModelInfo(CModelObjectManager::TYPE typeOld, int nTypeOld)
{
	if (typeOld != m_type || nTypeOld != m_nType)
	{// モデルを変える
		if (NULL != m_pModel)
		{// 使われている場合破棄する
			if (CModelObjectManager::TYPE_ENEMY == typeOld)
			{// 敵だったとき
				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{// モデルクラスの破棄
					delete m_pModel[nCntModel];
					m_pModel[nCntModel] = NULL;
				}

				// 破棄
				delete m_pModel;
				m_pModel = NULL;
			}
			else
			{// 敵以外
				if (NULL != m_pModel[0])
				{// 使われている
					delete m_pModel[0];
				}

				// 破棄
				delete m_pModel;
				m_pModel = NULL;
			}
		}

		char **pModelName = NULL;

		if (NULL == m_pModel)
		{
			// モデルのパスを取得
			if (m_type == CModelObjectManager::TYPE_ENEMY)
			{// 敵
				m_nNumModel = CEnemy::GetNumParts(m_nType);
				m_pModel = new CModel*[m_nNumModel];		// モデル分のメモリ確保

				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{// メモリ確保
					m_pModel[nCntModel] = CModel::Create();
				}
				CEnemy::GetInfo(m_nType, m_pModel);
			}
			else
			{// 敵以外
				pModelName = CModelObjectManager::GetModelFileName(m_type);
				m_nNumModel = 1;

				if (NULL == m_pModel)
				{// メモリ確保
					m_pModel = new CModel*;
					m_pModel[0] = CModel::Create();
				}

				// モデルを設定する
				m_pModel[0]->SetModel(pModelName[m_nType]);
				m_pModel[0]->SetParent(NULL);

				// 初期化処理
				m_pModel[0]->Init();
			}
		}
	}
}

//=============================================================================
// 範囲チェック
//=============================================================================
bool CSetPlayer::Range(D3DXVECTOR3 pos)
{
	bool bHit = false;

	// 相手のXYの範囲
	float fLength = powf(m_fRange, 2);

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