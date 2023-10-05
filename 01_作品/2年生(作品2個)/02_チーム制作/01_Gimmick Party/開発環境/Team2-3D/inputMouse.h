//=============================================================================
//
// inputMouse.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef INPUT_MOUSE_H
#define INPUT_MOUSE_H

//インクルードファイル
#include "input.h"

class CInputMouse : public CInput
{
public:
	CInputMouse();																//コンストラクタ
	~CInputMouse();																//デストラクタ
																				
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, GUID guid) override;			//初期化処理
	void Uninit(void) override;													//終了処理
	void Update(void) override;													//更新処理

	DIMOUSESTATE*GetMouseState(void);											//マウス情報の取得処理
	bool GetMouseLeftClick(void);												//マウスの左クリック状態の取得処理
	bool GetMouseRightClick(void);												//マウスの右クリック状態の取得処理

private:

	DIMOUSESTATE m_MouseState;						// マウス状態

};

#endif // !INPUT_MOUSE_H