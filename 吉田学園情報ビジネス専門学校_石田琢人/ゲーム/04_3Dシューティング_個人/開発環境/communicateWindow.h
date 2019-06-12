//=============================================================================
//
// �ʐM�E�B���h�E���� [communicateWindow.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _COMMUNICATEWINDOW_H_
#define _COMMUNICATEWINDOW_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_POLYGON_COMMUNICATE	(2)		// �|���S���̍ő吔

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPolygon;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCommunicateWindow : public CScene
{
public:
	CCommunicateWindow();
	~CCommunicateWindow();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CCommunicateWindow* Create(void);

	bool GetCompleted(void) { return m_bCompleted; };

private:
	static CPolygon *m_apPolygon[MAX_POLYGON_COMMUNICATE];	// �|���S���N���X�̃|�C���^�ϐ�
	int m_nCntTransparent;									// ����������܂ł̒l
	bool m_bCompleted;										// �ʐM�E�B���h�E�̏������ł�����true
};

#endif
