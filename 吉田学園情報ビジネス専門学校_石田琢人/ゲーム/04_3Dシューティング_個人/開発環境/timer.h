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
#define TIMER_DIGITS	(2)		// ����

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTimer : public CScene
{
public:
	CTimer(int nPriority = 6);
	~CTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTimer* Create(D3DXVECTOR3 pos);

	static int GetTime(void) { return m_nTimer; };

private:
	static CNumber *m_pNumber[TIMER_DIGITS];	// �i���o�[�ϐ��ւ̃|�C���^
	static int		m_nTimer;					// ��������
	int				m_nCntSecond;				// �b�̌v�Z�ɕK�v
};

#endif
