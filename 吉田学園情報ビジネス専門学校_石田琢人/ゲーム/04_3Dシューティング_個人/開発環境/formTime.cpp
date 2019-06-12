//=============================================================================
//
// �ό`�\���ۏ��� [formTime.h]
// Author : Ishida Takuto
//
//=============================================================================
#include "formTime.h"
#include "polygon.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BAR_WIDTH_FORMTIME	(200)
#define BAR_HEIGHT_FORMTIME	(20)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// ��������
//=============================================================================
CFormTime* CFormTime::Create(void)
{
	// ���������m��
	CFormTime *pFormTime = new CFormTime;;

	// �o�[�S�̂̒���
	pFormTime->m_pPolygon[0] = CPolygon::Create(1, D3DXVECTOR3(10.0f, 700.0f, 0.0f), BAR_WIDTH_FORMTIME, BAR_HEIGHT_FORMTIME);

	// �l�������o�[�̒���
	pFormTime->m_pPolygon[1] = CPolygon::Create(1, D3DXVECTOR3(10.0f, 700.0f, 0.0f), 0, BAR_HEIGHT_FORMTIME);

	// ����������
	pFormTime->Init();

	return pFormTime;
}

//=========================================
// �R���X�g���N�^
//=========================================
CFormTime::CFormTime(int nPriority) : CScene(nPriority)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_GAUGE_FORMTIME; nCntPolygon++)
	{// �Q�[�W�̃|���S��
		m_pPolygon[nCntPolygon] = NULL;
	}

	m_nFormTime = 0;		// �ό`�\�Q�[�W�̒l
	m_CntFormTime = 0;		// �Q�[�W�����܂�n�߂�܂ł̃J�E���^
	m_bSuccess = false;		// �ό`�ł��邩�ǂ����̃t���O
}

//=========================================
// �f�X�g���N�^
//=========================================
CFormTime::~CFormTime()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFormTime::Init(void)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_UI);

	m_pPolygon[0]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));		// �o�[�S�̂̐F
	m_pPolygon[1]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.0f, 1.0f));		// �l�������o�[�̐F
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CFormTime::Uninit(void)
{
	// �e�N�X�`���̊J��
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_GAUGE_FORMTIME; nCntPolygon++)
	{
		if (m_pPolygon[nCntPolygon] != NULL)
		{// �g���Ă�����
			m_pPolygon[nCntPolygon]->Uninit();
			delete m_pPolygon[nCntPolygon];
			m_pPolygon[nCntPolygon] = NULL;
		}
	}

	// �����̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CFormTime::Update(void)
{
	// �ό`�\�Q�[�W�����܂�n�߂�܂ł̃J�E���g
	m_CntFormTime++;

	if (m_CntFormTime > 60)
	{
		if (m_nFormTime < MAX_GAUGE_FORMTIME)
		{// ���̒l�܂ő��₷
			m_nFormTime++;
		}
	}

	// �Q�[�W�����݂̒l�Ɠ����ɂȂ�܂ő���
	float fLife = (BAR_WIDTH_FORMTIME * 1.0f) / ((MAX_GAUGE_FORMTIME * 1.0f) / m_nFormTime);
	m_pPolygon[1]->SetWidth(fLife);				// �o�[�̒�����ݒ�

	if (fLife >= BAR_WIDTH_FORMTIME)
	{// �Q�[�W���ő�l�ɒB�����Ƃ�
		m_bSuccess = true;
		m_pPolygon[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));		// �o�[�̐F
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CFormTime::Draw(void)
{
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON_GAUGE_FORMTIME; nCntPolygon++)
	{// �|���S���̕`��
		m_pPolygon[nCntPolygon]->Draw();
	}
}

//=============================================================================
// �ό`������̐��l���Z�b�g
//=============================================================================
void CFormTime::FormTimeReset(void)
{
	m_bSuccess = false;
	m_CntFormTime = 0;
	m_nFormTime = 0;
	m_pPolygon[1]->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.0f, 1.0f));		// �o�[�̐F
}