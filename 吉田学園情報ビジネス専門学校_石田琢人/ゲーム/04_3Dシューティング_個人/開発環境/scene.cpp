//=============================================================================
//
// オブジェクト処理 [scene.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "scene.h"
#include "bullet.h"
#include "player.h"
#include "enemy.h"
#include "field.h"
#include "model.h"
#include "number.h"
#include "polygon.h"

//==================================
// 静的メンバ変数宣言
//==================================
int CScene::m_nNumAll = 0;
CScene *CScene::m_apScene[NUM_PRIORITY][MAX_OBJECT] = {};

//==================================
// ポリゴンの解放処理
//==================================
void CScene::Release(void)
{
	if (m_apScene[m_nPriority][m_nIndex] != NULL)
	{
		int nID;
		int nPriority;

		nID = m_nIndex;				// 番号を記憶
		nPriority = m_nPriority;	// 優先順位を記憶

		//Uninit()	要らない
		delete m_apScene[nPriority][nID];	// m_nIndexも消える				delete thisでもできるけどNULLが入らない
		m_apScene[nPriority][nID] = NULL;	// 消したやつにNULLを入れる

		m_nNumAll--;			// 総数を減らす
	}
}

//==================================
// 全てのポリゴンの解放処理
//==================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				m_apScene[nCntPriority][nCntScene]->Uninit();
			}
		}
	}
}

//==================================
// 全てのオブジェクトの更新処理
//==================================
void CScene::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			if (NULL != m_apScene[nCntPriority][nCntScene])
			{
				m_apScene[nCntPriority][nCntScene]->Update();
			}
		}
	}
}

//==================================
// 全てのオブジェクトの描画処理
//==================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			if (NULL != m_apScene[nCntPriority][nCntScene])
			{
				m_apScene[nCntPriority][nCntScene]->Draw();
			}
		}
	}
}

//=========================================
// コンストラクタ
//=========================================
CScene::CScene(int nPriority)
{
	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		if (NULL == m_apScene[nPriority][nCntScene])
		{
			m_apScene[nPriority][nCntScene] = this;	// 呼び出した本人を入れる
			m_nIndex = nCntScene;			// 自分自身の番号を指定
			m_nNumAll++;					// 総数を増やす
			m_objType = OBJTYPE_NONE;		// オブジェクトの種類に初期値を入れる
			m_nPriority = nPriority;		// 優先順位の指定
			break;
		}
	}
}

//=========================================
// デストラクタ
//=========================================
CScene::~CScene()
{
}
