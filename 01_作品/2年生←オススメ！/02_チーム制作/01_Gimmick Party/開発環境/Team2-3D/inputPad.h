//=============================================================================
//
// inputPad.h
// Author : KAJITA HIROMU
//
//=============================================================================

#ifndef INPUT_PAD_H
#define INPUT_PAD_H

//インクルードファイル
#include "input.h"
#include "XInput.h"

//ライブラリリンク
#pragma comment(lib,"xinput.lib")

//====================================
//マクロ定義
//====================================


class CInputPad : public CInput
{
public:
	//====================================
	//列挙型
	//====================================
	//キー種類(ジョイパッド)
	enum JOYKEY
	{
		JOYKEY_UP = 0,				//十字ボタン上
		JOYKEY_DOWN,				//十字ボタン下
		JOYKEY_LEFT,				//十字ボタン左
		JOYKEY_RIGHT,				//十字ボタン右
		JOYKEY_START,				//スタートボタン
		JOYKEY_BACK,				//バックボタン
		JOYKEY_LEFT_THUMB,			//左スティック押込み
		JOYKEY_RIGHT_THUMB,			//右スティック押込み
		JOYKEY_LEFT_SHOULDER,		//L１ボタン
		JOYKEY_RIGHT_SHOULDER,		//R１ボタン
		JOYKEY_LEFT_TRIGGER,		//L２トリガー
		JOYKEY_RIGHT_TRIGGER,		//R２トリガー
		JOYKEY_A,					//Aボタン
		JOYKEY_B,					//Bボタン
		JOYKEY_X,					//Xボタン
		JOYKEY_Y,					//Yボタン
		JOYKEY_LEFT_STICK,			//左スティック
		JOYKEY_RIGHT_STICK,			//右スティック
		JOYKEY_MAX
	};

	CInputPad();															//コンストラクタ
	~CInputPad();															//デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, GUID guid) override;		//初期化処理
	void Uninit(void) override;												//終了処理
	void Update(void) override;												//更新処理

	static bool GetJoypadPress(JOYKEY Key, int nPlayer);					//ジョイパッドプレス処理
	static bool GetJoypadTrigger(JOYKEY Key, int nPlayer);					//ジョイパッドトリガー処理
	static D3DXVECTOR3 GetJoypadStick(JOYKEY Key, int nPlayer);				//ジョイパッドスティック処理
	static int GetJoypadTriggerPedal(JOYKEY Key, int nPlayer);				//ジョイパッドトリガーペダル処理
	static void JoypadVibration(int nTime, WORD nStrength, int nPlayer);	//ジョイパッド振動制御

private:
	static XINPUT_STATE m_JoyKeyState[PLAYER_MAX];							//ジョイパットのプレス情報
	static XINPUT_STATE m_JoyKeyStateTrigger[PLAYER_MAX];					//ジョイパットのトリガー情報
	static D3DXVECTOR3 m_JoyStickPos[PLAYER_MAX];							//ジョイスティックの傾き
	static int m_nTime[PLAYER_MAX];											//振動持続時間
	static WORD m_nStrength[PLAYER_MAX];									//振動の強さ (0 - 65535)

	XINPUT_VIBRATION m_JoyMoter[PLAYER_MAX];								//ジョイパッドのモーター

};

#endif // !INPUT_MOUSE_H
