//=============================================================================
//
// 文字表示処理 [string.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _STRING_H_
#define _STRING_H_

#include "main.h"
#include <string.h>		// 文字列に必要

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_FONT	(2)		// 文字列フォントの数
#define MAX_STRING	(32)	// 文字列の最大数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CString
{
public:
	CString();
	~CString();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);	// 文字フォントを読み込む
	static void Unload(void);	// 文字フォントを開放

	static CString *Create(void);		// 描画処理で呼ぶ
	bool Set(char *str, RECT rect, D3DXCOLOR col, int nDispTime, int nFont = 0);

private:
	static LPD3DXFONT	m_pFont[NUM_FONT];		// フォントへのポインタ
	int m_nCntString;							// 文字を表示するまでの時間
	char *m_pStringOld;							// 文字列を記憶
};

#endif