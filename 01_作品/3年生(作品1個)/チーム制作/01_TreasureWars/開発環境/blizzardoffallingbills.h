//==============================================================================================
//
// �D����@�@�@blizzardoffallingbills.h
// Author :�@koduna hirohito
//
//==============================================================================================
#ifndef _BLIZZARD_OF_FALLING_BILLS_H_
#define	_BLIZZARD_OF_FALLING_BILLS_H_

#include"object2D.h"


class CBlizzardOfFallingBills : public CObject2D
{
	static const int EFFECT_PATTERN_MAX = 4;		//�D����̃p�^�[����
	static const float WIDTH_PER_PATTERN_ONE;		//�p�^�[���������̕�
	static const float BASIC_ROT_MOVE;				//Rot�̊�{�ړ���
	static const float BASIC_POS_MOVE;				//Pos�̊�{�ړ���
	static const float RANGE_WITH_PARENT;			//���z�e�Ƃ̋���
	static const float RIGHT_LEFT_WIDTH;			//�E���̕�

public:
	CBlizzardOfFallingBills(int nPriority);
	~CBlizzardOfFallingBills() override;

	// ��`�����̕`��̏����ݒ� = 3
	static CBlizzardOfFallingBills *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nPriority = 3);

	HRESULT Init() override;
	void Update() override;

	void SetParentPos(D3DXVECTOR3 pos) { m_ParentPos = pos; }
private:

	float m_fParentRot;		//���z�e��Rot
	D3DXVECTOR3 m_ParentPos;		//���z�e��Pos
	float m_fParentRotMove;	//���z�e��Rot�̈ړ���
	D3DXVECTOR3 m_ParentPosMove;	//���z�e��Pos�̈ړ���
	float m_fRangeWithParent;		//���z�e�Ƃ̋���
	bool m_bRightLeft;				//�E��
};

#endif // !_BLIZZARD_OF_FALLING_BILLS_H_
