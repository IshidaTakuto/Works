//=============================================================================
//
// �w�i���� [bg.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BG		(3)		// �w�i�̍ő吔
#define BG_PRIORITY	(1)		// �����̗D��ԍ�

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBg : public CScene2D
{
public:
	CBg(CScene::OBJTYPE objType = CScene::OBJTYPE_BG);
	~CBg();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBg* Create(int nTexture, bool bAdd, int nPriority = BG_PRIORITY);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ���
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

	void TexMove(float fMove);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_BG];	// ���L�e�N�X�`���ւ̃|�C���^
	bool m_bAdd;									// ���Z�������ǂ���
	static int m_nPriority;							// �D��ԍ�
};

#endif
