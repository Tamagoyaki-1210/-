//=============================================================================
//
// �e
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _BULLET_H_
#define _BILLET_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define MAX_ENEMY_HIT			(100)	//�G�L���������q�b�g�ő吔
#define MAX_OBSTACLE_HIT		(40)	//��Q�������q�b�g�ő吔

//---------------------------
//�O���錾
//---------------------------
class CTexture;
class CEnemy;
class CObstacle;
class CRay;

//---------------------------
//�N���X
//---------------------------
class CBullet: public CObject2D
{
public:
	enum Bullet_Player
	{
		PLAYER_NONE = 0,//�v���C���[�e�s�g�p
		PLAYER_NORMAL,	//�v���C���[�ʏ�e
		PLAYER_LASER,	//�v���C���[���[�U�[
		PLAYER_BOMM,	//�v���C���[�{��
		PLAYER_MAX,		//�v���C���[�e�ő�l
	};

	enum Bullet_Enemy
	{
		ENEMY_NONE = 0,	//�G�L�����e�s�g�p
		ENEMY_NORMAL,	//�G�L�����ʏ�e
		ENEMY_LASER,	//�G�L�������[�U�[
		ENEMY_SPREAD,	//�G�L�����g�U�e
		ENEMY_MAX,		//�G�L�����e�ő�l
	};

	enum Bullet_Collision
	{
		COLLISION_CIRCLE = 0,	//�G�L�����e�s�g�p
		COLLISION_SQUARE,		//�G�L�����ʏ�e
		COLLISION_MAX,			//�G�L�����e�ő�l
	};

	CBullet();
	~CBullet() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void TypeAI(D3DXVECTOR3 pos);
	void OffScreen(D3DXVECTOR3 pos);
	void Destroy();
	void Collision(D3DXVECTOR3 pos, float fLength);

	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, Bullet_Player playerBullet);
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, Bullet_Enemy enemyBullet);
	void LoadPlayerBullet();
	void LoadEnemyBullet();
private:
	D3DXVECTOR3 m_poslast;		//�O��ʒu
	D3DXVECTOR3 m_move;			//�ړ���
	int m_nLife;				//�̗�
	int m_nPower;				//�З�
	int m_nPenetrat;			//�ђʉ�
	float m_fMaxWidth;			//�ő咷����
	float m_fWidth;				//������
	float m_fHeight;			//�����c
	float m_fSpeed;				//�ړ����x
	bool m_bHitDestroy;			//�e�ђʔ���
	Bullet_Player m_playerBullet;
	Bullet_Enemy m_enemyBullet;
	Bullet_Collision m_collisionBullet;
	CEnemy *m_pEnemy[MAX_ENEMY_HIT];
	CObstacle *m_pObstacle[MAX_OBSTACLE_HIT];
};

#endif // !_BILLET_H_
