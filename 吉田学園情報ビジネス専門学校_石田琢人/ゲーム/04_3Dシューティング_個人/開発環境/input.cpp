//=============================================================================
//
// ���͏��� [input.cpp]
// Author : Ishida Takuto
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DI_JOY_I_INPUT	(1000)  // �X�e�B�b�N�̕������͎�t���s���͈�
#define REPEAT_TIME		(30)	// ���s�[�g�N���܂ł̃t���[��
#define REPEAT_FRAME     (20)   // ���s�[�g�����N�����n�߂�t���[�����̍��v
#define REPEAT_TRIGGER   (6)    // ���s�[�g�����N������t���[���̊Ԋu

//=========================================
// �ÓI�����o�ϐ�
//=========================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;
int CInputJoypad::m_ConnectPad = 0;
LPDIRECTINPUTDEVICE8 CInputJoypad::m_pDevice[MAX_JOYPAD] = {};

//=========================================
// �R���X�g���N�^
//=========================================
CInput::CInput()
{
}

//=========================================
// �f�X�g���N�^
//=========================================
CInput::~CInput()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInput::Uninit(void)
{
	// DirectInput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//	�L�[�{�[�h
//
//=========================================
// �R���X�g���N�^
//=========================================
CInputKeyboard::CInputKeyboard()
{
	m_pDevice = NULL;

	for (int nCount = 0; nCount < MAX_KEY; nCount++)
	{
		m_aState[nCount] = 0;
		m_aStateTrigger[nCount] = 0;
		m_aStateRelease[nCount] = 0;
		m_aStateRepeat[nCount] = 0;
		m_nCntKey[nCount] = 0;
	}
}

//=========================================
// �f�X�g���N�^
//=========================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();	//�L�[�{�[�h�̃A�N�Z�X�������
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	CInput::Uninit();
}

