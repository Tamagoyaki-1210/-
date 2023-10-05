//=============================================================================
//
// �w�i
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object2D.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define BG_WIDTH	(SCREEN_WIDTH)
#define BG_HEIGHT	(SCREEN_HEIGHT)

//---------------------------
//�N���X
//---------------------------
class CBG : public CObject2D
{
public:
	enum BG_Type
	{
		TYPE_BG01 = 0,		//�w�i
		TYPE_Tutorial01,	//�`���[�g���A��01
		TYPE_Tutorial02,	//�`���[�g���A��02
		TYPE_Tutorial03,	//�`���[�g���A��03
		TYPE_Clear,			//�N���A�w�i
		TYPE_MAX,			//�ő�l
	};

	CBG();
	~CBG() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void UpdateType();
	static CBG *Create(D3DXVECTOR3 pos, BG_Type bgType);
private:
	int m_nDestroyCounter;
	D3DXVECTOR3 m_move;	//�ړ���
	float m_fWidth;		//������
	float m_fHeight;	//�����c
	BG_Type m_bgType;	//�w�i���
};

#endif // !_PLAYER_H_
