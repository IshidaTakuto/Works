//=============================================================================
//
// 3Dモデルオブジェクト処理 [modelObject.h]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "modelObject.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "meshField.h"
#include "enemy.h"
#include "setPlayer.h"

#include <stdio.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_SAVE_FILENAME	"data/TEXT/MODEL/model_object_save.txt"	// セーブ用テキストファイル

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9	*CModelObjectManager::m_pTexture[TYPE_MAX] = {};	// テクスチャ情報
LPD3DXMESH			*CModelObjectManager::m_pMesh[TYPE_MAX] = {};		// メッシュ情報
LPD3DXBUFFER		*CModelObjectManager::m_pBuffMat[TYPE_MAX] = {};	// マテリアル情報
DWORD				*CModelObjectManager::m_pNumMat[TYPE_MAX] = {};		// マテリアル数
int					CModelObjectManager::m_nNumType[TYPE_MAX] = {};		// モデルの種類
int					CModelObjectManager::m_nNumAllModel = 0;			// モデルの総数
char				**CModelObjectManager::m_apModelName[TYPE_MAX] = {};		// モデルファイル名
bool				CModelObjectManager::m_bSave = false;

//=============================================================================
// モデル情報の読み込み
//=============================================================================
HRESULT CModelObjectManager::Load(void)
{
	// 配置物の削除
	CScene::LoadDelete();

	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;		// ファイルのポインタ変数
	char aStr[128];		// 取得する文字列
	int nCntModel[TYPE_MAX] = {};	// モデルのカウント
	char *pFileName = CManager::GetSetFileName(0);

	// ファイルを開く
	pFile = fopen(pFileName, "r");

	if (NULL != pFile)
	{// ファイルがあった
		while (strcmp(aStr, "END_SCRIPT") != 0)
		{// 終了メッセージまでループ
			fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(aStr, "TYPE_MODEL") == 0)
			{// オブジェクトの種類がモデルのとき
				fscanf(pFile, " = %d", &m_nNumType[TYPE_MODEL]);

				// モデルファイル名保存変数のメモリ確保
				m_apModelName[TYPE_MODEL] = new char*[m_nNumType[TYPE_MODEL]];

				// モデル情報のメモリ確保
				NumObjectType(TYPE_MODEL);
			}
			if (strcmp(aStr, "TYPE_START") == 0)
			{// オブジェクトの種類がスタート地点のとき
				fscanf(pFile, " = %d", &m_nNumType[TYPE_START]);

				// モデルファイル名保存変数のメモリ確保
				m_apModelName[TYPE_START] = new char*[m_nNumType[TYPE_START]];

				// モデル情報のメモリ確保
				NumObjectType(TYPE_START);
			}
			if (strcmp(aStr, "TYPE_ENEMY") == 0)
			{// オブジェクトの種類が敵のとき
				fscanf(pFile, " = %d", &m_nNumType[TYPE_ENEMY]);

				// モデルファイル名保存変数のメモリ確保
				m_apModelName[TYPE_ENEMY] = new char*[m_nNumType[TYPE_ENEMY]];

				// モデル情報のメモリ確保
				NumObjectType(TYPE_ENEMY);
			}
			if (strcmp(aStr, "TYPE_ITEM") == 0)
			{// オブジェクトの種類がアイテムのとき
				fscanf(pFile, " = %d", &m_nNumType[TYPE_ITEM]);

				// モデルファイル名保存変数のメモリ確保
				m_apModelName[TYPE_ITEM] = new char*[m_nNumType[TYPE_ITEM]];

				// モデル情報のメモリ確保
				NumObjectType(TYPE_ITEM);
			}
			if (strcmp(aStr, "LOAD") == 0)
			{// 読み込むモデル名
				while (strcmp(aStr, "END_LOAD") != 0)
				{// 終了メッセージまでループ
					fscanf(pFile, "%s", &aStr[0]);

					if (strcmp(aStr, "OBJECT_TYPE") == 0)
					{// 読み込むオブジェクトの種類
						fscanf(pFile, " = %s", &aStr[0]);
						TYPE type = TYPE_MODEL;

						// 各オブジェクトの数をカウント
						if (strcmp(aStr, "TYPE_MODEL") == 0) { type = TYPE_MODEL; }
						if (strcmp(aStr, "TYPE_START") == 0) { type = TYPE_START; }
						if (strcmp(aStr, "TYPE_ENEMY") == 0) { type = TYPE_ENEMY; }
						if (strcmp(aStr, "TYPE_ITEM") == 0) { type = TYPE_ITEM; }

						while (1)
						{// ループさせる
							fscanf(pFile, "%s", &aStr[0]);

							if (strcmp(aStr, "MODEL_FILENAME") == 0)
							{// 読み込むモデルファイル
								fscanf(pFile, " = %s", &aStr[0]);

								// 必要な分メモリ確保
								m_apModelName[type][nCntModel[type]] = new char[strlen(&aStr[0])];
								strcpy(m_apModelName[type][nCntModel[type]], aStr);

								if (type != TYPE_ENEMY)
								{// モデル情報の作成
									CreateModelInfo(type, pDevice, &aStr[0], nCntModel[type]);
								}

								nCntModel[type]++;	// オブジェクト数のカウント
								break;
							}
						}
					}
				}

				CEnemy::Load();		// 敵のモデル情報の作成
			}
			if (strcmp(aStr, "MODELSET") == 0)
			{// モデル配置情報
				CModelObjectManager::TYPE type = CModelObjectManager::TYPE_MODEL;
				int nType = 0;	// モデル番号
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
				D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き

				while (strcmp(aStr, "END_MODELSET") != 0)
				{// 終了メッセージまでループ
					fscanf(pFile, "%s", &aStr[0]);

					if (strcmp(aStr, "OBJ") == 0)
					{// オブジェクトの種類
						fscanf(pFile, " = %s", &aStr[0]);

						if (strcmp(aStr, "TYPE_MODEL") == 0)
						{// 障害物
							type = TYPE_MODEL;
						}
						else if (strcmp(aStr, "TYPE_START") == 0)
						{// スタート地点
							type = TYPE_START;
						}
						else if (strcmp(aStr, "TYPE_ENEMY") == 0)
						{// 敵
							type = TYPE_ENEMY;
						}
						else if (strcmp(aStr, "TYPE_ITEM") == 0)
						{// アイテム
							type = TYPE_ITEM;
						}
					}
					if (strcmp(aStr, "TYPE") == 0)
					{// モデル番号
						fscanf(pFile, " = %d", &nType);
					}
					if (strcmp(aStr, "POS") == 0)
					{// 位置
						fscanf(pFile, " = %f %f %f", &pos.x, &pos.y, &pos.z);
					}
					if (strcmp(aStr, "ROT") == 0)
					{// 向き
						fscanf(pFile, " = %f %f %f", &rot.x, &rot.y, &rot.z);
					}
				}

				if (type != TYPE_ENEMY)
				{// オブジェクトの生成
					CModelObject::Create(pos, rot, type, nType);
				}
				else if (type == TYPE_ENEMY)
				{// 敵の生成
					CEnemy::Create(pos, nType);
				}
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開けない
		MessageBox(0, "ファイルがありません！", "field.ini", MB_OK);
	}

	return S_OK;
}

//=============================================================================
// オブジェクトの種類ごとの総数分メモリ確保
//=============================================================================
void CModelObjectManager::NumObjectType(TYPE type)
{
	m_pTexture[type] = new LPDIRECT3DTEXTURE9[m_nNumType[type]];
	for (int nCnt = 0; nCnt < m_nNumType[type]; nCnt++)
	{
		m_pTexture[type][nCnt] = NULL;
	}

	m_pMesh[type] = new LPD3DXMESH[m_nNumType[type]];
	for (int nCnt = 0; nCnt < m_nNumType[type]; nCnt++)
	{
		m_pMesh[type][nCnt] = NULL;
	}

	m_pBuffMat[type] = new LPD3DXBUFFER[m_nNumType[type]];
	for (int nCnt = 0; nCnt < m_nNumType[type]; nCnt++)
	{
		m_pBuffMat[type][nCnt] = NULL;
	}

	m_pNumMat[type] = new DWORD[m_nNumType[type]];
	for (int nCnt = 0; nCnt < m_nNumType[type]; nCnt++)
	{
		m_pNumMat[type][nCnt] = NULL;
	}

}

//=============================================================================
// モデル情報の作成
//=============================================================================
void CModelObjectManager::CreateModelInfo(TYPE type, LPDIRECT3DDEVICE9 pDevice, char *pModelFileName, int nCntModel)
{
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	// Xファイルの読み込み
	D3DXLoadMeshFromX(pModelFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat[type][nCntModel],
		NULL,
		&m_pNumMat[type][nCntModel],
		&m_pMesh[type][nCntModel]);

	// テクスチャの作成
	pMat = (D3DXMATERIAL*)m_pBuffMat[type][nCntModel]->GetBufferPointer();
	D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_pTexture[type][nCntModel]);
}

