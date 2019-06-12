//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : �L�n�@���u
//
//=============================================================================
#define	_CRT_SECURE_NO_WARNINGS		//scanf�G���[����
#include <stdio.h>					//stdio
#include "player.h"					//�v���C���[
#include "input.h"					//�L�[�{�[�h
#include "camera.h"					//�J����
#include "shadow.h"					//�e
#include "sound.h"					//�T�E���h
#include "object.h"					//�I�u�W�F�N�g
#include "object2.h"					//�I�u�W�F�N�g
#include "object3.h"					//�I�u�W�F�N�g
#include "enemy.h"
#include "killLog.h"
#include "game.h"
#include "fade.h"
#include "life.h"
#include "Blood.h"
#include "load.h"
#include "gauge.h"
#include "particle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER1_SPEED			(5.0f)

#define ATTACK_PARTS			(6)
#define ATTACK_START			(0)			// �U���̊J�n�L�[
#define ATTACK_END				(8)			// �U���̏I���L�[
#define ATTACK_RADIUS			(30.0f)		// �c�q�̔��a
#define ATTACK_NUM				(3)			// �c�q�̐�
#define PLAYER_DAMAGE_TIME		(60)		// �U�����󂯂��Ƃ��̖��G����
#define DRAGON_TIME				(600)		// �h���S����Ԃ̎���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Player		g_Player;							//�v���C���[
int			g_nCntTimePlayer;					//�v���C���[�J�E���^�[
float		g_fJoyStickAnglePlayer1;			//
int			g_nCntPlayerType;					// �h���S����Ԃ̃J�E���^

