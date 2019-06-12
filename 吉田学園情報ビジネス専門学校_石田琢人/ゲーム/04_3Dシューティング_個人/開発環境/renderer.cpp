//=============================================================================
//
// �����_�����O���� [renderer.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//=========================================
// �R���X�g���N�^
//=========================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;			// Direct3D�I�u�W�F�N�g
	m_pD3DDevice = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)

#ifdef _DEBUG
	m_pFont = NULL;			// �t�H���g�ւ̃|�C���^
#endif

}

//=========================================
// �f�X�g���N�^
//=========================================
CRenderer::~CRenderer()
{
}

//=========================================
// ����������
//=========================================
HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;						// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ���C�e�B���O���[�h�L��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O������ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	//�n�[�h�E�F�A�̔\�͂�\���\����
	D3DCAPS9 caps;

	//�t�H�O�p�����[�^
	const FLOAT fogStart = 300;
	const FLOAT fogEnd = 7800;

	ZeroMemory(&caps, sizeof(D3DCAPS9));

	m_pD3DDevice->GetDeviceCaps(&caps);

	//�t�H�O���g�p����
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	//�t�H�O�̐F��ݒ肷��
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(255, 130, 130, 130));

	//���_�t�H�O���g�p���Ȃ�
	m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);

	//�t�H�O�̓K�p���@
	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

	//�t�H�O�̊J�n�ʒu
	m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&fogStart));

	//�t�H�O�̏I���ʒu
	m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&fogEnd));

#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

	return S_OK;
}

//=========================================
// �I������
//=========================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif

	// �f�o�C�X�̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=========================================
// �X�V����
//=========================================
void CRenderer::Update(void)
{
	CScene::UpdateAll();
}

//=========================================
// �`�揈��
//=========================================
void CRenderer::Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		CCamera *pCamera = NULL;			// �J�����̏����i�[
		pCamera = CManager::GetCamera();	// �J�����̏����擾

		if (NULL != pCamera)
		{// �k���`�F�b�N
			pCamera->Set();	// �J�����̃Z�b�g
		}

		CManager::MODE mode = CManager::GetMode();

		CTitle *pTitle = NULL;
		CTutorial *pTutorial = NULL;
		CGame *pGame = NULL;
		CResult *pResult = NULL;
		CRanking *pRanking = NULL;
		CScene *pScene = NULL;

		switch (mode)
		{
		case MODE_TITLE:	// �^�C�g��
			pTitle = CManager::GetTitle();	// �^�C�g���̎擾
			if (pTitle != NULL)
			{
				pTitle->Draw();
			}

			CScene::DrawAll();	// �I�u�W�F�N�g��`�悷��

			break;

		case MODE_TUTORIAL:	// �`���[�g���A��
			// �t�F�[�h�ȊO�̃I�u�W�F�N�g�̕`��
			for (int nCntPriority = 0; nCntPriority < 7; nCntPriority++)
			{
				for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
				{
					pScene = CScene::GetScene(nCntPriority, nCntObject);

					if (pScene != NULL)
					{// �I�u�W�F�N�g�̕`��
						pScene->Draw();
					}
				}
			}

			pTutorial = CManager::GetTutorial();	// �`���[�g���A���̎擾
			if (pTutorial != NULL)
			{// �`���[�g���A���N���X�̕`��
				pTutorial->Draw();
			}

			for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
			{
				pScene = CScene::GetScene(NUM_PRIORITY - 1, nCntObject);

				if (pScene != NULL)
				{// �I�u�W�F�N�g�̕`��
					pScene->Draw();
				}
			}

			break;

		case MODE_GAME:		// �Q�[��
			// �t�F�[�h�ȊO�̃I�u�W�F�N�g�̕`��
			for (int nCntPriority = 0; nCntPriority < 7; nCntPriority++)
			{
				for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
				{
					pScene = CScene::GetScene(nCntPriority, nCntObject);

					if (pScene != NULL)
					{// �I�u�W�F�N�g�̕`��
						pScene->Draw();
					}
				}
			}

			pGame = CManager::GetGame();	// �Q�[���̎擾
			if (pGame != NULL)
			{// �Q�[���̕`��
				pGame->Draw();
			}

			for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
			{
				pScene = CScene::GetScene(NUM_PRIORITY - 1, nCntObject);

				if (pScene != NULL)
				{// �I�u�W�F�N�g�̕`��
					pScene->Draw();
				}
			}

			break;

		case MODE_RESULT:		// ���U���g
			pResult = CManager::GetResult();	// �Q�[���̎擾
			if (pResult != NULL)
			{
				pResult->Draw();
			}

			CScene::DrawAll();	// �I�u�W�F�N�g��`�悷��
			break;

		case MODE_GAMEOVER:
			pResult = CManager::GetResult();	// �Q�[���̎擾
			if (pResult != NULL)
			{
				pResult->Draw();
			}

			CScene::DrawAll();	// �I�u�W�F�N�g��`�悷��
			break;

		case MODE_RANKING:		// ���U���g
			pRanking = CManager::GetRanking();	// �Q�[���̎擾
			if (pRanking != NULL)
			{
				pRanking->Draw();
			}

			CScene::DrawAll();	// �I�u�W�F�N�g��`�悷��
			break;
		}

#ifdef _DEBUG
		// FPS�\��
		DrawFPS();
#endif
		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

#ifdef _DEBUG
//=============================================================================
// FPS�\��
//=============================================================================
void CRenderer::DrawFPS(void) const
{
	int nCntFPS;

	nCntFPS = GetFPS();

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];

	wsprintf(str, "FPS:%d\n", nCntFPS);

	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 50, 200, 0xff));
}
#endif
