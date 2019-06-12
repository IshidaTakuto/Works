//=============================================================================
//
// �W�F�l���[�^���� [generator.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "generator.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GENERATOR_TEXTURENAME	"data/TEXTURE/generator.png"	// �w�i�̃e�N�X�`����
#define GENERATOR_SIZE	(50.0f)	// �W�F�l���[�^�̑傫��
#define MAX_GENERATOR	(7)	// �W�F�l���[�^�̍ő吔

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGenerator = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGenerator = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
GENERATOR g_aGenerator[MAX_GENERATOR];
int g_nCntGenerator;
int g_nCntpVtx;

//=============================================================================
// �|���S������������
//=============================================================================
void InitGenerator(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �W�F�l���[�^�̏���������
	for (int nCntGenerator = 0; nCntGenerator < MAX_GENERATOR; nCntGenerator++)
	{
		g_aGenerator[nCntGenerator].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aGenerator[nCntGenerator].type = BLOCKTYPE_NORMAL;
		g_aGenerator[nCntGenerator].interval = 0;
		g_aGenerator[nCntGenerator].des = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aGenerator[nCntGenerator].fHeight = 0.0f;
		g_aGenerator[nCntGenerator].fWidth = 0.0f;
		g_aGenerator[nCntGenerator].bUse = false;
	}

	g_nCntGenerator = 0;
	g_nCntpVtx = 0;

	// �e�N�X�`���̓ǂݍ���
	//D3DXCreateTextureFromFile(pDevice, GENERATOR_TEXTURENAME, &g_pTextureGenerator);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GENERATOR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGenerator,
		NULL);

	VERTEX_2D*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffGenerator->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	for (int nCntGenerator = 0; nCntGenerator < MAX_GENERATOR; nCntGenerator++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x - GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y - GENERATOR_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x + GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y - GENERATOR_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x - GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y + GENERATOR_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x + GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y + GENERATOR_SIZE, 0.0f);

		// 1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffGenerator->Unlock();
}

//=============================================================================
// �|���S���I������
//=============================================================================
void UninitGenerator(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureGenerator != NULL)
	{
		g_pTextureGenerator->Release();
		g_pTextureGenerator = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffGenerator != NULL)
	{
		g_pVtxBuffGenerator->Release();
		g_pVtxBuffGenerator = NULL;
	}
}

//=============================================================================
// �|���S���X�V����
//=============================================================================
void UpdateGenerator(void)
{
	int nCntGenerator;
	VERTEX_2D*pVtx;	// ���_���ւ̃|�C���^

	GAMESTATE Gamestate;
	Gamestate = GetGameState();

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffGenerator->Lock(0, 0, (void**)&pVtx, 0);

	g_nCntGenerator++;	// �u���b�N���o���Ԋu�̃J�E���^

	for (nCntGenerator = 0; nCntGenerator < MAX_GENERATOR; nCntGenerator++)
	{
		if (Gamestate == GAMESTATE_NORMAL)
		{
			if (g_aGenerator[nCntGenerator].bUse == true)
			{// �g���Ă���
				if (g_nCntGenerator % g_aGenerator[nCntGenerator].interval == 0)
				{// ��o��
					SetBlock(D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x, g_aGenerator[nCntGenerator].pos.y, 0.0f), g_aGenerator[nCntGenerator].fWidth, g_aGenerator[nCntGenerator].fHeight, g_aGenerator[nCntGenerator].des, g_aGenerator[nCntGenerator].type);
				}
			}
		}

		// ���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x - GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y - GENERATOR_SIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x + GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y - GENERATOR_SIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x - GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y + GENERATOR_SIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aGenerator[nCntGenerator].pos.x + GENERATOR_SIZE, g_aGenerator[nCntGenerator].pos.y + GENERATOR_SIZE, 0.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffGenerator->Unlock();
}

//=============================================================================
// �|���S���`�揈��
//=============================================================================
void DrawGenerator(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGenerator, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureGenerator);

	for (int nCntGenerator = 0; nCntGenerator < MAX_GENERATOR; nCntGenerator++)
	{// �|���S���̕`��
		if (g_aGenerator[nCntGenerator].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntGenerator, 2);
		}
	}
}

//=============================================================================
// �W�F�l���[�^�̐ݒ�
//=============================================================================
void SetGenerator(D3DXVECTOR3 pos, BLOCKTYPE type, float fWidth, float fHeight, D3DXVECTOR3 des, int interval)
{
	int nCntGenerator;
	VERTEX_2D*pVtx;	// ���_���ւ̃|�C���^

	for (nCntGenerator = 0; nCntGenerator < MAX_GENERATOR; nCntGenerator++)
	{
		if (g_aGenerator[nCntGenerator].bUse == false)
		{
			g_aGenerator[nCntGenerator].pos = pos;
			g_aGenerator[nCntGenerator].type = type;
			g_aGenerator[nCntGenerator].fWidth = fWidth;
			g_aGenerator[nCntGenerator].fHeight = fHeight;
			g_aGenerator[nCntGenerator].des = des;
			g_aGenerator[nCntGenerator].interval = interval;
			g_aGenerator[nCntGenerator].bUse = true;

			// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			g_pVtxBuffGenerator->Lock(0, 0, (void**)&pVtx, 0);

			if (g_aGenerator[nCntGenerator].pos.x > SCREEN_WIDTH / 2)
			{// ��ʔ������E��
				// �e�N�X�`�����W
				pVtx[0 + g_nCntpVtx].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[1 + g_nCntpVtx].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2 + g_nCntpVtx].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3 + g_nCntpVtx].tex = D3DXVECTOR2(0.0f, 1.0f);
			}
			// ���_�o�b�t�@���A�����b�N
			g_pVtxBuffGenerator->Unlock();

			g_nCntpVtx += 4;
			break;
		}

	}
}