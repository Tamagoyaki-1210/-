//==============================================================================================
//
// �����_�����O�����@�@�@renderer.h
// Author : tutida ryousei
// Author : tanimoto kosuke
// Autor : Saito Shian
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

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();

	const LPDIRECT3DDEVICE9 GetDevice() { return m_pD3DDevice; }

#ifdef _DEBUG
	int GetFPS();
	static LPD3DXFONT GetFont() { return m_pFont; }
#endif

private:
	LPDIRECT3D9 m_pD3D;					// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Device�I�u�W�F�N�g

#ifdef _DEBUG
	static LPD3DXFONT m_pFont;			// �t�H���g
	void DrawFPS();						//FPS�̕`��
#endif
};

#endif // !_RENDERER_H_
