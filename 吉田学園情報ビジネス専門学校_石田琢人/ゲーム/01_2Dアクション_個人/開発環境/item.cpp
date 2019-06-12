//=============================================================================
//
// �A�C�e������ [item.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "item.h"
#include "input.h"
#include "score.h"
#include "player.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ITEM_TEXTURENAME_0	"data/TEXTURE/egg.png"
#define ITEM_TEXTURENAME_1	"data/TEXTURE/explosion000.png"
#define TEX_PATTERN	(4)	// �e�N�X�`���̃p�^�[����
#define MAX_TEXTURE	(2)	// �e�N�X�`���̐�
#define TEX_UPDATE_SPEED	(8)	// �e�N�X�`���̍X�V���x
#define EGG_SIZE	(30.0f)	// ���̑傫��
#define EGG_SIZE_Y	(35.0f)	// ���̑傫��

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureItem[MAX_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
ITEM g_aItem[MAX_ITEM];	//�A�C�e���̏��
int g_nTotalEGG;

//=============================================================================
// ����������
//=============================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntItem;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �A�C�e���̏��̏�����
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].fWidth = 0.0f;
		g_aItem[nCntItem].fHeight = 0.0f;
		g_aItem[nCntItem].fAngle = 0.0f;
		g_aItem[nCntItem].fLength = 0.0f;
		g_aItem[nCntItem].nCounterAnim = 0;
		g_aItem[nCntItem].nPatternAnim = 0;
		g_aItem[nCntItem].type = ITEMTYPE_NONE;
		g_aItem[nCntItem].state = ITEMSTATE_NORMAL;
		g_aItem[nCntItem].nCntState = 0;
		g_aItem[nCntItem].bUse = false;
	}

	g_nTotalEGG = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_0, &g_pTextureItem[0]);
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURENAME_1, &g_pTextureItem[1]);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE_Y, 0.0f);

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

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitItem(void)
{
	int nCntItem;

	for (nCntItem = 0; nCntItem < MAX_TEXTURE; nCntItem++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureItem[nCntItem] != NULL)
		{
			g_pTextureItem[nCntItem]->Release();
			g_pTextureItem[nCntItem] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateItem(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	PLAYER Player;

	Player = GetPlayer();

	int nCntItem;

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		switch (g_aItem[nCntItem].type)
		{
		case ITEMTYPE_NONE:
			break;

		case ITEMTYPE_EGG:
			if (g_aItem[nCntItem].state == ITEMSTATE_APPEAR)
			{
				PlaySound(SOUND_LABEL_SE_EGG);
				g_aItem[nCntItem].type = ITEMTYPE_NONE;
				AddScore(5000);
			}
			break;

		case ITEMTYPE_DIRTY:
			if (g_aItem[nCntItem].state == ITEMSTATE_APPEAR)
			{
				PlaySound(SOUND_LABEL_SE_DRESSING);
				PlaySound(SOUND_LABEL_SE_SCREAM001);
				g_aItem[nCntItem].type = ITEMTYPE_NONE;
				SetEffect(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 600, 120, EFFECTSTATE_DIRTY);
			}
			break;
		}

		switch (g_aItem[nCntItem].state)
		{
		case ITEMSTATE_APPEAR:
			break;

		case ITEMSTATE_NORMAL:
			break;

		case ITEMSTATE_GET:
			g_aItem[nCntItem].nCntState++;
			g_aItem[nCntItem].pos.y -= 15.0f;
			if (g_aItem[nCntItem].nCntState >= 10)
			{
				g_aItem[nCntItem].state = ITEMSTATE_APPEAR;
				g_aItem[nCntItem].bUse = false;
			}
			break;

		case ITEMSTATE_MOVE:
			g_aItem[nCntItem].pos.x -= SCROLL_SPEED;
			break;
		}

		if (Player.pos.x >= SCREEN_WIDTH / 3 && g_aItem[nCntItem].state == ITEMSTATE_NORMAL)
		{
			g_aItem[nCntItem].state = ITEMSTATE_MOVE;
		}
		


		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE_Y, 0.0f);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffItem->Unlock();

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].nCounterAnim++;

			if (g_aItem[nCntItem].bUse == true)
			{// �A�C�e�����g�p����Ă���
				if (g_aItem[nCntItem].type == ITEMTYPE_EGG)
				{// ��
					//if ((g_aItem[nCntItem].nCounterAnim % TEX_UPDATE_SPEED) == 0)
					//{
						//// �p�^�[��No.�̍X�V
						//g_aItem[nCntItem].nPatternAnim = (g_aItem[nCntItem].nPatternAnim + 1) % 4;

						//g_aItem[nCntItem].fAngle = atan2f(-EGG_SIZE, -EGG_SIZE_Y);
						//g_aItem[nCntItem].fLength = sqrtf(EGG_SIZE * EGG_SIZE + EGG_SIZE_Y * EGG_SIZE_Y);

						//// ���_�̍��W
						//pVtx[0].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;
						//pVtx[0].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;

						//g_aItem[nCntItem].fAngle = atan2f(EGG_SIZE, -EGG_SIZE_Y);

						//pVtx[1].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;
						//pVtx[1].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;

						//g_aItem[nCntItem].fAngle = atan2f(-EGG_SIZE, EGG_SIZE_Y);

						//pVtx[2].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;
						//pVtx[2].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;

						//g_aItem[nCntItem].fAngle = atan2f(EGG_SIZE, EGG_SIZE_Y);

						//pVtx[3].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;
						//pVtx[3].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle) * g_aItem[nCntItem].fLength;

						//// �e�N�X�`�����W���X�V
						//pVtx[0].tex = D3DXVECTOR2(0.0f + (0.25f * g_aItem[nCntItem].nPatternAnim), 0.0f);
						//pVtx[1].tex = D3DXVECTOR2(0.25f + (0.25f * g_aItem[nCntItem].nPatternAnim), 0.0f);
						//pVtx[2].tex = D3DXVECTOR2(0.0f + (0.25f * g_aItem[nCntItem].nPatternAnim), 1.0f);
						//pVtx[3].tex = D3DXVECTOR2(0.25f + (0.25f * g_aItem[nCntItem].nPatternAnim), 1.0f);
					//}
				}
				else if (g_aItem[nCntItem].type == ITEMTYPE_DIRTY)
				{// ����
					if ((g_aItem[nCntItem].nCounterAnim % TEX_UPDATE_SPEED - 6) == 0)
					{
						// �p�^�[��No.�̍X�V
						g_aItem[nCntItem].nPatternAnim = (g_aItem[nCntItem].nPatternAnim + 1) % 8;

						//���_�J���[
						pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.3f, 1.0f);
						pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.3f, 1.0f);
						pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.3f, 1.0f);
						pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.3f, 1.0f);

						// �e�N�X�`�����W���X�V
						pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f * g_aItem[nCntItem].nPatternAnim), 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.125f + (0.125f * g_aItem[nCntItem].nPatternAnim), 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.0f + (0.125f * g_aItem[nCntItem].nPatternAnim), 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.125f + (0.125f * g_aItem[nCntItem].nPatternAnim), 1.0f);
					}
				}
			}
		pVtx += 4;	// ���_�f�[�^�̃|�C���^��4�i�߂�
	}
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntItem;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)	// �g�p����Ă���
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureItem[g_aItem[nCntItem].type - 1]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntItem, 2);
		}
	}
}

