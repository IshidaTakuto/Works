//=============================================================================
//
// �C���f�b�N�X�o�b�t�@3D�I�u�W�F�N�g���� [scene3DIndex.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCENE3DINDEX_H_
#define _SCENE3DINDEX_H_

#include "scene.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene3DIndex : public CScene
{
public:
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_FIELD,			// �n��
		TYPE_WALL,			// ��
		TYPE_CYLINDER,		// �V�����_�[
		TYPE_MAX
	}TYPE;

	CScene3DIndex(int nPriority = 3);
	~CScene3DIndex();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(D3DXVECTOR3 *pos, float fRadius);

	void MakeVertex(int meshX, int meshY, int meshZ, int nLengthX, int nLengthY, int nLengthZ, TYPE type);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;				// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;				// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X

	D3DXVECTOR3				m_pos;					// �|���S���̈ʒu
	D3DXVECTOR3				m_rot;					// ��]
	int						m_nLengthX;				// ���_��X���̋���
	int						m_nLengthY;				// ���_��Y���̋���
	int						m_nLengthZ;				// ���_��Z���̋���
	int						m_nMeshX;				// X���̕�����
	int						m_nMeshY;				// Y���̕�����
	int						m_nMeshZ;				// Z���̕�����
	int						m_nNumVtx;				// ���_��
	int						m_nNumPolygon;			// �|���S����
	TYPE					m_type;					// �|���S���̎��
};

#endif