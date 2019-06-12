//=============================================================================
//
// ���͏��� [input.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_KEY	(256)
#define MAX_JOYPAD	(1)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CManager;
class CRenderer;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CInput
{
public:
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;		// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �L�[�{�[�h
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetAnyKey(void);

	bool GetPress(int nKey) { return(m_aState[nKey] & 0x80) ? true : false; };
	bool GetTrigger(int nKey) { return(m_aStateTrigger[nKey] & 0x80) ? true : false; };
	bool GetRelease(int nIdxPad, int nKey) { return(m_aStateRelease[nKey] & 0x80) ? true : false; };
	bool GetRepeat(int nKey);

private:
	LPDIRECTINPUTDEVICE8 m_pDevice;		// ���̓f�o�C�X�ւ̃|�C���^

	BYTE m_aState[MAX_KEY];				// �L�[�{�[�h�̓��͏��(�v���X���)
	BYTE m_aStateTrigger[MAX_KEY];		// �L�[�{�[�h�̓��͏��(�g���K�[���)
	BYTE m_aStateRelease[MAX_KEY];		// �L�[�{�[�h�̓��͏��(�����[�X���)
	BYTE m_aStateRepeat[MAX_KEY];		// �L�[�{�[�h�̓��͏��(���s�[�g���)

	int m_nCntKey[MAX_KEY];				// ���s�[�g�p�̃J�E���^
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�E�X
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CInputMouse : public CInput
{
public:
	CInputMouse();
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);


private:
	LPDIRECTINPUTDEVICE8 m_pDevice;		// ���̓f�o�C�X�ւ̃|�C���^
	DIMOUSESTATE m_mouseState;			// �}�E�X���
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �W���C�p�b�h
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CInputJoypad : public CInput
{
public:
	typedef enum
	{
		DIJS_BUTTON_0 = 0,  // A�{�^��
		DIJS_BUTTON_1,      // B�{�^��
		DIJS_BUTTON_2,      // X�{�^��
		DIJS_BUTTON_3,      // Y�{�^��
		DIJS_BUTTON_4,      // LB�{�^��
		DIJS_BUTTON_5,      // RB�{�^��
		DIJS_BUTTON_6,      // BACK�{�^��
		DIJS_BUTTON_7,      // START�{�^��
		DIJS_BUTTON_8,      // LT�{�^��
		DIJS_BUTTON_9,      // RT�{�^��
		DIJS_BUTTON_10,     // ���A�i���O�X�e�B�b�N���������
		DIJS_BUTTON_11,     // ���A�i���O�X�e�B�b�N����������
		DIJS_BUTTON_12,     // ���A�i���O�X�e�B�b�N�E��������
		DIJS_BUTTON_13,     // ���A�i���O�X�e�B�b�N����������
		DIJS_BUTTON_14,     // �E�A�i���O�X�e�B�b�N���������
		DIJS_BUTTON_15,     // �E�A�i���O�X�e�B�b�N����������
		DIJS_BUTTON_16,     // �E�A�i���O�X�e�B�b�N�E��������
		DIJS_BUTTON_17,     // �E�A�i���O�X�e�B�b�N����������
		DIJS_BUTTON_18,     // �\���L�[���������
		DIJS_BUTTON_19,     // �\���L�[����������
		DIJS_BUTTON_20,     // �\���L�[�E��������
		DIJS_BUTTON_21,     // �\���L�[����������
							// �ȉ����蓖�ĂȂ�
							DIJS_BUTTON_22,
							DIJS_BUTTON_23,
							DIJS_BUTTON_24,
							DIJS_BUTTON_25,
							DIJS_BUTTON_26,
							DIJS_BUTTON_27,
							DIJS_BUTTON_28,
							DIJS_BUTTON_29,
							DIJS_BUTTON_30,
							DIJS_BUTTON_31,
							DIJS_BUTTON_MAX
	}PADBUTTON;

	CInputJoypad();
	~CInputJoypad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nIdxPad, int nKey) { return(m_aState[nIdxPad][nKey] & 0x80) ? true : false; };
	bool GetTrigger(int nIdxPad, int nKey) { return(m_aStateTrigger[nIdxPad][nKey] & 0x80) ? true : false; };
	bool GetRelease(int nIdxPad, int nKey) { return(m_aStateRelease[nIdxPad][nKey] & 0x80) ? true : false; };
	bool GetRepeat(int nIdxPad, int nKey) { return(m_aStateRepeat[nIdxPad][nKey] & 0x80) ? true : false; };
	bool GetAnyTrigger(int nIdxPad);

	float GetRadian(int nIdxPad);
	float GetLeftStick(int nIdxPad);
	float GetRightStick(int nIdxPad);

private:
	static BOOL PASCAL EnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

	static LPDIRECTINPUTDEVICE8 m_pDevice[MAX_JOYPAD];		// ���̓f�o�C�X(�W���C�p�b�h)�ւ̃|�C���^

	BYTE m_aState[MAX_JOYPAD][DIJS_BUTTON_MAX];			// �W���C�p�b�h�̓��͏��(�v���X���)
	BYTE m_aStateTrigger[MAX_JOYPAD][DIJS_BUTTON_MAX];	// �W���C�p�b�h�̓��͏��(�g���K�[���)
	BYTE m_aStateRelease[MAX_JOYPAD][DIJS_BUTTON_MAX];	// �W���C�p�b�h�̓��͏��(�����[�X���)
	BYTE m_aStateRepeat[MAX_JOYPAD][DIJS_BUTTON_MAX];	// �W���C�p�b�h�̓��͏��(���s�[�g���)
	int m_aCounter[MAX_JOYPAD][DIJS_BUTTON_MAX];		// ���s�[�g�p�̃J�E���^
	static int   m_ConnectPad;							// �ڑ����ꂽ�p�b�h�̐�
	float m_Radian[MAX_JOYPAD];							// �\���L�[�̃��W�A�����i�[����
	float m_XAxisL[MAX_JOYPAD];						    // ���A�i���O�X�e�B�b�N��X�����i�[����
	float m_YAxisL[MAX_JOYPAD];							// ���A�i���O�X�e�B�b�N��Y�����i�[����
	float m_XAxisR[MAX_JOYPAD];						    // �E�A�i���O�X�e�B�b�N��X�����i�[����
	float m_YAxisR[MAX_JOYPAD];							// �E�A�i���O�X�e�B�b�N��Y�����i�[����
};

#endif