//=============================================================================
// �A�C�e���̐ݒ菈��
//=============================================================================
void SetItem(D3DXVECTOR3 pos, ITEMTYPE type)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntItem;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if(type == ITEMTYPE_EGG)
		{
			if (g_aItem[nCntItem].bUse == false)	//�e���g�p����Ă��Ȃ�
			{
				g_aItem[nCntItem].pos = pos;
				g_aItem[nCntItem].type = type;
				g_aItem[nCntItem].nCounterAnim = 0;
				g_aItem[nCntItem].nPatternAnim = 0;
				g_aItem[nCntItem].fWidth = EGG_SIZE;
				g_aItem[nCntItem].fHeight = EGG_SIZE_Y;
				g_aItem[nCntItem].bUse = true;	//�g�p���Ă����Ԃɂ���

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE_Y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE_Y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE_Y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE_Y, 0.0f);
				g_nTotalEGG++;

				break;
			}
		}
		else if (type == ITEMTYPE_DIRTY)
		{
			if (g_aItem[nCntItem].bUse == false)	//�e���g�p����Ă��Ȃ�
			{
				g_aItem[nCntItem].pos = pos;
				g_aItem[nCntItem].type = type;
				g_aItem[nCntItem].nCounterAnim = 0;
				g_aItem[nCntItem].nPatternAnim = 0;
				g_aItem[nCntItem].fWidth = EGG_SIZE;
				g_aItem[nCntItem].fHeight = EGG_SIZE_Y;
				g_aItem[nCntItem].bUse = true;	//�g�p���Ă����Ԃɂ���

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y - EGG_SIZE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + EGG_SIZE, g_aItem[nCntItem].pos.y + EGG_SIZE, 0.0f);
				break;
			}
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// �A�C�e���̎擾
//=============================================================================
ITEM *GetItem(void)
{
	return &g_aItem[0];
}
