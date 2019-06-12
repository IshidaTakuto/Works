//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSound
{
public:
	//*************************************
	// �T�E���h�t�@�C��
	//*************************************
	typedef enum
	{
		SOUND_LABEL_TITLE = 0,		// �^�C�g��
		SOUND_LABEL_TUTORIAL,		// �`���[�g���A��
		SOUND_LABEL_GAME,			// �Q�[��
		SOUND_LABEL_CLEAR,			// �Q�[���N���A
		SOUND_LABEL_OVER,			// �Q�[���I�[�o�[
		SOUND_LABEL_PAUSE,			// �|�[�Y��
		SOUND_LABEL_DECIDE,			// ���艹
		SOUND_LABEL_CANCEL,			// �L�����Z����
		SOUND_LABEL_BOTTON000,		// �J�[�\���ړ���
		SOUND_LABEL_BULLET000,		// �ʏ�e����
		SOUND_LABEL_BULLET001,		// �퓬�@�̓���e����
		SOUND_LABEL_BULLET003,		// ��Ԃ̓���e����
		SOUND_LABEL_EXPLOSION,		// ����
		SOUND_LABEL_HIT,			// �q�b�g��
		SOUND_LABEL_HIT1,			// �q�b�g��1
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	CSound();
	~CSound();
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

private:
	//*****************************************************************************
	// �p�����[�^�\���̒�`
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g(-1�Ń��[�v�Đ�)
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X�ւ̃|�C���^
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X�ւ̃|�C���^
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�ւ̃|�C���^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y

																// �e���f�ނ̃p�����[�^
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/title.wav", -1 },			// �^�C�g��
		{ "data/BGM/tutorial.wav", -1 },		// �`���[�g���A��
		{ "data/BGM/game.wav", -1 },			// �Q�[��
		{ "data/BGM/clear.wav", -1 },			// �Q�[���N���A
		{ "data/BGM/over.wav", -1 },			// �Q�[���I�[�o�[
		{ "data/SE/pause.wav", 0 },			// �|�[�Y��
		{ "data/SE/decide.wav", 0 },			// ���艹
		{ "data/SE/cancel.wav", 0 },			// �L�����Z����
		{ "data/SE/select.wav", 0 },			// �J�[�\���ړ���
		{ "data/SE/shot.wav", 0 },				// �ʏ�e����
		{ "data/SE/beam.wav", 0 },				// �퓬�@�̓���e����
		{ "data/SE/bomb.wav", 0 },				// ��Ԃ̓���e����
		{ "data/SE/explosion.wav", 0 },			// ����
		{ "data/SE/hit000.wav", 0 },			// �q�b�g��
		{ "data/SE/hit001.wav", 0 },			// �q�b�g��
	};
};

#endif
