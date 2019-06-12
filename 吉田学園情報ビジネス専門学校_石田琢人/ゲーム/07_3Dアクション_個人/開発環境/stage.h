//=============================================================================
//
// ステージ管理処理 [stage.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "scene.h"
#include "logo.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STAGE_PRIORITY			(7)		// ステージ管理の優先番号
#define STAGELOGO_PRIORITY		(6)		// ステージロゴの優先番号

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CStage : public CScene2D
{
public:
	typedef enum
	{// フェードの状態
		FADE_NONE = 0,		// 何もしていない状態
		FADE_IN,			// フェードイン状態
		FADE_OUT,			// フェードアウト状態
		FADE_MAX
	} FADE;

	static CStage *Create(int nStage);

	CStage(int nPriority = STAGE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_STAGE);
	~CStage();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void Stage(void);	// ステージ入れ替えの処理

	FADE m_fade;		// フェードの状態
	int m_nStage;		// 現在のステージ
};

#endif