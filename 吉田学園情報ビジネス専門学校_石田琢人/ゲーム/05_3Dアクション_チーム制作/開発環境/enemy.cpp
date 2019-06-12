//=============================================================================
//
// �G�̏��� [enemy.cpp]
// Author : �L�n�@���u
//
//=============================================================================
#define	_CRT_SECURE_NO_WARNINGS		//scanf�G���[����
#include <stdio.h>					//stdio
#include "enemy.h"					//�G
#include "input.h"					//�L�[�{�[�h
#include "camera.h"					//�J����
#include "shadow.h"					//�e
#include "sound.h"					//�T�E���h
#include "player.h"					//�v���C���[
#include "object.h"					//�I�u�W�F�N�g
#include "object2.h"					//�I�u�W�F�N�g
#include "object3.h"					//�I�u�W�F�N�g
#include "Blood.h"
#include "particle.h"
#include "game.h"
#include "clear.h"
#include "fade.h"
#include "remnantsEnemy.h"
#include "combo.h"
#include "killLog.h"
#include "load.h"
#include "gauge.h"
#include "dragon.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY1_SPEED			(5.0f)
#define MAX_ENEMY				(30)
#define PULS					(20)

#define ENEMY_LIFE				(3)		// �G�̃��C�t

#define ATTACK_PARTS			(5)
#define ATTACK_START			(3)			// �U���̊J�n�L�[
#define ATTACK_END				(4)			// �U���̏I���L�[
#define DAMAGE_TIME_ENEMY		(30)		// �_���[�W��Ԃ̃t���[��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Enemy		g_Enemy[MAX_ENEMY];					// �G
float		g_fJoyStickAngleEnemy1;				//
int g_nNumEnemy;								// �X�e�[�W�̓G�̑���
int g_nNumAppearEnemy;							// �o�����̓G�̑���
int g_nClearMessage;							// �N���A���b�Z�[�W���łĂ��邩

