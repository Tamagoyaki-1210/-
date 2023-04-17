//=============================================================================
//
// sound.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef SOUND_H
#define SOUND_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include "xaudio2.h"					// 音楽処理に必要

class CSound
{
public:

	enum SOUND_LABEL
	{
		SOUND_LABEL_BGM_TITLE = 0,	
		SOUND_LABEL_BGM_SELECT,
		SOUND_LABEL_BGM_GAME01,
		SOUND_LABEL_BGM_GAME02,
		SOUND_LABEL_BGM_GAME03,
		SOUND_LABEL_BGM_RESULT,

		SOUND_LABEL_SE_SELECT,
		SOUND_LABEL_SE_YES,
		SOUND_LABEL_SE_YES_STAGE,
		SOUND_LABEL_SE_NO,
		SOUND_LABEL_SE_WHISTLE_START,
		SOUND_LABEL_SE_WHISTLE_STOP,
		SOUND_LABEL_SE_WHISTLE_FINISH,
		SOUND_LABEL_SE_COIN_GET,
		SOUND_LABEL_SE_WIN,
		SOUND_LABEL_SE_COUNTDOWN,
		SOUND_LABEL_SE_GAMESTART,
		SOUND_LABEL_SE_PISTOL,
		SOUND_LABEL_SE_DAMAGE,
		SOUND_LABEL_SE_DAMAGE_PUNCH,
		SOUND_LABEL_SE_CHEERS01,
		SOUND_LABEL_SE_CHEERS02,
		SOUND_LABEL_SE_JUMP,
		SOUND_LABEL_SE_WARNINGS,
		SOUND_LABEL_SE_PUNCH,
		SOUND_LABEL_MAX
	};

	CSound();								//コンストラクタ
	~CSound();								//デストラクタ

	HRESULT Init(HWND hWnd);				//初期化処理
	void Uninit(void);						//終了処理
	HRESULT Play(SOUND_LABEL label);		//再生処理
	void Stop(SOUND_LABEL label);			//停止処理
	void Stop(void);						//停止処理

	static CSound* Create(HWND hWnd);		//生成処理

private:

	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;


	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);


	static SOUNDPARAM g_aParam[SOUND_LABEL_MAX];

	IXAudio2 *g_pXAudio2 = nullptr;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *g_pMasteringVoice = nullptr;		// マスターボイス
	IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

};

#endif