//=============================================================================
//
// alien.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _ALIEN_H_
#define _ALIEN_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "enemy.h"

//---------------------------
// �N���X�錾
//---------------------------
class CAlien : public CEnemy
{
public:

	// ���f���̔z�u���
	enum Alien_Model
	{
		ALIEN_MODEL_UFO = 0,	// [0]UFO
		ALIEN_MODEL_ALIEN,		// [1]�G�C���A��
		ALIEN_MODEL_MAX,
	};

	// ���[�V�����̃^�C�v
	enum Alien_Motion_type
	{
		ALIEN_MOTION_NEUTRAL = 0,	// �j���[�g����
		ALIEN_MOTION_WALK,			// ����
		ALIEN_MOTION_MAX,
	};

	// �p�j���
	enum ALIEN_Roaming_Type
	{
		ROAMING_NONE = 0,	// �p�j���Ă��Ȃ�
		ROAMING_BOTTOM,		// ������
		ROAMING_LEFT,		// ������
		ROAMING_UP,			// �����
		ROAMING_RIGHT,		// �E����
		ROAMING_MAX,
	};

	CAlien();
	~CAlien() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void ChangeMotion() override;		// ���[�V�����̐؂�ւ�

	void AI();			// AI����
	void Search();		// �T������
	void Roaming();		// �p�j����
	void CollEnemy();

	static CAlien* Create(const D3DXVECTOR3 pos, const float alpha = 1.0f);
private:
	static const int Alien_KnockBack_Power = 10;	// �G�C���A���̃m�b�N�o�b�N�������

	static const float Alien_Roaming_Speed;			// �G�C���A���̜p�j���x
	static const float Alien_Search_Speed;			// �G�C���A���̒T�����x
	static const float Alien_Roaming_Clear;			// �G�C���A���̜p�j�����x
	static const float Alien_Radius_Collision;		// �����蔻��̃T�C�Y(���a)
	static const float Alien_Radius_Search;			// �����̃T�C�Y(���a)
	static const float Alien_Roaming_Length;		// �p�j���鋗��
	static const float Alien_Clear_Speed;			// �����ɂȂ鑬�x

	D3DXVECTOR3 m_anchorPos;						// �ʒu�̎�
	ALIEN_Roaming_Type m_Roaming;					// �p�j���
	bool m_bClockwise;								// ���v��肩�ǂ���
	float m_fAlpha;									// �A���t�@�l
};

#endif// _GHOST_H_