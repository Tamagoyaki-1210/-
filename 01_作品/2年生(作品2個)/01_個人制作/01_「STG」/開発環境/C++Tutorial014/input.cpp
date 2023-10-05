//=============================================================================
//
// 入力処理
// Author : tanimoto kosuke
//
//=============================================================================

//---------------------------
//インクルードファイル
//---------------------------
#include "input.h"

LPDIRECTINPUTDEVICE8 CInput::m_pDevKeyboard = nullptr;
LPDIRECTINPUT8 CInput::m_pInput = nullptr;
BYTE CInput::m_aKeyState[NUM_KEY_MAX] = {};
BYTE CInput::m_aKeyStateTrigger[NUM_KEY_MAX] = {};
BYTE CInput::m_aKeyStateRelease[NUM_KEY_MAX] = {};

//=====================================
//デフォルトコンストラクタ
//=====================================
CInput::CInput()
{
}

//=====================================
//デストラクタ
//=====================================
CInput::~CInput()
{

}

//=====================================
//入力処理全ての初期化
//=====================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=====================================
//入力処理全部の終了処理
//=====================================
void CInput::Uninit(void)
{
	//キーボードの終了処理
	UninitKeyboard();
}

//=====================================
//入力処理全部の更新処理
//=====================================
void CInput::Update(void)
{
	//キーボードの更新処理
	UpdateKeyboard();
}

//********************************
//キーボード入力処理
//********************************
//=====================================
//キーボード初期化処理
//=====================================
HRESULT CInput::InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(m_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(m_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	m_pDevKeyboard->Acquire();

	return S_OK;
}

//=====================================
//キーボード終了処理
//=====================================
void CInput::UninitKeyboard(void)
{
	//入力デバイス(キーボード)の放棄
	if (m_pDevKeyboard != NULL)
	{
		m_pDevKeyboard->Unacquire();		//キーボードへのアクセス権を放棄
		m_pDevKeyboard->Release();
		m_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの破棄
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//=====================================
//キーボード更新処理
//=====================================
void CInput::UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];		//キーボードの入力情報

										//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = ~m_aKeyState[nCntKey] & aKeyState[nCntKey];	//キーボードのトリガー情報を保存
			m_aKeyStateRelease[nCntKey] = m_aKeyState[nCntKey] & ~aKeyState[nCntKey];	//キーボードのリリース情報を保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey];									//キーボードのプレス情報を保存
		}
	}
	else
	{
		m_pDevKeyboard->Acquire();		//キーボードへのアクセス権を獲得
	}
}

//=====================================
//キーボードの全キープレス情報を取得
//=====================================
bool CInput::GetKeyboardPress(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (GetKeyboardPress(nCntKey))
		{
			return true;
		}
	}
	return false;
}

//=====================================
//キーボードのプレス情報を取得
//=====================================
bool CInput::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=====================================
//キーボードの全キートリガー情報を取得
//=====================================
bool CInput::GetKeyboardTrigger(void)
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if (GetKeyboardTrigger(nCntKey))
		{
			return true;
		}
	}
	return false;
}

//=====================================
//キーボードのトリガー情報を取得
//=====================================
bool CInput::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}