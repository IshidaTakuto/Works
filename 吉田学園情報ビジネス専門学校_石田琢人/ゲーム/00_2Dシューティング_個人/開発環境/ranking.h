//=============================================================================
//
// �����L���O���� [ranking.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	int nType;
}RANKING;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);

void SetRankScore(int nScore);

#endif
