//==============================================
//
// ����U��.h (weapon_attack.h)
// Author : Tanimoto Kosuke
//
//==============================================
#ifndef  _WEAPON_ATTACK_H_				//���̃}�N����`������ĂȂ�������
#define  _WEAPON_ATTACK_H_				//��d�C���N���[�h�h�~�̃}�N����`

//==============================================
// �C���N���[�h
//==============================================
#include "attack_collision.h"

//==============================================
// �N���X
//==============================================
class CWeapon_Attack : public CAttack_Collision
{

public:

	CWeapon_Attack(const PRIORITY priority);		// �i���o�[�̃R���X�g���N�^
	~CWeapon_Attack() override;					// �i���o�[�̃f�X�g���N�^

	HRESULT Init() override;				// ����������
	void Uninit() override;					// �I������
	void Update() override;					// �X�V����
	void Draw() override;					// �`�揈��

	// ����U��(�ʒu�A�傫���A�����A�T�C�h�A�З�, ����)
	static CWeapon_Attack* Create(const D3DXVECTOR3 pos, const float radius, const bool side, const int power, const int life);

private:

};
#endif //_WEAPON_ATTACK_H_