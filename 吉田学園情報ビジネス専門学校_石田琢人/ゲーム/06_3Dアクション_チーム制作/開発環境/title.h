//*****************************************************************************
//
//     タイトルの処理[title.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CLogo;
class CLogoPress;
class CMap;
//*****************************************************************************
//    タイトルクラスの定義
//*****************************************************************************
class CTitle : public CScene
{
public:     // 誰でもアクセス可能
	CTitle(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_TITLE);
	~CTitle();

	static CTitle *Create(void);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	static LPDIRECT3DTEXTURE9 m_apTexture;	// テクスチャへのポインタ
	CLogo      *m_pLogo;        // タイトルロゴクラスへのポインタ
	CLogoPress *m_pLogoPress;   // PRESSENTERクラスへのポインタ
	int        m_nCounter;      // 遷移を管理するカウンター
	CMap	   *m_pMap;			// マップクラスへのポインタ
};

#endif