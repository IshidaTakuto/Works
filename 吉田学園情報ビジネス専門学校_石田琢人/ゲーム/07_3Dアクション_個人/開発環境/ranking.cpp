//=============================================================================
//
// �����L���O���� [ranking.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "ranking.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "debugProc.h"
#include "number.h"
#include "logo.h"
#include "bg.h"
#include "stage.h"
#include "modelSet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RANKING_LOGO			"data/TEXTURE/ranking_logo00.png"
#define RANKING_RANK1			"data/TEXTURE/rank_01.png"
#define RANKING_RANK2			"data/TEXTURE/rank_02.png"
#define RANKING_RANK3			"data/TEXTURE/rank_03.png"
#define RANKING_RANK4			"data/TEXTURE/rank_04.png"
#define RANKING_RANK5			"data/TEXTURE/rank_05.png"

#define RANKING_LOGO_WIDTH		(600.0f)		// ���S�̕�
#define RANKING_LOGO_HEIGHT		(100.0f)		// ���S�̍���

#define RANKING_PRESS_WIDTH		(500.0f)		// ���͑҂��\���̕�
#define RANKING_PRESS_HEIGHT	(100.0f)		// ���͑҂��\���̍���
#define RANKING_BEFORE_FLASH	(60)			// �{�^������͂���O�̓_��
#define RANKING_AFTER_FLASH		(5)				// �{�^������͂�����̓_��

#define RANKING_TIME			(420)			// �^�C�g����ʂ܂ł̎���
#define RANKINGNUM_SIZE			(80.0f)			// �����̑傫��

