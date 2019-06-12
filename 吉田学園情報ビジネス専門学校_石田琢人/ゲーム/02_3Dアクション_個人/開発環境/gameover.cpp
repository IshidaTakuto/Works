//=============================================================================
//
// �Q�[���I�[�o�[���� [gameover.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "gameover.h"
#include "fade.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GAMEOVER_TEXTURENAME0	"DATA/TEXTURE/player.png"			// �e�N�X�`����
#define GAMEOVER_TEXTURENAME1	"DATA/TEXTURE/result_logo001.png"	// �e�N�X�`����
#define GAMEOVER_TEXTURENAME2	"DATA/TEXTURE/press_enter.png"	// �e�N�X�`����

#define MAX_GAMEOVER	(3)
#define LOGO_SIZE_X	(500.0f)
#define LOGO_SIZE_Y	(100.0f)
#define GAMEOVER_TIME	(600)	// �Q�[���I�[�o�[��ʂ̕\������

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGameover[MAX_GAMEOVER] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameover = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
GAMEOVER g_aGameover[MAX_GAMEOVER];
int g_nCntGameover;
int g_nGameoverTransparent;

//=============================================================================
// ����������
//=============================================================================
void InitGameover(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	InitCamera();

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ������
	g_aGameover[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 + 200, 0.0f);
	g_aGameover[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	g_aGameover[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT + -100, 0.0f);

	g_aGameover[0].nType = 0;
	g_aGameover[1].nType = 1;
	g_aGameover[2].nType = 2;

	g_nCntGameover = 0;
	g_nGameoverTransparent = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, GAMEOVER_TEXTURENAME0, &g_pTextureGameover[0]);
	D3DXCreateTextureFromFile(pDevice, GAMEOVER_TEXTURENAME1, &g_pTextureGameover[1]);
	D3DXCreateTextureFromFile(pDevice, GAMEOVER_TEXTURENAME2, &g_pTextureGameover[2]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GAMEOVER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameover,
		NULL);

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffGameover->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(g_aGameover[0].pos.x - LOGO_SIZE_X, g_aGameover[0].pos.y - LOGO_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aGameover[0].pos.x + LOGO_SIZE_X, g_aGameover[0].pos.y - LOGO_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aGameover[0].pos.x - LOGO_SIZE_X, g_aGameover[0].pos.y + LOGO_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aGameover[0].pos.x + LOGO_SIZE_X, g_aGameover[0].pos.y + LOGO_SIZE_Y, 0.0f);

	//1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�̍��W
	pVtx[4].pos = D3DXVECTOR3(g_aGameover[1].pos.x - LOGO_SIZE_X, g_aGameover[1].pos.y - LOGO_SIZE_Y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_aGameover[1].pos.x + LOGO_SIZE_X, g_aGameover[1].pos.y - LOGO_SIZE_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_aGameover[1].pos.x - LOGO_SIZE_X, g_aGameover[1].pos.y + LOGO_SIZE_Y, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_aGameover[1].pos.x + LOGO_SIZE_X, g_aGameover[1].pos.y + LOGO_SIZE_Y, 0.0f);

	//1.0f�ŌŒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//���_�J���[
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�̍��W
	pVtx[8].pos = D3DXVECTOR3(g_aGameover[0].pos.x - 200, g_aGameover[0].pos.y - 50, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(g_aGameover[0].pos.x + 200, g_aGameover[0].pos.y - 50, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(g_aGameover[0].pos.x - 200, g_aGameover[0].pos.y + 50, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(g_aGameover[0].pos.x + 200, g_aGameover[0].pos.y + 50, 0.0f);

	//1.0f�ŌŒ�
	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	//���_�J���[
	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffGameover->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitGameover(void)
{
	UninitCamera();

	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < MAX_GAMEOVER; nCntTex++)
	{
		if (g_pTextureGameover[nCntTex] != NULL)
		{
			g_pTextureGameover[nCntTex]->Release();
			g_pTextureGameover[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGameover != NULL)
	{
		g_pVtxBuffGameover->Release();
		g_pVtxBuffGameover = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGameover(void)
{
	UpdateCamera();

	FADE pFade;

	g_nCntGameover++;

	//�t�F�[�h�̎擾
	pFade = *GetFade();
	if (pFade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			//PlaySound(SOUND_LABEL_SE_DECIDE);

			//���[�h�I��
			SetFade(MODE_RANKING);
		}
		else if (g_nCntGameover >= GAMEOVER_TIME)
		{
			//���[�h�I��
			SetFade(MODE_RANKING);
		}
	}

	if (g_aGameover[2].col.a <= 1.0f && g_nGameoverTransparent == 0)
	{//	�����x��1.0f�̎�
		g_aGameover[2].col.a -= 0.02f;
		if (g_aGameover[2].col.a <= 0.0f)
		{// 0.0f�ȉ��ɂȂ����Ƃ�
			g_aGameover[2].col.a = 0.0f;
			g_nGameoverTransparent = 1;
		}
	}
	else if (g_aGameover[2].col.a >= 0.0f && g_nGameoverTransparent == 1)
	{//	�����x��0.0f�̎�
		g_aGameover[2].col.a += 0.02f;
		if (g_aGameover[2].col.a >= 1.0f)
		{// 1.0f�ȏ�ɂȂ����Ƃ�
			g_aGameover[2].col.a = 1.0f;
			g_nGameoverTransparent = 0;
		}
	}

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffGameover->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[
	pVtx[8].col = g_aGameover[2].col;
	pVtx[9].col = g_aGameover[2].col;
	pVtx[10].col = g_aGameover[2].col;
	pVtx[11].col = g_aGameover[2].col;

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffGameover->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGameover(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X���擾����
	pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGameover, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGameover = 0; nCntGameover < MAX_GAMEOVER; nCntGameover++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureGameover[g_aGameover[nCntGameover].nType]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntGameover, 2);
	}
}
