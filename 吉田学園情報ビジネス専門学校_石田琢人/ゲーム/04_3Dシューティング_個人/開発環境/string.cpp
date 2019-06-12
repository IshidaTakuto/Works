//=============================================================================
//
// 文字表示処理 [string.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "string.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPD3DXFONT CString::m_pFont[NUM_FONT] = {};

//=============================================================================
// 文字フォントの設定
//=============================================================================
HRESULT CString::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 文字列フォントの初期化
	D3DXCreateFont(pDevice,
		27,                     // 文字高さ
		14,                     // 文字幅
		FW_NORMAL,              // フォントスタイル(太さ)
		NULL,                   // ミップマップモデルの数
		FALSE,                  // 斜体にするかどうか
		SHIFTJIS_CHARSET,       // 文字セット(言語)
		OUT_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		"tahoma",               //フォントの種類
		&m_pFont[0]);

	D3DXCreateFont(pDevice,
		18,                     // 文字高さ
		8,                     // 文字幅
		FW_NORMAL,              // フォントスタイル(太さ)
		NULL,                   // ミップマップモデルの数
		FALSE,                  // 斜体にするかどうか
		SHIFTJIS_CHARSET,       // 文字セット(言語)
		OUT_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		"tahoma",               //フォントの種類
		&m_pFont[1]);

	return S_OK;
}

//=============================================================================
// 文字フォントの開放
//=============================================================================
void CString::Unload(void)
{
	for (int nCntString = 0; nCntString < NUM_FONT; nCntString++)
	{// 開放
		if (NULL != m_pFont[nCntString])
		{
			m_pFont[nCntString]->Release();
			m_pFont[nCntString] = NULL;
		}
	}
}

//=============================================================================
// 文字列の生成
//=============================================================================
CString *CString::Create(void)
{
	CString *pString = NULL;

	pString = new CString;

	if (NULL != pString)
	{
		pString->Init();
	}

	return pString;
}

//=============================================================================
// 文字列の設定
//=============================================================================
bool CString::Set(char *str, RECT rect,D3DXCOLOR col, int nDispTime, int nFont)
{
	bool bComp = false;

	if (m_pStringOld != str)
	{// 前回の文字列と違ったらカウンタを初期化
		m_nCntString = 0;
	}

	m_pStringOld = str;	// 文字列を記憶

	RECT rectShadow{ rect.left + 1, rect.top + 1, rect.right, rect.bottom };

	if (nDispTime < 0)
	{// 一文字の表示間隔が負の数だと一括表示にする
		// 影文字列サイズを計算
		m_pFont[nFont]->DrawText(
			NULL,
			&str[0],		// 表示する文字列
			-1,	// 表示サイズ(-1で全部)
			&rectShadow,			// 表示範囲
			DT_CALCRECT,	// 表示範囲に調整
			NULL);			// 色

		// 文字の影
		m_pFont[nFont]->DrawText(
			NULL,
			&str[0],							// 表示する文字列
			-1,									// 表示サイズ(-1で全部)
			&rectShadow,						// 表示範囲
			DT_LEFT | DT_BOTTOM,				// 左詰めで両端揃え
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// 色

		// 文字列サイズを計算
		m_pFont[nFont]->DrawText(
			NULL,
			&str[0],		// 表示する文字列
			-1,	// 表示サイズ(-1で全部)
			&rect,			// 表示範囲
			DT_CALCRECT,	// 表示範囲に調整
			NULL);			// 色

		// そのサイズでレンダリング
		m_pFont[nFont]->DrawText(
			NULL,
			&str[0],				// 表示する文字列
			-1,						//表示サイズ(-1で全部)
			&rect,					//表示範囲
			DT_LEFT | DT_BOTTOM,	//左詰めで両端揃え
			col);					//色

		// 文字列が全て描画された
		bComp = true;
	}
	else
	{// 指定した表示間隔で文字列を表示
		int nStringDisp = 0;
		int nStrLength = strlen(str);

		nStringDisp = m_nCntString / nDispTime;		// 一定フレーム経過で文字を増やす

		if (str[nStringDisp] == '\0')
		{
			// 影文字列サイズを計算
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],		// 表示する文字列
				-1,	// 表示サイズ(-1で全部)
				&rectShadow,			// 表示範囲
				DT_CALCRECT,	// 表示範囲に調整
				NULL);			// 色

			// 文字の影
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],							// 表示する文字列
				-1,						// 表示サイズ(-1で全部)
				&rectShadow,						// 表示範囲
				DT_LEFT | DT_BOTTOM,				// 左詰めで両端揃え
				D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// 色

			// 文字列サイズを計算
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],		// 表示する文字列
				-1,	// 表示サイズ(-1で全部)
				&rect,			// 表示範囲
				DT_CALCRECT,	// 表示範囲に調整
				NULL);			// 色

			//そのサイズでレンダリング
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],				// 表示する文字列
				-1,						// 表示サイズ(-1で全部)
				&rect,					// 表示範囲
				DT_LEFT | DT_BOTTOM,	// 左詰めで両端揃え
				col);					// 色

			// 文字列が全て描画された
			bComp = true;
		}
		else if (nStringDisp < nStrLength)
		{
			if (m_nCntString % nDispTime == 0 && m_nCntString != 0)
			{// 2バイト分表示させる
				nStringDisp++;
				m_nCntString += nDispTime;
			}

			// 文字を表示するまでのカウンタ
			m_nCntString++;

			// 影文字列サイズを計算
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],		// 表示する文字列
				nStringDisp,	// 表示サイズ(-1で全部)
				&rectShadow,	// 表示範囲
				DT_CALCRECT,	// 表示範囲に調整
				NULL);			// 色

								// 文字の影
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],							// 表示する文字列
				nStringDisp,						// 表示サイズ(-1で全部)
				&rectShadow,						// 表示範囲
				DT_LEFT | DT_BOTTOM,				// 左詰めで両端揃え
				D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// 色

			//文字列サイズを計算
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],		// 表示する文字列
				nStringDisp,	// 表示サイズ(-1で全部)
				&rect,			// 表示範囲
				DT_CALCRECT,	// 表示範囲に調整
				NULL);			// 色

								//そのサイズでレンダリング
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],				// 表示する文字列
				nStringDisp,			//表示サイズ(-1で全部)
				&rect,					//表示範囲
				DT_LEFT | DT_BOTTOM,	//左詰めで両端揃え
				col);					//色
		}
	}

	// 入力の情報を取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_1) || pKeyboard->GetTrigger(DIK_RETURN))
	{
		m_nCntString = strlen(str) * nDispTime;
	}

	return bComp;
}

//=========================================
// コンストラクタ
//=========================================
CString::CString()
{
	m_nCntString = 0;
	m_pStringOld = NULL;
}

//=========================================
// デストラクタ
//=========================================
CString::~CString()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CString::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CString::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void CString::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CString::Draw(void)
{
}