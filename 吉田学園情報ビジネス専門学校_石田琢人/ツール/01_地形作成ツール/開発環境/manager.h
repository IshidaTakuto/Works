//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

class CInput;
class CRenderer;
class CInputKeyboard;
class CCamera;
class CLight;
class CDebugProc;
class CPlayer;
class CMeshField;
class CCylinder;

//=====================================
// �N���X��`
//=====================================
class CManager
{
public:
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRenderer *GetRenderer(void) { return m_pRenderer; };		// �����_���[�̎擾
	static CInputKeyboard *GetInput(void) { return m_pInput; };			// �C���v�b�g�̎擾
	static CCamera *GetCamera(void) { return m_pCamera; };				// �J�����̎擾
	static CDebugProc *GetDebugProc(void) { return m_pDebugProc; };		// �f�o�b�O�����̎擾
	static CPlayer *GetPlayer(void) { return m_pPlayer; };				// �v���C���[�̎擾
	static CMeshField *GetMeshField(void) { return m_pMeshField; };		// ���b�V���t�B�[���h�̎擾
	static CCylinder *GetCylinder(void) { return m_pCylinder; };		// ���b�V���V�����_�[�̎擾

private:
	static CRenderer		*m_pRenderer;	// �����_�����O�N���X�̃|�C���^�ϐ�
	static CInputKeyboard	*m_pInput;		// �C���v�b�g�N���X�̃|�C���^�ϐ�
	static CCamera			*m_pCamera;		// �J�����N���X�̃|�C���^�ϐ�
	static CLight			*m_pLight;		// ���C�g�N���X�̃|�C���^�ϐ�
	static CDebugProc		*m_pDebugProc;	// �f�o�b�O�����N���X�̃|�C���^�ϐ�

	static CPlayer *m_pPlayer;				// �v���C���[�N���X�̃|�C���^�ϐ�
	static CMeshField *m_pMeshField;		// ���b�V���t�B�[���h�N���X�̃|�C���^�ϐ�
	static CCylinder *m_pCylinder;			// ���b�V���V�����_�[�N���X�̃|�C���^�ϐ�
};

#endif