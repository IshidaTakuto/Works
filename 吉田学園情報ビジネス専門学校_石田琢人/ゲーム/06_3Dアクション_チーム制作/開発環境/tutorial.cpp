//*****************************************************************************
//
//     チュートリアルの処理[tutorial.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "tutorial.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "scene2DSquare.h"

#include "debuglog.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define TUTORIAL_TEXTURENAME_0 "data/TEXTURE/TUTORIAL/tutorial.jpg"
#define TUTORIAL_TEXTURENAME_1 "data/TEXTURE/TUTORIAL/TUTORIAL.png"

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTutorial::m_apTexture[2] = {};  // チュートリアルのテクスチャ画像

//=============================================================================
//    コンストラクタ
//=============================================================================
CTutorial::CTutorial(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	for (int nCntLogo = 0; nCntLogo < 2; nCntLogo++)
	{
		m_apSceneSquare[nCntLogo] = NULL;
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CTutorial *CTutorial::Create(void)
{
	CTutorial *pTutorial = NULL;  // チュートリアルクラス型のポインタ
	if (pTutorial == NULL)
	{// メモリが確保できる状態にある
		pTutorial = new CTutorial;
		if (pTutorial != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pTutorial->Init();
		}
		else
		{// メモリが確保できなかった
			return NULL;
		}
	}
	else
	{// メモリが確保できる状態にない
		return NULL;
	}

	// インスタンスを返す
	return pTutorial;
}

//=============================================================================
//    テクスチャの読み込み処理
//=============================================================================
HRESULT CTutorial::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			char aTexFileName[2][256] =
			{
				TUTORIAL_TEXTURENAME_0,
				TUTORIAL_TEXTURENAME_1,
			};
			for (int nCntTex = 0; nCntTex < 2; nCntTex++)
			{
				D3DXCreateTextureFromFile(pDevice, aTexFileName[nCntTex], &m_apTexture[nCntTex]);
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    テクスチャの解放処理
//=============================================================================
void CTutorial::UnLoad(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < 2; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CTutorial::Init(void)
{
	// テクスチャの読み込み
	Load();

	// 一枚絵の生成
	if (m_apSceneSquare[0] == NULL)
	{
		m_apSceneSquare[0] = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		if (m_apSceneSquare[0] != NULL)
		{
			m_apSceneSquare[0]->BindTexture(m_apTexture[0]);
		}
	}

	// ロゴの生成
	if (m_apSceneSquare[1] == NULL)
	{
		m_apSceneSquare[1] = CScene2DSquare::Create(D3DXVECTOR3(1060.0f, 660.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 230.0f, 60.0f);
		if (m_apSceneSquare[1] != NULL)
		{
			m_apSceneSquare[1]->BindTexture(m_apTexture[1]);
		}
	}


	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	// テクスチャの開放
	UnLoad();

	// ポリゴンの開放
	for (int nCntLogo = 0; nCntLogo < 2; nCntLogo++)
	{
		if (m_apSceneSquare[nCntLogo] != NULL)
		{
			m_apSceneSquare[nCntLogo]->Uninit();
			m_apSceneSquare[nCntLogo] = NULL;
		}
	}

	// チュートリアルオブジェクトの破棄
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CTutorial::Update(void)
{
	if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
	{// フェードの状態が使用されていない状態である
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// 決定ボタンが押された
				CManager::GetFade()->SetFade(CManager::MODE_GAME);
			}
			else
			{// キーボードの決定ボタンが押されていない
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_12; nCntButton++)
				{// 反応するボタン数の数だけ繰り返し
					if (CManager::GetJoyStick()->GetJoyStickDevice(0)->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// ボタンが押された
						CManager::GetFade()->SetFade(CManager::MODE_GAME);
					}
				}
			}
		}
	}

	CDebugProc::Print("チュートリアルなう\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CTutorial::Draw(void)
{

}