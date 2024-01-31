//=============================================================================
//
// collision.cpp
// Author : Tutida Ryousei
//
//=============================================================================
#include "collision.h"
#include "collision_manager.h"
#include "application.h"
#include "tutorial.h"
#include "game.h"
#include "move_object.h"
#include "object3D.h"
#include "meshfield.h"

#include"debugProc.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCollision::CCollision()
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	// �������Ɏ��g�̃|�C���^�𓖂��蔻��}�l�[�W���[�ɐݒ�
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		CTutorial::GetCollision_Manager()->SetCollision(this);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		CGame::GetCollision_Manager()->SetCollision(this);
	}
	m_bDeath = false;
	m_bNoneHit = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCollision::~CCollision()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCollision::Init()
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCollision::Uninit()
{
	// �e�̓����蔻��|�C���^����������
	m_pParent->CollisionDestroy();

#ifdef _DEBUG
	// �f�o�b�O�I�u�W�F�N�g���g�p���̏ꍇ
	if (m_pDebugObj != nullptr)
	{
		m_pDebugObj->Uninit();
		m_pDebugObj = nullptr;
	}
#endif // _DEBUG

	Releace();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCollision::Update()
{
#ifdef _DEBUG
	DebugObj();
#endif // _DEBUG

	// �����蔻��
	Collision();
}

//=============================================================================
// �����蔻��
//=============================================================================
void CCollision::Collision()
{
	// �q�b�g������ǂݍ��ޏꍇ
	if (m_bNoneHit == false)
	{
		// �����̈ʒu
		D3DXVECTOR3 pos = m_pParent->GetCenterPos();
		// ���a
		float fRadius = m_pParent->GetRadius();
		// �T�C�Y
		D3DXVECTOR3 Size = m_pParent->GetSize();

		// ���݂̃��[�h
		CApplication::MODE Mode = CApplication::GetModeType();

		CCollision_Manager* pCollision_Manager = nullptr;

		// ���[�h���Ƀv���C���[��ǂݍ���
		if (Mode == CApplication::MODE_TUTORIAL)
		{
			pCollision_Manager = CTutorial::GetCollision_Manager();
		}
		else if (Mode == CApplication::MODE_GAME)
		{
			pCollision_Manager = CGame::GetCollision_Manager();
		}

		// �S�Ă̓����蔻����ʂɔ���
		for (auto pCollision : pCollision_Manager->GetAllCollision())
		{
			// �����蔻�肪���݂���ꍇ && �����蔻�肪���g�ł͂Ȃ��ꍇ && �q�b�g���肪����ꍇ
			if (!pCollision->GetParent()->GetDeathFlag() && !pCollision->GetDeath() && pCollision != this && !pCollision->m_bNoneHit)
			{
				// �����moveobject�̏��
				CMove_Object* pMove_Object = pCollision->GetParent();

				// ����̈ʒu
				D3DXVECTOR3 OtherPos = pMove_Object->GetCenterPos();

				// �������Ă��邩�ǂ���
				bool bHit = false;

				// �����蔻��̃^�C�v���~�̏ꍇ
				if (pMove_Object->GetCollision_Type() == CMove_Object::COLLISION_TYPE_SHERER)
				{
					// ���a
					float fOtherRadius = pMove_Object->GetRadius();

					// �~���m�̓����蔻��̌v�Z
					bHit = Sphere_Collision(pos, fRadius, OtherPos, fOtherRadius);
				}
				// �����蔻��̃^�C�v����`�̏ꍇ
				else if (pMove_Object->GetCollision_Type() == CMove_Object::COLLISION_TYPE_BLOCK
					&& pMove_Object->GetCollision_Type() != m_pParent->GetCollision_Type())
				{
					D3DXVECTOR3 Pos = m_pParent->GetPos();
					// �O��̈ʒu
					D3DXVECTOR3 PosOld = m_pParent->GetPosOld();

					// �ʒu
					D3DXVECTOR3 Otherpos = pMove_Object->GetPos();
					// �T�C�Y
					D3DXVECTOR3 OtherSize = pMove_Object->GetSize();

					// ��`�̓����蔻��̌v�Z
					bHit = Block_Collision(Pos, PosOld, Size, Otherpos, OtherSize, m_pParent, pMove_Object);
				}

				// �q�b�g�����ꍇ
				if (bHit)
				{
					// ���g�̐ڐG����
					m_pParent->Hit(pMove_Object);

					// ����̐ڐG����
					pMove_Object->Hit(m_pParent);
				}
			}
		}
	}
}

//==============================================================================================
// �I�u�W�F�N�g���p���������̂̓����蔻��
//==============================================================================================
bool CCollision::Sphere_Collision(const D3DXVECTOR3 pos, const float radius, const D3DXVECTOR3 otherPos, const float otherRadius)
{
	// �ʒu���m�̋����̌v�Z�p�ϐ�
	D3DXVECTOR3 Distance = otherPos - pos;

	float Dis = sqrtf((otherPos.x - pos.x) * (otherPos.x - pos.x)
		+ (otherPos.z - pos.z) * (otherPos.z - pos.z));

	float DisY = otherPos.y - pos.y;
	if (DisY < 0)
		DisY *= -1;

	if (Dis <= radius + otherRadius
		&& DisY <= radius + otherRadius)
		return true;

	return false;
}

//=============================================================================
// ��`�̓����蔻��
//=============================================================================
bool CCollision::Block_Collision(const D3DXVECTOR3 pos, const D3DXVECTOR3 posold, const D3DXVECTOR3 size, const D3DXVECTOR3 otherpos, const D3DXVECTOR3 othersize, CMove_Object *objParent, CMove_Object *objOther)
{
	D3DXVECTOR3 Pos = pos;
	bool bHit = false;

	// ��`�̏�ɏ�����ꍇ ���� �ݒu���Ă��锻��
	if ((otherpos.y + othersize.y > pos.y && otherpos.y < pos.y + size.y + othersize.y)
		|| objParent->GetLandObj())
	{
		//// ��ɏ��
		//if (otherpos.y + othersize.y <= posold.y
		//	&& otherpos.x + othersize.x > pos.x - size.x
		//	&& otherpos.x - othersize.x < pos.x + size.x
		//	&& otherpos.z + othersize.z > pos.z - size.z
		//	&& otherpos.z - othersize.z < pos.z + size.z)
		//{
		//	// �I�u�W�F�N�g�ɏ���Ă鐔
		//	int nOnObj = objOther->GetOnObjCnt();
		//	bool bOn = false;

		//	for (int nCnt = 0; nCnt < nOnObj; nCnt++)
		//	{
		//		if (objOther->GetOnObj(nCnt) == objParent)
		//			bOn = true;
		//	}

		//	if (!bOn)
		//	{
		//		objOther->SetOnObj(objParent, nOnObj);
		//		nOnObj++;
		//	}

		//	objOther->SetOnObjCnt(nOnObj);

		//	objOther->SetLandObj(true);		// Move_Object����ɏ���Ă邩
		//	objOther->SetObjXZ(true);

		//	objParent->SetOnObj(objOther, 0);
		//	objParent->SetLandObj(true);									// �I�u�W�F�N�g�̏�ɏ���Ă邩
		//	objParent->SetObjXZ(true);										// �I�u�W�F�N�g��XZ�����d�Ȃ��Ă邩
		//	objParent->SetObjY(otherpos.y + othersize.y);					// �I�u�W�F�N�g�̍���
		//	objParent->SetPos({ Pos.x,otherpos.y + othersize.y,Pos.z });	// �����o��
		//	bHit = true;
		//}
		//// �~���
		//else
		//{
		//	for (int nCnt = 0; nCnt < objOther->GetOnObjCnt(); nCnt++)
		//	{
		//		if (objOther->GetOnObj(nCnt) != nullptr
		//			&& objOther->GetLandObj() && objOther->GetObjXZ()
		//			&& objOther->GetOnObj(nCnt)->GetLandObj() && objOther->GetOnObj(nCnt)->GetObjXZ()
		//			&& objOther->GetOnObj(nCnt) == objParent)
		//		{
		//			objOther->SetLandObj(false);
		//			objOther->SetObjXZ(false);

		//			for (int nCnt2 = 0; nCnt2 < objOther->GetOnObjCnt(); nCnt2++)
		//			{
		//				objOther->SetOnObj(nullptr, nCnt2);
		//			}
		//			objOther->SetOnObjCnt(0);

		//			objParent->SetLandObj(false);
		//			objParent->SetGround(false);
		//			objParent->SetObjXZ(false);
		//			bHit = false;
		//		}
		//	}
		//}
		// ������Ԃ���
		/*if (otherpos.y > posold.y
			&& otherpos.x + othersize.x > pos.x - size.x
			&& otherpos.x - othersize.x < pos.x + size.x
			&& otherpos.z + othersize.z > pos.z - size.z
			&& otherpos.z - othersize.z < pos.z + size.z)
		{
			objParent->SetPos({ Pos.x,otherpos.y - size.y - othersize.y,Pos.z });	// �����o��
		}
		// ���n���Ă���ꍇ
		else */if (!objParent->GetLandObj())
		{
			D3DXVECTOR3 Index[4] = {};				// �I�u�W�F�N�g��4���_�̈ʒu
			D3DXVECTOR3 Index_Vec[4] = {};			// ���_���璸�_�܂ł̃x�N�g��
			D3DXVECTOR3 Char_Vec[4] = {};			// ���_����L�����܂ł̃x�N�g��
			D3DXVECTOR3 CharCenter_Vec[4] = {};		// ���_����L�����̒��S�܂ł̃x�N�g��
			float fCp[4] = {};						// �O��
			float fCenter_Cp[4] = {};				// �O��
			int nCp_Count = 0;						// �L�����������ɂ���x�N�g���̃J�E���g
			int nHit_Index = 0;						// �q�b�g�����x�N�g���̔ԍ�

			D3DXVECTOR3 Posplus[4] = {};
			Posplus[0] = { pos.x + size.x,0.0f,pos.z };
			Posplus[1] = { pos.x,0.0f,pos.z - size.z };
			Posplus[2] = { pos.x - size.x,0.0f,pos.z };
			Posplus[3] = { pos.x,0.0f,pos.z + size.z };

			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				// �I�u�W�F�N�g�̎l���_�̈ʒu
				Index[nCnt] = otherpos + objOther->GetIndex(nCnt);
			}
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				if (nCnt != 3)
					Index_Vec[nCnt] = Index[nCnt + 1] - Index[nCnt];
				else
					Index_Vec[nCnt] = Index[0] - Index[nCnt];

				Char_Vec[nCnt] = Posplus[nCnt] - Index[nCnt];
				CharCenter_Vec[nCnt] = pos - Index[nCnt];

				// �v���X���O��
				fCp[nCnt] = Index_Vec[nCnt].x * Char_Vec[nCnt].z - Index_Vec[nCnt].z * Char_Vec[nCnt].x;
				fCenter_Cp[nCnt] = Index_Vec[nCnt].x * CharCenter_Vec[nCnt].z - Index_Vec[nCnt].z * CharCenter_Vec[nCnt].x;

				if (fCp[nCnt] <= 0)
					nCp_Count++;
			}

			// �L�������I�u�W�F�N�g�Ƀq�b�g���Ă���ꍇ
			if (nCp_Count == 4)
			{
				for (int nCnt = 0; nCnt < 4; nCnt++)
				{
					if (fCenter_Cp[nCnt] >= 0)
					{
						float fInner = D3DXVec3Dot(&Index_Vec[nCnt], &CharCenter_Vec[nCnt]);

						if (fInner > 0.0f)
						{
							// �������Ă��
							nHit_Index = nCnt;
							bHit = true;

							break;
						}
					}
				}

				//if (objParent->GetCollision_Type() != CMove_Object::COLLISION_TYPE_BOSS)
				//{
				//	// �����o��
				//	switch (nHit_Index)
				//	{
				//	case 0:
				//		objParent->SetPos({ Index[nHit_Index].x - size.x,pos.y,pos.z });
				//		break;
				//	case 1:
				//		objParent->SetPos({ pos.x,pos.y,Index[nHit_Index].z + size.z });
				//		break;
				//	case 2:
				//		objParent->SetPos({ Index[nHit_Index].x + size.x,pos.y,pos.z });
				//		break;
				//	case 3:
				//		objParent->SetPos({ pos.x,pos.y,Index[nHit_Index].z - size.z });
				//		break;
				//	default:
				//		break;
				//	}
				//}
			}
		}
	}

	return bHit;
}

