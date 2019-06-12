//=============================================================================
//
// メッシュフィールド処理 [meshField.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "meshField.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME	"data/TEXT/field_texture.txt"
#define MESH_X			(6)			// x軸方向のマス数
#define MESH_Z			(6)			// z軸方向のマス数
#define MESH_WIDTH		(400.0f)	// 幅
#define MESH_DEPTH		(400.0f)	// 奥行
#define MAX_POLYGON		((MESH_X * MESH_Z * 2) + (4 * (MESH_Z - 1)))		// ポリゴン数

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 *CMeshField::m_pTexture = NULL;	// テクスチャ情報へのポインタ
int CMeshField::m_nNumTex = 0;						// テクスチャ数

//==================================
// 生成処理
//==================================
CMeshField* CMeshField::Create(void)
{
	CMeshField *pMeshField;

	pMeshField = new CMeshField;	// メモリを確保
	pMeshField->Init();

	return pMeshField;
}

//=========================================
// テクスチャの読み込み
//=========================================
HRESULT CMeshField::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;			// ファイルのポインタ変数
	char aTexName[64];		// テクスチャの名前
	int nCntTexture;

	// ファイルを開く
	pFile = fopen(TEXTURE_NAME, "r");

	if (NULL != pFile)
	{// ファイルがあった
		fscanf(pFile, "%d", &m_nNumTex);		// テクスチャ数

		m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumTex];	// テクスチャ数分メモリ確保

		for (nCntTexture = 0; nCntTexture < m_nNumTex; nCntTexture++)
		{
			// テクスチャのパスを記憶
			fscanf(pFile, "%s\n", &aTexName[0]);

			if (&m_pTexture[nCntTexture] != NULL)
			{// テクスチャの生成
				D3DXCreateTextureFromFile(pDevice, &aTexName[0], &m_pTexture[nCntTexture]);
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開けない
		MessageBox(0, "ファイルがありません！", "field_texture.txt", MB_OK);
	}

	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CMeshField::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < m_nNumTex; nCntTexture++)
	{
		if (NULL != m_pTexture[nCntTexture])
		{// テクスチャの破棄
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}

	// メモリの開放
	delete m_pTexture;

	// テクスチャ数の初期化
	m_nNumTex = 0;
}

//=========================================
// コンストラクタ
//=========================================
CMeshField::CMeshField()
{
	m_nWire = 0;
	m_nType = 0;
}

//=========================================
// デストラクタ
//=========================================
CMeshField::~CMeshField()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshField::Init(void)
{
	CScene3DIndex::SetObjType(CScene::OBJTYPE_FIELD);
	CScene3DIndex::Init();
	CScene3DIndex::MakeVertex();
	CScene3DIndex::BindTexture(m_pTexture[0]);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMeshField::Uninit(void)
{
	CScene3DIndex::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMeshField::Update(void)
{
	CScene3DIndex::Update();

	// キーボードの入力を取得
	CInputKeyboard *pKeyboard = CManager::GetInput();

	if (pKeyboard->GetTrigger(DIK_F8) == true)
	{// ワイヤーフレーム表示
		m_nWire = (2 + m_nWire) % 4;
	}

	if (pKeyboard->GetTrigger(DIK_F7) == true)
	{// テクスチャ表示
		m_nType = (1 + m_nType) % (m_nNumTex + 1);

		if (m_nType < m_nNumTex)
		{// テクスチャを指定
			CScene3DIndex::BindTexture(m_pTexture[m_nType]);
		}
		else
		{// テクスチャを張らない
			CScene3DIndex::BindTexture(NULL);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshField::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ワイヤーフレームを表示
	pDevice->SetRenderState(D3DRS_FILLMODE, m_nWire);

	CScene3DIndex::Draw();

	// ワイヤーフレームを非表示
	pDevice->SetRenderState(D3DRS_FILLMODE, 0);
}
