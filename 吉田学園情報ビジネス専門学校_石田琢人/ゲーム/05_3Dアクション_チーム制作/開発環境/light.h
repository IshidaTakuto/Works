//=============================================================================
//
// ���C�g���� [light.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	LIGHTTYPE_NORMAL = 0,
	LIGHTTYPE_TITLE,
	LIGHTTYPE_RESULT,
	LIGHTTYPE_MAX
}LIGHTTYPE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitLight(void);
void UninitLight(void);
void UpdateLight(void);

bool GetLightRain(void);
LIGHTTYPE *GetLightType(void);
#endif