#define RANKING_MOVE_CAMERA		(-1.0f)			// �J�����̓������x

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CRanking::m_nRanking[MAX_RANKING] = { 150, 200, 250, 300, 350 };
int CRanking::m_nScore = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRanking::CRanking()
{
	int nCntRanking;
	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ������������
		m_apNumber[nCntRanking] = NULL;
		m_apPolygon[nCntRanking] = NULL;
		m_apTexture[nCntRanking] = NULL;
	}

	for (; nCntRanking < RANKING_MAX_TEXTURE; nCntRanking++)
	{// �c��̃e�N�X�`����������
		m_apTexture[nCntRanking] = NULL;
	}

	m_nCntFade = 0;
	m_nCntHighScore = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRanking::~CRanking()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRanking::Init(void)
{
	// �e�N�X�`���̓ǂݍ���
	CNumber::Load();
	CBg::Load();
	CWeed::Load();

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_RANKING);

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���S�̐���
	m_pLogo = CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 50.0f, 0.0f), RANKING_LOGO_WIDTH, RANKING_LOGO_HEIGHT);
	m_pLogo->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F�̐ݒ�

	// ���͑҂��\���𐶐�
	m_pPress = CLogoPressButton::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 650.0f, 0.0f), RANKING_PRESS_WIDTH, RANKING_PRESS_HEIGHT);
	m_pPress->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F�̐ݒ�

	// �����L���O�̏��ʃ��S
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{// �����L���O�X�R�A�̐����[�v
		m_apPolygon[nCntRank] = CScene2D::Create();
		m_apPolygon[nCntRank]->SetPos(D3DXVECTOR3(100.0f, 125.0f + (nCntRank * RANKINGNUM_SIZE * 1.2f), 0.0f));
		m_apPolygon[nCntRank]->SetSize(RANKINGNUM_SIZE, RANKINGNUM_SIZE);
	}

	// ���ʂ̃e�N�X�`���ɐF��t����
	m_apPolygon[0]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));	// 1��
	m_apPolygon[1]->SetColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));	// 2��
	m_apPolygon[2]->SetColor(D3DXCOLOR(0.7f, 0.7f, 0.0f, 1.0f));	// 3��
	m_apPolygon[3]->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));	// 4��
	m_apPolygon[4]->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));	// 5��

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, RANKING_LOGO, &m_apTexture[0]);
	m_pLogo->BindTexture(m_apTexture[0]);

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, RANKING_RANK1, &m_apTexture[1]);
	m_apPolygon[0]->BindTexture(m_apTexture[1]);

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, RANKING_RANK2, &m_apTexture[2]);
	m_apPolygon[1]->BindTexture(m_apTexture[2]);

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, RANKING_RANK3, &m_apTexture[3]);
	m_apPolygon[2]->BindTexture(m_apTexture[3]);

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, RANKING_RANK4, &m_apTexture[4]);
	m_apPolygon[3]->BindTexture(m_apTexture[4]);

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, RANKING_RANK5, &m_apTexture[5]);
	m_apPolygon[4]->BindTexture(m_apTexture[5]);

	// �����L���O�̃X�R�A(����)
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�X�R�A�̐����[�v
		m_nDigits[nCntRanking] = (int)log10f((float)m_nRanking[nCntRanking]) + 1;
		if (0 >= m_nDigits[nCntRanking]) { m_nDigits[nCntRanking] = 1; }

		m_apNumber[nCntRanking] = new CNumber*[m_nDigits[nCntRanking]];

		for (int nCntDigit = 0; nCntDigit < m_nDigits[nCntRanking]; nCntDigit++)
		{// �����L���O�X�R�A�̌������[�v
			m_apNumber[nCntRanking][nCntDigit] = CNumber::Create(D3DXVECTOR3((700 + (RANKINGNUM_SIZE * m_nDigits[nCntRanking]) - RANKINGNUM_SIZE) - (nCntDigit * RANKINGNUM_SIZE * 2), 125.0f + (nCntRanking * RANKINGNUM_SIZE * 1.2f), 0.0f), RANKINGNUM_SIZE, RANKINGNUM_SIZE);

			// �����̐ݒ�
			int aNumber = m_nRanking[nCntRanking] % (int)powf(10.0f, (float)(nCntDigit + 1)) / (int)powf(10.0f, (float)nCntDigit);
			m_apNumber[nCntRanking][nCntDigit]->SetNumber(aNumber);
		}
	}

	// ���͑҂��\���̓_�ő��x
	m_nFlash = RANKING_BEFORE_FLASH;

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
	pCamera->SetPosV(D3DXVECTOR3(2800.0f, 80.0f, -500.0f));
	pCamera->SetPosR(D3DXVECTOR3(2800.0f, 80.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRanking::Uninit(void)
{
	// �e�N�X�`���̔j��
	CBg::Unload();
	CModelSetManager::Unload();
	CWeed::Unload();
	CNumber::Unload();

	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < RANKING_MAX_TEXTURE; nCntTexture++)
	{// �e�N�X�`���������[�v
		if (NULL != m_apTexture[nCntTexture])
		{// �e�N�X�`���̊J��
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�X�R�A�̐����[�v
		for (int nCntNumber = 0; nCntNumber < m_nDigits[nCntRanking]; nCntNumber++)
		{// ���������[�v
			if (NULL != m_apNumber[nCntRanking][nCntNumber])
			{// �����̔j��
				m_apNumber[nCntRanking][nCntNumber]->Uninit();
				m_apNumber[nCntRanking][nCntNumber] = NULL;

			}
		}

		if (NULL != m_apNumber[nCntRanking])
		{// �����̔j��
			delete m_apNumber[nCntRanking];
			m_apNumber[nCntRanking] = NULL;
		}

		if (NULL != m_apPolygon[nCntRanking])
		{// �|���S���̔j��
			m_apPolygon[nCntRanking]->Uninit();
			m_apPolygon[nCntRanking] = NULL;
		}
	}

	for (int nCntBg = 0; nCntBg < RANKING_NUM_BG; nCntBg++)
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

	// ������j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CRanking::Update(void)
{
	// �_�ŗp�J�E���g�A�b�v
	m_nCntHighScore++;
	m_nCntHighScore = m_nCntHighScore % 4;

	// �����̎擾�����X�R�A��_�ł�����
	if (m_nScore > -1)
	{// �X�R�A�������Ă���
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
		{
			if (m_nRanking[nCntRanking] == m_nScore)
			{// �����̃X�R�A��_�ł�����
				for (int nCntNumber = 0; nCntNumber < m_nDigits[nCntRanking]; nCntNumber++)
				{// �������܂킷
					if (m_nCntHighScore < 2)
					{
						m_apNumber[nCntRanking][nCntNumber]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					}
					else if (m_nCntHighScore >= 2)
					{
						m_apNumber[nCntRanking][nCntNumber]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
					}
				}
				break;	// �����X�R�A���������ꍇ�̕ی�
			}
		}
	}

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
		posCameraV.x += RANKING_MOVE_CAMERA;
		posCameraR.x += RANKING_MOVE_CAMERA;
	}
	else
	{// �����L���O�ɑJ��
		CFade::Create(CManager::MODE_RANKING);
	}

	// �J�����̈ʒu�̐ݒ�
	pCamera->SetPosV(posCameraV);
	pCamera->SetPosR(posCameraR);

	// ���͑҂��̓_��
	m_pPress->Flashing(m_nFlash);

	// �^�C�g���փt�F�[�h����܂ł̃J�E���g
	m_nCntFade++;
	if (m_nCntFade >= RANKING_TIME)
	{//���[�h�I��
		CFade::Create(CManager::MODE_TITLE);
	}

	CInputKeyboard	*pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̓��͂��擾
	CXInput			*pXInput = CManager::GetXInput();			// XInput�̎擾
	CSound			*pSound = CManager::GetSound();				// �T�E���h�̎擾

	if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
	{// ��ʑJ��
		if (CFade::GetFade() == CFade::FADE_NONE)
		{// �t�F�[�h���Ȃ��Ƃ�
			CFade::Create(CManager::MODE_TITLE);
			pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);

			// �_�ł��鑬�x���グ��
			if (CFade::FADE_NONE != CFade::GetFade()) { m_nFlash = RANKING_AFTER_FLASH; }
		}
	}

	CDebugProc::Print("�����L���O");

}

//=============================================================================
// �`�揈��
//=============================================================================
void CRanking::Draw(void)
{
}

//=============================================================================
// �X�R�A�̃Z�b�g
//=============================================================================
void CRanking::Set(int nScore)
{
	int nSwap;							// ����ւ��p
	int aEmptyData[MAX_RANKING + 1];	// ����ւ��p

	for (int nCntNum = 0; nCntNum < MAX_RANKING; nCntNum++)
	{// ���܂ł̋L�^����̃f�[�^�ɓ����
		aEmptyData[nCntNum] = m_nRanking[nCntNum];
	}

	aEmptyData[MAX_RANKING] = nScore;	// �Ō�̔z��ɍŐV�̃X�R�A������

	// ����ւ�
	for (int nCntNum = 1; nCntNum < MAX_RANKING + 1; nCntNum++)
	{
		for (int nCntNum1 = nCntNum; nCntNum1 > 0; nCntNum1--)
		{
			if (aEmptyData[nCntNum1 - 1] >= aEmptyData[nCntNum1])
			{// ����ւ�
				nSwap = aEmptyData[nCntNum1];
				aEmptyData[nCntNum1] = aEmptyData[nCntNum1 - 1];
				aEmptyData[nCntNum1 - 1] = nSwap;
			}
		}
	}

	for (int nCntNum = 0; nCntNum < MAX_RANKING; nCntNum++)
	{// ����ւ����I������f�[�^�������L���O�ɔ��f����
		m_nRanking[nCntNum] = aEmptyData[nCntNum];

	}

	m_nScore = nScore;	// �ŐV�̃X�R�A���X�V����
}
