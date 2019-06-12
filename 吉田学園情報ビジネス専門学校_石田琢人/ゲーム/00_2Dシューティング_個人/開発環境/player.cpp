//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "game.h"
#include "sound.h"
#include "life.h"
#include "beam.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_TEXTURENAME	"data/TEXTURE/player000.png"	//�e�N�X�`����
#define PLAYER_POS_X	(50)	//����X���W
#define PLAYER_POS_Y	(50)	//����Y���W
#define PLAYER_WIDTH	(100)	//��
#define PLAYER_HEIGHT	(100)	//����
#define PLAYER_SPEED	(1.5f)		//�ړ����x
#define PLAYER_MOVE_X	(350.0f)	//�ړ�X���W
#define PLAYER_MOVE_Y	(250.0f)	//�ړ�Y���W
#define PLAYER_SLOWING	(0.1f)		//����
#define DAMAGE_TIME		(5)	//�_���[�W��Ԃ̎���
#define PLAYER_LIFE		(5)	//�v���C���[�̗̑�
#define PLAYER_STOCK	(3)	//�v���C���[�̎c�@
#define MAX_GAUGE		(200)	//�r�[�������̂ɕK�v�Ȓl

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posPlayer;	//�|���S���̈ʒu
D3DXVECTOR3 g_movePlayer;	//�|���S���̈ړ���
Player g_player;
int g_nMoveX;	//���x�ɕK�v
int g_nMoveY;	//���x�ɕK�v

