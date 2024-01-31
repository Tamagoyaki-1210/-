//=============================================================================
//
// sound.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
//=============================================================================
// インクルードファイル
//=============================================================================
#include "sound.h"

// 曲の場所を設定
const CSound::SOUNDPARAM CSound::m_aParam[SOUND_LABEL_MAX] =
{
	{ "data/sounds/BGM/Title.wav", -1 },			// タイトル画面BGM
	{ "data/sounds/BGM/Game.wav", -1 },				// ゲーム画面BGM1
	{ "data/sounds/BGM/Result.wav", -1 },			// リザルト画面BGM
	{ "data/sounds/BGM/Character.wav", -1 },		// キャラ選択画面BGM
	{ "data/sounds/BGM/Boss.wav", -1 },				// ボスBGM

	{ nullptr, -1 },								// BGM最大値

	{ "data/sounds/SE/Select.wav", 0 },				// 選択
	{ "data/sounds/SE/Enter001.wav", 0 },			// 決定
	{ "data/sounds/SE/Enter002.wav", 0 },			// 最終決定

	{ "data/sounds/SE/game/BossEntry.wav", 0 },		// ボス登場
	{ "data/sounds/SE/game/BreakBill.wav", 0 },		// ビル倒壊
	{ "data/sounds/SE/game/Damage.wav", 0 },		// ダメージ
	{ "data/sounds/SE/game/Bullet_Damage.wav", 0 },	// 弾ダメージ
	{ "data/sounds/SE/game/Explosion.wav", 0 },		// 爆発

	{ "data/sounds/SE/game/Warning.wav", 0 },		// 警告音
	{ "data/sounds/SE/game/Gun_Standby.wav", 0 },	// 銃_構え
	{ "data/sounds/SE/game/Out_of_Bullets.wav", 0 },// 弾切れ

	{ "data/sounds/SE/game/bullet001.wav", 0 },		// 弾発射(AR)
	{ "data/sounds/SE/game/bullet002.wav", 0 },		// 弾発射(MG)
	{ "data/sounds/SE/game/bullet003.wav", 0 },		// 弾発射(SG)
	{ "data/sounds/SE/game/bullet004.wav", 0 },		// 弾発射(SR)

	{ "data/sounds/SE/game/punchHit.wav", 0 },		// 打撃
	{ "data/sounds/SE/game/Slash.wav", 0 },			// 剣振り
	{ "data/sounds/SE/game/Thrust.wav", 0 },		// 刺突

	{ "data/sounds/SE/game/PlayerBoost.wav", 0 },	// プレイヤーブースト
	{ "data/sounds/SE/game/PlayerWalk.wav", 0 },	// プレイヤーの歩き

	

	{ nullptr, 0 },									// SE最大値
};

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CSound::CSound()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CSound::~CSound()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (m_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		//memset(&m_aWfx[nCntSound], 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		if (m_aParam[nCntSound].pFilename != nullptr)
		{
			// サウンドデータファイルの生成
			hFile = CreateFile(m_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
				return HRESULT_FROM_WIN32(GetLastError());
			}
			if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタを先頭に移動
				MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
				return HRESULT_FROM_WIN32(GetLastError());
			}

			// WAVEファイルのチェック
			hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}
			hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}
			if (dwFiletype != 'EVAW')
			{
				MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}

			// フォーマットチェック
			hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}
			hr = ReadChunkData(hFile, &m_aWfx[nCntSound], dwChunkSize, dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}

			// オーディオデータ読み込み
			hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}
			m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
			hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}

			// ファイルをクローズ
			CloseHandle(hFile);
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSound::Uninit()
{
	// リストの全ての要素の終了処理
	while (!m_listSourceVoice.empty())
	{
		// 一時停止
		m_listSourceVoice.back().pSourceVoice->Stop(0);

		// オーディオバッファの削除
		m_listSourceVoice.back().pSourceVoice->FlushSourceBuffers();

		// ソースボイスの破棄
		m_listSourceVoice.back().pSourceVoice->DestroyVoice();
		m_listSourceVoice.back().pSourceVoice = NULL;

		// 一番後ろの配列を除外
		m_listSourceVoice.pop_back();
	}

	// 一時停止
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		// オーディオデータの開放
		free(m_apDataAudio[nCntSound]);
		m_apDataAudio[nCntSound] = NULL;
	}

	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;

	if (m_pXAudio2)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSound::Update()
{
	// 流し終えたソースボイスを終了させる処理
	Destroy();
}

