//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "game.h"
#include "sound.h"
#include "stock.h"
#include "destroyer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_TEXTURENAME	"data/TEXTURE/player001.png"	// �e�N�X�`����
#define PLAYER_SIZE		(30)	// �v���C���[�̑傫��
#define JUMP_HEIGHT		(-30)	// �W�����v�̍���
#define GRAVITY			(2)		// �d��
#define ANIM_SPEED		(6)		// �A�j���[�V�����̈ړ����x
#define TEX_UPDATE		((g_Player.nCounterAnim % 4) * 0.25f)	// �e�N�X�`��X���W�̍X�V
#define TEX_PATTERN		(4)	// �e�N�X�`���̃p�^�[����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//���_�o�b�t�@�ւ̃|�C���^
PLAYER g_Player;
int g_nCntStock;

//=============================================================================
// �v���C���[����������
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �v���C���[�̏��̏�����
	g_Player.pos = D3DXVECTOR3(60, 400, 0.0f);
	g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.fHeight = PLAYER_SIZE * 2;
	g_Player.fWidth = PLAYER_SIZE / 2;
	g_Player.nCounterAnim = 0;
	g_Player.nPatternAnim = 0;
	g_Player.nDirectionMove = 1;
	g_Player.bJump = false;
	g_Player.bDisp = true;
	g_Player.nCounterState = 0;
	g_Player.BlockOld.type = BLOCKTYPE_NORMAL;
	g_Player.state = PLAYERSTATE_NORMAL;
	g_Player.pBlock = NULL;

	g_nCntStock = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEXTURENAME, &g_pTexturePlayer);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_SIZE, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_SIZE, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_SIZE, g_Player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_SIZE, g_Player.pos.y, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================
