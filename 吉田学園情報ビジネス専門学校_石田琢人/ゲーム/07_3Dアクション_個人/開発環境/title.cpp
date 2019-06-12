//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "title.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "manager.h"
#include "debugProc.h"
#include "logo.h"
#include "stage.h"
#include "modelSet.h"
#include "bg.h"
#include "camera.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TITLE_TEXTURE00	"data/TEXTURE/title_logo.png"

#define TITLE_TIME		(1200)		// �����L���O��ʂ܂ł̎���

#define TITLE_LOGO_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 250.0f, 0.0f))	// �^�C�g�����S�̈ʒu
#define TITLE_LOGO_WIDTH	(700.0f)	// �^�C�g�����S�̕�
#define TITLE_LOGO_HEIGHT	(300.0f)	// �^�C�g�����S�̍���

#define TITLE_PRESSANY_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 600.0f, 0.0f))	// ���͑҂��\���̈ʒu
#define TITLE_PRESSANY_WIDTH	(500.0f)	// ���͑҂��̕�
#define TITLE_PRESSANY_HEIGHT	(100.0f)	// ���͑҂��̍���
#define TITLE_BEFORE_FLASH		(60)		// �{�^������͂���O�̓_��
#define TITLE_AFTER_FLASH		(5)			// �{�^������͂�����̓_��

#define TITLE_MOVE_CAMERA		(1.0f)		// �J�����̓������x

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//=========================================
// �R���X�g���N�^
//=========================================
CTitle::CTitle()
{
	m_nCntTitle = 0;	// �����L���O�܂ł̃J�E���^
}

//=========================================
// �f�X�g���N�^
//=========================================
CTitle::~CTitle()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	// �e�N�X�`���̓ǂݍ���
	CBg::Load();
	CWeed::Load();

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	CScene::SetObjType(CScene::OBJTYPE_TITLE);

	// ���S�̐���
	m_pLogo = CLogo::Create(TITLE_LOGO_POS, TITLE_LOGO_WIDTH, TITLE_LOGO_HEIGHT);
	m_pPressButton = CLogoPressButton::Create(TITLE_PRESSANY_POS, TITLE_PRESSANY_WIDTH, TITLE_PRESSANY_HEIGHT);

	// �F�̐ݒ�
	m_pLogo->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_pPressButton->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	LPDIRECT3DTEXTURE9 pTexture;
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE00, &pTexture);		// �e�N�X�`���̐���
	m_pLogo->BindTexture(pTexture);										// �e�N�X�`���̐ݒ�

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
	pCamera->SetPosV(D3DXVECTOR3(-2800.0f, 4000.0f, 0.0f));
	pCamera->SetPosR(D3DXVECTOR3(-2800.0f, 0.0f, 0.0f));

	// �_�ł���Ԋu
	m_nFlash = TITLE_BEFORE_FLASH;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
{
	// �e�N�X�`���̔j��
	CBg::Unload();
	CModelSetManager::Unload();
	CWeed::Unload();

	if (NULL != m_pLogo)
	{// ���S�̔j��
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	if (NULL != m_pPressButton)
	{// ���S�̔j��
		m_pPressButton->Uninit();
		m_pPressButton = NULL;
	}

	for (int nCntBg = 0; nCntBg < TITLE_NUM_BG; nCntBg++)
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

	// �����̔j��
	CScene::Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	// �^�C�g���̌o�ߎ���
	m_nCntTitle++;

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
		posCameraV.x += TITLE_MOVE_CAMERA;
		posCameraR.x += TITLE_MOVE_CAMERA;
	}
	else
	{// �����L���O�ɑJ��
		CFade::Create(CManager::MODE_RANKING);
	}

	// �J�����̈ʒu�̐ݒ�
	pCamera->SetPosV(posCameraV);
	pCamera->SetPosR(posCameraR);

	// ���͂̏����擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	CXInput *pXInput = CManager::GetXInput();

	// �T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	// �F�̎擾
	D3DXCOLOR col = m_pLogo->GetColor();

	if (col.a >= 1.0f)
	{// �����łȂ����

		m_pPressButton->Flashing(m_nFlash);	// ���͑҂��e�N�X�`����_��

		if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
		{// ��ʑJ��
			if (CFade::GetFade() == CFade::FADE_NONE)
			{// �t�F�[�h���Ȃ��Ƃ�
				CFade::Create(CManager::MODE_TUTORIAL);
				pSound->PlaySound(CSound::SOUND_LABEL_DECIDE);

				// �_�ł��鑬�x���グ��
				m_nFlash = TITLE_AFTER_FLASH;
			}
		}
		else if (m_nCntTitle >= TITLE_TIME)
		{// ��莞�Ԍo��
			CFade::Create(CManager::MODE_RANKING);
		}
	}
	else
	{// �����ȏ��
		col.a += 0.01f;
	}

	if (pKeyboard->GetAnyKey() || pXInput->GetAnyButton(0))
	{// �{�^���������Ƃ����ɕ\�������
		col.a = 1.0f;
	}

	// �F�̐ݒ�
	m_pLogo->SetColor(col);

	CDebugProc::Print("�^�C�g��");
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{
}
