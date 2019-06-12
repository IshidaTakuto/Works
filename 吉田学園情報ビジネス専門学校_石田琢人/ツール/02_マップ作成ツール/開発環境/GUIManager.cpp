//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "GUIManager.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#include "modelObject.h"
#include "setPlayer.h"
#include "manager.h"
#include "enemy.h"
#include "meshField.h"

#include <stdio.h>

//=========================================
// マクロ定義
//=========================================
#define IMGUI_TEXT_SIZE	(15.0f)		// ImGuiのテキストサイズ

//=========================================
// 静的メンバ変数
//=========================================
int CGuiManager::m_nType = 0;
CModelObjectManager::TYPE CGuiManager::m_type = CModelObjectManager::TYPE_MODEL;
float CGuiManager::m_fHeight = 0.0f;				// 上げる高さ
float CGuiManager::m_fRange = 0.001f;				// 選択範囲
float CGuiManager::m_aLength[2] = { 0.0f,0.0f };	// 長さ
int CGuiManager::m_aSplit[2] = { 0,0 };				// 分割数

//=========================================
// 生成処理
//=========================================
CGuiManager *CGuiManager::Create(void)
{
	CGuiManager *pGUIManager = NULL;
	pGUIManager = new CGuiManager;		// メモリを確保

	if (pGUIManager != NULL)
	{// メモリが確保できた
	}

	return pGUIManager;
}

//=========================================
// コンストラクタ
//=========================================
CGuiManager::CGuiManager()
{
	m_showDemoWindow = false;
	m_showFieldWindow = false;
	m_clearColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	m_bRepeat = false;
}

//=========================================
// デストラクタ
//=========================================
CGuiManager::~CGuiManager()
{
}

//=========================================
// 初期化処理
//=========================================
HRESULT CGuiManager::Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	// スタイルのセットアップ
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();

	// プラットフォーム/レンダラのセットアップ
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);

#if 0	// 全角だけフォント変えるときに必要
	ImFontConfig config;
	config.MergeMode = true;
	io.Fonts->AddFontDefault();
