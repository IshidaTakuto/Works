//=============================================================================
//
// �C���f�b�N�X3D�I�u�W�F�N�g���� [scene3DIndex.h]
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
		TYPE_FIELD = 0,
		TYPE_CYLINDER,
		TYPE_MAX
	}TYPE;

	CScene3DIndex(int nPriority = 3);
	~CScene3DIndex();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void MakeVertex(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	float GetHeight(D3DXVECTOR3 pos);
	void SetHeight(D3DXVECTOR3 pos, float fValue, float fRange);		// ���_�̍����̐ݒ�

	void SaveVtx(void);					// ���_�����t�@�C���ɏo�͂���
	void LoadVtx(void);					// ���_�����t�@�C������ǂݍ���

	void Save(void);						// �t�@�C���ɏo�͂���
	void Load(void);						// �t�@�C������ǂݍ���

	int GetSplitX(void) { return m_nSplitX; };		// X�̕������̎擾
	void SetSplitX(int nValue);						// X�̕������̐ݒ�

	int GetSplitZ(void) { return m_nSplitZ; };		// Z�̕������̎擾
	void SetSplitZ(int nValue);						// Z�̕������̐ݒ�

	float GetLengthX(void) { return m_fLengthX; };	// X�̒����̎擾
	void SetLengthX(float fValue);					// X�̒����̐ݒ�

	float GetLengthZ(void) { return m_fLengthZ; };	// Z�̒����̎擾
	void SetLengthZ(float fValue);					// Z�̒����̐ݒ�

	int GetNumVtx(void) { return m_nNumVtx; };		// ���_���̎擾

	void SetVtx(void);						// ���_���̐ݒ�
	void SetNor(VERTEX_3D *pVtx);			// �@���̌v�Z

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; };
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 pVtxBuff) { m_pVtxBuff = pVtxBuff; };

	void GetInfo(float *pWidth, float *pHeight, float *pDepth, int *pMeshX, int *pMeshY, int *pMeshZ);

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;				// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;				// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X

	D3DXVECTOR3				m_pos;					// �|���S���̈ʒu
	D3DXVECTOR3				m_rot;					// ��]
	float					m_fLengthX;				// ���_��X���̋���
	float					m_fLengthY;				// ���_��Y���̋���
	float					m_fLengthZ;				// ���_��Z���̋���
	int						m_nSplitX;				// X���̕�����
	int						m_nSplitY;				// Y���̕�����
	int						m_nSplitZ;				// Z���̕�����
	int						m_nNumVtx;				// ���_��
	int						m_nNumPolygon;			// �|���S����

	char					m_aFileName[64];		// ���_���̃t�@�C����
	char					m_aTexFileName[64];		// �e�N�X�`����
};

#endif