//=============================================================================
// ����������
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�@�C���̃|�C���^
	FILE *pFile;
	//�v���C���[�J�E���^�[
	int nCntPlayer;

	// �ʒu�E�����̏����ݒ�
	g_Player.pos = D3DXVECTOR3(0, 0, 40);
	g_Player.rot = D3DXVECTOR3(0, 0, 0);
	g_Player.move = D3DXVECTOR3(0, 0, 0);
	g_Player.nCntPlayer = 1;
	g_Player.bUse = true;
	g_Player.bLand = true;
	g_Player.bAttack = false;
	g_Player.bDamage = false;
	g_Player.type = TYPE_HUMAN;
	g_Player.nAnimnow = PLAYERANIM_NEUTRAL;
	g_Player.bLegWhich = false;

	g_nCntPlayerType = 0;
	//�t�@�C���ݒ�
	pFile = fopen(FILE_NAME, "r");

	//�v���C���[�́��e�L�X�g�f�[�^�̓ǂݍ���
	if (pFile != NULL)
	{
		//�g���p�[�c��
		fscanf(pFile, "%d", &g_Player.nMaxPartsNum);	//�ǂݍ���

		//�t�@�C����
		for (nCntPlayer = 0; nCntPlayer < g_Player.nMaxPartsNum; nCntPlayer++)
		{
			fscanf(pFile, "%s", &g_Player.aModel[nCntPlayer].FileName[0]); //�ǂݍ���
		}

		for (nCntPlayer = 0; nCntPlayer < g_Player.nMaxPartsNum; nCntPlayer++)
		{//�����l�ƌ���
			fscanf(pFile, "%d", &g_Player.aModel[nCntPlayer].nIdxModelParent);	//�ǂݍ���
			fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].InitPos.x);		//�ǂݍ���
			fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].InitPos.y);		//�ǂݍ���
			fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].InitPos.z);		//�ǂݍ���
			fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].InitRot.x);		//�ǂݍ���
			fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].InitRot.y);		//�ǂݍ���
			fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].InitRot.z);		//�ǂݍ���
		}

		//�t�@�C���ǂݍ���
		fscanf(pFile, "%d", &g_Player.nMaxAnimNum);

		for (int nCntPlayerAnim = 0; nCntPlayerAnim < g_Player.nMaxAnimNum; nCntPlayerAnim++)
		{
			//�t���[����
			fscanf(pFile, "%d", &g_Player.nAnimloop[nCntPlayerAnim]);

			//�L�[��
			fscanf(pFile, "%d", &g_Player.nAnimKey[nCntPlayerAnim]);

			for (int nCntPlayerAnimKey = 0; nCntPlayerAnimKey < g_Player.nAnimKey[nCntPlayerAnim]; nCntPlayerAnimKey++)
			{
				fscanf(pFile, "%d", &g_Player.nAnimfram[nCntPlayerAnim][nCntPlayerAnimKey]);//�ǂݍ���

				for (nCntPlayer = 0; nCntPlayer < g_Player.nMaxPartsNum; nCntPlayer++)
				{
					fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].posAnim[nCntPlayerAnim][nCntPlayerAnimKey].x);		//�ǂݍ���
					fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].posAnim[nCntPlayerAnim][nCntPlayerAnimKey].y);		//�ǂݍ���
					fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].posAnim[nCntPlayerAnim][nCntPlayerAnimKey].z);		//�ǂݍ���
					fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].rotAnim[nCntPlayerAnim][nCntPlayerAnimKey].x);		//�ǂݍ���
					fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].rotAnim[nCntPlayerAnim][nCntPlayerAnimKey].y);		//�ǂݍ���
					fscanf(pFile, "%f", &g_Player.aModel[nCntPlayer].rotAnim[nCntPlayerAnim][nCntPlayerAnimKey].z);		//�ǂݍ���

					g_Player.aModel[nCntPlayer].InitRot = g_Player.aModel[nCntPlayer].rotAnim[PLAYERANIM_NEUTRAL][0];
					g_Player.aModel[nCntPlayer].rotDest = g_Player.aModel[nCntPlayer].rotAnim[PLAYERANIM_NEUTRAL][0];
				}
			}
		}

		g_Player.aModel[0].InitPos.y = g_Player.pos.y;

		fclose(pFile);	//�t�@�C�������
	}
	else
	{
		MessageBox(0, "�Z�[�u�t�@�C���̃`�F�b�N�Ɏ��s!", "�x��", MB_ICONWARNING);
	}

	//x�t�@�C���̓ǂݍ���
	for (nCntPlayer = 0; nCntPlayer < g_Player.nMaxPartsNum; nCntPlayer++)
	{
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX
		(
			&g_Player.aModel[nCntPlayer].FileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Player.aModel[nCntPlayer].pBuffMatModel,
			NULL,
			&g_Player.aModel[nCntPlayer].nNumMatModel,
			&g_Player.aModel[nCntPlayer].pMeshModel
		);
	}

	//�e�̏������ݒ�
	g_Player.nIdxShadow = SetShadow(g_Player.pos,g_Player.rot,25, 25);

	// ���_���W�̍ő�E�ŏ�
	for (nCntPlayer = 0; nCntPlayer < g_Player.nMaxPartsNum; nCntPlayer++)
	{
		g_Player.aModel[nCntPlayer].vtxMin = D3DXVECTOR3(10000, 10000, 10000);
		g_Player.aModel[nCntPlayer].vtxMax = D3DXVECTOR3(1, 1, 1);

		int nNumVtx;		// ���_��
		DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
		BYTE *pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

		// ���_�����擾
		nNumVtx = g_Player.aModel[nCntPlayer].pMeshModel->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_Player.aModel[nCntPlayer].pMeshModel->GetFVF());

		// ���_�o�b�t�@�����b�N
		g_Player.aModel[nCntPlayer].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

		// X�̒l�̔�r
		if (g_Player.aModel[nCntPlayer].vtxMin.x > vtx.x)
		{// �ŏ��l�Ɣ�r
			g_Player.aModel[nCntPlayer].vtxMin.x = vtx.x;	// �ŏ��l��菬������Α��
		}

		if (g_Player.aModel[nCntPlayer].vtxMax.x < vtx.x)
		{// �ő�l�Ɣ�r
			g_Player.aModel[nCntPlayer].vtxMax.x = vtx.x;	// �ő�l���傫����Α��
		}

		// Y�̒l�̔�r
		if (g_Player.aModel[nCntPlayer].vtxMin.y > vtx.y)
		{// �ŏ��l�Ɣ�r
			g_Player.aModel[nCntPlayer].vtxMin.y = vtx.y;	// �ŏ��l��菬������Α��
		}

		if (g_Player.aModel[nCntPlayer].vtxMax.y < vtx.y)
		{// �ő�l�Ɣ�r
			g_Player.aModel[nCntPlayer].vtxMax.y = vtx.y;	// �ő�l���傫����Α��
		}

		// Z�̒l�̔�r
		if (g_Player.aModel[nCntPlayer].vtxMin.z > vtx.z)
		{// �ŏ��l�Ɣ�r
			g_Player.aModel[nCntPlayer].vtxMin.z = vtx.z;	// �ŏ��l��菬������Α��
		}

		if (g_Player.aModel[nCntPlayer].vtxMax.z < vtx.z)
		{// �ő�l�Ɣ�r
			g_Player.aModel[nCntPlayer].vtxMax.z = vtx.z;	// �ő�l���傫����Α��
		}

		// ���_�o�b�t�@���A�����b�N
		g_Player.aModel[nCntPlayer].pMeshModel->UnlockVertexBuffer();
	}
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	DeletePlayer();

	for (int nCount = 0; nCount < MAX_PLAYER_PARTS; nCount++)
	{
		// ���b�V���̊J��
		if (g_Player.aModel[nCount].pMeshModel != NULL)
		{
			g_Player.aModel[nCount].pMeshModel->Release();
			g_Player.aModel[nCount].pMeshModel = NULL;
		}
	}
	for (int nCount = 0; nCount < MAX_PLAYER_PARTS; nCount++)
	{
		// �}�e���A���̊J��
		if (g_Player.aModel[nCount].pBuffMatModel != NULL)
		{
			g_Player.aModel[nCount].pBuffMatModel->Release();
			g_Player.aModel[nCount].pBuffMatModel = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	bool bSwap = GetStageSwat();
	bool bFade = GetFadeNow();
	LOADFADE loadFade = GetLoadFade();

	//���[�h�̃|�C���^
	MODE pMode = GetMode();

	//�J�����̏��
	Camera *pCamera = GetCamera();

	//�O��̈ʒu��ۑ�
	g_Player.posOld = g_Player.pos;

	float nSpeed = 5.0f;	//���x
	//float fDiffAngle;

	//�ǂ̂����蔻��
	if (g_Player.pos.z >= 740)
	{//��
		g_Player.pos.z = 740;
	}
	if (g_Player.pos.z <= -740)
	{//��
		g_Player.pos.z = -740;
	}
	if (g_Player.pos.x >= 740)
	{//�E
		g_Player.pos.x = 740;
	}
	if (g_Player.pos.x <= -740)
	{//��
		g_Player.pos.x = -740;
	}

	//�Q�[����������
	if (pMode == MODE_GAME)
	{
		//�t���[���J�E���g
		g_nCntTimePlayer++;

		if (false == bSwap && false == bFade && LOADFADE_NONE == loadFade)
		{// �X�e�[�W�؂�ւ����Ȃǂ͓����Ȃ�����
			if (GetJoyPadPress(DIJS_BUTTON_10, 0) == true || GetJoyPadPress(DIJS_BUTTON_11, 0) == true || GetJoyPadPress(DIJS_BUTTON_12, 0) == true || GetJoyPadPress(DIJS_BUTTON_13, 0) == true)
			{//�W���C�R��
				g_Player.pos.x -= sinf(pCamera->fAngle - GetJotPadLeftStick(0)) * PLAYER1_SPEED;
				g_Player.pos.z -= cosf(pCamera->fAngle - GetJotPadLeftStick(0)) * PLAYER1_SPEED;

				g_Player.rot.y = pCamera->fAngle - GetJotPadLeftStick(0);

				if (g_Player.nAnimnow != PLAYERANIM_WALK && g_Player.bAttack == false)
				{//�����A�j���[�V����
					g_Player.nAnimnow = PLAYERANIM_WALK;
					g_nCntTimePlayer = 0;

					if (g_Player.bLegWhich == true)
					{// �O�񍶑����o����
						g_Player.nAnimKeynow = 0;
					}
					else
					{// �O��E�����o����
						g_Player.nAnimKeynow = 3;
					}
				}
			}
			else
			{
				if (g_Player.nAnimnow != PLAYERANIM_NEUTRAL && g_Player.bAttack == false)
				{//�ҋ@���[�V����
					g_Player.nAnimnow = PLAYERANIM_NEUTRAL;
					g_nCntTimePlayer = 0;
					g_Player.nAnimKeynow = 0;
				}
			}

			if (GetJoyPadTrigger(DIJS_BUTTON_2, 0) == true && g_Player.type == TYPE_HUMAN)
			{
				if (g_Player.nAnimnow != PLAYERANIM_ATTACK && g_Player.bAttack == false)
				{//�U���A�j���[�V����
					g_Player.bAttack = true;

					g_Player.nAnimnow = PLAYERANIM_ATTACK;
					g_nCntTimePlayer = 0;
					g_Player.nAnimKeynow = 0;

					PlaySound(SOUND_LABEL_SWORDGESTURE);
				}
			}

			int nGauge = GetGauge();	// �Q�[�W�̎擾

			if (nGauge >= MAX_GAUGE)
			{// �Q�[�W���ő�l
				if (GetJoyPadTrigger(DIJS_BUTTON_1, 0) == true)
				{// �ϐg����
					SetParticleEmitter(g_Player.pos, 6);
					g_Player.type = TYPE_DRAGON;
					AddSubtractGauge(-MAX_GAUGE);
				}
			}

			if (g_nCntPlayerType >= DRAGON_TIME)
			{// �h���S������l��
				SetParticleEmitter(g_Player.pos, 6);
				g_Player.type = TYPE_HUMAN;
				g_nCntPlayerType = 0;
			}

			if (g_Player.type == TYPE_DRAGON)
			{
				g_nCntPlayerType++;
			}
		}
		else
		{
			if (g_Player.nAnimnow != PLAYERANIM_NEUTRAL && g_Player.bAttack == false)
			{//�ҋ@���[�V����
				g_Player.nAnimnow = PLAYERANIM_NEUTRAL;
				g_nCntTimePlayer = 0;
				g_Player.nAnimKeynow = 0;
			}
		}

		//�ʒu���ړ��ɑ��
		g_Player.pos += g_Player.move;

		//����������
		g_Player.move.x += (0.0f - g_Player.move.x) * 0.2f;
		g_Player.move.z += (0.0f - g_Player.move.z) * 0.2f;

		//�e�����Ă���
		SetPositionShadow(g_Player.nIdxShadow, g_Player.pos);

		//�����蔻��
		CollisionObject(&g_Player.pos, &g_Player.posOld, &g_Player.move);
		CollisionObject2(&g_Player.pos, &g_Player.posOld, &g_Player.move);
		CollisionObject3(&g_Player.pos, &g_Player.posOld, &g_Player.move);
		CollisionEnemy(&g_Player.pos, &g_Player.posOld, &g_Player.move, 18.0f);
		if (CollisionAttackEnemy(g_Player.pos, 30) && g_Player.type == TYPE_HUMAN)
		{// �G�̍U������
			if (false == g_Player.bDamage)
			{// �_���[�W��ԂłȂ��Ƃ�
				PlaySound(SOUND_LABEL_HIT);
				AddSubtractLife(-1);
				g_Player.bDamage = true;
				g_Player.nCntDamage = 0;

				int nLife = GetLife();
				if (nLife <= 0)
				{// ����
					SetBlood(g_Player.pos, g_Player.rot, 50, 50, D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f));
					SetGameState(GAMESTATE_END);
				}
			}
		}

		if (true == g_Player.bDamage)
		{// ��Ԃ�߂�
			g_Player.nCntDamage++;	// ���G���Ԃ̃J�E���g

			if (g_Player.nCntDamage >= PLAYER_DAMAGE_TIME)
			{// ��莞�Ԍo��
				g_Player.bDamage = false;	// ��Ԃ�߂�
			}
		}

		//�����̃��[�J���ϐ�
		float fDes;
		float frac;

		//�L�[��i�߂�I
		if (g_nCntTimePlayer % g_Player.nAnimfram[g_Player.nAnimnow][g_Player.nAnimKeynow] == 0)
		{
			g_nCntTimePlayer = 0;
			g_Player.nAnimKeynow = (g_Player.nAnimKeynow + 1) % g_Player.nAnimKey[g_Player.nAnimnow];

			if (g_Player.nAnimnow == PLAYERANIM_ATTACK && g_Player.nAnimKey[g_Player.nAnimnow] - 1 == g_Player.nAnimKeynow)
			{//�U���A�j���[�V����
				g_Player.nAnimnow = PLAYERANIM_ATTACK;
				g_nCntTimePlayer = 0;
				g_Player.nAnimKeynow = 0;

				g_Player.bAttack = false;
			}

			if (g_Player.nAnimnow == PLAYERANIM_WALK && g_Player.bAttack == false)
			{//�����A�j���[�V����
			 	if ((g_Player.nAnimKeynow == 1 || g_Player.nAnimKeynow == 4) && g_Player.type == TYPE_HUMAN)
				{// �n�ʂ��R�����Ƃ�
					SetParticleEmitter(D3DXVECTOR3(g_Player.pos.x, 0.0f, g_Player.pos.z), 2);
					PlaySound(SOUND_LABEL_FOOTSTEPS);
				}
				else if(g_Player.nAnimKeynow == 0 || g_Player.nAnimKeynow == 3)
				{// �������~�߂�
					StopSound(SOUND_LABEL_FOOTSTEPS);
				}

				if (g_Player.nAnimKeynow < 3)
				{// �������O�ɏo��
					g_Player.bLegWhich = false;
				}
				else if (g_Player.nAnimKeynow >= 3)
				{// �E�����O�ɏo��
					g_Player.bLegWhich = true;
				}
			}
			else if(g_Player.nAnimnow != PLAYERANIM_WALK)
			{// ���s���[�V�����ȊO�ő������~�߂�
				StopSound(SOUND_LABEL_FOOTSTEPS);
			}
		}

		if (g_Player.pos.y <= 50)
		{// ��
			g_Player.pos.y = 50;
		}

		//�����̌v�Z
		frac = (g_Player.nAnimfram[g_Player.nAnimnow][g_Player.nAnimKeynow] - g_nCntTimePlayer) * 1.0f;

		//�A�j���[�V����
		for (int nCntPlayerAnim = 0; nCntPlayerAnim < g_Player.nMaxPartsNum; nCntPlayerAnim++)
		{
			//===================
			// X�̊p�x
			//===================

			//�ړI�̊p�x������ׂ�
			g_Player.aModel[nCntPlayerAnim].rotDest = g_Player.aModel[nCntPlayerAnim].rotAnim[g_Player.nAnimnow][g_Player.nAnimKeynow];

			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].rotDest.x > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.x -= D3DX_PI * 2.0f;
			}
			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].rotDest.x < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.x += D3DX_PI * 2.0f;
			}

			//
			fDes = g_Player.aModel[nCntPlayerAnim].rotDest.x - g_Player.aModel[nCntPlayerAnim].InitRot.x;

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
			g_Player.aModel[nCntPlayerAnim].InitRot.x += fDes / frac;

			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].InitRot.x > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.x -= D3DX_PI * 2.0f;
			}
			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].InitRot.x < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.x += D3DX_PI * 2.0f;
			}

			//===================
			// Y�̊p�x
			//===================

			//y�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].rotDest.y > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.y -= D3DX_PI * 2.0f;
			}
			//y�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].rotDest.y < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.y += D3DX_PI * 2.0f;
			}

			//
			fDes = g_Player.aModel[nCntPlayerAnim].rotDest.y - g_Player.aModel[nCntPlayerAnim].InitRot.y;

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
			g_Player.aModel[nCntPlayerAnim].InitRot.y += fDes / frac;

			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].InitRot.y > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.y -= D3DX_PI * 2.0f;
			}
			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].InitRot.y < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.y += D3DX_PI * 2.0f;
			}

			//===================
			// Z�̊p�x
			//===================

			//z�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].rotDest.z > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.z -= D3DX_PI * 2.0f;
			}
			//z�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].rotDest.z < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.z += D3DX_PI * 2.0f;
			}

			//
			fDes = g_Player.aModel[nCntPlayerAnim].rotDest.z - g_Player.aModel[nCntPlayerAnim].InitRot.z;

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
			g_Player.aModel[nCntPlayerAnim].InitRot.z += fDes / frac;

			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].InitRot.z > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.z -= D3DX_PI * 2.0f;
			}
			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].InitRot.z < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.z += D3DX_PI * 2.0f;
			}
		}

		// �p�[�c�̈ʒu
		D3DXVECTOR3 DestPos = g_Player.aModel[0].posAnim[g_Player.nAnimnow][g_Player.nAnimKeynow];
		D3DXVECTOR3 defPos;
		frac = (g_Player.nAnimfram[g_Player.nAnimnow][g_Player.nAnimKeynow] - g_Player.nCntPlayer) * 1.0f;

		defPos = DestPos - g_Player.aModel[0].InitPos;
		g_Player.aModel[0].InitPos.x += defPos.x / frac;
		g_Player.aModel[0].InitPos.y += defPos.y / frac;
		g_Player.aModel[0].InitPos.z += defPos.z / frac;
	}
	else if(pMode == MODE_TITLE)
	{
		//�t���[���J�E���g
		g_nCntTimePlayer++;

		if (g_Player.nAnimnow != PLAYERANIM_NEUTRAL && g_Player.bAttack == false)
		{//�ҋ@���[�V����
			g_Player.nAnimnow = PLAYERANIM_NEUTRAL;
			g_nCntTimePlayer = 0;
			g_Player.nAnimKeynow = 0;
		}

		//�����̃��[�J���ϐ�
		float fDes;
		float frac;

		//�L�[��i�߂�I
		if (g_nCntTimePlayer % g_Player.nAnimfram[g_Player.nAnimnow][g_Player.nAnimKeynow] == 0)
		{
			g_nCntTimePlayer = 0;
			g_Player.nAnimKeynow = (g_Player.nAnimKeynow + 1) % g_Player.nAnimKey[g_Player.nAnimnow];

			if (g_Player.nAnimnow == PLAYERANIM_ATTACK && g_Player.nAnimKey[g_Player.nAnimnow] - 1 == g_Player.nAnimKeynow)
			{//�U���A�j���[�V����
				g_Player.nAnimnow = PLAYERANIM_ATTACK;
				g_nCntTimePlayer = 0;
				g_Player.nAnimKeynow = 0;

				g_Player.bAttack = false;
			}
		}

		if (g_Player.pos.y <= 50)
		{// ��
			g_Player.pos.y = 50;
		}

		//�����̌v�Z
		frac = (g_Player.nAnimfram[g_Player.nAnimnow][g_Player.nAnimKeynow] - g_nCntTimePlayer) * 1.0f;

		//�A�j���[�V����
		for (int nCntPlayerAnim = 0; nCntPlayerAnim < g_Player.nMaxPartsNum; nCntPlayerAnim++)
		{
			//===================
			// X�̊p�x
			//===================

			//�ړI�̊p�x������ׂ�
			g_Player.aModel[nCntPlayerAnim].rotDest = g_Player.aModel[nCntPlayerAnim].rotAnim[g_Player.nAnimnow][g_Player.nAnimKeynow];

			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].rotDest.x > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.x -= D3DX_PI * 2.0f;
			}
			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].rotDest.x < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.x += D3DX_PI * 2.0f;
			}

			//
			fDes = g_Player.aModel[nCntPlayerAnim].rotDest.x - g_Player.aModel[nCntPlayerAnim].InitRot.x;

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
			g_Player.aModel[nCntPlayerAnim].InitRot.x += fDes / frac;

			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].InitRot.x > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.x -= D3DX_PI * 2.0f;
			}
			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].InitRot.x < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.x += D3DX_PI * 2.0f;
			}

			//===================
			// Y�̊p�x
			//===================

			//y�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].rotDest.y > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.y -= D3DX_PI * 2.0f;
			}
			//y�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].rotDest.y < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.y += D3DX_PI * 2.0f;
			}

			//
			fDes = g_Player.aModel[nCntPlayerAnim].rotDest.y - g_Player.aModel[nCntPlayerAnim].InitRot.y;

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
			g_Player.aModel[nCntPlayerAnim].InitRot.y += fDes / frac;

			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].InitRot.y > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.y -= D3DX_PI * 2.0f;
			}
			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].InitRot.y < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.y += D3DX_PI * 2.0f;
			}

			//===================
			// Z�̊p�x
			//===================

			//z�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].rotDest.z > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.z -= D3DX_PI * 2.0f;
			}
			//z�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].rotDest.z < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].rotDest.z += D3DX_PI * 2.0f;
			}

			//
			fDes = g_Player.aModel[nCntPlayerAnim].rotDest.z - g_Player.aModel[nCntPlayerAnim].InitRot.z;

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
			g_Player.aModel[nCntPlayerAnim].InitRot.z += fDes / frac;

			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].InitRot.z > D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.z -= D3DX_PI * 2.0f;
			}
			//x�ړI�̈ʒu�̒���
			if (g_Player.aModel[nCntPlayerAnim].InitRot.z < -D3DX_PI)
			{
				g_Player.aModel[nCntPlayerAnim].InitRot.z += D3DX_PI * 2.0f;
			}
		}

		// �p�[�c�̈ʒu
		D3DXVECTOR3 DestPos = g_Player.aModel[0].posAnim[g_Player.nAnimnow][g_Player.nAnimKeynow];
		D3DXVECTOR3 defPos;
		frac = (g_Player.nAnimfram[g_Player.nAnimnow][g_Player.nAnimKeynow] - g_Player.nCntPlayer) * 1.0f;

		defPos = DestPos - g_Player.aModel[0].InitPos;
		g_Player.aModel[0].InitPos.x += defPos.x / frac;
		g_Player.aModel[0].InitPos.y += defPos.y / frac;
		g_Player.aModel[0].InitPos.z += defPos.z / frac;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A����ۑ�
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxParent;						//

	{//�v���C���[
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Player.mtxWorldPlayer);
		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
		D3DXMatrixMultiply(&g_Player.mtxWorldPlayer, &g_Player.mtxWorldPlayer, &mtxRot);
		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
		D3DXMatrixMultiply(&g_Player.mtxWorldPlayer, &g_Player.mtxWorldPlayer, &mtxTrans);
		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorldPlayer);
	}
	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{
		if (g_Player.aModel[nCntParts].nIdxModelParent < 0)
		{//�e���f��
			mtxParent = g_Player.mtxWorldPlayer;
		}
		else
		{//�q���f��
			mtxParent = g_Player.aModel[g_Player.aModel[nCntParts].nIdxModelParent].mtxWorldModel;
		}
		{//�e���f��
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Player.aModel[nCntParts].mtxWorldModel);
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll
			(
				&mtxRot,
				g_Player.aModel[nCntParts].InitRot.y,
				g_Player.aModel[nCntParts].InitRot.x,
				g_Player.aModel[nCntParts].InitRot.z
			);
			D3DXMatrixMultiply
			(
				&g_Player.aModel[nCntParts].mtxWorldModel,
				&g_Player.aModel[nCntParts].mtxWorldModel,
				&mtxRot
			);
			// �ʒu�𔽉f
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_Player.aModel[nCntParts].InitPos.x,
				g_Player.aModel[nCntParts].InitPos.y,
				g_Player.aModel[nCntParts].InitPos.z
			);
			D3DXMatrixMultiply
			(
				&g_Player.aModel[nCntParts].mtxWorldModel,
				&g_Player.aModel[nCntParts].mtxWorldModel,
				&mtxTrans
			);

			D3DXMatrixMultiply
			(
				&g_Player.aModel[nCntParts].mtxWorldModel,
				&g_Player.aModel[nCntParts].mtxWorldModel,
				&mtxParent
			);
			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Player.aModel[nCntParts].mtxWorldModel);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_Player.aModel[nCntParts].pBuffMatModel->GetBufferPointer();

			// �|���S���̕`��
			if (g_Player.bUse == true)
			{
				for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[nCntParts].nNumMatModel; nCntMat++)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					// �e�N�X�`����NULL�ɂ���
					pDevice->SetTexture(0, NULL);

					//pDevice->SetTexture(0, g_Player.nNumMatPlayer);

					// �v���C���[(�p�[�c)�̕`��
					g_Player.aModel[nCntParts].pMeshModel->DrawSubset(nCntMat);
				}
			}
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=========================================================================================================================
// �v���C���[�̍U���̓����蔻��
//=========================================================================================================================
bool CollisionAttack(D3DXVECTOR3 pos, float fRange)
{
	bool bAttack = false;

	// �U���̓����蔻��
	if (g_Player.nAnimnow == PLAYERANIM_ATTACK)
	{// �U�����[�V������
		if (g_Player.nAnimKeynow >= ATTACK_START && g_Player.nAnimKeynow <= ATTACK_END)
		{// ���肠��̃L�[�͈͒�
			float fLengthOneself;

			fLengthOneself = powf(fRange, 3);		// �����XZ�͈̔�

			float fLengthX = pos.x - g_Player.aModel[ATTACK_PARTS].mtxWorldModel._41;	// X�̍�
			float fLengthY = pos.y - (g_Player.aModel[ATTACK_PARTS].mtxWorldModel._42 + g_Player.aModel[ATTACK_PARTS].vtxMax.y);	// Y�̍�
			float fLengthZ = pos.z - g_Player.aModel[ATTACK_PARTS].mtxWorldModel._43;	// Z�̍�
			float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2) + powf(fLengthY, 2);					// XZY�̍��̓��

			if (fLengthOneself >= fLengthTotal)
			{// ����Ǝ����̔���
				if (false == bAttack)
				{// �U�������L���ɂ���
					bAttack = true;
					return bAttack;
				}
			}
		}
	}

	return bAttack;
}

