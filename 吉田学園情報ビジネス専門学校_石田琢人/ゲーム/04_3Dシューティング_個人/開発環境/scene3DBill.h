//=============================================================================
//
// 3D�r���{�[�h�I�u�W�F�N�g���� [scene3DBill.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCENE3DBILL_H_
#define _SCENE3DBILL_H_

#include "scene.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene3DBill : public CScene
{
public:
	CScene3DBill(int nPriority = 3);
	~CScene3DBill();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(D3DXVECTOR3 pos, float fRange);
	bool CollisionXY(D3DXVECTOR3 pos, float fRange);
	bool CollisionXZ(D3DXVECTOR3 pos, float fRange);
	bool CollisionY(float posY, float fRange);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos);

	const void SetColor(D3DXCOLOR col);
	const void AddColor(D3DXCOLOR col);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	int GetLife(void) { return m_nLife; };
	const void SetLife(int nLife) { m_nLife = nLife; };
	const int SubtractLife(int nDamage);

	const void SetLength(float fLength0, float fLength1);
	float GetLengthX(void) { return m_fLengthX; };
	float GetLengthY(void) { return m_fLengthY; };

	const void SetTexPos(D3DXVECTOR2 *tex);
	const void SetVtxPos(D3DXVECTOR3 *vtxPos);

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;				// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X

	D3DXVECTOR3				m_pos;					// �|���S���̈ʒu
	D3DXVECTOR3				m_rot;					// ��]
	float					m_fLengthX;				// ���_��X���̋���
	float					m_fLengthY;				// ���_��Y���̋���
	float					m_fLengthZ;				// ���_��Z���̋���

	int						m_nLife;				// ����
};

#endif