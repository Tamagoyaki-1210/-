//=============================================================================
//
// �����_���[
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"

//�����_���[�N���X
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

	LPDIRECT3D9 m_pD3D;	// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice;	// Device�I�u�W�F�N�g
	LPD3DXFONT m_pFont;
};

#endif // !1