#endif

	//フォントの設定
	LoadFont();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, m_glyphRangesJapanese);
	//io.Fonts->AddFontFromFileTTF("C:/Users/game104/AppData/Local/Microsoft/Windows/Fonts/TakaoGothic.ttf", IMGUI_TEXT_SIZE, NULL, m_glyphRangesJapanese);

	// 変数の初期値を決める
	m_showDemoWindow = false;
	m_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// モデルファイル名の保存
	m_aModelFileName[CModelObjectManager::TYPE_MODEL] = CModelObjectManager::GetModelFileName(CModelObjectManager::TYPE_MODEL);
	m_aModelFileName[CModelObjectManager::TYPE_START] = CModelObjectManager::GetModelFileName(CModelObjectManager::TYPE_START);
	m_aModelFileName[CModelObjectManager::TYPE_ENEMY] = CModelObjectManager::GetModelFileName(CModelObjectManager::TYPE_ENEMY);
	m_aModelFileName[CModelObjectManager::TYPE_ITEM] = CModelObjectManager::GetModelFileName(CModelObjectManager::TYPE_ITEM);

	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CGuiManager::Uninit(void)
{
	// 破棄
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//=========================================
// 更新処理
//=========================================
void CGuiManager::Update(void)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (m_showDemoWindow)
	{// デモウィンドウを表示
		ImGui::ShowDemoWindow(&m_showDemoWindow);
	}

	// 位置
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// メインウィンドウを表示
	{
		ImGui::Begin(u8"メインウインドウ");							// ウインドウのキャプション名

		ImGui::Text(u8"ウインドウの表示");							// テキスト表示
		//ImGui::Checkbox(u8"デモウインドウ", &m_showDemoWindow);		// デモウインドウのチェックボックス
		ImGui::Checkbox(u8"地面ウインドウ", &m_showFieldWindow);	// 地面ウインドウのチェックボックス

		// 仕切りを入れる
		SpaceLine(true);

		//------------------------------------------------
		// オブジェクトの種類を選択する
		//------------------------------------------------
		int nType = 0;		// combo関数に必要
		int nTypeOld = m_type;
		nType = (int)m_type;
		if (ImGui::Combo(u8"オブジェクトタイプ", &nType, "MODEL\0START\0ENEMY\0ITEM\0"))
		{// オブジェクトタイプが選択された
			int nNumType = CModelObjectManager::GetNumType((CModelObjectManager::TYPE)nType);
			if (nNumType <= 0)
			{// 選択したオブジェクトの種類がなかったとき
				nType = nTypeOld;	// 選択を戻す

				MessageBox(NULL, "選択したオブジェクトはありません。", "警告", MB_OK);
			}
			m_type = (CModelObjectManager::TYPE)nType;	// 変わった値を入れる

			m_nType = 0;
		}

		//------------------------------------------------
		// モデルの種類の選択
		//------------------------------------------------
		if (CModelObjectManager::TYPE_START == m_type)
		{// スタート
			ImGui::Combo(u8"モデルの種類", &m_nType, u8"スタート\0ゴール\0", CModelObjectManager::GetNumType(m_type));
		}
		else
		{// スタート以外
			ImGui::Combo(u8"モデルの種類", &m_nType, m_aModelFileName[m_type], CModelObjectManager::GetNumType(m_type));
		}

		//------------------------------------------------
		// モデルの位置
		//------------------------------------------------
		if (CManager::GetSetPlayer() != NULL)
		{// 使用しているとき
			pos = CManager::GetSetPlayer()->GetPos();
		}

		ImGui::DragFloat3(u8"位置", pos);

		if (CManager::GetSetPlayer() != NULL)
		{// 使用しているとき
			CManager::GetSetPlayer()->SetPos(pos);
		}


		//------------------------------------------------
		// モデルの向き
		//------------------------------------------------
		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		if (CManager::GetSetPlayer() != NULL)
		{// 使用しているとき
			rot = CManager::GetSetPlayer()->GetRot();
		}

		ImGui::DragFloat3(u8"向き", rot,D3DX_PI * 0.01f);

		// 一回転しないように角度を調節
		if (rot.x > D3DX_PI) { rot.x -= D3DX_PI * 2.0f; }
		if (rot.x < -D3DX_PI) { rot.x += D3DX_PI * 2.0f; }

		if (rot.y > D3DX_PI) { rot.y -= D3DX_PI * 2.0f; }
		if (rot.y < -D3DX_PI) { rot.y += D3DX_PI * 2.0f; }

		if (rot.z > D3DX_PI) { rot.z -= D3DX_PI * 2.0f; }
		if (rot.z < -D3DX_PI) { rot.z += D3DX_PI * 2.0f; }

		if (CManager::GetSetPlayer() != NULL)
		{// 使用しているとき
			CManager::GetSetPlayer()->SetRot(rot);
		}

		//------------------------------------------------
		// 選択範囲の設定
		//------------------------------------------------
		ImGui::SliderFloat(u8"選択範囲", &m_fRange, 0.001f, 1000.0f);

		// 改行を入れる
		SpaceLine();

		//------------------------------------------------
		// モデルの設置
		//------------------------------------------------
		if (ImGui::Button(u8"設置"))
		{// モデル設置
			if (CModelObjectManager::TYPE_ENEMY == m_type)
			{// 敵
				CEnemy::Create(pos, m_nType);
			}
			else
			{// 敵以外
				CModelObject::Create(pos, rot, m_type, m_nType);
			}
		}

		//------------------------------------------------
		// モデルの削除
		//------------------------------------------------
		if (ImGui::Button(u8"削除"))
		{// モデル設置
			CScene::RangeDelete();
		}

		// 仕切りを入れる
		SpaceLine(true);

		//------------------------------------------------
		// 配置データの保存
		//------------------------------------------------
		if (ImGui::Button(u8"セーブ"))
		{// 配置データをセーブする
			int nID = MessageBox(NULL, "保存しますか？", "配置データ", MB_YESNO);
			if (nID == IDYES)
			{// セーブする
				CModelObjectManager::Save();
			}
		}

		//------------------------------------------------
		// 配置データの読み込み
		//------------------------------------------------
		if (ImGui::Button(u8"ロード"))
		{// 配置データをロードする
			int nID = MessageBox(NULL, "読み込みますか？", "配置データ", MB_YESNO);
			if (nID == IDYES)
			{// ロードする
				CModelObjectManager::Load();
				MessageBox(0, "読み込みが終わりました。", "配置データ", MB_OK);
			}
		}

		ImGui::End();
	}

	if (m_showFieldWindow)
	{// 地面ウインドウの表示
		ImGui::Begin(u8"地面ウインドウ");	// ウインドウのキャプション名

		//------------------------------------------------
		// 上げる高さの設定
		//------------------------------------------------
		ImGui::DragFloat(u8"上げる高さ", &m_fHeight, 0.1f, -10.0f, 10.0f);

		//------------------------------------------------
		// 起伏の設定
		//------------------------------------------------
		ImGui::Button(u8"起伏");
		if (ImGui::IsItemClicked(0))
		{
			m_bRepeat = true;
		}
		else
		{
			m_bRepeat = false;
		}

		if (m_bRepeat)
		{// 起伏を作る
			CManager::GetMeshField()->CMeshField::SetHeight(pos, m_fHeight, m_fRange);
		}

		// 仕切りを入れる
		SpaceLine(true);

		//------------------------------------------------
		// 分割数の設定
		//------------------------------------------------
		ImGui::DragInt2(u8"分割数(X,Z)", m_aSplit, 1, 1, 1000);

		//------------------------------------------------
		// 長さの設定
		//------------------------------------------------
		ImGui::DragFloat2(u8"長さ(X,Z)", m_aLength, 0.1f, 0.001f, 1000.0f);

		// 仕切りを入れる
		SpaceLine(true);

		//------------------------------------------------
		// 地面データの保存
		//------------------------------------------------
		if (ImGui::Button(u8"セーブ"))
		{// 配置データをセーブする
			int nID = MessageBox(NULL, "保存しますか？", "地面データ", MB_YESNO);
			if (nID == IDYES)
			{// セーブする
				CManager::GetMeshField()->Save();
			}
		}

		//------------------------------------------------
		// 地面データの読み込み
		//------------------------------------------------
		if (ImGui::Button(u8"ロード"))
		{// 配置データをセーブする
			int nID = MessageBox(NULL, "読み込みますか？", "地面データ", MB_YESNO);
			if (nID == IDYES)
			{// ロードする
				CManager::GetMeshField()->Load();
				CManager::GetMeshField()->LoadVtx();
				MessageBox(0, "読み込みが終わりました。", "地面データ", MB_OK);
			}
		}

		ImGui::End();
	}

	ImGui::EndFrame();
}

