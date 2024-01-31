//==============================================
//
// ���C�g�̏���[light.h]
// Author�Ftutida ryousei
// Author�Ftanimoto kosuke
//
//==============================================
#ifndef _LIGHT_H_
#define	_LIGHT_H_

#include "renderer.h"

class CLight
{
	static const int MAX_LIGHT = 4;				// ���C�g�̍ő吔
	static const D3DXCOLOR LIGHT_DIFFUSE_COLOR;	// ���C�g�̊����̐F

public:
	CLight();
	~CLight();

	void Init();
	void Uninit();
	void Update();


	const D3DLIGHT9 GetLight() { return m_light[0]; }

private:

	D3DLIGHT9 m_light[MAX_LIGHT];		// ���C�g���
};

#endif // !_LIGHT_H_