//=============================================================================
//
// �v���C���[���� [dragon.cpp]
// Author : �L�n�@���u
//
//=============================================================================
#define	_CRT_SECURE_NO_WARNINGS		//scanf�G���[����
#include <stdio.h>					//stdio
#include "dragon.h"					//�v���C���[
#include "input.h"					//�L�[�{�[�h
#include "camera.h"					//�J����
#include "shadow.h"					//�e
#include "sound.h"					//�T�E���h
#include "object.h"					//�I�u�W�F�N�g
#include "object2.h"				//�I�u�W�F�N�g
#include "object3.h"				//�I�u�W�F�N�g
#include "enemy.h"
#include "killLog.h"
#include "game.h"
#include "fade.h"
#include "player.h"
#include "particle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DRAGON1_SPEED			(5.0f)

#define ATTACK_PARTS			(5)
#define ATTACK_START			(0)			// �U���̊J�n�L�[
#define ATTACK_END				(8)			// �U���̏I���L�[
#define ATTACK_RADIUS			(10.0f)		// �c�q�̔��a
#define ATTACK_NUM				(3)			// �c�q�̐�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Dragon		g_Dragon;							//�v���C���[
int			g_nCntTimeDragon;					//�v���C���[�J�E���^�[
float		g_fJoyStickAngleDragon1;			//
float		g_Dragonboxrot = 0;
bool		g_bOnOffDragn = false;
int			g_nCntAttack;		// �U������

//=============================================================================
// ����������
//=============================================================================
void InitDragon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�@�C���̃|�C���^
	FILE *pFile;
	//�v���C���[�J�E���^�[
	int nCntDragon;

	// �ʒu�E�����̏����ݒ�
	g_Dragon.pos = D3DXVECTOR3(0, 0, 40);
	g_Dragon.rot = D3DXVECTOR3(0, 0, 0);
	g_Dragon.move = D3DXVECTOR3(0, 0, 0);
	g_Dragon.nCntDragon = 1;
	g_Dragon.bUse = true;
	g_Dragon.bLand = true;
	g_Dragon.bAttack = false;

	g_nCntAttack = 0;

	//�t�@�C���ݒ�
	pFile = fopen(FILE_DRAGON_NAME, "r");

	//�v���C���[�́��e�L�X�g�f�[�^�̓ǂݍ���
	if (pFile != NULL)
	{
		//�g���p�[�c��
		fscanf(pFile, "%d", &g_Dragon.nMaxPartsNum);	//�ǂݍ���

		//�t�@�C����
		for (nCntDragon = 0; nCntDragon < g_Dragon.nMaxPartsNum; nCntDragon++)
		{
			fscanf(pFile, "%s", &g_Dragon.aModel[nCntDragon].FileName[0]); //�ǂݍ���
		}

		for (nCntDragon = 0; nCntDragon < g_Dragon.nMaxPartsNum; nCntDragon++)
		{//�����l�ƌ���
			fscanf(pFile, "%d", &g_Dragon.aModel[nCntDragon].nIdxModelParent);	//�ǂݍ���
			fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].InitPos.x);		//�ǂݍ���
			fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].InitPos.y);		//�ǂݍ���
			fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].InitPos.z);		//�ǂݍ���
			fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].InitRot.x);		//�ǂݍ���
			fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].InitRot.y);		//�ǂݍ���
			fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].InitRot.z);		//�ǂݍ���
		}

		//�t�@�C���ǂݍ���
		fscanf(pFile, "%d", &g_Dragon.nMaxAnimNum);

		for (int nCntDragonAnim = 0; nCntDragonAnim < g_Dragon.nMaxPartsNum; nCntDragonAnim++)
		{
			//�t���[����
			fscanf(pFile, "%d", &g_Dragon.nAnimloop[nCntDragonAnim]);

			//�L�[��
			fscanf(pFile, "%d", &g_Dragon.nAnimKey[nCntDragonAnim]);

			for (int nCntDragonAnimKey = 0; nCntDragonAnimKey < g_Dragon.nAnimKey[nCntDragonAnim]; nCntDragonAnimKey++)
			{
				fscanf(pFile, "%d", &g_Dragon.nAnimfram[nCntDragonAnim][nCntDragonAnimKey]);//�ǂݍ���

				for (nCntDragon = 0; nCntDragon < g_Dragon.nMaxPartsNum; nCntDragon++)
				{
					fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].posAnim[nCntDragonAnim][nCntDragonAnimKey].x);		//�ǂݍ���
					fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].posAnim[nCntDragonAnim][nCntDragonAnimKey].y);		//�ǂݍ���
					fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].posAnim[nCntDragonAnim][nCntDragonAnimKey].z);		//�ǂݍ���
					fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].rotAnim[nCntDragonAnim][nCntDragonAnimKey].x);		//�ǂݍ���
					fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].rotAnim[nCntDragonAnim][nCntDragonAnimKey].y);		//�ǂݍ���
					fscanf(pFile, "%f", &g_Dragon.aModel[nCntDragon].rotAnim[nCntDragonAnim][nCntDragonAnimKey].z);		//�ǂݍ���
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
	for (nCntDragon = 0; nCntDragon < g_Dragon.nMaxPartsNum; nCntDragon++)
	{
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX
		(
			&g_Dragon.aModel[nCntDragon].FileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Dragon.aModel[nCntDragon].pBuffMatModel,
			NULL,
			&g_Dragon.aModel[nCntDragon].nNumMatModel,
			&g_Dragon.aModel[nCntDragon].pMeshModel
		);
	}

	Player *pPlayer = GetPlayer();

	if (pPlayer->type == TYPE_DRAGON)
	{
		//�e�̏������ݒ�
		g_Dragon.nIdxShadow = SetShadow(g_Dragon.pos, g_Dragon.rot, 25, 25);
	}

	g_Dragon.aModel[ATTACK_PARTS].playerAttack.fRadius = ATTACK_RADIUS;
	g_Dragon.aModel[ATTACK_PARTS].playerAttack.nNumJudgment = ATTACK_NUM;

}