//=============================================================================
// �v���C���[����������
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�v���C���[�̏��̏�����
	g_player.bDisp = true;
	g_player.nLife = PLAYER_LIFE;
	g_player.stock = PLAYER_STOCK;
	g_player.state = PLAYERSTATE_NOMAL;
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.nCounterState = 0;
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_posPlayer = D3DXVECTOR3(SCREEN_WIDTH / 2, 400, 0.0f);	//�|���S���̈ʒu
	g_nMoveX = 0;
	g_nMoveY = 0;


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
	pVtx[0].pos = D3DXVECTOR3(PLAYER_POS_X - 10, PLAYER_POS_Y - 10, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PLAYER_POS_X + 10, PLAYER_POS_Y - 10, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PLAYER_POS_X - 10, PLAYER_POS_Y + 10, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PLAYER_POS_X + 10, PLAYER_POS_Y + 10, 0.0f);

	//1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

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
	switch (g_player.state)
	{
	case PLAYERSTATE_APPEAR:
		g_player.nCounterState--;
		if (g_player.nCounterState <= 0)
		{
			InitLife();
			g_player.nLife = PLAYER_LIFE;
			g_posPlayer = D3DXVECTOR3(SCREEN_WIDTH / 2, 400, 0.0f);
			g_player.bDisp = true;
			g_player.state = PLAYERSTATE_NOMAL;
			VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

			//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffPlayer->Unlock();
		}
		break;

	case PLAYERSTATE_NOMAL:
		break;

	case PLAYERSTATE_DAMAGE:
		g_player.nCounterState--;
		if (g_player.nCounterState <= 0)
		{
			g_player.state = PLAYERSTATE_NOMAL;

			VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

			//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffPlayer->Unlock();
		}
		break;

	case PLAYERSTATE_DEATH:
		g_player.state = PLAYERSTATE_NOMAL;
		SetGameState(GAMESTATE_END);
		return;
		break;

	case PLAYERSTATE_BEAM:
		g_player.nCounterState--;
		if (g_player.nCounterState <= 0)
		{
			g_player.state = PLAYERSTATE_NOMAL;
		}
		break;
	}

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_player.state == PLAYERSTATE_NOMAL || g_player.state == PLAYERSTATE_DAMAGE)
	{
		//�C�ӂ̃L�[(A�L�[)�������ꂽ���ǂ���
		if (GetKeyboardPress(DIK_A) == true)
		{
			if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������ꂽ
			 //�|���S��������Ɉړ�
				g_movePlayer.x += sinf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
				g_movePlayer.y += cosf(-D3DX_PI * 0.75f) * PLAYER_SPEED;

				MovePlayer();
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������ꂽ
			 //�|���S���������Ɉړ�
				g_movePlayer.x += sinf(-D3DX_PI * 0.25) * PLAYER_SPEED;
				g_movePlayer.y += cosf(-D3DX_PI * 0.25) * PLAYER_SPEED;

				MovePlayer();
			}
			else
			{//���Ɉړ�
				g_movePlayer.x -= PLAYER_SPEED;	//�|���S�������Ɉړ�

				MovePlayer();
			}
		}
		//�C�ӂ̃L�[(D�L�[)�������ꂽ���ǂ���
		else if (GetKeyboardPress(DIK_D) == true)
		{
			if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������ꂽ
			 //�|���S�����E��Ɉړ�
				g_movePlayer.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED;
				g_movePlayer.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED;

				MovePlayer();
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������ꂽ
			 //�|���S�����E���Ɉړ�
				g_movePlayer.x += sinf(D3DX_PI * 0.25) * PLAYER_SPEED;
				g_movePlayer.y += cosf(D3DX_PI * 0.25) * PLAYER_SPEED;

				MovePlayer();
			}
			else
			{//�E�Ɉړ�
				g_movePlayer.x += PLAYER_SPEED;	//�|���S�����E�Ɉړ�

				MovePlayer();
			}
		}
		//�C�ӂ̃L�[(W�L�[)�������ꂽ���ǂ���
		else if (GetKeyboardPress(DIK_W) == true)
		{//��Ɉړ�
			g_movePlayer.y -= PLAYER_SPEED;	//�|���S������Ɉړ�

			MovePlayer();
		}
		//�C�ӂ̃L�[(S�L�[)�������ꂽ���ǂ���
		else if (GetKeyboardPress(DIK_S) == true)
		{//���Ɉړ�
			g_movePlayer.y += PLAYER_SPEED;	//�|���S�������Ɉړ�

			MovePlayer();
		}

		//�ʒu�X�V(�ړ�)
		g_posPlayer.x += g_movePlayer.x;
		g_posPlayer.y += g_movePlayer.y;

		//����
		g_movePlayer.x += (0.0f - g_movePlayer.x) * PLAYER_SLOWING;
		g_movePlayer.y += (0.0f - g_movePlayer.y) * PLAYER_SLOWING;
	}
	if (g_player.bDisp == true)
	{//�v���C���[�������Ă���
		//space�L�[�������ꂽ
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{//�e������
			PlaySound(SOUND_LABEL_SE_SHOT000);

			SetBullet(g_posPlayer, D3DXVECTOR3(0.0f, -10.0f, 0.0f), BULLETTYPE_PLAYER);		//�O
			SetBullet(g_posPlayer, D3DXVECTOR3(-2.0f, -10.0f, 0.0f), BULLETTYPE_PLAYER);	//���O
			SetBullet(g_posPlayer, D3DXVECTOR3(2.0f, -10.0f, 0.0f), BULLETTYPE_PLAYER);		//�E��
		}

		if (GetKeyboardTrigger(DIK_H) == true)
		{// �ǔ��e
			PlaySound(SOUND_LABEL_SE_SHOT000);

			D3DXVECTOR3 BulletMove(0.0f, 0.0f, 0.0f);
			static float fAngle;
			int nCntEnemy;
			Enemy *pEnemy;
			pEnemy = GetEnemy();

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				if (pEnemy->bUse == false)
				{//�G�l�~�[������ł���
					pEnemy++;
				}

				if (pEnemy->bUse == true)
				{//�G�l�~�[�������Ă���
					fAngle = atan2f(pEnemy->pos.x - g_player.pos.x, pEnemy->pos.y - g_player.pos.y);

					BulletMove.x = sinf(fAngle) * 5.0f;
					BulletMove.y = cosf(fAngle) * 5.0f;
				}
			}
			SetBullet(g_player.pos, BulletMove, BULLETTYPE_PLAYER);
		}

		BEAM *pBeam;
		pBeam = GetBeam();

		if (GetKeyboardTrigger(DIK_V) == true)
		{//�r�[��
			if (pBeam->nCntGauge >= MAX_GAUGE)
			{
				PlaySound(SOUND_LABEL_SE_BEAM);

				g_player.nCounterState = 170;
				g_player.state = PLAYERSTATE_BEAM;

				SetEffect(g_player.pos, D3DXCOLOR(0.0f, 0.3f, 1.0f, 1.0f), 15.0f, 170, EFFECTSTATE_EXPLOSION);
				SetBullet(g_player.pos + D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), BULLETTYPE_PLAYER_BEAM);
			}
		}
	}

	if (g_posPlayer.x < 0.0f + 25.0f)
	{// X���W�����l�𒴂�����ʒu��߂�
		g_posPlayer.x = 0.0f + 25.0f;
	}
	else if (g_posPlayer.x > SCREEN_WIDTH - 25.0f)
	{// X���W�����l�𒴂�����ʒu��߂�
		g_posPlayer.x = SCREEN_WIDTH - 25.0f;
	}

	if(g_posPlayer.y < 0.0f + 25.0f)
	{// Y���W�����l�𒴂�����ʒu��߂�
		g_posPlayer.y = 0.0f + 25.0f;
	}
	else if (g_posPlayer.y > SCREEN_HEIGHT - 25.0f)
	{// Y���W�����l�𒴂�����ʒu��߂�
		g_posPlayer.y = SCREEN_HEIGHT - 25.0f;
	}

	//���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(g_posPlayer.x - 25, g_posPlayer.y - 25, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posPlayer.x + 25, g_posPlayer.y - 25, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posPlayer.x - 25, g_posPlayer.y + 25, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posPlayer.x + 25, g_posPlayer.y + 25, 0.0f);

	g_player.pos = g_posPlayer;
	g_player.move = g_movePlayer;

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================
// �v���C���[�`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X���擾����
	pDevice = GetDevice();

	if (g_player.bDisp == true)
	{
		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	}
}

