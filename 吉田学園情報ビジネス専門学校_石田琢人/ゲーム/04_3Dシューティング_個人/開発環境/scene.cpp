//=============================================================================
//
// �I�u�W�F�N�g���� [scene.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "scene.h"
#include "bullet.h"
#include "player.h"
#include "enemy.h"
#include "field.h"
#include "model.h"
#include "number.h"
#include "polygon.h"

//==================================
// �ÓI�����o�ϐ��錾
//==================================
int CScene::m_nNumAll = 0;
CScene *CScene::m_apScene[NUM_PRIORITY][MAX_OBJECT] = {};

//==================================
// �|���S���̉������
//==================================
void CScene::Release(void)
{
	if (m_apScene[m_nPriority][m_nIndex] != NULL)
	{
		int nID;
		int nPriority;

		nID = m_nIndex;				// �ԍ����L��
		nPriority = m_nPriority;	// �D�揇�ʂ��L��

		//Uninit()	�v��Ȃ�
		delete m_apScene[nPriority][nID];	// m_nIndex��������				delete this�ł��ł��邯��NULL������Ȃ�
		m_apScene[nPriority][nID] = NULL;	// ���������NULL������

		m_nNumAll--;			// ���������炷
	}
}

//==================================
// �S�Ẵ|���S���̉������
//==================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				m_apScene[nCntPriority][nCntScene]->Uninit();
			}
		}
	}
}

//==================================
// �S�ẴI�u�W�F�N�g�̍X�V����
//==================================
void CScene::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			if (NULL != m_apScene[nCntPriority][nCntScene])
			{
				m_apScene[nCntPriority][nCntScene]->Update();
			}
		}
	}
}

//==================================
// �S�ẴI�u�W�F�N�g�̕`�揈��
//==================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
		{
			if (NULL != m_apScene[nCntPriority][nCntScene])
			{
				m_apScene[nCntPriority][nCntScene]->Draw();
			}
		}
	}
}

//=========================================
// �R���X�g���N�^
//=========================================
CScene::CScene(int nPriority)
{
	for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
	{
		if (NULL == m_apScene[nPriority][nCntScene])
		{
			m_apScene[nPriority][nCntScene] = this;	// �Ăяo�����{�l������
			m_nIndex = nCntScene;			// �������g�̔ԍ����w��
			m_nNumAll++;					// �����𑝂₷
			m_objType = OBJTYPE_NONE;		// �I�u�W�F�N�g�̎�ނɏ����l������
			m_nPriority = nPriority;		// �D�揇�ʂ̎w��
			break;
		}
	}
}

//=========================================
// �f�X�g���N�^
//=========================================
CScene::~CScene()
{
}
