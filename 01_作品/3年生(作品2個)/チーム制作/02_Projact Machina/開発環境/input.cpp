//=============================================================================
//
// 入力処理 [input.cpp]
// Author1 : KOZUNA HIROHITO
//
//=============================================================================

//-----------------------------------------------------------------------------
//インクルードファイル
//-----------------------------------------------------------------------------
#include "input.h"
#include "DirectInput.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "inputmouse.h"
#include <assert.h>

//-----------------------------------------------------------------------------
//静的メンバ変数宣言
//-----------------------------------------------------------------------------
CInput *CInput::m_pInput = nullptr;//このクラスの情報

//*************************************************************************************
//コンストラクタ
//*************************************************************************************
CInput::CInput()
{
	m_pKeyboard = nullptr;		//キーボードの情報
	m_pJoyPad = nullptr;		//ジョイパッドの情報
	m_pMouse = nullptr;			//マウスの情報
	m_nOldInputType = INPUT_TYPE_KEYBOARD;
	m_nKeyLock = 0; //キーの初期定設（キーなし）
}

//*************************************************************************************
//デストラクタ
//*************************************************************************************
CInput::~CInput()
{
}

//*************************************************************************************
//初期化
//*************************************************************************************
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(CDirectInput::Create(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//キーボードの生成
	m_pKeyboard = new CInputKeyboard;

	//キーボードの初期化処理
	if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//ジョイパッドの生成
	m_pJoyPad = new CInputJoyPad;

	//ジョイパッドの初期化処理
	if (FAILED(m_pJoyPad->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//マウスの生成
	m_pMouse = new CInputMouse;

	//マウスの初期化処理
	if (FAILED(m_pMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//*************************************************************************************
//終了処理
//*************************************************************************************
void CInput::Uninit()
{
	//キーボードの破棄
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	//ジョイパッドの破棄
	if (m_pJoyPad != nullptr)
	{
		m_pJoyPad->Uninit();
		delete m_pJoyPad;
		m_pJoyPad = nullptr;
	}

	//マウスの破棄
	if (m_pMouse != nullptr)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = nullptr;
	}

	//DirectInputオブジェクトの破棄
	CDirectInput::Break();

	//自己破棄
	if (m_pInput != nullptr)
	{
		delete m_pInput;
		m_pInput = nullptr;
	}
}

//*************************************************************************************
//更新処理
//*************************************************************************************
void CInput::Update()
{
	//キーボードの更新
	m_pKeyboard->Update();
	//ジョイパッドの更新
	m_pJoyPad->Update();
	//マウスの更新
	m_pMouse->Update();

	//最後に触ったデバイス
	if (m_pJoyPad->GetPressAll())
	{
		m_nOldInputType = INPUT_TYPE_JOYPAD;
	}
	else if (m_pKeyboard->GetKeyboardAllPress())
	{
		m_nOldInputType = INPUT_TYPE_KEYBOARD;
	}
	else if (m_pMouse->GetPressAll())
	{
		m_nOldInputType = INPUT_TYPE_MOUSE;
	}
}

//*************************************************************************************
//インプットの生成
//*************************************************************************************
CInput *CInput::Create()
{
	//Inputの自己生成
	m_pInput = new CInput;
	return m_pInput;
}

//*************************************************************************************
//本ゲーム専用移動key
//*************************************************************************************
bool CInput::MovePress(BOOK_GAME_DEDICATED_MOVE_KEY key,int nKeyLock)
{
	switch (key)
	{
	case GAME_MOVE_UP:
		return (Press(DIK_W, nKeyLock) || StickPress(JOYKEY_CROSS_UP, nKeyLock));
		break;
	case GAME_MOVE_DOWN:
		return (Press(DIK_S, nKeyLock) || StickPress(JOYKEY_CROSS_DOWN, nKeyLock));
		break;
	case GAME_MOVE_LEFT:
		return (Press(DIK_A, nKeyLock) || StickPress(JOYKEY_CROSS_LEFT, nKeyLock));
		break;
	case GAME_MOVE_RIGHT:
		return (Press(DIK_D, nKeyLock) || StickPress(JOYKEY_CROSS_RIGHT, nKeyLock));
		break;
	case GAME_MOVE_ALL:
		return (MovePress(GAME_MOVE_UP, nKeyLock) || MovePress(GAME_MOVE_DOWN, nKeyLock) || MovePress(GAME_MOVE_LEFT, nKeyLock) || MovePress(GAME_MOVE_RIGHT, nKeyLock));
		break;
	default:
		assert(false);
	}
	return false;
}

//*************************************************************************************
//移動用ベクトルの取得
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMove()
{
	D3DXVECTOR3 KeyboardVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 JoypadVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bool bright = false;
	bool bup = false;

	if (Press(DIK_W))
	{
		KeyboardVec.y = -0.5f;
	}
	if (Press(DIK_S))
	{
		KeyboardVec.y = 0.5f;
	}
	if (Press(DIK_A))
	{
		KeyboardVec.x = -0.5f;
	}
	if (Press(DIK_D))
	{
		KeyboardVec.x = 0.5f;
	}

	//長さ１のベクトル変換
	D3DXVec3Normalize(&KeyboardVec, &KeyboardVec);

	JoypadVec = VectorMoveJoyStick();

	if (JoypadVec.x > KeyboardVec.x || JoypadVec.x < KeyboardVec.x || JoypadVec.y > KeyboardVec.y || JoypadVec.y < KeyboardVec.y)
	{
		return JoypadVec;
	}

	return KeyboardVec;
}

//*************************************************************************************
// 入力したデバイスの番号を取得 (Press)
//*************************************************************************************
std::vector<int> CInput::PressDevice(STAN_DART_INPUT_KEY key)
{
	std::vector<int> inputedDeviceIndex;

	// キーボード入力の調査
	if (Release(key, -1))
	{
		inputedDeviceIndex.push_back(-1);
	}

	// JoyPad入力の調査
	for (int i = 0; i < m_pJoyPad->GetJoyPadNumMax(); i++)
	{
		if (Release(key, i))
		{
			inputedDeviceIndex.push_back(i);
		}
	}

	return inputedDeviceIndex;
}

//*************************************************************************************
// 入力したデバイスの番号を取得 (Trigger)
//*************************************************************************************
std::vector<int> CInput::TriggerDevice(STAN_DART_INPUT_KEY key)
{
	std::vector<int> inputedDeviceIndex;

	// キーボード入力の調査
	if (Trigger(key, -1))
	{
		inputedDeviceIndex.push_back(-1);
	}

	// JoyPad入力の調査
	for (int i = 0; i < m_pJoyPad->GetJoyPadNumMax(); i++)
	{
		if (Trigger(key, i))
		{
			inputedDeviceIndex.push_back(i);
		}
	}

	return inputedDeviceIndex;
}

//*************************************************************************************
// 入力したデバイスの番号を取得 (Release)
//*************************************************************************************
std::vector<int> CInput::ReleaseDevice(STAN_DART_INPUT_KEY key)
{
	std::vector<int> inputedDeviceIndex;

	// キーボード入力の調査
	if (Release(key, -1))
	{
		inputedDeviceIndex.push_back(-1);
	}

	// JoyPad入力の調査
	for (int i = 0; i < m_pJoyPad->GetJoyPadNumMax(); i++)
	{
		if (Release(key, i))
		{
			inputedDeviceIndex.push_back(i);
		}
	}

	return inputedDeviceIndex;
}

//*************************************************************************************
//プレス処理(キーボード)
//*************************************************************************************
bool CInput::Press(int nKey, int nKeyLock, int)
{
	if (m_nKeyLock != nKeyLock)
	{
		return false;
	}
	return m_pKeyboard->GetKeyboardPress(nKey);
}

//*************************************************************************************
//トリガー処理(キーボード)
//*************************************************************************************
bool CInput::Trigger(int nkey, int nKeyLock, int)
{
	if (m_nKeyLock != nKeyLock)
	{
		return false;
	}
	return m_pKeyboard->GetKeyboardTrigger(nkey);
}

//*************************************************************************************
//リリース処理(キーボード)
//*************************************************************************************
bool CInput::Release(int nkey, int nKeyLock, int)
{
	if (m_nKeyLock != nKeyLock)
	{
		return false;
	}
	return m_pKeyboard->GetKeyboardRelease(nkey);
}

//*************************************************************************************
//プレス処理(ジョイパッド)
//*************************************************************************************
bool CInput::Press(DirectJoypad key, int nKeyLock, int nNum)
{
	if (m_nKeyLock != nKeyLock)
	{
		return false;
	}
	return m_pJoyPad->GetPress(key, nNum);
}

//*************************************************************************************
//トリガー処理(ジョイパッド)
//*************************************************************************************
bool CInput::Trigger(DirectJoypad key, int nKeyLock, int nNum)
{
	if (m_nKeyLock != nKeyLock)
	{
		return false;
	}
	return m_pJoyPad->GetTrigger(key, nNum);
}

//*************************************************************************************
//リリース処理(ジョイパッド)
//*************************************************************************************
bool CInput::Release(DirectJoypad key, int nKeyLock, int nNum)
{
	if (m_nKeyLock != nKeyLock)
	{
		return false;
	}
	return m_pJoyPad->GetRelease(key, nNum);
}

//*************************************************************************************
//スティックのプレス
//*************************************************************************************
bool CInput::StickPress(JOYKEY_DIRECT_CROSS key, int nNum, bool bleftandright, float frot, int nKeyLock)
{
	if (m_nKeyLock != nKeyLock)
	{
		return false;
	}
	switch (key)
	{
	case JOYKEY_CROSS_UP:
		return (VectorMoveJoyStick(nNum, bleftandright).y < -frot);
		break;
	case JOYKEY_CROSS_UP_RIGHT:
		return (VectorMoveJoyStick(nNum, bleftandright).y < -frot) && (VectorMoveJoyStick(nNum, bleftandright).x > frot);
		break;
	case JOYKEY_CROSS_RIGHT:
		return (VectorMoveJoyStick(nNum, bleftandright).x > frot);
		break;
	case JOYKEY_CROSS_DOWN_RIGHT:
		return (VectorMoveJoyStick(nNum, bleftandright).y > frot) && (VectorMoveJoyStick(nNum, bleftandright).x > frot);
		break;
	case JOYKEY_CROSS_DOWN:
		return (VectorMoveJoyStick(nNum, bleftandright).y > frot);
		break;
	case JOYKEY_CROSS_DOWN_LEFT:
		return (VectorMoveJoyStick(nNum, bleftandright).y > frot) && (VectorMoveJoyStick(nNum, bleftandright).x < -frot);
		break;
	case JOYKEY_CROSS_LEFT:
		return (VectorMoveJoyStick(nNum, bleftandright).x < -frot);
		break;
	case JOYKEY_CROSS_UP_LEFT:
		return (VectorMoveJoyStick(nNum, bleftandright).y > frot) && (VectorMoveJoyStick(nNum, bleftandright).x < -frot);
		break;
	default:
		assert(false);
	}
	return false;
}

//*************************************************************************************
//プレス処理(マウス)
//*************************************************************************************
bool CInput::Press(MOUSE_KEY Key, int nKeyLock)
{
	if (m_nKeyLock != nKeyLock)
	{
		return false;
	}
	return m_pMouse->GetPress(Key);
}

//*************************************************************************************
//トリガー処理(マウス)
//*************************************************************************************
bool CInput::Trigger(MOUSE_KEY Key, int nKeyLock)
{
	if (m_nKeyLock != nKeyLock)
	{
		return false;
	}
	return m_pMouse->GetTrigger(Key);
}

//*************************************************************************************
//リリース処理(マウス)
//*************************************************************************************
bool CInput::Release(MOUSE_KEY Key, int nKeyLock)
{
	if (m_nKeyLock != nKeyLock)
	{
		return false;
	}
	return m_pMouse->GetRelease(Key);
}

//*************************************************************************************
//十字キーのベクトル
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveKey(int nNum)
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}

	if (m_pJoyPad->GetJoyPadNumMax() <= nNum)
	{
		return VectorMove;
	}

	int nCheck = m_pJoyPad->GetCross(nNum);

	if (nCheck > 360 || nCheck < 0)
	{//十字キーの角度が指定外だったら０を返す
		return VectorMove;
	}

	float fRot = D3DXToRadian(nCheck);

	VectorMove.x = sinf(fRot);
	VectorMove.y = -cosf(fRot);

	//長さ１のベクトル変換
	D3DXVec3Normalize(&VectorMove, &VectorMove);

	return VectorMove;
}

D3DXVECTOR3 CInput::VectorMoveKeyAll()
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nJoyPad = m_pJoyPad->GetJoyPadNumMax();
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}

	for (int nCnt = 0; nCnt < nJoyPad; nCnt++)
	{
		int nCheck = m_pJoyPad->GetCross(nCnt);

		if (nCheck > 360 || nCheck < 0)
		{//十字キーの角度が指定外だったら０を返す
			return VectorMove;
		}

		float fRot = D3DXToRadian(nCheck);

		VectorMove.x = sinf(fRot);
		VectorMove.y = -cosf(fRot);

		//長さ１のベクトル変換
		D3DXVec3Normalize(&VectorMove, &VectorMove);

		if (VectorMove != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			return VectorMove;
		}

	}
	return VectorMove;
}

//*************************************************************************************
//ジョイスティックのベクトル
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveJoyStick(int nNum, bool bleftandright)
{
	D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (m_pJoyPad->GetJoyPadNumMax() == 0)
	{
		return VectorMove;
	}

	VectorMove = m_pJoyPad->GetJoyStickData(nNum, bleftandright);
	//長さ１のベクトル変換
	D3DXVec3Normalize(&VectorMove, &VectorMove);
	return VectorMove;
}

//*************************************************************************************
//ジョイスティックのベクトル
//*************************************************************************************
D3DXVECTOR3 CInput::VectorMoveJoyStickAll(bool bleftandright)
{
	D3DXVECTOR3 VectorNull = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		D3DXVECTOR3 VectorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		VectorMove = m_pJoyPad->GetJoyStickData(nCnt, bleftandright);
		//長さ１のベクトル変換
		D3DXVec3Normalize(&VectorMove, &VectorMove);
		if (VectorNull != VectorMove)
		{
			return  VectorMove;
		}
	}

	return VectorNull;
}

