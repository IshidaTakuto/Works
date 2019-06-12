//=============================================================================
//
// 血の処理 [Blood.h]
// Author : KOJI INAI
//
//=============================================================================
#ifndef _BLOOD_H_
#define _BLOOD_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBlood(void);
void UninitBlood(void);
void UpdateBlood(void);
void DrawBlood(void);
void DeleteBlood(void);
int SetBlood(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWide, D3DXCOLOR col);

#endif
