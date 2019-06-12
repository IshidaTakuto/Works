//=============================================================================
//
// �`���[�g���A������ [tutorial.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "field.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CString;
class CCommunicateWindow;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTutorial
{
public:
	CTutorial();
	~CTutorial();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CField *GetField(int nIndex) { return m_pField[nIndex]; };	// �t�B�[���h�̎擾

private:
	static CField	*m_pField[MAX_FIELD];	// �t�B�[���h�N���X�̃|�C���^�ϐ�

	CString *m_pString[2];		// �����\���N���X�̃|�C���^�ϐ�
	CCommunicateWindow *m_pCommunicateWindow;
	int	m_nCntString;			// �����̕\�����Ԃ̃J�E���^
	int m_nCntEnemyAppear;		// �G�̏o���p�J�E���^
	int m_nCntTutorialTime;		// �`���[�g���A���̎��Ԍv��
};

#endif