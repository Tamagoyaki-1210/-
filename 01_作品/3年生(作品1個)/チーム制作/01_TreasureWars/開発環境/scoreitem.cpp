//=============================================================================
//
// scoreitem.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include <time.h>
#include "scoreitem.h"
#include "application.h"
#include "renderer.h"
#include "game.h"
#include "time.h"
#include "charmanager.h"
#include "scoreitem_stack.h"
#include "shadow.h"
#include "shop.h"
#include "stage_select.h"
#include "scoreitemmanager.h"

const char* CScoreItem::STAGE_SCOREITEM_01[] =
{
	"Data\\model\\Drop_Score_Item\\Stage_01\\Gem_Diamond.x",
	"Data\\model\\Drop_Score_Item\\Stage_01\\Gem_Ruby.x",
	"Data\\model\\Drop_Score_Item\\Stage_01\\Gem_Sapphire.x",
	"Data\\model\\Drop_Score_Item\\Stage_01\\Gem_Emerald.x",
	"Data\\model\\Drop_Score_Item\\Stage_01\\Gem_Topaz.x",
};

const char* CScoreItem::STAGE_SCOREITEM_02[] =
{
	"Data\\model\\Drop_Score_Item\\Stage_02\\Fossil_Head.x",
	"Data\\model\\Drop_Score_Item\\Stage_02\\Fossil_Amber.x",
	"Data\\model\\Drop_Score_Item\\Stage_02\\Fossil_Bone.x",
	"Data\\model\\Drop_Score_Item\\Stage_02\\Fossil_Footprints.x",
	"Data\\model\\Drop_Score_Item\\Stage_02\\Fossil_Ammonite.x",
};

const char* CScoreItem::STAGE_SCOREITEM_03[] =
{
	"Data\\model\\Drop_Score_Item\\Stage_03\\Debris_CPUChip.x",
	"Data\\model\\Drop_Score_Item\\Stage_03\\Debris_SolarPanel.x",
	"Data\\model\\Drop_Score_Item\\Stage_03\\Debris_ParabolaAntenna.x",
	"Data\\model\\Drop_Score_Item\\Stage_03\\Debris_RocketEngine.x",
	"Data\\model\\Drop_Score_Item\\Stage_03\\Debris_OxygenBomb.x",
};

const float CScoreItem::ScoreItem_Radius = 45.0f;		// �X�R�A�A�C�e���̃T�C�Y
const float CScoreItem::ScoreItem_Gravity = 20.0f;		// �X�R�A�A�C�e���̏d��
const float CScoreItem::ScoreItem_Inertia = 0.90f;		// �X�R�A�A�C�e���̊���
const float CScoreItem::ScoreItem_Launch = 60.0f;		// �X�R�A�A�C�e�����c�ɑł��オ���
const float CScoreItem::ScoreItem_Blink_Speed = 0.1f;	// �X�R�A�A�C�e���̓_�ő��x
											//=====================================
											// �f�t�H���g�R���X�g���N�^
											//=====================================
CScoreItem::CScoreItem() : m_bGround(false)
{
	// �A�C�e���������Ɏ��g�̃|�C���^���A�C�e���}�l�[�W���[�ɐݒ�
	CGame::GetScoreItemManager()->SetScoreItem(this);

	m_nDestroyCounter = 0;
	m_nFloatingCounter = 0;
}

//=====================================
// �f�X�g���N�^
//=====================================
CScoreItem::~CScoreItem()
{
	CScoreItemManager* pManager = CGame::GetScoreItemManager();

	// �}�l�[�W���[���g�p���̏ꍇ�A������Ɏ��g�̃|�C���^��z�񂩂珜�O����
	if (pManager != nullptr) pManager->DestroyScoreItem(this);
}