//=============================================================================
// ����������
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�@�C���̃|�C���^
	FILE *pFile;
	//�G�J�E���^�[
	int nCntEnemy;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// �ʒu�E�����̏����ݒ�
		g_Enemy[nCntEnemy].pos = D3DXVECTOR3(-300.0f +(100 * nCntEnemy), 50, -40);
		g_Enemy[nCntEnemy].rot = D3DXVECTOR3(0, 0, 0);
		g_Enemy[nCntEnemy].move = D3DXVECTOR3(0, 0, 0);
		g_Enemy[nCntEnemy].bUse = false;
		g_Enemy[nCntEnemy].bLand = false;
		g_Enemy[nCntEnemy].bAttack = false;
		g_Enemy[nCntEnemy].nAttackCount = 0;
		g_Enemy[nCntEnemy].nLife = 0;
		g_Enemy[nCntEnemy].bDamage = false;
		g_Enemy[nCntEnemy].nCntDamage = 0;
	}

	g_nNumEnemy = 0;
	g_nNumAppearEnemy = 0;
	g_nClearMessage = 0;

	for (int nCntEnemyAll = 0; nCntEnemyAll < MAX_ENEMY; nCntEnemyAll++)
	{
		//�t�@�C���ݒ�
		pFile = fopen(TEXT_NAME, "r");

		//�G�́��e�L�X�g�f�[�^�̓ǂݍ���
		if (pFile != NULL)
		{
			//�g���p�[�c��
			fscanf(pFile, "%d", &g_Enemy[nCntEnemyAll].nMaxPartsNum);	//�ǂݍ���

			//�t�@�C����
			for (nCntEnemy = 0; nCntEnemy < g_Enemy[nCntEnemyAll].nMaxPartsNum; nCntEnemy++)
			{
				fscanf(pFile, "%s", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].FileName[0]); //�ǂݍ���
			}

			for (nCntEnemy = 0; nCntEnemy < g_Enemy[nCntEnemyAll].nMaxPartsNum; nCntEnemy++)
			{//�����l�ƌ���
				fscanf(pFile, "%d", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].nIdxModelParent);//�ǂݍ���
				fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].InitPos.x);//�ǂݍ���
				fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].InitPos.y);//�ǂݍ���
				fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].InitPos.z);//�ǂݍ���
				fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].InitRot.x);//�ǂݍ���
				fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].InitRot.y);//�ǂݍ���
				fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].InitRot.z);//�ǂݍ���
			}

			//�t�@�C���ǂݍ���
			fscanf(pFile, "%d", &g_Enemy[nCntEnemyAll].nMaxAnimNum);

			for (int nCntEnemyAnim = 0; nCntEnemyAnim < g_Enemy[nCntEnemyAll].nMaxPartsNum; nCntEnemyAnim++)
			{
				//�t���[����
				fscanf(pFile, "%d", &g_Enemy[nCntEnemyAll].nAnimloop[nCntEnemyAnim]);

				//�L�[��
				fscanf(pFile, "%d", &g_Enemy[nCntEnemyAll].nAnimKey[nCntEnemyAnim]);

				for (int nCntEnemyAnimKey = 0; nCntEnemyAnimKey < g_Enemy[nCntEnemyAll].nAnimKey[nCntEnemyAnim]; nCntEnemyAnimKey++)
				{
					fscanf(pFile, "%d", &g_Enemy[nCntEnemyAll].nAnimfram[nCntEnemyAnim][nCntEnemyAnimKey]);//�ǂݍ���

					for (nCntEnemy = 0; nCntEnemy < g_Enemy[nCntEnemyAll].nMaxPartsNum; nCntEnemy++)
					{
						fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].posAnim[nCntEnemyAnim][nCntEnemyAnimKey].x);		//�ǂݍ���
						fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].posAnim[nCntEnemyAnim][nCntEnemyAnimKey].y);		//�ǂݍ���
						fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].posAnim[nCntEnemyAnim][nCntEnemyAnimKey].z);		//�ǂݍ���
						fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].rotAnim[nCntEnemyAnim][nCntEnemyAnimKey].x);		//�ǂݍ���
						fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].rotAnim[nCntEnemyAnim][nCntEnemyAnimKey].y);		//�ǂݍ���
						fscanf(pFile, "%f", &g_Enemy[nCntEnemyAll].aModel[nCntEnemy].rotAnim[nCntEnemyAnim][nCntEnemyAnimKey].z);		//�ǂݍ���
					}
				}
			}

			fclose(pFile);	//�t�@�C�������
		}
		else
		{
			MessageBox(0, "�Z�[�u�t�@�C���̃`�F�b�N�Ɏ��s!", "�x��", MB_ICONWARNING);
		}

		//x�t�@�C���̓ǂݍ���
		for (nCntEnemy = 0; nCntEnemy < g_Enemy[nCntEnemyAll].nMaxPartsNum; nCntEnemy++)
		{
			// X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX
			(
				&g_Enemy[nCntEnemyAll].aModel[nCntEnemy].FileName[0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_Enemy[nCntEnemyAll].aModel[nCntEnemy].pBuffMatModel,
				NULL,
				&g_Enemy[nCntEnemyAll].aModel[nCntEnemy].nNumMatModel,
				&g_Enemy[nCntEnemyAll].aModel[nCntEnemy].pMeshModel
			);
		}

		int nNumVtx;			//���_��
		DWORD sizeFVF;			//���_�t�H�[�}�b�g�̃T�C�Y
		BYTE *pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^

		for (int nCntModel = 0; nCntModel < g_Enemy[nCntEnemyAll].nMaxPartsNum; nCntModel++)
		{
			//���_�����擾
			nNumVtx = g_Enemy[nCntEnemyAll].aModel[nCntModel].pMeshModel->GetNumVertices();

			//���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(g_Enemy[nCntEnemyAll].aModel[nCntModel].pMeshModel->GetFVF());

			//���_�o�b�t�@�����b�N
			g_Enemy[nCntEnemyAll].aModel[nCntModel].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff; //���_���W�̑��

				//�͈͎w��
				//�ő�l
				if (vtx.x < g_Enemy[nCntEnemyAll].VtxMin.x)
				{
					g_Enemy[nCntEnemyAll].VtxMin.x = vtx.x;
				}
				if (vtx.z < g_Enemy[nCntEnemyAll].VtxMin.z)
				{
					g_Enemy[nCntEnemyAll].VtxMin.z = vtx.z;
				}
				if (vtx.y < g_Enemy[nCntEnemyAll].VtxMin.y)
				{
					g_Enemy[nCntEnemyAll].VtxMin.y = vtx.y;
				}
				//�ŏ��l
				if (vtx.x > g_Enemy[nCntEnemyAll].VtxMax.x)
				{
					g_Enemy[nCntEnemyAll].VtxMax.x = vtx.x;
				}
				if (vtx.z > g_Enemy[nCntEnemyAll].VtxMax.z)
				{
					g_Enemy[nCntEnemyAll].VtxMax.z = vtx.z;
				}
				if (vtx.y > g_Enemy[nCntEnemyAll].VtxMax.y)
				{
					g_Enemy[nCntEnemyAll].VtxMax.y = vtx.y;
				}
				pVtxBuff += sizeFVF;		//�T�C�Y���|�C���^��i�߂�
			}

			// ���_�o�b�t�@���A�����b�N
			g_Enemy[nCntEnemyAll].aModel[nCntModel].pMeshModel->UnlockVertexBuffer();
		}
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		for (int nCount = 0; nCount < MAX_PARTS; nCount++)
		{
			// ���b�V���̊J��
			if (g_Enemy[nCntEnemy].aModel[nCount].pMeshModel != NULL)
			{
				g_Enemy[nCntEnemy].aModel[nCount].pMeshModel->Release();
				g_Enemy[nCntEnemy].aModel[nCount].pMeshModel = NULL;
			}
		}
		for (int nCount = 0; nCount < MAX_PARTS; nCount++)
		{
			// �}�e���A���̊J��
			if (g_Enemy[nCntEnemy].aModel[nCount].pBuffMatModel != NULL)
			{
				g_Enemy[nCntEnemy].aModel[nCount].pBuffMatModel->Release();
				g_Enemy[nCntEnemy].aModel[nCount].pBuffMatModel = NULL;
			}
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	bool bSwap = GetStageSwat();
	bool bFade = GetFadeNow();
	LOADFADE loadFade = GetLoadFade();

	//�J�����̏��
	Camera *pCamera = GetCamera();

	//�v���C���[�̃|�C���^
	Player *pPlayer = GetPlayer();

	if (g_nClearMessage == 1)
	{// �G�����ׂē|����
		SetClearMessage(TYPE_COMPLETE);
		g_nClearMessage = 2;
	}
	else if (g_nClearMessage == 0)
	{
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			if (g_Enemy[nCntEnemy].bUse == true)
			{
				//�t���[���J�E���g
				g_Enemy[nCntEnemy].nCntTimeEnemy++;

				//�O��̈ʒu��ۑ�
				g_Enemy[nCntEnemy].posOld = g_Enemy[nCntEnemy].pos;

				float nSpeed = 5.0f;	//���x
										//float fDiffAngle;

				//�ǂ̂����蔻��
				if (g_Enemy[nCntEnemy].pos.z >= 740)
				{//��
					g_Enemy[nCntEnemy].pos.z = 740;
				}
				if (g_Enemy[nCntEnemy].pos.z <= -740)
				{//��
					g_Enemy[nCntEnemy].pos.z = -740;
				}
				if (g_Enemy[nCntEnemy].pos.x >= 740)
				{//�E
					g_Enemy[nCntEnemy].pos.x = 740;
				}
				if (g_Enemy[nCntEnemy].pos.x <= -740)
				{//��
					g_Enemy[nCntEnemy].pos.x = -740;
				}

				if (false == bSwap && false == bFade && LOADFADE_NONE == loadFade)
				{
					if (g_Enemy[nCntEnemy].pos.x <= pPlayer->pos.x + 80 && g_Enemy[nCntEnemy].pos.x >= pPlayer->pos.x - 80 && g_Enemy[nCntEnemy].pos.z <= pPlayer->pos.z + 80 && g_Enemy[nCntEnemy].pos.z >= pPlayer->pos.z - 80)
					{
						g_Enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

						if (g_Enemy[nCntEnemy].nAnimnow != ENEMYANIM_NEUTRAL && g_Enemy[nCntEnemy].bAttack == false)
						{//�����A�j���[�V����
							g_Enemy[nCntEnemy].nAnimnow = ENEMYANIM_NEUTRAL;
							g_Enemy[nCntEnemy].nCntTimeEnemy = 0;
							g_Enemy[nCntEnemy].nAnimKeynow = 0;
						}

						if (g_Enemy[nCntEnemy].nAttackCount >= 239)
						{
							if (g_Enemy[nCntEnemy].nAnimnow != ENEMYANIM_ATTACK && g_Enemy[nCntEnemy].bAttack == false)
							{//�U���A�j���[�V����
								g_Enemy[nCntEnemy].bAttack = true;

								g_Enemy[nCntEnemy].nAnimnow = ENEMYANIM_ATTACK;
								g_Enemy[nCntEnemy].nCntTimeEnemy = 0;
								g_Enemy[nCntEnemy].nAnimKeynow = 0;
							}
						}

						g_Enemy[nCntEnemy].nAttackCount++;
					}
					//�v���C���[�ɒǔ�
					else if (g_Enemy[nCntEnemy].pos.x <= pPlayer->pos.x + 300 && g_Enemy[nCntEnemy].pos.x >= pPlayer->pos.x - 300 && g_Enemy[nCntEnemy].pos.z <= pPlayer->pos.z + 300 && g_Enemy[nCntEnemy].pos.z >= pPlayer->pos.z - 300 && g_Enemy[nCntEnemy].move.y <= 0.1f)
					{
						//�v���C���[�̈ʒu�Ɍ������Ĉړ�����
						g_Enemy[nCntEnemy].move = D3DXVECTOR3(sinf(atan2f(-g_Enemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_Enemy[nCntEnemy].pos.z - -pPlayer->pos.z)) * 2.5f, 0.0f, cosf(atan2f(-g_Enemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_Enemy[nCntEnemy].pos.z - -pPlayer->pos.z)) * 2.5f);

						//�v���C���[�̂ق��Ɍ���
						g_Enemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, atan2f(-g_Enemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_Enemy[nCntEnemy].pos.z - -pPlayer->pos.z) + D3DX_PI, 0.0f);

						if (g_Enemy[nCntEnemy].nAnimnow != ENEMYANIM_WALK && g_Enemy[nCntEnemy].bAttack == false)
						{//�����A�j���[�V����
							g_Enemy[nCntEnemy].nAnimnow = ENEMYANIM_WALK;
							g_Enemy[nCntEnemy].nCntTimeEnemy = 0;
							g_Enemy[nCntEnemy].nAnimKeynow = 0;
						}
					}
					else if (g_Enemy[nCntEnemy].nAnimnow != ENEMYANIM_NEUTRAL && g_Enemy[nCntEnemy].bAttack == false)
					{//�����A�j���[�V����
						g_Enemy[nCntEnemy].nAnimnow = ENEMYANIM_NEUTRAL;
						g_Enemy[nCntEnemy].nCntTimeEnemy = 0;
						g_Enemy[nCntEnemy].nAnimKeynow = 0;
					}
				}
				else
				{
					if (g_Enemy[nCntEnemy].nAnimnow != ENEMYANIM_NEUTRAL && g_Enemy[nCntEnemy].bAttack == false)
					{//�ҋ@���[�V����
						g_Enemy[nCntEnemy].nAnimnow = ENEMYANIM_NEUTRAL;
						g_Enemy[nCntEnemy].nCntTimeEnemy = 0;
						g_Enemy[nCntEnemy].nAnimKeynow = 0;
					}
				}

				//�e�����Ă���
				SetPositionShadow(g_Enemy[nCntEnemy].nIdxShadow, g_Enemy[nCntEnemy].pos);

				//����������
				g_Enemy[nCntEnemy].move.x += (0.0f - g_Enemy[nCntEnemy].move.x) * 0.2f;
				g_Enemy[nCntEnemy].move.z += (0.0f - g_Enemy[nCntEnemy].move.z) * 0.2f;
				g_Enemy[nCntEnemy].blowMove.x += (0.0f - g_Enemy[nCntEnemy].blowMove.x) * 0.2f;
				g_Enemy[nCntEnemy].blowMove.y += (0.0f - g_Enemy[nCntEnemy].blowMove.y) * 0.2f;
				g_Enemy[nCntEnemy].blowMove.z += (0.0f - g_Enemy[nCntEnemy].blowMove.z) * 0.2f;

				//�ʒu���ړ��ɑ��
				g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].move;
				g_Enemy[nCntEnemy].pos += g_Enemy[nCntEnemy].blowMove;

				g_Enemy[nCntEnemy].pos.y -= GRAVITY;	// �d�͂����Z

				if (g_Enemy[nCntEnemy].pos.y <= 50)
				{// ��
					g_Enemy[nCntEnemy].pos.y = 50;
				}

				//�����蔻��
				CollisionObject(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld, &g_Enemy[nCntEnemy].move);
				CollisionObject2(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld, &g_Enemy[nCntEnemy].move);
				CollisionObject3(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld, &g_Enemy[nCntEnemy].move);
				CollisionEnemies(&g_Enemy[nCntEnemy].pos, &g_Enemy[nCntEnemy].posOld, &g_Enemy[nCntEnemy].move, 10, nCntEnemy);

				// �_���[�W���󂯂�
				if (CollisionAttack(g_Enemy[nCntEnemy].pos, g_Enemy[nCntEnemy].VtxMax.x) == true && g_Enemy[nCntEnemy].nAnimnow != ENEMYANIM_DAMAGE && !g_Enemy[nCntEnemy].bDamage)
				{
					PlaySound(SOUND_LABEL_SWORDHIT);	// �q�b�g��
					SetParticleEmitter(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + 20, g_Enemy[nCntEnemy].pos.z), 1);	// �G���猌���o��
					AddCombo();
					g_Enemy[nCntEnemy].bDamage = true;
					g_Enemy[nCntEnemy].nCntDamage = 0;
					g_Enemy[nCntEnemy].nLife--;
					AddSubtractGauge(1);

					// ������Ԋp�x
					float fAngle = atan2f(pPlayer->pos.x - g_Enemy[nCntEnemy].pos.x, pPlayer->pos.z - g_Enemy[nCntEnemy].pos.z);

					// ������ԋ���
					g_Enemy[nCntEnemy].blowMove = D3DXVECTOR3(-sinf(fAngle) * 25.0f, 10.0f, -cosf(fAngle) * 25.0f);

				}

				PlayerType type = GetPlayerType();

				if(CollisionDragonAttack(g_Enemy[nCntEnemy].pos, 60) && g_Enemy[nCntEnemy].nAnimnow != ENEMYANIM_DAMAGE && !g_Enemy[nCntEnemy].bDamage && type == TYPE_DRAGON)
				{// �h���S���̉�
					PlaySound(SOUND_LABEL_HIT);	// �q�b�g��
					SetParticleEmitter(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y + 20, g_Enemy[nCntEnemy].pos.z), 1);	// �G���猌���o��
					AddCombo();
					g_Enemy[nCntEnemy].bDamage = true;
					g_Enemy[nCntEnemy].nCntDamage = 0;
					g_Enemy[nCntEnemy].nLife--;
					AddSubtractGauge(1);
				}

				if (g_Enemy[nCntEnemy].nLife <= 0)
				{// ���񂾂Ƃ�
					PlaySound(SOUND_LABEL_ENEMYVOICE);				// ����
					g_Enemy[nCntEnemy].bUse = false;				// �G������
					DeleteShadow(g_Enemy[nCntEnemy].nIdxShadow);	// �e������
					g_nNumEnemy--;
					g_nNumAppearEnemy--;
					SetRemnants(g_nNumEnemy);
					if (g_nNumEnemy <= 0)
					{
						g_nClearMessage = 1;
					}
					AddKill();

					// �n�ʂɌ����o��
					SetBlood(D3DXVECTOR3(g_Enemy[nCntEnemy].pos.x, 0.0f, g_Enemy[nCntEnemy].pos.z), D3DXVECTOR3(g_Enemy[nCntEnemy].rot.x, g_Enemy[nCntEnemy].rot.y - (rand() % 30), g_Enemy[nCntEnemy].rot.z), 40.0f + (rand() % 10), 40.0f + (rand() % 10), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
				}

				// �_���[�W���
				if (g_Enemy[nCntEnemy].bDamage)
				{
					g_Enemy[nCntEnemy].nCntDamage++;

					if (g_Enemy[nCntEnemy].nCntDamage >= DAMAGE_TIME_ENEMY)
					{
						g_Enemy[nCntEnemy].bDamage = false;
					}
				}

				//�����̃��[�J���ϐ�
				float fDes;
				float frac;

				//�L�[��i�߂�I
				if (g_Enemy[nCntEnemy].nCntTimeEnemy % g_Enemy[nCntEnemy].nAnimfram[g_Enemy[nCntEnemy].nAnimnow][g_Enemy[nCntEnemy].nAnimKeynow] == 0)
				{
					g_Enemy[nCntEnemy].nCntTimeEnemy = 0;
					g_Enemy[nCntEnemy].nAnimKeynow = (g_Enemy[nCntEnemy].nAnimKeynow + 1) % g_Enemy[nCntEnemy].nAnimKey[g_Enemy[nCntEnemy].nAnimnow];

					if (g_Enemy[nCntEnemy].nAnimnow == ENEMYANIM_ATTACK && g_Enemy[nCntEnemy].nAnimKey[g_Enemy[nCntEnemy].nAnimnow] - 1 == g_Enemy[nCntEnemy].nAnimKeynow)
					{//�����A�j���[�V����
						g_Enemy[nCntEnemy].nAnimnow = ENEMYANIM_ATTACK;
						g_Enemy[nCntEnemy].nCntTimeEnemy = 0;
						g_Enemy[nCntEnemy].nAnimKeynow = 0;
						g_Enemy[nCntEnemy].nAttackCount = 0;
						g_Enemy[nCntEnemy].bAttack = false;
					}
				}

				//�����̌v�Z
				frac = (g_Enemy[nCntEnemy].nAnimfram[g_Enemy[nCntEnemy].nAnimnow][g_Enemy[nCntEnemy].nAnimKeynow] - g_Enemy[nCntEnemy].nCntTimeEnemy) * 1.0f;

				//�A�j���[�V����
				for (int nCntEnemyAnim = 0; nCntEnemyAnim < g_Enemy[nCntEnemy].nMaxPartsNum; nCntEnemyAnim++)
				{
					//===================
					// X�̊p�x
					//===================

					//�ړI�̊p�x������ׂ�
					g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest = g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotAnim[g_Enemy[nCntEnemy].nAnimnow][g_Enemy[nCntEnemy].nAnimKeynow];

					//x�ړI�̈ʒu�̒���
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.x > D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.x -= D3DX_PI * 2.0f;
					}
					//x�ړI�̈ʒu�̒���
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.x < -D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.x += D3DX_PI * 2.0f;
					}

					//
					fDes = g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.x - g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.x;

					//�����̒���
					if (fDes > D3DX_PI)
					{
						fDes -= D3DX_PI * 2.0f;
					}
					if (fDes > D3DX_PI)
					{
						fDes += D3DX_PI * 2.0f;
					}

					//���݂̌����̐ݒ�
					g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.x += fDes / frac;

					//x�ړI�̈ʒu�̒���
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.x > D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.x -= D3DX_PI * 2.0f;
					}
					//x�ړI�̈ʒu�̒���
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.x < -D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.x += D3DX_PI * 2.0f;
					}

					//===================
					// Y�̊p�x
					//===================

					//y�ړI�̈ʒu�̒���
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.y > D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.y -= D3DX_PI * 2.0f;
					}
					//y�ړI�̈ʒu�̒���
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.y < -D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.y += D3DX_PI * 2.0f;
					}

					//
					fDes = g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.y - g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.y;

					//�����̒���
					if (fDes > D3DX_PI)
					{
						fDes -= D3DX_PI * 2.0f;
					}
					if (fDes > D3DX_PI)
					{
						fDes += D3DX_PI * 2.0f;
					}

					//���݂̌����̐ݒ�
					g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.y += fDes / frac;

					//x�ړI�̈ʒu�̒���
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.y > D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.y -= D3DX_PI * 2.0f;
					}
					//x�ړI�̈ʒu�̒���
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.y < -D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.y += D3DX_PI * 2.0f;
					}

					//===================
					// Z�̊p�x
					//===================

					//z�ړI�̈ʒu�̒���
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.z > D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.z -= D3DX_PI * 2.0f;
					}
					//z�ړI�̈ʒu�̒���
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.z < -D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.z += D3DX_PI * 2.0f;
					}

					//
					fDes = g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].rotDest.z - g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.z;

					//�����̒���
					if (fDes > D3DX_PI)
					{
						fDes -= D3DX_PI * 2.0f;
					}
					if (fDes > D3DX_PI)
					{
						fDes += D3DX_PI * 2.0f;
					}

					//���݂̌����̐ݒ�
					g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.z += fDes / frac;

					//x�ړI�̈ʒu�̒���
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.z > D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.z -= D3DX_PI * 2.0f;
					}
					//x�ړI�̈ʒu�̒���
					if (g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.z < -D3DX_PI)
					{
						g_Enemy[nCntEnemy].aModel[nCntEnemyAnim].InitRot.z += D3DX_PI * 2.0f;
					}
				}
				// �p�[�c�̈ʒu
				D3DXVECTOR3 DestPos = g_Enemy[nCntEnemy].aModel[0].posAnim[g_Enemy[nCntEnemy].nAnimnow][g_Enemy[nCntEnemy].nAnimKeynow];
				D3DXVECTOR3 defPos;
				frac = (g_Enemy[nCntEnemy].nAnimfram[g_Enemy[nCntEnemy].nAnimnow][g_Enemy[nCntEnemy].nAnimKeynow] - g_Enemy[nCntEnemy].nCntTimeEnemy) * 1.0f;

				defPos = DestPos - g_Enemy[nCntEnemy].aModel[0].InitPos;
				g_Enemy[nCntEnemy].aModel[0].InitPos.x += defPos.x / frac;
				g_Enemy[nCntEnemy].aModel[0].InitPos.y += defPos.y / frac;
				g_Enemy[nCntEnemy].aModel[0].InitPos.z += defPos.z / frac;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A����ۑ�
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxParent;						//

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//�G
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Enemy[nCntEnemy].mtxWorldEnemy);
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[nCntEnemy].rot.y, g_Enemy[nCntEnemy].rot.x, g_Enemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorldEnemy, &g_Enemy[nCntEnemy].mtxWorldEnemy, &mtxRot);
			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y, g_Enemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorldEnemy, &g_Enemy[nCntEnemy].mtxWorldEnemy, &mtxTrans);
			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].mtxWorldEnemy);

		for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
		{
			if (g_Enemy[nCntEnemy].aModel[nCntParts].nIdxModelParent < 0)
			{//�e���f��
				mtxParent = g_Enemy[nCntEnemy].mtxWorldEnemy;
			}
			else
			{//�q���f��
				mtxParent = g_Enemy[nCntEnemy].aModel[g_Enemy[nCntEnemy].aModel[nCntParts].nIdxModelParent].mtxWorldModel;
			}
			{//�e���f��
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel);
				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll
				(
					&mtxRot,
					g_Enemy[nCntEnemy].aModel[nCntParts].InitRot.y,
					g_Enemy[nCntEnemy].aModel[nCntParts].InitRot.x,
					g_Enemy[nCntEnemy].aModel[nCntParts].InitRot.z
				);
				D3DXMatrixMultiply
				(
					&g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel,
					&g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel,
					&mtxRot
				);
				// �ʒu�𔽉f
				D3DXMatrixTranslation
				(
					&mtxTrans,
					g_Enemy[nCntEnemy].aModel[nCntParts].InitPos.x,
					g_Enemy[nCntEnemy].aModel[nCntParts].InitPos.y,
					g_Enemy[nCntEnemy].aModel[nCntParts].InitPos.z
				);
				D3DXMatrixMultiply
				(
					&g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel,
					&g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel,
					&mtxTrans
				);

				D3DXMatrixMultiply
				(
					&g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel,
					&g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel,
					&mtxParent
				);
				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].aModel[nCntParts].mtxWorldModel);

				// ���݂̃}�e���A�����擾
				pDevice->GetMaterial(&matDef);

				// �}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_Enemy[nCntEnemy].aModel[nCntParts].pBuffMatModel->GetBufferPointer();

				// �|���S���̕`��
				if (g_Enemy[nCntEnemy].bUse == true)
				{
					for (int nCntMat = 0; nCntMat < (int)g_Enemy[nCntEnemy].aModel[nCntParts].nNumMatModel; nCntMat++)
					{
						// �}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						// �e�N�X�`����NULL�ɂ���
						pDevice->SetTexture(0, NULL);

						//pDevice->SetTexture(0, g_Enemy[nCntEnemy].nNumMatEnemy);

						// �G(�p�[�c)�̕`��
						g_Enemy[nCntEnemy].aModel[nCntParts].pMeshModel->DrawSubset(nCntMat);
					}
				}
				// �}�e���A�����f�t�H���g�ɖ߂�
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}
//=========================================================================================================================
// �G�̎擾����
//=========================================================================================================================
Enemy *GetEnemy(void)
{
	return &g_Enemy[0];
}

