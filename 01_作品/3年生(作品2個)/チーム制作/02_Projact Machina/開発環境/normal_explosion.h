//==============================================================================================
//
// í èÌîöî≠ 		normal_explosion.h
// tutida ryousei
//
//==============================================================================================
#ifndef _NORMAL_EXPLOSION_H_
#define _NORMAL_EXPLOSION_H_

#include"explosion.h"

class CNormal_Explosion : public CExplosion
{
public:
	CNormal_Explosion(const PRIORITY priority);
	~CNormal_Explosion();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CNormal_Explosion *Create(D3DXVECTOR3 pos, float radius, int power, const bool side, const CObject::PRIORITY priority);

private:
	int m_nContinuation;
};

#endif // !_NORMAL_EXPLOSION_H_
