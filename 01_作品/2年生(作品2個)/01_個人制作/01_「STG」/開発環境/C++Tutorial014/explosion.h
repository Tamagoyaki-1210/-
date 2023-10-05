//=============================================================================
//
// ����
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define EXPLOSION_WIDTH		(80)
#define EXPLOSION_HEIGHT	(80)

//---------------------------
//�O���錾
//---------------------------
class CExplosion;

//---------------------------
//�N���X
//---------------------------
class CExplosion : public CObject2D
{
public:
	CExplosion();
	~CExplosion() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CExplosion *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
private:
	D3DXVECTOR3 m_move;		//�ړ���
	int m_nLife;			//�̗�
	float m_fWidth;			//������
	float m_fHeight;		//�����c
};

#endif // !_EXPLOSION_H_