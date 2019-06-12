//=============================================================================
//
// �u���b�N���� [block.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "block.h"
#include "game.h"
#include "sound.h"
#include "block.h"
#include "destroyer.h"
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BLOCK_TEXTURENAME0	"data/TEXTURE/bar.png"
#define BLOCK_TEXTURENAME1	"data/TEXTURE/bard000.png"
#define BLOCK_TEXTURENAME2	"data/TEXTURE/bar001.png"
#define BLOCK_TEXTURENAME3	"data/TEXTURE/dorsal.png"
#define BLOCK_DRAW		(200)	// �u���b�N��`�悷��͈�
#define MAX_TEX			(4)		// �e�N�X�`���̐�
#define ANIM_SPEED		(7)		// �e�N�X�`���A�j���[�V�����̑��x
#define TEX_PATTERN		(8)		// �e�N�X�`���A�j���[�V�����̃p�^�[����
#define GOAL_HEIGHT		(25)	// �S�[���̒��̔w���̍���

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBlock[MAX_TEX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
BLOCK g_aBlock[MAX_BLOCK];	//�u���b�N�̏��
int g_nCntMove;		// ���̒ǂ��Ă��鎞��

//=============================================================================
// ����������
//=============================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �u���b�N�̏��̏�����
	for(nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].des = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].fWidth = 0.0f;
		g_aBlock[nCntBlock].fHeight = 0.0f;
		g_aBlock[nCntBlock].nCounterAnim = 0;
		g_aBlock[nCntBlock].nPatternAnim = 0;
		g_aBlock[nCntBlock].nDirectionMove = 0;
		g_aBlock[nCntBlock].type = BLOCKTYPE_NORMAL;
		g_aBlock[nCntBlock].state = BLOCKSTATE_NORMAL;
		g_aBlock[nCntBlock].nCounterState = 0;
		g_aBlock[nCntBlock].nCntMove = 0;
		g_aBlock[nCntBlock].nType = 0;
		g_aBlock[nCntBlock].nVertical = 0;
		g_aBlock[nCntBlock].bUse = false;
		g_aBlock[nCntBlock].bScroll = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME0, &g_pTextureBlock[0]);
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME1, &g_pTextureBlock[1]);
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME2, &g_pTextureBlock[2]);
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURENAME3, &g_pTextureBlock[3]);

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
									D3DUSAGE_WRITEONLY,
									FVF_VERTEX_2D,
									D3DPOOL_MANAGED,
									&g_pVtxBuffBlock,
									NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);

		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitBlock(void)
{
	for (int nCntBlock = 0; nCntBlock < MAX_TEX; nCntBlock++)
	{
		// �e�N�X�`���̊J��
		if (g_pTextureBlock[nCntBlock] != NULL)
		{
			g_pTextureBlock[nCntBlock]->Release();
			g_pTextureBlock[nCntBlock] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBlock(void)
{
	int nCntBlock;
	PLAYER pPlayer;
	pPlayer = GetPlayer();

	VERTEX_2D *pVtx;		// ���_���̍쐬

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// �ʒu�̕ۑ�
		g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			switch (g_aBlock[nCntBlock].state)
			{
			case BLOCKSTATE_NONE:
				break;

			case BLOCKSTATE_NORMAL:
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_ENEMY || g_aBlock[nCntBlock].type == BLOCKTYPE_HORIZONTAL)
				{
					g_aBlock[nCntBlock].bUse = false;
				}
				break;

			case BLOCKSTATE_MOVE:
				if (g_aBlock[nCntBlock].type != BLOCKTYPE_HORIZONTAL)
				{
					g_aBlock[nCntBlock].bScroll = true;
				}
				break;

			case BLOCKSTATE_CLEAR:
				g_aBlock[nCntBlock].nCounterState++;
				for (int nCntBlock2 = 0; nCntBlock2 < MAX_BLOCK; nCntBlock2++)
				{
					if (g_aBlock[nCntBlock2].bUse == true && g_aBlock[nCntBlock2].nType == 1)
					{// �����グ��
						g_aBlock[nCntBlock2].move.x = 0;
						g_aBlock[nCntBlock2].pos.y -= 5.5f;
					}
				}
					PlaySound(SOUND_LABEL_SE_FLY);
				if (g_aBlock[nCntBlock].nCounterState % 60 == 0)
				{// �N���A��ʂ�
					PlaySound(SOUND_LABEL_SE_BIRD);
					g_aBlock[nCntBlock].state = BLOCKSTATE_NONE;
					SetGameState(GAMESTATE_CLEAR);
				}
				break;
			}

			switch (g_aBlock[nCntBlock].type)
			{
			case BLOCKTYPE_NORMAL:		// �ʏ�
				if (g_aBlock[nCntBlock].pos.x < 0 - BLOCK_SIZE)
				{
					g_aBlock[nCntBlock].bUse = false;
				}

				g_aBlock[nCntBlock].nType = 1;	// ��

				break;

			case BLOCKTYPE_VERTICAL:	// �����ړ�
				if (g_aBlock[nCntBlock].pos.y >= g_aBlock[nCntBlock].des.y)
				{// ��ړ�
					g_aBlock[nCntBlock].move.y -= 1 * 0.02f;
				}
				else if (g_aBlock[nCntBlock].pos.y <= g_aBlock[nCntBlock].posOld.y)
				{// ���ړ�
					g_aBlock[nCntBlock].move.y += 1 * 0.02f;
				}

				if (g_aBlock[nCntBlock].pos.y >= SCREEN_HEIGHT || g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth <= 0)
				{// ��ʊO�ɍs�����Ƃ�
					g_aBlock[nCntBlock].bUse = false;
				}

				g_aBlock[nCntBlock].pos.y += g_aBlock[nCntBlock].move.y;

				g_aBlock[nCntBlock].nType = 1;	// ��

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

				break;

			case BLOCKTYPE_HORIZONTAL:	// �����ړ�
				if (g_aBlock[nCntBlock].pos.x >= g_aBlock[nCntBlock].des.x)
				{// ���ړ�
					g_aBlock[nCntBlock].move.x -= 1 * 0.02f;
				}
				else if (g_aBlock[nCntBlock].pos.x <= g_aBlock[nCntBlock].des.x)
				{// �E�ړ�
					g_aBlock[nCntBlock].move.x += 1 * 0.02f;
				}

				if (g_aBlock[nCntBlock].pos.x <= -BLOCK_DRAW || g_aBlock[nCntBlock].pos.x >= SCREEN_WIDTH + BLOCK_DRAW)
				{// ��ʊO�ɍs�����Ƃ�
					g_aBlock[nCntBlock].bUse = false;
				}

				g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;

				g_aBlock[nCntBlock].nType = 1;	// ��

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				break;

			case BLOCKTYPE_TITLE:
				if (g_aBlock[nCntBlock].pos.x >= g_aBlock[nCntBlock].des.x)
				{// ���ړ�
					g_aBlock[nCntBlock].move.x -= 1 * 0.02f;
				}
				else if (g_aBlock[nCntBlock].pos.x <= g_aBlock[nCntBlock].des.x)
				{// �E�ړ�
					g_aBlock[nCntBlock].move.x += 1 * 0.02f;
				}

				if (g_aBlock[nCntBlock].pos.x <= -BLOCK_DRAW || g_aBlock[nCntBlock].pos.x >= SCREEN_WIDTH + BLOCK_DRAW)
				{// ��ʊO�ɍs�����Ƃ�
					g_aBlock[nCntBlock].bUse = false;
				}

				g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;

				g_aBlock[nCntBlock].nType = 1;	// ��

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				break;

			case BLOCKTYPE_BELT_LEFT:	// �x���g�R���x�A�[(������)
					// �u���b�N�̈ړ�
				g_aBlock[nCntBlock].move.x -= (1 + g_aBlock[nCntBlock].move.x) * 0.01f;
				g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;

				if (g_aBlock[nCntBlock].pos.y >= g_aBlock[nCntBlock].des.y)
				{// ��ړ�
					g_aBlock[nCntBlock].move.y -= 1 * 0.02f;
				}
				else if (g_aBlock[nCntBlock].pos.y <= g_aBlock[nCntBlock].posOld.y)
				{// ���ړ�
					g_aBlock[nCntBlock].move.y += 1 * 0.02f;
				}

				if (g_aBlock[nCntBlock].pos.y <= 0)
				{// ����
					g_aBlock[nCntBlock].bUse = false;
				}

				g_aBlock[nCntBlock].pos.y += g_aBlock[nCntBlock].move.y;

				g_aBlock[nCntBlock].nType = 0;	// ��

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;

			case BLOCKTYPE_BELT_RIGHT:	// �x���g�R���x�A�[(�E����)				
					// �u���b�N�̈ړ�
				g_aBlock[nCntBlock].move.x -= (1 + g_aBlock[nCntBlock].move.x) * 0.01f;
				g_aBlock[nCntBlock].pos.x -= g_aBlock[nCntBlock].move.x;

				if (g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth > SCREEN_WIDTH / 3 - 50)
				{// �^�񒆂�����ɂ����Ƃ�
					g_aBlock[nCntBlock].pos.y += 1;
					if (g_aBlock[nCntBlock].pos.y >= SCREEN_HEIGHT)
					{
						g_aBlock[nCntBlock].bUse = false;
					}
				}

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
				break;

			case BLOCKTYPE_FALL:	// ������
				if (g_aBlock[nCntBlock].pos.y >= SCREEN_HEIGHT || g_aBlock[nCntBlock].pos.x < -200)
				{
					g_aBlock[nCntBlock].bUse = false;
				}

				g_aBlock[nCntBlock].nType = 1;	// ��

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				break;

			case BLOCKTYPE_GOAL:		// �S�[��
				g_aBlock[nCntBlock].nType = 1;	// ��
				g_aBlock[nCntBlock].nDirectionMove = 1;

				if (g_aBlock[nCntBlock].pos.x <= 1000)
				{// ���n�_�Ŏ~�܂�
					g_aBlock[nCntBlock].bScroll = false;
				}

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(0.7f, 1.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.7f, 1.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.7f, 1.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.7f, 1.0f, 0.0f, 1.0f);
				break;

			case BLOCKTYPE_SHARK:
				if (g_aBlock[nCntBlock].pos.x >= g_aBlock[nCntBlock].des.x)
				{// ���ړ�
					g_aBlock[nCntBlock].move.x -= 1 * 0.02f;
				}
				else if (g_aBlock[nCntBlock].pos.x <= g_aBlock[nCntBlock].des.x)
				{// �E�ړ�
					g_aBlock[nCntBlock].move.x += 1 * 0.02f;
				}

				if (g_aBlock[nCntBlock].pos.y >= g_aBlock[nCntBlock].des.y)
				{// ��ړ�
					g_aBlock[nCntBlock].move.y -= 1 * 0.02f;
				}
				else if (g_aBlock[nCntBlock].pos.y <= g_aBlock[nCntBlock].posOld.y)
				{// ���ړ�
					g_aBlock[nCntBlock].move.y += 1 * 0.02f;
				}

				if (g_aBlock[nCntBlock].pos.x <= -BLOCK_DRAW || g_aBlock[nCntBlock].pos.x >= SCREEN_WIDTH + BLOCK_DRAW)
				{// ��ʊO�ɍs�����Ƃ�
					g_aBlock[nCntBlock].bUse = false;
				}

				g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;
				g_aBlock[nCntBlock].pos.y += g_aBlock[nCntBlock].move.y;

				g_aBlock[nCntBlock].nType = 3;	// �w�т�

				// �e�N�X�`�����W
				pVtx[0].tex = D3DXVECTOR2(0.0f, g_aBlock[nCntBlock].nDirectionMove * 0.5f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, g_aBlock[nCntBlock].nDirectionMove * 0.5f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, g_aBlock[nCntBlock].nDirectionMove * 0.5f + 0.5f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, g_aBlock[nCntBlock].nDirectionMove * 0.5f + 0.5f);

				break;

			case BLOCKTYPE_ENEMY:	// �˂�����ł���
				static float fAngle;
				D3DXVECTOR3 BlockMove(0.0f, 0.0f, 0.0f);

				g_aBlock[nCntBlock].nCntMove++;
				if (g_aBlock[nCntBlock].state != BLOCKSTATE_CLEAR)
				{
					if (g_aBlock[nCntBlock].nCntMove <= 60)
					{// �ǂ��Ă��鎞��
						fAngle = atan2f(pPlayer.pos.x - g_aBlock[nCntBlock].pos.x, pPlayer.pos.y - g_aBlock[nCntBlock].pos.y);

						BlockMove.x = sinf(fAngle) * 5.0f;
						BlockMove.y = cosf(fAngle) * 5.0f;

						g_aBlock[nCntBlock].move = BlockMove;
					}
				}
				else
				{
					g_aBlock[nCntBlock].move.y -= 15.0f;
				}

				if (g_aBlock[nCntBlock].pos.x <= -BLOCK_DRAW
					&& g_aBlock[nCntBlock].pos.x >= SCREEN_WIDTH + BLOCK_DRAW
					&& g_aBlock[nCntBlock].pos.y <= -BLOCK_DRAW
					&& g_aBlock[nCntBlock].pos.y >= SCREEN_HEIGHT + BLOCK_DRAW)
				{
					g_aBlock[nCntBlock].bUse = false;
				}

				if (g_aBlock[nCntBlock].pos.x <= pPlayer.pos.x)
				{// �v���C���[��荶���ŉE����
					g_aBlock[nCntBlock].nDirectionMove = 0;
				}
				else
				{// �v���C���[���E���ō�����
					g_aBlock[nCntBlock].nDirectionMove = 1;
				}
				g_aBlock[nCntBlock].nType = 1;	// ��

				g_aBlock[nCntBlock].pos += g_aBlock[nCntBlock].move;

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				break;
			}

			if (pPlayer.pos.x >= SCREEN_WIDTH / 3 && g_aBlock[nCntBlock].state == BLOCKSTATE_NORMAL)
			{// �����X�N���[��
				g_aBlock[nCntBlock].state = BLOCKSTATE_MOVE;
			}
			else if (g_aBlock[nCntBlock].type == BLOCKTYPE_GOAL && g_aBlock[nCntBlock].pos.x <= 1000)
			{
				g_aBlock[nCntBlock].state = BLOCKSTATE_NONE;
			}

			if (g_aBlock[nCntBlock].bScroll == true)
			{// ��ʃX�N���[��
				g_aBlock[nCntBlock].pos.x -= SCROLL_SPEED;
			}

			g_aBlock[nCntBlock].nCounterAnim++;

			if (g_aBlock[nCntBlock].nType == 1)
			{
				if ((g_aBlock[nCntBlock].nCounterAnim % ANIM_SPEED) == 0)
				{// �e�N�X�`���𓮂���
					g_aBlock[nCntBlock].nPatternAnim = (g_aBlock[nCntBlock].nPatternAnim + 1) % TEX_PATTERN;	//�p�^�[��No.�X�V

					// �e�N�X�`�����W
					pVtx[0].tex = D3DXVECTOR2((g_aBlock[nCntBlock].nPatternAnim % 8) * 0.125f, g_aBlock[nCntBlock].nDirectionMove * 0.5f);
					pVtx[1].tex = D3DXVECTOR2((g_aBlock[nCntBlock].nPatternAnim % 8) * 0.125f + 0.125f, g_aBlock[nCntBlock].nDirectionMove * 0.5f);
					pVtx[2].tex = D3DXVECTOR2((g_aBlock[nCntBlock].nPatternAnim % 8) * 0.125f, g_aBlock[nCntBlock].nDirectionMove * 0.5f + 0.5f);
					pVtx[3].tex = D3DXVECTOR2((g_aBlock[nCntBlock].nPatternAnim % 8) * 0.125f + 0.125f, g_aBlock[nCntBlock].nDirectionMove * 0.5f + 0.5f);
				}
			}

			if (CollisionDestroyer(&g_aBlock[nCntBlock].pos, g_aBlock[nCntBlock].fHeight, g_aBlock[nCntBlock].fWidth) == true)
			{// �f�X�g���C���[�ɐG�ꂽ��
				if (g_aBlock[nCntBlock].nType == 0)
				{
					g_aBlock[nCntBlock].nType = 2;
				}
				else if (g_aBlock[nCntBlock].nType == 0 && g_aBlock[nCntBlock].type != BLOCKTYPE_HORIZONTAL)
				{
					g_aBlock[nCntBlock].bUse = false;
				}
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight, 0.0f);
		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for(int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{//�u���b�N���g�p����Ă���
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBlock[g_aBlock[nCntBlock].nType]);

			if (g_aBlock[nCntBlock].pos.x >= -BLOCK_DRAW
				&& g_aBlock[nCntBlock].pos.x <= SCREEN_WIDTH + BLOCK_DRAW
				&& g_aBlock[nCntBlock].pos.y >= -BLOCK_DRAW
				&& g_aBlock[nCntBlock].pos.y <= SCREEN_HEIGHT + BLOCK_DRAW)
			{
				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBlock, 2);
			}
		}
	}
}