//============================================================================
// ����������
//============================================================================
HRESULT CScoreItem::Init()
{
	// �I�u�W�F�N�gX�̏���������
	CObjectX::Init();

	Bounce();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CScoreItem::Uninit()
{
	// �I�u�W�F�N�gX�̏I������
	CObjectX::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CScoreItem::Update()
{
	// �e�̈ʒu���X�V����
	m_pShadow->SetShadow(GetPos());

	// ��΂���]������
	AddRot({ 0.0f, 0.1f, 0.0f });

	// �I�u�W�F�N�gX�̍X�V����
	CObjectX::Update();

	// ���n���ł���Γ����蔻������
	if (!m_bGround)
	{
		Move();
	}
	else
	{
		Floating();

		Blink();
	}
}

//============================================================================
// �`�揈��
//============================================================================
void CScoreItem::Draw()
{
	// �I�u�W�F�N�gX�̕`�揈��
	CObjectX::Draw();
}

//============================================================================
// �����̏���
//============================================================================
void CScoreItem::Destroy()
{
	// �e�̏I������
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}
	// ���g�̏I������
	Uninit();
}

//============================================================================
// �ړ��ʂ̏���
//============================================================================
void CScoreItem::Move()
{
	D3DXVECTOR3 pos = GetPos();
	// �d�͂̒ǉ�
	pos.y -= ScoreItem_Gravity;

	// �ړ��ʂɊ���������
	m_move = m_move * ScoreItem_Inertia;

	// �ړ��ʂ̒ǉ�
	pos += m_move;

	// �n�ʈȉ��̈ʒu�ɗ������ꍇ
	if (pos.y <= 0.0f)
	{
		// ���n�����ʒu���Œ肷��
		m_bGround = true;
		pos.y = 0.0f;
	}

	// �ʒu���X�V����
	SetPos(pos);

	// �ǂƂ̓����蔻��
	WallColl();
}

//============================================================================
// �L�����N�^�[�Ƃ̓����蔻��
//============================================================================
void CScoreItem::CharColl()
{
	// �S�ẴL�����N�^�[��ǂݍ���
	for (int index = 0; index < CCharManager::Max_Character; index++)
	{
		// �L�����N�^�[�̃|�C���^
		CCharacter* pChara = CApplication::GetCharManager()->GetChara(index);

		// �L�����N�^�[���g�p���̏ꍇ
		if (pChara != nullptr)
		{
			// �����蔻��͈͓̔��ɂ���ꍇ
			if (CollisionCircle(GetPos(), ScoreItem_Radius, pChara->GetPos(), pChara->GetRadius()))
			{
				CScoreItemStack* pScoreItemStack = pChara->GetGemStack();

				// ���݂̃X�^�b�N���ő�X�^�b�N�����̏ꍇ
				if (pChara->GetNum_StackScoreItem() < pChara->GetNumGemStack())
				{
					// �X�R�A�A�C�e��������
					pChara->SetStackScoreItem();

					// ���_�l��SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_GEM_GET);

					// �A�C�e���X�R�A���l�������Ď��g���I��������
					pScoreItemStack->SetScoreItem(m_nType);

					Destroy();

					break;
				}
			}
		}
	}
}

//============================================================================
// �ǂƂ̓����蔻��
//============================================================================
void CScoreItem::WallColl()
{
	const D3DXVECTOR3 pos = GetPos();

	// ���E�̕ǂ֔�񂾏ꍇ
	if (pos.x < ScoreItem_Radius * 2.0f || pos.x > 1500.0f - ScoreItem_Radius * 2.0f)
	{
		// �����̈ړ��ʂ𔽓]������
		m_move.x = -m_move.x;
	}

	// �㉺�̕ǂ֔�񂾏ꍇ
	if (pos.z < ScoreItem_Radius * 2.0f || pos.z > 1500.0f - ScoreItem_Radius * 2.0f)
	{
		// �c���̈ړ��ʂ𔽓]������
		m_move.z = -m_move.z;
	}
}

//============================================================================
// ��΂�_�ł����鏈��
//============================================================================
void CScoreItem::Blink()
{
	// �_�ł����鎞�Ԃɓ��B�����ꍇ
	if (m_nDestroyCounter > Blink_Timer)
	{
		// ������܂ł̎��ԂɒB�����ꍇ
		if (m_nDestroyCounter >= Max_Destroy_Timer)
		{
			// ��������
			Destroy();
			return;
		}
		// �A���t�@�l��0�`9�Ńf�N�������g�A10�`19�ŃC���N�������g
		AddAlpha(m_nDestroyCounter % (ScoreItem_Blink_interval * 2) < ScoreItem_Blink_interval ? -ScoreItem_Blink_Speed : ScoreItem_Blink_Speed);

	}
	// �_�ł��n�܂����^�C�~���O
	else if (m_nDestroyCounter == Blink_Timer)
	{
		// �e�̕`�������
		m_pShadow->SetDrawFlag(false);
	}

	m_nDestroyCounter++;

	// �����蔻���ǂݍ���
	CharColl();
}

//============================================================================
// ���˂鏈��
//============================================================================
void CScoreItem::Bounce()
{
	// �����_���Ȓl�̕ϐ�
	const float fRand = ((rand() % 629) - 314) / 100.0f;

	// ���˂�͂̒l�̕ϐ�
	const float fBounce = (rand() % 100) / 10.0f + 10;

	// �ړ��ʂ�ݒ�
	m_move = { sinf(fRand) * fBounce, ScoreItem_Launch, cosf(fRand) * fBounce };
}

//============================================================================
// ���V���鏈��
//============================================================================
void CScoreItem::Floating()
{
	// ���V������
	AddPos({ 0.0f, m_nFloatingCounter++ % (Floating_Timer * 2) < Floating_Timer ? 1.0f : -1.0f, 0.0f });

	// ���V���[�v�����������珉����
	if (m_nFloatingCounter >= Floating_Timer * 2)
	{
		// ���V���Ԃ̏�����
		m_nFloatingCounter = 0;
	}
}

//============================================================================
// �e�̐�������
//============================================================================
void CScoreItem::SetShadow(const D3DXVECTOR3 pos)
{
	m_pShadow = CShadow::Create({ pos.x, 5.0f, pos.z }, { ScoreItem_Radius, ScoreItem_Radius, 0.0f });
}

//============================================================================
// ��������
//============================================================================
CScoreItem* CScoreItem::RandomCreate(const D3DXVECTOR3 pos)
{
	CScoreItem* pScoreItem = new CScoreItem;

	if (FAILED(pScoreItem->Init()))
	{
		return nullptr;
	}

	// �����_���Ȓl % �ő�̒l
	int nRand = rand() % (int)(0.5f * (CScoreItem::MAX_SCOREITEM_MODEL * (CScoreItem::MAX_SCOREITEM_MODEL + 1)));

	// �X�R�A�A�C�e���̎��
	int nType = 0;

	// �o�^���ꂽ�~���ɘA��m��Up
	for (int nCnt = 0, i = 1; nCnt < CScoreItem::MAX_SCOREITEM_MODEL; nCnt++, i += (nCnt + 1), nType++)
	{
		// �m���ɓ��I���Ă���ꍇ
		if (nRand < i)
		{
			break;
		}
	}

	// �X�R�A�A�C�e���̎�ނ�ݒ�
	pScoreItem->m_nType = nType;

	// �X�e�[�W�ԍ��Ń��f�������߂�
	switch (CStage_Select::GetStage_Number())
	{
	case CStage_Select::STAGE_1:
		pScoreItem->SetIndex(pScoreItem->ReadObject(STAGE_SCOREITEM_01[nType]));
		break;
	case CStage_Select::STAGE_2:
		pScoreItem->SetIndex(pScoreItem->ReadObject(STAGE_SCOREITEM_02[nType]));
		break;
	case CStage_Select::STAGE_3:
		pScoreItem->SetIndex(pScoreItem->ReadObject(STAGE_SCOREITEM_03[nType]));
		break;
	default:
		break;
	}

	pScoreItem->SetPos(pos);
	pScoreItem->SetRot({ 0.0f, D3DX_PI / 2, 0.0f });
	pScoreItem->SetSize({ 1.0f,1.0f,1.0f });
	pScoreItem->SetShadow(pos);

	return pScoreItem;
}

//============================================================================
// ��������
//============================================================================
CScoreItem* CScoreItem::Create(const D3DXVECTOR3 pos, const int type)
{
	CScoreItem* pScoreItem = new CScoreItem;

	if (FAILED(pScoreItem->Init()))
	{
		return nullptr;
	}

	pScoreItem->m_nType = type;

	// �X�e�[�W�ԍ��Ń��f�������߂�
	switch (CStage_Select::GetStage_Number())
	{
	case CStage_Select::STAGE_1:
		pScoreItem->SetIndex(pScoreItem->ReadObject(STAGE_SCOREITEM_01[type]));
		break;
	case CStage_Select::STAGE_2:
		pScoreItem->SetIndex(pScoreItem->ReadObject(STAGE_SCOREITEM_02[type]));
		break;
	case CStage_Select::STAGE_3:
		pScoreItem->SetIndex(pScoreItem->ReadObject(STAGE_SCOREITEM_03[type]));
		break;
	default:
		break;
	}

	pScoreItem->SetPos(pos);
	pScoreItem->SetRot({ 0.0f, D3DX_PI / 2, 0.0f });
	pScoreItem->SetSize({ 1.0f,1.0f,1.0f });
	pScoreItem->SetShadow(pos);

	return pScoreItem;
}