//*************************************************************************************
// マウスカーソルのスクリーン座標の取得
//*************************************************************************************
D3DXVECTOR3 CInput::GetMouseCursor(void)
{
	return m_pMouse->GetMouseCursor();
}

//*************************************************************************************
// マウスのホイールの動き処理
//*************************************************************************************
int CInput::GetMouseWheel(void)
{
	return m_pMouse->GetMouseWheel();
}

//*************************************************************************************
// マウスの移動量を出力処理
//*************************************************************************************
D3DXVECTOR3 CInput::GetMouseMove(void)
{
	return m_pMouse->GetMouseMove();
}

//*************************************************************************************
// 画面内のカーソルを消すかどうか
//*************************************************************************************
void CInput::SetCursorErase(bool bUse)
{
	// 画面内のカーソルを消すかどうか
	m_pMouse->SetCursorErase(bUse);
}

//*************************************************************************************
//キーの入れ替え
//*************************************************************************************
void CInput::SetJoypadKeyConfig(int nPlayerNum, DirectJoypad OldKey, DirectJoypad NewKey)
{
	//変更したキーが変更可能番号以上だったらエラー
	if (OldKey > JOYPAD_HOME || NewKey > JOYPAD_HOME)
	{
		return;
	}
	//ジョイパッドのキー入れ替え関数を呼ぶ
	m_pJoyPad->SetKeyConfig(nPlayerNum, OldKey, NewKey);
}

