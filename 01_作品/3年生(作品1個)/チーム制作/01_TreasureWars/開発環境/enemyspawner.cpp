//=============================================================================
//
// enemyspawner.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "enemyspawner.h"
#include "application.h"
#include "enemymanager.h"
#include "particle_emitter.h"
#include "ghost.h"
#include "pteranodon.h"
#include "alien.h"
#include "game.h"
#include "item.h"
#include "scoreitem.h"

const float CEnemySpawner::First_Pos = -300.0f;	// �X�|�i�[�̏����ʒu
//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CEnemySpawner::CEnemySpawner() : CObjectX(2)
{
}

//=====================================
// �f�X�g���N�^
//=====================================
CEnemySpawner::~CEnemySpawner()
{
}

//============================================================================
// ����������
//============================================================================
HRESULT CEnemySpawner::Init()
{
	// �I�u�W�F�N�gX�̏���������
	CObjectX::Init();

	m_bAppearance = false;
	m_nSpawn_Counter = 0;

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CEnemySpawner::Uninit()
{
	// �I�u�W�F�N�gX�̏I������
	CObjectX::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CEnemySpawner::Update()
{
	// �I�u�W�F�N�gX�̍X�V����
	CObjectX::Update();

	// �o���������Ă��Ȃ��ꍇ
	if (m_bAppearance == false)
	{
		// �o���܂ł̏���
		Appearance();
	}
	// �o�����������ꍇ
	else
	{
		// �����܂ł̎��Ԃ��v�鏈��
		SpawnCount();
	}
}

//============================================================================
// �`�揈��
//============================================================================
void CEnemySpawner::Draw()
{
	// �I�u�W�F�N�gX�̕`�揈��
	CObjectX::Draw();
}

//============================================================================
// �o���܂ł̏���
//============================================================================
void CEnemySpawner::Appearance()
{
	D3DXVECTOR3 pos = GetPos();
 	pos.y += 4.0f;

	// ��ɏグ�����Ɍ��_������̏ꍇ
	if (pos.y >= 0.0f)
	{
		SetAlpha(1.0f);
  		pos.y = 0.0f;
		m_bAppearance = true;

		// �z�Ώo�����p�[�e�B�N��
		CParticleEmitter::Create("PopOre", pos);

		// �ŏ��̐������Ԃ��w��
		m_nSpawn_Counter = 0;
	}

	SetPos(pos);	// �ʒu�̐ݒ���s��
}

//============================================================================
// �����܂ł̎��Ԃ��v�鏈��
//============================================================================
void CEnemySpawner::SpawnCount()
{
	// ���݂̓G�L���������Ȃ��ꍇ
	if (CGame::GetEnemyManager()->GetAllEnemy().empty())
	{
		if (m_nSpawn_Counter++ >= Enemy_Spawn_Timer)
		{
			// ���߂�ꂽ�񐔂�����������
			for (int nCnt = 0; nCnt < m_nMaxSpawn; nCnt++)
			{
				// �G�L�������o�������鏈��
				EnemySpawn();
			}
			m_nSpawn_Counter = 0;
		}
	}
}

//============================================================================
// �G�L�������o�������鏈��
//============================================================================
void CEnemySpawner::EnemySpawn()
{
	// �H��o�����p�[�e�B�N��
	CParticleEmitter::Create("PopGhost", GetPos());

	switch (m_type)
	{
	case CEnemySpawner::EnemySpawn_1:
		// �H��̐���
		CGhost::Create(GetPos());
		break;
	case CEnemySpawner::EnemySpawn_2:
		// �v�e���m�h���̐���
		CPteranodon::Create(GetPos());
		break;
	case CEnemySpawner::EnemySpawn_3:
		// �G�C���A���̐���
		CAlien::Create(GetPos());
		break;
	default:
		break;
	}
}

//============================================================================
// ��e����
//============================================================================
void CEnemySpawner::Damage()
{
	const D3DXVECTOR3 pos = GetPos();

	// �@��p�[�e�B�N��
	std::move(CParticleEmitter::Create("Mine", pos));

	// �̌@SE
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_MINE_MINE);

	switch (m_type)
	{
	case CEnemySpawner::EnemySpawn_1:
		// �Ԃ݂�ǉ�
		AddColor(D3DXCOLOR(0.1f, 0.0f, 0.0f, 0.0f));
		break;
	case CEnemySpawner::EnemySpawn_2:
		// �Ԃ݂�ǉ�
		AddColor(D3DXCOLOR(0.1f, 0.0f, 0.0f, 0.0f));
		break;
	case CEnemySpawner::EnemySpawn_3:
		// �Ԃ݂�ǉ�
		AddColor(D3DXCOLOR(0.02f, 0.0f, 0.0f, 0.0f));
		break;
	default:
		break;
	}

	// �̗͂����炵��0�ɂȂ��������
	if (--m_nLife <= 0)
	{
		// �z�Ώ������p�[�e�B�N��
		CParticleEmitter::Create("Disappear", pos);

		for (int nCnt = 0; nCnt < 5 + (int)m_type * 2; nCnt++)
		{
			// �����_���ɃX�R�A�A�C�e�����΂�T������
			CScoreItem::RandomCreate(pos);
		}

		// �m���ŃA�C�e���̐���
		CItem::Create(D3DXVECTOR3(pos.x, pos.y + 10.0f, pos.z), { 2.0f,2.0f,2.0f });

		// �S�Ă̓G�L�����̉������
		CGame::GetEnemyManager()->ReleaseAllEnemy();

		// �G�L�����X�|�i�[������
		CGame::EnemySpawnerDeath();
	}
}

//============================================================================
// ��������
//============================================================================
CEnemySpawner* CEnemySpawner::Create(const D3DXVECTOR3 pos, const int type)
{
	CEnemySpawner* pEnemySpawner = new CEnemySpawner;

	if (FAILED(pEnemySpawner->Init()))
	{
		return nullptr;
	}
	pEnemySpawner->SetPos(D3DXVECTOR3(pos.x, First_Pos, pos.z));
	pEnemySpawner->m_type = (EnemySpawn_Type)type;

	// ��ޖ��̏����l�̈Ⴂ
	switch (type)
	{
	case CEnemySpawner::EnemySpawn_1:
		pEnemySpawner->SetIndex(ReadObject("Data/model/Enemy_Spawn/tombstone.x"));
		pEnemySpawner->m_nMaxSpawn = 4;
		pEnemySpawner->m_nLife = 3;
		pEnemySpawner->m_fRadius = 50.0f;
		break;
	case CEnemySpawner::EnemySpawn_2:
		pEnemySpawner->SetIndex(ReadObject("Data/model/Enemy_Spawn/ptera_spawn.x"));
		pEnemySpawner->m_nMaxSpawn = 2;
		pEnemySpawner->m_nLife = 4;
		pEnemySpawner->m_fRadius = 50.0f;
		break;
	case CEnemySpawner::EnemySpawn_3:
		pEnemySpawner->SetIndex(ReadObject("Data/model/Enemy_Spawn/ufo.x"));
		pEnemySpawner->m_nMaxSpawn = 5;
		pEnemySpawner->m_nLife = 5;
		pEnemySpawner->m_fRadius = 151.0f;
		break;
	default:
		break;
	}
	pEnemySpawner->SetAlpha(0.5f);

	return pEnemySpawner;
}