//=========================================================================================================================
// �v���C���[�̎擾����
//=========================================================================================================================
Player *GetPlayer(void)
{
	return &g_Player;
}

//=========================================================================================================================
// �v���C���[�̓����蔻��
//=========================================================================================================================
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange)
{
	bool bPlayer = false;			// �v���C���[�ɐG��Ă��邩
	float fLengthOneself;			// ����̉~��͈�
	float fRange0 = fRange;			// ����͈̔͂̔��a�̒���

	fLengthOneself = powf(fRange0 * 3, 2);		// �����XZ�͈̔�

	float fLengthX = pPos->x - g_Player.mtxWorldPlayer._41;			// X�̍�
	float fLengthY = pPos->y - g_Player.mtxWorldPlayer._42;			// Y�̍�
	float fLengthZ = pPos->z - g_Player.mtxWorldPlayer._43;			// Z�̍�
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2) + powf(fLengthY, 2);					// XZY�̍��̓��

	if (fLengthOneself >= fLengthTotal)
	{// ����Ǝ����̔���
		*pPos = *pPosold;									// �ʒu��߂�
		*pMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ��ʂ�0�ɂ���

		bPlayer = true;		// �v���C���[�ɐG��Ă�����
	}

	return bPlayer;
}
//=========================================================================================================================
// �v���C���[�̏�����
//=========================================================================================================================
void DeletePlayer(void)
{
	// �ʒu�E�����̏����ݒ�
	g_Player.pos = D3DXVECTOR3(0, 0, 40);
	g_Player.rot = D3DXVECTOR3(0, 0, 0);
	g_Player.move = D3DXVECTOR3(0, 0, 0);
	g_Player.nCntPlayer = 1;
	g_Player.bUse = true;
	g_Player.bLand = true;
	g_Player.bAttack = false;
	g_Player.nAnimnow = PLAYERANIM_NEUTRAL;
	g_nCntTimePlayer = 0;
	g_Player.nAnimKeynow = 0;
}

//=========================================================================================================================
// �l���h���S�����̎擾
//=========================================================================================================================
PlayerType GetPlayerType(void)
{
	return g_Player.type;
}