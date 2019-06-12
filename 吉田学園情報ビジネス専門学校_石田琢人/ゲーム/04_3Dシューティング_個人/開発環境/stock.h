//=============================================================================
//
// �c�@���� [stock.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _STOCK_H_
#define _STOCK_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_STOCK	(4)		// �c�@�̍ő吔
#define MAX_STOCK	(1)		// �\������|���S���̍ő吔

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPolygon;
class CNumber;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CStock : public CScene
{
public:
	CStock(int nPriority = 6);
	~CStock();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CStock* Create(void);

	static const void AddSubtract(int nValue);
	static int GetStock(void) { return m_nStock; };

private:
	static CPolygon *m_pPolygon[MAX_STOCK];		// �|���S���N���X�̃|�C���^�ϐ�
	static CNumber *m_pNumber;					// �i���o�[�N���X�̃|�C���^�ϐ�
	static int m_nStock;						// ���݂̎c�@
};


#endif
