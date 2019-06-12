//=============================================================================
//
// �c�@���� [stock.h]
// Author : TAKUTO ISHIDA
//
//=============================================================================
#ifndef _STOCK_H_
#define _STOCK_H_

#include "main.h"

#define NUM_STOCK	(3)		// �c�@�̍ő包��

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	int nCntStock;		// �̗�
	int nType;			// ���
	bool bUse;			// �g�p���Ă��邩�ǂ���
}STOCK;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitStock(void);
void UninitStock(void);
void UpdateStock(void);
void DrawStock(void);

void SubtractionStock(int nValue);
STOCK *GetStock(void);

#endif
