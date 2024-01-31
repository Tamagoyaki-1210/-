//=============================================================================
//
// ����U��.cpp (weapon_attack.cpp)
// Author : Tanimoto Kosuke
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "application.h"
#include "renderer.h"
#include "weapon_attack.h"
#include "game.h"
#include "object3D.h"
#include "tutorial.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CWeapon_Attack::CWeapon_Attack(const CObject::PRIORITY priority) : CAttack_Collision(priority)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CWeapon_Attack::~CWeapon_Attack()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CWeapon_Attack::Init()
{
	CAttack_Collision::Init();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CWeapon_Attack::Uninit()
{
	CAttack_Collision::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CWeapon_Attack::Update()
{
	CAttack_Collision::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CWeapon_Attack::Draw()
{

}

//=============================================================================
// ��������
//=============================================================================
CWeapon_Attack* CWeapon_Attack::Create(const D3DXVECTOR3 pos, const float radius, const bool side, const int power, const int life)
{
	//�N���X�̐���
	CWeapon_Attack* pWeapon_Attack = new CWeapon_Attack(PRIORITY_BACK);

	//null�`�F�b�N
	if (pWeapon_Attack != nullptr)
	{
		//����������
		pWeapon_Attack->Init();
		//�ݒ菈��
		pWeapon_Attack->SetPos(pos);
		pWeapon_Attack->SetRadius(radius);
		pWeapon_Attack->SetPlayerSide(side);
		pWeapon_Attack->SetPower(power);
		pWeapon_Attack->SetLife(life);

#ifdef _DEBUG
		// �f�o�b�O�I�u�W�F�N�g�̐���
		CObject3D* pObj3D = CObject3D::Create(pos, { radius, radius }, PRIORITY_BACK);

		// �e�N�X�`��
		pObj3D->SetTexture(CTexture::TEXTURE_CIRCLE);

		// �r���{�[�h�̐ݒ�
		pObj3D->Setbillboard(true);

		// �f�o�b�O�I�u�W�F�N�g�̐ݒ�
		pWeapon_Attack->SetObj3D(pObj3D);
#endif
	}
	else
	{
		assert(false);
	}

	return pWeapon_Attack;
}