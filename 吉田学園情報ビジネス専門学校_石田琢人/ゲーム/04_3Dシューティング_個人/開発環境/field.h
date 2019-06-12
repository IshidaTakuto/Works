//=============================================================================
//
// �t�B�[���h���� [field.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "scene3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FIELD_TEXTURE	(3)			// �e�N�X�`����
#define FIELD_PRIORITY	(2)			// �����̗D��ԍ�
#define MAX_FIELD		(4)			// �t�B�[���h�̐�
#define FIELD_WIDTH		(3000.0f)	// ��̒n�ʂ̐i�s�����̒���
#define FIELD_DEPTH		(8000.0f)	// �n�ʂ̉��s

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CWall;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CField : public CScene3D
{
public:
	//+++++++++++++++++++++
	// �t�B�[���h�̎��
	//+++++++++++++++++++++
	typedef enum
	{
		TYPE_GROUND,	// �n��
		TYPE_SEA,		// �C
		TYPE_MAX
	}TYPE;

	CField(int nPriority = FIELD_PRIORITY);
	~CField();
	HRESULT Init(TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CField* Create(TYPE type);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ���
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

	bool Collision(D3DXVECTOR3 *pos, float fVtxMinY);
	TYPE GetType(void) { return m_type; };

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[FIELD_TEXTURE];	// ���L�e�N�X�`���ւ̃|�C���^
	TYPE m_type;				// ���
	static int m_nCntPos;		// �ʒu�̃J�E���^
	CScene3D *m_pScene3D;		// 3D�I�u�W�F�N�g�̃|�C���^�ϐ�
	CWall *m_pWall;				// �ǂւ̃|�C���^�ϐ�
};

#endif
