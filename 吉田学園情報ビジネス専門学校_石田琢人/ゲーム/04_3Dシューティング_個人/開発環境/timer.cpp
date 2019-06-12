//=============================================================================
//
// �^�C�}�[���� [timer.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "timer.h"
#include "number.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TIMER0		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TIMER1		"data/TEXTURE/frame_timer.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define NUM_TIMER	(2)		// �^�C�}�[�̍ő包��
#define TIMER_SIZE	(40)	// �^�C�}�[�̕����̑傫��
#define MAX_TEX		(2)		// �e�N�X�`���̐�
#define MAX_TIMER	(NUM_TIMER + 1)
#define FRAME_SIZE_X	(120)	// �t���[���̑傫��
#define FRAME_SIZE_Y	(100)	// �t���[���̑傫��
#define TIMER_TIME		(60)	// �Q�[������

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CNumber *CTimer::m_pNumber[TIMER_DIGITS] = {};
int CTimer::m_nTimer = 0;

//=============================================================================
// ��������
//=============================================================================
CTimer* CTimer::Create(D3DXVECTOR3 pos)
{
	CTimer *pTimer;

	pTimer = new CTimer;	// ���������m��
	for (int nCntScore = 0; nCntScore < TIMER_DIGITS; nCntScore++)
	{
		pTimer->m_pNumber[nCntScore] = CNumber::Create(D3DXVECTOR3((pos.x - (nCntScore * 40)) + (40 * (TIMER_DIGITS / 2.0f)) - 40 / 2, pos.y, 0.0f), TIMER_SIZE);
	}

	pTimer->Init();

	return pTimer;
}

//=========================================
// �R���X�g���N�^
//=========================================
CTimer::CTimer(int nPriority) : CScene(nPriority)
{
	m_nCntSecond = 0;
}

//=========================================
// �f�X�g���N�^
//=========================================
CTimer::~CTimer()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTimer::Init(void)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_UI);

	m_nTimer = TIMER_TIME;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTimer::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < TIMER_DIGITS; nCntScore++)
	{
		if (NULL != m_pNumber[nCntScore])
		{
			m_pNumber[nCntScore]->Uninit();
			delete m_pNumber[nCntScore];
			m_pNumber[nCntScore] = NULL;
		}
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTimer::Update(void)
{
	int aNumber[NUM_TIMER];		// �v�Z�p

	m_nCntSecond++;

	if (m_nCntSecond % 60 == 0 && m_nTimer > 0)
	{// ���Ԃ����炷
		m_nTimer--;	// �J�E���g�_�E��
	}

	if (m_nTimer <= 0 && m_nCntSecond % 180 == 0)
	{// �^�C���I�[�o�[
		CGame::SetGameState(CGame::STATE_CLEAR);
	}

	aNumber[0] = m_nTimer % 10 / 1;
	aNumber[1] = m_nTimer % 100 / 10;

	// �^�C�}�[�̍X�V
	for (int nCntScore = 0; nCntScore < TIMER_DIGITS; nCntScore++)
	{
		m_pNumber[nCntScore]->SetNumber(aNumber[nCntScore]);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTimer::Draw(void)
{
	for (int nCntScore = 0; nCntScore < TIMER_DIGITS; nCntScore++)
	{// �i���o�[�|���S���̕`��
		m_pNumber[nCntScore]->Draw();
	}
}
