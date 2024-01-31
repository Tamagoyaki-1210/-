//=============================================================================
//
// 入力処理 [inputjoypad.cpp]
// Author1 : KOZUNA HIROHITO
// input.hを参照を推奨
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "inputjoypad.h"
#include <stdio.h>
#include <assert.h>

//*************************************************************************************
//コンストラクタ
//*************************************************************************************
CInputJoyPad::CInputJoyPad()
{
	ZeroMemory(&m_apDevice, sizeof(m_apDevice));
	ZeroMemory(&m_JoyPadData, sizeof(m_JoyPadData));
	m_AllOldKeyTrigger = JOYPAD_MAX;
	m_AllOldKeyRelease = JOYPAD_MAX;
	m_hWnd = nullptr;
	m_nAfterAppExecutionJoyNumCnt = 0;
	m_nDeviceAdditionalIntervalExecutingApp = 0;
	m_nJoyNumCnt = 0;
}

//*************************************************************************************
//デストラクタ
//*************************************************************************************
CInputJoyPad::~CInputJoyPad()
{
}

//*************************************************************************************
//デバイスを列挙してデバイスを作成してくれるコールバック関数
//*************************************************************************************
BOOL CALLBACK CInputJoyPad::EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	HRESULT hr;

	//Thisポインタの取得
	CInputJoyPad *pThis = (CInputJoyPad*)pContext;

	//デバイスポインタ
	LPDIRECTINPUTDEVICE8 pInputDevice = nullptr;

	//デバイスの取得
	hr = m_pInput->CreateDevice(pdidInstance->guidInstance, &pInputDevice, NULL);

	//上記のエラー検知
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	//入力デバイスへのポインタの設定
	pThis->SetDevicePointer(pInputDevice);

	//次のデバイスを調べるときはDIENUM_CONTINUE最初の一回のみの場合はDIENUM_STOP
	return DIENUM_CONTINUE;
}

//デバイスに対してスティックの範囲等を指定
BOOL CALLBACK CInputJoyPad::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	LPDIRECTINPUTDEVICE8 pInputDevice = (LPDIRECTINPUTDEVICE8)pContext;
	// 入力範囲のセット
	DIPROPRANGE diprg;
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMax = 1000;
	diprg.lMin = -1000;

	// X軸
	diprg.diph.dwObj = DIJOFS_X;
	pInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	// Y軸
	diprg.diph.dwObj = DIJOFS_Y;
	pInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	// Z軸
	diprg.diph.dwObj = DIJOFS_Z;
	pInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	// RX軸
	diprg.diph.dwObj = DIJOFS_RX;
	pInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	// RY軸
	diprg.diph.dwObj = DIJOFS_RY;
	pInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	// RZ軸
	diprg.diph.dwObj = DIJOFS_RZ;
	pInputDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	return DIENUM_CONTINUE;
}

//*************************************************************************************
//初期化
//*************************************************************************************
HRESULT CInputJoyPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//ウィンドウハンドルの保存
	m_hWnd = hWnd;

	//入力デバイスの登録関数
	if (FAILED(JoyPadDeviceRegistration(m_hWnd)))
	{
		return E_FAIL;
	}

	//キーコンフィグの読み込み
	KeyConfigLoading();

	return S_OK;
}

//*************************************************************************************
//終了処理
//*************************************************************************************
void CInputJoyPad::Uninit(void)
{
	//キーコンフィグの保存
	KeyConfigSave();

	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		//入力デバイスの放棄
		if (m_JoyPadData[nCnt].pInputDevice != nullptr)
		{
			m_JoyPadData[nCnt].pInputDevice->Unacquire();
			m_JoyPadData[nCnt].pInputDevice->Release();
			m_JoyPadData[nCnt].pInputDevice = nullptr;
		}
	}
}

