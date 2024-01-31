//==============================================
//
//ライトの処理[light.h]
//Author：tutida ryousei
//
//==============================================
#ifndef _LIGHT_H_
#define	_LIGHT_H_

#include"renderer.h"

class CLight
{
public:
	CLight();
	~CLight();

	void Init();
	void Uninit();
	void Update();


	D3DLIGHT9 GetLight() { return m_light[0]; }

private:
	static const int Max_Light = 8;				// ライトの最大数
	static const D3DXCOLOR Light_Diffuse_Color;	// ライトの環境光の色

	D3DLIGHT9 m_light[Max_Light];		// ライト情報
};

#endif // !_LIGHT_H_