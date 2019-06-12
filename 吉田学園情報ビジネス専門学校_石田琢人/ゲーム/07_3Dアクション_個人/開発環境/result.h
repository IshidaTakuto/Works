//=============================================================================
//
// ���U���g���� [result.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RESULT_MAX_TEXTURE	(2)		// �e�N�X�`���̐�
#define RESULT_NUM_BG		(2)		// �w�i�̐�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;
class CScene2D;
class CLogo;
class CLogoPressButton;
class CBg;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CResult : public CScene
{
public:
	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetTime(int nTime) { m_nTime = nTime; };

private:
	static int	m_nTime;			// ����
	int			m_nTimeDigit;		// ���Ԃ̌�
	int			m_nCntResult;		// ���U���g�̎���
	int			m_nFlash;			// �_�ł����鑬�x

	LPDIRECT3DTEXTURE9 m_pTexture[RESULT_MAX_TEXTURE];	// �e�N�X�`���ւ̃|�C���^�ϐ�

	CNumber				**m_pNumber;			// �����N���X�̃|�C���^�ϐ�
	CScene2D			*m_pPolygon;			// 2D�|���S���N���X�̃|�C���^�ϐ�
	CLogo				*m_pLogo;				// ���S�N���X�̃|�C���^�ϐ�
	CLogoPressButton	*m_pPressButton;		// ���͑҂��\���N���X�̃|�C���^�ϐ�
	CBg					*m_pBg[RESULT_NUM_BG];	// �w�i�̃|�C���^�ϐ�
	CBg					*m_pMountain;			// �w�i�̎R
};

#endif