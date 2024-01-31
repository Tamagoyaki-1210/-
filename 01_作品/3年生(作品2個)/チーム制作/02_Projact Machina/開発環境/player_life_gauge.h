//==============================================================================================
//
// �v���C���[�̗̑̓Q�[�W	�@�@player_life_gauge.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _PLAYER_LIFE_GAUGE_H_
#define	_PLAYER_LIFE_GAUGE_H_

class CObject2D;

#include"gauge_manager.h"

class CPlayer_Life_Gauge : public CGauge_Manager
{
public:
	CPlayer_Life_Gauge();
	~CPlayer_Life_Gauge();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	void Fluctuation() override;
	void Col();

	static CPlayer_Life_Gauge *Create(const D3DXVECTOR3 &pos, D3DXVECTOR2 size);

	void SetDrawGauge(bool draw) override;

private:
	float m_fLife_Percent;		// �̗͂̊���

	CObject2D *m_FrontGauge;	// �Q�[�W�̑O�i
	CObject2D *m_BackGauge;		// �Q�[�W�̔w�i
};

#endif // !_PLAYER_LIFE_GAUGE_H_