//=============================================================================
// �L�[�{�[�h�̍X�V����
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[MAX_KEY];	//�L�[�{�[�h�̓��͏��
	int nCntKey;

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{//�L�[�{�[�h�̓��͏��ۑ�
		 //�g���K�[(��񂾂�)
			m_aStateTrigger[nCntKey] = (m_aState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

			//�����[�X(�L�[�𗣂�����)
			m_aStateRelease[nCntKey] = (m_aState[nCntKey] ^ aKeyState[nCntKey]) & m_aState[nCntKey];

			// ���s�[�g
			m_aStateRepeat[nCntKey] = aKeyState[nCntKey];

			//�v���X(�������ςȂ�)		��ԉ��ɂ���
			m_aState[nCntKey] = aKeyState[nCntKey];

			if ((m_aStateRepeat[nCntKey] & 0x80) == false)
			{// �L�[�������ĂȂ��Ƃ����s�[�g�p�̃J�E���^��0�ɂ���
				m_nCntKey[nCntKey] = 0;
			}
		}
	}
	else
	{
		m_pDevice->Acquire();	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//=============================================================================
// �L�[�{�[�h�̓��͏��(���s�[�g���)���擾
//=============================================================================
bool CInputKeyboard::GetRepeat(int nKey)
{
	bool bPush = false;

	if (GetPress(nKey) == true)
	{
		m_nCntKey[nKey]++;
	}

	if ((m_nCntKey[nKey] > 30 && m_nCntKey[nKey] % 3 == 0) || m_nCntKey[nKey] == 1)
	{//30F�ȏ㉟���Ă���
		bPush = true;
	}
	else if (m_nCntKey[nKey] <= 30)
	{
		bPush = false;
	}

	return bPush;
}

//=============================================================================
// �L�[��������Ă��邩�m�F
//=============================================================================
bool CInputKeyboard::GetAnyKey(void)
{
	bool bKey = false;

	for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
	{//�L�[�{�[�h�̓��͏��ۑ�
		//�g���K�[(��񂾂�)
		bKey = (m_aStateTrigger[nCntKey] & 0x80) ? true : false;

		if (bKey == true)
		{// ������Ă���L�[������΃u���C�N����
			break;
		}
	}

	return bKey;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//	�W���C�p�b�h
//
//=========================================
// �R���X�g���N�^
//=========================================
CInputJoypad::CInputJoypad()
{
	for (int nCntPad = 0; nCntPad < MAX_JOYPAD; nCntPad++)
	{
		for (int nCount = 0; nCount < DIJS_BUTTON_MAX; nCount++)
		{
			m_aState[nCntPad][nCount] = 0;
			m_aStateTrigger[nCntPad][nCount] = 0;
			m_aStateRelease[nCntPad][nCount] = 0;
			m_aStateRepeat[nCntPad][nCount] = 0;
			m_aCounter[nCntPad][nCount] = 0;
		}
	}
}

//=========================================
// �f�X�g���N�^
//=========================================
CInputJoypad::~CInputJoypad()
{
}

//=============================================================================
// �W���C�p�b�h�̏�����
//=============================================================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyDeviceProc, NULL, DIEDFL_ATTACHEDONLY)))
	{// �����̃f�o�C�X��񋓂���
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �W���C�p�b�h�̏I������
//=============================================================================
void CInputJoypad::Uninit(void)
{
	// ���̓f�o�C�X(�W���C�p�b�h)�̊J��
	for (int nCntPad = 0; nCntPad < MAX_JOYPAD; nCntPad++)
	{
		if (m_pDevice[nCntPad] != NULL)
		{
			m_pDevice[nCntPad]->Unacquire();   // �W���C�p�b�h�ւ̃A�N�Z�X�������
			m_pDevice[nCntPad]->Release();
			m_pDevice[nCntPad] = NULL;
		}
	}

	CInput::Uninit();
}

//=============================================================================
// �W���C�p�b�h�̍X�V����
//=============================================================================
void CInputJoypad::Update(void)
{
	for (int nCntPad = 0; nCntPad < m_ConnectPad; nCntPad++)
	{// �p�b�h�̐������J��Ԃ�
		DIJOYSTATE dijs[MAX_JOYPAD];    // �W���C�p�b�h�̓��͏����i�[����
		if (m_pDevice != NULL)
		{// �����ł���
		 // ���̓f�o�C�X(�W���C�p�b�h)����f�[�^���擾
			m_pDevice[nCntPad]->Poll();
			if (SUCCEEDED(m_pDevice[nCntPad]->GetDeviceState(sizeof(DIJOYSTATE), &dijs[nCntPad])))
			{
				m_YAxisL[nCntPad] = 0.0f;
				m_XAxisL[nCntPad] = 0.0f;

				m_XAxisL[nCntPad] = (float)dijs[nCntPad].lX;
				m_YAxisL[nCntPad] = (float)dijs[nCntPad].lY;

				m_YAxisR[nCntPad] = 0.0f;
				m_XAxisR[nCntPad] = 0.0f;

				m_XAxisR[nCntPad] = (float)dijs[nCntPad].lRx;
				m_YAxisR[nCntPad] = (float)dijs[nCntPad].lRy;

				//-----------------------------
				// ���A�i���O�X�e�B�b�N
				//-----------------------------
				if (dijs[nCntPad].lY <= -100)
				{// ���A�i���O�X�e�B�b�N����ɓ|���ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_10] = 0x80;
				}
				if (dijs[nCntPad].lY >= 100)
				{// ���A�i���O�X�e�B�b�N�����ɓ|���ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_11] = 0x80;
				}
				if (dijs[nCntPad].lX <= -100)
				{// ���A�i���O�X�e�B�b�N�����ɓ|���ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_13] = 0x80;
				}
				if (dijs[nCntPad].lX >= 100)
				{// ���A�i���O�X�e�B�b�N���E�ɓ|���ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_12] = 0x80;
				}

				//-----------------------------
				// �E�A�i���O�X�e�B�b�N
				//-----------------------------
				if (dijs[nCntPad].lRy <= -100)
				{// �E�A�i���O�X�e�B�b�N����ɓ|���ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_14] = 0x80;
				}
				if (dijs[nCntPad].lRy >= 100)
				{// �E�A�i���O�X�e�B�b�N�����ɓ|���ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_15] = 0x80;
				}
				if (dijs[nCntPad].lRx <= -100)
				{// �E�A�i���O�X�e�B�b�N�����ɓ|���ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_17] = 0x80;
				}
				if (dijs[nCntPad].lRx >= 100)
				{// �E�A�i���O�X�e�B�b�N���E�ɓ|���ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_16] = 0x80;
				}

				//-----------------------------
				// �\���L�[
				//-----------------------------
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 0)
				{// �\���L�[�̏オ�����ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_18] = 0x80;
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 45)
				{// �\���L�[�̉E�オ�����ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_18] = 0x80;
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_20] = 0x80;
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 90)
				{// �\���L�[�̉��������ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_20] = 0x80;
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 135)
				{// �\���L�[�̉E���������ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_19] = 0x80;
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_20] = 0x80;
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 180)
				{// �\���L�[�̉E�������ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_19] = 0x80;
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 225)
				{// �\���L�[�̍����������ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_19] = 0x80;
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_21] = 0x80;
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 270)
				{// �\���L�[�̍��������ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_21] = 0x80;
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}
				if (dijs[nCntPad].rgdwPOV[0] / 100 == 315)
				{// �\���L�[�̍��オ�����ꂽ
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_18] = 0x80;
					dijs[nCntPad].rgbButtons[DIJS_BUTTON_21] = 0x80;
					m_Radian[nCntPad] = D3DXToRadian(dijs[nCntPad].rgdwPOV[0]);
				}


				for (int nCntJoyButton = 0; nCntJoyButton < DIJS_BUTTON_MAX; nCntJoyButton++)
				{// �W���C�p�b�h�̃{�^���̐������J��Ԃ�
					m_aStateTrigger[nCntPad][nCntJoyButton] = (m_aState[nCntPad][nCntJoyButton] ^ dijs[nCntPad].rgbButtons[nCntJoyButton]) & dijs[nCntPad].rgbButtons[nCntJoyButton];  // �W���C�p�b�h�̓��͏��(�g���K�[���)�ۑ�
					m_aStateRelease[nCntPad][nCntJoyButton] = (m_aState[nCntPad][nCntJoyButton] ^ dijs[nCntPad].rgbButtons[nCntJoyButton]) & m_aState[nCntPad][nCntJoyButton];      // �W���C�p�b�h�̓��͏��(�����[�X���)�ۑ�
					if (dijs[nCntPad].rgbButtons[nCntJoyButton] == 0x80)
					{// �W���C�p�b�h�����͂���Ă���
						m_aCounter[nCntPad][nCntJoyButton]++;    // �J�E���^�[��i�߂�
						if (m_aCounter[nCntPad][nCntJoyButton] == 1)
						{// �ŏ���1�t���[�������N��
							m_aStateRepeat[nCntPad][nCntJoyButton] = 0x80;
						}
						else if (m_aCounter[nCntPad][nCntJoyButton] == 2)
						{
							m_aStateRepeat[nCntPad][nCntJoyButton] = 0;      // �W���C�p�b�h�̓��͏��(���s�[�g���)���~
						}
						else if (m_aCounter[nCntPad][nCntJoyButton] >= REPEAT_FRAME)
						{// 20�t���[�����L�[��������Ă���
							if (m_aCounter[nCntPad][nCntJoyButton] % REPEAT_TRIGGER == 0)
							{// 3�t���[������
								m_aStateRepeat[nCntPad][nCntJoyButton] = 0x80;   // �W���C�p�b�h�̓��͏��(���s�[�g���)���N��
							}
							else
							{// ����ȊO�̃t���[��
								m_aStateRepeat[nCntPad][nCntJoyButton] = 0;     // �W���C�p�b�h�̓��͏��(���s�[�g���)���ꎞ��~
							}
						}
					}
					else
					{// �W���C�p�b�h�����͂���Ă��Ȃ�
						m_aCounter[nCntPad][nCntJoyButton] = 0;          // �J�E���^�[��߂�
						m_aStateRepeat[nCntPad][nCntJoyButton] = 0;      // �W���C�p�b�h�̓��͏��(���s�[�g���)���~
					}
					m_aState[nCntPad][nCntJoyButton] = dijs[nCntPad].rgbButtons[nCntJoyButton];   // �W���C�p�b�h�̓��͏��(�v���X���)�ۑ�
				}
			}
			else
			{
				m_pDevice[nCntPad]->Acquire(); // �W���C�p�b�h�ւ̃A�N�Z�X�����l��
			}
		}
	}
}

