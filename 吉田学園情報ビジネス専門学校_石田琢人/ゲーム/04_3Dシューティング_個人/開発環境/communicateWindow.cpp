//=============================================================================
//
// �Q�[�W���� [gauge.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "communicateWindow.h"
#include "polygon.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define COMMUNICATEWINDOW_TEXTURENAME	"data/TEXTURE/communicate_window.jpg"
#define COMMUNICATEWINDOW_WIDTH			(600)
#define COMMUNICATEWINDOW_HEIGHT		(60)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CPolygon *CCommunicateWindow::m_apPolygon[MAX_POLYGON_COMMUNICATE] = {};

//=============================================================================
// ��������
//=============================================================================
CCommunicateWindow* CCommunicateWindow::Create(void)
{
	CCommunicateWindow *pCommunicateWindow;

	pCommunicateWindow = new CCommunicateWindow;	// ���������m��

	pCommunicateWindow->m_apPolygon[0] = CPolygon::Create(0, D3DXVECTOR3(COMMUNICATEWINDOW_WIDTH + 10.0f, COMMUNICATEWINDOW_HEIGHT + 10.0f, 0.0f), 0, 0, CPolygon::TYPE_COMMUNICATEWINDOW);	// �ʐM�E�B���h�E
	pCommunicateWindow->m_apPolygon[1] = CPolygon::Create(0, D3DXVECTOR3(COMMUNICATEWINDOW_WIDTH + 10.0f, COMMUNICATEWINDOW_HEIGHT + 10.0f, 0.0f), 0, 0);	// �����|���S��

	pCommunicateWindow->Init();		// ����������

	return pCommunicateWindow;
}

//=========================================
// �R���X�g���N�^
//=========================================
CCommunicateWindow::CCommunicateWindow()
{
	m_nCntTransparent = 0;
	m_bCompleted = false;
}

//=========================================
// �f�X�g���N�^
//=========================================
CCommunicateWindow::~CCommunicateWindow()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCommunicateWindow::Init(void)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_UI);

	m_apPolygon[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// �ʐM�E�B���h�E
	m_apPolygon[1]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));		// �����|���S��
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCommunicateWindow::Uninit(void)
{
	// �|���S���N���X�̊J��
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_COMMUNICATE; nCntPolygon++)
	{
		if (m_apPolygon[nCntPolygon] != NULL)
		{
			m_apPolygon[nCntPolygon]->Uninit();
			delete m_apPolygon[nCntPolygon];
			m_apPolygon[nCntPolygon] = NULL;
		}
	}

	Release();	// �J��
}

//=============================================================================
// �X�V����
//=============================================================================
void CCommunicateWindow::Update(void)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_COMMUNICATE; nCntPolygon++)
	{
		float fWidth = m_apPolygon[nCntPolygon]->GetWidth();		// �����擾
		float fHeight = m_apPolygon[nCntPolygon]->GetHeight();		// �������擾

		if (fWidth < COMMUNICATEWINDOW_WIDTH)
		{// ���̒l��傫������
			fWidth += (COMMUNICATEWINDOW_WIDTH * 0.01f) * 4;			// ���̒l��傫������
		}
		if (fHeight < COMMUNICATEWINDOW_HEIGHT)
		{// �����̒l��傫������
			fHeight += (COMMUNICATEWINDOW_HEIGHT * 0.01f) * 4;			// �����̒l��傫������
		}

		// �傫�����X�V����
		m_apPolygon[nCntPolygon]->SetWidth(fWidth);
		m_apPolygon[nCntPolygon]->SetHeight(fHeight);

		if (fWidth >= COMMUNICATEWINDOW_WIDTH && fHeight >= COMMUNICATEWINDOW_HEIGHT)
		{// �傫�����K��l�ɒB�����Ƃ�
			D3DXCOLOR col = m_apPolygon[1]->GetColor();		// �F�̎擾
			col.a -= 0.02f;		// �A���t�@�l�����炷

			if (col.a > 0)
			{// �����x��0���傫���Ƃ�
				m_apPolygon[1]->SetColor(col);	// �O�ʂ̃|���S���𓧖���������
			}
			else
			{
				m_bCompleted = true;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCommunicateWindow::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_COMMUNICATE; nCntPolygon++)
	{// �|���S���̕`��
		m_apPolygon[nCntPolygon]->Draw();
	}
}