//=============================================================================
// �I������
//=============================================================================
void UninitDragon(void)
{
	for (int nCount = 0; nCount < MAX_DRAGON_PARTS; nCount++)
	{
		// ���b�V���̊J��
		if (g_Dragon.aModel[nCount].pMeshModel != NULL)
		{
			g_Dragon.aModel[nCount].pMeshModel->Release();
			g_Dragon.aModel[nCount].pMeshModel = NULL;
		}
	}
	for (int nCount = 0; nCount < MAX_DRAGON_PARTS; nCount++)
	{
		// �}�e���A���̊J��
		if (g_Dragon.aModel[nCount].pBuffMatModel != NULL)
		{
			g_Dragon.aModel[nCount].pBuffMatModel->Release();
			g_Dragon.aModel[nCount].pBuffMatModel = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateDragon(void)
{
	bool bSwap = GetStageSwat();
	bool bFade = GetFadeNow();

	//�J�����̏��
	Camera *pCamera = GetCamera();

	Player *pPlayer = GetPlayer();

	//�t���[���J�E���g
	g_nCntTimeDragon++;

	//�h���S���̈ʒu�ƃv���C���[�̈ʒu�����킹��
	g_Dragon.pos = pPlayer->pos;

	//�O��̈ʒu��ۑ�
	g_Dragon.posOld = g_Dragon.pos;

	float nSpeed = 5.0f;	//���x
	//float fDiffAngle;

	//�ǂ̂����蔻��
	if (g_Dragon.pos.z >= 740)
	{//��
		g_Dragon.pos.z = 740;
	}
	if (g_Dragon.pos.z <= -740)
	{//��
		g_Dragon.pos.z = -740;
	}
	if (g_Dragon.pos.x >= 740)
	{//�E
		g_Dragon.pos.x = 740;
	}
	if (g_Dragon.pos.x <= -740)
	{//��
		g_Dragon.pos.x = -740;
	}

	//�����̃��[�J���ϐ�
	//float fDes;
	//float frac;
	int nCount = 0;

	//���[�h�̃|�C���^
	MODE pMode = GetMode();

	if (pMode == MODE_GAME)
	{
		if (false == bSwap && false == bFade)
		{
			PlayerType type = GetPlayerType();

			if (GetJoyPadPress(DIJS_BUTTON_10, 0) == true || GetJoyPadPress(DIJS_BUTTON_11, 0) == true || GetJoyPadPress(DIJS_BUTTON_12, 0) == true || GetJoyPadPress(DIJS_BUTTON_13, 0) == true)
			{//�W���C�R��
				g_Dragon.pos.x -= sinf(pCamera->fAngle - GetJotPadLeftStick(0)) * DRAGON1_SPEED;
				g_Dragon.pos.z -= cosf(pCamera->fAngle - GetJotPadLeftStick(0)) * DRAGON1_SPEED;

				g_Dragon.rot.y = pCamera->fAngle - GetJotPadLeftStick(0);

				if (g_Dragon.nAnimnow != DRAGONANIM_WALK && g_Dragon.bAttack == false)
				{//�����A�j���[�V����
					g_Dragon.nAnimnow = DRAGONANIM_WALK;
					g_nCntTimeDragon = 0;
					g_Dragon.nAnimKeynow = 0;
				}
			}
			else if (GetJoyPadPress(DIJS_BUTTON_2, 0) == true && type == TYPE_DRAGON)
			{
				if (g_Dragon.nAnimnow != DRAGONANIM_ATTACK && g_Dragon.bAttack == false)
				{//�U���A�j���[�V����
					g_Dragon.bAttack = true;

					g_Dragon.nAnimnow = DRAGONANIM_ATTACK;
					g_nCntTimeDragon = 0;
					g_Dragon.nAnimKeynow = 0;
					PlaySound(SOUND_LABEL_BREATH);
				}
			}
			else
			{
				if (g_Dragon.nAnimnow != DRAGONANIM_NEUTRAL && g_Dragon.bAttack == false)
				{//�ҋ@���[�V����
					g_Dragon.nAnimnow = DRAGONANIM_NEUTRAL;
					g_nCntTimeDragon = 0;
					g_Dragon.nAnimKeynow = 0;
				}
			}
		}
		else
		{
			if (g_Dragon.nAnimnow != DRAGONANIM_NEUTRAL && g_Dragon.bAttack == false)
			{//�ҋ@���[�V����
				g_Dragon.nAnimnow = DRAGONANIM_NEUTRAL;
				g_nCntTimeDragon = 0;
				g_Dragon.nAnimKeynow = 0;
			}
		}

		if (pPlayer->type == TYPE_DRAGON)
		{// �h���S��
			//�e�����Ă���
			SetPositionShadow(g_Dragon.nIdxShadow, g_Dragon.pos);

			//�����蔻��
			CollisionObject(&g_Dragon.pos, &g_Dragon.posOld, &g_Dragon.move);
			CollisionObject2(&g_Dragon.pos, &g_Dragon.posOld, &g_Dragon.move);
			CollisionObject3(&g_Dragon.pos, &g_Dragon.posOld, &g_Dragon.move);
			CollisionEnemy(&g_Dragon.pos, &g_Dragon.posOld, &g_Dragon.move, 18.0f);

			// �U���̉�
			if (g_Dragon.nAnimnow == DRAGONANIM_ATTACK)
			{// ������΂��o��
				if (g_Dragon.nAnimKeynow > 3 && g_Dragon.nAnimKeynow < g_Dragon.nAnimKey[DRAGONANIM_ATTACK])
				{// ���[�V�����ɍ��킹��
					SetParticleEmitter(D3DXVECTOR3(g_Dragon.aModel[4].mtxWorldModel._41, g_Dragon.aModel[4].mtxWorldModel._42, g_Dragon.aModel[4].mtxWorldModel._43), 7);
				}
			}

			//�����̃��[�J���ϐ�
			float fDes;
			float frac;

			//�L�[��i�߂�I
			if (g_nCntTimeDragon % g_Dragon.nAnimfram[g_Dragon.nAnimnow][g_Dragon.nAnimKeynow] == 0)
			{
				g_nCntTimeDragon = 0;
				g_Dragon.nAnimKeynow = (g_Dragon.nAnimKeynow + 1) % g_Dragon.nAnimKey[g_Dragon.nAnimnow];

				if (g_Dragon.nAnimnow == DRAGONANIM_ATTACK && g_Dragon.nAnimKey[g_Dragon.nAnimnow] - 1 == g_Dragon.nAnimKeynow)
				{//�U���A�j���[�V����
					g_Dragon.nAnimnow = DRAGONANIM_ATTACK;
					g_nCntTimeDragon = 0;
					g_Dragon.nAnimKeynow = 0;
				}
			}

			if (g_Dragon.bAttack)
			{// �U����
				g_nCntAttack++;

				if (g_nCntAttack > 90)
				{
					g_Dragon.bAttack = false;
					g_nCntAttack = 0;
				}
			}

			//�����̌v�Z
			frac = (g_Dragon.nAnimfram[g_Dragon.nAnimnow][g_Dragon.nAnimKeynow] - g_nCntTimeDragon) * 1.0f;

			//�A�j���[�V����
			for (int nCntDragonAnim = 0; nCntDragonAnim < g_Dragon.nMaxPartsNum; nCntDragonAnim++)
			{
				//===================
				// X�̊p�x
				//===================

				//�ړI�̊p�x������ׂ�
				g_Dragon.aModel[nCntDragonAnim].rotDest = g_Dragon.aModel[nCntDragonAnim].rotAnim[g_Dragon.nAnimnow][g_Dragon.nAnimKeynow];

				//x�ړI�̈ʒu�̒���
				if (g_Dragon.aModel[nCntDragonAnim].rotDest.x > D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].rotDest.x -= D3DX_PI * 2.0f;
				}
				//x�ړI�̈ʒu�̒���
				if (g_Dragon.aModel[nCntDragonAnim].rotDest.x < -D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].rotDest.x += D3DX_PI * 2.0f;
				}

				//
				fDes = g_Dragon.aModel[nCntDragonAnim].rotDest.x - g_Dragon.aModel[nCntDragonAnim].InitRot.x;

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
				g_Dragon.aModel[nCntDragonAnim].InitRot.x += fDes / frac;

				//x�ړI�̈ʒu�̒���
				if (g_Dragon.aModel[nCntDragonAnim].InitRot.x > D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].InitRot.x -= D3DX_PI * 2.0f;
				}
				//x�ړI�̈ʒu�̒���
				if (g_Dragon.aModel[nCntDragonAnim].InitRot.x < -D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].InitRot.x += D3DX_PI * 2.0f;
				}

				//===================
				// Y�̊p�x
				//===================

				//y�ړI�̈ʒu�̒���
				if (g_Dragon.aModel[nCntDragonAnim].rotDest.y > D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].rotDest.y -= D3DX_PI * 2.0f;
				}
				//y�ړI�̈ʒu�̒���
				if (g_Dragon.aModel[nCntDragonAnim].rotDest.y < -D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].rotDest.y += D3DX_PI * 2.0f;
				}

				//
				fDes = g_Dragon.aModel[nCntDragonAnim].rotDest.y - g_Dragon.aModel[nCntDragonAnim].InitRot.y;

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
				g_Dragon.aModel[nCntDragonAnim].InitRot.y += fDes / frac;

				//x�ړI�̈ʒu�̒���
				if (g_Dragon.aModel[nCntDragonAnim].InitRot.y > D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].InitRot.y -= D3DX_PI * 2.0f;
				}
				//x�ړI�̈ʒu�̒���
				if (g_Dragon.aModel[nCntDragonAnim].InitRot.y < -D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].InitRot.y += D3DX_PI * 2.0f;
				}

				//===================
				// Z�̊p�x
				//===================

				//z�ړI�̈ʒu�̒���
				if (g_Dragon.aModel[nCntDragonAnim].rotDest.z > D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].rotDest.z -= D3DX_PI * 2.0f;
				}
				//z�ړI�̈ʒu�̒���
				if (g_Dragon.aModel[nCntDragonAnim].rotDest.z < -D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].rotDest.z += D3DX_PI * 2.0f;
				}

				//
				fDes = g_Dragon.aModel[nCntDragonAnim].rotDest.z - g_Dragon.aModel[nCntDragonAnim].InitRot.z;

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
				g_Dragon.aModel[nCntDragonAnim].InitRot.z += fDes / frac;

				//x�ړI�̈ʒu�̒���
				if (g_Dragon.aModel[nCntDragonAnim].InitRot.z > D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].InitRot.z -= D3DX_PI * 2.0f;
				}
				//x�ړI�̈ʒu�̒���
				if (g_Dragon.aModel[nCntDragonAnim].InitRot.z < -D3DX_PI)
				{
					g_Dragon.aModel[nCntDragonAnim].InitRot.z += D3DX_PI * 2.0f;
				}
			}
			// �p�[�c�̈ʒu
			D3DXVECTOR3 DestPos = g_Dragon.aModel[0].posAnim[g_Dragon.nAnimnow][g_Dragon.nAnimKeynow];
			D3DXVECTOR3 defPos;
			frac = (g_Dragon.nAnimfram[g_Dragon.nAnimnow][g_Dragon.nAnimKeynow] - g_Dragon.nCntDragon) * 1.0f;

			defPos = DestPos - g_Dragon.aModel[0].InitPos;
			g_Dragon.aModel[0].InitPos.x += defPos.x / frac;
			g_Dragon.aModel[0].InitPos.y += defPos.y / frac;
			g_Dragon.aModel[0].InitPos.z += defPos.z / frac;
		}
		//�ʒu���ړ��ɑ��
		g_Dragon.pos += g_Dragon.move;

		//����������
		g_Dragon.move.x += (0.0f - g_Dragon.move.x) * 0.2f;
		g_Dragon.move.z += (0.0f - g_Dragon.move.z) * 0.2f;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawDragon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A����ۑ�
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxParent;						//

	{//�v���C���[
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Dragon.mtxWorldDragon);
		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Dragon.rot.y, g_Dragon.rot.x, g_Dragon.rot.z);
		D3DXMatrixMultiply(&g_Dragon.mtxWorldDragon, &g_Dragon.mtxWorldDragon, &mtxRot);
		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Dragon.pos.x, g_Dragon.pos.y, g_Dragon.pos.z);
		D3DXMatrixMultiply(&g_Dragon.mtxWorldDragon, &g_Dragon.mtxWorldDragon, &mtxTrans);
		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Dragon.mtxWorldDragon);
	}
	for (int nCntParts = 0; nCntParts < MAX_DRAGON_PARTS; nCntParts++)
	{
		if (g_Dragon.aModel[nCntParts].nIdxModelParent < 0)
		{//�e���f��
			mtxParent = g_Dragon.mtxWorldDragon;
		}
		else
		{//�q���f��
			mtxParent = g_Dragon.aModel[g_Dragon.aModel[nCntParts].nIdxModelParent].mtxWorldModel;
		}
		{//�e���f��
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Dragon.aModel[nCntParts].mtxWorldModel);
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll
			(
				&mtxRot,
				g_Dragon.aModel[nCntParts].InitRot.y,
				g_Dragon.aModel[nCntParts].InitRot.x,
				g_Dragon.aModel[nCntParts].InitRot.z
			);
			D3DXMatrixMultiply
			(
				&g_Dragon.aModel[nCntParts].mtxWorldModel,
				&g_Dragon.aModel[nCntParts].mtxWorldModel,
				&mtxRot
			);
			// �ʒu�𔽉f
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_Dragon.aModel[nCntParts].InitPos.x,
				g_Dragon.aModel[nCntParts].InitPos.y,
				g_Dragon.aModel[nCntParts].InitPos.z
			);
			D3DXMatrixMultiply
			(
				&g_Dragon.aModel[nCntParts].mtxWorldModel,
				&g_Dragon.aModel[nCntParts].mtxWorldModel,
				&mtxTrans
			);

			D3DXMatrixMultiply
			(
				&g_Dragon.aModel[nCntParts].mtxWorldModel,
				&g_Dragon.aModel[nCntParts].mtxWorldModel,
				&mtxParent
			);
			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Dragon.aModel[nCntParts].mtxWorldModel);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_Dragon.aModel[nCntParts].pBuffMatModel->GetBufferPointer();

			// �|���S���̕`��
			if (g_Dragon.bUse == true)
			{
				for (int nCntMat = 0; nCntMat < (int)g_Dragon.aModel[nCntParts].nNumMatModel; nCntMat++)
				{
					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					// �e�N�X�`����NULL�ɂ���
					pDevice->SetTexture(0, NULL);

					//pDevice->SetTexture(0, g_Dragon.nNumMatDragon);

					// �v���C���[(�p�[�c)�̕`��
					g_Dragon.aModel[nCntParts].pMeshModel->DrawSubset(nCntMat);
				}
			}
			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=========================================================================================================================