//*************************************************************************************
//ジョイパッドが接続されているかどうか
//*************************************************************************************
bool CInput::IsConnected(int nNum)
{
	//デバイスデータが存在するかどうか
	if (m_pJoyPad->GetInputDevice(nNum) != nullptr)
	{
		return true;
	}
	return false;
}

//*************************************************************************************
//ジョイパッドの接続数
//*************************************************************************************
int CInput::GetIsConnectedNumMax()
{
	return m_pJoyPad->GetJoyPadNumMax();
}

//*************************************************************************************
//デバイスの途中検知のオンオフ
//*************************************************************************************
void CInput::IntermediateReception(bool bIR)
{
	m_pJoyPad->IntermediateReception(bIR);
}

//*************************************************************************************
//現在デバイスの途中検知を行っているかどうか
//*************************************************************************************
bool CInput::GetIntermediateReception()
{
	return m_pJoyPad->GetIntermediateReception();
}

//*************************************************************************************
//キーの設定(上書き設定はエラーを表示)
//*************************************************************************************
void CInput::SetKeyLock(int nKeyLock)
{
	//キーの上書きエラー
	if (m_nKeyLock != 0)
	{
		assert(false);
	}

	//キーの設定
	m_nKeyLock = nKeyLock;
}

//*************************************************************************************
//キーの解除
//*************************************************************************************
void CInput::UnlockKey(int nKeyLock)
{
	if (m_nKeyLock == nKeyLock)
	{
		m_nKeyLock = 0;
	}
	else
	{
		assert(false);
	}
}

