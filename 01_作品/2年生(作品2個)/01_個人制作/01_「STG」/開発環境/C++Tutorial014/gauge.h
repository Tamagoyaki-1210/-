//=============================================================================
//
// �Q�[�W
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define GAUGE_WIDTH		(250)
#define GAUGE_HEIGHT	(50)

//---------------------------
//�N���X
//---------------------------
class CGauge : public CObject2D
{
public:
	CGauge();
	~CGauge() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetGauge(int life, int maxlife, int fastmaxlife);
	static CGauge *Create(D3DXVECTOR3 pos);
private:
	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_basepos;	//���̈ʒu
	D3DXVECTOR3 m_move;		//�ړ���
	float m_fWidth;			//������
	float m_fHeight;		//�����c
};

#endif // !_LIFE_H_

