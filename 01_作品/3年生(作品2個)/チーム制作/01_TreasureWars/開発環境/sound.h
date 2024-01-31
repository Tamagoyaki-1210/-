//=============================================================================
//
// sound.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _SOUND_H
#define _SOUND_H

//*****************************************************************************
// インクルード
//*****************************************************************************
//#include <d3dx9.h>
#include "xaudio2.h"	// 音楽処理に必要
#include <vector>

//---------------------------
// クラス宣言
//---------------------------
class CSound
{
public:
	enum SOUND_LABEL
	{
		SOUND_LABEL_BGM_TITLE = 0,			// タイトル画面BGM
		SOUND_LABEL_BGM_ENTRY,				// エントリー画面BGM
		SOUND_LABEL_BGM_GAME1,				// ゲーム画面BGM1
		SOUND_LABEL_BGM_GAME2,				// ゲーム画面BGM2
		SOUND_LABEL_BGM_GAME3,				// ゲーム画面BGM3
		SOUND_LABEL_BGM_RESULT,				// リザルト画面BGM

		SOUND_LABEL_SE_SELECT,				// 選択
		SOUND_LABEL_SE_YES,					// 決定
		SOUND_LABEL_SE_WHISTLE_START,		// 笛_開始
		SOUND_LABEL_SE_WHISTLE_STOP,		// 笛_停止
		SOUND_LABEL_SE_WHISTLE_FINISH,		// 笛_終了
		SOUND_LABEL_SE_COUNTDOWN,			// カウントダウン
		SOUND_LABEL_SE_GAMESTART,			// ゲーム開始
		SOUND_LABEL_SE_MINE_SWING,			// 採掘モーション_振り下ろす
		SOUND_LABEL_SE_MINE_HIT,			// 採掘モーション_叩く
		SOUND_LABEL_SE_MINE_MINE,			// 採掘モーション_採掘
		SOUND_LABEL_SE_GEM_GET,				// 宝石_獲得
		SOUND_LABEL_SE_SALE,				// 売却
		SOUND_LABEL_SE_DAMAGE,				// ダメージ
		SOUND_LABEL_SE_ITEM,				// アイテム取得
		SOUND_LABEL_SE_TV_START,			// テレビ_開始
		SOUND_LABEL_SE_TV_FINISH,			// テレビ_終了
		SOUND_LABEL_SE_TV_DISPLAY,			// テレビ_表示
		SOUND_LABEL_SE_WARNING,				// 警報

		SOUND_LABEL_MAX
	};

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit();
	void Update();
	void Destroy();						// 流し終えたソースボイスを終了させる処理
	HRESULT Play(SOUND_LABEL label);	// 選択した曲の再生処理
	void StopLabel(SOUND_LABEL label);	// 選択した曲の停止処理
	void StopAll();						// 全ての曲の停止処理

	static CSound* Create(HWND hWnd);

private:

	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	struct SSourceVoice
	{
		IXAudio2SourceVoice* pSourceVoice;		// ソースボイスのポインタ
		SOUND_LABEL label;						// ラベルの種類
	};

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	// 各音素材のパラメータ
	static const SOUNDPARAM m_aParam[SOUND_LABEL_MAX];

	IXAudio2* m_pXAudio2 = nullptr;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice = nullptr;		// マスターボイス
	BYTE* m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ
	WAVEFORMATEXTENSIBLE m_aWfx[SOUND_LABEL_MAX] = {};			// .wavフォーマットの情報

	std::vector<SSourceVoice> m_listSourceVoice = {};			// ソースボイスの構造体
};

#endif // !_SOUND_H