//*************************************************************************************
//更新処理
//*************************************************************************************
void CInputJoyPad::Update(void)
{
	//デバイスの途中取得の有無
	if (m_bIntermediateReception)
	{
		//アプリ実行中のデバイス追加間隔
		if (DEVICE_ADDITIONAL_INTERVAL_EXECUTING_APP < m_nDeviceAdditionalIntervalExecutingApp)
		{
			//カウンタの初期化
			m_nDeviceAdditionalIntervalExecutingApp = 0;
			//入力デバイスの登録関数
			if (FAILED(JoyPadDeviceRegistration(m_hWnd)))
			{
				assert(false);
			}
		}
		else
		{
			//カウンタの加算
			m_nDeviceAdditionalIntervalExecutingApp++;
		}
	}

	//デバイスロスト検知用
	bool bLost = false;

	//入力処理の更新
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		if (m_JoyPadData[nCnt].pInputDevice == nullptr)
		{
			continue;
		}
		HRESULT hr;
		//デバイスからデータを取得できることを確認し、確認できなかった場合にはアクセス権が取得
		hr = m_JoyPadData[nCnt].pInputDevice->Poll();
		if (FAILED(hr))
		{
			
			hr = m_JoyPadData[nCnt].pInputDevice->Acquire();

			//デバイスがロストした場合
			if (hr == DIERR_UNPLUGGED)
			{
				bLost = true;
				continue;
			}
		}

		DIJOYSTATE JoyKey;
		//コントローラーの状態を取得
		if (SUCCEEDED(m_JoyPadData[nCnt].pInputDevice->GetDeviceState(sizeof(DIJOYSTATE), &JoyKey)))
		{
			for (int nButtons = 0; nButtons < MAX_JOY_KEY; nButtons++)
			{
				//トリガー情報を保存
				m_JoyPadData[nCnt].aKeyStateTrigger.rgbButtons[nButtons] = ~m_JoyPadData[nCnt].aKeyState.rgbButtons[nButtons] & JoyKey.rgbButtons[nButtons];

				//リリース情報を保存
				m_JoyPadData[nCnt].aKeyStateRelease.rgbButtons[nButtons] = m_JoyPadData[nCnt].aKeyState.rgbButtons[nButtons] & ~JoyKey.rgbButtons[nButtons];


			}
			m_JoyPadData[nCnt].aKeyState = JoyKey;//プレス処理の保管
			m_JoyPadData[nCnt].nCrossPressRot = (int)(m_JoyPadData[nCnt].aKeyState.rgdwPOV[0] / 100.0f);//ジョイパッドの十字キーの押されている方向
		}
		
	}

	//デバイスロストによるデバイスの入れ替え
	if (bLost)
	{
		//入力デバイスの登録関数
		if (FAILED(JoyPadDeviceRegistration(m_hWnd)))
		{
			assert(false);
		}
	}
}

