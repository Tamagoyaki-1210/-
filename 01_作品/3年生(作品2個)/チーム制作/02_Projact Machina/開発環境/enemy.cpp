//=============================================================================
//
// enemy.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "enemy.h"
#include "enemy_manager.h"
#include "application.h"
#include "tutorial.h"
#include "game.h"
#include "player.h"
#include "player_manager.h"

//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CEnemy::CEnemy()
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	// �������Ɏ��g�̃|�C���^��G�L�����}�l�[�W���[�ɐݒ�
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		CTutorial::GetEnemyManager()->SetEnemy(this);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		CGame::GetEnemyManager()->SetEnemy(this);
	}
}

//=====================================
// �f�X�g���N�^
//=====================================
CEnemy::~CEnemy()
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	// �}�l�[�W���[���g�p���̏ꍇ�A������Ɏ��g�̃|�C���^��z�񂩂珜�O����
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		CEnemyManager* pManager = CTutorial::GetEnemyManager();
		if(pManager != nullptr) pManager->DestroyEnemy(this);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		CEnemyManager* pManager = CGame::GetEnemyManager();
		if (pManager != nullptr) pManager->DestroyEnemy(this);
	}
}

//============================================================================
// ����������
//============================================================================
HRESULT CEnemy::Init()
{
	// �G���ɐݒ�
	SetPlayerSide(false);

	// �T�C�Y�̐ݒ�
	SetSize({ GetRadius(),GetRadius(),GetRadius() });

	// �����蔻��̐���
	SetCollision();

	CCharacter::Init();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CEnemy::Uninit()
{
	CCharacter::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CEnemy::Update()
{
	// �L�����N�^�[�̍X�V
	CCharacter::Update();

	// �ړ�����
	Move();
}

//============================================================================
// �`�揈��
//============================================================================
void CEnemy::Draw()
{
	CCharacter::Draw();
}

//============================================================================
// �ړ�����
//============================================================================
void CEnemy::Move()
{
	CPlayer* pPlayer = nullptr;

	// �v���C���[�̏��
	pPlayer = CGame::GetPlayerManager()->GetPlayer(0);

	// �ʒu�̊i�[��
	D3DXVECTOR3 Player_Pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 Enemy_Pos = { 0.0f,0.0f,0.0f };

	if (pPlayer != nullptr)
	{
		// �ʒu�̎擾
		Player_Pos = pPlayer->GetPos();
		Enemy_Pos = GetPos();
	}

	// �v���C���[����G�̋���
	D3DXVECTOR3 Vec = Player_Pos - Enemy_Pos;

	// �����̎Z�o
	m_fDistance = sqrtf(Vec.x * Vec.x + Vec.z * Vec.z);

	// �p�x�̎Z�o
	m_fAngle = atan2(Vec.x, Vec.z);
}

//============================================================================
// ��e����
//============================================================================
void CEnemy::Hit(CMove_Object* pHit)
{
	// �q�b�g�����ړ��I�u�W�F�N�g���g�p���̏ꍇ
	if (pHit != nullptr && GetPlayerSide() != pHit->GetPlayerSide())
	{
		// �q�b�g�����ړ��I�u�W�F�N�g�̃^�O���擾
		TAG tag = pHit->GetTag();

		// �^�O�����ނ�I��
		switch (tag)
		{
		case TAG_CHARACTER:
			break;
		case TAG_BULLET:
			// �e�̃_���[�W��Ԃ�
			Damage(pHit->GetPower());

			// �_���[�W
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_BULLET_DAMAGE);
			break;
		case TAG_ATTACK:
			// �e�̃_���[�W��Ԃ�
			Damage(pHit->GetPower());

			// ���G��Ԃ�t�^����
			SetCollisionNoneHit(true);

			// �_���[�W
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE);
			break;
		case TAG_EXPLOSION:
			// �����̃_���[�W��Ԃ�
			Damage(pHit->GetPower());

			// ���G��Ԃ�t�^����
			SetCollisionNoneHit(true);
			break;
		case TAG_MAP_OBJECT:
			break;
		default:
			break;
		}
	}
}

//============================================================================
// �j�󏈗�
//============================================================================
void CEnemy::Destroy()
{
	CCharacter::Destroy();
}