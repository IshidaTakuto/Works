//=============================================================================
//
// �X�e�[�W�Ǘ����� [stage.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "stage.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"
#include "scene2D.h"
#include "meshField.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "logo.h"
#include "modelSet.h"
#include "camera.h"

#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define STAGE_FADE			(0.02f)		// �t�F�[�h�̓����x�̑����l

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//=========================================
// ��������
//=========================================
CStage *CStage::Create(int nStage)
{
	CStage *pStage = NULL;

	// �������m��
	pStage = new CStage();

	if (pStage != NULL)
	{// �������m�ې���
		pStage->m_nStage = nStage;
		pStage->Init();
	}

	return pStage;
}

//=========================================
// �R���X�g���N�^
//=========================================
CStage::CStage(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_nStage = 0;
	m_fade = FADE_NONE;
}

//=========================================
// �f�X�g���N�^
//=========================================
CStage::~CStage()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CStage::Init(void)
{
	m_fade = FADE_OUT;	// �t�F�[�h�A�E�g���

	// ����������
	CScene2D::Init();

	// �ʒu�̐ݒ�
	CScene2D::SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �傫���̐ݒ�
	CScene2D::SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	if (0 == m_nStage)
	{// �����X�e�[�W
		CScene2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{// �Q�X�e�[�W�ڈȍ~
		CScene2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CStage::Uninit(void)
{
	// �I�u�W�F�N�g��j��
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CStage::Update(void)
{
	bool bDelete = false;

	// �F�̎擾
	D3DXCOLOR col = CScene2D::GetColor();

	if (m_fade != FADE_NONE)
	{// �t�F�[�h�̏�Ԃ��������Ă��Ȃ��Ƃ��̓X���[
		if (m_fade == FADE_IN)		// �t�F�[�h�C�����
		{
			col.a -= STAGE_FADE;	// ��ʂ𓧖��ɂ��Ă���

			if (col.a <= 0.0f)
			{// �����ɂȂ���
				col.a = 0.0f;
				m_fade = FADE_NONE;	// �������Ă��Ȃ����

				bDelete = true;		// ���S�t���O�𗧂Ă�
			}
		}
		else if (m_fade == FADE_OUT)
		{
			col.a += STAGE_FADE;	// ��ʂ�s�����ɂ��Ă���

			if (col.a >= 1.0f)
			{// �s�����ɂȂ���
				col.a = 1.0f;
				Stage();			// �X�e�[�W�ړ�
				m_fade = FADE_IN;	// �t�F�[�h�C����Ԃ�
			}
		}
	}

	// �F�̐ݒ�
	CScene2D::SetColor(col);

	if (bDelete)
	{// �j������
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CStage::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �X�e�[�W�ړ�
//=============================================================================
void CStage::Stage(void)
{
	// �X�e�[�W�ړ��p�j������
	CScene::StageUninit();

	// ���݂̃X�e�[�W��ݒ�
	CGame::SetStage(m_nStage);

	// �n�ʂ̐���
	CMeshField::Create(m_nStage);

	CEnemy::Unload();
	CModelSetManager::Unload();
	CModelSetManager::Load(m_nStage);

	// �v���C���[�I�u�W�F�N�g��T��
	CScene *pScene = CScene::GetSceneTop(PLAYER_PRIORITY);

	while (pScene != NULL)
	{// NULL�ɂȂ�܂Ń��[�v
		CScene *pSceneNext = pScene->GetSceneNext();		// ���̃I�u�W�F�N�g��ۊ�
		CScene::OBJTYPE objType = pScene->GetObjType();		// �I�u�W�F�N�g�̎�ނ��擾

		if (objType == CScene::OBJTYPE_PLAYER)
		{// �v���C���[�������Ƃ�
			CPlayer *pPlayer = (CPlayer*)pScene;
			D3DXVECTOR3 posPlayer = pPlayer->GetPos();

			pPlayer->SetPos(CModelSetManager::GetStartPos());
			if (pPlayer->GetSlipFlag())
			{// ������Ԃ̂Ƃ�
				pPlayer->SetSlipFlag(false);
			}

			// �J�����̐ݒ�
			D3DXVECTOR3 posV = CManager::GetCamera()->GetPosV();
			posV.y = posPlayer.y + 80.0f;
			CManager::GetCamera()->SetPosV(posV);

			D3DXVECTOR3 posR = CManager::GetCamera()->GetPosR();
			posR.y = posPlayer.y + 80.0f;
			CManager::GetCamera()->SetPosR(posR);

			break;
		}

		// ���̃I�u�W�F�N�g������
		pScene = pSceneNext;
	}
}