//=============================================================================
// �u���b�N�̐ݒ�
//=============================================================================
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXVECTOR3 des, BLOCKTYPE type)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntBlock;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{//�u���b�N���g�p����Ă��Ȃ�
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f,0.0f,0.0f);
			g_aBlock[nCntBlock].fWidth = fWidth;
			g_aBlock[nCntBlock].fHeight = fHeight;
			g_aBlock[nCntBlock].des = des;
			g_aBlock[nCntBlock].type = type;
			g_aBlock[nCntBlock].bUse = true;	//�g�p���Ă����Ԃɂ���

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + fWidth, g_aBlock[nCntBlock].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + fWidth, g_aBlock[nCntBlock].pos.y + fHeight, 0.0f);

			if (g_aBlock[nCntBlock].nType == -100)
			{// �e�N�X�`���𕪊����ē\��
				//�e�N�X�`�����W
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aBlock[nCntBlock].fWidth / BLOCK_SIZE, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, g_aBlock[nCntBlock].fHeight / BLOCK_SIZE);
				pVtx[3].tex = D3DXVECTOR2(g_aBlock[nCntBlock].fWidth / BLOCK_SIZE, g_aBlock[nCntBlock].fHeight / BLOCK_SIZE);
			}
			else
			{
				//�e�N�X�`�����W
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			if (g_aBlock[nCntBlock].pos.x > SCREEN_WIDTH / 2)
			{// ��ʔ������E��
				g_aBlock[nCntBlock].nDirectionMove = 1;
			}
			else if (g_aBlock[nCntBlock].pos.x < SCREEN_WIDTH / 2)
			{// ��ʔ�����荶��
				g_aBlock[nCntBlock].nDirectionMove = 0;
			}

				break;
		}
		pVtx += 4;
	}
	
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// �u���b�N�̎擾
//=============================================================================
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}

