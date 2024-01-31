//==============================================================================================
//
// �����_�����O�����@�@�@renderer.h
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
	const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE);	// ���_�t�H�[�}�b�g
	const int SCREEN_WIDTH = 1280;		// �X�N���[���̕�
	const int SCREEN_HEIGHT = 720;		// �X�N���[���̍���
	LPDIRECT3D9 m_pD3D;					// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Device�I�u�W�F�N�g

#ifdef _DEBUG
	static LPD3DXFONT m_pFont;			// �t�H���g
	void DrawFPS();						//FPS�̕`��
#endif
};

#endif // !_RENDERER_H_
