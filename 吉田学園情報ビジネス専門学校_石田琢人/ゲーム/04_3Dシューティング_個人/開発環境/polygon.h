//=============================================================================
//
// ポリゴン処理 [polygon.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPolygon
{
public:
	//**************************************
	// テクスチャの種類
	//**************************************
	typedef enum
	{
		TYPE_PRESS_ANY = 0,		// PRESS_ANY
		TYPE_CONTINUE,			// ポーズ中のコンテニュー
		TYPE_RETRY,				// ポーズ中のリトライ
		TYPE_QUIT,				// ポーズ中のタイトルに戻る
		TYPE_TITLE_LOGO,		// タイトルのロゴ
		TYPE_RESULT_LOGO,		// リザルトのロゴ
		TYPE_GAMEOVER_LOGO,		// ゲームオーバーのロゴ
		TYPE_RANKING_LOGO,		// ランキングのロゴ
		TYPE_COMMUNICATEWINDOW,	// 通信ウィンドウ
		TYPE_STOCK00,			// ストックアイコン（戦闘機）
		TYPE_STOCK01,			// ストックアイコン（戦艦）
		TYPE_STOCK02,			// ストックアイコン（戦車）
		TYPE_MAX
	}TYPE;

	CPolygon();
	~CPolygon();
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPolygon* Create(int nType, D3DXVECTOR3 pos, float fWidth, float fHeight, int nTexType = -1);

	static HRESULT Load(void);				// テクスチャを読み込む
	static void Unload(void);				// 読み込んだテクスチャを解放

	const void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; };

	const void SetColor(D3DXCOLOR col);
	const void AddColor(D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; };

	const void SetWidth(float fWidth);
	float GetWidth(void) { return m_fWidth; };

	const void SetHeight(float fHeight);
	float GetHeight(void) { return m_fHeight; };

	void Flashing(int nFlame);

	void TextureChange(TYPE texType) { m_nTexType = texType; };

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];		// 共有テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// 頂点バッファへのポインタ

	D3DXVECTOR3 m_pos;									// 座標
	D3DXCOLOR m_col;									// 色
	float m_fWidth;										// ポリゴンのＸ軸
	float m_fHeight;									// ポリゴンのＹ軸
	int	m_nTexType;										// テクスチャ番号
	int m_nType;										// 作り方の種類
	int m_nCntColor;									// 色のカウンタ
};

#endif