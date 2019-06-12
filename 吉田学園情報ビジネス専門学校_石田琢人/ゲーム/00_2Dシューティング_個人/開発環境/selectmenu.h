//=============================================================================
//
// �Z���N�g���j���[���� [selectmenu.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _SELECTMENU_H_
#define _SELECTMENU_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	SELECTMENUSTATE_CONTINUE = 0,
	SELECTMENUSTATE_RETRY,
	SELECTMENUSTATE_QUIT,
	SELECTMENUSTATE_MAX
}SELECTMENUSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�F
	SELECTMENUSTATE state;
} SELECTMENU;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitSelectmenu(void);
void UninitSelectmenu(void);
void UpdateSelectmenu(void);
void DrawSelectmenu(void);
void SetSelectmenu(D3DXVECTOR3 pos, D3DXCOLOR col, SELECTMENUSTATE state);

#endif