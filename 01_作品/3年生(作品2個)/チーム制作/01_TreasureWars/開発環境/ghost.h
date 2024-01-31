//=============================================================================
//
// ghost.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _GHOST_H_
#define _GHOST_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "enemy.h"

//---------------------------
// �N���X�錾
//---------------------------
class CGhost : public CEnemy
{
public:

	// ���f���̔z�u���
	enum Ghost_Model
	{
		GHOST_MODEL_BODY = 0,
		GHOST_MODEL_MAX,
	};

	// ���[�V�����̃^�C�v
	enum Ghost_Motion_type
	{
		GHOST_MOTION_NEUTRAL = 0,	// �j���[�g����
		GHOST_MOTION_WALK,			// ����
		//GHOST_MOTION_ATTACK,		// �U��
		GHOST_MOTION_MAX,
	};

	// �p�j���
	enum Ghost_Roaming_Type
	{
		ROAMING_NONE = 0,	// �p�j���Ă��Ȃ�
		ROAMING_BOTTOM,		// ������
		ROAMING_LEFT,		// ������
		ROAMING_UP,			// �����
		ROAMING_RIGHT,		// �E����
		ROAMING_MAX,
	};

	CGhost();
	~CGhost() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void ChangeMotion() override;		// ���[�V�����̐؂�ւ�

	void AI();			// AI����
	void Search();		// �T������
	void Roaming();		// �p�j����
	void CollEnemy ();

	static CGhost* Create(const D3DXVECTOR3 pos, const float alpha = 1.0f);
private:
	static const int Ghost_KnockBack_Power = 10;	// �S�[�X�g�̃m�b�N�o�b�N�������

	static const float Ghost_Roaming_Speed;			// �S�[�X�g�̜p�j���x
	static const float Ghost_Search_Speed;			// �S�[�X�g�̒T�����x
	static const float Ghost_Roaming_Clear;			// �S�[�X�g�̜p�j�����x
	static const float Ghost_Radius_Collision;		// �����蔻��̃T�C�Y(���a)
	static const float Ghost_Radius_Search;			// �����̃T�C�Y(���a)
	static const float Ghost_Roaming_Length;		// �p�j���鋗��
	static const float Ghost_Clear_Speed;			// �����ɂȂ鑬�x

	D3DXVECTOR3 m_anchorPos;						// �ʒu�̎�
	Ghost_Roaming_Type m_Roaming;					// �p�j���
	bool m_bClockwise;								// ���v��肩�ǂ���
	float m_fAlpha;									// �A���t�@�l
};

#endif// _GHOST_H_