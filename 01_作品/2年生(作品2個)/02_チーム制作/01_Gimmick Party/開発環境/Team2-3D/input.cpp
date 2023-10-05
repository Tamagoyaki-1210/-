//=============================================================================
//
// input.cpp
// Author : Ricci Alex
//
//=============================================================================

//インクルードファイル
#include "input.h"

//静的メンバー変数の宣言
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//コンストラクタ
CInput::CInput()
{
	//メンバー変数をクリアする
	m_pDevice = nullptr;
}

//デストラクタ
CInput::~CInput()
{

}

//初期化処理
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd, GUID guid)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}
	//入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(guid, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//終了処理
void CInput::Uninit(void)
{
	//インプットデバイスの破棄
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}

	//デバイスの破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

//インプットデバイスの取得処理
LPDIRECTINPUTDEVICE8 CInput::GetInputDevice(void)
{
	return m_pDevice;
}