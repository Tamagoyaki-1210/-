//==============================================================================================
//
// ボスの体力ゲージ	　　boss_life_gauge.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _BOSS_LIFE_GAUGE_H_
#define _BOSS_LIFE_GAUGE_H_

class CObject2D;

#include"gauge_manager.h"


class CBoss_Life_Gauge : public CGauge_Manager
{
public:
	CBoss_Life_Gauge();
	~CBoss_Life_Gauge();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	void Fluctuation() override;

	static CBoss_Life_Gauge *Create(const D3DXVECTOR3 &pos, D3DXVECTOR2 size);

	void SetDrawGauge(bool draw) override;

	CObject2D *GetBackGauge() { return m_BackGauge; }

private:
	int m_nBoss_Base_Life;	// ボスの元の体力

	CObject2D *m_BackGauge;		// ゲージの背景
	CObject2D *m_FrontGauge;	// ゲージの前景
};

#endif // !_BOSS_LIFE_GAUGE_H_

