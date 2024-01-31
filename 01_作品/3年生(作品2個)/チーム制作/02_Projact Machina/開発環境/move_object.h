//==============================================================================================
//
// �����I�u�W�F�N�g�@�@�@move_object.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _MOVE_OBJECT_H_
#define	_MOVE_OBJECT_H_

#include"object.h"
#include"collision.h"

class CObjectX;

class CMove_Object : public CObject
{
public:
	static const int DRAW_DROP_DISTANCE = 7000;		// ����A�p�[�c��\�����鋗��

	// �ړ��I�u�W�F�N�g�̃^�O
	enum TAG
	{
		TAG_NONE = 0,
		TAG_CHARACTER,		// �L�����N�^�[�^�O
		TAG_BULLET,			// �e�^�O
		TAG_ATTACK,			// �U���^�O
		TAG_EXPLOSION,		// �����^�O
		TAG_MAP_OBJECT,		// �}�b�v�I�u�W�F�N�g
		TAG_MAX,
	};

	enum COLLISION_TYPE
	{
		COLLISION_NONE = 0,
		COLLISION_TYPE_SHERER,
		COLLISION_TYPE_BLOCK,
		COLLISION_TYPE_BOSS,
		COLLISION_MAX,
	};

	CMove_Object(const PRIORITY priority);
	~CMove_Object();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	virtual void Hit(CMove_Object* pHit) = 0;
	void DelayCollision();							// �����蔻���x�����Đݒ肷�鏈��
	void CollisionDestroy();											// �����蔻��|�C���^����������

	void AddPos(const D3DXVECTOR3 pos) { m_pos += pos; }				// �ʒu�̉��Z

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }					// �ʒu�̐ݒ�
	void SetPosOld(const D3DXVECTOR3 posold) { m_posOld = posold; }					// �ʒu�̐ݒ�
	void SetCenterPos(const D3DXVECTOR3 pos) { m_CenterPos = pos; }		// �����ʒu�̐ݒ�
	void SetRadius(const float radius) { m_fRadius = radius; }			// ���a�̐ݒ�
	void SetTag(TAG tag) { m_tag = tag; }								// �^�O�̐ݒ�
	void SetPlayerSide(const bool side) { m_bPlayerSide = side; }		// �v���C���[�����ǂ����̐ݒ�
	void SetCollisionNoneHit(const bool noneHit) { m_bCollision_NoneHit = noneHit; }	// �����蔻��̃q�b�g������ǂݍ��܂Ȃ�����̐ݒ�
	void SetCollision();						// �����蔻��̐���
	void SetDelayCollision() { m_bCollision_Delay = true; }				// �����蔻���x�����Đ���
	void SetCollision_Type(COLLISION_TYPE type) { m_Collision_Type = type; }
	void SetSize(D3DXVECTOR3 size) { m_Size = size; }
	void SetGround(const bool ground) { m_bGround = ground; }
	void SetLandObj(bool land) { m_bLand_Obj = land; }
	void SetObjY(const float Y) { m_fObjY = Y; }
	void SetObjXZ(const bool xz) { m_bInXZ = xz; }
	//void SetOnObj(CMove_Object *onobj, int index) { m_OnObject[index] = onobj; }
	void SetIndex(D3DXVECTOR3 index, int cnt) { m_Index[cnt] = index; }
	void AddOnObjCnt(int cnt) { m_nOnObjCnt += cnt; }
	void SetOnObjCnt(int cnt) { m_nOnObjCnt = cnt; }
	void SetObjectX(CObjectX *objectX) { m_pObjectX = objectX; }

	const D3DXVECTOR3 GetPos() { return m_pos; }						// �ʒu�̎擾
	const D3DXVECTOR3 GetPosOld() { return m_posOld; }
	const D3DXVECTOR3 GetCenterPos() { return m_pos + m_CenterPos; }	// ���S�ʒu�̎擾
	virtual const int GetPower() { return NULL; }						// �З͂̎擾
	const float GetRadius() { return m_fRadius; }						// ���a�̎擾
	TAG GetTag() { return m_tag; }										// �^�O�̎擾
	const bool GetPlayerSide() { return m_bPlayerSide; }				// �v���C���[�����ǂ����̎擾
	const bool GetCollisionNoneHit() { return m_bCollision_NoneHit; }
	CCollision* GetCollision() { return m_pCollision; }					// �����蔻��̏��
	const COLLISION_TYPE GetCollision_Type() { return m_Collision_Type; }
	const D3DXVECTOR3 GetSize() { return m_Size; }
	const bool GetGround() { return m_bGround; }
	bool GetLandObj() { return m_bLand_Obj; }
	const float GetObjY() { return m_fObjY; }
	const bool GetObjXZ() { return m_bInXZ; }
	//CMove_Object *GetOnObj(int index) { return m_OnObject[index]; }
	D3DXVECTOR3 GetIndex(int cnt) { return m_Index[cnt]; }
	int GetOnObjCnt() { return m_nOnObjCnt; }
	CObjectX *GetObjectX() { return m_pObjectX; }

private:
	D3DXVECTOR3 m_pos;			// �ʒu
	D3DXVECTOR3 m_posOld;		// �O��̈ʒu
	D3DXVECTOR3 m_CenterPos;	// ���S�ʒu
	float m_fRadius;			// ���a
	TAG m_tag;					// �^�O
	CCollision* m_pCollision;	// �����蔻��̏��
	bool m_bPlayerSide;			// �v���C���[�����ǂ���
	bool m_bCollision_Delay;	// �����蔻��̒x���ݒu����
	bool m_bCollision_NoneHit;	// �����蔻��̃q�b�g������ǂݍ��܂Ȃ�����
	COLLISION_TYPE m_Collision_Type;		// �����蔻��̃^�C�v
	D3DXVECTOR3 m_Size;			// �T�C�Y
	bool m_bGround;				// �ڒn����
	bool m_bLand_Obj;			// �I�u�W�F�N�g�ɒ��n
	bool m_bInXZ;				// XZ���d�Ȃ��Ă邩
	float m_fObjY;				// �I�u�W�F�N�g��Y���W
	D3DXVECTOR3 m_Index[4];		// �I�u�W�F�N�g��4���_
	//CMove_Object *m_OnObject[128];
	int m_nOnObjCnt;
	CObjectX *m_pObjectX;
};

#endif // !_MOVE_OBJECT_H_
