//=============================================================================
//
// �|�[�Y���j���[���� [pause.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "pause.h"
#include "polygon.h"
#include "scene.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PMENU_SIZE_X	(200.0f)		// ���j���[�̑傫��(X)
#define PMENU_SIZE_Y	(45.0f)			// ���j���[�̑傫��(Y)

//=============================================================================
// ��������
//=============================================================================
CPause* CPause::Create(void)
{
	CPause *pPause = NULL;	// �|�[�Y�N���X�̃|�C���^�ϐ�

	pPause = new CPause;	// ���������m��

	if (NULL != pPause)
	{// ���������m�ۂł����Ƃ�
		// �|�[�Y���j���[�̑I����
		pPause->m_apPolygon[TYPE_CONTINUE] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH / 2, 250, 0.0f), PMENU_SIZE_X, PMENU_SIZE_Y, CPolygon::TYPE_CONTINUE);
		pPause->m_apPolygon[TYPE_RETRY] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH / 2, 350, 0.0f), PMENU_SIZE_X, PMENU_SIZE_Y, CPolygon::TYPE_RETRY);
		pPause->m_apPolygon[TYPE_QUIT] = CPolygon::Create(0, D3DXVECTOR3(SCREEN_WIDTH / 2, 450, 0.0f), PMENU_SIZE_X, PMENU_SIZE_Y, CPolygon::TYPE_QUIT);

		// �������̔w�i
		pPause->m_apPolygon[TYPE_MAX] = CPolygon::Create(1, D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);

		// ����������
		pPause->Init();
	}

	return pPause;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPause::CPause()
{
	for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
	{
		m_apPolygon[nCntPolygon] = NULL;
	}

	m_nNumSelect = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPause::~CPause()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPause::Init(void)
{
	// �ŏ��̑I���J�[�\���͈�ԏ�ɍ��킹��
	m_nNumSelect = TYPE_CONTINUE;

	// ���_�J���[
	m_apPolygon[TYPE_CONTINUE]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_apPolygon[TYPE_RETRY]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	m_apPolygon[TYPE_QUIT]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	m_apPolygon[TYPE_MAX]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPause::Uninit(void)
{
	int nCntMenu;

	for (nCntMenu = 0; nCntMenu <= TYPE_MAX; nCntMenu++)
	{// �|���S���̔j��
		if (m_apPolygon[nCntMenu] != NULL)
		{
			m_apPolygon[nCntMenu]->Uninit();
			delete m_apPolygon[nCntMenu];
			m_apPolygon[nCntMenu] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CPause::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// �W���C�p�b�h�̎擾
	CSound *pSound = CManager::GetSound();						// �T�E���h�̏����擾

	// �I������
	Select();

	if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_7) || pKeyboard->GetTrigger(DIK_P))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_CANCEL);
		m_nNumSelect = TYPE_CONTINUE;
	}

	if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_10) || pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_18) || pKeyboard->GetTrigger(DIK_W) || pKeyboard->GetTrigger(DIK_UP))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_BOTTON000);
		m_nNumSelect = (m_nNumSelect + (TYPE_MAX - 1)) % TYPE_MAX;	// �I����Ԃ��P�グ��

		for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
		{// �I�𒆂Ƒ��ŐF�𕪂���
			if (nCntPolygon == m_nNumSelect)
			{// �I��
				m_apPolygon[nCntPolygon]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{// ��
				m_apPolygon[nCntPolygon]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
			}
		}
	}

	if (pJoypad->GetPress(0, CInputJoypad::DIJS_BUTTON_11) || pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_19) || pKeyboard->GetTrigger(DIK_S) || pKeyboard->GetTrigger(DIK_DOWN))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_BOTTON000);
		m_nNumSelect = (m_nNumSelect + 1) % TYPE_MAX;	// �I����Ԃ��P������

		for (int nCntPolygon = 0; nCntPolygon < TYPE_MAX; nCntPolygon++)
		{// �I�𒆂Ƒ��ŐF�𕪂���
			if (nCntPolygon == m_nNumSelect)
			{// �I��
				m_apPolygon[nCntPolygon]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{// ��
				m_apPolygon[nCntPolygon]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPause::Draw(void)
{
	for (int nCntPolygon = TYPE_MAX; nCntPolygon >= 0; nCntPolygon--)
	{// �z��̑傫��������`�悷��
		m_apPolygon[nCntPolygon]->Draw();
	}

}

//=============================================================================
// �I������
//=============================================================================
void CPause::Select(void)
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CInputJoypad *pJoypad = CManager::GetInputJoypad();			// �W���C�p�b�h�̎擾
	CSound *pSound = CManager::GetSound();						// �T�E���h�̏����擾

	bool bPause = CGame::GetPause();

	if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_1) || pKeyboard->GetTrigger(DIK_RETURN))
	{
		pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);

		// �|�[�Y��ʂ��甲����
		bPause = false;
		CGame::SetPause(bPause);

		// �I�𒆂̍���
		switch (m_nNumSelect)
		{
		case TYPE_CONTINUE:
			break;

		case TYPE_RETRY:
				// ��ʑJ��
				CFade::Create(CManager::MODE_GAME);

			break;

		case TYPE_QUIT:
				// ��ʑJ��
				CFade::Create(CManager::MODE_TITLE);

			break;
		}
	}
}