//=============================================================================
// �u���b�N�̓����蔻�菈��
//=============================================================================
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float *pHeight, float *pWidth)
{
	int nCntBlock;
	bool bLand = false;

	// �v���C���[�̏����擾
	PLAYER pPlayer;
	pPlayer = GetPlayer();

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{// �g�p����Ă����
			if (pPos->x + *pWidth >= g_aBlock[nCntBlock].pos.x && pPos->x - *pWidth <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth)
			{// �u���b�N�̕��͈͓̔�
				if (pPos->y > g_aBlock[nCntBlock].pos.y && pPosOld->y <= g_aBlock[nCntBlock].posOld.y)
				{// �ォ�瓖��������
					bLand = true;
					pPos->y = g_aBlock[nCntBlock].pos.y;
					pMove->y = 0.0f;
					pPlayer.BlockOld = g_aBlock[nCntBlock];	// �Ō�ɏ�����u���b�N��ۑ�

					if (g_aBlock[nCntBlock].bScroll == true)
					{// �X�N���[���ړ�
						pPos->x -= SCROLL_SPEED;
					}

					if (g_aBlock[nCntBlock].move.y <= -5.0f)
					{// �c�̈ړ��ɂ��Ă���
						pPos->y = g_aBlock[nCntBlock].pos.y - 5.0f;
					}
					if (g_aBlock[nCntBlock].move.y >= 0.0f)
					{// �c�̈ړ��ɂ��Ă���
						pPos->y = g_aBlock[nCntBlock].pos.y;
					}

					if (g_aBlock[nCntBlock].type == BLOCKTYPE_HORIZONTAL)
					{// �����ړ��u���b�N
						pPos->x += g_aBlock[nCntBlock].move.x;
					}
					if (g_aBlock[nCntBlock].type == BLOCKTYPE_BELT_LEFT)
					{// �x���g�R���x�A�[(������)
						pPos->x += g_aBlock[nCntBlock].move.x;
						if (g_aBlock[nCntBlock].pos.y < g_aBlock[nCntBlock].des.y
							&& g_aBlock[nCntBlock].pos.y < 680)
						{// ���܂���
							g_aBlock[nCntBlock].move.y += 1 * 0.05f;
						}
					}
					if (g_aBlock[nCntBlock].type == BLOCKTYPE_BELT_RIGHT)
					{// �x���g�R���x�A�[(�E����)
						pPos->x -= g_aBlock[nCntBlock].move.x;
					}
					if (g_aBlock[nCntBlock].type == BLOCKTYPE_FALL)
					{// ������
						if (g_aBlock[nCntBlock].pos.y < 700)
						{
							g_aBlock[nCntBlock].pos.y++;
						}
					}
					if (g_aBlock[nCntBlock].type == BLOCKTYPE_GOAL)
					{// �S�[��
						pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2;
						pMove->y = 0.0f;
						bLand = false;
						if (g_aBlock[nCntBlock].state == BLOCKSTATE_NONE)
						{// �N���A
							g_aBlock[nCntBlock].state = BLOCKSTATE_CLEAR;
						}
					}
					if (g_aBlock[nCntBlock].type == BLOCKTYPE_ENEMY)
					{// �˂�����ł�����
						pMove->y = JUMP_HEIGHT;
						PlaySound(SOUND_LABEL_SE_SCORE_UP);
						AddScore(4500);
						g_aBlock[nCntBlock].pos.y += 25.0f;
					}
				}
				else if (pPos->y - *pHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight && pPosOld->y - *pHeight > g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
				{// �����瓖��������
					pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + *pHeight;
					pMove->y = 0.0f;
				}
				else if (pPos->y > g_aBlock[nCntBlock].pos.y && pPos->y <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight ||					// �������͈͓�
					pPos->y - *pHeight >= g_aBlock[nCntBlock].pos.y && pPos->y - *pHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight ||	// �����͈͓�
					pPos->y - *pHeight <= g_aBlock[nCntBlock].pos.y && pPos->y > g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)				// �u���b�N�����Ƒ��̊Ԃɂ���
				{// �����瓖��������
					if (pPos->x < g_aBlock[nCntBlock].pos.x + 10)
					{// ������
						pPos->x = g_aBlock[nCntBlock].pos.x - *pWidth;
						pMove->x = 0.0f;
					}
					else if (pPos->x > g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2)
					{// �E����
						pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth + *pWidth;
						pMove->x = 0.0f;
					}
				}
			}

			if (bLand == false)
			{// �u���b�N���痣�ꂽ���̏���
				if (pPlayer.BlockOld.type == BLOCKTYPE_HORIZONTAL)
				{// �����ړ��u���b�N
					if (pPlayer.BlockOld.move.x < 0)
					{// ���ړ�
						pPlayer.pos.x += pPlayer.BlockOld.move.x * 0.015f;
					}
					else if(pPlayer.BlockOld.move.x > 0)
					{// �E�ړ�
						pPlayer.pos.x += pPlayer.BlockOld.move.x * 0.015f;
					}
				}
				else if (pPlayer.BlockOld.type == BLOCKTYPE_BELT_LEFT)
				{// �������x���g�R���x�A�[
					pPlayer.pos.x += g_aBlock[nCntBlock].move.x * 0.08f;
				}
				else if (pPlayer.BlockOld.type == BLOCKTYPE_BELT_RIGHT)
				{// �E�����x���g�R���x�A�[
					pPlayer.pos.x -= g_aBlock[nCntBlock].move.x * 0.08f;
				}
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_FALL || 
					pPlayer.BlockOld.type == BLOCKTYPE_BELT_LEFT)
				{
					if (g_aBlock[nCntBlock].pos.y > g_aBlock[nCntBlock].des.y)
					{// �~���Əオ��
						g_aBlock[nCntBlock].pos.y -= 5.0f;
					}
				}
			}
		}
	}
	return bLand;
}
