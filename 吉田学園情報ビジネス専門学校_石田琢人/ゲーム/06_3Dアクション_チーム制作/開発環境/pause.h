//*****************************************************************************
//
//     ポーズの処理[pause.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CScene2DSquare;
class CLogoPause;

//*****************************************************************************
//    ポーズクラスの定義
//*****************************************************************************
class CPause : CScene
{
public:    // 誰でもアクセス可能
	//------------------------
	//  ポーズ項目の種類
	//------------------------
	typedef enum
	{
		SELECT_CONTINUE = 0, // 再開
		SELECT_RETRY,        // やり直し
		SELECT_QUIT,         // 終了
		SELECT_MAX
	}SELECT;

	CPause(int nPriority = 0, OBJTYPE objType = OBJTYPE_PAUSE);
	~CPause();

	static CPause *Create(void);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetSelect(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static LPDIRECT3DTEXTURE9 m_pTexture;                 // テクスチャへのポインタ
	CLogoPause              *m_apLogoPause[SELECT_MAX];   // ポーズ項目の種類
	CScene2DSquare          *m_pBlackPolygon;             // ポーズに使用する黒いポリゴン
	CScene2DSquare          *m_pPauseBg;                  // ポーズの背景
	int                     m_nSelect;                    // 現在選択している項目
	int                     m_nCounter;                   // 描画を管理するカウンター
};

#endif