//*************************************************************************************
//入力デバイスの登録関数
//*************************************************************************************
HRESULT CInputJoyPad::JoyPadDeviceRegistration(HWND hWnd)
{
	//登録のためのカウンターの初期化
	m_nAfterAppExecutionJoyNumCnt = 0;

	// デバイスの列挙
	if (FAILED(m_pInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		EnumJoysticksCallback,
		this,
		DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	//データの個数
	m_nJoyNumCnt = 0;

	//データの個数のチェック
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		if (m_JoyPadData[nCnt].pInputDevice != nullptr)
		{
			m_nJoyNumCnt++;
		}
	}

	//前回の登録個数と現在の接続数の比較
	if (m_nJoyNumCnt == m_nAfterAppExecutionJoyNumCnt)
	{
		ZeroMemory(&m_apDevice, sizeof(m_apDevice));
		return S_OK;
	}

	//一時データの初期化
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		//入力デバイスの放棄
		if (m_JoyPadData[nCnt].pInputDevice != nullptr)
		{
			m_JoyPadData[nCnt].pInputDevice->Unacquire();
			m_JoyPadData[nCnt].pInputDevice->Release();
			m_JoyPadData[nCnt].pInputDevice = nullptr;
		}
	}
	m_nJoyNumCnt = 0;
	ZeroMemory(&m_JoyPadData, sizeof(m_JoyPadData));

	//データの挿入と被りのチェック
	for (int nCnt = 0; nCnt < m_nAfterAppExecutionJoyNumCnt; nCnt++)
	{
		//デバイスポインタの挿入
		m_JoyPadData[nCnt].pInputDevice = m_apDevice[nCnt];

		//デバイスのNULLチェック
		if (m_JoyPadData[nCnt].pInputDevice == nullptr)
		{
			continue;
		}

		// デバイスのフォーマットの設定
		HRESULT hr = m_JoyPadData[nCnt].pInputDevice->SetDataFormat(&c_dfDIJoystick);

		if (FAILED(hr))
		{
			return E_FAIL;
		}

		// 協調モードの設定
		if (FAILED(m_JoyPadData[nCnt].pInputDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
		{
			return E_FAIL;
		}

		//デバイスに対して十字キーの範囲等を指定
		if (FAILED(m_JoyPadData[nCnt].pInputDevice->EnumObjects(EnumAxesCallback,
			m_JoyPadData[nCnt].pInputDevice,
			DIDFT_AXIS)))
		{
			return E_FAIL;
		}

		m_nJoyNumCnt++;
	}

	return S_OK;
}

//入力デバイスへのポインタの設定
int CInputJoyPad::SetInputDevice(LPDIRECTINPUTDEVICE8 pInputDeviceint)
{
	//空いている保存先に検索
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		//保存先が空いているかの確認
		if(m_JoyPadData[nCnt].pInputDevice != nullptr)
		{
			continue;
		}

		//保存
		m_JoyPadData[nCnt].pInputDevice = pInputDeviceint;

		//処理の終わり
		return nCnt;

	}
	
	return -1;
}

//プレス処理
bool CInputJoyPad::GetPress(DirectJoypad eKey, int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	if (eKey > JOYPAD_HOME)
	{
		return GetCrossPress(eKey, nNum);
	}
	return (m_JoyPadData[nNum].aKeyState.rgbButtons[m_KeyConfig[nNum][eKey]] & 0x80) ? true : false;
}

//トリガー処理
bool CInputJoyPad::GetTrigger(DirectJoypad eKey, int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	if (eKey > JOYPAD_HOME)
	{
		return GetCrossTrigger(eKey, nNum);
	}
	return (m_JoyPadData[nNum].aKeyStateTrigger.rgbButtons[m_KeyConfig[nNum][eKey]] & 0x80) ? true : false;
}

//リリース処理
bool CInputJoyPad::GetRelease(DirectJoypad eKey, int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	if (eKey > JOYPAD_HOME)
	{
		return GetCrossRelease(eKey, nNum);
	}
	return (m_JoyPadData[nNum].aKeyStateRelease.rgbButtons[m_KeyConfig[nNum][eKey]] & 0x80) ? true : false;
}

//十字キープレス処理
bool CInputJoyPad::GetCrossPress(DirectJoypad eKey, int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	switch (eKey)
	{
	case JOYPAD_UP:
		if (m_JoyPadData[nNum].aKeyState.rgdwPOV[0] == JOYKEY_DIRECT_CROSS::JOYKEY_CROSS_UP
			|| m_JoyPadData[nNum].aKeyState.lY <= -700
			|| m_JoyPadData[nNum].aKeyState.lRz <= -700)
		{
			return true;
		}
		break;
	case JOYPAD_UP_LEFT:
		if (m_JoyPadData[nNum].aKeyState.rgdwPOV[0] == JOYKEY_DIRECT_CROSS::JOYKEY_CROSS_UP_LEFT
			|| (m_JoyPadData[nNum].aKeyState.lY <= -700 && m_JoyPadData[nNum].aKeyState.lX <= -700)
			|| (m_JoyPadData[nNum].aKeyState.lRz <= -700 && m_JoyPadData[nNum].aKeyState.lZ <= -700))
		{
			return true;
		}
		break;
	case JOYPAD_UP_RIGHT:
		if (m_JoyPadData[nNum].aKeyState.rgdwPOV[0] == JOYKEY_DIRECT_CROSS::JOYKEY_CROSS_UP_RIGHT
			|| (m_JoyPadData[nNum].aKeyState.lY <= -700 && m_JoyPadData[nNum].aKeyState.lX >= 700)
			|| (m_JoyPadData[nNum].aKeyState.lRz <= -700 && m_JoyPadData[nNum].aKeyState.lZ >= 700))
		{
			return true;
		}
		break;
	case JOYPAD_DOWN:
		if (m_JoyPadData[nNum].aKeyState.rgdwPOV[0] == JOYKEY_DIRECT_CROSS::JOYKEY_CROSS_DOWN
			|| m_JoyPadData[nNum].aKeyState.lY >= 700
			|| m_JoyPadData[nNum].aKeyState.lRz >= 700)
		{
			return true;
		}
		break;
	case JOYPAD_DOWN_LEFT:
		if (m_JoyPadData[nNum].aKeyState.rgdwPOV[0] == JOYKEY_DIRECT_CROSS::JOYKEY_CROSS_DOWN_LEFT
			|| (m_JoyPadData[nNum].aKeyState.lY >= 700 && m_JoyPadData[nNum].aKeyState.lX <= -700)
			|| (m_JoyPadData[nNum].aKeyState.lRz >= 700 && m_JoyPadData[nNum].aKeyState.lZ <= -700))
		{
			return true;
		}
		break;
	case JOYPAD_DOWN_RIGHT:
		if (m_JoyPadData[nNum].aKeyState.rgdwPOV[0] == JOYKEY_DIRECT_CROSS::JOYKEY_CROSS_DOWN_RIGHT
			|| (m_JoyPadData[nNum].aKeyState.lY >= 700 && m_JoyPadData[nNum].aKeyState.lX >= 700)
			|| (m_JoyPadData[nNum].aKeyState.lRz >= 700 && m_JoyPadData[nNum].aKeyState.lZ >= 700))
		{
			return true;
		}
		break;
	case JOYPAD_LEFT:
		if (m_JoyPadData[nNum].aKeyState.rgdwPOV[0] == JOYKEY_DIRECT_CROSS::JOYKEY_CROSS_LEFT
			|| m_JoyPadData[nNum].aKeyState.lX <= -700
			|| m_JoyPadData[nNum].aKeyState.lZ <= -700)
		{
			return true;
		}
		break;
	case JOYPAD_RIGHT:
		if (m_JoyPadData[nNum].aKeyState.rgdwPOV[0] == JOYKEY_DIRECT_CROSS::JOYKEY_CROSS_RIGHT
			|| m_JoyPadData[nNum].aKeyState.lX >= 700
			|| m_JoyPadData[nNum].aKeyState.lZ >= 700)
		{
			return true;
		}
		break;
	default:
		return false;
		break;
	}
	return false;
}

//十字キートリガー処理
bool CInputJoyPad::GetCrossTrigger(DirectJoypad eKey, int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	if (!m_JoyPadData[nNum].bOldKeyTrigger[eKey - JOYPAD_UP]
		&& GetCrossPress(eKey, nNum))
	{
		m_JoyPadData[nNum].bOldKeyTrigger[eKey - JOYPAD_UP] = true;
		return true;
	}
	else if (m_JoyPadData[nNum].bOldKeyTrigger[eKey - JOYPAD_UP]
		&& GetCrossPress(eKey, nNum))
	{
		return false;
	}

	m_JoyPadData[nNum].bOldKeyTrigger[eKey - JOYPAD_UP] = false;
	return false;
}

//十字キーリリース処理
bool CInputJoyPad::GetCrossRelease(DirectJoypad eKey, int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	if (GetPress(eKey, nNum))
	{
		m_JoyPadData[nNum].bOldKeyRelease[eKey - JOYPAD_UP] = true;
		return false;
	}
	else if(m_JoyPadData[nNum].bOldKeyRelease[eKey - JOYPAD_UP]
		&& !GetPress(eKey, nNum)
		)
	{
		m_JoyPadData[nNum].bOldKeyRelease[eKey - JOYPAD_UP] = false;
		return true;
	}
	
	return false;
}

//オールプレス処理	（キー指定あり、プレイヤー指定なし）
bool CInputJoyPad::GetPressAll(DirectJoypad eKey)
{
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		if (GetPress(eKey, nCnt))
		{
			return true;
		}
	}
	return false;
}

