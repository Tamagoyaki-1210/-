//=============================================================================
//
// ghost.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "alien.h"
#include "application.h"
#include "game.h"
#include "charmanager.h"
#include "enemymanager.h"

const float CAlien::Alien_Roaming_Speed = 0.3f;			// �G�C���A���̜p�j���x
const float CAlien::Alien_Search_Speed = 0.3f;			// �G�C���A���̒T�����x
const float CAlien::Alien_Roaming_Clear = 0.5f;			// �G�C���A���̜p�j�����x
const float CAlien::Alien_Radius_Collision = 73.0f;		// �����蔻��̃T�C�Y(���a)
const float CAlien::Alien_Radius_Search = 251.0f;		// �����̃T�C�Y(���a)
const float CAlien::Alien_Roaming_Length = 461.0f;		// �p�j���鋗��
const float CAlien::Alien_Clear_Speed = 0.05f;			// �����ɂȂ鑬�x

														//=====================================
														// �f�t�H���g�R���X�g���N�^
														//=====================================
CAlien::CAlien()
{
	SetKnockBack(Alien_KnockBack_Power);
	SetRadius(Alien_Radius_Collision);
	SetSpeed(Alien_Roaming_Speed);
	m_Roaming = ROAMING_NONE;
}

//=====================================
// �f�X�g���N�^
//=====================================
CAlien::~CAlien()
{

}

//============================================================================
// ����������
//============================================================================
HRESULT CAlien::Init()
{
	CEnemy::Init();

	// ���������������_���ɂ���
	SetRotDest({ 0.0f, ((rand() % 628) - 314) / 100.0f, 0.0f });

	// �����̊m���ŉ�]������I��
	m_bClockwise = rand() % 2 == 0;

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CAlien::Uninit()
{
	CEnemy::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CAlien::Update()
{
	// ���[�V�����ԍ��̐ݒ�
	ChangeMotion();

	// ���[�V����
	Motion();

	// �Q�[�����J�n����Ă���ꍇ
	if (CApplication::GetGameStart())
	{
		AI();
		NormalizeRot();
		CEnemy::Update();
	}
}

//============================================================================
// �`�揈��
//============================================================================
void CAlien::Draw()
{
	CEnemy::Draw();
}

//============================================================================
// ���[�V�����ύX
//============================================================================
void CAlien::ChangeMotion()
{
	// ���݂̃��[�V����
	const int nCuttentMotion = GetCurrentMotion();
	const int nMotion = GetMotion();

	// ���݂̃��[�V��������ς�����ꍇ
	if (nCuttentMotion != nMotion)
	{
		// ���݃��[�V�����̏I������
		switch (nCuttentMotion)
		{
		case ALIEN_MOTION_NEUTRAL:
			break;
		case ALIEN_MOTION_WALK:
			m_Roaming = ROAMING_NONE;
			AddRotDest({ 0.0f, D3DX_PI, 0.0f });
			break;
		default:
			break;
		}

		// ���݃��[�V�����̊J�n����
		switch (nMotion)
		{
		case ALIEN_MOTION_NEUTRAL:
			SetSpeed(Alien_Roaming_Speed);
			// �����̊m���ŉ�]������I��
			m_bClockwise = rand() % 2 == 0;
			break;
		case ALIEN_MOTION_WALK:
			SetSpeed(Alien_Search_Speed);
			break;
		default:
			break;
		}

		CEnemy::ChangeMotion();
	}
}

//============================================================================
// AI����
//============================================================================
void CAlien::AI()
{
	Search();
}

//============================================================================
// �T������
//============================================================================
void CAlien::Search()
{
	CCharacter* pNearChara = nullptr;	// ��������ԋ߂��L�����N�^�[�̃|�C���^
	float fNearDistance = 0.0f;

	// ��ԋ������߂��L�����N�^�[��T��
	for (int index = 0; index < CCharManager::Max_Character; index++)
	{
		// �L�����N�^�[�̃|�C���^���擾
		CCharacter* pChara = CApplication::GetCharManager()->GetChara(index);

		// �g�p���̈��S�n�тɂ��Ȃ��L������ǂݍ���
		if (pChara != nullptr)
		{
			// �ʒu���m�̋����̌v�Z�p�ϐ�
			D3DXVECTOR2 distance = { pChara->GetPos().x - GetPos().x, pChara->GetPos().z - GetPos().z };
			float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y));

			// �������������ꍇ�A���͍ŏ��̏����̏ꍇ�m�ۂ���
			if (fNearDistance > fDistance || fNearDistance == 0)
			{
				fNearDistance = fDistance;
				pNearChara = pChara;
			}
		}
	}

	// �L�����N�^�[����l���g�p����Ă��Ȃ��ꍇ
	if (pNearChara == nullptr)
	{
		SetMotion(ALIEN_MOTION_NEUTRAL);
		Roaming();
		return;
	}

	// ���G�͈͂ɓ������ꍇ��
	if (CollisionCircle(GetPos(), Alien_Radius_Search, pNearChara->GetPos(), pNearChara->GetRadius()))
	{
		// �^�[�Q�b�g�����S�n�тɂ���ꍇ�I������
		if (pNearChara->GetSafe() == true)
		{
			Death();
			return;
		}
		// �ړI�ʒu�Ǝ��g�̈ʒu�̍���
		D3DXVECTOR2 distance = { pNearChara->GetPos().x - GetPos().x, pNearChara->GetPos().z - GetPos().z };

		// �����������֌���
		float fAngle = (float)atan2(-distance.x, -distance.y);

		// �L�����N�^�[�̕�������
		SetRotDest({ 0.0f, fAngle, 0.0f });

		SetMotion(ALIEN_MOTION_WALK);
		m_Roaming = ROAMING_NONE;

		float valNormalize = 0.0f;

		D3DXVECTOR2 move = { 0.0f, 0.0f };
		D3DXVec2Normalize(&move, &distance);
		AddMove({ move.x, 0.0f, move.y });

		// ���f���̃A���t�@�l�̐ݒ�
		if (m_fAlpha != 1.0f)
		{
			// �s�����ɂ���
			m_fAlpha < 1.0f ? m_fAlpha += Alien_Clear_Speed : m_fAlpha = 1.0f;

			// ���g�̑S�Ẵ��f���ɃA���t�@�l��ݒ肷��
			for (auto pModel : GetModel()) pModel->SetAlpha(m_fAlpha);
		}
		// �����蔻���ǂݍ���
		CollEnemy();
	}
	// �����͈͂ɒN�����Ȃ��ꍇ
	else
	{
		// �p�j������
		SetMotion(ALIEN_MOTION_NEUTRAL);
		Roaming();
	}
}

