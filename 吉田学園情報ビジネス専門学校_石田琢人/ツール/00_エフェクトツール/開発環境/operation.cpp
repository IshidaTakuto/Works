//=============================================================================
//
// ���쐧�䏈�� [operation.cpp]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#include "operation.h"
#include "string.h"
#include "input.h"
#include "player.h"
#include "particle.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define STRING_FILE	"data/string.txt"		// ������̏����i�[����t�@�C��

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
int g_nNumMode;		// ���[�h�̐���
char g_aStrData[2][MAX_STRING];			// ���[�h�ɂ���Ē��g���ς�镶����̕\��
char g_aString[TYPE_MAX][MAX_STRING];	// �t�@�C������̃f�[�^���i�[
char g_aButton[TYPE_MAX][MAX_STRING];	// �g���L�[�̕\��

bool g_bMove[MAX_EMITTER];		// �p�[�e�B�N���𓮂������ǂ���
char g_aStr[2][MAX_STRING];		// ��������i�[

int g_nSelectParticle;				// �p�[�e�B�N���ԍ��̎w��

//=============================================================================
// ����������
//=============================================================================
void InitOperation(void)
{
	g_nNumMode = 1;

	// �p�[�e�B�N������������
	InitParticle();

	// �O���[�o���ϐ��̏�����
	g_nSelectParticle = 0;

	strcpy(&g_aString[0][0], "[���[�h�I��]");
	strcpy(&g_aString[1][0], "[�t�@�C���o��]");
	strcpy(&g_aString[2][0], "[���l�ύX]");
	strcpy(&g_aString[3][0], "[���ڐ؂�ւ�]");
}

//=============================================================================
// �I������
//=============================================================================
void UninitOperation(void)
{
	UninitParticle();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateOperation(void)
{
	int nSelectEmitter = 0;	// �G�~�b�^�ԍ����L��
	Player *pPlayer;

	pPlayer = GetPlayer();

	switch (g_nNumMode)
	{
	case MODE_PARTICLE:		// �p�[�e�B�N��
		sprintf(&g_aStrData[0][0], "[ MODE_PARTICLE ]");		// ���݂̃��[�h��\��
		sprintf(&g_aStrData[1][0], "[ data/TEXT/EFFECT/save_particle.txt ]");	// �ۑ�����t�@�C����

		if (GetKeyboardTrigger(DIK_P) == true)
		{// �p�[�e�B�N���̃G�~�b�^���o��
			SetParticleEmitter(pPlayer->pos);
		}

		// �G�t�F�N�g�ԍ��̎w��
		if (GetKeyboardRepeat(DIK_2) == true)
		{// ���Z����
			if (g_nSelectParticle < MAX_EMITTER)
			{// �ő���z���Ȃ�
				g_nSelectParticle++;
			}
		}
		else if (GetKeyboardRepeat(DIK_1) == true)
		{// ���Z����
			if (g_nSelectParticle > 0)
			{// 0��艺���Ȃ�
				g_nSelectParticle--;
			}
		}
		wsprintf(&g_aStr[1][0], "[ %d ]", g_nSelectParticle);

		if (GetKeyboardTrigger(DIK_DELETE) == true)
		{// �I�𒆂̃p�[�e�B�N��������
			DestroyParticle(g_nSelectParticle);
		}

		if (GetKeyboardTrigger(DIK_M) == true)
		{// �p�[�e�B�N���𓮂������ǂ���
			g_bMove[g_nSelectParticle] = g_bMove[g_nSelectParticle] ? false : true;
		}
		wsprintf(&g_aStr[0][0], "[ %s ]", g_bMove[g_nSelectParticle] ? "����" : "���Ȃ�");

		for (int nCount = 0; nCount < MAX_EMITTER; nCount++)
		{
			if (g_bMove[nCount] == true)
			{// �����Ă����Ԃ̂Ƃ�
				SetPosParticleEmitter(nCount, pPlayer->pos);	// �ʒu���X�V
			}
		}

		// �p�[�e�B�N���̍X�V
		UpdateParticle();

		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawOperation(void)
{
	// ���݂̃��[�h
	SetString(&g_aStrData[0][0], { 300,10,0,0 }, 0, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	// �o�͂���t�@�C����
	SetString(&g_aStrData[1][0], { 300,30,0,0 }, 0, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	// ���㕔��---------------------------------------------------------------------------------------
	strcpy(&g_aButton[TYPE_MODE][0], &MODE_CHANGE.aKeyNamy[0]);

	strcpy(&g_aButton[TYPE_SELECT][0], &SELECT_DOWN.aKeyNamy[0]);
	strcat(&g_aButton[TYPE_SELECT][0], " ");
	strcat(&g_aButton[TYPE_SELECT][0], &SELECT_UP.aKeyNamy[0]);

	strcpy(&g_aButton[TYPE_NUM][0], &NUM_DOWN.aKeyNamy[0]);
	strcat(&g_aButton[TYPE_NUM][0], " ");
	strcat(&g_aButton[TYPE_NUM][0], &NUM_UP.aKeyNamy[0]);
	strcat(&g_aButton[TYPE_NUM][0], " (�ꕔLSHIFT�Ή�)");

	strcpy(&g_aButton[TYPE_SAVE_FILE][0], &FILE_OUTPUT.aKeyNamy[0]);

	for (int nCntString = 0; nCntString < TYPE_MAX; nCntString++)
	{// �`�悷��
		SetString(&g_aString[nCntString][0], { 10, 10 + (nCntString * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetString(&g_aButton[nCntString][0], { 200, 10 + (nCntString * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	SetString("[ �I�𒆂̐��l���Z�b�g ]", { 10,10 + (TYPE_MAX * 20),0,0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetString("[ ENTER ]", { 200,10 + (TYPE_MAX * 20),0,0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	SetString("[ �J���[�̕ύX]", { 10, 10 + ((TYPE_MAX + 1) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetString("[ (��:r ��:g ��:b �����x:t) �����Ȃ���]", { 200, 10 + ((TYPE_MAX + 1) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	switch (g_nNumMode)
	{
	case MODE_PARTICLE:		// �p�[�e�B�N��
		SetString("[ �ړ��ʂ̕ύX]", { 10, 10 + ((TYPE_MAX + 2) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetString("[ (�w��:x �x��:y �y��:z) �����Ȃ��� ]", { 200, 10 + ((TYPE_MAX + 2) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		SetString("[ �p�[�e�B�N���𐶐� ]", { 10, 10 + ((TYPE_MAX + 3) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetString("[ P ]", { 200, 10 + ((TYPE_MAX + 3) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		SetString("[ �ړ��̗L�� ]", { 10, 10 + ((TYPE_MAX + 4) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetString("[ M ]", { 200, 10 + ((TYPE_MAX + 4) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetString(&g_aStr[0][0], { 300, 10 + ((TYPE_MAX + 4) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

		SetString("[ �I�𒆂̃p�[�e�B�N�� ]", { 10, 10 + ((TYPE_MAX + 5) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetString("[ 1 2 ]", { 200, 10 + ((TYPE_MAX + 5) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetString(&g_aStr[1][0], { 300, 10 + ((TYPE_MAX + 5) * 20), 0, 0 }, 0, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

		DrawParticle();
		break;
	}

}