//*************************************************************************************
// 全デバイスの入力を確認
//*************************************************************************************
bool CInput::KeyChackAll(STAN_DART_INPUT_KEY key, int nKeyLock, int type)
{
	auto lambda = [this, nKeyLock,type](auto a)
	{
		if (a == KEY_ALL)
		{
			switch (type)
			{
			case 1:
				return m_pKeyboard->GetKeyboardAllPress() || m_pJoyPad->GetPressAll() || m_pMouse->GetPressAll();
				break;
			case 2:
				return m_pKeyboard->GetKeyboardAllTrigger() || m_pJoyPad->GetTriggerAll() || m_pMouse->GetTriggerAll();
				break;
			case 3:
				return m_pKeyboard->GetKeyboardAllRelease() || m_pJoyPad->GetReleaseAll() || m_pMouse->GetReleaseAll();
				break;
			default:
				break;
			}
		}

		switch (type)
		{
		case 1:
			return Press(a, nKeyLock);
			break;
		case 2:
			return Trigger(a, nKeyLock);
			break;
		case 3:
			return Release(a, nKeyLock);
			break;
		default:
			break;
		}
		return false;
	};

	switch (key)
	{
	case KEY_UP:
		return (lambda(DIK_W) || lambda(DIK_UP) || lambda(JOYPAD_UP));
		break;
	case KEY_UP_LEFT:
		return ((lambda(DIK_W) && lambda(DIK_A)) || (lambda(DIK_UP) && lambda(DIK_LEFT)) || lambda(JOYPAD_UP_LEFT));
		break;
	case KEY_UP_RIGHT:
		return ((lambda(DIK_W) && lambda(DIK_D)) || (lambda(DIK_UP) && lambda(DIK_RIGHT)) || lambda(JOYPAD_UP_RIGHT));
		break;
	case KEY_DOWN:
		return (lambda(DIK_S) || lambda(DIK_DOWN) || lambda(JOYPAD_DOWN));
		break;
	case KEY_DOWN_LEFT:
		return ((lambda(DIK_S) && lambda(DIK_A)) || (lambda(DIK_DOWN) && lambda(DIK_LEFT)) || lambda(JOYPAD_DOWN_LEFT));
		break;
	case KEY_DOWN_RIGHT:
		return (lambda(DIK_S) && lambda(DIK_D)) || (lambda(DIK_DOWN) && lambda(DIK_RIGHT)) || lambda(JOYPAD_DOWN_RIGHT);
		break;
	case KEY_LEFT:
		return lambda(DIK_A) || lambda(DIK_LEFT) || lambda(JOYPAD_LEFT);
		break;
	case KEY_RIGHT:
		return lambda(DIK_D) || lambda(DIK_RIGHT) || lambda(JOYPAD_RIGHT);
		break;
	case KEY_DECISION:
		return lambda(DIK_RETURN) || lambda(JOYPAD_A);
		break;
	case KEY_SHOT:
		return lambda(DIK_SPACE) || lambda(JOYPAD_R1);
		break;
	case KEY_BACK:
		return lambda(DIK_BACKSPACE) || lambda(DIK_B) || lambda(JOYPAD_BACK) || lambda(JOYPAD_B);
		break;
	case KEY_SHIFT:
		return lambda(DIK_RSHIFT) || lambda(DIK_LSHIFT) || lambda(JOYPAD_A);
		break;
	case KEY_MOVE:
		return Trigger(KEY_UP) || Trigger(KEY_DOWN) || Trigger(KEY_LEFT) || Trigger(KEY_RIGHT);
		break;
	case KEY_PAUSE:
		return lambda(DIK_P) || lambda(JOYPAD_START) || lambda(JOYPAD_HOME);
		break;
	case KEY_ALL:
		return lambda(key);
		break;
	default:
		break;
	}

	// 予定されてないキーが呼ばれた
	assert(false);
	return false;
}