//=============================================================================
// �G���m�̓����蔻��
//=============================================================================
bool CollisionEnemies(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange, int nIdx)
{
	int nCount;
	bool bEnemy = false;

	for (nCount = 0; nCount < MAX_ENEMY; nCount++)
	{// �S�Ă̓G����
		if (true == g_Enemy[nCount].bUse)
		{// �g�p����Ă���G�̂�
			if (nCount != nIdx)
			{// �����̔ԍ��̂Ƃ��̓X�L�b�v
				float fLengthOneself;		// ����̉~��͈�
				float fRange0 = fRange;		// ����͈̔͂̔��a�̒���

				fLengthOneself = powf(fRange0 * 3, 2);		// �����XZ�͈̔�

				float fLengthX = pPos->x - g_Enemy[nCount].mtxWorldEnemy._41;	// X�̍�
				float fLengthY = pPos->y - g_Enemy[nCount].mtxWorldEnemy._42;	// Y�̍�
				float fLengthZ = pPos->z - g_Enemy[nCount].mtxWorldEnemy._43;	// Z�̍�
				float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2) + powf(fLengthY, 2);					// XZY�̍��̓��

				if (fLengthOneself >= fLengthTotal)
				{// ����Ǝ����̔���
					*pPos = *pPosold;							// �ʒu��߂�
					*pMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ��ʂ�0�ɂ���

					bEnemy = true;
				}
			}
		}
	}
	return bEnemy;
}