//オールトリガー処理（キー指定あり、プレイヤー指定なし）
bool CInputJoyPad::GetTriggerAll(DirectJoypad eKey)
{
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		if (GetPress(eKey, nCnt) && m_AllOldKeyTrigger == JOYPAD_MAX)
		{
			m_AllOldKeyTrigger = eKey;
			return true;
		}
	}

	if (GetPressAll())
	{//誰かがキーを押していたら
		return false;
	}

	m_AllOldKeyTrigger = JOYPAD_MAX;
	return false;
}

//オールリリース処理（キー指定あり、プレイヤー指定なし）
bool CInputJoyPad::GetReleaseAll(DirectJoypad eKey)
{
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		if (GetPress(eKey, nCnt))
		{
			m_AllOldKeyRelease = eKey;
			return false;
		}

		if (!GetPressAll() 
			&& m_AllOldKeyRelease == eKey
			&& m_AllOldKeyRelease != JOYPAD_MAX)
		{
			m_AllOldKeyRelease = JOYPAD_MAX;
			return true;
		}
	}

	if (GetPressAll())
	{//誰かがキーを押していたら
		return false;
	}

	m_AllOldKeyRelease = JOYPAD_MAX;
	return false;
}

//オールプレス処理	（キー指定なし、プレイヤー指定あり）
bool CInputJoyPad::GetPressAll(int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	for (int nCntKey = 0; nCntKey < JOYPAD_MAX; nCntKey++)
	{
		if (GetPress((DirectJoypad)nCntKey, nNum))
		{
			return true;
		}
	}
	return false;
}

