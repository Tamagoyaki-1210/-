//==============================================
//
//���C�g�̏���[light.h]
//Author�Ftutida ryousei
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
	static const int Max_Light = 8;				// ���C�g�̍ő吔
	static const D3DXCOLOR Light_Diffuse_Color;	// ���C�g�̊����̐F

	D3DLIGHT9 m_light[Max_Light];		// ���C�g���
};

#endif // !_LIGHT_H_