//*****************************************************************************
//
//     ロゴの処理[logo.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _LOGO_H_
#define _LOGO_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene2DRot.h"

//*****************************************************************************
//     ロゴクラスの定義
//*****************************************************************************
class CLogo : public CScene2DRot
{
public:     // 誰でもアクセス可能
	CLogo(int nPriority = 3,OBJTYPE objType = OBJTYPE_LOGO);
	~CLogo();
	static CLogo *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, int nPriority);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:    // このクラスだけがアクセス可能
};

//*****************************************************************************
//     セレクトロゴクラスの定義
//*****************************************************************************
class CLogoSelect : public CScene2DRot
{
public:    // 誰でもアクセス可能
	//------------------------
	//  セレクト項目の種類
	//------------------------
	typedef enum
	{
		TYPE_GAME = 0,  // ゲーム画面
		TYPE_RANKING,   // ランキング画面
		TYPE_TITLE,     // タイトル画面
		TYPE_MAX
	}TYPE;

	//------------------------
	//  セレクト項目の状態
	//------------------------
	typedef enum
	{
		STATE_NONE = 0,  // 選択されていない状態
		STATE_SELECT,    // 選択されている状態
		STATE_PUSH,      // 決定が押された状態
		STATE_MAX
	}STATE;

	CLogoSelect(int nPriority = 3);
	~CLogoSelect();

	static CLogoSelect *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state, int nPriority);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetType(TYPE type);
	void SetState(STATE state);

	void ResetTexU(void);

	TYPE GetType(void);
	STATE GetState(void);

private:   // このクラスだけがアクセス可能
	static LPDIRECT3DTEXTURE9  m_apTexture[TYPE_MAX];  // テクスチャへのポインタ
	static float               m_fColTrans;            // 色の変化に使用
	TYPE  m_Type;          // 種類
	STATE m_State;         // 状態
	int   m_nPatternAnimU; // 現在のテクスチャU座標
	int   m_nCounterAnim;  // アニメーションカウンター
};

//*****************************************************************************
//     ポーズロゴクラスの定義
//*****************************************************************************
class CLogoPause : public CScene2DRot
{
public:   // 誰でもアクセス可能
	//------------------------
	//  ポーズ項目の種類
	//------------------------
	typedef enum
	{
		TYPE_CONTINUE = 0,  // 再開
		TYPE_RETRY,         // やり直し
		TYPE_QUIT,          // 終了
		TYPE_MAX
	}TYPE;

	//------------------------
	//  ポーズ項目の状態
	//------------------------
	typedef enum
	{
		STATE_NONE = 0,  // 選択されていない状態
		STATE_SELECT,    // 選択されている状態
		STATE_PUSH,      // 決定が押された状態
		STATE_MAX
	}STATE;

	CLogoPause(int nPriority = 3);
	~CLogoPause();

	static CLogoPause *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state, int nPriority);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetType(TYPE type);
	void SetState(STATE state);

	TYPE GetType(void);
	STATE GetState(void);

private:   // このクラスだけがアクセス可能
	static LPDIRECT3DTEXTURE9  m_apTexture[TYPE_MAX];  // テクスチャへのポインタ
	static float               m_fColTrans;            // 色の変化に使用
	TYPE  m_Type;      // 種類
	STATE m_State;     // 状態
};

//*****************************************************************************
//     プレスロゴクラスの定義
//*****************************************************************************
class CLogoPress : public CScene2DRot
{
public:   // 誰でもアクセス可能
	//------------------------
    //  PRESSの状態
	//------------------------
	typedef enum
	{
		STATE_NONE = 0,  // なにもされていない状態
		STATE_PUSH,      // 決定が押された状態
		STATE_MAX
	}STATE;

	CLogoPress(int nPriority = 3);
	~CLogoPress();

	static CLogoPress *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, int nPriority);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetScale(float fScale);
	void SetState(STATE state);

	float GetScale(void);
	STATE GetState(void);

private:   // このクラスだけがアクセス可能
	static LPDIRECT3DTEXTURE9  m_pTexture;  // テクスチャへのポインタ
	STATE m_State;     // 状態
	int   m_nCounter;  // 描画を管理するカウンター
};

#endif