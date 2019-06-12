//*****************************************************************************
//
//     数字ポリゴンの処理[number.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene2DSquare.h"

//*****************************************************************************
//     数字ポリゴンクラスの定義
//*****************************************************************************
class CNumber : public CScene2DSquare
{
public:    // 誰でもアクセス可能
	CNumber(int nPriority = 3,OBJTYPE objType = OBJTYPE_NUMBER);
	~CNumber();

	static CNumber *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nNumber, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nNumber);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetNumber(int nNumber);

private:   // このクラスだけがアクセス可能
	static LPDIRECT3DTEXTURE9      m_pTexture;  // テクスチャへのポインタ
	int m_nNumber;   // 表示する数字
};
#endif