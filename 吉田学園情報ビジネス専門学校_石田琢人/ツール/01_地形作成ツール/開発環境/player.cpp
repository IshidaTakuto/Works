//=============================================================================
//
// プレイヤー処理 [Player.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "meshField.h"
#include "debugProc.h"
#include "meshCylinder.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODELNAME		"data/MODEL/player000.x"		// 読み込むxファイル名

//==================================
// 静的メンバ変数宣言
//==================================
LPDIRECT3DTEXTURE9	CPlayer::m_pTexture[MAX_PARTS_PLAYER] = {};
LPD3DXMESH			CPlayer::m_pMesh[MAX_PARTS_PLAYER] = {};
LPD3DXBUFFER		CPlayer::m_pBuffMat[MAX_PARTS_PLAYER] = {};
DWORD				CPlayer::m_nNumMat[MAX_PARTS_PLAYER] = {};

//==================================
// 生成処理
//==================================
CPlayer* CPlayer::Create(void)
{
	CPlayer *pPlayer;

	pPlayer = new CPlayer;	// メモリを確保
	pPlayer->BindInfo(m_pTexture[0], m_pMesh[0], m_pBuffMat[0], m_nNumMat[0]);
	pPlayer->Init();

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

	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	for (int nCntModel = 0; nCntModel < MAX_PARTS_PLAYER; nCntModel++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(MODELNAME,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_pBuffMat[nCntModel],
			NULL,
			&m_nNumMat[nCntModel],
			&m_pMesh[nCntModel]);

		pMat = (D3DXMATERIAL*)m_pBuffMat[nCntModel]->GetBufferPointer();
		D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[nCntModel]);
	}

	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CPlayer::Unload(void)
{
	for (int nCntModel = 0; nCntModel < MAX_PARTS_PLAYER; nCntModel++)
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
		{// マテリアルの数を破棄
			m_nNumMat[nCntModel] = NULL;
		}
	}
}

//=========================================
// コンストラクタ
//=========================================
CPlayer::CPlayer()
{
	m_fSpeed = 0.0f;
	m_fRange = 1.0f;
	m_fHeight = 0.0f;
}

//=========================================
// デストラクタ
//=========================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	CSceneX::Init();

	// ポリゴンの位置を設定
	CSceneX::SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_fSpeed = 2.5f;			// 速度調整

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	CSceneX::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// キーボードの入力を取得
	CInputKeyboard *pKeyboard = CManager::GetInput();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	float fDestAngle = 0;								// 目的の角度
	D3DXVECTOR3 pos = CSceneX::GetPos();				// 位置の取得

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
	pos += move;

	float fHeight = 0.0f;	// 起伏の高さを求める
	pos.y = CManager::GetMeshField()->CMeshField::GetHeight(pos);

	if (pKeyboard->GetPress(DIK_Y) == true)
	{// 頂点の高さの設定
		CManager::GetMeshField()->CMeshField::SetHeight(pos, m_fHeight, m_fRange);
	}

	if (pKeyboard->GetPress(DIK_X) == true)
	{
		float fLength = 0;	// 長さの値
		int nMesh = 0;		// 分割数

		if (pKeyboard->GetPress(DIK_RSHIFT) || pKeyboard->GetPress(DIK_LSHIFT))
		{// シフトを押されている
			fLength = 5.0f;
			nMesh = 1;
		}
		else
		{// 押されてない
			fLength = 0.1f;
			nMesh = 1;
		}

		if (pKeyboard->GetRepeat(DIK_I) == true)
		{// X分割数を上げる
			CManager::GetMeshField()->CMeshField::SetNumMeshX(nMesh);
		}
		else if (pKeyboard->GetRepeat(DIK_K) == true)
		{// X分割数を下げる
			CManager::GetMeshField()->CMeshField::SetNumMeshX(-nMesh);
		}

		if (pKeyboard->GetRepeat(DIK_U) == true)
		{// X長さを上げる
			CManager::GetMeshField()->CMeshField::SetLengthX(fLength);
		}
		else if (pKeyboard->GetRepeat(DIK_J) == true)
		{// X長さを下げる
			CManager::GetMeshField()->CMeshField::SetLengthX(-fLength);
		}
	}

	if (pKeyboard->GetPress(DIK_Z) == true)
	{
		float fLength = 0;	// 長さの値
		int nMesh = 0;		// 分割数

		if (pKeyboard->GetPress(DIK_RSHIFT) || pKeyboard->GetPress(DIK_LSHIFT))
		{// シフトを押されている
			fLength = 5.0f;
			nMesh = 1;
		}
		else
		{// 押されてない
			fLength = 0.1f;
			nMesh = 1;
		}

		if (pKeyboard->GetRepeat(DIK_I) == true)
		{// Z分割数を上げる
			CManager::GetMeshField()->CMeshField::SetNumMeshZ(nMesh);
		}
		else if (pKeyboard->GetRepeat(DIK_K) == true)
		{// Z分割数を下げる
			CManager::GetMeshField()->CMeshField::SetNumMeshZ(-nMesh);
		}

		if (pKeyboard->GetRepeat(DIK_U) == true)
		{// Z長さを上げる
			CManager::GetMeshField()->CMeshField::SetLengthZ(fLength);
		}
		else if (pKeyboard->GetRepeat(DIK_J) == true)
		{// Z長さを下げる
			CManager::GetMeshField()->CMeshField::SetLengthZ(-fLength);
		}
	}

	if (pKeyboard->GetRepeat(DIK_UP))
	{// 範囲を拡大
		if (pKeyboard->GetPress(DIK_RSHIFT) || pKeyboard->GetPress(DIK_LSHIFT))
		{// シフトを押されている
			m_fRange += 10.0f;
		}
		else
		{// 押されてない
			m_fRange += 0.2f;
		}
	}
	if (pKeyboard->GetRepeat(DIK_DOWN))
	{// 範囲を縮小
		if (pKeyboard->GetPress(DIK_RSHIFT) || pKeyboard->GetPress(DIK_LSHIFT))
		{// シフトを押されている
			m_fRange -= 10.0f;
		}
		else
		{// 押されてない
			m_fRange -= 0.2f;
		}

		if (m_fRange <= 0.2f)
		{// 負の数になった
			m_fRange = 0.2f;
		}
	}
	if (pKeyboard->GetRepeat(DIK_RIGHT))
	{// 上げる値を増やす
		if (pKeyboard->GetPress(DIK_RSHIFT) || pKeyboard->GetPress(DIK_LSHIFT))
		{// シフトを押されている
			m_fHeight += 1.0f;
		}
		else
		{// 押されてない
			m_fHeight += 0.02f;
		}
	}
	if (pKeyboard->GetRepeat(DIK_LEFT))
	{// 上げる値を減らす
		if (pKeyboard->GetPress(DIK_RSHIFT) || pKeyboard->GetPress(DIK_LSHIFT))
		{// シフトを押されている
			m_fHeight -= 1.0f;
		}
		else
		{// 押されてない
			m_fHeight -= 0.02f;
		}
	}

	CSceneX::SetPos(pos);	// 位置の設定

	//CManager::GetCylinder()->SetRange(m_fRange);
	//CManager::GetCylinder()->SetPos(m_pos);

	// デバッグ表示
	CDebugProc::Print("選択範囲 ：%.1f", m_fRange);
	CDebugProc::Print("上げる値 ：%.2f", m_fHeight);
}

//=========================================
// 描画処理
//=========================================
void CPlayer::Draw(void)
{
	CSceneX::Draw();
}
