//==============================================================================================
//
// ‰Œ”š”­ 		smoke_explosion.h
// tutida ryousei
//
//==============================================================================================
#ifndef _SMOKE_EXPLOSION_H_
#define _SMOKE_EXPLOSION_H_

#include"explosion.h"

class CSmoke_Explosion : public CExplosion
{
	static const int Explosion_Timer = 60;	// ”š”­ŽžŠÔ

public:
	CSmoke_Explosion(const PRIORITY priority);
	~CSmoke_Explosion();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CSmoke_Explosion *Create(D3DXVECTOR3 pos, float radius, int power, const bool side, const CObject::PRIORITY priority);

private:
	int m_nContinuation;
};

#endif // !_SMOKE_EXPLOSION_H_
