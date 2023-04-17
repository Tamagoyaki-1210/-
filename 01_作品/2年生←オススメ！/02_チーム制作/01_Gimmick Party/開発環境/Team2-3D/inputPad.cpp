//=============================================================================
//
// inputPad.cpp
// Author1 : KAJITA HIROMU
// Author2 : TAKANO KEISUKE
//
//=============================================================================
//インクルードファイル
#include "inputPad.h"

XINPUT_STATE CInputPad::m_JoyKeyState[PLAYER_MAX] = {};				//ジョイパットのプレス情報
XINPUT_STATE CInputPad::m_JoyKeyStateTrigger[PLAYER_MAX] = {};		//ジョイパットのトリガー情報
D3DXVECTOR3 CInputPad::m_JoyStickPos[PLAYER_MAX] = {};				//ジョイスティックの傾き
int CInputPad::m_nTime[PLAYER_MAX] = {};							//振動持続時間
WORD CInputPad::m_nStrength[PLAYER_MAX] = {};						//振動の強さ (0 - 65535)

//コンストラクタ
CInputPad::CInputPad()
{
}

//デストラクタ
CInputPad::~CInputPad()
{
}

//ジョイパッドの初期化
HRESULT CInputPad::Init(HINSTANCE hInstance, HWND hWnd, GUID guid)
{

	//XInputのステートを設定（有効にする）
	XInputEnable(true);

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//メモリーのクリア
		memset(&m_JoyKeyState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&m_JoyKeyStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));

		//ジョイパッドの振動制御の０クリア
		ZeroMemory(&m_JoyMoter[nCnt], sizeof(XINPUT_VIBRATION));

		//振動制御用の初期化
		m_nStrength[nCnt] = 0;
		m_nTime[nCnt] = 0;
	}

	return S_OK;
}

//ジョイパッドの終了
void CInputPad::Uninit(void)
{
	//XInputのステートを設定（無効にする）
	XInputEnable(false);
}

//ジョイパッドの更新
void CInputPad::Update(void)
{
	XINPUT_STATE JoyKeyState[PLAYER_MAX];		//ジョイパッド入力情報

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		//ジョイパッドの状態を取得
		if (XInputGetState(nCnt, &JoyKeyState[nCnt]) == ERROR_SUCCESS)
		{
			m_JoyKeyStateTrigger[nCnt].Gamepad.wButtons
				= ~m_JoyKeyState[nCnt].Gamepad.wButtons
				& JoyKeyState[nCnt].Gamepad.wButtons; //トリガー情報を保存
			m_JoyKeyState[nCnt] = JoyKeyState[nCnt];  //プレス処理
		}

		//ジョイパッドの振動
		m_JoyMoter[nCnt].wLeftMotorSpeed = m_nStrength[nCnt];
		m_JoyMoter[nCnt].wRightMotorSpeed = m_nStrength[nCnt];
		XInputSetState(nCnt, &m_JoyMoter[nCnt]);

		if (m_nTime[nCnt] > 0)
		{
			m_nTime[nCnt]--;
		}
		else
		{
			m_nStrength[nCnt] = 0;
			m_nTime[nCnt] = 0;
		}
	}
}

//ジョイパッドのプレス処理
bool CInputPad::GetJoypadPress(JOYKEY Key, int nPlayer)
{
	return (m_JoyKeyState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//ジョイパッドのトリガー処理
bool CInputPad::GetJoypadTrigger(JOYKEY Key, int nPlayer)
{
	return (m_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//ジョイパット（スティックプレス）処理
D3DXVECTOR3 CInputPad::GetJoypadStick(JOYKEY Key, int nPlayer)
{
	switch (Key)
	{
	case JOYKEY_LEFT_STICK:
		m_JoyStickPos[nPlayer] = D3DXVECTOR3(m_JoyKeyState[nPlayer].Gamepad.sThumbLX / 32767.0f, -m_JoyKeyState[nPlayer].Gamepad.sThumbLY / 32767.0f, 0.0f);
		break;
	case JOYKEY_RIGHT_STICK:
		m_JoyStickPos[nPlayer] = D3DXVECTOR3(m_JoyKeyState[nPlayer].Gamepad.sThumbRX / 32767.0f, -m_JoyKeyState[nPlayer].Gamepad.sThumbRY / 32767.0f, 0.0f);
		break;
	}

	return m_JoyStickPos[nPlayer];
}

//ジョイパット（トリガーペダル）処理
int CInputPad::GetJoypadTriggerPedal(JOYKEY Key, int nPlayer)
{
	int nJoypadTriggerPedal = 0;
	switch (Key)
	{
	case JOYKEY_LEFT_TRIGGER:
		nJoypadTriggerPedal = m_JoyKeyState[nPlayer].Gamepad.bLeftTrigger;
		break;
	case JOYKEY_RIGHT_TRIGGER:
		nJoypadTriggerPedal = m_JoyKeyState[nPlayer].Gamepad.bRightTrigger;
		break;
	}

	return nJoypadTriggerPedal;
}

//コントローラーの振動制御
void CInputPad::JoypadVibration(int nTime, WORD nStrength, int nPlayer)
{
	m_nTime[nPlayer] = nTime;			//振動持続時間
	m_nStrength[nPlayer] = nStrength;	//振動の強さ
}