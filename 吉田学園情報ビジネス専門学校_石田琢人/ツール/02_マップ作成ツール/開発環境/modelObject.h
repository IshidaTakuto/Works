//=============================================================================
//
// 3Dモデルオブジェクト処理 [modelObject.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MODELOBJECT_H_
#define _MODELOBJECT_H_

#include "scene.h"
#include "sceneX.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_PRIORITY	(4)

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CModelObjectManager : public CScene
{
public:
	typedef enum
	{
		TYPE_MODEL = 0,		// モデル
		TYPE_START,			// スタート地点
		TYPE_ENEMY,			// 敵
		TYPE_ITEM,			// アイテム
		TYPE_MAX
	}TYPE;

	CModelObjectManager(int nPriority = 0, CScene::OBJTYPE objType = CScene::OBJTYPE_OBJECTMANAGER);
	~CModelObjectManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);				// テクスチャを読み込む
	static void Unload(void);				// 読み込んだテクスチャを解放

	static CModelObjectManager *Create(void);

	static LPDIRECT3DTEXTURE9	GetTexture(TYPE type, int nType) { return m_pTexture[type][nType]; };		// テクスチャ情報の取得
	static LPD3DXMESH			GetMesh(TYPE type, int nType) { return m_pMesh[type][nType]; };				// メッシュ情報の取得
	static LPD3DXBUFFER			GetBuffMat(TYPE type, int nType) { return m_pBuffMat[type][nType]; };		// マテリアル情報の取得
	static DWORD				GetNumMat(TYPE type, int nType) { return m_pNumMat[type][nType]; };			// マテリアル数の取得

	static int					GetNumModel(void) { return m_nNumAllModel; };					// モデル数の取得
	static void					SetNumModel(int nNumModel) { m_nNumAllModel = nNumModel; };	// モデル数の設定
	static void					AddNumModel(int nValue) { m_nNumAllModel += nValue; };			// モデル数の加算

	static int					GetNumType(TYPE type) { return m_nNumType[type]; };					// 種類の数の取得

	static char					**&GetModelFileName(TYPE type) { return m_apModelName[type]; };

	static void Save(void) { m_bSave = m_bSave ? false : true; };	// 情報の保存

protected:

private:
	static void NumObjectType(TYPE type);					// モデル情報を作成するためのメモリ確保
	static void CreateModelInfo(TYPE type, LPDIRECT3DDEVICE9 pDevice, char *pModelFileName, int nCntModel);	// モデル情報の作成

	static LPDIRECT3DTEXTURE9	*m_pTexture[TYPE_MAX];		// テクスチャへのポインタ
	static LPD3DXMESH			*m_pMesh[TYPE_MAX];			// メッシュ情報へのポインタ
	static LPD3DXBUFFER			*m_pBuffMat[TYPE_MAX];		// マテリアル情報へのポインタ
	static DWORD				*m_pNumMat[TYPE_MAX];		// マテリアル情報の数

	static int					m_nNumType[TYPE_MAX];		// 設置モデルの種類の数
	static int					m_nNumAllModel;				// モデルの合計数
	static char					**m_apModelName[TYPE_MAX];	// モデルの名前
	static bool					m_bSave;
};

/*========================*/
/*  障害物クラス  */
/*========================*/
class CModelObject : public CSceneX
{
public:
	CModelObject(int nPriority = MODEL_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_SETOBJ);
	~CModelObject();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelObjectManager::TYPE type, int nObject);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CModelObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModelObjectManager::TYPE type, int nObject);

	int GetType(void) { return m_nType; };
	char *GetTypeName(void) { return m_pType; };

	void Range(void);

protected:

private:
	int m_nType;
	char *m_pType;
	bool m_bRange;		// 範囲内かどうか
};

#endif