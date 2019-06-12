//=============================================================================
//
// GUI�Ǘ����� [GUIManager.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _GUIMANAGER_H_
#define _GUIMANAGER_H_

#include "main.h"
#include "ImGui/imgui.h"
#include "modelObject.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGuiManager
{
public:
	CGuiManager();
	~CGuiManager();
	HRESULT Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGuiManager *Create(void);

	static CModelObjectManager::TYPE GetObjectType(void) { return m_type; };		// �ݒu�I�u�W�F�N�g�̎�ނ̎擾
	static void SetObjectType(CModelObjectManager::TYPE type) { m_type = type; };	// �ݒu�I�u�W�F�N�g�̎�ނ̐ݒ�

	static int GetType(void) { return m_nType; };						// ���f���̎�ނ̎擾
	static float GetRange(void) { return m_fRange; };					// �I��͈͂̎擾
	static float GetHeight(void) { return m_fHeight; };					// �グ�鍂���̎擾
	static float GetLength(int nArray) { return m_aLength[nArray]; };	// �����̎擾
	static void SetLength(float nLengthX, float nLengthZ);				// �����̐ݒ�

	static int GetSplit(int nArray) { return m_aSplit[nArray]; };		// �������̎擾
	static void SetSplit(int nSplitX, int nSplitZ);						// �������̐ݒ�

	void SetPresent(HRESULT result, LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS d3dpp);

private:
	void LoadFont(void);									// �S�Ă̓��{����g����悤�ɂ���
	void SpaceLine(bool bPartition = false, int nLine = 1);	// ���s

	static CModelObjectManager::TYPE m_type;	// �ݒu�I�u�W�F�N�g�̎��
	static int	m_nType;						// ���f���̎��
	static float m_fRange;						// �I��͈�
	static float m_fHeight;						// �グ�鍂��
	static float m_aLength[2];					// ����
	static int m_aSplit[2];						// ������

	char **m_aModelFileName[CModelObjectManager::TYPE_MAX];

	bool m_showDemoWindow;		// �f���E�C���h�E�̕\��
	bool m_showFieldWindow;		// �n�ʃE�C���h�E�̕\��
	ImVec4 m_clearColor;		// �F
	bool m_bRepeat;				// �{�^���������t���O

	ImWchar m_glyphRangesJapanese[8253];
};

#endif
