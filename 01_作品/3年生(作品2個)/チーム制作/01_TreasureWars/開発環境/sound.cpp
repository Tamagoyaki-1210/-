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
	{ "data/sounds/BGM/Title.wav", -1 },				// タイトル画面BGM
	{ "data/sounds/BGM/Entry.wav", -1 },				// エントリー画面BGM
	{ "data/sounds/BGM/Game1.wav", -1 },				// ゲーム画面BGM1
	{ "data/sounds/BGM/Game2.wav", -1 },				// ゲーム画面BGM2
	{ "data/sounds/BGM/Game3.wav", -1 },				// ゲーム画面BGM3
	{ "data/sounds/BGM/Result.wav", -1 },				// リザルト画面BGM

	{ "data/sounds/SE/Select/Select.wav", 0 },			// 選択
	{ "data/sounds/SE/Decision/Decision.wav", 0 },		// 決定
	{ "data/sounds/SE/Whistle/WhistleStart.wav", 0 },	// 笛_開始
	{ "data/sounds/SE/Whistle/WhistleStop.wav", 0 },	// 笛_停止
	{ "data/sounds/SE/Whistle/WhistleFinish.wav", 0 },	// 笛_終了
	{ "data/sounds/SE/Countdown/Countdown.wav", 0 },	// カウントダウン
	{ "data/sounds/SE/Countdown/GameStart.wav", 0 },	// ゲーム開始
	{ "data/sounds/SE/Mine/Swing.wav", 0 },				// 採掘モーション_振り下ろす
	{ "data/sounds/SE/Mine/Hit.wav", 0 },				// 採掘モーション_叩く
	{ "data/sounds/SE/Mine/Mine.wav", 0 },				// 採掘モーション_採掘
	{ "data/sounds/SE/Get/GemGet.wav", 0 },				// 宝石_獲得
	{ "data/sounds/SE/Sale/Sale.wav", 0 },				// 売却
	{ "data/sounds/SE/Damage/Damage.wav", 0 },			// ダメージ
	{ "data/sounds/SE/Item/Item.wav", 0 },				// アイテム取得
	{ "data/sounds/SE/TV/TVStart.wav", 0 },				// テレビ_開始
	{ "data/sounds/SE/TV/TVFinish.wav", 0 },			// テレビ_終了
	{ "data/sounds/SE/TV/TVDisplay.wav", 0 },			// テレビ_表示
	{ "data/sounds/SE/Warning/Warning.wav", 0 },		// 警報
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
// セグメント停止(全て)
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