// �v���C���[�I������
//=============================================================================
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=============================================================================
// �v���C���[�X�V����
//=============================================================================
void UpdatePlayer(void)
{
	g_Player.posOld = g_Player.pos;	// �O��̈ʒu��ۑ�

	switch (g_Player.state)
	{
	case PLAYERSTATE_NONE:
		break;

	case PLAYERSTATE_APPEAR:
		g_Player.nCounterState++;
		if (g_Player.nCounterState >= 60)
		{
			g_Player.nCounterState = 0;
			g_Player.bDisp = true;
			// ���ʂ���o�Ă���
			g_Player.move.y = JUMP_HEIGHT * 3.15f;
			
			// ���n�_�Ŏ~�܂�
			if (g_Player.pos.y <= 350)
			{
				g_Player.move.y = 0;
				g_Player.state = PLAYERSTATE_NORMAL;
			}
		}
		break;

	case PLAYERSTATE_NORMAL:
		break;

	case PLAYERSTATE_DEATH:
		g_Player.bDisp = false;
		g_Player.state = PLAYERSTATE_NONE;
		PlaySound(SOUND_LABEL_SE_SCREAM000);
		SetGameState(GAMESTATE_END);
		return;
		break;

	case PLAYERSTATE_CLEAR:
		g_Player.state = PLAYERSTATE_NONE;
		SetGameState(GAMESTATE_CLEAR);
		return;
		break;
	}

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	if (GetKeyboardPress(DIK_D) == true && g_Player.pos.y <= SCREEN_HEIGHT)
	{// �E�ړ�
		g_Player.move.x += (g_Player.move.x + MOVE_SPEED) * 0.9f;
		g_Player.nDirectionMove = 1; // �E�������
	}

	if (GetKeyboardPress(DIK_A) == true && g_Player.pos.y <= SCREEN_HEIGHT)
	{// ���ړ�
		g_Player.move.x += (g_Player.move.x - MOVE_SPEED) * 0.9f;
		g_Player.nDirectionMove = 0; // ���������
	}

	if (g_Player.move.x < 0 || g_Player.move.x > 0)
	{// �v���C���[�ړ���
		VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

		g_Player.nCounterAnim++;

		if ((g_Player.nCounterAnim % ANIM_SPEED) == 0)
		{// �ړ����Ƀe�N�X�`���𓮂���
			g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % TEX_PATTERN;	//�p�^�[��No.�X�V

			//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//�e�N�X�`�����W
			pVtx[0].tex.x = TEX_UPDATE;
			pVtx[1].tex.x = TEX_UPDATE + 0.25f;
			pVtx[2].tex.x = TEX_UPDATE;
			pVtx[3].tex.x = TEX_UPDATE + 0.25f;

			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffPlayer->Unlock();
		}
	}
	else
	{// ����
		if (g_Player.nPatternAnim == 1 || g_Player.nPatternAnim == 3)
		{// �������̃e�N�X�`���ɂ��낦��
			g_Player.nCounterAnim++;

			if ((g_Player.nCounterAnim % ANIM_SPEED) == 0)
			{
				g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % TEX_PATTERN;	//�p�^�[��No.�X�V

				//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
				g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

				//�e�N�X�`�����W
				pVtx[0].tex.x = TEX_UPDATE;
				pVtx[1].tex.x = TEX_UPDATE + 0.25f;
				pVtx[2].tex.x = TEX_UPDATE;
				pVtx[3].tex.x = TEX_UPDATE + 0.25f;

				//���_�o�b�t�@���A�����b�N
				g_pVtxBuffPlayer->Unlock();
			}
		}
	}

	// �W�����v
	if (GetKeyboardTrigger(DIK_SPACE) == true && g_Player.bJump == false)
	{
		PlaySound(SOUND_LABEL_SE_JUMP);	// �W�����v��

		// �W�����v���������_���ɂ���
		if (rand() % 2 == 0)
		{// ���P
			PlaySound(SOUND_LABEL_SE_JUMP_VOICE000);
		}
		else if (rand() % 2 == 1)
		{// ���Q
			PlaySound(SOUND_LABEL_SE_JUMP_VOICE001);
		}

		g_Player.bJump = true;	// �W�����v��Ԃ�
		g_Player.move.y = JUMP_HEIGHT;
	}

	g_Player.move.y += GRAVITY;	// �d�͉��Z

	g_Player.pos.x += g_Player.move.x;	// ���ړ�
	g_Player.pos.y += g_Player.move.y;	// �c�ړ�

	g_Player.move.x = 0;	// ������}����

	// ��ʒ[����
	if (g_Player.pos.x < -30)
	{// ���[�֍s�����Ƃ�
		g_Player.pos.x = SCREEN_WIDTH + 30;		// ��ʊO�ɍs���Ȃ��悤�ɂ���
	}
	else if (g_Player.pos.x > SCREEN_WIDTH + 30)
	{// �E�[�֍s�����Ƃ�
		g_Player.pos.x = -30;					// ��ʊO�ɍs���Ȃ��悤�ɂ���
	}

	if (g_Player.pos.y - g_Player.fHeight >= SCREEN_HEIGHT && g_Player.state == PLAYERSTATE_NORMAL)
	{// �ʏ펞�̂ݗ�����
		g_Player.bDisp = false;
		PlaySound(SOUND_LABEL_SE_SPLASH);
		g_Player.move.y = 0.0f;

		g_nCntStock++;
		if (g_nCntStock < NUM_STOCK + 1)
		{// �c�@���P���炷
			SubtractionStock(g_nCntStock);
			g_Player.state = PLAYERSTATE_APPEAR;		
		}
		else
		{// �Q�[���I�[�o�[
			g_Player.state = PLAYERSTATE_DEATH;
		}
	}

	// �A�C�e���擾����
	CollisionItem();

	g_Player.pBlock = NULL;

	// �u���b�N�̓����蔻��
	if (g_Player.state == PLAYERSTATE_NORMAL)
	{
		if (CollisionBlock(&g_Player.pos, &g_Player.posOld, &g_Player.move, &g_Player.fHeight, &g_Player.fWidth) == true)
		{
			g_Player.bJump = false;	// �W�����v�t���O�𗎂Ƃ�
		}
		else
		{
			g_Player.bJump = true;	// �W�����v�t���O�𗧂Ă�

			if (g_Player.nPatternAnim == 0 || g_Player.nPatternAnim == 2)
			{// �󒆎��̃e�N�X�`���ɂ��낦��
				g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % TEX_PATTERN;	// �p�^�[��No.�X�V

				// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
				g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

				// �e�N�X�`�����W
				pVtx[0].tex.x = TEX_UPDATE;
				pVtx[1].tex.x = TEX_UPDATE + 0.25f;
				pVtx[2].tex.x = TEX_UPDATE;
				pVtx[3].tex.x = TEX_UPDATE + 0.25f;

				//���_�o�b�t�@���A�����b�N
				g_pVtxBuffPlayer->Unlock();
			}
		}
	}

	if (CollisionDestroyer(&g_Player.pos, g_Player.fHeight, g_Player.fWidth) == true && g_Player.state == PLAYERSTATE_NORMAL)
	{// �T���ƂԂ�����
		SetBlood();
		g_Player.state = PLAYERSTATE_DEATH;	
	}

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_SIZE, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_SIZE, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_SIZE, g_Player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_SIZE, g_Player.pos.y, 0.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(g_Player.nPatternAnim * 0.25f, g_Player.nDirectionMove * 0.5f);
	pVtx[1].tex = D3DXVECTOR2(g_Player.nPatternAnim * 0.25f + 0.25f, g_Player.nDirectionMove * 0.5f);
	pVtx[2].tex = D3DXVECTOR2(g_Player.nPatternAnim * 0.25f + 0.0f, (g_Player.nDirectionMove * 0.5f) + 0.5f);
	pVtx[3].tex = D3DXVECTOR2(g_Player.nPatternAnim * 0.25f + 0.25f, (g_Player.nDirectionMove * 0.5f) + 0.5f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================
// �v���C���[�`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X���擾����
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePlayer);

	if (g_Player.bDisp == true)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=============================================================================
// �v���C���[�̎擾
//=============================================================================
PLAYER GetPlayer(void)
{
	return g_Player;	// �v���C���[�̃f�[�^��Ԃ�
}

//=============================================================================
// �A�C�e���̓����蔻�菈��
//=============================================================================
void CollisionItem(void)
{
	int nCntItem;

	ITEM *pItem;
	pItem = GetItem();

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == true)
		{// �g�p����Ă���
			if (g_Player.pos.x + g_Player.fWidth >= pItem->pos.x - pItem->fWidth
				&& g_Player.pos.x - g_Player.fWidth <= pItem->pos.x + pItem->fWidth 
				&& g_Player.pos.y - g_Player.fHeight / 2 <= pItem->pos.y + pItem->fHeight 
				&& g_Player.pos.y >= pItem->pos.y - pItem->fHeight)
			{// �����蔻��
				pItem->state = ITEMSTATE_GET;
			}
		}
	}
}

