//=============================================================================
//
// character.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "character.h"
#include "application.h"
#include "game.h"
#include "meshfield.h"
#include "tutorial.h"
#include "gauge_manager.h"
#include "debugProc.h"
#include "camera.h"

const float CCharacter::CHARACTER_FIRST_MOVE_SPEED = 10.0f;
const float CCharacter::CHARACTER_ROT_SPEED = 0.1f;
const float CCharacter::CHARACTER_MOVE_INERTIE = 0.2f;
const float CCharacter::CHARACTER_GRAVITY = 0.25f;

//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CCharacter::CCharacter(const CObject::PRIORITY priority) : CMove_Object(priority)
{
	// �^�O�̐ݒ�
	SetTag(TAG_CHARACTER);

	// �����蔻��̃^�C�v
	SetCollision_Type(CMove_Object::COLLISION_TYPE_SHERER);

	m_fSpeed = CHARACTER_FIRST_MOVE_SPEED;
	m_fRotSpeed = CHARACTER_ROT_SPEED;
	SetGround(false);
	m_bBoost = false;
}

//=====================================
// �f�X�g���N�^
//=====================================
CCharacter::~CCharacter()
{
}

//============================================================================
// ����������
//============================================================================
HRESULT CCharacter::Init()
{
	m_move = { 0.0f, 0.0f, 0.0f };

	SetPosOld(GetPos());

	CMove_Object::Init();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CCharacter::Uninit()
{
	// �p�[�c�̏I������
	for (int nCnt = 0; nCnt < (int)m_Parts.size(); nCnt++)
	{
		if (m_Parts[nCnt] != nullptr)
		{
			m_Parts[nCnt]->Uninit();
			m_Parts[nCnt] = nullptr;
		}
	}

	// �Q�[�W���g�p���̏ꍇ
	if (m_pGaugeManager != nullptr)
	{
		m_pGaugeManager->Uninit();
		m_pGaugeManager = nullptr;
	}

	//if (GetLandObj())
	//{
	//	for (int nCnt = 0; nCnt < GetOnObj(0)->GetOnObjCnt(); nCnt++)
	//	{
	//		GetOnObj(0)->SetOnObj(nullptr, nCnt);
	//	}
	//	GetOnObj(0)->SetOnObjCnt(0);
	//}

	m_Parts.clear();

	CMove_Object::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CCharacter::Update()
{
	CMove_Object::Update();

	SetPosOld(GetPos());

	// �ړ��ʂ̏���
	Move();

	// �p�x�̐��K��
	NormalizeRot();

	// ���̓����蔻��
	FieldCollision();

	// �������
	Invincible();
}

//============================================================================
// �`�揈��
//============================================================================
void CCharacter::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//==============================================================================================
// �ړ�����
//==============================================================================================
void CCharacter::Move()
{
	if (!m_bAvoidance)
		// �ړ��ʂ��X�V(����������)
		m_move -= m_move * CHARACTER_MOVE_INERTIE;
	else
	{
		m_move -= m_move * 0.05f;

		m_nAvoidance_Count--;
		if (m_nAvoidance_Count <= 0)
			m_bAvoidance = false;
	}

	// �ʒu�X�V
	AddPos(m_move * m_fSpeed);
}

//==============================================================================================
// �_���[�W����(�^�_���[�W)
//==============================================================================================
void CCharacter::Damage(const int value)
{
	// �̗͂��c���Ă���ꍇ
	if (m_nLife > 0)
	{
		// �x�[�X�̗̑͂̐ݒ�
		if (m_pGaugeManager != nullptr
			&& m_pGaugeManager->GetBeaseLife() == 0)
		{
			m_pGaugeManager->SetBeaseLife(m_nLife);
		}

		// �̗� - �^�_���[�W
		m_nLife -= value;
		m_fDamageReceived += value;

		// �̗̓Q�[�W�̑���
		if (m_pGaugeManager != nullptr)
		{
			m_pGaugeManager->SetLife(m_nLife);
			m_pGaugeManager->Fluctuation();
		}

		// �̗̓`�F�b�N
		if (m_nLife <= 0)
		{
			// �̗͂�0�ɂ���
			m_nLife = 0;

			// ���g��j�󂷂鏈��
			Destroy();
		}
	}
}

//==============================================================================================
// �񕜏���(�񕜗�)
//==============================================================================================
void CCharacter::Recovery(const int value)
{
	// �̗� + �񕜗�
	m_nLife += value;

	// �̗̓`�F�b�N
	if (m_nLife > m_nMaxLife)
	{
		// �̗͂̍ő�l�𒴂��Ȃ��悤�ɂ���
		m_nLife = m_nMaxLife;
	}

	// �̗̓Q�[�W�̑���
	if (m_pGaugeManager != nullptr)
	{
		m_pGaugeManager->SetLife(m_nLife);
		m_pGaugeManager->Fluctuation();
	}
}

//============================================================================
// ���g��j�󂷂鏈��
//============================================================================
void CCharacter::Destroy()
{
	// ���g�̏I������
	Uninit();
}

//============================================================================
// ���n����
//============================================================================
void CCharacter::Landing(const D3DXVECTOR3 pos)
{
	// �ʒu��ݒ肷��
	SetPos(pos);

	// ���n�����^�ɂ���
	SetGround(true);
}

//============================================================================
// ���̓����蔻��
//============================================================================
void CCharacter::FieldCollision()
{
		// ���݂̈ʒu��萔�Ƃ��Ď擾
		const D3DXVECTOR3 pos = CCharacter::GetPos();

		CMeshField* pMeshField = nullptr;
		float a = 0.0f;

		if (CApplication::GetModeType() == CApplication::MODE_GAME)
			pMeshField = CGame::GetMeshField();
		else if (CApplication::GetModeType() == CApplication::MODE_TUTORIAL)
			pMeshField = CTutorial::GetMeshField();

		if (pMeshField != nullptr)
		{
			// ���̓����蔻�肩�獂����萔�Ƃ��Ď擾
			if (pMeshField != nullptr)
				a = pMeshField->MeshCollision(pos);
		}
		// �ڒn���Ă���ꍇ
		if (GetGround())
		{
			// �v���C���[�̍�����ݒ�
			CCharacter::SetPos({ pos.x, a, pos.z });
		}
		// �ڒn���Ă��Ȃ��ꍇ
		else
		{
			if (!m_bAvoidance)
			{
				if (CApplication::GetModeType() == CApplication::MODE_GAME)
				{
					CCamera *pCamera = CApplication::GetCamera();

					if (!pCamera->GetOpening())
						// ���b�V���t�B�[���h�̏�ɂ���ꍇ�͏d�͂�������
						CCharacter::AddMove({ 0.0f, -CHARACTER_GRAVITY, 0.0f });
					else
						// �I�[�v�j���O���̏d��
						CCharacter::AddMove({ 0.0f, -CHARACTER_GRAVITY * 5, 0.0f });
				}

				// ���b�V���t�B�[���h��艺�̈ʒu�ɂ���ꍇ
				if (a >= pos.y)
				{
					// ���n������ǂݍ���
					Landing({ pos.x, a, pos.z });
				}
			}
		}

		float y = GetObjY();

		if (!GetGround() && GetObjXZ() && y > pos.y)
		{
			// ���n������ǂݍ���
			Landing({ pos.x,y,pos.z });
		}
		else if (GetLandObj())
		{
			// �v���C���[�̍�����ݒ�
			CCharacter::SetPos({ pos.x, GetObjY(), pos.z });

			// �}�b�v�I�u�W�F�N�g�̏�ɂ���ꍇ�͏d�͂������Ȃ�
			CCharacter::SetMove({ 0.0f, 0.0f, 0.0f });
		}
}

//============================================================================
// �������
//============================================================================
void CCharacter::Invincible()
{
	// ���G��Ԃ̏ꍇ
	if (GetCollisionNoneHit() == true)
	{
		// �S�p�[�c��_�ł����鏈��
		for (auto pAllParts : GetAllParts()) for (auto pParts : pAllParts.second->GetModelAll())
			pParts->SetDrawFlag(m_nInvincible_Counter % CHARACTER_INVINCIBLE_SPEED * 2 < CHARACTER_INVINCIBLE_SPEED);
		
		// �ő厞�ԂɒB�����ꍇ
		if (++m_nInvincible_Counter > CHARACTER_INVINCIBLE_TIMER)
		{
			SetCollisionNoneHit(false);
			m_nInvincible_Counter = 0;

			// �S�p�[�c���̕`������ɖ߂�����
			for (auto pAllParts : GetAllParts()) for (auto pParts : pAllParts.second->GetModelAll())
				pParts->SetDrawFlag(true);
		}
	}
}

//==============================================================================================
// ��]�����֐U��������鏈��
//==============================================================================================
void CCharacter::Rotation()
{
	// �ړI�̊p�x�ɂ���
	m_rot.y += (m_rotDest.y - m_rot.y) * m_fRotSpeed;
}

//==============================================================================================
// �p�x�̐��K��
//==============================================================================================
void CCharacter::NormalizeRot()
{
	// �p�x�̐��K��
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}

	// �ړI�̊p�x�̐��K��
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rotDest.y += D3DX_PI * 2;
	}

	m_rot.x = 0.0f;
	m_rot.z = 0.0f;
}

