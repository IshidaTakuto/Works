//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
#include "modelObject.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GRAVITY				(0.5f)			// �d��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CInput;
class CRenderer;
class CInputKeyboard;
class CCamera;
class CLight;
class CDebugProc;
class CSetPlayer;
class CMeshField;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
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
	static CSetPlayer *GetSetPlayer(void) { return m_pSetPlayer; };		// �ݒu�p�v���C���[�̎擾
	static CMeshField *GetMeshField(void) { return m_pMeshField; };		// ���b�V���t�B�[���h�̎擾

	static int GetNumStage(void) { return m_nNumStage; };
	static char *GetFieldFileName(int nField) { return m_pFieldFileName[nField]; };
	static char *GetSetFileName(int nStage) { return m_pSetFileName[nStage]; };

	HRESULT Load(void);													// �V�X�e���X�N���v�g��ǂݍ���

private:
	int SetNumType(void);

	static CRenderer		*m_pRenderer;	// �����_�����O�N���X�̃|�C���^�ϐ�
	static CInputKeyboard	*m_pInput;		// �C���v�b�g�N���X�̃|�C���^�ϐ�
	static CCamera			*m_pCamera;		// �J�����N���X�̃|�C���^�ϐ�
	static CLight			*m_pLight;		// ���C�g�N���X�̃|�C���^�ϐ�
	static CDebugProc		*m_pDebugProc;	// �f�o�b�O�����N���X�̃|�C���^�ϐ�

	static CSetPlayer *m_pSetPlayer;		// �ݒu�p�v���C���[�N���X�̃|�C���^�ϐ�
	static CMeshField *m_pMeshField;		// ���b�V���t�B�[���h�N���X�̃|�C���^�ϐ�

	char	m_aTxtFileName[64];			// �ǂݍ��ރe�L�X�g�t�@�C����
	int		m_nNumEnemyType;			// �G�̎��
	CModelObjectManager::TYPE	m_type;	// �ݒu�I�u�W�F�N�g�̎��

	static int				m_nNumStage;			// �X�e�[�W��
	static char				**m_pFieldFileName;		// �n�ʃX�N���v�g��
	static char				**m_pSetFileName;		// �z�u�X�N���v�g��
};

#endif