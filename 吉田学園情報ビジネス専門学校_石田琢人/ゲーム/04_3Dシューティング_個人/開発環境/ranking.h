//=============================================================================
//
// �����L���O���� [ranking.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"
#include "field.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RANKING_DIGITS		(8)				// ����
#define MAX_RANKING			(5)				// �����L���O�̌�
#define NUM_POLYGON			(1)				// �|���S���̌�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPolygon;
class CNumber;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRanking
{
public:
	CRanking();
	~CRanking();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void Set(int nScore);

private:
	CField		*m_pField[MAX_FIELD];			// �t�B�[���h�N���X�̃|�C���^�ϐ�
	CPolygon	*m_apPolygon[NUM_POLYGON];		// �|���S���N���X�̃|�C���^�ϐ�
	CNumber		*m_apNumber[MAX_RANKING][RANKING_DIGITS];		// �i���o�[�N���X�̃|�C���^�ϐ�
	static int	m_nRanking[MAX_RANKING];		// �����L���O�X�R�A
	static int	m_nScore;						// �l�������X�R�A
	D3DXVECTOR3	m_aDestPos[MAX_RANKING][RANKING_DIGITS];		// �X�R�A�̕\���ʒu
	int			m_nCntFade;						// �^�C�g���փt�F�[�h����܂ł̎���
	int			m_nCntHighScore;				// �X�R�A�_�ŗp�J�E���^
	bool		m_bArrive[MAX_RANKING];			// �ړI�̈ʒu�ɂ������ǂ���
};

#endif