// �f�o�b�O�p�֐��̏���
#ifdef _DEBUG
//=============================================================================
// �f�o�b�O�I�u�W�F�N�g�̏���
//=============================================================================
void CCollision::DebugObj()
{
	// �f�o�b�O�I�u�W�F�N�g���g�p���̏ꍇ
	if (m_pDebugObj != nullptr)
	{
		// �ʒu��ݒ�
		m_pDebugObj->SetPos(m_pParent->GetCenterPos());
	}
}

//=============================================================================
// �f�o�b�O�I�u�W�F�N�g�̏���
//=============================================================================
void CCollision::SetDebugObj()
{
	const float fRadius = m_pParent->GetRadius() * 2.0f;

	// �f�o�b�O�I�u�W�F�N�g�̐���
	m_pDebugObj = CObject3D::Create(m_pParent->GetCenterPos(), D3DXVECTOR2(fRadius, fRadius));

	// �F�̐ݒ�(�v���C���[�����ǂ��� ? ���F : �ԐF)
	m_pDebugObj->SetCol(m_pParent->GetPlayerSide() ? D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f) : D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pDebugObj->SetTexture(CTexture::TEXTURE_CIRCLE);
	m_pDebugObj->Setbillboard(true);
}
#endif // _DEBUG

//=============================================================================
// ��������
//=============================================================================
CCollision* CCollision::Create(CMove_Object* pParent)
{
	CCollision* pCollision = new CCollision;

	if (pCollision != nullptr)
	{
		pCollision->Init();

		// �e�|�C���^���g�p���̏ꍇ
		if (pParent != nullptr)
		{
			pCollision->m_pParent = pParent;

#ifdef _DEBUG
			// �f�o�b�O�I�u�W�F�N�g�̐ݒ�
			pCollision->SetDebugObj();
#endif // _DEBUG

		}
	}

	return pCollision;
}