//=============================================================================
//
// ���C�t
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define LIFE_WIDTH	(250)
#define LIFE_HEIGHT	(50)

//---------------------------
//�O���錾
//---------------------------
class CGauge;

//---------------------------
//�N���X
//---------------------------
class CLife : public CObject2D
{
public:
	CLife();
	~CLife() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetLife(int life, int maxlife, int fastmaxlife);
	static CLife *Create(D3DXVECTOR3 pos);
private:
	int m_nLife;			//�̗�
	float m_fWidth;			//������
	float m_fHeight;		//�����c
	CGauge *m_pGauge;		//�Q�[�W�|�C���^
};

#endif // !_LIFE_H_

