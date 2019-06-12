//=============================================================================
//
// �����\������ [string.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _STRING_H_
#define _STRING_H_

#include "main.h"
#include <string.h>		// ������ɕK�v

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_FONT	(2)		// ������t�H���g�̐�
#define MAX_STRING	(32)	// ������̍ő吔

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CString
{
public:
	CString();
	~CString();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);	// �����t�H���g��ǂݍ���
	static void Unload(void);	// �����t�H���g���J��

	static CString *Create(void);		// �`�揈���ŌĂ�
	bool Set(char *str, RECT rect, D3DXCOLOR col, int nDispTime, int nFont = 0);

private:
	static LPD3DXFONT	m_pFont[NUM_FONT];		// �t�H���g�ւ̃|�C���^
	int m_nCntString;							// ������\������܂ł̎���
	char *m_pStringOld;							// ��������L��
};

#endif