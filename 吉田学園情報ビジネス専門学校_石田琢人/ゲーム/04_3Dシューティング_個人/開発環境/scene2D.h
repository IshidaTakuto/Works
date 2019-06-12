//=============================================================================
//
// 2D�I�u�W�F�N�g���� [scene2D.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "scene.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority = 3);
	~CScene2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(D3DXVECTOR3 pos, float vtxMaxX);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos);

	const void SetColor(D3DXCOLOR col);
	const void AddColor(D3DXCOLOR col);

	float GetLength(void) { return m_fLength; };
	const void SetLength(float fLength);

	const void SetVtxPos(D3DXVECTOR3 *vtxPos);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	const void SetTexPos(D3DXVECTOR2 *tex);

	int GetLife(void) { return m_nLife; };
	const void SetLife(int nLife) { m_nLife = nLife; };
	const int SubtractLife(int nDamage);

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;				// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3				m_pos;					// �|���S���̈ʒu
	//float					m_fCntRot;				// �p�x�̍X�V
	//float					m_fAngle;				// ���_�̊p�x
	float					m_fLength;				// ���_�̋���
	int						m_nLife;				// ����
};

#endif