//=============================================================================
// �v���C���[�̃_���[�W����
//=============================================================================
void HitPlayer(int nDamage)
{
	if (g_player.state == PLAYERSTATE_BEAM)
	{
	}
	else
	{
		SubtractionLife(g_player.nLife);		//�̗̓e�N�X�`�������炷
		g_player.nLife -= nDamage;
		//LIFE *pLife;
		//pLife = GetLife();

		if (g_player.nLife <= 0)
		{
			PlaySound(SOUND_LABEL_SE_EXPLOSION);
			//SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			SetEffect(g_player.pos, D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f), 15.0f, 30, EFFECTSTATE_EXPLOSION);
			SubtractionStock(g_player.stock);
			g_player.stock--;
			g_player.bDisp = false;
			g_player.nCounterState = 60;
			if (g_player.stock == 0)
			{
				SetExplosion(g_posPlayer, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
				g_player.state = PLAYERSTATE_DEATH;
			}
			else
			{
				g_player.state = PLAYERSTATE_APPEAR;
			}
		}
		else
		{
			PlaySound(SOUND_LABEL_SE_HIT000);
			if (g_player.state == PLAYERSTATE_BEAM)
			{
			}
			else
			{
				g_player.state = PLAYERSTATE_DAMAGE;
				g_player.nCounterState = DAMAGE_TIME;	//�_���[�W��Ԃ̎���

				VERTEX_2D *pVtx;

				//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
				g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				//���_�o�b�t�@���A�����b�N
				g_pVtxBuffPlayer->Unlock();
			}
		}
	}
}

//=============================================================================
// �v���C���[�ړ�����
//=============================================================================
void MovePlayer(void)
{
	VERTEX_2D *pVtx;

	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_posPlayer.y > 0 || g_posPlayer.y <= SCREEN_HEIGHT ||
		g_posPlayer.x > 0 || g_posPlayer.x <= SCREEN_WIDTH)
	{
		g_nMoveX = 0;
		g_nMoveY = 0;
	}

	pVtx[0].pos.x = g_posPlayer.x - g_nMoveX;	//X���W��ݒ�
	pVtx[0].pos.y = g_posPlayer.y - g_nMoveY;	//Y���W��ݒ�
	pVtx[1].pos.x = g_posPlayer.x + g_nMoveX;	//X���W��ݒ�
	pVtx[1].pos.y = g_posPlayer.y - g_nMoveY;	//Y���W��ݒ�
	pVtx[2].pos.x = g_posPlayer.x - g_nMoveX;	//X���W��ݒ�
	pVtx[2].pos.y = g_posPlayer.y + g_nMoveY;	//Y���W��ݒ�
	pVtx[3].pos.x = g_posPlayer.x + g_nMoveX;	//X���W��ݒ�
	pVtx[3].pos.y = g_posPlayer.y + g_nMoveY;	//Y���W��ݒ�

	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
Player *GetPlayer(void)
{
	return &g_player;
}