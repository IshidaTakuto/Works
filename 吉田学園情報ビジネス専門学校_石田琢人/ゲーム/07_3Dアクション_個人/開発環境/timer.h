//=============================================================================
//
// �^�C�}�[���� [timer.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TIMER_PRIORITY	(6)		// �����̗D�揇��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;
class CScene2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTimer : public CScene
{
public:
	CTimer(int nPriority = TIMER_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_STAGE);
	~CTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTimer* Create(D3DXVECTOR3 pos, int nTime);

	int GetTime(void) { return m_nTimer; };

private:
	CScene2D		*m_pFrame;			// �g
	D3DXVECTOR3		m_pos;				// �ʒu
	CNumber			**m_pNumber;		// �i���o�[�ϐ��ւ̃|�C���^
	int				m_nTimer;			// ���݂̎���
	int				m_nCntSecond;		// �b�̌v�Z�ɕK�v
	int				m_nDigits;			// ����
};

#endif