//============================================================================
// �p�j����
//============================================================================
void CAlien::Roaming()
{
	// ���g�̈ʒu���擾
	const D3DXVECTOR3 pos = GetPos();

	// ��Ɍ�����
	if (m_anchorPos.x - Alien_Roaming_Length > pos.x && (m_Roaming == (m_bClockwise ? ROAMING_LEFT : ROAMING_RIGHT) || m_Roaming == ROAMING_NONE))
	{
		SetRotDest({ 0.0f, m_bClockwise ? D3DX_PI : 0.0f, 0.0f });
		m_Roaming = ROAMING_UP;
	}
	// �E�Ɍ�����
	if (m_anchorPos.z + Alien_Roaming_Length < pos.z && (m_Roaming == (m_bClockwise ? ROAMING_UP : ROAMING_BOTTOM) || m_Roaming == ROAMING_NONE))
	{
		SetRotDest({ 0.0f, m_bClockwise ? -D3DX_PI / 2 : D3DX_PI / 2, 0.0f });
		m_Roaming = ROAMING_RIGHT;
	}
	// ���Ɍ�����
	if (m_anchorPos.x + Alien_Roaming_Length < pos.x && (m_Roaming == (m_bClockwise ? ROAMING_RIGHT : ROAMING_LEFT) || m_Roaming == ROAMING_NONE))
	{
		SetRotDest({ 0.0f, m_bClockwise ? 0.0f : D3DX_PI, 0.0f });
		m_Roaming = ROAMING_BOTTOM;
	}
	// ���Ɍ�����
	if (m_anchorPos.z - Alien_Roaming_Length > pos.z && (m_Roaming == (m_bClockwise ? ROAMING_BOTTOM : ROAMING_UP) || m_Roaming == ROAMING_NONE))
	{
		SetRotDest({ 0.0f, m_bClockwise ? D3DX_PI / 2 : -D3DX_PI / 2, 0.0f });
		m_Roaming = ROAMING_LEFT;
	}

	// �p�x�̕����ɐ^�������ړ�
	AddMove({ -sinf(GetRot().y), 0.0f, -cosf(GetRot().y) });

	// ���f���̃A���t�@�l�̐ݒ�
	if (m_fAlpha != Alien_Roaming_Clear)
	{
		// �����ɂ���
		m_fAlpha > Alien_Roaming_Clear ? m_fAlpha -= Alien_Clear_Speed : m_fAlpha = Alien_Roaming_Clear;

		// ���g�̑S�Ẵ��f���ɃA���t�@�l��ݒ肷��
		for (auto pModel : GetModel()) pModel->SetAlpha(m_fAlpha);
	}
}

//============================================================================
// �����蔻��̏���
//============================================================================
void CAlien::CollEnemy()
{
	// ���g�̈ʒu���擾
	const D3DXVECTOR3 pos = GetPos();

	// �S�Ă̓G�L�����̃|�C���^��ǂݍ���
	for (auto pEnemy : CGame::GetEnemyManager()->GetAllEnemy())
	{
		//�|�C���^�̉��
		if (pEnemy != nullptr && pEnemy != this)
		{
			const D3DXVECTOR3 atherPos = pEnemy->GetPos();

			//�Փˌ��m
			if (CollisionCircle(pos, GetRadius(), atherPos, pEnemy->GetRadius()))
			{
				//�x�N�g���̎Z�o
				D3DXVECTOR3 vector = pos - atherPos;
				D3DXVec3Normalize(&vector, &vector);

				//�����̒���(�G�C���A�� + �G�L����)
				AddMove(vector);

				// 1�C������ǂݍ���ŏI��
				break;
			}
		}
	}
}

//============================================================================
// ��������
//============================================================================
CAlien* CAlien::Create(const D3DXVECTOR3 pos, const float alpha)
{
	CAlien* pAlien = new CAlien;

	pAlien->LoadFileEnemy("Data/model/Motion_Enemy/Motion_Alien/motion_alien.txt");

	if (FAILED(pAlien->Init()))
	{
		return nullptr;
	}

	pAlien->SetPos(pos);
	pAlien->m_anchorPos = pos;
	pAlien->m_fAlpha = alpha;

	// ���f����a�l�ݒ�
	for (auto pModel : pAlien->GetModel())
	{
		pModel->SetAlpha(alpha);

		// �G�C���A���̉e��`�悵�Ȃ�
		pModel->SetShadow(false);
	}

	return pAlien;
}