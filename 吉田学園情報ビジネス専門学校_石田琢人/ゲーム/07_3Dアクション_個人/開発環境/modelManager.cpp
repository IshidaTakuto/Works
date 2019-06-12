//=============================================================================
//
// モデル処理 [model.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "modelManager.h"
#include "manager.h"
#include "renderer.h"
#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_FILE "data/TEXT/PLAYER/model.txt"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int	CModelManager::m_NumModel = 0;		// モデル数
char **CModelManager::m_pStr = NULL;

//=========================================
// モデルの読み込み
//=========================================
HRESULT CModelManager::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;
	int nCntModel = 0;

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
						fscanf(pFile, " = %d", &m_NumModel);
						m_pStr = new char*[m_NumModel];

						//m_pMesh = new LPD3DXMESH[m_NumModel];
						//m_pBuffMat = new LPD3DXBUFFER[m_NumModel];
						//m_nNumMat = new DWORD[m_NumModel];
					}
					else if (strcmp(aStr, "MODEL_FILENAME") == 0)
					{// Xファイルの読み込み
						m_pStr[nCntModel] = new char[64];
						fscanf(pFile, " = %s", &m_pStr[nCntModel][0]);

						//if (nCntModel < m_NumModel)
						//{// メモリを確保した個数分のみ
						//	D3DXLoadMeshFromX(aStr,
						//		D3DXMESH_SYSTEMMEM,
						//		pDevice,
						//		NULL,
						//		&m_pBuffMat[nCntModel],
						//		NULL,
						//		&m_nNumMat[nCntModel],
						//		&m_pMesh[nCntModel]);
						//}
						nCntModel++;	// カウンタを進める
					}
				}
				break;
			}
		}
		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// ファイルがないとき
		MessageBox(0, "ない", "model.txt", MB_OK);
	}
	return S_OK;
}

//=========================================
// 読み込んだテクスチャの解放
//=========================================
void CModelManager::Unload(void)
{
	if (m_pStr != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_NumModel; nCntModel++)
		{
			if (m_pStr[nCntModel] != NULL)
			{// 文字の破棄
				delete m_pStr[nCntModel];
				m_pStr[nCntModel] = NULL;
			}
		}

		delete[] m_pStr;
		m_pStr = NULL;
	}
}

//=========================================
// コンストラクタ
//=========================================
CModelManager::CModelManager()
{
}

//=========================================
// デストラクタ
//=========================================
CModelManager::~CModelManager()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModelManager::Init(void)
{

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModelManager::Uninit(void)
{
	CScene::Release();
}

//=========================================
// 更新処理
//=========================================
void CModelManager::Update(void)
{
}

//=========================================
// 描画処理
//=========================================
void CModelManager::Draw(void)
{
}

//=============================================================================
// モデル情報の取得
//=============================================================================
void CModelManager::GetInfo(int nIdx, LPD3DXMESH &pMesh, LPD3DXBUFFER &pBuffMat, DWORD *nNumMat)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXLoadMeshFromX(m_pStr[nIdx],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pBuffMat,
		NULL,
		nNumMat,
		&pMesh);

	//pMesh = m_pMesh[nIdx];
	//pBuffMat = m_pBuffMat[nIdx];
	//*nNumMat = m_nNumMat[nIdx];
}