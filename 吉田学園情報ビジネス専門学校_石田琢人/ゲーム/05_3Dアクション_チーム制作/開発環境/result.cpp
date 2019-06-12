//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "result.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RESULT_TEXTURENAME0	"DATA/TEXTURE/result_logo.png"	// �e�N�X�`����
#define RESULT_TEXTURENAME1	"DATA/TEXTURE/pressany.png"		// �e�N�X�`����
#define RESULT_TEXTURENAME2	"DATA/TEXTURE/result_clear.png"	// �e�N�X�`����
#define RESULT_TEXTURENAME3	"DATA/TEXTURE/result_over.png"	// �e�N�X�`����
#define RESULT_TEXTURENAME4	"DATA/TEXTURE/result_clear_bg.png"	// �e�N�X�`����
#define RESULT_TEXTURENAME5	"DATA/TEXTURE/result_over_bg.png"	// �e�N�X�`����
#define RESULT_TIME	(600)	// ���U���g�\������
#define MAX_RESULT	(4)
#define MAX_RESULT_TEXTURE	(6)
#define LOGO_SIZE_X	(200)
#define LOGO_SIZE_Y	(50)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
RESULT g_aResult[MAX_RESULT];
int g_nCntResult;
D3DXCOLOR g_colorResult;

//=============================================================================
// ����������
//=============================================================================
void InitResult(void)
{
	int nCntResult;

	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���S
	g_aResult[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f);
	g_aResult[0].nType = 0;
	g_aResult[0].fWidth = LOGO_SIZE_X;
	g_aResult[0].fHeight = LOGO_SIZE_Y;

	// ���͑҂�
	g_aResult[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, 0.0f);
	g_aResult[1].nType = 1;
	g_aResult[1].fWidth = 200 * 1.5f;
	g_aResult[1].fHeight = 40 * 1.5f;

	// �N���A���Q�[���I�[�o�[�̕������S
	g_aResult[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aResult[2].fWidth = 450;
	g_aResult[2].fHeight = 100;

	// �w�i
	g_aResult[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aResult[3].fWidth = SCREEN_WIDTH / 2;
	g_aResult[3].fHeight = SCREEN_HEIGHT / 2;


	g_colorResult = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_nCntResult = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME0, &g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME1, &g_pTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME2, &g_pTextureResult[2]);
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME3, &g_pTextureResult[3]);
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME4, &g_pTextureResult[4]);
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME5, &g_pTextureResult[5]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RESULT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aResult[nCntResult].pos.x - g_aResult[nCntResult].fWidth, g_aResult[nCntResult].pos.y - g_aResult[nCntResult].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResult[nCntResult].pos.x + g_aResult[nCntResult].fWidth, g_aResult[nCntResult].pos.y - g_aResult[nCntResult].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aResult[nCntResult].pos.x - g_aResult[nCntResult].fWidth, g_aResult[nCntResult].pos.y + g_aResult[nCntResult].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResult[nCntResult].pos.x + g_aResult[nCntResult].fWidth, g_aResult[nCntResult].pos.y + g_aResult[nCntResult].fHeight, 0.0f);

		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntResult == MAX_RESULT - 1)
		{
			//���_�J���[
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
		else
		{
			//���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffResult->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
	for (int nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateResult(void)
{
	FADE pFade;

	g_nCntResult++;

	//�t�F�[�h�̎擾
	pFade = *GetFade();
	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || GetAnyTrigger(1,0) == true)
		{// ����L�[����
			PlaySound(SOUND_LABEL_SELECT);

			SetFadeMode(MODE_RANKING);
		}
		if (g_nCntResult >= RESULT_TIME)
		{// ��莞�Ԍo��
			SetFadeMode(MODE_RANKING);
		}
	}

	if (g_colorResult.a <= 1.0f && g_aResult[1].nTransparent == 0)
	{//	�����x��1.0f�̎�
		g_colorResult.a -= 0.02f;
		if (g_colorResult.a <= 0.0f)
		{// 0.0f�ȉ��ɂȂ����Ƃ�
			g_colorResult.a = 0.0f;
			g_aResult[1].nTransparent = 1;
		}
	}
	else if (g_colorResult.a >= 0.0f && g_aResult[1].nTransparent == 1)
	{//	�����x��0.0f�̎�
		g_colorResult.a += 0.02f;
		if (g_colorResult.a >= 1.0f)
		{// 1.0f�ȏ�ɂȂ����Ƃ�
			g_colorResult.a = 1.0f;
			g_aResult[1].nTransparent = 0;
		}
	}

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[
	pVtx[4].col = g_colorResult;
	pVtx[5].col = g_colorResult;
	pVtx[6].col = g_colorResult;
	pVtx[7].col = g_colorResult;

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffResult->Unlock();

	MODE mode = GetMode();
	// ���[�h�ɂ���ăe�N�X�`����ς���
	if (mode == MODE_RESULT)
	{
		g_aResult[2].nType = 2;
		g_aResult[3].nType = 4;
	}
	else if (mode == MODE_GAMEOVER)
	{
		g_aResult[2].nType = 3;
		g_aResult[3].nType = 5;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X���擾����
	pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntResult = MAX_RESULT - 1; nCntResult >= 0; nCntResult--)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureResult[g_aResult[nCntResult].nType]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntResult, 2);
	}
}
