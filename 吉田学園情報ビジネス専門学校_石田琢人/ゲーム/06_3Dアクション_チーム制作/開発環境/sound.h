//*****************************************************************************
//
//     サウンドの処理[sound.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     サウンドクラスの定義
//*****************************************************************************
class CSound
{
public:    // 誰でもアクセス可能
	//------------------------
	//  サウンドラベル
	//------------------------
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,     // タイトルのBGM
		SOUND_LABEL_BGM001,         // チュートリアルのBGM
		SOUND_LABEL_BGM002,         // ゲームのBGM
		SOUND_LABEL_BGM003,         // リザルトのBGM
		SOUND_LABEL_BGM004,         // ランキングのBGM
		SOUND_LABEL_SE_WALK000,     // 歩く音(1つ目)
		SOUND_LABEL_SE_WALK001,     // 歩く音(2つ目)
		SOUND_LABEL_SE_JUMP,        // ジャンプする音
		SOUND_LABEL_SE_LANDING,     // 着地する音
		SOUND_LABEL_SE_PUSH,        // 押す音
		SOUND_LABEL_SE_PANCH,       // パンチする音
		SOUND_LABEL_SE_KICK,        // キックする音
		SOUND_LABEL_SE_ZIPLINE,     // ジップラインの音
		SOUND_LABEL_SE_CARRYFINISH, // ジップラインで運び終わった後の音
		SOUND_LABEL_SE_BREAK,       // ものを壊す音
		SOUND_LABEL_SE_WATER,       // 水に入った時の音
		SOUND_LABEL_SE_PRESS,       // 決定ボタンが押されたとき
		SOUND_LABEL_SE_SELECT,      // 選択項目が移動したとき
		SOUND_LABEL_SE_PAUSE_OPEN,  // ポーズが開いたときの音
		SOUND_LABEL_SE_FINISH,      // ゲーム終了時の音
		SOUND_LABEL_SE_WARNING,     // 残り時間が無くなった時の音
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	CSound();
	~CSound();
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

private:   // このクラスだけがアクセス可能
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								    // XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice;			    // マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータへのポインタ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

	//------------------------
	//  各音素材のパラメータ
	//------------------------
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =    // 音素材のパラメータ(サウンドラベル分用意)
	{// -1ならBGM  0ならSE
		{ "data/BGM/bgm000.wav",-1 },
		{ "data/BGM/bgm001.wav",-1 },
		{ "data/BGM/bgm002.wav",-1 },
		{ "data/BGM/bgm003.wav",-1 },
		{ "data/BGM/bgm004.wav",-1 },
		{ "data/SE/walk000.wav",0 },
		{ "data/SE/walk001.wav",0 },
		{ "data/SE/jump.wav",0 },
		{ "data/SE/landing.wav",0 },
		{ "data/SE/push.wav",0 },
		{ "data/SE/panch.wav",0 },
		{ "data/SE/kick.wav",0 },
		{ "data/SE/zipline.wav",-1 },
		{ "data/SE/zipline_end.wav",0 },
		{ "data/SE/break.wav",0 },
		{ "data/SE/water.wav",0 },
		{ "data/SE/Gorira.wav",0 },
		{ "data/SE/pauseselect.wav",0 },
		{ "data/SE/pauseopen.wav",0 },
		{ "data/SE/finish.wav",0 },
		{ "data/SE/warning.wav",0 },
	};
};

#endif