//オールトリガー処理（キー指定なし、プレイヤー指定あり）
bool CInputJoyPad::GetTriggerAll(int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	for (int nCntKey = 0; nCntKey < JOYPAD_MAX; nCntKey++)
	{
		if (GetPress((DirectJoypad)nCntKey, nNum) && m_JoyPadData[nNum].aOldKeyTrigger != nCntKey)
		{
			m_JoyPadData[nNum].aOldKeyTrigger = (DirectJoypad)nCntKey;
			return true;
		}
		else if (GetPress((DirectJoypad)nCntKey, nNum) && m_JoyPadData[nNum].aOldKeyTrigger == nCntKey)
		{
			return false;
		}
	}

	m_JoyPadData[nNum].aOldKeyTrigger = JOYPAD_MAX;
	return false;
}

//オールリリース処理（キー指定なし、プレイヤー指定あり）
bool CInputJoyPad::GetReleaseAll(int nNum)
{
	if (m_JoyPadData[nNum].pInputDevice == nullptr)
	{
		return false;
	}

	for (int nCntKey = 0; nCntKey < JOYPAD_MAX; nCntKey++)
	{
		if (!GetPressAll() && m_JoyPadData[nNum].aOldKeyRelease == nCntKey)
		{
			m_JoyPadData[nNum].aOldKeyRelease = JOYPAD_MAX;
			return true;
		}
		else if (GetPress((DirectJoypad)nCntKey, nNum))
		{
			m_JoyPadData[nNum].aOldKeyRelease = (DirectJoypad)nCntKey;
			return false;
		}
	}

	m_JoyPadData[nNum].aOldKeyRelease = JOYPAD_MAX;
	return false;
}

//オールプレス処理	（キー指定なし、プレイヤー指定なし）
bool CInputJoyPad::GetPressAll()
{
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		if (m_JoyPadData[nCnt].pInputDevice == nullptr)
		{
			return false;
		}

		if (GetPressAll(nCnt))
		{
			return true;
		}
	}
	return false;
}

//オールトリガー処理（キー指定なし、プレイヤー指定なし）
bool CInputJoyPad::GetTriggerAll()
{
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		if (m_JoyPadData[nCnt].pInputDevice == nullptr)
		{
			continue;
		}

		for (int nCntKey = 0; nCntKey < JOYPAD_MAX; nCntKey++)
		{
			if (GetPress((DirectJoypad)nCntKey, nCnt))
			{
				if (m_AllOldKeyTrigger != nCntKey)
				{
					m_AllOldKeyTrigger = (DirectJoypad)nCntKey;
					return true;
				}
				else if (m_AllOldKeyTrigger == nCntKey)
				{
					return false;
				}
			}
		}
	}

	m_AllOldKeyTrigger =JOYPAD_MAX;
	return false;
}

