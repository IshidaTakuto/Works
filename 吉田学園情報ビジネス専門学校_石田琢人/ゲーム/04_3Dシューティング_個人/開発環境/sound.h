//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Ishida Takuto
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSound
{
public:
	//*************************************
	// サウンドファイル
	//*************************************
	typedef enum
	{
		SOUND_LABEL_TITLE = 0,		// タイトル
		SOUND_LABEL_TUTORIAL,		// チュートリアル
		SOUND_LABEL_GAME,			// ゲーム
		SOUND_LABEL_CLEAR,			// ゲームクリア
		SOUND_LABEL_OVER,			// ゲームオーバー
		SOUND_LABEL_PAUSE,			// ポーズ音
		SOUND_LABEL_DECIDE,			// 決定音
		SOUND_LABEL_CANCEL,			// キャンセル音
		SOUND_LABEL_BOTTON000,		// カーソル移動音
		SOUND_LABEL_BULLET000,		// 通常弾発射
		SOUND_LABEL_BULLET001,		// 戦闘機の特殊弾発射
		SOUND_LABEL_BULLET003,		// 戦車の特殊弾爆発
		SOUND_LABEL_EXPLOSION,		// 爆発
		SOUND_LABEL_HIT,			// ヒット音
		SOUND_LABEL_HIT1,			// ヒット音1
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
	// パラメータ構造体定義
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータへのポインタ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

																// 各音素材のパラメータ
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/title.wav", -1 },			// タイトル
		{ "data/BGM/tutorial.wav", -1 },		// チュートリアル
		{ "data/BGM/game.wav", -1 },			// ゲーム
		{ "data/BGM/clear.wav", -1 },			// ゲームクリア
		{ "data/BGM/over.wav", -1 },			// ゲームオーバー
		{ "data/SE/pause.wav", 0 },			// ポーズ音
		{ "data/SE/decide.wav", 0 },			// 決定音
		{ "data/SE/cancel.wav", 0 },			// キャンセル音
		{ "data/SE/select.wav", 0 },			// カーソル移動音
		{ "data/SE/shot.wav", 0 },				// 通常弾発射
		{ "data/SE/beam.wav", 0 },				// 戦闘機の特殊弾発射
		{ "data/SE/bomb.wav", 0 },				// 戦車の特殊弾発射
		{ "data/SE/explosion.wav", 0 },			// 爆発
		{ "data/SE/hit000.wav", 0 },			// ヒット音
		{ "data/SE/hit001.wav", 0 },			// ヒット音
	};
};

#endif
