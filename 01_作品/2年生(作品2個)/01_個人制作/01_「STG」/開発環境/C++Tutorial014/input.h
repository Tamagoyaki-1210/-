//=============================================================================
//
// 入力処理
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//***********************************************
//ライブラリーファイルのリンク
//***********************************************
#pragma comment(lib,"dxguid.lib")	//固有ID（GUID）が登録されているライブラリ
#pragma comment(lib,"dinput8.lib")	//入力処理に使用(キーボード)
#pragma comment(lib,"xinput.lib")	//入力処理に使用(ジョイパッド)

//---------------------------
//マクロ変数
//---------------------------
#define NUM_KEY_MAX		(256)	//キーの最大数(キーボード)
#define MOUSE_KEY_MAX	(8)		//キーの最大数(マウス)

//---------------------------
//インクルードファイル
//---------------------------
#include "main.h"
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>

class CInput
{
public:
	CInput();
	~CInput();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	//キーボード
	bool GetKeyboardPress(void);
	bool GetKeyboardPress(int nKey);
	bool GetKeyboardTrigger(void);
	bool GetKeyboardTrigger(int nKey);

private:
	//キーボード
	HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
	void UninitKeyboard(void);
	void UpdateKeyboard(void);
	
	static LPDIRECTINPUTDEVICE8 m_pDevKeyboard;		//入力デバイス(キーボード)へのポインタ
	static LPDIRECTINPUT8 m_pInput;
	static BYTE m_aKeyState[NUM_KEY_MAX];			//キーボードのプレス情報
	static BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	//キーボードのトリガー情報
	static BYTE m_aKeyStateRelease[NUM_KEY_MAX];	//キーボードのリリース情報
};

#endif
