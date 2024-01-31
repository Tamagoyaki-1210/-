//==============================================================================================
//
// ”š”­ 		explosion.h
// tutida ryousei
//
//==============================================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include"move_object.h"

class CExplosion : public CMove_Object
{
public:
	CExplosion(const PRIORITY priority);
	~CExplosion();

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Hit(CMove_Object* pHit) override;

	void SetPower(const int power) { m_nPower = power; }
	const int GetPower() override { return m_nPower; }		// ˆÐ—Í‚ÌŽæ“¾

private:
	int m_nPower;
	int m_nContinuation;
};

#endif // !_EXPLOSION_H_