//=============================================================================
// �G�Ƃ̓����蔻��
//=============================================================================
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange)
{
	int nCount;
	bool bEnemy = false;

	for (nCount = 0; nCount < MAX_ENEMY; nCount++)
	{// �S�Ă̓G����
		if (true == g_Enemy[nCount].bUse)
		{// �g�p����Ă���G�̂�
			float fLengthOneself;		// ����̉~��͈�
			float fRange0 = fRange;		// ����͈̔͂̔��a�̒���

			fLengthOneself = powf(fRange0, 3);		// �����XZ�͈̔�

			float fLengthX = pPos->x - g_Enemy[nCount].mtxWorldEnemy._41;	// X�̍�
			float fLengthY = pPos->y - g_Enemy[nCount].mtxWorldEnemy._42;	// Y�̍�
			float fLengthZ = pPos->z - g_Enemy[nCount].mtxWorldEnemy._43;	// Z�̍�
			float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2) + powf(fLengthY, 2);		// XZY�̍��̓��

			if (fLengthOneself >= fLengthTotal)
			{// ����Ǝ����̔���
				*pPos = *pPosold;							// �ʒu��߂�
				*pMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ��ʂ�0�ɂ���

				bEnemy = true;
			}
		}
	}
	return bEnemy;
}

//=========================================================================================================================
// �U���̓����蔻��
//=========================================================================================================================
bool CollisionAttackEnemy(D3DXVECTOR3 pos, float vtxMaxX)
{
	bool bAttack = false;
	int nCount;

	for (nCount = 0; nCount < MAX_ENEMY; nCount++)
	{// �S�Ă̓G����
		if (true == g_Enemy[nCount].bUse)
		{// �g�p����Ă���G�̂�
			if (g_Enemy[nCount].nAnimnow == PLAYERANIM_ATTACK)
			{// �U�����[�V������
				if (g_Enemy[nCount].nAnimKeynow >= ATTACK_START && g_Enemy[nCount].nAnimKeynow <= ATTACK_END)
				{// ���肠��̃L�[�͈͒�
					float fLengthOneself;
					float fRange0 = vtxMaxX;

					fLengthOneself = powf(fRange0 * 2, 2);		// �����XZ�͈̔�

					float fLengthX = pos.x - g_Enemy[nCount].aModel[ATTACK_PARTS].mtxWorldModel._41;	// X�̍�
					float fLengthY = pos.y - g_Enemy[nCount].aModel[ATTACK_PARTS].mtxWorldModel._42;	// Y�̍�
					float fLengthZ = pos.z - g_Enemy[nCount].aModel[ATTACK_PARTS].mtxWorldModel._43;	// Z�̍�
					float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2) + powf(fLengthY, 2);					// XZ�̍��̓��

					if (fLengthOneself >= fLengthTotal)
					{// ����Ǝ����̔���
						if (false == bAttack)
						{// �U�������L���ɂ���
							bAttack = true;
						}
					}
				}
			}
		}
	}
	return bAttack;
}

