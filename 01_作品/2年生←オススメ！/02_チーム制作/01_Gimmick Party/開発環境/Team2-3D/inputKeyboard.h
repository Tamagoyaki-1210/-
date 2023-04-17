//=============================================================================
//
// inputKeyboard.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef INPUT_KEYBOARD_H
#define INPUT_KEYBOARD_H

//インクルードファイル
#include "input.h"

//キーボードのクラス(インプットの派生クラス)
class CInputKeyboard : public CInput
{
public:

	static const int NumMaxKey = 256;										//ボタンの最大数
																			
	CInputKeyboard();														//コンストラクタ
	~CInputKeyboard() override;												//デストラクタ
																
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, GUID guid) override;		//初期化処理
	void Uninit(void) override;												//終了処理
	void Update(void) override;												//更新処理
																			
	static bool GetKeyboardPress(int nKey);									//プレスの判定
	static bool GetKeyboardTrigger(int nKey);								//トリガーの判定
																			
private:																	
	static BYTE m_aKeyState[NumMaxKey];										//キープレス状態
	static BYTE m_aKeyStateTrigger[NumMaxKey];								//キートリガー状態
};

#endif // !INPUT_KEYBOARD_H