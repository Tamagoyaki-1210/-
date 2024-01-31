//==============================================================================================
//
// レンダリング処理　　　renderer.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _RENDERER_H_
#define	_RENDERER_H_

#include <d3dx9.h>
#include <tchar.h> // _T
#include <assert.h>

class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();

	LPDIRECT3DDEVICE9 GetDevice() { return m_pD3DDevice; }

#ifdef _DEBUG
	int GetFPS();
	static LPD3DXFONT GetFont() { return m_pFont; }
#endif

private:
	const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE);	// 頂点フォーマット
	const int SCREEN_WIDTH = 1280;		// スクリーンの幅
	const int SCREEN_HEIGHT = 720;		// スクリーンの高さ
	LPDIRECT3D9 m_pD3D;					// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Deviceオブジェクト

#ifdef _DEBUG
	static LPD3DXFONT m_pFont;			// フォント
	void DrawFPS();						//FPSの描画
#endif
};

#endif // !_RENDERER_H_