//オールリリース処理（キー指定なし、プレイヤー指定なし）
bool CInputJoyPad::GetReleaseAll()
{
	for (int nCnt = 0; nCnt < JOYPAD_DATA_MAX; nCnt++)
	{
		for (int nKey = 0; nKey < MAX_JOY_KEY; nKey++)
		{
			if (GetPress((DirectJoypad)nKey, nCnt))
			{
				m_AllOldKeyRelease = (DirectJoypad)nKey;
				return false;
			}

			if (!GetPressAll()
				&& m_AllOldKeyRelease == (DirectJoypad)nKey
				&& m_AllOldKeyRelease != JOYPAD_MAX)
			{
				m_AllOldKeyRelease = JOYPAD_MAX;
				return true;
			}
		}
	}

	if (GetPressAll())
	{//誰かがキーを押していたら
		return false;
	}

	m_AllOldKeyRelease = JOYPAD_MAX;
	return false;
}

//ジョイスティックの値を返す
D3DXVECTOR3 CInputJoyPad::GetJoyStickData(int nNum, bool bleftandright)
{
	if (bleftandright)
	{//スティックの右左(true  = 右、false = 左)
		if (m_JoyPadData[nNum].aKeyState.lRz != 0
			|| m_JoyPadData[nNum].aKeyState.lZ != 0)
		{
			return D3DXVECTOR3((float)m_JoyPadData[nNum].aKeyState.lZ, (float)m_JoyPadData[nNum].aKeyState.lRz, 0.0f);
		}

		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}


	if (m_JoyPadData[nNum].aKeyState.lY != 0
		|| m_JoyPadData[nNum].aKeyState.lX != 0)
	{
		return D3DXVECTOR3((float)m_JoyPadData[nNum].aKeyState.lX, (float)m_JoyPadData[nNum].aKeyState.lY, 0.0f);
	}

	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*************************************************************************************
//キーの入れ替え
//*************************************************************************************
void CInputJoyPad::SetKeyConfig(int nPlayerNum, DirectJoypad OldKey, DirectJoypad NewKey)
{
	//変更する前のキーの保存
	DirectJoypad KeySave = m_KeyConfig[nPlayerNum][OldKey];

	//変更対象のキーを変更後のキーに変更
	m_KeyConfig[nPlayerNum][OldKey] = m_KeyConfig[nPlayerNum][NewKey];

	//一時保存した変更前のキーを変更後キーに入れ替え
	m_KeyConfig[nPlayerNum][NewKey] = KeySave;
}

//キーコンフィグの読み込み
void CInputJoyPad::KeyConfigLoading()
{
	FILE *pFile = NULL;			//ファイルポインター宣言

	char cBff[LINE_MAX_READING_LENGTH];		//一行分読み取るための変数
	char cBffHead[LINE_MAX_READING_LENGTH];	//頭の文字を読み取るための変数

	//ファイルを開く
	pFile = fopen("Data\\text\\KeyConfigData.txt", "r");

	if (pFile == nullptr)
	{//開けなかった時用
		assert(false);
	}

	//読み込むコントローラーのカウンター
	int nCntJoypad = 0;

	//文字列の読み取りループ処理
	while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != nullptr)
	{
		//文字列の分析
		sscanf(cBff, "%s", &cBffHead);

		if (strcmp(&cBffHead[0], "ONE_MINUTES_LOADING") == 0)
		{//コントローラー　一個分のキーコンフィグの読み込み

			//読み込んだコントローラーの数が規定数以上だったら
			if (nCntJoypad >= JOYPAD_DATA_MAX)
			{
				assert(false);
			}

			//読み込んだKeyのカウンター
			int nCntKey = 0; 

			//キーコンフィグデータの一時保存
			DirectJoypad KeyConfig[KEY_CONFIG_SUPPORTED_KEY_NUMBER];

			//文字列の読み取りループ処理
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != nullptr)
			{
				//文字列の分析
				sscanf(cBff, "%s", &cBffHead);
				
				if (strcmp(&cBffHead[0], "KEY") == 0)
				{//ボタン一個分のキーコンフィグの読み込み

					//カウンターが規定数以上だったら
					if (nCntKey >= KEY_CONFIG_SUPPORTED_KEY_NUMBER)
					{
						//以下の処理を無視する
						continue;
					}

					//文字列の分析
					sscanf(cBff, "%s = %d", &cBffHead, &KeyConfig[nCntKey]);

					//規定の範囲を超えていた場合はエラー防止のため０を代入する
					if (KeyConfig[nCntKey] >= KEY_CONFIG_SUPPORTED_KEY_NUMBER || KeyConfig[nCntKey] < 0)
					{
						KeyConfig[nCntKey] = (DirectJoypad)0;
					}

					//読み込みカウンターを進める
					nCntKey++;
				}
				else if (strcmp(&cBffHead[0], "ONE_MINUTES_LOADING_END") == 0)
				{//コントローラー　一個分のキーコンフィグの読み込みの終わり

					//読み込んだ情報の保存
					for (int nCnt = 0; nCnt < KEY_CONFIG_SUPPORTED_KEY_NUMBER; nCnt++)
					{
						m_KeyConfig[nCntJoypad][nCnt] = KeyConfig[nCnt];
					}

					//読み込んだコントローラーカウンターを進める
					nCntJoypad++;

					break;
				}

				//保存中の文字列の初期化
				ZeroMemory(&cBff, sizeof(cBff));
				ZeroMemory(&cBffHead, sizeof(cBffHead));
			}
		}
		else if (strcmp(&cBffHead[0], "END_SCRIPT") == 0)
		{//スクリプトの終わり
			break;
		}

		//保存中の文字列の初期化
		ZeroMemory(&cBff, sizeof(cBff));
		ZeroMemory(&cBffHead, sizeof(cBffHead));
	}

	//ファイルを閉じる
	fclose(pFile);
}

