//=============================================================================
//
// pteranodon.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _PTERANODON_H_
#define _PTERANODON_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "enemy.h"

//---------------------------
// �N���X�錾
//---------------------------
class CPteranodon : public CEnemy
{
public:
	// ���f���̔z�u���
	enum Ptera_Model
	{
		PTERA_MODEL_BODY = 0,	// [0]��
		PTERA_MODEL_HEAD,		// [1]��
		PTERA_MODEL_ARM_LEFT,	// [2]���r
		PTERA_MODEL_ARM_RIGHT,	// [3]�E�r
		PTERA_MODEL_LEG,		// [4]��
		PTERA_MODEL_MAX,
	};							
	// ���[�V�����̃^�C�v
	enum Ptera_Motion_type
	{
		PTERA_MOTION_NEUTRAL = 0,	// �j���[�g����
		PTERA_MOTION_WALK,			// ����
		//PTERA_MOTION_ATTACK,		// �U��
		PTERA_MOTION_MAX,
	};

	// �p�j���
	enum Ptera_Roaming_Type
	{
		ROAMING_NONE = 0,	// �p�j���Ă��Ȃ�
		ROAMING_BOTTOM,		// ������
		ROAMING_LEFT,		// ������
		ROAMING_UP,			// �����
		ROAMING_RIGHT,		// �E����
		ROAMING_MAX,
	};

	CPteranodon();
	~CPteranodon() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void ChangeMotion() override;		// ���[�V�����̐؂�ւ�

	void AI();			// AI����
	void Search();		// �T������
	void Roaming();		// �p�j����
	void CollEnemy();

	static CPteranodon* Create(const D3DXVECTOR3 pos, const float alpha = 1.0f);
private:
	static const int Ghost_KnockBack_Power = 10;	// �S�[�X�g�̃m�b�N�o�b�N�������

	static const float Ptera_Roaming_Speed;			// �S�[�X�g�̜p�j���x
	static const float Ptera_Search_Speed;			// �S�[�X�g�̒T�����x
	static const float Ptera_Roaming_Clear;			// �S�[�X�g�̜p�j�����x
	static const float Ptera_Radius_Collision;		// �����蔻��̃T�C�Y(���a)
	static const float Ptera_Radius_Search;			// �����̃T�C�Y(���a)
	static const float Ptera_Roaming_Length;		// �p�j���鋗��
	static const float Ptera_Clear_Speed;			// �����ɂȂ鑬�x

	D3DXVECTOR3 m_anchorPos;						// �ʒu�̎�
	Ptera_Roaming_Type m_Roaming;					// �p�j���
	bool m_bClockwise;								// ���v��肩�ǂ���
	float m_fAlpha;									// �A���t�@�l
};

#endif// _PTERANODON_H_