//=============================================================================
// 流し終えたソースボイスを終了させる処理
//=============================================================================
void CSound::Destroy()
{
	// 流し終えたソースボイスを終了させる処理
	for (auto itr = m_listSourceVoice.begin(); itr != m_listSourceVoice.end();)
	{
		SSourceVoice sourceVoice = *itr;

		XAUDIO2_VOICE_STATE xa2state;

		// 状態取得
		sourceVoice.pSourceVoice->GetState(&xa2state);

		// バッファが使用されていない場合
		if (xa2state.BuffersQueued == 0)
		{
			// 一時停止
			sourceVoice.pSourceVoice->Stop(0);

			// オーディオバッファの削除
			sourceVoice.pSourceVoice->FlushSourceBuffers();

			// ソースボイスの破棄
			sourceVoice.pSourceVoice->DestroyVoice();
			sourceVoice.pSourceVoice = NULL;

			// 自身をリストから除外
			itr = m_listSourceVoice.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT CSound::Play(SOUND_LABEL label)
{
	// 空のソースボイスの構造体
	SSourceVoice sourceVoice = {};

	// ソースボイスの生成
	m_pXAudio2->CreateSourceVoice(&sourceVoice.pSourceVoice, &(m_aWfx[label].Format));

	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_aParam[label].nCntLoop;

	// オーディオバッファの登録
	sourceVoice.pSourceVoice->SubmitSourceBuffer(&buffer);

	// 再生
	sourceVoice.pSourceVoice->Start(0);

	// ラベルの種類を設定する
	sourceVoice.label = label;

	// リストの一番後ろに入れる
	m_listSourceVoice.push_back(sourceVoice);

	return S_OK;
}

//=============================================================================
// セグメント停止(選択した曲を全て消す)
//=============================================================================
void CSound::StopLabel(SOUND_LABEL label)
{
	// 流し終えたソースボイスを終了させる処理
	for (auto itr = m_listSourceVoice.begin(); itr != m_listSourceVoice.end();)
	{
		SSourceVoice sourceVoice = *itr;

		// ラベルの種類が一致した場合
		if (sourceVoice.label == label)
		{
			// 一時停止
			sourceVoice.pSourceVoice->Stop(0);

			// オーディオバッファの削除
			sourceVoice.pSourceVoice->FlushSourceBuffers();

			// ソースボイスの破棄
			sourceVoice.pSourceVoice->DestroyVoice();
			sourceVoice.pSourceVoice = NULL;

			// 自身をリストから除外
			itr = m_listSourceVoice.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

//=============================================================================
// セグメント停止(全ての曲)
//=============================================================================
void CSound::StopAll()
{
	// リストの全ての要素の終了処理
	while (!m_listSourceVoice.empty())
	{
		// 一時停止
		m_listSourceVoice.back().pSourceVoice->Stop(0);

		// オーディオバッファの削除
		m_listSourceVoice.back().pSourceVoice->FlushSourceBuffers();

		// ソースボイスの破棄
		m_listSourceVoice.back().pSourceVoice->DestroyVoice();
		m_listSourceVoice.back().pSourceVoice = NULL;

		// 一番後ろの配列を除外
		m_listSourceVoice.pop_back();
	}
}

//=============================================================================
// セグメント停止(全てのBGM)
//=============================================================================
void CSound::StopAllBGM()
{
	// リストの全ての要素の終了処理
	for (auto itr = m_listSourceVoice.begin(); itr != m_listSourceVoice.end();)
	{
		SSourceVoice sourceVoice = *itr;

		// サウンドラベルがBGMより下の場合
		if (sourceVoice.label < SOUND_BGM_MAX)
		{
			XAUDIO2_VOICE_STATE xa2state;

			// 状態取得
			sourceVoice.pSourceVoice->GetState(&xa2state);

			// 一時停止
			sourceVoice.pSourceVoice->Stop(0);

			// オーディオバッファの削除
			sourceVoice.pSourceVoice->FlushSourceBuffers();

			// ソースボイスの破棄
			sourceVoice.pSourceVoice->DestroyVoice();
			sourceVoice.pSourceVoice = NULL;

			// 自身をリストから除外
			itr = m_listSourceVoice.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

//=============================================================================
// 生成処理
//=============================================================================
CSound* CSound::Create(HWND hWnd)
{
	CSound* pSound = new CSound;

	if (FAILED(pSound->Init(hWnd)))
	{
		return nullptr;
	}

	return pSound;
}