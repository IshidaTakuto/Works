//=============================================================================
//
// ���U���g���� [result.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "field.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RESULT_POLYGON		(2)		// �g���|���S���̐�
#define RESULT_SCORE_DIGITS	(8)		// �X�R�A�̍ő包��
#define RESULT_KILL_DIGITS	(3)		// �|�����G�̍ő包��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPolygon;
class CNumber;
class CScene3DModel;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CResult
{
public:
	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetScore(int nScore) { m_nScore = nScore; };
	static void AddKillEnemy(void) { m_nNumKill++; };

private:
	CPolygon *m_apPolygon[RESULT_POLYGON];		// �|���S���N���X�̃|�C���^�ϐ�
	CNumber *m_apNumScore[RESULT_SCORE_DIGITS];	// �i���o�[�N���X�̃|�C���^�ϐ�
	CNumber *m_apNumKill[RESULT_KILL_DIGITS];	// �i���o�[�N���X�̃|�C���^�ϐ�

	CField	*m_pField[MAX_FIELD];	// �t�B�[���h�N���X�̃|�C���^�ϐ�
	CScene3DModel *m_model;			// ���f���N���X�̃|�C���^�ϐ�

	static int	m_nScore;			// �X�R�A
	static int	m_nNumKill;			// �|�����G�̐�
	int			m_nCntResult;		// ���U���g�̎���
	int			m_nScoreDigit;		// �X�R�A�̌�
	int			m_nKillDigit;		// �L�����̌�
};

#endif