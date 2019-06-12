//=============================================================================
//
// �Q�[������ [game.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"
#include "stage.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GAME_PRIORITY		(7)				// �D��ԍ�
#define MAX_ENEMY_APPEAR	(32)			// �G�̏o����
#define GRAVITY				(0.5f)			// �d��
#define GAME_NUM_BG			(2)				// �w�i�̐�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPause;
class CBg;
class CTimer;
class CLogoTutorial;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGame : public CScene
{
public:
	typedef enum
	{
		STATE_NONE = 0,	// �������Ă��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_END,		// �Q�[���I�[�o�[
		STATE_CLEAR,	// �Q�[���N���A
		STATE_MAX
	}STATE;

	CGame(int nPriority = GAME_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_GAME);
	~CGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static const void SetGameState(STATE state);
	STATE GetGameState(void) { return m_state; };

	const void SetPauseBool(bool bPause) { m_bPause = bPause; };
	bool GetPauseBool(void) { return m_bPause; };

	CPause *GetPause(void) { return m_pPause; };

	static int GetStage(void) { return m_nCurStage; };
	static void NextStage(void);
	static void SetStage(int stage);

	static void MoveMountain(float fMove);

private:
	CPause *m_pPause;				// �|�[�Y�N���X�̃|�C���^�ϐ�
	static STATE m_state;			// �Q�[���̏��
	bool m_bPause;					// �|�[�YON/OFF�̐ؑ�
	int m_modeCounter;				// �J�ڂ���܂ł̃J�E���^
	int	m_nCntEnemyAppear;			// �G���o��������J�E���^
	CBg *m_pBg[GAME_NUM_BG];		// �w�i�̃|�C���^�ϐ�
	static CBg *m_pMountain;		// �w�i�̎R
	CTimer *m_pTimer;				// �^�C�}�[�N���X�̃|�C���^�ϐ�
	static int m_nCurStage;			// ���݂̃X�e�[�W
	CLogoTutorial	*m_pTutorial;	// ��������\���N���X�̃|�C���^�ϐ�
};

#endif