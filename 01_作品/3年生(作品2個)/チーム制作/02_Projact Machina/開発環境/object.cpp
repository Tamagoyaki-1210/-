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
#include <assert.h>

//**********************************************************************************************
// �ÓI�����o�ϐ��̐錾
//**********************************************************************************************
CObject *CObject::m_pTop[] = {};
CObject *CObject::m_pCurrent[] = {};

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CObject::CObject(const PRIORITY priority)
{
	// Top��null�̏ꍇ
	if (m_pTop[priority] == nullptr)
	{
		m_pTop[priority] = this;
		m_pTop[priority]->m_pNext = nullptr;
		m_pTop[priority]->m_pPrev = nullptr;
	}

	// Current��null����Ȃ��ꍇ
	if (m_pCurrent[priority] != nullptr)
	{
		m_pCurrent[priority]->m_pNext = this;
		m_pCurrent[priority]->m_pNext->m_pNext = nullptr;
		m_pCurrent[priority]->m_pNext->m_pPrev = m_pCurrent[priority];
	}

	m_pCurrent[priority] = this;

	// �v���C�I���e�B�[�̐ݒ�
	m_nPriority = priority;

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
	for (int nPri = 0; nPri < PRIORITY_MAX; nPri++)
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
	for (int nPri = 0; nPri < PRIORITY_MAX; nPri++)
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
	for (int nPri = 0; nPri < PRIORITY_MAX; nPri++)
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
// �I�u�W�F�N�g�̉��
//==============================================================================================
void CObject::Death(const int nPriority)
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
// �I�u�W�F�N�g���p���������̂̓����蔻��
//==============================================================================================
bool CObject::Sphere_Collision(const D3DXVECTOR3 pos, const float radius, const D3DXVECTOR3 otherPos, const float otherRadius)
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
