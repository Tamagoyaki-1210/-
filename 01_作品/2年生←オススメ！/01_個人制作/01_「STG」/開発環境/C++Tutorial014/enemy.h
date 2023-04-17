//=============================================================================
//
// �G�L����
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object2D.h"

//---------------------------
//�N���X
//---------------------------
class CEnemy : public CObject2D
{
public:
	enum Enemy_Type
	{
		TYPE_01 = 0,	//�G�L����1
		TYPE_02,		//�G�L����2
		TYPE_03,		//�G�L����3
		TYPE_04,		//�G�L����4
		TYPE_MAX,
	};

	//�G�L����AI
	enum Enemy_AI
	{
		AI_NONE = 0,	//�����Ȃ����
		AI_01,			//AI����1
		AI_02,			//AI����2
		AI_03,			//AI����3
		AI_MAX,
	};

	//�G�L�������
	enum Enemy_Situation
	{
		SITU_NONE = 0,		//�����Ȃ����
		SITU_DAMAGE,		//�_���[�W���
		SITU_NONEANIM,		//�A�j���[�V�������Ȃ����
		SITU_MAX,
	};

	CEnemy();
	~CEnemy() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void TypeAI(D3DXVECTOR3 pos);
	void Hit(int nDamage);
	void SetType();
	void OffScreen(D3DXVECTOR3 pos);
	void Destroy(D3DXVECTOR3 pos);
	void BlinkColor();
	void SituationType();

	static CEnemy *Create(D3DXVECTOR3 pos, Enemy_Type type);
	void Load();

	void SetSitu(Enemy_Situation situ) { m_situ = situ; }
	Enemy_Type GetType() { return m_type; }
private:
	int m_nLife;			//�̗�
	int m_nAnimX;			//�A�j���[�V����X���W
	int m_nAnimY;			//�A�j���[�V����Y���W
	int m_nBulletTimer;		//�e���˂܂ł̊Ԋu
	int m_nBulletCounter;	//�e���˃J�E���^�[
	int m_nBlinkLoop;		//�_�Ń��[�v��
	D3DXVECTOR3 m_move;		//�ړ���
	float m_fWidth;			//������
	float m_fHeight;		//�����c
	float m_fSpeed;			//�ړ����x
	float m_fBlinkCounter;	//�_�ŃJ�E���^�[
	float m_bBlink;			//�_�Ŕ���
	Enemy_Type m_type;		//�G�L�������
	Enemy_AI m_AI;			//�G�L����AI
	Enemy_Situation m_situ;	//�G�L�������
};

#endif // !_ENEMY_H_
