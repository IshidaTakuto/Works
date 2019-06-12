//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "camera.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "camera.h"
#include "debugProc.h"
#include "result.h"
#include "sound.h"
#include "life.h"
#include "modelSet.h"
#include "player.h"
#include "enemy.h"
#include "meshField.h"
#include "modelManager.h"
#include "motion.h"
#include "bg.h"
#include "pause.h"
#include "stage.h"
#include "timer.h"
#include "number.h"
#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CGame::STATE CGame::m_state = CGame::STATE_NONE;
int CGame::m_nCurStage = 0;
CBg *CGame::m_pMountain = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGame::CGame(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_modeCounter = 0;

	for (int nCntBg = 0; nCntBg < GAME_NUM_BG; nCntBg++)
	{// �w�i�̏�����
		m_pBg[nCntBg] = NULL;
	}
	m_pPause = NULL;
	m_bPause = false;
	m_nCurStage = 0;
	m_pTimer = NULL;
	m_pTutorial = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGame::Init(void)
{
	// �ǂݍ���
	CModelManager::Load();
	CMotionManager::Load();
	CBg::Load();
	CLifeManager::Load();
	CWeed::Load();
	CNumber::Load();
	CPause::Load();

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_GAME);

	// �X�e�[�W�ړ��N���X
	CStage::Create(m_nCurStage);

	// �^�C�}�[�̐���
	m_pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 10.0f, 0.0f), 0);

	// ��������̐���
	m_pTutorial = CLogoTutorial::Create(D3DXVECTOR3(1100.0f, 80.0f, 0.0f), 300.0f, 150.0f);

	// �w�i�̐���
	CBg::Create(0, false);
	m_pBg[0] = CBg::Create(1, true);
	m_pBg[1] = CBg::Create(1, true);
	m_pMountain = CBg::Create(2, false);

	// ���_���̎擾
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = m_pBg[1]->GetVtxBuff();

	// ���_����ݒ�
	VERTEX_2D *pVtx;	// ���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W���X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	vtxBuff->Unlock();

	// �|���S���̈ʒu��ݒ�
	m_pBg[1]->SetVtxBuff(vtxBuff);

	// ���_���̎擾
	vtxBuff = m_pMountain->GetVtxBuff();

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W���X�V
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	vtxBuff->Unlock();

	// �|���S���̈ʒu��ݒ�
	m_pMountain->SetVtxBuff(vtxBuff);

	// �v���C���[�̐���
	CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGame::Uninit(void)
{
	// �f�[�^�̔j��
	CModelManager::Unload();
	CMotionManager::Unload();
	CBg::Unload();
	CEnemy::Unload();
	CModelSetManager::Unload();
	CLifeManager::Unload();
	CWeed::Unload();
	CNumber::Unload();
	CPause::Unload();

	for (int nCntBg = 0; nCntBg < GAME_NUM_BG; nCntBg++)
	{
		if (m_pBg[nCntBg] != NULL)
		{// �w�i�̔j��
			m_pBg[nCntBg]->Uninit();
			m_pBg[nCntBg] = NULL;
		}
	}

	if (NULL != m_pTutorial)
	{// ��������\���N���X�̔j��
		m_pTutorial->Uninit();
		m_pTutorial = NULL;
	}

	if (NULL != m_pMountain)
	{// �w�i�̔j��
		m_pMountain->Uninit();
		m_pMountain = NULL;
	}

	// �I�u�W�F�N�g��j��
	CScene::Release();
}

//=============================================================================
// �Q�[���X�V����
//=============================================================================
void CGame::Update(void)
{
	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CXInput			*pXInput = CManager::GetXInput();			// XInput�̎擾
	CSound *pSound = CManager::GetSound();						// �T�E���h�̎擾

#ifdef _DEBUG
	if (CDebugProc::GetDisp())
	{
		if (pKeyboard->GetTrigger(DIK_1))
		{
			CStage::Create(0);
		}
		if (pKeyboard->GetTrigger(DIK_2))
		{
			CStage::Create(1);
		}
		if (pKeyboard->GetTrigger(DIK_3))
		{
			CGame::SetGameState(CGame::STATE_CLEAR);
		}
	}
#endif

	if (m_pBg[0] != NULL)
	{// �e�N�X�`�����W�𓮂���
		m_pBg[0]->TexMove(0.0001f);
	}

	if (m_pBg[1] != NULL)
	{// �e�N�X�`�����W�𓮂���
		m_pBg[1]->TexMove(0.0003f);
	}

	// �t�F�[�h�̎擾
	CFade::FADE fade = CFade::GetFade();

	if (fade == CFade::FADE_NONE)
	{// �t�F�[�h���̓|�[�Y�ɂł��Ȃ�����
		if (pKeyboard->GetTrigger(DIK_P) || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_4))
		{// �|�[�YON/OFF
			m_bPause = m_bPause ? false : true;

			if (m_bPause)
			{// �|�[�Y�ɂȂ����Ƃ�
				if (m_pPause == NULL)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_PAUSEON);
					m_pPause = CPause::Create();	// �|�[�Y�N���X�𐶐�
				}
			}
		}

		if(!m_bPause)
		{// �|�[�Y�����Ƃ�
			if (m_pPause != NULL)
			{// �|�[�Y�łȂ��Ȃ�����j��
				m_pPause->Uninit();
				m_pPause = NULL;
				pSound->PlaySound(CSound::SOUND_LABEL_PAUSEOFF);
			}
		}
	}

	switch (m_state)
	{
	case STATE_NORMAL:
		break;

	case STATE_END:
		m_modeCounter++;
		if (m_modeCounter >= 60)
		{
			m_state = STATE_NONE;

			// ���Ԃ̎擾
			CResult::SetTime(m_pTimer->GetTime());

			// ���(���[�h)�̐ݒ�
			CFade::Create(CManager::MODE_GAMEOVER);
		}
		break;

	case STATE_CLEAR:
		m_modeCounter++;
		if (m_modeCounter >= 60)
		{
			m_state = STATE_NONE;

			// ���Ԃ̎擾
			CResult::SetTime(m_pTimer->GetTime());

			// ���(���[�h)�̐ݒ�
			CFade::Create(CManager::MODE_RESULT);
		}
		break;
	}

	CDebugProc::Print("�Q�[��");
	CDebugProc::Print("�X�e�[�W%d", m_nCurStage + 1);
}

//=============================================================================
// �Q�[���`�揈��
//=============================================================================
void CGame::Draw(void)
{
}

//=============================================================================
// �Q�[���̏�Ԃ̐ݒ�
//=============================================================================
const void CGame::SetGameState(STATE state)
{
	m_state = state;
}

//=============================================================================
// �X�e�[�W�̐ݒ�
//=============================================================================
void CGame::SetStage(int nStage)
{
	// ���݂̃X�e�[�W��ݒ�
	m_nCurStage = nStage;
}

//=============================================================================
// ���̃X�e�[�W��
//=============================================================================
void CGame::NextStage(void)
{
	// �X�e�[�W���J�E���g�A�b�v
	m_nCurStage++;

	if (CManager::GetNumStage() > m_nCurStage)
	{// �X�e�[�W�̐���
		CStage::Create(m_nCurStage);
	}
	else
	{// �Q�[���N���A
		SetGameState(STATE_CLEAR);
	}
}

//=============================================================================
// �R�̃e�N�X�`���𓮂���
//=============================================================================
void CGame::MoveMountain(float fMove)
{
	if (m_pMountain != NULL)
	{// �e�N�X�`�����W�𓮂���
		m_pMountain->TexMove(fMove * 0.00002f);
	}
}