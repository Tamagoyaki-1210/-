//==============================================================================================
//
// �Q�[�W�@�@�@gauge.h
// tutida ryousei
//
//==============================================================================================
#ifndef _GAUGE_H_
#define	_GAUGE_H_

#include"object2D.h"
#include"ui.h"

class CGauge : public CUi
{
public:
	CGauge();
	~CGauge() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CGauge *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	void ZeroGauge();	// �Q�[�W��0�ɂ���
	void GaugeUp();		// �Q�[�W�𑝂₷

	void SetBase_Size(D3DXVECTOR3 base_size) { m_Base_Size = base_size; }
	
private:
	
	CObject2D *m_pBar;			// ���̍����o�[
	CObject2D *m_pGauge;		// �O���̐L�яk�݂���Q�[�W
	D3DXVECTOR3 m_Base_Size;	// ���̃T�C�Y
	D3DXVECTOR3 m_Size;			// ���݂̃T�C�Y
	float m_fGauge_Heal;		// �Q�[�W�̉񕜑��x
	bool m_bZero;
	static bool m_bUse;			// �N�����Q�[�W���g�p�������̔���
};

#endif // !_GAUGE_H_
