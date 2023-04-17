//=============================================================================
//
// rendering.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef RENDERING_H
#define RENDERING_H

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// レンダリングクラス
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();									//コンストラクタ
	~CRenderer();									//デストラクタ
	HRESULT Init(HWND hWnd, bool bWindow);			//初期化処理
	void Uninit(void);								//終了処理
	void Update(void);								//更新処理
	void Draw(void);								//描画処理
													
	LPDIRECT3DDEVICE9 GetDevice(void);				//デバイスの取得処理

private:
	void DrawFPS(void);								//FPSの描画処理

private:
	
	LPDIRECT3D9 m_pD3D;								// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;					// Deviceオブジェクト
	LPD3DXFONT m_pFont;								//フォント
};

#endif // !RENDERING_H