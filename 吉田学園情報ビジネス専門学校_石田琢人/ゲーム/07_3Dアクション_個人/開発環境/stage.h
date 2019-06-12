//=============================================================================
//
// �X�e�[�W�Ǘ����� [stage.h]
// Author : Takuto Ishida
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "scene.h"
#include "logo.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define STAGE_PRIORITY			(7)		// �X�e�[�W�Ǘ��̗D��ԍ�
#define STAGELOGO_PRIORITY		(6)		// �X�e�[�W���S�̗D��ԍ�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CStage : public CScene2D
{
public:
	typedef enum
	{// �t�F�[�h�̏��
		FADE_NONE = 0,		// �������Ă��Ȃ����
		FADE_IN,			// �t�F�[�h�C�����
		FADE_OUT,			// �t�F�[�h�A�E�g���
		FADE_MAX
	} FADE;

	static CStage *Create(int nStage);

	CStage(int nPriority = STAGE_PRIORITY, CScene::OBJTYPE objType = CScene::OBJTYPE_STAGE);
	~CStage();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void Stage(void);	// �X�e�[�W����ւ��̏���

	FADE m_fade;		// �t�F�[�h�̏��
	int m_nStage;		// ���݂̃X�e�[�W
};

#endif