//=============================================================================
//
// �̗͏��� [life.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "life.h"
#include "polygon.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BAR_WIDTH_LIFE	(300)		// ���C�t�o�[�̕�
#define BAR_HEIGHT_LIFE	(40)		// ���C�t�o�[�̍���

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// ��������
//=============================================================================
CLife* CLife::Create(void)
{
	CLife *pLife;

	pLife = new CLife;	// ���������m��

	pLife->m_pPolygon[0] = CPolygon::Create(1, D3DXVECTOR3(10.0f, 630.0f, 0.0f), BAR_WIDTH_LIFE, BAR_HEIGHT_LIFE);		// �o�[�̒���
	pLife->m_pPolygon[1] = CPolygon::Create(1, D3DXVECTOR3(10.0f, 630.0f, 0.0f), BAR_WIDTH_LIFE, BAR_HEIGHT_LIFE);		// ���݂̃��C�t
	pLife->m_pPolygon[2] = CPolygon::Create(1, D3DXVECTOR3(10.0f, 630.0f, 0.0f), BAR_WIDTH_LIFE, BAR_HEIGHT_LIFE);		// ���݂̃��C�t�Ǝ󂯂��_���[�W�̍�

	pLife->Init();

	return pLife;
}

//=========================================
// �R���X�g���N�^
//=========================================
CLife::CLife(int nPriority) : CScene(nPriority)
{
	for (int nCntLife = 0; nCntLife < MAX_POLYGON_LIFE; nCntLife++)
	{// �Q�[�W�̃|���S��
		m_pPolygon[nCntLife] = NULL;
	}

	m_nLife = 0;			// ���C�t�̒l
	m_nCntSubtraction = 0;	// �_���[�W�o�[�����炵�n�߂�܂ł̃J�E���^
}

//=========================================
// �f�X�g���N�^
//=========================================
CLife::~CLife()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLife::Init(void)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_UI);

	m_nLife = MAX_LIFE;

	m_pPolygon[0]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));		// �ő�l��\���o�[
	m_pPolygon[1]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));		// ���̃o�[
	m_pPolygon[2]->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));		// �O�̃o�[

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLife::Uninit(void)
{
	// �e�N�X�`���̊J��
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_LIFE; nCntPolygon++)
	{
		if (m_pPolygon[nCntPolygon] != NULL)
		{
			m_pPolygon[nCntPolygon]->Uninit();
			delete m_pPolygon[nCntPolygon];
			m_pPolygon[nCntPolygon] = NULL;
		}
	}

	// ������j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLife::Update(void)
{
	float fLife = m_pPolygon[1]->GetWidth();		// �_���[�W�o�[�̎擾
	float fLifeGreen = m_pPolygon[2]->GetWidth();	// �_���[�W�o�[�̎擾

	if (fLife > fLifeGreen)
	{// ���C�t�Ɠ����l�ɂȂ�܂Ō��̃Q�[�W�����炷
		m_nCntSubtraction++;					// �_���[�W�o�[�����炵�n�߂邽�߂̃J�E���g

		if (m_nCntSubtraction >= 25)
		{// ��莞�Ԍo��
			fLife -= BAR_WIDTH_LIFE * 0.01f;	// �_���[�W�o�[�̒�����1�������炷

			m_pPolygon[1]->SetWidth(fLife);		// �_���[�W�o�[�̒�����ݒ�
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLife::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_LIFE; nCntPolygon++)
	{// �|���S���̕`��
		m_pPolygon[nCntPolygon]->Draw();
	}
}

//=============================================================================
// �̗͂̑���
//=============================================================================
const void CLife::AddSubtract(int nValue)
{
	m_nLife += nValue;		// ���C�t�𑝌�������
	float fLife;			// �o�[�̒����̒l

	if (m_nLife > 0)
	{// ���C�t��0���傫���Ƃ�
		fLife = (BAR_WIDTH_LIFE * 1.0f) / ((MAX_LIFE * 1.0f) / m_nLife);	// �o�[�̌��̒�����100%�Ƃ��ă��C�t�𑝌�������
	}
	else
	{// ���C�t��0�̂Ƃ�
		fLife = 0;
	}

	if (m_nLife >= MAX_LIFE)
	{// �ő�l�𒴂�����߂�
		m_nLife = MAX_LIFE;
		fLife = BAR_WIDTH_LIFE;
	}

	// �o�[�̒������X�V
	m_pPolygon[2]->SetWidth(fLife);

	if (nValue > 0)
	{// �񕜂����Ƃ����̃o�[�����₷
		m_pPolygon[1]->SetWidth(fLife);			// �o�[�̒������X�V
	}
	m_nCntSubtraction = 0;						// �_���[�W�o�[�����炵�n�߂邽�߂̃J�E���^��������
}
