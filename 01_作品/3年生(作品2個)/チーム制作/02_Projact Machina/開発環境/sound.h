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
		SOUND_LABEL_BGM_GAME,				// ゲーム画面BGM1
		SOUND_LABEL_BGM_RESULT,				// リザルト画面BGM
		SOUND_LABEL_BGM_CHARACTER,			// キャラ選択画面BGM
		SOUND_LABEL_BGM_BOSS,				// ボスBGM

		SOUND_BGM_MAX,						// BGM最大値

		SOUND_LABEL_SE_SELECT,				// 選択
		SOUND_LABEL_SE_YES,					// 決定
		SOUND_LABEL_SE_ENTER,				// 最終決定

		SOUND_LABEL_SE_BOSSENTRY,			// ボス登場
		SOUND_LABEL_SE_BREAKBILL,			// ビル倒壊
		SOUND_LABEL_SE_DAMAGE,				// ダメージ
		SOUND_LABEL_SE_BULLET_DAMAGE,		// 弾ダメージ
		SOUND_LABEL_SE_EXPLOSION,			// 爆発

		SOUND_LABEL_SE_WARNING,				// 警告音
		SOUND_LABEL_SE_GUN_STANDBY,			// 銃_構え
		SOUND_LABEL_SE_OUT_OF_BULLET,		// 弾切れ

		SOUND_LABEL_SE_BULLET_AR,			// 弾発射(AR)
		SOUND_LABEL_SE_BULLET_MG,			// 弾発射(MG)
		SOUND_LABEL_SE_BULLET_SG,			// 弾発射(SG)
		SOUND_LABEL_SE_BULLET_SR,			// 弾発射(SR)

		SOUND_LABEL_SE_PUNCH,				// 打撃
		SOUND_LABEL_SE_SLASH,				// 剣振り
		SOUND_LABEL_SE_THRUST,				// 刺突

		SOUND_LABEL_SE_PLAYERBOOST,			// ブースト
		SOUND_LABEL_SE_PLAYERWALK,			// 歩き

		SOUND_SE_MAX,						// SE最大値

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
	void StopAllBGM();					// 全てのBGMの停止処理

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