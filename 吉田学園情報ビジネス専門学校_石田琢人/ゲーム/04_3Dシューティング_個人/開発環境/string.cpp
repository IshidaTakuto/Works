//=============================================================================
//
// �����\������ [string.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "string.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPD3DXFONT CString::m_pFont[NUM_FONT] = {};

//=============================================================================
// �����t�H���g�̐ݒ�
//=============================================================================
HRESULT CString::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ������t�H���g�̏�����
	D3DXCreateFont(pDevice,
		27,                     // ��������
		14,                     // ������
		FW_NORMAL,              // �t�H���g�X�^�C��(����)
		NULL,                   // �~�b�v�}�b�v���f���̐�
		FALSE,                  // �Α̂ɂ��邩�ǂ���
		SHIFTJIS_CHARSET,       // �����Z�b�g(����)
		OUT_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		"tahoma",               //�t�H���g�̎��
		&m_pFont[0]);

	D3DXCreateFont(pDevice,
		18,                     // ��������
		8,                     // ������
		FW_NORMAL,              // �t�H���g�X�^�C��(����)
		NULL,                   // �~�b�v�}�b�v���f���̐�
		FALSE,                  // �Α̂ɂ��邩�ǂ���
		SHIFTJIS_CHARSET,       // �����Z�b�g(����)
		OUT_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		"tahoma",               //�t�H���g�̎��
		&m_pFont[1]);

	return S_OK;
}

//=============================================================================
// �����t�H���g�̊J��
//=============================================================================
void CString::Unload(void)
{
	for (int nCntString = 0; nCntString < NUM_FONT; nCntString++)
	{// �J��
		if (NULL != m_pFont[nCntString])
		{
			m_pFont[nCntString]->Release();
			m_pFont[nCntString] = NULL;
		}
	}
}

//=============================================================================
// ������̐���
//=============================================================================
CString *CString::Create(void)
{
	CString *pString = NULL;

	pString = new CString;

	if (NULL != pString)
	{
		pString->Init();
	}

	return pString;
}

//=============================================================================
// ������̐ݒ�
//=============================================================================
bool CString::Set(char *str, RECT rect,D3DXCOLOR col, int nDispTime, int nFont)
{
	bool bComp = false;

	if (m_pStringOld != str)
	{// �O��̕�����ƈ������J�E���^��������
		m_nCntString = 0;
	}

	m_pStringOld = str;	// ��������L��

	RECT rectShadow{ rect.left + 1, rect.top + 1, rect.right, rect.bottom };

	if (nDispTime < 0)
	{// �ꕶ���̕\���Ԋu�����̐����ƈꊇ�\���ɂ���
		// �e������T�C�Y���v�Z
		m_pFont[nFont]->DrawText(
			NULL,
			&str[0],		// �\�����镶����
			-1,	// �\���T�C�Y(-1�őS��)
			&rectShadow,			// �\���͈�
			DT_CALCRECT,	// �\���͈͂ɒ���
			NULL);			// �F

		// �����̉e
		m_pFont[nFont]->DrawText(
			NULL,
			&str[0],							// �\�����镶����
			-1,									// �\���T�C�Y(-1�őS��)
			&rectShadow,						// �\���͈�
			DT_LEFT | DT_BOTTOM,				// ���l�߂ŗ��[����
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// �F

		// ������T�C�Y���v�Z
		m_pFont[nFont]->DrawText(
			NULL,
			&str[0],		// �\�����镶����
			-1,	// �\���T�C�Y(-1�őS��)
			&rect,			// �\���͈�
			DT_CALCRECT,	// �\���͈͂ɒ���
			NULL);			// �F

		// ���̃T�C�Y�Ń����_�����O
		m_pFont[nFont]->DrawText(
			NULL,
			&str[0],				// �\�����镶����
			-1,						//�\���T�C�Y(-1�őS��)
			&rect,					//�\���͈�
			DT_LEFT | DT_BOTTOM,	//���l�߂ŗ��[����
			col);					//�F

		// �����񂪑S�ĕ`�悳�ꂽ
		bComp = true;
	}
	else
	{// �w�肵���\���Ԋu�ŕ������\��
		int nStringDisp = 0;
		int nStrLength = strlen(str);

		nStringDisp = m_nCntString / nDispTime;		// ���t���[���o�߂ŕ����𑝂₷

		if (str[nStringDisp] == '\0')
		{
			// �e������T�C�Y���v�Z
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],		// �\�����镶����
				-1,	// �\���T�C�Y(-1�őS��)
				&rectShadow,			// �\���͈�
				DT_CALCRECT,	// �\���͈͂ɒ���
				NULL);			// �F

			// �����̉e
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],							// �\�����镶����
				-1,						// �\���T�C�Y(-1�őS��)
				&rectShadow,						// �\���͈�
				DT_LEFT | DT_BOTTOM,				// ���l�߂ŗ��[����
				D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// �F

			// ������T�C�Y���v�Z
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],		// �\�����镶����
				-1,	// �\���T�C�Y(-1�őS��)
				&rect,			// �\���͈�
				DT_CALCRECT,	// �\���͈͂ɒ���
				NULL);			// �F

			//���̃T�C�Y�Ń����_�����O
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],				// �\�����镶����
				-1,						// �\���T�C�Y(-1�őS��)
				&rect,					// �\���͈�
				DT_LEFT | DT_BOTTOM,	// ���l�߂ŗ��[����
				col);					// �F

			// �����񂪑S�ĕ`�悳�ꂽ
			bComp = true;
		}
		else if (nStringDisp < nStrLength)
		{
			if (m_nCntString % nDispTime == 0 && m_nCntString != 0)
			{// 2�o�C�g���\��������
				nStringDisp++;
				m_nCntString += nDispTime;
			}

			// ������\������܂ł̃J�E���^
			m_nCntString++;

			// �e������T�C�Y���v�Z
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],		// �\�����镶����
				nStringDisp,	// �\���T�C�Y(-1�őS��)
				&rectShadow,	// �\���͈�
				DT_CALCRECT,	// �\���͈͂ɒ���
				NULL);			// �F

								// �����̉e
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],							// �\�����镶����
				nStringDisp,						// �\���T�C�Y(-1�őS��)
				&rectShadow,						// �\���͈�
				DT_LEFT | DT_BOTTOM,				// ���l�߂ŗ��[����
				D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	// �F

			//������T�C�Y���v�Z
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],		// �\�����镶����
				nStringDisp,	// �\���T�C�Y(-1�őS��)
				&rect,			// �\���͈�
				DT_CALCRECT,	// �\���͈͂ɒ���
				NULL);			// �F

								//���̃T�C�Y�Ń����_�����O
			m_pFont[nFont]->DrawText(
				NULL,
				&str[0],				// �\�����镶����
				nStringDisp,			//�\���T�C�Y(-1�őS��)
				&rect,					//�\���͈�
				DT_LEFT | DT_BOTTOM,	//���l�߂ŗ��[����
				col);					//�F
		}
	}

	// ���͂̏����擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	if (pJoypad->GetTrigger(0, CInputJoypad::DIJS_BUTTON_1) || pKeyboard->GetTrigger(DIK_RETURN))
	{
		m_nCntString = strlen(str) * nDispTime;
	}

	return bComp;
}

//=========================================
// �R���X�g���N�^
//=========================================
CString::CString()
{
	m_nCntString = 0;
	m_pStringOld = NULL;
}

//=========================================
// �f�X�g���N�^
//=========================================
CString::~CString()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CString::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CString::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void CString::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CString::Draw(void)
{
}