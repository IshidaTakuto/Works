//=============================================================================
//
// 3D�I�u�W�F�N�g���� [scene3D.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "scene.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene3D : public CScene
{
public:
	CScene3D(int nPriority = 3, CScene::OBJTYPE objType = CScene::OBJTYPE_NONE);
	~CScene3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	const void SetRot(D3DXVECTOR3 rot);

	D3DXVECTOR3 GetLength(void) { return m_length; };
	const void SetLength(D3DXVECTOR3 length);

	const void SetColor(D3DXCOLOR col);
	const void AddColor(D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; };

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	const void SetTexPos(D3DXVECTOR2 *texPos);
	void MoveTexPos(D3DXVECTOR2 speed);

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;				// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X

	D3DXVECTOR3				m_pos;					// �|���S���̈ʒu
	D3DXVECTOR3				m_rot;					// ��]
	D3DXVECTOR3				m_length;				// ���_����̋���
	D3DXCOLOR				m_col;					// �F
	int						m_nLife;				// ����
};

#endif