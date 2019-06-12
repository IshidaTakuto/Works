//=============================================================================
//
// 残機処理 [stock.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _STOCK_H_
#define _STOCK_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_STOCK	(4)		// 残機の最大数
#define MAX_STOCK	(1)		// 表示するポリゴンの最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPolygon;
class CNumber;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CStock : public CScene
{
public:
	CStock(int nPriority = 6);
	~CStock();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CStock* Create(void);

	static const void AddSubtract(int nValue);
	static int GetStock(void) { return m_nStock; };

private:
	static CPolygon *m_pPolygon[MAX_STOCK];		// ポリゴンクラスのポインタ変数
	static CNumber *m_pNumber;					// ナンバークラスのポインタ変数
	static int m_nStock;						// 現在の残機
};


#endif
