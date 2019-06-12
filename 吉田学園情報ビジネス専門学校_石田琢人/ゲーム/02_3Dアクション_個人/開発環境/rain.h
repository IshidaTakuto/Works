//=============================================================================
//
// �J���� [rain.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _RAIN_H_
#define _RAIN_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	RAINTYPE_TITLE = 0,
	RAINTYPE_GAME,
	RAINTYPE_MAX
}RAINTYPE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	bool bUse;
	RAINTYPE type;
	D3DXMATRIX mtxWorld;
}RAIN;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitRain(void);
void UninitRain(void);
void UpdateRain(void);
void DrawRain(void);

#endif