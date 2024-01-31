//=============================================================================
//
// ghost.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "ghost.h"
#include "application.h"
#include "game.h"
#include "charmanager.h"
#include "enemymanager.h"

const float CGhost::Ghost_Roaming_Speed = 0.4f;			// �S�[�X�g�̜p�j���x
const float CGhost::Ghost_Search_Speed = 0.5f;			// �S�[�X�g�̒T�����x
const float CGhost::Ghost_Roaming_Clear = 0.5f;			// �S�[�X�g�̜p�j�����x
const float CGhost::Ghost_Radius_Collision = 45.0f;		// �����蔻��̃T�C�Y(���a)
const float CGhost::Ghost_Radius_Search = 250.0f;		// �����̃T�C�Y(���a)
const float CGhost::Ghost_Roaming_Length = 450.0f;		// �p�j���鋗��
const float CGhost::Ghost_Clear_Speed = 0.05f;			// �����ɂȂ鑬�x

//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CGhost::CGhost()
{
	SetKnockBack(Ghost_KnockBack_Power);
	SetRadius(Ghost_Radius_Collision);
	SetSpeed(Ghost_Roaming_Speed);
	m_Roaming = ROAMING_NONE;
}

//=====================================
// �f�X�g���N�^
//=====================================
CGhost::~CGhost()
{

}

//============================================================================
// ����������
//============================================================================
HRESULT CGhost::Init()
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
void CGhost::Uninit()
{
	CEnemy::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CGhost::Update()
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
void CGhost::Draw()
{
	CEnemy::Draw();
}

//============================================================================
// ���[�V�����ύX
//============================================================================
void CGhost::ChangeMotion()
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
		case GHOST_MOTION_NEUTRAL:
			break;
		case GHOST_MOTION_WALK:
			m_Roaming = ROAMING_NONE;
			AddRotDest({ 0.0f, D3DX_PI, 0.0f });
			break;
		default:
			break;
		}

		// ���݃��[�V�����̊J�n����
		switch (nMotion)
		{
		case GHOST_MOTION_NEUTRAL:
			SetSpeed(Ghost_Roaming_Speed);
			// �����̊m���ŉ�]������I��
			m_bClockwise = rand() % 2 == 0;
			break;
		case GHOST_MOTION_WALK:
			SetSpeed(Ghost_Search_Speed);
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
void CGhost::AI()
{
	Search();
}

//============================================================================
// �T������
//============================================================================
void CGhost::Search()
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
		SetMotion(GHOST_MOTION_NEUTRAL);
		Roaming();
		return;
	}

	// ���G�͈͂ɓ������ꍇ��
	if (CollisionCircle(GetPos(), Ghost_Radius_Search, pNearChara->GetPos(), pNearChara->GetRadius()))
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

		SetMotion(GHOST_MOTION_WALK);
		m_Roaming = ROAMING_NONE;

		float valNormalize = 0.0f;

		D3DXVECTOR2 move = { 0.0f, 0.0f };
		D3DXVec2Normalize(&move, &distance);
		AddMove({ move.x, 0.0f, move.y });

		// ���f���̃A���t�@�l�̐ݒ�
		if (m_fAlpha != 1.0f)
		{
			// �s�����ɂ���
			m_fAlpha < 1.0f ? m_fAlpha += Ghost_Clear_Speed : m_fAlpha = 1.0f;

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
		SetMotion(GHOST_MOTION_NEUTRAL);
		Roaming();
	}
}

//============================================================================
// �p�j����
//============================================================================
void CGhost::Roaming()
{
	// ���g�̈ʒu���擾
	const D3DXVECTOR3 pos = GetPos();

	// ��Ɍ�����
	if (m_anchorPos.x - Ghost_Roaming_Length > pos.x && (m_Roaming == (m_bClockwise ? ROAMING_LEFT : ROAMING_RIGHT) || m_Roaming == ROAMING_NONE))
	{
		SetRotDest({ 0.0f, m_bClockwise ? D3DX_PI : 0.0f, 0.0f });
		m_Roaming = ROAMING_UP;
	}
	// �E�Ɍ�����
	if (m_anchorPos.z + Ghost_Roaming_Length < pos.z && (m_Roaming == (m_bClockwise ? ROAMING_UP : ROAMING_BOTTOM) || m_Roaming == ROAMING_NONE))
	{
		SetRotDest({ 0.0f, m_bClockwise ? -D3DX_PI / 2 : D3DX_PI / 2, 0.0f });
		m_Roaming = ROAMING_RIGHT;
	}
	// ���Ɍ�����
	if (m_anchorPos.x + Ghost_Roaming_Length < pos.x && (m_Roaming == (m_bClockwise ? ROAMING_RIGHT : ROAMING_LEFT) || m_Roaming == ROAMING_NONE))
	{
		SetRotDest({ 0.0f, m_bClockwise ? 0.0f : D3DX_PI, 0.0f });
		m_Roaming = ROAMING_BOTTOM;
	}
	// ���Ɍ�����
	if (m_anchorPos.z - Ghost_Roaming_Length > pos.z && (m_Roaming == (m_bClockwise ? ROAMING_BOTTOM : ROAMING_UP) || m_Roaming == ROAMING_NONE))
	{
		SetRotDest({ 0.0f, m_bClockwise ? D3DX_PI / 2 : -D3DX_PI / 2, 0.0f });
		m_Roaming = ROAMING_LEFT;
	}

	// �p�x�̕����ɐ^�������ړ�
	AddMove({ -sinf(GetRot().y), 0.0f, -cosf(GetRot().y) });

	// ���f���̃A���t�@�l�̐ݒ�
	if (m_fAlpha != Ghost_Roaming_Clear)
	{
		// �����ɂ���
		m_fAlpha > Ghost_Roaming_Clear ? m_fAlpha -= Ghost_Clear_Speed : m_fAlpha = Ghost_Roaming_Clear;

		// ���g�̑S�Ẵ��f���ɃA���t�@�l��ݒ肷��
		for (auto pModel : GetModel()) pModel->SetAlpha(m_fAlpha);
	}
}

//============================================================================
// �����蔻��̏���
//============================================================================
void CGhost::CollEnemy()
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

				//�����̒���(�S�[�X�g + �G�L����)
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
CGhost* CGhost::Create(const D3DXVECTOR3 pos, const float alpha)
{
	CGhost* pGhost = new CGhost;

	pGhost->LoadFileEnemy("Data/model/Motion_Enemy/Motion_Ghost/motion_ghost.txt");

	if (FAILED(pGhost->Init()))
	{
		return nullptr;
	}

	pGhost->SetPos(pos);
	pGhost->m_anchorPos = pos;
	pGhost->m_fAlpha = alpha;

	// ���f����a�l�ݒ�
	for (auto pModel : pGhost->GetModel())
	{
		pModel->SetAlpha(alpha);

		// �S�[�X�g�̉e�͕`�悵�Ȃ�
		pModel->SetShadow(false);
	}

	return pGhost;
}