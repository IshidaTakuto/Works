//=============================================================================
//
// ポーズメニュー処理 [pause.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "pause.h"
#include "polygon.h"
#include "scene.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PMENU_SIZE_X	(200.0f)		// メニューの大きさ(X)
#define PMENU_SIZE_Y	(45.0f)			// メニューの大きさ(Y)

//=============================================================================
// 生成処理
//=============================================================================
CPause* CPause::Create(void)
{
	CPause *pPause = NULL;	// ポーズクラスのポインタ変数

	pPause = new CPause;	// メモリを確保

	if (NULL != pPause)
	{// メモリを確保できたとき
		// ポーズメニューの選択肢
		pPause->m_apPolygon[TYPE_CONTINUE] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH / 2, 250, 0.0f), PMENU_SIZE_X, PMENU_SIZE_Y, CPolygon::TYPE_CONTINUE);
		pPause->m_apPolygon[TYPE_RETRY] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH / 2, 350, 0.0f), PMENU_SIZE_X, PMENU_SIZE_Y, CPolygon::TYPE_RETRY);
		pPause->m_apPolygon[TYPE_QUIT] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH / 2, 450, 0.0f), PMENU_SIZE_X, PMENU_SIZE_Y, CPolygon::TYPE_QUIT);

		// 半透明の背景
		pPause->m_apPolygon[TYPE_MAX] = CPolygon::Create(1, D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);

		// 初期化処理
		pPause->Init();
	}

	return pPause;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPause::CPause()
{
	for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
	{
		m_apPolygon[nCntPolygon] = NULL;
	}

	m_nNumSelect = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPause::~CPause()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPause::Init(void)
{
	// 最初の選択カーソルは一番上に合わせる
	m_nNumSelect = TYPE_CONTINUE;

	// 頂点カラー
	m_apPolygon[TYPE_CONTINUE]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apPolygon[TYPE_RETRY]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	m_apPolygon[TYPE_QUIT]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	m_apPolygon[TYPE_MAX]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPause::Uninit(void)
{
	int nCntMenu;

	for (nCntMenu = 0; nCntMenu <= TYPE_MAX; nCntMenu++)
	{// ポリゴンの破棄
		if (m_apPolygon[nCntMenu] != NULL)
		{
			m_apPolygon[nCntMenu]->Uninit();
			delete m_apPolygon[nCntMenu];
			m_apPolygon[nCntMenu] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CPause::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// ジョイパッドの取得
	CSound *pSound = CManager::GetSound();						// サウンドの情報を取得

	// 選択処理
	Select();

	if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_7) || pKeyboard->GetTrigger(DIK_P))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_CANCEL);
		m_nNumSelect = TYPE_CONTINUE;
	}

	if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_18) || pKeyboard->GetTrigger(DIK_W) || pKeyboard->GetTrigger(DIK_UP))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_BOTTON000);
		m_nNumSelect = (m_nNumSelect + (TYPE_MAX - 1)) % TYPE_MAX;	// 選択状態を１つ上げる

		for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
		{// 選択中と他で色を分ける
			if (nCntPolygon == m_nNumSelect)
			{// 選択中
				m_apPolygon[nCntPolygon]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{// 他
				m_apPolygon[nCntPolygon]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
			}
		}
	}

	if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_19) || pKeyboard->GetTrigger(DIK_S) || pKeyboard->GetTrigger(DIK_DOWN))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_BOTTON000);
		m_nNumSelect = (m_nNumSelect + 1) % TYPE_MAX;	// 選択状態を１つ下げる

		for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
		{// 選択中と他で色を分ける
			if (nCntPolygon == m_nNumSelect)
			{// 選択中
				m_apPolygon[nCntPolygon]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{// 他
				m_apPolygon[nCntPolygon]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPause::Draw(void)
{
	for (int nCntPolygon = TYPE_MAX; nCntPolygon >= 0; nCntPolygon--)
	{// 配列の大きい方から描画する
		m_apPolygon[nCntPolygon]->Draw();
	}

}

//=============================================================================
// 選択処理
//=============================================================================
void CPause::Select(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// ジョイパッドの取得
	CSound *pSound = CManager::GetSound();						// サウンドの情報を取得

	bool bPause = CGame::GetPause();

	if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_1) || pKeyboard->GetTrigger(DIK_RETURN))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);

		// ポーズ画面から抜ける
		bPause = false;
		CGame::SetPause(bPause);

		// 選択中の項目
		switch (m_nNumSelect)
		{
		case TYPE_CONTINUE:
			break;

		case TYPE_RETRY:
				// 画面遷移
				CFade::Create(CManager::MODE_GAME);

			break;

		case TYPE_QUIT:
				// 画面遷移
				CFade::Create(CManager::MODE_TITLE);

			break;
		}
	}
}

