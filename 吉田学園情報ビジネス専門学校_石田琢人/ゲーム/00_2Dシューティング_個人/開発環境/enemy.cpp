//=============================================================================
//
// �G���� [enemy.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "game.h"
#include "score.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_TYPE_ENEMY		(4)		// �G�̎��
#define ENEMY_TEXTURENAME0	"data/TEXTURE/enemy000.png"
#define ENEMY_TEXTURENAME1	"data/TEXTURE/enemy001.png"
#define ENEMY_TEXTURENAME2	"data/TEXTURE/enemy002.png"
#define ENEMY_TEXTURENAME3	"data/TEXTURE/enemy003.png"
#define DAMAGE_TIME	(5)	//�_���[�W��Ԃ̎���
#define ENEMY_LIFE	(3)	//�G�̗̑�
#define TEX_PATTERN	(2)	//�e�N�X�`���̃p�^�[����
#define TEX_X_UPDATE	(g_aEnemy[nCntEnemy].nPatternAnim * 0.5f)	//�e�N�X�`��X���W�̍X�V
#define ANIM_SPEED		(30)		//�A�j���[�V�����̈ړ����x
#define CNT_BEAM_CHAGE	(10)			//�v���C���[�̃r�[�������܂�̂ɕK�v�ȓ|���G�̐�

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureEnemy[MAX_TYPE_ENEMY];	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
Enemy g_aEnemy[MAX_ENEMY];	//�G�̏��
int g_nNumEnemy;			//�G�̑���

//=============================================================================
// ����������
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �G�̏��̏�����
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].nCounterAnim = 0;		//�J�E���^�[������������
		g_aEnemy[nCntEnemy].nPatternAnim = 0;		//�p�^�[��No.������������
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NOMAL;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE;
		g_aEnemy[nCntEnemy].bUse = false;
	}

	g_nNumEnemy = 0;	//�G�̑������N���A

	// �e�N�X�`���̓ǂݍ���(�S�̕�)
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURENAME0, &g_apTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURENAME1, &g_apTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURENAME2, &g_apTextureEnemy[2]);
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURENAME3, &g_apTextureEnemy[3]);

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
									D3DUSAGE_WRITEONLY,
									FVF_VERTEX_2D,
									D3DPOOL_MANAGED,
									&g_pVtxBuffEnemy,
									NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	int nCntTex;

	// �e�N�X�`���̊J��
	for(nCntTex = 0; nCntTex < MAX_TYPE_ENEMY; nCntTex++)
	{
		if (g_apTextureEnemy[nCntTex] != NULL)
		{
			g_apTextureEnemy[nCntTex]->Release();
			g_apTextureEnemy[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	int nCntEnemy;
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].nCounterAnim++;

		if ((g_aEnemy[nCntEnemy].nCounterAnim % ANIM_SPEED) == 0)
		{
			g_aEnemy[nCntEnemy].nPatternAnim = (g_aEnemy[nCntEnemy].nPatternAnim + 1) % TEX_PATTERN;	//�p�^�[��No.�X�V

			if (g_aEnemy[nCntEnemy].bUse == true)
			{
				//�e�N�X�`�����W���X�V
				pVtx[0].tex = D3DXVECTOR2(0.0f + TEX_X_UPDATE, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.5f + TEX_X_UPDATE, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + TEX_X_UPDATE, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f + TEX_X_UPDATE, 1.0f);
			}
		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemy->Unlock();


	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�G���g�p����Ă���
			switch (g_aEnemy[nCntEnemy].state)	
			{
			case ENEMYSTATE_NOMAL:
				if (rand() % 480 == 0)
				{
					D3DXVECTOR3 BulletMove(0.0f, 0.0f, 0.0f);
					Player *pPlayer;
					static float fAngle;

					//�v���C���[�̎擾
					pPlayer = GetPlayer();

					fAngle = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);

					BulletMove.x = sinf(fAngle) * 5.0f;
					BulletMove.y = cosf(fAngle) * 5.0f;

					SetBullet(g_aEnemy[nCntEnemy].pos, BulletMove, BULLETTYPE_ENEMY);
				}
				break;

			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;
				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NOMAL;

					VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

					//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
					g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

					pVtx += (nCntEnemy * 4);	//�Y���̈ʒu�܂Ői�߂�

					//���_�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//���_�o�b�t�@���A�����b�N
					g_pVtxBuffEnemy->Unlock();
				}
				break;
			}
		}
		////���_�o�b�t�@�����b�N
		//g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		//g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
		//g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;

		////���_���W�̐ݒ�
		//pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].move.y, 0.0f);
		//pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].move.y, 0.0f);
		//pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].move.y, 0.0f);
		//pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].move.x, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].move.y, 0.0f);

		//pVtx += 4;	//���_�f�[�^�̃|�C���^��4�i�߂�

		////���_�o�b�t�@���A�����b�N
		//g_pVtxBuffEnemy->Unlock();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�G���g�p����Ă���
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEnemy, 2);
		}
	}
}

//=============================================================================
// �G�̐ݒ�
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nCntEnemy;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//�G���g�p����Ă��Ȃ�
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].nType = nType;
			//g_aEnemy[nCntEnemy].move = move;
			g_aEnemy[nCntEnemy].bUse = true;	//�g�p���Ă����Ԃɂ���

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y - 30.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30.0f, g_aEnemy[nCntEnemy].pos.y + 30.0f, 0.0f);

			g_nNumEnemy++;	//�G�̑��������Z����
			break;
		}
		pVtx += 4;
	}
	
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// �G�̎擾
//=============================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=============================================================================
// �G�̃_���[�W����
//=============================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	g_aEnemy[nCntEnemy].nLife -= nDamage;
	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		//SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		SetEffect(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f), 15.0f, 30, EFFECTSTATE_EXPLOSION);
		g_aEnemy[nCntEnemy].bUse = false;
		AddScore(1000);
		g_nNumEnemy--;
		AddBeam(15);
		if (g_nNumEnemy <= 0)
		{//�G���S�ł�����
			SetGameState(GAMESTATE_END);
		}
	}
	else
	{
		PlaySound(SOUND_LABEL_SE_HIT001);
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = DAMAGE_TIME;	//�_���[�W��Ԃ̎���

		AddScore(50);

		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nCntEnemy * 4);	//�Y���̈ʒu�܂Ői�߂�

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffEnemy->Unlock();
	}
}