// �v���C���[�̎擾����
//=========================================================================================================================
Dragon *GetDragon(void)
{
	return &g_Dragon;
}

//=========================================================================================================================
// �v���C���[�̓����蔻��
//=========================================================================================================================
bool CollisionDragon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosold, D3DXVECTOR3 *pMove, float fRange)
{
	bool bDragon = false;					// �v���C���[�ɐG��Ă��邩
	float fLengthOneself;			// ����̉~��͈�
	float fRange0 = fRange;			// ����͈̔͂̔��a�̒���

	fLengthOneself = powf(fRange0 * 3, 2);		// �����XZ�͈̔�

	float fLengthX = pPos->x - g_Dragon.mtxWorldDragon._41;			// X�̍�
	float fLengthY = pPos->y - g_Dragon.mtxWorldDragon._42;	// Y�̍�
	float fLengthZ = pPos->z - g_Dragon.mtxWorldDragon._43;			// Z�̍�
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2) + powf(fLengthY, 2);					// XZY�̍��̓��

	if (fLengthOneself >= fLengthTotal)
	{// ����Ǝ����̔���
		*pPos = *pPosold;									// �ʒu��߂�
		*pMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ��ʂ�0�ɂ���

		bDragon = true;		// �v���C���[�ɐG��Ă�����
	}

	return bDragon;
}
//=========================================================================================================================
// �v���C���[�̏�����
//=========================================================================================================================
void DeleteDragon(void)
{
	// �ʒu�E�����̏����ݒ�
	g_Dragon.pos = D3DXVECTOR3(0, 0, 40);
	g_Dragon.rot = D3DXVECTOR3(0, 0, 0);
	g_Dragon.move = D3DXVECTOR3(0, 0, 0);
	g_Dragon.nCntDragon = 1;
	g_Dragon.bUse = true;
	g_Dragon.bLand = true;
	g_Dragon.bAttack = false;
}

