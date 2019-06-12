//=============================================================================
//
// リザルト処理 [result.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULT_MAX_TEXTURE	(2)		// テクスチャの数
#define RESULT_NUM_BG		(2)		// 背景の数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;
class CScene2D;
class CLogo;
class CLogoPressButton;
class CBg;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CResult : public CScene
{
public:
	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetTime(int nTime) { m_nTime = nTime; };

private:
	static int	m_nTime;			// 時間
	int			m_nTimeDigit;		// 時間の桁
	int			m_nCntResult;		// リザルトの時間
	int			m_nFlash;			// 点滅させる速度

	LPDIRECT3DTEXTURE9 m_pTexture[RESULT_MAX_TEXTURE];	// テクスチャへのポインタ変数

	CNumber				**m_pNumber;			// 数字クラスのポインタ変数
	CScene2D			*m_pPolygon;			// 2Dポリゴンクラスのポインタ変数
	CLogo				*m_pLogo;				// ロゴクラスのポインタ変数
	CLogoPressButton	*m_pPressButton;		// 入力待ち表示クラスのポインタ変数
	CBg					*m_pBg[RESULT_NUM_BG];	// 背景のポインタ変数
	CBg					*m_pMountain;			// 背景の山
};

#endif