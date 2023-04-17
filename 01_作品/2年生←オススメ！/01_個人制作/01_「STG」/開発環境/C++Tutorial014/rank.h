//=============================================================================
//
// ����
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _RANK_H_
#define _RANK_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define COMMA_WIDTH		(40)
#define COMMA_HEIGHT	(80)

//---------------------------
//�N���X
//---------------------------
class CRank : public CObject2D
{
public:
	CRank();
	~CRank() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CRank *Create(D3DXVECTOR3 pos, int nID);
private:
	float m_fWidth;		//������
	float m_fHeight;	//�����c
};

#endif // !_RANK_H_
