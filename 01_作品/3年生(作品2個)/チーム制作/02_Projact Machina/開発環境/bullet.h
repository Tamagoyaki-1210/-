//==============================================
//
// �e.h (Bullet.h)
// Author : Saito Shian
// Author : Tanimoto Kosuke
//
//==============================================
#ifndef  _BULLET_H_				//���̃}�N����`������ĂȂ�������
#define  _BULLET_H_				//��d�C���N���[�h�h�~�̃}�N����`

//==============================================
// �C���N���[�h
//==============================================
#include "main.h"
#include "move_object.h"

//==============================================
// �O���錾
//==============================================
class CObject3D;
class CObjectX;

//==============================================
// �e�N���X
//==============================================
class CBullet : public CMove_Object
{
	static const float BULLET_SPEED_XZ;			// �e�̑��x(XZ���W)
	static const float BULLET_SPEED_Y;			// �e�̑��x(Y���W)
	static const float BULLET_COLLISION_RADIUS;	// �e�̓����蔻��̑傫��
public:

	//****************
	// public�Ȋ֐�
	//****************
	CBullet(const PRIORITY priority);		// �i���o�[�̃R���X�g���N�^
	~CBullet() override;					// �i���o�[�̃f�X�g���N�^

	HRESULT Init() override;				// ����������
	void Uninit() override;					// �I������
	void Update() override;					// �X�V����
	void Draw() override;					// �`�揈��

	void FieldCollision();					//���̓����蔻��
	void Hit(CMove_Object* pHit) override;	// ��e����
	virtual void Destroy();							// �j�󏈗�
	void Map_Object_Collision();

	void SetLife(const int life) { m_nLife = life; }		// �e�̎����̐ݒ�
	void SetSpeed_XZ(const float speed) { m_fSpeed_XZ = speed; }	// �e�̑��x�̐ݒ�
	void SetSpeed_Y(const float speed) { m_fSpeed_Y = speed; }	// �e�̑��x�̐ݒ�
	void SetSpeed(const float speed) { m_fSpeed_XZ = speed; m_fSpeed_Y = speed;}	// �e�̑��x�̐ݒ�
	void SetPower(const int power) { m_nPower = power; }
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }	// �ړ��ʂ̐ݒ�
	void AddMove(const D3DXVECTOR3 move) { m_move += move; }// �ړ��ʂ̉��Z
	void SetSize(const D3DXVECTOR2 size) { m_size = size; }	// �T�C�Y�̐ݒ�
	void SetObj3D(CObject3D *obj3d) { m_pObj3D = obj3d; }
	void SetObjX(CObjectX *objx) { m_pObjX = objx; }
	void SetExplosion(bool explosion) { m_bExplosion = explosion; }

	const int GetLife() { return m_nLife; }					// �e�̎����̎擾
	const float GetSpeed_XZ() { return m_fSpeed_XZ; }				// �e�̑��x�̎擾
	const float GetSpeed_Y() { return m_fSpeed_Y; }				// �e�̑��x�̎擾
	const int GetPower() override { return m_nPower; }		// �З͂̎擾
	const D3DXVECTOR3 GetMove() { return m_move; }			// �ړ��ʂ̎擾
	const D3DXVECTOR2 GetSize() { return m_size; }			// �T�C�Y�̎擾
	CObject3D *GetObj3D() { return m_pObj3D; }
	CObjectX *GetObjX() { return m_pObjX; }

	static CBullet* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const D3DXVECTOR3 move, const bool side, const int power, const float speed, const int life);	// ����

private:
	//****************
	// private�ȕϐ�
	//****************
	D3DXVECTOR3 m_move;						// �e�̈ړ���
	D3DXVECTOR3 m_nPosOld;					// �O��̈ʒu
	D3DXVECTOR2 m_size;						// �e�̃T�C�Y
	int m_nLife;							// �e�̎���
	int m_nPower;							// �e�̈З�
	float m_fSpeed_XZ;						// �e�̃X�s�[�h
	float m_fSpeed_Y;						// �e�̃X�s�[�h
	bool m_bExplosion;						// �q�b�g���ɔ������邩

	CObject3D* m_pObj3D;					// �I�u�W�F�N�g3D�̃|�C���^
	CObjectX* m_pObjX;					// �I�u�W�F�N�gX�̃|�C���^
};
#endif
