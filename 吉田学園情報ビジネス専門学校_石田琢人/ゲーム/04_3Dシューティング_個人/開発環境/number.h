//=============================================================================
//
// 数字処理 [number.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CNumber
{
public:
	CNumber();
	~CNumber();
	HRESULT Init(D3DXVECTOR3 pos, float fLength);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CNumber* Create(D3DXVECTOR3 pos, float fLength);

	static HRESULT Load(void);				// テクスチャを読み込む
	static void Unload(void);				// 読み込んだテクスチャを解放

	const void SetNumber(int nNumber);

	const void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; };

	const void SetColor(D3DXCOLOR col);
	const void SetLength(float fLength);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// 共有テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ

	D3DXVECTOR3 m_pos;			// 座標
	float m_fLength;			// ポリゴンの長さ
	int m_nCountAnim;			// アニメーションのカウンタ
	int m_nPatternAnim;			// アニメーションのパターン数
};

#endif