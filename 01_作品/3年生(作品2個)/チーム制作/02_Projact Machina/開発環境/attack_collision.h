//==============================================
//
// �U�������蔻��.h (attack_collision.h)
// Author : Tanimoto Kosuke
//
//==============================================
#ifndef  _ATTACK_COLLISION_H_				//���̃}�N����`������ĂȂ�������
#define  _ATTACK_COLLISION_H_				//��d�C���N���[�h�h�~�̃}�N����`

//==============================================
// �C���N���[�h
//==============================================
#include "main.h"
#include "move_object.h"

//==============================================
// �O���錾
//==============================================
class CObject3D;

//==============================================
// �N���X
//==============================================
class CAttack_Collision : public CMove_Object
{

public:

	CAttack_Collision(const PRIORITY priority);		// �i���o�[�̃R���X�g���N�^
	virtual ~CAttack_Collision() override;					// �i���o�[�̃f�X�g���N�^

	virtual HRESULT Init() override;				// ����������
	virtual void Uninit() override;					// �I������
	virtual void Update() override;					// �X�V����
	virtual void Draw() override;					// �`�揈��

	void Hit(CMove_Object* pHit) override;	// ��e����
	virtual void Destroy();							// �j�󏈗�

	void SetLife(const int life) { m_nLife = life; }		// �e�̎����̐ݒ�
	void SetPower(const int power) { m_nPower = power; }
	void SetObj3D(CObject3D *obj3d) { m_pObj3D = obj3d; }

	const int GetLife() { return m_nLife; }					// �e�̎����̎擾
	const int GetPower() override { return m_nPower; }		// �З͂̎擾
	CObject3D* GetObj3D() { return m_pObj3D; }

private:

	D3DXVECTOR3 m_nPosOld;					// �O��̈ʒu
	int m_nLife;							// �e�̎���
	int m_nPower;							// �e�̈З�

	CObject3D* m_pObj3D;					// �I�u�W�F�N�g3D�̃|�C���^
};
#endif //_ATTACK_COLLISION_H_