//=============================================================================
// �W���C�p�b�h�̓��͔�����擾
//=============================================================================
bool CInputJoypad::GetAnyTrigger(int nIdxPad)
{
	for (int nCount = 0; nCount < DIJS_BUTTON_MAX; nCount++)
	{// �S���̃{�^���𒲂ׂ�
		if (m_aStateTrigger[nIdxPad][nCount] == 0x80)
		{// ���͂��ꂽ
			return true;
		}
	}

	// ���͂��ꂽ�{�^�����Ȃ�����
	return false;
}

//=============================================================================
// �W���C�p�b�h�̏\���L�[�̃��W�A�����擾
//=============================================================================
float CInputJoypad::GetRadian(int nIdxPad)
{
	return m_Radian[nIdxPad] / 100;
}

//=============================================================================
// �W���C�p�b�h�̍��A�i���O�X�e�B�b�N�̎����擾
//=============================================================================
float CInputJoypad::GetLeftStick(int nIdxPad)
{
	// ���̊p�x�����߂�
	float Ziku = atan2f(m_XAxisL[nIdxPad], m_YAxisL[nIdxPad]);

	return Ziku;
}

//=============================================================================
// �W���C�p�b�h�̉E�A�i���O�X�e�B�b�N�̎����擾
//=============================================================================
float CInputJoypad::GetRightStick(int nIdxPad)
{
	// ���̊p�x�����߂�
	float Ziku = atan2f(m_XAxisR[nIdxPad], m_YAxisR[nIdxPad]);

	return Ziku;
}

