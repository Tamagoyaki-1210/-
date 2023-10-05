//=============================================================================
//
// ��Q��
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object2D.h"

//---------------------------
//�N���X
//---------------------------
class CObstacle : public CObject2D
{
public:
	enum Obstacle_Type
	{
		TYPE_01 = 0,	//��Q��1
		TYPE_02,		//��Q��2
		TYPE_MAX,
	};

	enum Obstacle_Flip
	{
		FLIP_CIRCLE = 0,	//�~�`
		FLIP_SQUARE,		//�l�p�`
		FLIP_MAX,
	};

	//��Q�����
	enum Obstacle_Situation
	{
		SITU_NONE = 0,		//�����Ȃ����
		SITU_DAMAGE,		//�_���[�W���
		SITU_MAX,
	};

	CObstacle();
	~CObstacle() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void TypeAI();
	void Hit(int nDamage);
	void SetType();
	void OffScreen(D3DXVECTOR3 pos);
	void Destroy(D3DXVECTOR3 pos);
	void BlinkColor();
	void SituationType();

	D3DXVECTOR3 FlipCircle(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fLength);
	D3DXVECTOR3 FlipSquare(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fWidth, float fHeight);
	static CObstacle *Create(D3DXVECTOR3 pos, Obstacle_Type type);
	void Load();

	void SetSitu(Obstacle_Situation situ) { m_situ = situ; }

	Obstacle_Flip GetFlip() { return m_flip; }
private:
	int m_nLife;			//�̗�
	int m_animX;			//�A�j���[�V����X���W
	int m_animY;			//�A�j���[�V����Y���W
	int m_nBlinkLoop;		//�_�Ń��[�v��
	D3DXVECTOR3 m_posOld;	//�O��ʒu
	D3DXVECTOR3 m_move;		//�ړ���
	float m_fWidth;			//������
	float m_fHeight;		//�����c
	float m_fBlinkCounter;	//�_�ŃJ�E���^�[
	float m_bBlink;			//�_�Ŕ���
	Obstacle_Type m_type;	//��Q���̎��
	Obstacle_Flip m_flip;	//�e������̎��
	Obstacle_Situation m_situ;	//�G�L�������
};

#endif // !_OBSTACLE_H_
