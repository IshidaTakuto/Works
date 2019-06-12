//=============================================================================
//
// �J�������� [camera.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCamera
{
public:
	typedef enum
	{
		TYPE_PLANE,		// �퓬�@�p
		TYPE_SHIP,		// ��͗p
		TYPE_TANK,		// ��ԗp
		TYPE_MAX
	}TYPE;

	CCamera();
	~CCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Set(void);

	D3DXMATRIX GetProjection(void) { return m_mtxProjection; };
	D3DXMATRIX GetView(void) { return m_mtxView; };
	D3DXVECTOR3 GetPosV(void) { return m_posV; };
	D3DXVECTOR3 GetPosR(void) { return m_posR; };
	float GetAngle(void) { return m_fAngle; };

	void SetPosV(D3DXVECTOR3 posV) { m_posV = posV; };

private:
	D3DXVECTOR3 m_posV;				// ���_
	D3DXVECTOR3 m_posR;				// �����_
	D3DXVECTOR3 m_vecU;				// ������x�N�g��
	D3DXMATRIX	m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X	�i�e���r�j
	D3DXMATRIX	m_mtxView;			// �r���[�}�g���b�N�X			�i�J�����}���j
	float		m_fLength;			// �����_�Ƃ̋���
	float		m_fAngle;			// �p�x
};

#endif
