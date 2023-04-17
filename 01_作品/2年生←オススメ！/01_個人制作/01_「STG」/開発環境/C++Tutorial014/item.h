//=============================================================================
//
// �A�C�e��
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object2D.h"

//---------------------------
//�N���X
//---------------------------
class CItem : public CObject2D
{
public:
	enum Item_Type
	{
		ENERGY_NONE = 0,	//�G�l���M�[�����̏��
		ENERGY_01,			//�G�l���M�[1
		ENERGY_02,			//�G�l���M�[2
		ENERGY_03,			//�G�l���M�[3
		ENERGY_04,			//�G�l���M�[4
		ENERGY_MAX,			//�A�C�e���ő�l
	};

	CItem();
	~CItem() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void TypeSet();
	void Destroy();
	void BlinkColor();
	void Collision(D3DXVECTOR3 pos, float fLength);
	//bool CollisionCircle(D3DXVECTOR3 pos, float fLength, D3DXVECTOR3 posSub, float fLengthSub);

	static CItem *Create(D3DXVECTOR3 pos, Item_Type settype);
private:
	D3DXVECTOR3 m_move;		//�ړ���
	int m_nLife;			//�̗�
	int m_nBlinkLoop;
	float m_fWidth;			//������
	float m_fHeight;		//�����c
	float m_fBlinkCounter;
	bool m_bBlink;
	Item_Type m_type;
};

#endif // !_ITEM_H_
