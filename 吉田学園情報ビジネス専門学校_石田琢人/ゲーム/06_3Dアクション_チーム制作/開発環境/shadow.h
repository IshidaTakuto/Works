//*****************************************************************************
//
//     影の処理[shadow.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SHADOW_H_
#define _SHADOW_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene3D.h"

//*****************************************************************************
//    Xファイルモデルクラスの定義
//*****************************************************************************
class CShadow : public CScene3D
{
public:    // 誰でもアクセス可能
	CShadow(int nPriority = 3, OBJTYPE objType = OBJTYPE_SHADOW);
	~CShadow();
	static CShadow *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nPriority);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetRadius(const float fRadius);

	float GetRadius(void);
	D3DXVECTOR3 GetPosOld(void);

protected: // このクラスと派生クラスだけがアクセス可能
	static LPDIRECT3DTEXTURE9 m_apTexture;   // テクスチャへのポインタ
	D3DXVECTOR3 m_PosOld;    // 前回の座標
	float       m_fRadius;   // 影の半径
};

#endif