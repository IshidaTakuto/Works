//=============================================================================
//
// 吹き出しの処理 [balloon.h]
// Author : Atsushi Yamashita
//
//=============================================================================
#ifndef _BALLOON_H_
#define _BALLOON_H_

#include "main.h"
#include "scene2DSquare.h"
class CScene2DSquare;

#define MAX_BALLOON_TEXTURE (5)
#define BALLOON_PRIORITY	(4)

class CBalloon : public CScene2DSquare
{
public:
	CBalloon(int nPriority = BALLOON_PRIORITY, OBJTYPE objType = OBJTYPE_BALLOON);
	~CBalloon();

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBalloon *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight,int nTexNumber);
	static HRESULT Load(void);										//テクスチャ読み込み
	static void Unload(void);

	 void SetType(int nType);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_BALLOON_TEXTURE];

	D3DXVECTOR3 m_pos;			// 頂点座標
	D3DXVECTOR3 m_move;			//法線ベクトル
	D3DXCOLOR   m_col;
	float m_fCol;
	D3DXMATRIX m_mtxWorld;		// 頂点カラー
	int m_nTexNumber;			//テクスチャの番号
	int m_nType;
	int m_nCounter;

};
#endif
