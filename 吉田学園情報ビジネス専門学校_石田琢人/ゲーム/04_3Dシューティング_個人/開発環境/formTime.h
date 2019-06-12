//=============================================================================
//
// �ό`�\���ۏ��� [formTime.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _FORMTIME_H_
#define _FORMTIME_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_POLYGON_GAUGE_FORMTIME	(2)		// �|���S���̍ő吔
#define MAX_GAUGE_FORMTIME	(150)			// �Q�[�W�̏��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPolygon;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CFormTime : public CScene
{
public:
	CFormTime(int nPriority = 6);
	~CFormTime();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CFormTime* Create(void);

	bool GetFormSuccess(void) { return m_bSuccess; };	// �ό`�\�t���O�̎擾
	void FormTimeReset(void);							// �l�̃��Z�b�g

private:
	CPolygon *m_pPolygon[MAX_POLYGON_GAUGE_FORMTIME];	// �|���S���N���X�̃|�C���^�ϐ�
	int m_nFormTime;									// �ό`�\�Q�[�W�̒l
	bool m_bSuccess;									// �ό`�ł��邩�ǂ����̃t���O
	int m_CntFormTime;									// �Q�[�W�����܂�n�߂�܂ł̃J�E���^
};

#endif
