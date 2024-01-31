//==============================================================================================
//
// �I�u�W�F�N�g�@�@�@object.cpp
// tutida ryousei
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"object.h"

//**********************************************************************************************
// �ÓI�����o�ϐ��̐錾
//**********************************************************************************************
CObject *CObject::m_pTop[] = {};
CObject *CObject::m_pCurrent[] = {};

const float CObject::AttackLength_Decreace = 50.0f;	// �U���˒��̌���
//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CObject::CObject(int nPriority)
{
	// Top��null�̏ꍇ
	if (m_pTop[nPriority] == nullptr)
	{
		m_pTop[nPriority] = this;
		m_pTop[nPriority]->m_pNext = nullptr;
		m_pTop[nPriority]->m_pPrev = nullptr;
	}

	// Current��null����Ȃ��ꍇ
	if (m_pCurrent[nPriority] != nullptr)
	{
		m_pCurrent[nPriority]->m_pNext = this;
		m_pCurrent[nPriority]->m_pNext->m_pNext = nullptr;
		m_pCurrent[nPriority]->m_pNext->m_pPrev = m_pCurrent[nPriority];
	}

	m_pCurrent[nPriority] = this;

	// �v���C�I���e�B�[�̐ݒ�
	m_nPriority = nPriority;

	m_bDraw = true;
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CObject::~CObject()
{
}

//==============================================================================================
// �S�̂̉������
//==============================================================================================
void CObject::ReleaseAll()
{
	for (int nPri = 0; nPri < m_nMax_Pri; nPri++)
	{
		if (m_pTop[nPri] != nullptr)
		{
			CObject *pObject = m_pTop[nPri];

			while (pObject != nullptr)
			{
				CObject *pObjectNext = pObject->m_pNext;

				pObject->Uninit();
				pObject->Death(nPri);

				pObject = pObjectNext;
			}
		}
	}
}

//==============================================================================================
// �S�̂̍X�V����
//==============================================================================================
void CObject::UpdateAll()
{
	for (int nPri = 0; nPri < m_nMax_Pri; nPri++)
	{
		if (m_pTop[nPri] != nullptr)
		{
			CObject *pObject = m_pTop[nPri];

			while (pObject != nullptr)
			{
				CObject *pObjectNext = pObject->m_pNext;

				// ���S�t���O�̊m�F
				if (!pObject->m_bDeath)
				{
					pObject->Update();
				}

				pObject = pObjectNext;
			}

			pObject = m_pTop[nPri];

			while (pObject != nullptr)
			{
				CObject *pObjectNext = pObject->m_pNext;

				// ���S�t���O�̊m�F
				if (pObject->m_bDeath)
				{
					pObject->Death(nPri);
				}

				pObject = pObjectNext;
			}
		}
	}
}

//==============================================================================================
// �S�̂̕`�揈��
//==============================================================================================
void CObject::DrawAll()
{
	for (int nPri = 0; nPri < m_nMax_Pri; nPri++)
	{
		if (m_pTop[nPri] != nullptr)
		{
			CObject *pObject = m_pTop[nPri];

			while (pObject != nullptr)
			{
				CObject *pObjectNext = pObject->m_pNext;

				if (pObject->GetDrawFlag())
				{
					if (!pObject->m_bDeath)
					{
						pObject->Draw();
					}
				}

				pObject = pObjectNext;
			}
		}
	}
}

//==============================================================================================
// ���S�t���O�𗧂Ă�
//==============================================================================================
void CObject::Release()
{
	this->m_bDeath = true;
}

//==============================================================================================
// �I�u�W�F�N�g�̉��
//==============================================================================================
void CObject::Death(int nPriority)
{
	// �I�u�W�F�N�g(�������g)�����X�g����폜
	// Top��������
	if (this == m_pTop[nPriority])
	{
		m_pTop[nPriority] = m_pTop[nPriority]->m_pNext;

		if (m_pTop[nPriority] != nullptr)
		{
			m_pTop[nPriority]->m_pPrev = nullptr;
		}
		else
		{
			m_pCurrent[nPriority] = nullptr;
		}
	}
	// Current��������
	else if (this == m_pCurrent[nPriority])
	{
		m_pCurrent[nPriority] = m_pCurrent[nPriority]->m_pPrev;
		m_pCurrent[nPriority]->m_pNext = nullptr;
	}
	else
	{
		this->m_pPrev->m_pNext = this->m_pNext;
		this->m_pNext->m_pPrev = this->m_pPrev;
	}

	// �I�u�W�F�N�g(�������g)��j��

	delete this;
}

//==============================================================================================
// ��̈ʒu�̋���
//==============================================================================================
float CObject::Distance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	float Distance = sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.z - pos2.z) * (pos1.z - pos2.z));

	return Distance;
}