//*************************************************************************************
// 指定したデバイスの入力を確認
//*************************************************************************************
bool CInput::KeyChackNum(STAN_DART_INPUT_KEY key, int nKeyLock, int type, int nNum)
{
	auto lambda = [this, nKeyLock, type, nNum](auto a)
	{
		switch (type)
		{
		case 1:
			return Press(a, nKeyLock, nNum);
			break;
		case 2:
			return Trigger(a, nKeyLock, nNum);
			break;
		case 3:
			return Release(a, nKeyLock, nNum);
			break;
		default:
			break;
		}
		return false;
	};

	switch (key)
	{
	case KEY_UP:
		return (lambda(DIK_W) || lambda(DIK_UP) || lambda(JOYPAD_UP));
		break;
	case KEY_UP_LEFT:
		return ((lambda(DIK_W) && lambda(DIK_A)) || (lambda(DIK_UP) && lambda(DIK_LEFT)) || lambda(JOYPAD_UP_LEFT));
		break;
	case KEY_UP_RIGHT:
		return ((lambda(DIK_W) && lambda(DIK_D)) || (lambda(DIK_UP) && lambda(DIK_RIGHT)) || lambda(JOYPAD_UP_RIGHT));
		break;
	case KEY_DOWN:
		return (lambda(DIK_S) || lambda(DIK_DOWN) || lambda(JOYPAD_DOWN));
		break;
	case KEY_DOWN_LEFT:
		return ((lambda(DIK_S) && lambda(DIK_A)) || (lambda(DIK_DOWN) && lambda(DIK_LEFT)) || lambda(JOYPAD_DOWN_LEFT));
		break;
	case KEY_DOWN_RIGHT:
		return (lambda(DIK_S) && lambda(DIK_D)) || (lambda(DIK_DOWN) && lambda(DIK_RIGHT)) || lambda(JOYPAD_DOWN_RIGHT);
		break;
	case KEY_LEFT:
		return lambda(DIK_A) || lambda(DIK_LEFT) || lambda(JOYPAD_LEFT);
		break;
	case KEY_RIGHT:
		return lambda(DIK_D) || lambda(DIK_RIGHT) || lambda(JOYPAD_RIGHT);
		break;
	case KEY_DECISION:
		return lambda(DIK_RETURN) || lambda(JOYPAD_A);
		break;
	case KEY_SHOT:
		return lambda(DIK_SPACE) || lambda(JOYPAD_R1);
		break;
	case KEY_BACK:
		return lambda(DIK_BACKSPACE) || lambda(DIK_B) || lambda(JOYPAD_BACK) || lambda(JOYPAD_B);
		break;
	case KEY_SHIFT:
		return lambda(DIK_RSHIFT) || lambda(DIK_LSHIFT) || lambda(JOYPAD_A);
		break;
	case KEY_MOVE:
		return Trigger(KEY_UP) || Trigger(KEY_DOWN) || Trigger(KEY_LEFT) || Trigger(KEY_RIGHT);
		break;
	case KEY_PAUSE:
		return lambda(DIK_P) || lambda(JOYPAD_START) || lambda(JOYPAD_HOME);
		break;
	case KEY_ALL:
		return lambda(-2);
		break;
	default:
		break;

	}

	// 予定されてないキーが呼ばれた
	assert(false);
	return false;
}
