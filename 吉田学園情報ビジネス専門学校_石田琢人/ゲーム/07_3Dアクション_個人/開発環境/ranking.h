//=============================================================================
//
// �����L���O���� [ranking.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_RANKING			(5)					// �����L���O�̌�
#define RANKING_MAX_TEXTURE	(MAX_RANKING + 1)	// �e�N�X�`���̐�
#define RANKING_NUM_BG		(2)					// �����L���O�̔w�i�̐�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;
class CNumber;
class CLogo;
class CLogoPressButton;
class CBg;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRanking : public CScene
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
	LPDIRECT3DTEXTURE9 m_apTexture[RANKING_MAX_TEXTURE];	// �e�N�X�`���ւ̃|�C���^�ϐ�

	CScene2D			*m_apPolygon[MAX_RANKING];			// 2D�|���S���N���X�̃|�C���^�ϐ�
	CNumber				**m_apNumber[MAX_RANKING];			// �i���o�[�N���X�̃|�C���^�ϐ�
	static int			m_nRanking[MAX_RANKING];			// �����L���O�X�R�A
	static int			m_nScore;							// �l�������X�R�A
	CLogo				*m_pLogo;							// ���S�N���X�̃|�C���^�ϐ�
	CLogoPressButton	*m_pPress;							// ���͑҂��N���X�̃|�C���^�ϐ�
	int					m_nCntFade;							// �^�C�g���փt�F�[�h����܂ł̎���
	int					m_nCntHighScore;					// �X�R�A�_�ŗp�J�E���^
	int					m_nDigits[MAX_RANKING];				// �X�R�A�̌���
	CBg					*m_pBg[RANKING_NUM_BG];				// �w�i�̃|�C���^�ϐ�
	CBg					*m_pMountain;						// �w�i�̎R
	int					m_nFlash;							// �_�ł����鑬�x
};

#endif