//==============================================================================================
// �~���m�̓����蔻��(������ : [�ʒu�A�����蔻��̃T�C�Y(���a)]�A���葤 : [�ʒu�A�����蔻��̃T�C�Y(���a)])
//==============================================================================================
bool CObject::CollisionCircle(const D3DXVECTOR3 pos, const float radius, const D3DXVECTOR3 otherPos, const float otherRadius)
{
	// �ʒu���m�̋����̌v�Z�p�ϐ�
	D3DXVECTOR2 distance = { otherPos.x - pos.x, otherPos.z - pos.z };

	// �����Ƒ傫������v�Z���A�Փ˂��Ă���ꍇ
	return sqrtf((distance.x * distance.x) + (distance.y * distance.y)) <= (radius + otherRadius);
}

//==============================================================================================
// �U���l�̓����蔻��(������ : [�ʒu�A�����A�U���˒��A�U���͈�]�A���葤 : [�ʒu�A�����蔻��̃T�C�Y(���a)])
//==============================================================================================
bool CObject::AttackCollision(const D3DXVECTOR3 pos, const float rot, const float length, const float angle, const D3DXVECTOR3 otherPos, const float otherRadius)
{
	// �U���̌��_
	D3DXVECTOR3 startPos = pos + D3DXVECTOR3(sinf(rot) * AttackLength_Decreace, 0.0f, cosf(rot) * AttackLength_Decreace);

	// �_�Ɛ�̃x�N�g��
	D3DXVECTOR2 vec_fan_to_point = { otherPos.x - startPos.x, otherPos.z - startPos.z };

	// �~(�˒�)�Ɖ~(����̑傫��)�œ������Ă��邩�ǂ���
	if (!CollisionCircle(startPos, length, otherPos, otherRadius)) return false;

	// ���2����������̃x�N�g�������߂�
	D3DXVECTOR2 fan_dir = { -sinf(rot), -cosf(rot) };

	// �~�Ɛ�̃x�N�g���𐳋K��
	D3DXVec2Normalize(&vec_fan_to_point, &vec_fan_to_point);

	// �~�̓��ς��v�Z����
	float dot = (vec_fan_to_point.x * fan_dir.x) + (vec_fan_to_point.y * fan_dir.y);

	// �~�̓��ς���͈͓̔��ɂ��邩���r����
	return cosf(D3DXToRadian(angle * 0.5f)) <= dot;
}

//==============================================================================================
// �w��Pos����w��p�x���ɑΏ�Pos�����݂��邩�ǂ���
//==============================================================================================
bool CObject::CheckInAngle(const D3DXVECTOR3 pos, const float rot, const float angle, const D3DXVECTOR3 otherPos)
{
	// �_�Ɛ�̃x�N�g��
	D3DXVECTOR2 vec_fan_to_point = { otherPos.x - pos.x, otherPos.z - pos.z };

	// ���2����������̃x�N�g�������߂�
	D3DXVECTOR2 fan_dir = { sinf(rot), -cosf(rot) };

	// �~�Ɛ�̃x�N�g���𐳋K��
	D3DXVec2Normalize(&vec_fan_to_point, &vec_fan_to_point);
	D3DXVec2Normalize(&fan_dir, &fan_dir);

	// �~�̓��ς��v�Z����
	float dot = (vec_fan_to_point.x * fan_dir.x) + (vec_fan_to_point.y * fan_dir.y);

	// �~�̓��ς���͈͓̔��ɂ��邩���r����
	return cosf(D3DXToRadian(angle * 0.5f)) <= dot;
}
