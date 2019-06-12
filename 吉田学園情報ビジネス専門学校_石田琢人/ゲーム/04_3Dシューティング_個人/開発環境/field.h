//=============================================================================
//
// フィールド処理 [field.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "scene3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FIELD_TEXTURE	(3)			// テクスチャ数
#define FIELD_PRIORITY	(2)			// 処理の優先番号
#define MAX_FIELD		(4)			// フィールドの数
#define FIELD_WIDTH		(3000.0f)	// 一つの地面の進行方向の長さ
#define FIELD_DEPTH		(8000.0f)	// 地面の奥行

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CWall;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CField : public CScene3D
{
public:
	//+++++++++++++++++++++
	// フィールドの種類
	//+++++++++++++++++++++
	typedef enum
	{
		TYPE_GROUND,	// 地面
		TYPE_SEA,		// 海
		TYPE_MAX
	}TYPE;

	CField(int nPriority = FIELD_PRIORITY);
	~CField();
	HRESULT Init(TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CField* Create(TYPE type);

	static HRESULT Load(void);				// テクスチャを読み込む
	static void Unload(void);				// 読み込んだテクスチャを解放

	bool Collision(D3DXVECTOR3 *pos, float fVtxMinY);
	TYPE GetType(void) { return m_type; };

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[FIELD_TEXTURE];	// 共有テクスチャへのポインタ
	TYPE m_type;				// 種類
	static int m_nCntPos;		// 位置のカウンタ
	CScene3D *m_pScene3D;		// 3Dオブジェクトのポインタ変数
	CWall *m_pWall;				// 壁へのポインタ変数
};

#endif
