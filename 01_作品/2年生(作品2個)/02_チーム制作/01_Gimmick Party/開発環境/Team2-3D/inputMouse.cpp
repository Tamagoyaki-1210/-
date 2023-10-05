//=============================================================================
//
// inputMouse.cpp
// Author : Ricci Alex
//
//=============================================================================

//インクルードファイル
#include "inputMouse.h"

//コンストラクタ
CInputMouse::CInputMouse()
{
	
}

//デストラクタ
CInputMouse::~CInputMouse()
{

}

//初期化処理
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd, GUID guid)
{
	//基本クラスの初期化処理
	if (FAILED(CInput::Init(hInstance, hWnd, GUID_SysMouse)))
	{
		return -1;
	}

	//デバイスフォーマットを設定
	if (FAILED(CInput::GetInputDevice()->SetDataFormat(&c_dfDIMouse)))
	{
		return -1;
	}
	//協調モードを設定
	if (FAILED(CInput::GetInputDevice()->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return -1;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	if (FAILED(CInput::GetInputDevice()->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		// デバイスの設定に失敗
		return false;
	}

	CInput::GetInputDevice()->Acquire();

	return S_OK;
}

//終了処理
void CInputMouse::Uninit(void)
{
	//基本クラスの終了処理
	CInput::Uninit();
}

//更新処理
void CInputMouse::Update(void)
{
	LPDIRECTINPUTDEVICE8 pDevice = CInput::GetInputDevice();

	if (SUCCEEDED(pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState)))
	{

	}
	else
	{
		pDevice->Acquire();
	}
}

//マウス状態の取得処理
DIMOUSESTATE* CInputMouse::GetMouseState(void)
{
	return &m_MouseState;
}

//左クリックのトリガー
bool CInputMouse::GetMouseLeftClick(void)
{
	return (GetKeyState(VK_LBUTTON) & 0x8000) ? true : false;
}

//右クリックのトリガー
bool CInputMouse::GetMouseRightClick(void)
{
	return (GetKeyState(VK_RBUTTON) & 0x8000) ? true : false;
}