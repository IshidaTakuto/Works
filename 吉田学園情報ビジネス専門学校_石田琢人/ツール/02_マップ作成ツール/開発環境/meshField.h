//=============================================================================
//
// メッシュフィールド処理 [meshField.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "scene3DIndex.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHFIELD_PRIORITY	(3)		// 地面の処理優先順位

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMeshField : public CScene3DIndex
{
public:
	CMeshField();
	~CMeshField();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshField* Create(void);

	D3DXVECTOR3 GetNor(D3DXVECTOR3 pos);

	float GetAngle(D3DXVECTOR3 pos, float fAngle);
	float GetSlipAngle(D3DXVECTOR3 pos);

	void RangeColor(D3DXVECTOR3 pos, float fRange);

private:
	int m_nType;			// テクスチャの種類
	int m_nWire;			// ワイヤーフレームを表示するか
};

#endif
