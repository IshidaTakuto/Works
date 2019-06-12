//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CInput;
class CRenderer;
class CSound;
class CInputKeyboard;
class CInputJoypad;
class CCamera;
class CLight;
class CTitle;
class CTutorial;
class CGame;
class CResult;
class CRanking;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CManager
{
public:
	typedef enum
	{// �t���[�����[�N
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_GAMEOVER,
		MODE_RANKING,
		MODE_MAX
	}MODE;

	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRenderer *GetRenderer(void) { return m_pRenderer; };				// �����_���[�̎擾
	static CCamera *GetCamera(void) { return m_pCamera; };						// �J�����̎擾
	static CLight *GetLight(void) { return m_pLight; };							// ���C�g�̎擾
	static CSound *GetSound(void) { return m_pSound; };							// �T�E���h�̎擾
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; };	// �L�[�{�[�h�̃C���v�b�g�̎擾
	static CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; };		// �p�b�h�̃C���v�b�g�̎擾
	static CTitle *GetTitle(void) { return m_pTitle; };							// �^�C�g���̎擾
	static CTutorial *GetTutorial(void) { return m_pTutorial; };				// �`���[�g���A���̎擾
	static CGame *GetGame(void) { return m_pGame; };							// �Q�[���̎擾
	static CResult *GetResult(void) { return m_pResult; };						// ���U���g�̎擾
	static CRanking *GetRanking(void) { return m_pRanking; };					// �����L���O�̎擾

	static MODE GetMode(void) { return m_mode; };
	static const void SetMode(MODE mode);

private:
	static CRenderer		*m_pRenderer;				// �����_�����O�N���X�̃|�C���^�ϐ�
	static CCamera			*m_pCamera;					// �J�����N���X�̃|�C���^�ϐ�
	static CLight			*m_pLight;					// ���C�g�N���X�̃|�C���^�ϐ�
	static CSound			*m_pSound;					// �T�E���h�N���X�̃|�C���^�ϐ�
	static CInputKeyboard	*m_pInputKeyboard;			// �L�[�{�[�h�̃C���v�b�g�N���X�̃|�C���^�ϐ�
	static CInputJoypad		*m_pInputJoypad;			// �p�b�h�̃C���v�b�g�N���X�̃|�C���^�ϐ�
	static CTitle			*m_pTitle;					// �^�C�g���N���X�̃|�C���^�ϐ�
	static CTutorial		*m_pTutorial;				// �`���[�g���A���N���X�̃|�C���^�ϐ�
	static CGame			*m_pGame;					// �Q�[���N���X�̃|�C���^�ϐ�
	static CResult			*m_pResult;					// ���U���g�N���X�̃|�C���^�ϐ�
	static CRanking			*m_pRanking;				// �����L���O�N���X�̃|�C���^�ϐ�
	static MODE				m_mode;						// ���݂̃t���[�����[�N
};

#endif