//=========================================================================================================================
// �h���S���̈ʒu�̐ݒ�
//=========================================================================================================================
void SetPosDragon(D3DXVECTOR3 pos)
{
	g_Dragon.pos = pos;
}

//=========================================================================================================================
// �h���S���̈ʒu�̎擾
//=========================================================================================================================
D3DXVECTOR3 GetPosDragon(void)
{
	return g_Dragon.pos;
}

//=========================================================================================================================
// ��̊p�x�̎擾
//=========================================================================================================================
D3DXVECTOR3 GetNeckRot(void)
{
	//return g_Dragon.aModel[3].mtxWorldModel._32;

	return g_Dragon.rot;
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CollisionDragonAttack(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// �U���̓����蔻��
	if (g_Dragon.nAnimnow == PLAYERANIM_ATTACK)
	{// �U�����[�V������
		if (g_Dragon.nAnimKeynow >= 3 && g_Dragon.nAnimKeynow <= g_Dragon.nAnimKey[g_Dragon.nAnimnow])
		{// ���肠��̃L�[�͈͒�

			float fLengthOneself;		// ����̉~��͈�
			float fRange0 = fRange;		// ����͈̔͂̔��a�̒���

			fLengthOneself = powf(fRange0 * 2, 2);		// �����XZ�͈̔�

			float fLengthX = pos.x - (g_Dragon.mtxWorldDragon._41 + (sinf(g_Dragon.rot.y) * 1.0f));	// X�̍�
			float fLengthY = pos.y - g_Dragon.mtxWorldDragon._42;	// Y�̍�
			float fLengthZ = pos.z - (g_Dragon.mtxWorldDragon._43 + (cosf(g_Dragon.rot.y) * 1.0f));	// Z�̍�
			float fLengthTotal = powf(fLengthX, 2) + powf(fLengthZ, 2) + powf(fLengthY, 2);					// XZY�̍��̓��

			if (fLengthOneself >= fLengthTotal)
			{// ����Ǝ����̔���
				bHit = true;
			}
		}
	}
	return bHit;
}