//=========================================
// 描画処理
//=========================================
void CGuiManager::Draw(void)
{
	// ImGuiの描画
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//=========================================
// D3D9デバイスのハンドル紛失時の処理
//=========================================
void CGuiManager::SetPresent(HRESULT result, LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS d3dpp)
{
	if (result == D3DERR_DEVICELOST && pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		pDevice->Reset(&d3dpp);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

//=========================================
// フォントの読み取り
//=========================================
void CGuiManager::LoadFont(void)
{
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data/TEXT/font.bin", "rb");

	if (NULL != pFile)
	{// ファイルがあった

		// 頂点情報をファイルに書き込む
		fread((void*)m_glyphRangesJapanese, sizeof(ImWchar), 8253, pFile);

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開けない
		MessageBox(0, "ファイルがありません！\nファイルを作成します。", "font.bin", MB_OK);
	}
}

//=========================================
// 長さの設定
//=========================================
void CGuiManager::SetLength(float nLengthX, float nLengthZ)
{
	m_aLength[0] = nLengthX;
	m_aLength[1] = nLengthZ;
}

//=========================================
// 分割数の設定
//=========================================
void CGuiManager::SetSplit(int nSplitX, int nSplitZ)
{
	m_aSplit[0] = nSplitX;
	m_aSplit[1] = nSplitZ;
}

//=========================================
// 任意の数分改行させる
//=========================================
void CGuiManager::SpaceLine(bool bPartition, int nLine)
{
	int nCntLine = 0;
	while (nCntLine < nLine)
	{// 新しい行をつくる
		ImGui::NewLine();
		nCntLine++;
	}

	if (bPartition)
	{// 仕切り線
		ImGui::Separator();
	}
}