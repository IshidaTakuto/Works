//=============================================================================
//
// �Q�[�W���� [gauge.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "gauge.h"
#include "polygon.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BAR_WIDTH	(200)
#define BAR_HEIGHT	(30)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CPolygon *CGauge::m_pPolygon[MAX_POLYGON_GAUGE] = {};
int CGauge::m_nGauge = 0;

//=============================================================================
// ��������
//=============================================================================
CGauge* CGauge::Create(void)
{
	CGauge *pGauge;

	pGauge = new CGauge;	// ���������m��

	pGauge->m_pPolygon[0] = CPolygon::Create(1, D3DXVECTOR3(10.0f, 670.0f, 0.0f), BAR_WIDTH, BAR_HEIGHT);	// �o�[�̒���
	pGauge->m_pPolygon[1] = CPolygon::Create(1, D3DXVECTOR3(10.0f, 670.0f, 0.0f), 0, BAR_HEIGHT);			// ���݂̃o�[�̒���

	pGauge->Init();

	return pGauge;
}

//=========================================
// �R���X�g���N�^
//=========================================
CGauge::CGauge(int nPriority) : CScene(nPriority)
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CGauge::~CGauge()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGauge::Init(void)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_UI);

	m_pPolygon[0]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));		// �o�[�̐F
	m_pPolygon[1]->SetColor(D3DXCOLOR(0.0f, 0.1f, 1.0f, 1.0f));		// �o�[�̐F

	m_nGauge = MAX_GAUGE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGauge::Uninit(void)
{
	// �e�N�X�`���̊J��
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_GAUGE; nCntPolygon++)
	{
		if (m_pPolygon[nCntPolygon] != NULL)
		{
			m_pPolygon[nCntPolygon]->Uninit();
			delete m_pPolygon[nCntPolygon];
			m_pPolygon[nCntPolygon] = NULL;
		}
	}

	// �g���I��������̂�0������
	if (0 != m_nGauge)
	{
		m_nGauge = 0;
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGauge::Update(void)
{
	float fLife = m_pPolygon[1]->GetWidth();	// �_���[�W�o�[�̎擾
	float fGaugeView = fLife * (1 / (BAR_WIDTH * 0.01f));	// ���ɂ���

	if (m_nGauge < fGaugeView)
	{// �Q�[�W�����݂̒l�Ɠ����ɂȂ�܂Ō��炷
		fLife -= BAR_WIDTH * 0.01f;			// �Q�[�W�̒�����1�������炷
		m_pPolygon[1]->SetWidth(fLife);		// �_���[�W�o�[�̒�����ݒ�
	}
	else if (m_nGauge > fGaugeView)
	{// �Q�[�W�����݂̒l�Ɠ����ɂȂ�܂ő��₷
		fLife += BAR_WIDTH * 0.01f;			// �Q�[�W�̒�����1�������₷
		m_pPolygon[1]->SetWidth(fLife);		// �_���[�W�o�[�̒�����ݒ�
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGauge::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_GAUGE; nCntPolygon++)
	{// �|���S���̕`��
		m_pPolygon[nCntPolygon]->Draw();
	}
}

//=============================================================================
// �Q�[�W�̑���
//=============================================================================
const void CGauge::AddSubtract(int nValue)
{
  	m_nGauge += nValue;		// ���C�t�𑝌�������

	if (m_nGauge >= MAX_GAUGE)
	{// �ő�l�𒴂�����߂�
		m_nGauge = MAX_GAUGE;
	}
	else if(m_nGauge <= 0)
	{// 0���Ⴍ�Ȃ�Ȃ��悤�ɂ���
		m_nGauge = 0;
	}
}