//=============================================================================
// 読み込んだ情報の解放
//=============================================================================
void CModelObjectManager::Unload(void)
{
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// オブジェクトの種類分ループ
		for (int nCntNumType = 0; nCntNumType < m_nNumType[nCntType]; nCntNumType++)
		{
			if (NULL != m_pTexture[nCntType][nCntNumType])
			{// テクスチャの破棄
				m_pTexture[nCntType][nCntNumType]->Release();
				m_pTexture[nCntType][nCntNumType] = NULL;
			}

			if (NULL != m_pMesh[nCntType][nCntNumType])
			{// メッシュ情報の破棄
				m_pMesh[nCntType][nCntNumType]->Release();
				m_pMesh[nCntType][nCntNumType] = NULL;
			}

			if (NULL != m_pBuffMat[nCntType][nCntNumType])
			{// マテリアル情報の破棄
				m_pBuffMat[nCntType][nCntNumType]->Release();
				m_pBuffMat[nCntType][nCntNumType] = NULL;
			}

			if (NULL != m_pNumMat[nCntType][nCntNumType])
			{// マテリアルの数を破棄
				m_pNumMat[nCntType][nCntNumType] = NULL;
			}
		}
	}

	// モデル数の初期化
	m_nNumAllModel = 0;
}

//=============================================================================
// 生成処理
//=============================================================================
CModelObjectManager *CModelObjectManager::Create(void)
{
	CModelObjectManager *pModelObjectManager = NULL;

	// メモリ確保
	pModelObjectManager = new CModelObjectManager;

	if (pModelObjectManager != NULL)
	{// メモリ確保成功
		pModelObjectManager->Init();
	}

	return pModelObjectManager;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CModelObjectManager::CModelObjectManager(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CModelObjectManager::~CModelObjectManager()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModelObjectManager::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModelObjectManager::Uninit(void)
{
	// 自分を破棄
	CScene::Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CModelObjectManager::Update(void)
{
	// キーボードの入力を取得
	CInputKeyboard *pKeyboard = CManager::GetInput();

	if (m_bSave)
	{// セーブする
		FILE *pFile;

		// ファイルを開く
		pFile = fopen(MODEL_SAVE_FILENAME, "w");

		if (NULL != pFile)
		{// ファイルがあった
			fprintf(pFile, "#********************************************************************\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#    モデル配置スクリプトファイル[model_object_save.txt]\n");
			fprintf(pFile, "#    Auther : TAKUTO ISHIDA\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#********************************************************************\n");

			fprintf(pFile, "SCRIPT\n\n");

			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "# 表示するモデルの配置情報\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			// モデルオブジェクトを探す
			CScene *pScene = CScene::GetSceneTop(MODEL_PRIORITY);

			while (pScene != NULL)
			{// NULLになるまでループ
				CScene *pSceneNext = pScene->GetSceneNext();
				CScene::OBJTYPE objType = pScene->GetObjType();

				if (CScene::OBJTYPE_SETOBJ == objType)
				{// モデルの場合
					CModelObject *pModel = (CModelObject*)pScene;
					char *pType = pModel->GetTypeName();
					int nType = pModel->GetType();			// 種類の取得
					D3DXVECTOR3 pos = pModel->GetPos();		// 位置の取得
					D3DXVECTOR3 rot = pModel->GetRot();		// 向きの取得

					fprintf(pFile, "MODELSET\n");
					fprintf(pFile, "	OBJ = %s		# オブジェクトの種類\n", pType);
					fprintf(pFile, "	TYPE = %d				# 種類\n", nType);
					fprintf(pFile, "	POS = %.1f %.1f %.1f		# 位置\n", pos.x, pos.y, pos.z);
					fprintf(pFile, "	ROT = %.1f %.1f %.1f		# 向き\n", rot.x, rot.y, rot.z);
					fprintf(pFile, "END_MODELSET\n\n");
				}
				else if (CScene::OBJTYPE_ENEMY == objType)
				{// 敵の場合
					CEnemy *pEnemy = (CEnemy*)pScene;
					int nType = pEnemy->GetType();			// 種類の取得
					D3DXVECTOR3 pos = pEnemy->GetPos();		// 位置の取得
					D3DXVECTOR3 rot = pEnemy->GetRot();		// 向きの取得

					fprintf(pFile, "MODELSET\n");
					fprintf(pFile, "	OBJ = TYPE_ENEMY		# オブジェクトの種類\n");
					fprintf(pFile, "	TYPE = %d				# 種類\n", nType);
					fprintf(pFile, "	POS = %.1f %.1f %.1f		# 位置\n", pos.x, pos.y, pos.z);
					fprintf(pFile, "	ROT = %.1f %.1f %.1f		# 向き\n", rot.x, rot.y, rot.z);
					fprintf(pFile, "END_MODELSET\n\n");
				}

				// 次のオブジェクトを見る
				pScene = pSceneNext;
			}

			fprintf(pFile, "\nEND_SCRIPT		＃ 絶対に消さない！");

			// ファイルを閉じる
			fclose(pFile);

			MessageBox(NULL, "セーブしました。", "data/TEXT/MODEL/model_object_save.txt", MB_OK);
		}
		else
		{// ファイルが開けない
			MessageBox(0, "ファイルがありません！\nファイルを作成します。", "field.txt", MB_OK);
		}

		m_bSave = false;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CModelObjectManager::Draw(void)
{
}

/*==================================================================================*/
/*																					*/
/*								モデルオブジェクトクラス							*/
/*																					*/
/*==================================================================================*/

//=============================================================================
// 生成処理
//=============================================================================
CModelObject* CModelObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelObjectManager::TYPE type, int nType)
{
	CModelObject *pObject = NULL;

	pObject = new CModelObject;	// メモリを確保

	if (pObject != NULL)
	{// メモリ確保成功
		pObject->Init(pos, rot, type, nType);

		// モデル数の設定
		CModelObjectManager::AddNumModel(1);
	}

	return pObject;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CModelObject::CModelObject(int nPriority, CScene::OBJTYPE objType) : CSceneX(nPriority, objType)
{
	m_nType = 0;
	m_pType = NULL;
	m_bRange = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CModelObject::~CModelObject()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModelObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelObjectManager::TYPE type, int nType)
{
	// モデル情報の受け渡し
	CSceneX::BindInfo(CModelObjectManager::GetTexture(type, nType), CModelObjectManager::GetMesh(type, nType), CModelObjectManager::GetBuffMat(type, nType), CModelObjectManager::GetNumMat(type, nType));

	// 初期化処理
	CSceneX::Init();

	// 位置の設定
	CSceneX::SetPos(pos);

	// 向きの設定
	CSceneX::SetRot(rot);

	// 種類の保存
	m_nType = nType;

	if (type == CModelObjectManager::TYPE_MODEL)
	{// モデル
		m_pType = new char[strlen("TYPE_MODEL")];
		strcpy(m_pType, "TYPE_MODEL");
	}
	else if (type == CModelObjectManager::TYPE_START)
	{// スタート地点
		m_pType = new char[strlen("TYPE_START")];
		strcpy(m_pType, "TYPE_START");
	}
	else if (type == CModelObjectManager::TYPE_ENEMY)
	{// 敵
		m_pType = new char[strlen("TYPE_ENEMY")];
		strcpy(m_pType, "TYPE_ENEMY");
	}
	else if (type == CModelObjectManager::TYPE_ITEM)
	{// アイテム
		m_pType = new char[strlen("TYPE_ITEM")];
		strcpy(m_pType, "TYPE_ITEM");
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModelObject::Uninit(void)
{
	// 自分を破棄
	CSceneX::Uninit();

	// モデル数の設定
	CModelObjectManager::AddNumModel(-1);
}

//=============================================================================
// 更新処理
//=============================================================================
void CModelObject::Update(void)
{
	D3DXVECTOR3 pos = CSceneX::GetPos();
	pos.y = CManager::GetMeshField()->CMeshField::GetHeight(pos);
	CSceneX::SetPos(pos);

	// 地面オブジェクトを探す
	CScene *pScene = CScene::GetSceneTop(SETPLAYER_PRIORITY);

	while (pScene != NULL)
	{// NULLになるまでループ
		CScene *pSceneNext = pScene->GetSceneNext();
		CScene::OBJTYPE objType = pScene->GetObjType();

		if (objType == CScene::OBJTYPE_SETPLAYER)
		{// 配置用プレイヤーだったとき
			CSetPlayer *pSetPlayer = (CSetPlayer*)pScene;	// 地面のポインタを取得

			m_bRange = pSetPlayer->Range(pos);			// 地面の高さの取得
		}

		// 次のオブジェクトを見る
		pScene = pSceneNext;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CModelObject::Draw(void)
{
	if (m_bRange)
	{// 範囲内のとき色を加算
		CSceneX::AddColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, -0.3f));
	}

	// 描画処理
	CSceneX::Draw();

	if (m_bRange)
	{// 色を戻す
		CSceneX::AddColor(D3DXCOLOR(-1.0f, 0.0f, 0.0f, 0.3f));
	}
}

//=============================================================================
// 範囲内の敵を削除
//=============================================================================
void CModelObject::Range(void)
{
	if (m_bRange)
	{// 範囲内のとき削除
		Uninit();
	}
}