//*****************************************************************************
//
//     タイトルの処理[title.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "title.h"
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "logo.h"
#include "scene2DRot.h"
#include "sound.h"

#include "debuglog.h"
#include "map.h"
#include "cylinder.h"
#include "dome.h"
#include "shadow.h"
#include "field.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define TITLE_PRIORITY           (6)                                       // タイトルの描画優先順位

#define TITLE_MODE_CHANGETIMING  (600)                                     // どれくらいの時間で勝手に遷移するか
#define TITLE_PRESSLOGO_POS      (D3DXVECTOR3(SCREEN_WIDTH / 2,640,0.0f))  // PRESSENTERを出す位置
#define TITLE_PRESSLOGO_SCALE    (350)                                     // PRESSENTERの大きさ

#define TITLE_LOGO_POS           (D3DXVECTOR3(SCREEN_WIDTH / 2,280,0.0f))  // タイトルロゴを出す位置
#define TITLE_LOGO_SCALE         (550)                                     // タイトルロゴの大きさ
#define TITLE_LOGO_TEXTURENAME   "data/TEXTURE/TITLE/titlelogo.png"

#define GAME_MANAGER_INITFILENAME  "data/TEXT/MODE/game.ini"   // ゲームのシステムファイル名

// 値を読み取るパス名
#define PLAYER_FILENAME "PLAYER_FILENAME"
#define MAP_FILENAME    "MAP_FILENAME"

#define NUM_MODEL        "NUM_MODEL = "
#define MODEL_FILENAME   "MODEL_FILENAME = "

#define GAME_MAP_FILENAME_0        "data/TEXT/MAP/map000.txt"  // ゲームで使用するマップスクリプトファイル名
#define GAME_MAP_FILENAME_1        "data/TEXT/MAP/map001.txt"  // ゲームで使用するマップスクリプトファイル名
//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTitle::m_apTexture = NULL;  // タイトルのテクスチャ画像

//=============================================================================
//    コンストラクタ
//=============================================================================
CTitle::CTitle(int nPriority, OBJTYPE objType) : CScene(nPriority,objType)
{
	// 各種値のクリア
	m_pMap = NULL;        // マップクラスへのポインタ
	m_pLogoPress = NULL;  // PRESSENTERクラスへのポインタ
	m_nCounter = 0;       // 遷移を管理するカウンター
	m_pLogo = NULL;       // タイトルロゴクラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CTitle *CTitle::Create(void)
{
	CTitle *pTitle = NULL;  // タイトルクラス型のポインタ
	if (pTitle == NULL)
	{// メモリが確保できる状態にある
		pTitle = new CTitle;
		if (pTitle != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pTitle->Init();
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
	return pTitle;
}

//=============================================================================
//    テクスチャの読み込み処理
//=============================================================================
HRESULT CTitle::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	 // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			D3DXCreateTextureFromFile(pDevice, TITLE_LOGO_TEXTURENAME, &m_apTexture);
		}
	}

	return S_OK;
}

