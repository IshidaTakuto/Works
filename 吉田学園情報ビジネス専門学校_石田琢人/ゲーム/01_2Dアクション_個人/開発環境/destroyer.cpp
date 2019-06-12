//=============================================================================
//
// �T���̏��� [destroyer.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "destroyer.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_DESTROYER		"data/TEXTURE/shark.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define DESTROYER_SIZE_X	(200)
#define DESTROYER_SIZE_Y	(150)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureDestroyer = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDestroyer = NULL;	//���_�o�b�t�@�ւ̃|�C���^
DESTROYER g_Destroyer;
int g_nCntDestroyer;
float g_fSpeed;

//=============================================================================
// ����������
//=============================================================================
void InitDestroyer(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�w�i�̏��̏�����
	g_Destroyer.pos = D3DXVECTOR3(-DESTROYER_SIZE_X, SCREEN_HEIGHT + DESTROYER_SIZE_Y, 0.0f);
	g_Destroyer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Destroyer.state = DESTROYERSTATE_NONE;

	g_nCntDestroyer = 0;
	g_fSpeed = 1;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_DESTROYER, &g_pTextureDestroyer);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDestroyer,
		NULL);

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffDestroyer->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(g_Destroyer.pos.x - DESTROYER_SIZE_X, g_Destroyer.pos.y - DESTROYER_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Destroyer.pos.x + DESTROYER_SIZE_X, g_Destroyer.pos.y - DESTROYER_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Destroyer.pos.x - DESTROYER_SIZE_X, g_Destroyer.pos.y + DESTROYER_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Destroyer.pos.x + DESTROYER_SIZE_X, g_Destroyer.pos.y + DESTROYER_SIZE_Y, 0.0f);

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

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffDestroyer->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitDestroyer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureDestroyer != NULL)
	{
		g_pTextureDestroyer->Release();
		g_pTextureDestroyer = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffDestroyer != NULL)
	{
		g_pVtxBuffDestroyer->Release();
		g_pVtxBuffDestroyer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateDestroyer(void)
{
	PLAYER pPlayer;
	pPlayer = GetPlayer();

	g_nCntDestroyer++;

	switch (g_Destroyer.state)
	{
	case DESTROYERSTATE_NORMAL:
		g_Destroyer.pos.y += 0.6f * g_fSpeed;
		if (g_nCntDestroyer % 90 == 0)
		{
			g_fSpeed = g_fSpeed * -1;
		}

		break;

	case DESTROYERSTATE_MOVE:
		if(g_Destroyer.pos.x - DESTROYER_SIZE_X < 0 && g_Destroyer.pos.y + DESTROYER_SIZE_Y > SCREEN_HEIGHT + 10)
		{
			g_Destroyer.move.x += 0.06f;
			g_Destroyer.move.y -= 0.04f;

			g_Destroyer.pos.y += g_Destroyer.move.y;
			g_Destroyer.pos.x += g_Destroyer.move.x;

		}
		else
		{// �ړ��I��
 			g_Destroyer.state = DESTROYERSTATE_NORMAL;
		}
		break;
	}

	if (pPlayer.pos.x >= SCREEN_WIDTH / 3 && g_Destroyer.state == DESTROYERSTATE_NONE)
	{
 		g_Destroyer.state = DESTROYERSTATE_MOVE;
	}

	VERTEX_2D*pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffDestroyer->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(g_Destroyer.pos.x - DESTROYER_SIZE_X, g_Destroyer.pos.y - DESTROYER_SIZE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Destroyer.pos.x + DESTROYER_SIZE_X, g_Destroyer.pos.y - DESTROYER_SIZE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Destroyer.pos.x - DESTROYER_SIZE_X, g_Destroyer.pos.y + DESTROYER_SIZE_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Destroyer.pos.x + DESTROYER_SIZE_X, g_Destroyer.pos.y + DESTROYER_SIZE_Y, 0.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffDestroyer->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawDestroyer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X���擾����
	pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffDestroyer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureDestroyer);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �f�X�g���C���[�̓����蔻�菈��
//=============================================================================
bool CollisionDestroyer(D3DXVECTOR3 *pPos, float pHeight, float pWidth)
{
	bool bTouch = false;

	if (pPos->x>= g_Destroyer.pos.x - DESTROYER_SIZE_X && pPos->x <= g_Destroyer.pos.x + DESTROYER_SIZE_X)
	{// ���͈͓̔�
		if (pPos->y > g_Destroyer.pos.y - DESTROYER_SIZE_Y && pPos->y <= g_Destroyer.pos.y - DESTROYER_SIZE_Y ||
			pPos->y > g_Destroyer.pos.y - DESTROYER_SIZE_Y && pPos->y <= g_Destroyer.pos.y + DESTROYER_SIZE_Y ||
			pPos->y - pHeight >= g_Destroyer.pos.y - DESTROYER_SIZE_Y && pPos->y - pHeight < g_Destroyer.pos.y + DESTROYER_SIZE_Y ||
			pPos->y - pHeight <= g_Destroyer.pos.y - DESTROYER_SIZE_Y && pPos->y > g_Destroyer.pos.y + DESTROYER_SIZE_Y
			)
		{// ��Ɖ��̔���
			bTouch = true;
		}
	}
	return bTouch;
}


DESTROYER GetDestroyer(void)
{
	return g_Destroyer;
}