//=============================================================================
//
// �Q�[�W���� [gauge.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_POLYGON_GAUGE	(2)		// �|���S���̍ő吔
#define MAX_GAUGE	(100)			// �Q�[�W�̏��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPolygon;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGauge : public CScene
{
public:
	CGauge(int nPriority = 6);
	~CGauge();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGauge* Create(void);

	static const void AddSubtract(int nValue);	// ���C�t�𑝌�������
	static int Get(void) { return m_nGauge; };	// ���C�t�̎擾
	static void Set(int nGauge) { m_nGauge = nGauge; };

private:
	static CPolygon *m_pPolygon[MAX_POLYGON_GAUGE];	// �|���S���N���X�̃|�C���^�ϐ�
	static int m_nGauge;						// ���݂̃Q�[�W�̒l
};

#endif
