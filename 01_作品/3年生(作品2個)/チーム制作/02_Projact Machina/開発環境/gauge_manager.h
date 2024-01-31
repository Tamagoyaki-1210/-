//==============================================================================================
//
// �Q�[�W�}�l�[�W���[	�@�@gauge_manager.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _GAUGE_MANAGER_H_
#define	_GAUGE_MANAGER_H_

#include"object.h"

class CGauge_Manager : public CObject
{
public:
	CGauge_Manager(const PRIORITY priority = PRIORITY_SCREEN);
	virtual ~CGauge_Manager() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw()  override;

	virtual void Fluctuation() = 0;

	void SetGaugePos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetGaugeSize(D3DXVECTOR2 size) { m_Size = size; }
	void SetBeaseSize(int beasesize) { m_nBease_GaugeSize = beasesize; }
	void SetBeaseLife(int beaselife) { m_nBease_Life = beaselife; }
	void SetLife(int life) { m_nLife = life; }
	virtual void SetDrawGauge(bool draw) = 0;

	D3DXVECTOR3 GetGaugePos() { return m_Pos; }
	D3DXVECTOR2 GetGaugeSize() { return m_Size; }
	int GetBeaseSize() { return m_nBease_GaugeSize; }
	int GetBeaseLife() { return m_nBease_Life; }
	int GetLife() { return m_nLife; }

private:
	D3DXVECTOR3 m_Pos;			// �ʒu
	D3DXVECTOR2 m_Size;			// �T�C�Y
	int m_nBease_GaugeSize;		// ���̃Q�[�W�T�C�Y
	int m_nBease_Life;			// ���̗̑�
	int m_nLife;				// ���݂̗̑�
};

#endif // !_GAUGE_MANAGER_H_
