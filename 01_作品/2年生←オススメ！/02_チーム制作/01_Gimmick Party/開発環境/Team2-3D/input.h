//=============================================================================
//
// input.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef INPUT_H
#define INPUT_H

//インクルードファイル
#include "main.h"

#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"

//ライブラリリンク
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

//インプットの基本クラス
class CInput
{
public:

	CInput();																//コンストラクタ
	virtual ~CInput();														//デストラクタ
																
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd, GUID guid);		//初期化処理
	virtual void Uninit(void);												//終了処理
	virtual void Update(void) = 0;											//更新処理
																			
	LPDIRECTINPUTDEVICE8 GetInputDevice(void);								//インプットデバイスの取得処理
																
private:														
	static LPDIRECTINPUT8 m_pInput;					//インプットデバイス												
	LPDIRECTINPUTDEVICE8 m_pDevice;					//デバイス	
};

#endif // !INPUT_H