//=============================================================================
// �񋓂��ꂽ�W���C�p�b�h�̕��f�o�C�X�𐶐�����
//=============================================================================
BOOL PASCAL CInputJoypad::EnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	// ���̓f�o�C�X(�W���C�p�b�h)�̐���
	if (FAILED(m_pInput->CreateDevice(lpddi->guidInstance, &m_pDevice[m_ConnectPad], NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice[m_ConnectPad]->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	//if (FAILED(m_pDevice[m_ConnectPad]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	//{
	//	return E_FAIL;
	//}

	if (m_pDevice[m_ConnectPad] != NULL)
	{// �����ł���
		DIPROPRANGE diprg;

		// ���̒l�͈̔͂�ݒ�
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -DI_JOY_I_INPUT;
		diprg.lMax = DI_JOY_I_INPUT;

		// ���̐ݒ�
		// ���A�i���O�X�e�B�b�N
		diprg.diph.dwObj = DIJOFS_X;
		m_pDevice[m_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Y;
		m_pDevice[m_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Z;
		m_pDevice[m_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �E�A�i���O�X�e�B�b�N
		diprg.diph.dwObj = DIJOFS_RX;
		m_pDevice[m_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RY;
		m_pDevice[m_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RZ;
		m_pDevice[m_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ?
		//diprg.diph.dwObj = DIJOFS_SLIDER(0);
		//m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �\���L�[(���0�x�Ƃ����v���Ɋp�x * 100)
		diprg.diph.dwObj = DIJOFS_POV(0);
		m_pDevice[m_ConnectPad]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �W���C�p�b�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
		m_pDevice[m_ConnectPad]->Acquire();

		m_ConnectPad++;    // �ڑ�����
	}

	// �����񋓂���
	return DIENUM_CONTINUE;
}