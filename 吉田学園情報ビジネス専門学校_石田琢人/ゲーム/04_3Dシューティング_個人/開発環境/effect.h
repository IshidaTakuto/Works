//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "scene3DBill.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EFFECT_PRIORITY		(4)		// エフェクトの優先順位
#define EFFECT_NUMTEXTURE	(2)		// テクスチャの数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEffect : public CScene3DBill
{
public:
	typedef enum
	{
		TYPE_NORMAL,	// 通常
		TYPE_RIPPLES,	// 波紋
		TYPE_BIIM,		// ビーム
		TYPE_MAX
	}TYPE;

	CEffect(int nPriority = EFFECT_PRIORITY);
	virtual ~CEffect();
	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static CEffect* Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRange, int nLife, TYPE type = TYPE_NORMAL);

	static HRESULT Load(void);				// テクスチャを読み込む
	static void Unload(void);				// 読み込んだテクスチャを解放

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[EFFECT_NUMTEXTURE];	// 共有テクスチャへのポインタ
	TYPE m_type;							// 種類
};

#endif