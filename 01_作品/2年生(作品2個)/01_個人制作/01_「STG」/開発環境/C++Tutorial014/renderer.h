//=============================================================================
//
// レンダラー
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

//レンダラークラス
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();

	LPDIRECT3DDEVICE9 GetDevice();

private:
	void DrawFPS();

	LPDIRECT3D9 m_pD3D;	// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;	// Deviceオブジェクト
	LPD3DXFONT m_pFont;
};

#endif // !1
