//=============================================================================
//
// �������� [number.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CNumber
{
public:
	CNumber();
	~CNumber();
	HRESULT Init(D3DXVECTOR3 pos, float fLength);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CNumber* Create(D3DXVECTOR3 pos, float fLength);

	static HRESULT Load(void);				// �e�N�X�`����ǂݍ���
	static void Unload(void);				// �ǂݍ��񂾃e�N�X�`�������

	const void SetNumber(int nNumber);

	const void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void) { return m_pos; };

	const void SetColor(D3DXCOLOR col);
	const void SetLength(float fLength);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// ���L�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3 m_pos;			// ���W
	float m_fLength;			// �|���S���̒���
	int m_nCountAnim;			// �A�j���[�V�����̃J�E���^
	int m_nPatternAnim;			// �A�j���[�V�����̃p�^�[����
};

#endif