//==============================================
//
// ライトの処理[light.h]
// Author：tutida ryousei
// Author：tanimoto kosuke
//
//==============================================
#ifndef _LIGHT_H_
#define	_LIGHT_H_

#include "renderer.h"

class CLight
{
	static const int MAX_LIGHT = 4;				// ライトの最大数
	static const D3DXCOLOR LIGHT_DIFFUSE_COLOR;	// ライトの環境光の色

public:
	CLight();
	~CLight();

	void Init();
	void Uninit();
	void Update();


	const D3DLIGHT9 GetLight() { return m_light[0]; }

private:

	D3DLIGHT9 m_light[MAX_LIGHT];		// ライト情報
};

#endif // !_LIGHT_H_