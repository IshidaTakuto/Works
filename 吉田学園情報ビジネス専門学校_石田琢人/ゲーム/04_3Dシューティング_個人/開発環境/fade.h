//=============================================================================
//
// �t�F�[�h���� [fade.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "scene2D.h"
#include "manager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CFade : public CScene2D
{
public:
	// �t�F�[�h�̏��
	typedef enum
	{
		FADE_NONE = 0,		// �������Ă��Ȃ����
		FADE_IN,			// �t�F�[�h�C�����
		FADE_OUT,			// �t�F�[�h�A�E�g���
		FADE_MAX
	} FADE;

	CFade(int nPriority = 7);
	~CFade();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CFade *Create(CManager::MODE modeNext);

	static FADE GetFade(void) { return m_fade; };

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// ���L�e�N�X�`���ւ̃|�C���^
	static FADE				m_fade;			// �t�F�[�h���
	static CManager::MODE	m_modeNext;		// ���̃��[�h
	static D3DXCOLOR		m_colFade;		// �t�F�[�h�F
};

#endif
