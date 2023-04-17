//=============================================================================
//
// �v���C���[
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object2D.h"

//---------------------------
//�O���錾
//---------------------------
class CLife;

//---------------------------
//�N���X
//---------------------------
class CPlayer : public CObject2D
{
public:
	//�v���C���[���
	enum Player_Situation
	{
		SITU_NONE = 0,		//�����Ȃ����
		SITU_DAMAGE,		//�_���[�W���
		SITU_DEATH,			//���G���
		SITU_MAX,
	};

	CPlayer();
	~CPlayer() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Hit(int nDamage);
	void AddLife(int addLife);
	void AddMaxLife(int addMaxLife);
	void AddSpeed(float nSpeed);
	void AddBullet();
	void Input(D3DXVECTOR3 pos);
	void OffScreen(D3DXVECTOR3 pos);
	void Destroy(D3DXVECTOR3 pos);
	void Collision();
	void BlinkColor();
	void SituationType();

	static CPlayer *GetPlayer() { return m_pPlayer; }		//�v���C���[�擾
	static CPlayer *Create(D3DXVECTOR3 pos);
	void Load();

	Player_Situation GetSitu() { return m_situ; }
private:
	D3DXVECTOR3 m_posOld;		//�O��ʒu
	D3DXVECTOR3 m_move;			//�ړ���
	int m_nLife;				//�̗�
	int m_nMaxLife;				//�̗͍ő�l
	int m_nFastMaxLife;			//�����̗͍ő�l
	int m_nAnimX;				//�A�j���[�V����X���W
	int m_nAnimY;				//�A�j���[�V����Y���W
	int m_nBlinkLoop;			//�_�Ń��[�v��
	int m_nBulletCounter;		//�e�����J�E���^�[
	int m_nHoldBullet;			//�e���L��
	int m_nDeathTimer;			//���S����
	float m_fWidth;				//������
	float m_fHeight;			//�����c
	float m_fSpeed;				//���x
	float m_fBlinkCounter;		//�_�ŃJ�E���^�[
	float m_bBlink;				//�_�Ŕ���
	Player_Situation m_situ;	//�v���C���[���
	CLife *m_pLife;				//���C�t�|�C���^
	static CPlayer *m_pPlayer;
};

#endif // !_PLAYER_H_
