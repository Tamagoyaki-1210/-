//=============================================================================
//
// inputKeyboard.cpp
// Author : Ricci Alex
//
//=============================================================================

//インクルードファイル
#include "inputKeyboard.h"

//静的メンバー変数の宣言
BYTE CInputKeyboard::m_aKeyState[NumMaxKey] = {};
BYTE CInputKeyboard::m_aKeyStateTrigger[NumMaxKey] = {};

//コンストラクタ
CInputKeyboard::CInputKeyboard()
{

}

//デストラクタ
CInputKeyboard::~CInputKeyboard()
{

}

//初期化処理
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd, GUID guid)
{
	//基本クラスの初期化処理
	if (FAILED(CInput::Init(hInstance, hWnd, GUID_SysKeyboard)))
	{
		return -1;
	}

	//デバイスフォーマットを設定
	if (FAILED(CInput::GetInputDevice()->SetDataFormat(&c_dfDIKeyboard)))
	{
		return -1;
	}

	//協調モードを設定
	if (FAILED(CInput::GetInputDevice()->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return -1;
	}

	CInput::GetInputDevice()->Acquire();

	return S_OK;
}

//初期化処理
void CInputKeyboard::Uninit(void)
{
	//基本クラスの終了処理
	CInput::Uninit();
}

//更新処理
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NumMaxKey];		//キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(CInput::GetInputDevice()->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NumMaxKey; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = ((~m_aKeyState[nCntKey]) & aKeyState[nCntKey]);		//キーボードのトリガー情報を保存
			m_aKeyState[nCntKey] = aKeyState[nCntKey];											//キーボードのプレス情報を保存
		}
	}
	else
	{
		CInput::GetInputDevice()->Acquire();
	}
}

//キーボードのプレス
bool CInputKeyboard::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//キーボードのトリガー
bool CInputKeyboard::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}