//キーコンフィグの保存
void CInputJoyPad::KeyConfigSave()
{
	FILE *pFile = NULL;			//ファイルポインター宣言

	//ファイルを開く
	pFile = fopen("Data\\text\\KeyConfigData.txt", "w");

	if (pFile == nullptr)
	{//開けなかった時用
		assert(false);
	}

	//前文の出力
	fprintf(pFile, "#==============================================================================\n");
	fprintf(pFile, "#\n");
	fprintf(pFile, "# 『KeyConfigData』スクリプトファイル [KeyConfigData.txt]\n");
	fprintf(pFile, "# Author : 小綱啓仁\n");
	fprintf(pFile, "#\n");
	fprintf(pFile, "#==============================================================================\n\n");

	fprintf(pFile, "#------------------------------------------------------------------------------\n");
	fprintf(pFile, "# キーコンフィグの情報(4コントローラー分)\n");
	fprintf(pFile, "#------------------------------------------------------------------------------\n\n");

	//保存するコントローラーのFor文
	for (int nCntJoypad = 0; nCntJoypad < JOYPAD_DATA_MAX; nCntJoypad++)
	{
		//テキストにコントローラーのキーコンフィグ一個分の出力の始まり
		fprintf(pFile, "ONE_MINUTES_LOADING\n\n");

		//保存するKeyのFor文
		for (int nCntKey = 0; nCntKey < KEY_CONFIG_SUPPORTED_KEY_NUMBER; nCntKey++)
		{
			//Key一個分の出力
			fprintf(pFile, "KEY = %d\n",(int)m_KeyConfig[nCntJoypad][nCntKey]);
		}

		//テキストにコントローラーのキーコンフィグ一個分の出力の終わり
		fprintf(pFile, "\nONE_MINUTES_LOADING_END\n\n");
	}

	//終了テキストの出力
	fprintf(pFile, "END_SCRIPT		# この行は絶対消さないこと！\n");

	//ファイルを閉じる
	fclose(pFile);
}