//==============================================================================================
// �ő�̗͂̐ݒ菈��
//==============================================================================================
void CCharacter::SetLife(const int life)
{
	// �ő�̗͂��ݒ肳���̗͂�菭�Ȃ��ꍇ
	if (m_nMaxLife < life)
	{
		// ���݂̗̑͂�����܂łɐݒ肷��
		m_nLife = m_nMaxLife;
	}
	// �ő�̗͈ȓ��̏ꍇ
	else
	{
		// �̗͂̐ݒ�
		m_nLife = life;
	}
}

//==============================================================================================
// �ő�̗͂̐ݒ菈��
//==============================================================================================
void CCharacter::SetMaxLife(const int maxlife)
{
	// �̗͂��ݒ肳���ő�̗͂�葽���ꍇ
	if (m_nLife > maxlife)
	{
		// ���݂̗̑͂�������
		m_nLife = maxlife;
	}
	// �ő�̗͂̐ݒ�
	m_nMaxLife = maxlife;

	// �̗͂��ݒ肳��Ă��Ȃ��ꍇ
	if (m_nLife == 0)
	{
		m_nLife = m_nMaxLife;
	}

	// �Q�[�W���g�p���̏ꍇ
	if (m_pGaugeManager != nullptr)
	{
		m_pGaugeManager->SetBeaseLife(m_nMaxLife);
		m_pGaugeManager->Fluctuation();
	}
}

//==============================================================================================
// �p�[�c�̐ݒ菈��
//==============================================================================================
void CCharacter::SetParts(const int charaParts, const int partsIndex, CMotion::MOTION motion)
{
	// �p�[�c�����g�p�̏ꍇ
	if (m_Parts[charaParts] == nullptr)
	{
		m_Parts[charaParts] = CParts::Create(GetPos(), partsIndex, motion, this);
	}
}