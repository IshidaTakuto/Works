//=============================================================================
//
// �^�C�g������ [title.h]
// Author : shotaro kanamaru
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// �񋓑̂̒�`
//*****************************************************************************
typedef enum
{// �^�C�g���̎��
	TITLE_SCARS = 0,		// �܍�
	TITLE_ICON,				// �^�C�g���A�C�R��
	TITLE_CHAR,				// �^�C�g������
	TITLE_PRESS,			// �L�[���͂̑���
	TITLE_MAX,				// ���̑�
}TITLETYPE;

//*****************************************************************************
//�\���̂̒�`
//***************************************************************************** 
typedef struct
{// �^�C�g�����
	D3DXVECTOR3 pos,posInit;// ���݈ʒu�A�����ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR3 size;		// �傫��
	D3DXCOLOR	col;		// �F
	TITLETYPE	type;		// ���
}Title;

typedef struct
{// �z�u���
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 size;		// �傫��
	TITLETYPE	type;		// ���
}TitleInfo;

typedef struct
{// ���̑��ݒ���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		pTexture[TITLE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
	int						nNum;						// ����
	int						nCntAll;					// ���ʃJ�E���^
	int						nPatternAnim;				// �A�j���[�V�����p�^�[��
	float					fAnimWidth;					// �A�j���[�V�����̕�
	int						nCntChar;					// �^�C�g�����S�̃J�E���^
	int						nCntFlash;					// �t���b�V���̃J�E���^
	bool					bFlash;						// �t���b�V���̗L��
	bool					bPress;						// �L�[���͑����̎g�p
	bool					bEvent;						// ���o�̗L��
}TitleOther;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif
