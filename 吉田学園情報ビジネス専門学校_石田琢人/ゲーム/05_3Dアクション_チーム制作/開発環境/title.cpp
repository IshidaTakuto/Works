//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : shotaro kanamaru
//
//=============================================================================
#include "main.h"		// ���C��
#include "title.h"		// �^�C�g��
#include "input.h"		// ����
#include "sound.h"		// ��
#include "fade.h"		// �t�F�[�h
#include "camera.h"
#include "light.h"
#include "meshField.h"
#include "wall.h"
#include "object.h"
#include "objectnot.h"
#include "player.h"
#include "shadow.h"
#include "meshbg.h"
#include "rain.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define MAX_TITLE		(10)				// �ő吔
#define TITLE_DOTSIZE	(4.0f)				// �h�b�g�T�C�Y
// �܍�
#define TITLESCARS_TEX		"data/TEXTURE/title_scar.png"		// �e�N�X�`��
#define TITLESCARS_START	(30)								// �����J�n�̎���
#define TITLESCARS_ANIM		(6)									// �A�j���[�V������
#define TITLESCARS_SWITCH	(4)									// �A�j���[�V�������x
#define TITLESCARS_SIZE		D3DXVECTOR3(124.0f,145.0f,0.0f)		// �傫��
#define TITLESCARS_POS		D3DXVECTOR3(440.0f, 320.0f, 0.0f)	// �����ʒu
// �^�C�g���A�C�R��
#define TITLEICON_TEX		"data/TEXTURE/title_icon.png"		// �e�N�X�`��
#define TITLEICON_START		(43)								// �����J�n�̎���
#define TITLEICON_SIZE		D3DXVECTOR3(98.0f,79.0f,0.0f)		// �傫��
#define TITLEICON_POS		D3DXVECTOR3(440.0f, 320.0f, 0.0f)	// �����ʒu
// �^�C�g������
#define TITLECHAR_TEX		"data/TEXTURE/title_char.png"		// �e�N�X�`��
#define TITLECHAR_START		(78)								// �����J�n�̎���
#define TITLECHAR_SIZE		D3DXVECTOR3(144.0f,51.0f,0.0f)		// �傫��
#define TITLECHAR_POS		D3DXVECTOR3(440.0f, 500.0f, 0.0f)	// �����ʒu
#define TITLECHAR_POSY		(320.0f)							// �w��y�ʒu
// �L�[���͑���
#define TITLEPRESS_TEX		"data/TEXTURE/pressany.png"			// �e�N�X�`��
#define TITLEPRESS_SIZE		D3DXVECTOR3(200.0f,40.0f,0.0f)		// �傫��
#define TITLEPRESS_POS		D3DXVECTOR3(440.0f, 550.0f, 0.0f)	// �����ʒu
#define TITLE_COLASPEED		(0.08f)								// �����x�̕ω����x

//*****************************************************************************
//�v���g�^�C�v�錾
//*****************************************************************************
void TitleMakeVtx(void);						// ���_���̍쐬
void TitleScars(int nIndex);					// �܍�����
void TitleIcon(int nIndex);						// �^�C�g���A�C�R���̏���
void TitleChar(int nIndex);						// �^�C�g�������̏���
void TitlePress(int nIndex);					// �L�[���͑����̏���
void SkipEvent(int nIndex);						// ���o�X�L�b�v�̏���
void TitleColAFixed(int nIndex);				// �����x�̕��̌Œ�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Title			g_aTitle[MAX_TITLE];			// �^�C�g�����
TitleOther		g_titleOther;					// ���̑��ݒ���

//=============================================================================
//�^�C�g���̏���������
//=============================================================================
void InitTitle(void)
{
	// �^�C�g���̐ݒ���̏�����
	g_titleOther.nNum = 0;								// ����
	g_titleOther.nCntAll = 0;							// ���ʃJ�E���^
	g_titleOther.nPatternAnim = 0;						// �A�j���[�V�����p�^�[��
	g_titleOther.fAnimWidth = 1.0f / TITLESCARS_ANIM;	// �A�j���[�V�����̕�
	g_titleOther.nCntChar = 0;							// �^�C�g�������J�E���^
	g_titleOther.nCntFlash = 0;							// �t���b�V���J�E���^
	g_titleOther.bFlash = false;						// �t���b�V���̎g�p
	g_titleOther.bPress = false;						// �L�[���͑����̎g�p
	g_titleOther.bEvent = true;							// ���o

	// ���ڂ��Ƃ̏��ݒ�
	TitleInfo aTitletype[] =
	{// [�ʒu]:[�傫��]:[���]
		/*�܍�*/			TITLESCARS_POS,	TITLESCARS_SIZE,TITLE_SCARS,
		/*�^�C�g���A�C�R��*/TITLEICON_POS,	TITLEICON_SIZE,	TITLE_ICON,
		/*�^�C�g������*/	TITLECHAR_POS,	TITLECHAR_SIZE,	TITLE_CHAR,
		/*�L�[���͑���*/	TITLEPRESS_POS,	TITLEPRESS_SIZE,TITLE_PRESS,
	};

	// �ݒ肵���������������Z
	g_titleOther.nNum += sizeof aTitletype / sizeof(TitleInfo);

	for (int nCntTitle = 0; nCntTitle < g_titleOther.nNum; nCntTitle++)
	{
		g_aTitle[nCntTitle].pos = aTitletype[nCntTitle].pos;		// �ʒu
		g_aTitle[nCntTitle].posInit = g_aTitle[nCntTitle].pos;		// �����ʒu
		g_aTitle[nCntTitle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_aTitle[nCntTitle].size = aTitletype[nCntTitle].size * TITLE_DOTSIZE / 2;	// �傫��
		g_aTitle[nCntTitle].type = aTitletype[nCntTitle].type;		// �^�C�v
		g_aTitle[nCntTitle].col = D3DXCOLOR(0.4f, 0.7f, 0.5f, 0.0f);// �F
	}

	g_aTitle[TITLE_SCARS].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);// �F

	//�J�����̏���������
	InitCamera();

	InitLight();

	InitMeshField();

	InitMeshBg();

	InitShadow();

	InitPlayer();

	InitWall();

	InitObject();

	InitObjectNot();

	InitRain();

	//�ǂ̔z�u
	//��
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 870.0f), D3DXVECTOR3(0.0f, 0.0f * D3DX_PI, 0.0f));
	//�E
	SetWall(D3DXVECTOR3(870.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.5f * D3DX_PI, 0.0f));
	//��
	SetWall(D3DXVECTOR3(0.0f, 0.0f, -870.0f), D3DXVECTOR3(0.0f, 1.0f * D3DX_PI, 0.0f));
	//��
	SetWall(D3DXVECTOR3(-870.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -0.5f * D3DX_PI, 0.0f));

	//================================================================================
	// �����蔻��̂���I�u�W�F�N�g
	//================================================================================
	for (int nCount = 0; nCount < 5; nCount++)
	{
		//�E��
		SetObject(D3DXVECTOR3(800.0f, 0.0f, nCount * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetObject(D3DXVECTOR3(800.0f, 0.0f, nCount * -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//����
		SetObject(D3DXVECTOR3(-800.0f, 0.0f, nCount * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetObject(D3DXVECTOR3(-800.0f, 0.0f, nCount * -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//���
		SetObject(D3DXVECTOR3(nCount * 200.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetObject(D3DXVECTOR3(nCount * -200.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//����
		SetObject(D3DXVECTOR3(nCount * -200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetObject(D3DXVECTOR3(nCount * 200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}//40

	 //�E��p
	SetObject(D3DXVECTOR3(640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(660.0f, 0.0f, 540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(620.0f, 0.0f, 360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(730.0f, 0.0f, 120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(490.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(320.0f, 0.0f, 630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(120.0f, 0.0f, 730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//�����p
	SetObject(D3DXVECTOR3(-640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-660.0f, 0.0f, -540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-620.0f, 0.0f, -360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-730.0f, 0.0f, -120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-490.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-320.0f, 0.0f, -630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-120.0f, 0.0f, -730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//�E���p
	SetObject(D3DXVECTOR3(640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(660.0f, 0.0f, -540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(620.0f, 0.0f, -360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(730.0f, 0.0f, -120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(490.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(320.0f, 0.0f, -630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(120.0f, 0.0f, -730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//����p
	SetObject(D3DXVECTOR3(-640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-660.0f, 0.0f, 540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-620.0f, 0.0f, 360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-730.0f, 0.0f, 120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-490.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-320.0f, 0.0f, 630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-120.0f, 0.0f, 730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�}�b�v�ɎU��΂߂郂�f��
	SetObject(D3DXVECTOR3(-120.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(-320.0f, 0.0f, -240.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(340.0f, 0.0f, 40.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObject(D3DXVECTOR3(160.0f, 0.0f, -320.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//���݌� : 72

	//================================================================================
	//�����蔻��̂Ȃ��I�u�W�F�N�g
	//================================================================================
	for (int nCount = 0; nCount < 5; nCount++)
	{
		//�E��
		SetObjectNot(D3DXVECTOR3(800.0f, 0.0f, nCount * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetObjectNot(D3DXVECTOR3(800.0f, 0.0f, nCount * -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//����
		SetObjectNot(D3DXVECTOR3(-800.0f, 0.0f, nCount * 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetObjectNot(D3DXVECTOR3(-800.0f, 0.0f, nCount * -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//���
		SetObjectNot(D3DXVECTOR3(nCount * 200.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetObjectNot(D3DXVECTOR3(nCount * -200.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//����
		SetObjectNot(D3DXVECTOR3(nCount * -200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetObjectNot(D3DXVECTOR3(nCount * 200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}//40

	 //�E��p
	SetObjectNot(D3DXVECTOR3(640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(660.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(620.0f, 0.0f, 360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(730.0f, 0.0f, 120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(490.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(320.0f, 0.0f, 630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(120.0f, 0.0f, 730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//�����p
	SetObjectNot(D3DXVECTOR3(-640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(-660.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(-620.0f, 0.0f, -360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(-730.0f, 0.0f, -120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(-490.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(-320.0f, 0.0f, -630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(-120.0f, 0.0f, -730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//�E���p
	SetObjectNot(D3DXVECTOR3(640.0f, 0.0f, -650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(660.0f, 0.0f, -540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(620.0f, 0.0f, -360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(730.0f, 0.0f, -120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(490.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(320.0f, 0.0f, -630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(120.0f, 0.0f, -730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//����p
	SetObjectNot(D3DXVECTOR3(-640.0f, 0.0f, 650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(-660.0f, 0.0f, 540.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(-620.0f, 0.0f, 360.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(-730.0f, 0.0f, 120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(-490.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(-320.0f, 0.0f, 630.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(-120.0f, 0.0f, 730.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�}�b�v�ɎU��΂߂郂�f��
	SetObjectNot(D3DXVECTOR3(-120.0f, 0.0f, 340.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(-320.0f, 0.0f, -240.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(340.0f, 0.0f, 40.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetObjectNot(D3DXVECTOR3(160.0f, 0.0f, -320.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//���݌� : 72

	TitleMakeVtx();		// ���_���̍쐬
}

//=============================================================================
//�^�C�g���̏I������
//=============================================================================
void UninitTitle(void)
{
	// ���_�o�b�t�@�̔j��
	if (g_titleOther.pVtxBuff != NULL)
	{
		g_titleOther.pVtxBuff->Release();
		g_titleOther.pVtxBuff = NULL;
	}
	// �e�N�X�`���̔j��
	for (int nCntType = 0; nCntType < TITLE_MAX; nCntType++)
	{
		if (g_titleOther.pTexture[nCntType] != NULL)
		{
			g_titleOther.pTexture[nCntType]->Release();
			g_titleOther.pTexture[nCntType] = NULL;
		}
	}

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	UninitWall();

	UninitShadow();

	UninitPlayer();

	UninitObject();

	UninitObjectNot();

	UninitMeshField();

	UninitRain();

	UninitMeshBg();
}

//=============================================================================
//�^�C�g���̍X�V����
//=============================================================================
void UpdateTitle(void)
{
	VERTEX_2D*pVtx;  //���_���ւ̃|�C���^

	if (g_titleOther.pVtxBuff != NULL)
	{
		g_titleOther.nCntAll++;		// ���ʃJ�E���^�̉��Z

		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_titleOther.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ������SetFade�Ȃǂ��肢���܂�
		FADE fade = *GetFade();
		if (fade == FADE_NONE)
		{
			if (GetAnyTrigger(1, 0) == true || GetKeyboardTrigger(DIK_RETURN))
			{// ����L�[����
				PlaySound(SOUND_LABEL_SELECT);
				SetFadeMode(MODE_TUTORIAL);
				g_titleOther.bFlash = true;		// �����̃t���b�V��
				g_titleOther.bPress = false;	// �L�[���͑�����off��
			}
		}

		SkipEvent(0);

		for (int nCntTitle = 0; nCntTitle < g_titleOther.nNum; nCntTitle++, pVtx += 4)
		{
			TitleScars(nCntTitle);		// �܍��̏���
			TitleIcon(nCntTitle);		// �^�C�g���A�C�R���̏���
			TitleChar(nCntTitle);		// �^�C�g�������̏���
			TitlePress(nCntTitle);		// �L�[���͑����̏���
			TitleColAFixed(nCntTitle);	// �����x�̕��̌Œ�

			// �����̉��Z
			g_aTitle[nCntTitle].move += (D3DXVECTOR3(0.0f, 0.0f, 0.0f) - g_aTitle[nCntTitle].move) * 0.1f;

			// �ړ��ʂ̉��Z
			g_aTitle[nCntTitle].pos += g_aTitle[nCntTitle].move;

			// ���_���W
			pVtx[0].pos = D3DXVECTOR3(-g_aTitle[nCntTitle].size.x, -g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aTitle[nCntTitle].size.x, -g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aTitle[nCntTitle].size.x, g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aTitle[nCntTitle].size.x, g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;

			// ���_�J���[
			pVtx[0].col = g_aTitle[nCntTitle].col;
			pVtx[1].col = g_aTitle[nCntTitle].col;
			pVtx[2].col = g_aTitle[nCntTitle].col;
			pVtx[3].col = g_aTitle[nCntTitle].col;
		}

		//���_�o�b�t�@���A�����b�N
		g_titleOther.pVtxBuff->Unlock();
	}

	//���C�g�̍X�V����
	UpdateLight();

	//�J�����̍X�V����
	UpdateCamera();

	UpdateShadow();

	UpdatePlayer();

	UpdateMeshField();

	UpdateMeshBg();

	UpdateWall();

	UpdateObject();

	UpdateObjectNot();

	UpdateRain();
}

//=============================================================================
//�^�C�g���̕`�揈��
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	//�J�����̕`�揈��
	SetCamera(CAMERATYPE_TITLE);

	DrawObject();

	DrawObjectNot();

	DrawShadow();

	DrawPlayer();

	DrawMeshField();

	DrawWall();

	DrawMeshBg();

	DrawRain();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_titleOther.pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTitle = 0; nCntTitle < g_titleOther.nNum; nCntTitle++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_titleOther.pTexture[g_aTitle[nCntTitle].type]);
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTitle, 2);
	}
}

//=============================================================================
// �܍�����
//=============================================================================
void TitleScars(int nIndex)
{
	if (g_aTitle[nIndex].type == TITLE_SCARS)
	{// �܍��̎�
		VERTEX_2D*pVtx;  //���_���ւ̃|�C���^

		if (g_titleOther.nCntAll > TITLESCARS_START)
		{// �w��̎��ԂɂȂ�����
			if (g_titleOther.nPatternAnim != TITLESCARS_ANIM - 1)
			{// �A�j���[�V�������Ō�܂œ��B���Ă��Ȃ��Ƃ�
				if ((g_titleOther.nCntAll % TITLESCARS_SWITCH) == 0)
				{// �w��̊Ԋu�ɂȂ�����
					// �p�^�[���ԍ����X�V
					g_titleOther.nPatternAnim = (g_titleOther.nPatternAnim + 1) % TITLESCARS_ANIM;
				}
			}
		}

		g_aTitle[nIndex].col.a = 1.0f;	// �s������

		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_titleOther.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(g_titleOther.fAnimWidth * g_titleOther.nPatternAnim, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_titleOther.fAnimWidth * g_titleOther.nPatternAnim + g_titleOther.fAnimWidth, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_titleOther.fAnimWidth * g_titleOther.nPatternAnim, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_titleOther.fAnimWidth * g_titleOther.nPatternAnim + g_titleOther.fAnimWidth, 1.0f);

		//���_�o�b�t�@���A�����b�N
		g_titleOther.pVtxBuff->Unlock();
	}
}

//=============================================================================
// �^�C�g���A�C�R���̏���
//=============================================================================
void TitleIcon(int nIndex)
{
	if (g_aTitle[nIndex].type == TITLE_ICON)
	{// �^�C�g�������̎�
		if (g_titleOther.nCntAll > TITLEICON_START)
		{// �w��̎��ԂɂȂ�����
			g_aTitle[nIndex].col.a += 0.01f;		// �s�����x�̉��Z
		}

		if (!g_titleOther.bEvent)
		{// �C�x���g�X�L�b�v��
			g_aTitle[nIndex].col.a = 1.0f;				// �s������
		}
	}
}

//=============================================================================
// �^�C�g�������̏���
//=============================================================================
void TitleChar(int nIndex)
{
	if (g_aTitle[nIndex].type == TITLE_CHAR)
	{// �^�C�g�������̎�
			if (g_titleOther.nCntAll > TITLECHAR_START)
			{// �w��̎��ԂɂȂ�����
				g_aTitle[nIndex].col.a += 0.01f;		// �s�����x�̉��Z

				if (g_aTitle[nIndex].pos.y < TITLECHAR_POSY)
				{// �w��ʒu�ɗ�����
					g_titleOther.nCntChar++;			// �����J�E���^�����Z
					g_aTitle[nIndex].move.y = 0.0f;

					if (g_titleOther.nCntChar == 40 ||
						g_titleOther.nCntChar == 45)
					{// �w��̃^�C�~���O�ɂȂ�����
						g_titleOther.bFlash = true;		// �t���b�V��������
					}

					if (g_titleOther.nCntChar == 53)
					{
						// �L�[���͑������o��
						g_titleOther.bPress = true;
						g_titleOther.bEvent = false;
					}
				}
				else
				{
					g_aTitle[nIndex].move.y -= 0.5f;	// �w��ʒu�܂ŏ㏸
				}

				// �t���b�V������
				if (g_titleOther.bFlash)
				{// �t���b�V���g�p��
					g_titleOther.nCntFlash++;			// �t���b�V���J�E���^�����Z

					if (g_titleOther.nCntFlash > 2)
					{// �w�莞�ԃt���b�V��������
					 // �F�����Ƃɖ߂�
						g_aTitle[nIndex].col = D3DXCOLOR(0.7f, 0.5f, 0.5f, 1.0f);
						// �t���b�V���J�E���^�����Z�b�g
						g_titleOther.nCntFlash = 0;
						// �t���b�V���̏I��
						g_titleOther.bFlash = false;
					}
					else
					{
						// �t���b�V���F�ɂ���
						g_aTitle[nIndex].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}
				}
			}

			if (!g_titleOther.bEvent)
			{
				{// �C�x���g�X�L�b�v��
					g_aTitle[nIndex].pos.y = TITLECHAR_POSY;					// �w��ʒu��
					g_aTitle[nIndex].move.y = 0.0f;								// �ړ��ʂ�0��
					g_aTitle[nIndex].col = D3DXCOLOR(0.7f, 0.5f, 0.5f, 1.0f);	// �F��ݒ�
				}
			}
	}
}

//=============================================================================
// �L�[���͑����̏���
//=============================================================================
void TitlePress(int nIndex)
{
	static float fColAChange = 0.0f;				// �����x�̕ύX
	static bool bFrash = true;						// �_�ł̗L��

	if (g_aTitle[nIndex].type == TITLE_PRESS)
	{// PRESSANYKEY�̏���
		// �_�ŏ���
		if (g_titleOther.bPress)
		{// �g�p��
			if (g_aTitle[nIndex].col.a >= 1.0f)
			{// �����ɂ��Ă���
				fColAChange = -0.008f;
			}
			else if (g_aTitle[nIndex].col.a <= 0.2f)
			{// �s�����ɂ��Ă���
				fColAChange = 0.01f;
			}
		}
		else
		{// �_��off��
			fColAChange = -TITLE_COLASPEED;		// ������
		}

		g_aTitle[nIndex].col.a += fColAChange;	// �����x�̕ω������Z
	}
}

//=============================================================================
// ���o�X�L�b�v����
//=============================================================================
void SkipEvent(int nIndex)
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetAnyTrigger(1, 0))
	{
		if (g_titleOther.bEvent)
		{
			g_titleOther.bEvent = false;					// ���o�I��
			g_titleOther.bPress = true;						// �L�[���͑������o��
			g_titleOther.nPatternAnim = TITLESCARS_ANIM - 1;// �܍��A�j���[�V�������I��
		}
	}
}

//=============================================================================
// �����x�̕��̌Œ�
//=============================================================================
void TitleColAFixed(int nIndex)
{
	// �����x�̂ӂ蕝���Œ�
	if (g_aTitle[nIndex].col.a < 0.0f)
	{// �����x��0�����ɂȂ�����
	 // 0�Ŏ~�߂Ă���
		g_aTitle[nIndex].col.a = 0.0f;
	}

	if (g_aTitle[nIndex].col.a > 1.0f)
	{// �����x��1.0�𒴉߂�����
	 // 1.0�Ŏ~�߂Ă���
		g_aTitle[nIndex].col.a = 1.0f;
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void TitleMakeVtx(void)
{
	VERTEX_2D*pVtx;								// ���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	if (g_titleOther.pVtxBuff == NULL)
	{
		// �e�N�X�`���̓ǂݍ���
		// �܍�
		D3DXCreateTextureFromFile(pDevice,
			TITLESCARS_TEX,
			&g_titleOther.pTexture[TITLE_SCARS]);
		// �^�C�g���A�C�R��
		D3DXCreateTextureFromFile(pDevice,
			TITLEICON_TEX,
			&g_titleOther.pTexture[TITLE_ICON]);
		// �^�C�g������
		D3DXCreateTextureFromFile(pDevice,
			TITLECHAR_TEX,
			&g_titleOther.pTexture[TITLE_CHAR]);
		// �L�[���͑���
		D3DXCreateTextureFromFile(pDevice,
			TITLEPRESS_TEX,
			&g_titleOther.pTexture[TITLE_PRESS]);

		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * g_titleOther.nNum,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_titleOther.pVtxBuff,
			NULL);

		//���_�o�b�t�@�����b�N���Ē��_�f�[�^�ւ̃|�C���^���擾
		g_titleOther.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���̐ݒ�
		for (int nCntTitle = 0; nCntTitle < g_titleOther.nNum; nCntTitle++, pVtx += 4)
		{
			// ���_���W
			pVtx[0].pos = D3DXVECTOR3(-g_aTitle[nCntTitle].size.x, -g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[1].pos = D3DXVECTOR3(g_aTitle[nCntTitle].size.x, -g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[2].pos = D3DXVECTOR3(-g_aTitle[nCntTitle].size.x, g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;
			pVtx[3].pos = D3DXVECTOR3(g_aTitle[nCntTitle].size.x, g_aTitle[nCntTitle].size.y, 0.0f) + g_aTitle[nCntTitle].pos;

			// ���_�J���[
			pVtx[0].col = g_aTitle[nCntTitle].col;
			pVtx[1].col = g_aTitle[nCntTitle].col;
			pVtx[2].col = g_aTitle[nCntTitle].col;
			pVtx[3].col = g_aTitle[nCntTitle].col;

			// rhw
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// �e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		}

		//���_�o�b�t�@���A�����b�N
		g_titleOther.pVtxBuff->Unlock();
	}
}