//=============================================================================
//    テクスチャの解放処理
//=============================================================================
void CTitle::UnLoad(void)
{
	// テクスチャの破棄
	if (m_apTexture != NULL)
	{
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
	// テクスチャの読み込み
	CLogoPress::Load();   // PRESSENTER
	CCylinder::Load();    // 山
	CDome::Load();        // 空
	CField::Load();       // フィールド
	CShadow::Load();      // 影
	Load();               // タイトルロゴ

	FILE *pFile = NULL;
	char aString[256];
	char aMapFileName[256];
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(GAME_MANAGER_INITFILENAME, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// テキストが最後の行に到達するまで読み込みを続ける
				if (strcmp(&aString[0], MAP_FILENAME) == 0)
				{// マップのスクリプトファイル名がある
					fscanf(pFile, "%s %s", &aString[0], &aMapFileName[0]);
				}
			}
		}
	}

	if (m_pMap == NULL)
	{
		m_pMap = CMap::Create(aMapFileName);
	}

	// タイトルロゴのインスタンス生成
	if (m_pLogo == NULL)
	{// メモリが確保できる状態にある
		m_pLogo = CLogo::Create(TITLE_LOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TITLE_LOGO_SCALE, TITLE_PRIORITY);
		m_pLogo->BindTexture(m_apTexture);

		// 頂点バッファの取得
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pLogo->GetVtxBuff();

		// 頂点情報
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(-600.0f, -300.0f, 0.0f) + TITLE_LOGO_POS;
		pVtx[1].pos = D3DXVECTOR3(600.0f, -300.0f, 0.0f) + TITLE_LOGO_POS;
		pVtx[2].pos = D3DXVECTOR3(-600.0f, 300.0f, 0.0f) + TITLE_LOGO_POS;
		pVtx[3].pos = D3DXVECTOR3(600.0f, 300.0f, 0.0f) + TITLE_LOGO_POS;

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();

		// 頂点バッファの設定
		m_pLogo->SetVtxBuff(pVtxBuff);
	}

	// PRESSENTERのインスタンス生成
	if (m_pLogoPress == NULL)
	{// メモリが確保できる状態にある
		m_pLogoPress = CLogoPress::Create(TITLE_PRESSLOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TITLE_PRESSLOGO_SCALE, TITLE_PRIORITY);

		// 頂点バッファの取得
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pLogoPress->GetVtxBuff();

		// 頂点情報
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(-300.0f, -80.0f, 0.0f) + TITLE_PRESSLOGO_POS;
		pVtx[1].pos = D3DXVECTOR3(300.0f, -80.0f, 0.0f) + TITLE_PRESSLOGO_POS;
		pVtx[2].pos = D3DXVECTOR3(-300.0f, 80.0f, 0.0f) + TITLE_PRESSLOGO_POS;
		pVtx[3].pos = D3DXVECTOR3(300.0f, 80.0f, 0.0f) + TITLE_PRESSLOGO_POS;

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();

		// 頂点バッファの設定
		m_pLogoPress->SetVtxBuff(pVtxBuff);
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	// テクスチャの開放
	CLogoPress::UnLoad();   // PRESSENTER
	CCylinder::UnLoad();    // 山
	CDome::UnLoad();        // 空
	CField::UnLoad();       // フィールド
	CShadow::UnLoad();      // 影
	UnLoad();               // タイトルロゴ

	if (m_pLogoPress != NULL)
	{// メモリが確保できる状態にある
		// PRESSENTERの終了処理
		m_pLogoPress->Uninit();
		m_pLogoPress = NULL;
	}

	if (m_pLogo != NULL)
	{// メモリが確保できる状態にある
	    // タイトルロゴの終了処理
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	// マップの破棄
	if (m_pMap != NULL)
	{// メモリが確保されている
		m_pMap->Uninit();

		// メモリの開放
		delete m_pMap;
		m_pMap = NULL;
	}

	// タイトルオブジェクトの破棄
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CTitle::Update(void)
{
	if (m_pLogoPress != NULL)
	{
		m_pLogoPress->Update();
	}

	m_nCounter++;  // カウンターを進める

	if (m_nCounter >= TITLE_MODE_CHANGETIMING)
	{// カウンターが既定の値を超えた
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			CManager::GetFade()->SetFade(CManager::MODE_RANKING);
		}
	}
	else
	{// カウンターがまだ既定の値を超えていない
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// 決定ボタンが押された
				CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
			}
			else
			{// キーボードの決定ボタンが押されていない
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_12; nCntButton++)
				{// 反応するボタン数の数だけ繰り返し
					if (CManager::GetJoyStick()->GetJoyStickDevice(0)->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// ボタンが押された
						CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
					}
				}
			}
		}
	}

	CDebugProc::Print("タイトルなう\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CTitle::Draw(void)
{

}