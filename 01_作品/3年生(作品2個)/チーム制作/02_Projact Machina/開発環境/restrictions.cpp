//==============================================================================================
//
// �ړ������@�@�@restrictions.cpp
// tutida ryousei
//
//==============================================================================================
#include"restrictions.h"
#include"object3D.h"
#include"debugProc.h"
#include"application.h"
#include"player_manager.h"
#include"enemy_manager.h"
#include"game.h"
#include"tutorial.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CRestrictions::CRestrictions()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CRestrictions::~CRestrictions()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CRestrictions::Init()
{
	// ��������ʒu
	m_pDont_EnterPos[0] = { m_Center.x + m_fScope, m_Size.y, m_Center.z };
	m_pDont_EnterPos[1] = { m_Center.x, m_Size.y, m_Center.z + m_fScope };
	m_pDont_EnterPos[2] = { m_Center.x - m_fScope, m_Size.y, m_Center.z };
	m_pDont_EnterPos[3] = { m_Center.x, m_Size.y, m_Center.z - m_fScope };

	// �[����[�܂ł̕K�v�|���S����
	float num = m_fScope / m_Size.x;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < num * 2; nCnt2++)
		{
			D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

			// �ʒu�̒���
			float fAdj = m_Size.x / 2 * (num * 2 - 1);

			switch (nCnt)
			{
			case 0:
			case 2:
				pos = { m_pDont_EnterPos[nCnt].x,m_Size.y,m_pDont_EnterPos[nCnt].z - fAdj + m_Size.x * nCnt2 };
				break;
			case 1:
			case 3:
				pos = { m_pDont_EnterPos[nCnt].x - fAdj + m_Size.x * nCnt2 ,m_Size.y,m_pDont_EnterPos[nCnt].z };
				break;
			default:
				break;
			}

			m_pDont_Enter[nCnt][nCnt2] = CObject3D::Create(pos, m_Size, PRIORITY_BACK, m_Col);
			m_pDont_Enter[nCnt][nCnt2]->SetRot({ D3DX_PI / 2 - (D3DX_PI / 2 * nCnt),0.0f,0.0f });
			m_pDont_Enter[nCnt][nCnt2]->SetTexture(CTexture::TEXTURE_DONOT_ENTER);
		}
	}

	// �A���t�@�l�̏�����
	m_fAlpha = 0.0f;

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CRestrictions::Uninit()
{
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CRestrictions::Update()
{
	// �ړ�����
	Collision();

	// �_��
	Alpha();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CRestrictions::Draw()
{
}

//==============================================================================================
// ��������
//==============================================================================================
CRestrictions *CRestrictions::Create(const D3DXVECTOR3 center, const float scope, const D3DXVECTOR2 size, const D3DXCOLOR col, const CObject::PRIORITY priority)
{
	CRestrictions *pRestrictions = new	CRestrictions;

	if (pRestrictions != nullptr)
	{
		pRestrictions->m_Center = center;
		pRestrictions->m_fScope = scope;
		pRestrictions->m_Size = size;
		pRestrictions->m_Col = col;
		pRestrictions->Init();
	}

	return pRestrictions;
}

//==============================================================================================
// �ړ�����
//==============================================================================================
void CRestrictions::Collision()
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	CEnemyManager *pEnemy_Manager = nullptr;
	CPlayer *pPlayer = nullptr;

	// ���[�h���ɃL������ǂݍ���
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pEnemy_Manager = CTutorial::GetEnemyManager();
		pPlayer = CTutorial::GetPlayerManager()->GetPlayer(0);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pEnemy_Manager = CGame::GetEnemyManager();
		pPlayer = CGame::GetPlayerManager()->GetPlayer(0);
	}

	m_bHit = false;
	if (pPlayer != nullptr)
	{
		D3DXVECTOR3 Player_Pos = pPlayer->GetPos();
		D3DXVECTOR3 Push_Player = Player_Pos;

		if (Player_Pos.x > m_pDont_EnterPos[0].x - 500)
		{
			m_bHit = true;

			if (Player_Pos.x > m_pDont_EnterPos[0].x)
				Push_Player.x = m_pDont_EnterPos[0].x;
		}
		if (Player_Pos.z > m_pDont_EnterPos[1].z - 500)
		{
			m_bHit = true;

			if (Player_Pos.z > m_pDont_EnterPos[1].z)
				Push_Player.z = m_pDont_EnterPos[1].z;
		}
		if (Player_Pos.x < m_pDont_EnterPos[2].x + 500)
		{
			m_bHit = true;

			if (Player_Pos.x < m_pDont_EnterPos[2].x)
				Push_Player.x = m_pDont_EnterPos[2].x;
		}
		if (Player_Pos.z < m_pDont_EnterPos[3].z + 500)
		{
			m_bHit = true;

			if (Player_Pos.z < m_pDont_EnterPos[3].z)
				Push_Player.z = m_pDont_EnterPos[3].z;
		}

		// �����o��
		pPlayer->SetPos(Push_Player);

		// �G�̏��
		for (auto pEnemy : pEnemy_Manager->GetAllEnemy())
		{
			if (/*pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOB
				&& */pEnemy->GetLife() != 0)
			{
				D3DXVECTOR3 Mob_Pos = pEnemy->GetPos();
				D3DXVECTOR3 Push_Enemy = Mob_Pos;

				if (Mob_Pos.x > m_pDont_EnterPos[0].x)
					Push_Enemy.x = m_pDont_EnterPos[0].x;
				if (Mob_Pos.z > m_pDont_EnterPos[1].z)
					Push_Enemy.z = m_pDont_EnterPos[1].z;
				if (Mob_Pos.x < m_pDont_EnterPos[2].x)
					Push_Enemy.x = m_pDont_EnterPos[2].x;
				if (Mob_Pos.z < m_pDont_EnterPos[3].z)
					Push_Enemy.z = m_pDont_EnterPos[3].z;

				// �����o��
				pEnemy->SetPos(Push_Enemy);
			}
		}
	}
}

//==============================================================================================
// �A���t�@�l�̐ݒ�
//==============================================================================================
void CRestrictions::Alpha()
{
	// ���������Ŕ͈͊O
	if (m_bHit)
	{
		m_fAlpha_Count++;
		m_fAlpha = sinf(m_fAlpha_Count * 0.05f) + 1;
	}
	else
	{
		m_fAlpha_Count = 95;
		m_fAlpha = 0.0f;
	}

	// �[����[�܂ł̕K�v�|���S����
	float num = m_fScope / m_Size.x;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < num * 2; nCnt2++)
		{
			// �_��
			m_pDont_Enter[nCnt][nCnt2]->SetAlpha(m_fAlpha);
		}
	}
}