//=============================================================================
// �G�̐ݒ�
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	int nCount;

	for (nCount = 0; nCount < MAX_ENEMY; nCount++)
	{// �S�Ă̓G����
		if (false == g_Enemy[nCount].bUse)
		{// �g�p����Ă��Ȃ��G�̂�
			int nStage = GetNumState();

			if (nStage == 0)
			{// 0���h�~
				nStage = 1;
			}

			g_Enemy[nCount].pos = pos;
			g_Enemy[nCount].nAnimnow = ENEMYANIM_NEUTRAL;
			g_Enemy[nCount].nCntTimeEnemy = 0;
			g_Enemy[nCount].nAnimKeynow = 0;
			g_Enemy[nCount].nAttackCount = 0;
			g_Enemy[nCount].bAttack = false;
			g_Enemy[nCount].nLife = 2 + rand() % nStage;
			g_Enemy[nCount].bUse = true;

			g_nNumAppearEnemy++;
			g_Enemy[nCount].nIdxShadow = SetShadow(g_Enemy[nCount].pos, g_Enemy[nCount].rot, 25, 25);

			SetParticleEmitter(g_Enemy[nCount].pos, 8);
			break;
		}
	}
}
//
//
//
void DeleteEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// �ʒu�E�����̏����ݒ�
		g_Enemy[nCntEnemy].pos = D3DXVECTOR3(-300.0f + (100 * nCntEnemy), 0, -40);
		g_Enemy[nCntEnemy].rot = D3DXVECTOR3(0, 0, 0);
		g_Enemy[nCntEnemy].move = D3DXVECTOR3(0, 0, 0);
		g_Enemy[nCntEnemy].bUse = false;
		g_Enemy[nCntEnemy].bLand = true;
		g_Enemy[nCntEnemy].bAttack = false;
		g_Enemy[nCntEnemy].nAttackCount = 0;
		DeleteShadow(g_Enemy[nCntEnemy].nIdxShadow);
		g_nClearMessage = 0;
		g_nNumAppearEnemy = 0;
	}
}

//=============================================================================
// �o�����̓G�̑����̎擾
//=============================================================================
int GetNumEnemy(void)
{
	return g_nNumAppearEnemy;
}

//=============================================================================
// �G�̑����̐ݒ�
//=============================================================================
void SetNumEnemy(int nNumEnemy)
{
	g_nNumEnemy = nNumEnemy;
}