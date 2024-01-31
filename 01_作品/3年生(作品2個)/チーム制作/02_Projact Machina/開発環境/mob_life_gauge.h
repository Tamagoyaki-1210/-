//==============================================================================================
//
// �G���G�̗̑̓Q�[�W	�@�@mob_life_gauge.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _MOB_LIFE_GAUGE
#define	_MOB_LIFE_GAUGE

#include"gauge_manager.h"
#include"object3D.h"

class CMob_Life_Gauge : public CGauge_Manager
{
public:
	CMob_Life_Gauge();
	~CMob_Life_Gauge();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	void Fluctuation() override;

	static CMob_Life_Gauge *Create(const D3DXVECTOR3 &pos, D3DXVECTOR2 size);

	void SetMobLife_Pos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetDrawGauge(bool draw) override;

	CObject3D *GetBackGauge() { return m_BackGauge; }

private:
	D3DXVECTOR3 m_Pos;			// �ʒu
	CObject3D *m_BackGauge;		// �Q�[�W�̔w�i
	CObject3D *m_FrontGauge;	// �Q�[�W�̑O�i
};

#endif // !_MOB_LIFE_GAUGE
