//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//ParticleEmitter.h
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
#ifndef _PARTICLE_EMITTER_H_
#define _PARTICLE_EMITTER_H_

#include "particle.h"
#include <vector>

class CParticle;

class CParticleEmitter : public CObject
{
public:
	enum EBehavior
	{
		BEHAVIOR_NONE,
		BEHAVIOR_CIRCLE,
		BEHAVIOR_SPHERE,
		BEHAVIOR_ROTATE,
		BEHAVIOR_MAX
	};

	struct SEmitterInfo
	{
		EBehavior behavior;					// 挙動
		std::string texTag;					// テクスチャのタグ
		float radius;						// 半径
		int popParticleNum;					// 生成する数
		int emitterIndex;					// パーティクルエミッタ用の番号
	};

	CParticleEmitter();
	~CParticleEmitter();

	static CParticleEmitter* Create(std::string str, const D3DXVECTOR3 pos);

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override { ; }
	void CreateParticle(int index);
	void AfterRelease() { m_afterRelease = true; }
	void Destroy() { m_release = true; }

	void SetPos(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPos();
	bool GetDestroy() { return m_release; }

private:
	std::vector<CParticle*> m_particle;
	std::vector<CParticle::SInfo> m_particleInfo;
	std::vector<SEmitterInfo> m_emitterInfo;
	D3DXVECTOR3 m_popPos;
	D3DXVECTOR3 m_move;
	int m_particleIndex;
	int m_time;
	bool m_afterRelease;
	bool m_release;
};

#endif