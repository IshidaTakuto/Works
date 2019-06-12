//=============================================================================
//
// 3D���f���I�u�W�F�N�g���� [scene3DModel.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCENE3DMODEL_H_
#define _SCENE3DMODEL_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PARTS	(2)		// �p�[�c��

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene3DModel : public CScene
{
public:
	CScene3DModel(int nPriority = 3);
	~CScene3DModel();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; };
	const void SetPos(D3DXVECTOR3 pos);

	const void SetColor(D3DXCOLOR col);
	const void AddColor(D3DXCOLOR col);

	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange);
	bool CollisionAttack(D3DXVECTOR3 pos, float fRange);

	int GetLife(void) { return m_nLife; };
	const void SetLife(int nLife) { m_nLife = nLife; };
	const int SubtractLife(int nDamage);

	float GetLength(void) { return m_vtxMax.x; };
	float GetMinVtxY(void) { return m_vtxMin.y; };

	D3DXVECTOR3 GetRot(void) { return m_rot; };
	const void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; };

	const void CScene3DModel::BindInfo(LPDIRECT3DTEXTURE9 pTexture, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat);

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH				m_pMesh;				// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER			m_pBuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD					m_nNumMat;			// �}�e���A�����̐�

	D3DXMATERIAL			*m_pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3				m_vtxMin, m_vtxMax;		// ���f���̍ŏ��l�A�ő�l

	D3DXVECTOR3				m_pos;				// �ʒu
	D3DXVECTOR3				m_rot;				// ��]
	D3DXVECTOR3				m_posModel;				// ���f���̈ʒu
	D3DXVECTOR3				m_rotModel;				// ���f���̉�]
	int						m_nIdxShadow;					// �e�̔ԍ�
	int						m_nLife;						// ����
};

#endif