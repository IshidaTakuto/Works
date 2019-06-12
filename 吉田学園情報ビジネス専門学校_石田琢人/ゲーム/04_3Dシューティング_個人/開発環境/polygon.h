//=============================================================================
//
// �|���S������ [polygon.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPolygon
{
public:
	//**************************************
	// �e�N�X�`���̎��
	//**************************************
	typedef enum
	{
		TYPE_PRESS_ANY = 0,		// PRESS_ANY
		TYPE_CONTINUE,			// �|�[�Y���̃R���e�j���[
		TYPE_RETRY,				// �|�[�Y���̃��g���C
		TYPE_QUIT,				// �|�[�Y���̃^�C�g���ɖ߂�
		TYPE_TITLE_LOGO,		// �^�C�g���̃��S
		TYPE_RESULT_LOGO,		// ���U���g�̃��S
		TYPE_GAMEOVER_LOGO,		// �Q�[���I�[�o�[�̃��S
		TYPE_RANKING_LOGO,		// �����L���O�̃��S
		TYPE_COMMUNICATEWINDOW,	// �ʐM�E�B���h�E
		TYPE_STOCK00,			// �X�g�b�N�A�C�R���i�퓬�@�j
		TYPE_STOCK01,			// �X�g�b�N�A�C�R���i��́j
		TYPE_STOCK02,			// �X�g�b�N�A�C�R���i��ԁj
		TYPE_MAX
	}TYPE;

	CPolygon();
	~CPolygon();
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPolygon* Create(int nType, D3DXVECTOR3 pos, float fWidth, float fHeight, int nTexType = -1);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ���
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

	const void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; };

	const void SetColor(D3DXCOLOR col);
	const void AddColor(D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; };

	const void SetWidth(float fWidth);
	float GetWidth(void) { return m_fWidth; };

	const void SetHeight(float fHeight);
	float GetHeight(void) { return m_fHeight; };

	void Flashing(int nFlame);

	void TextureChange(TYPE texType) { m_nTexType = texType; };

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];		// ���L�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3 m_pos;									// ���W
	D3DXCOLOR m_col;									// �F
	float m_fWidth;										// �|���S���̂w��
	float m_fHeight;									// �|���S���̂x��
	int	m_nTexType;										// �e�N�X�`���ԍ�
	int m_nType;										// �����̎��
	int m_nCntColor;									// �F�̃J�E���^
};

#endif