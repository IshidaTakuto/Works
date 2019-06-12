//=============================================================================
//
// �X�R�A���� [score.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCORE_DIGITS	(8)		// ����

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScore : public CScene
{
public:
	CScore(int nPriority = 6);
	~CScore();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScore* Create(D3DXVECTOR3 pos);

	static const void AddScore(int nValue);

	static int GetScore(void) { return m_nScore; };

private:
	static CNumber *m_pNumber[SCORE_DIGITS];
	static int m_nScore;		// ���݂̃X�R�A
};

#endif
