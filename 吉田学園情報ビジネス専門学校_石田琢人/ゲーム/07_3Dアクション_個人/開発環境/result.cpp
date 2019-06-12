//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "result.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "ranking.h"
#include "camera.h"
#include "debugProc.h"
#include "number.h"
#include "logo.h"
#include "stage.h"
#include "modelSet.h"
#include "bg.h"
#include "ranking.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RESULT_LOGO00			"data/TEXTURE/result_logo00.png"
#define RESULT_LOGO01			"data/TEXTURE/result_logo01.png"
#define RESULT_LOGO_TIME		"data/TEXTURE/result_logo02.png"

#define RESULT_TIME				(420)			// �^�C�g����ʂ܂ł̎���
#define RESULT_NUMBER_SIZE		(200.0f)		// �����̑傫��
#define RESULT_LOGO_WIDTH		(600.0f)		// ���S�̕�
#define RESULT_LOGO_HEIGHT		(150.0f)		// ���S�̍���

#define RESULT_PRESS_WIDTH		(500.0f)		// ���͑҂��\���̕�
#define RESULT_PRESS_HEIGHT		(100.0f)		// ���͑҂��\���̍���
#define RESULT_BEFORE_FLASH		(60)			// �{�^������͂���O�̓_��
#define RESULT_AFTER_FLASH		(5)				// �{�^������͂�����̓_��

#define RESULT_MOVE_CAMERA		(1.0f)			// �J�����̓������x

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int CResult::m_nTime = 0;		// ����

//=========================================
// �R���X�g���N�^
//=========================================
CResult::CResult()
{
	m_nCntResult = 0;
	m_nTimeDigit = 0;
	m_nFlash = 0;
	m_pLogo = NULL;
	m_pPressButton = NULL;
	m_pNumber = NULL;
	m_pPolygon = NULL;

	for (int nCntTexture = 0; nCntTexture < RESULT_MAX_TEXTURE; nCntTexture++)
	{// �e�N�X�`���������[�v
		m_pTexture[nCntTexture] = NULL;
	}
}

