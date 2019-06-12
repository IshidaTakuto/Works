//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "scene3DBill.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EFFECT_PRIORITY		(4)		// �G�t�F�N�g�̗D�揇��
#define EFFECT_NUMTEXTURE	(2)		// �e�N�X�`���̐�

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEffect : public CScene3DBill
{
public:
	typedef enum
	{
		TYPE_NORMAL,	// �ʏ�
		TYPE_RIPPLES,	// �g��
		TYPE_BIIM,		// �r�[��
		TYPE_MAX
	}TYPE;

	CEffect(int nPriority = EFFECT_PRIORITY);
	virtual ~CEffect();
	virtual HRESULT Init(D3DXVECTOR3 pos);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static CEffect* Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRange, int nLife, TYPE type = TYPE_NORMAL);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ���
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[EFFECT_NUMTEXTURE];	// ���L�e�N�X�`���ւ̃|�C���^
	TYPE m_type;							// ���
};

#endif