//*****************************************************************************
//
//     水の処理[water.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _WATER_H_
#define _WATER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene3DMesh.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define MAX_WATER_TEX  (2)   // 読み込む水のテクスチャの数

//*****************************************************************************
//    水クラスの定義
//*****************************************************************************
class CWater : public CScene3DMesh
{
public:    // 誰でもアクセス可能
	typedef enum
	{
		STATE_NONE = 0,    // 何もしていない状態
		STATE_RIPPLE,      // 波紋を起こす状態
		STATE_MAX
	}STATE;

	CWater(int nPriority = 3, OBJTYPE objType = OBJTYPE_WATER);
	~CWater();
	static CWater *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fDepth, int nXBlock, int nZBlock, char *aVertexFileName, int nTexIdx, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fDepth, int nXBlock, int nZBlock, char *aVertexFileName, int nTexIdx);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeNormal(void);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

	float GetRipplesHeight(void);
	float GetPolyHeight(const D3DXVECTOR3 pos, D3DXVECTOR3 *pPolygonNormal);

	void SetState(const STATE state);
	void SetRipplePos(const D3DXVECTOR3 RipplePos);
	void SetRipplesHeight(const float fRipplesHeight);
	void SetRipples(D3DXVECTOR3 pos, float fRange, float fHeight);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static int                m_nNumAll;
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_WATER_TEX];  // テクスチャへのポインタ
	STATE       m_State;                 // 状態
	D3DXVECTOR3 m_RipplePos;             // 波紋を起こす座標
	float       m_fWidth;                // 1ブロック分の幅
	float       m_fDepth;                // 1ブロック分の奥行
	int         m_nXBlock;               // 横の分割数
	int         m_nZBlock;               // 奥行の分割数
	char        m_aVertexFileName[256];  // 頂点情報へのスクリプトファイル名
	float       m_fTexU;                 // テクスチャU座標
	float       m_fTexV;                 // テクスチャV座標
	float       m_fRipples;              // 波紋に使用する倍率
	float       m_fRipplesHeight;        // 波紋をどれくらい高くするか
};

#endif