//=========================================
// �f�X�g���N�^
//=========================================
CResult::~CResult()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init(void)
{
	// �e�N�X�`���̓ǂݍ���
	CBg::Load();
	CWeed::Load();
	CNumber::Load();

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_RESULT);

	// ���[�h�̎擾
	CManager::MODE mode = CManager::GetMode();

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	if (CManager::MODE_RESULT == mode) { D3DXCreateTextureFromFile(pDevice, RESULT_LOGO00, &m_pTexture[0]); }
	else if (CManager::MODE_GAMEOVER == mode) { D3DXCreateTextureFromFile(pDevice, RESULT_LOGO01, &m_pTexture[0]); }
	D3DXCreateTextureFromFile(pDevice, RESULT_LOGO_TIME, &m_pTexture[1]);

	// ���S�𐶐�
	m_pLogo = CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2,100.0f,0.0f), RESULT_LOGO_WIDTH, RESULT_LOGO_HEIGHT);
	m_pLogo->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F�̐ݒ�

	if (NULL != m_pLogo)
	{// �������m�ې���
		m_pLogo->BindTexture(m_pTexture[0]);
	}

	// ���͑҂��\���𐶐�
	m_pPressButton = CLogoPressButton::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), RESULT_PRESS_WIDTH, RESULT_PRESS_HEIGHT);
	m_pPressButton->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F�̐ݒ�

	m_nTimeDigit = (int)log10f((float)m_nTime) + 1;			// ���Ԃ̌���
	if (m_nTimeDigit == 0) { m_nTimeDigit = 1; }			// 0�������Ƃ�
	m_pNumber = new CNumber*[m_nTimeDigit];					// �����N���X�̐���
	for (int nCntDigit = 0; nCntDigit < m_nTimeDigit; nCntDigit++)
	{// ����
		m_pNumber[nCntDigit] = CNumber::Create(D3DXVECTOR3((700 + (RESULT_NUMBER_SIZE * m_nTimeDigit) - RESULT_NUMBER_SIZE) - (nCntDigit * (RESULT_NUMBER_SIZE * 0.5f)), 250, 0.0f), RESULT_NUMBER_SIZE, RESULT_NUMBER_SIZE);

		// �����̐ݒ�
		int aNumber = m_nTime % (int)powf(10.0f, (float)(nCntDigit + 1)) / (int)powf(10.0f, (float)nCntDigit);
		m_pNumber[nCntDigit]->SetNumber(aNumber);
	}

	// �|���S���̐���
	m_pPolygon = CScene2D::Create();

	if (NULL != m_pPolygon)
	{// �������m�ې���
		m_pPolygon->BindTexture(m_pTexture[1]);
		m_pPolygon->SetPos(D3DXVECTOR3(300.0f, 300.0f, 0.0f));
		m_pPolygon->SetSize(RESULT_NUMBER_SIZE, RESULT_NUMBER_SIZE / 2.0f);
	}

	// ���͑҂��\���̓_�ő��x
	m_nFlash = RESULT_BEFORE_FLASH;

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

	// �X�e�[�W�̐���
	CStage::Create(0);

	// �J�����̎擾
	CCamera *pCamera = CManager::GetCamera();

	// �J�����̏����ʒu��ݒ�
	pCamera->SetPosV(D3DXVECTOR3(-2800.0f, 80.0f, -500.0f));
	pCamera->SetPosR(D3DXVECTOR3(-2800.0f, 80.0f, 0.0f));

	if (CManager::MODE_RESULT == mode)
	{// �����L���O�ɃX�R�A��n��
		CRanking::Set(m_nTime);
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	// �e�N�X�`���̔j��
	CBg::Unload();
	CModelSetManager::Unload();
	CWeed::Unload();
	CNumber::Unload();

	for (int nCntTexture = 0; nCntTexture < RESULT_MAX_TEXTURE; nCntTexture++)
	{// �e�N�X�`���������[�v
		if (NULL != m_pTexture[nCntTexture])
		{// �e�N�X�`���̊J��
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}

	if (NULL != m_pLogo)
	{// ���S�̔j��
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	for (int nCntNumber = 0; nCntNumber < m_nTimeDigit; nCntNumber++)
	{// ���������[�v
		if (NULL != m_pNumber[nCntNumber])
		{// �����̔j��
			m_pNumber[nCntNumber]->Uninit();
			m_pNumber[nCntNumber] = NULL;

		}
	}

	if (NULL != m_pNumber)
	{// �����̔j��
		delete m_pNumber;
		m_pNumber = NULL;
	}

	if (NULL != m_pPolygon)
	{// �|���S���̔j��
		m_pPolygon->Uninit();
		m_pPolygon = NULL;
	}

	if (NULL != m_pPressButton)
	{// �|���S���̔j��
		m_pPressButton->Uninit();
		m_pPressButton = NULL;
	}

	for (int nCntBg = 0; nCntBg < RESULT_NUM_BG; nCntBg++)
	{
		if (NULL != m_pBg[nCntBg] != NULL)
		{// �w�i�̔j��
			m_pBg[nCntBg]->Uninit();
			m_pBg[nCntBg] = NULL;
		}
	}

	if (NULL != m_pMountain)
	{// �w�i�̔j��
		m_pMountain->Uninit();
		m_pMountain = NULL;
	}

	// �ÓI�ϐ��������l�ɂ���
	m_nTime = 0;

	// ������j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
{
	// ���U���g�̌o�ߎ��Ԃ��J�E���g
	m_nCntResult++;

	// �_��
	m_pPressButton->Flashing(m_nFlash);

	if (m_pBg[0] != NULL)
	{// �e�N�X�`�����W�𓮂���
		m_pBg[0]->TexMove(0.0001f);
	}

	if (m_pBg[1] != NULL)
	{// �e�N�X�`�����W�𓮂���
		m_pBg[1]->TexMove(0.0003f);
	}

	// �J�����̎擾
	CCamera *pCamera = CManager::GetCamera();

	// �ʒu�̎擾
	D3DXVECTOR3 posCameraV = pCamera->GetPosV();
	D3DXVECTOR3 posCameraR = pCamera->GetPosR();

	if (CModelSetManager::GetGoalPos().x > posCameraV.x)
	{// �J�����𓮂���
		posCameraV.x += RESULT_MOVE_CAMERA;
		posCameraR.x += RESULT_MOVE_CAMERA;
	}
	else
	{// �����L���O�ɑJ��
		CFade::Create(CManager::MODE_RANKING);
	}

	// �J�����̈ʒu�̐ݒ�
	pCamera->SetPosV(posCameraV);
	pCamera->SetPosR(posCameraR);

	// ���͂̏����擾
	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CXInput			*pXInput = CManager::GetXInput();			// XInput�̎擾
	CSound			*pSound = CManager::GetSound();				// �T�E���h�̎擾


	if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
	{// ��ʑJ��
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// �t�F�[�h���Ȃ��Ƃ�
			CFade::Create(CManager::MODE_RANKING);
			pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);

			// �_�ł��鑬�x���グ��
			m_nFlash = RESULT_AFTER_FLASH;
		}
	}

	if (m_nCntResult >= RESULT_TIME)
	{// ��ʑJ��
		CFade::Create(CManager::MODE_RANKING);
	}

	CDebugProc::Print("���U���g");

}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{
}
