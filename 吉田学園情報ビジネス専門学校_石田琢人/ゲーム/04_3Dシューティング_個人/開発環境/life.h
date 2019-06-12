//=============================================================================
//
// �̗͏��� [life.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_POLYGON_LIFE	(3)		// �|���S���̍ő吔
#define MAX_LIFE			(100)	// ���C�t�̏��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPolygon;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CLife : public CScene
{
public:
	CLife(int nPriority = 6);
	~CLife();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLife* Create(void);

	const void AddSubtract(int nValue);		// ���C�t�𑝌�������
	int GetLife(void) { return m_nLife; };	// ���C�t�̎擾

private:
	CPolygon *m_pPolygon[MAX_POLYGON_LIFE];	// �|���S���N���X�̃|�C���^�ϐ�
	int m_nLife;							// ���݂̃��C�t�̒l(�őO�̗΃o�[)
	int m_nCntSubtraction;					// �_���[�W�o�[�����炵�n�߂�܂ł̃J�E���^
};

#endif
