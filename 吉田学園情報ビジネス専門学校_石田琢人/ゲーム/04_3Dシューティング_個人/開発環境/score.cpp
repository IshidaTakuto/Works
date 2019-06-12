//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "score.h"
#include "number.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCORE_SIZE	(40)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CNumber *CScore::m_pNumber[SCORE_DIGITS] = {};
int CScore::m_nScore = 0;

//=============================================================================
// ��������
//=============================================================================
CScore* CScore::Create(D3DXVECTOR3 pos)
{
	CScore *pScore;

	pScore = new CScore;	// ���������m��
	for (int nCntScore = 0; nCntScore < SCORE_DIGITS; nCntScore++)
	{
		pScore->m_pNumber[nCntScore] = CNumber::Create(D3DXVECTOR3((pos.x - (nCntScore * SCORE_SIZE)) + (SCORE_SIZE * (SCORE_DIGITS / 2.0f)) - SCORE_SIZE, pos.y, 0.0f), SCORE_SIZE);
	}

	pScore->Init();

	return pScore;
}

//=========================================
// �R���X�g���N�^
//=========================================
CScore::CScore(int nPriority) : CScene(nPriority)
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CScore::~CScore()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScore::Init(void)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_UI);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < SCORE_DIGITS; nCntScore++)
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
void CScore::Update(void)
{
	for (int nCntScore = 0; nCntScore < SCORE_DIGITS; nCntScore++)
	{
		m_pNumber[nCntScore]->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntScore = 0; nCntScore < SCORE_DIGITS; nCntScore++)
	{// �i���o�[�|���S���̕`��
		m_pNumber[nCntScore]->Draw();
	}
}

//=============================================================================
// �X�R�A�̉��Z
//=============================================================================
const void CScore::AddScore(int nValue)
{
	int aNumber;	// �v�Z���ʂ��i�[

	m_nScore += nValue;			// �X�R�A�̉��Z

	for (int nCntDigits = 0; nCntDigits < SCORE_DIGITS; nCntDigits++)
	{// ���ʂɊi�[
		aNumber = m_nScore % (int)powf(10.0f, (nCntDigits + 1) * 1.0f) / (int)powf(10.0f, nCntDigits * 1.0f);
		m_pNumber[nCntDigits]->SetNumber(aNumber);	// �X�R�A�̍X�V
	}
}
