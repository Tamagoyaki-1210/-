//==============================================================================================
//
// �A�C�e���@�@�@	item.cpp
// tutida ryousei
//
//==============================================================================================
#include"item.h"
#include"application.h"
#include"charmanager.h"
#include"itemmanager.h"
#include"game.h"
#include "particle_emitter.h"
#include"shadow.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CItem::CItem()
{
	// �A�C�e���������Ɏ��g�̃|�C���^���A�C�e���}�l�[�W���[�ɐݒ�
	CGame::GetItemManager()->SetItem(this);

	m_fRadius = 0.0f;
	m_nLife = 0;
	m_emitTime = 0;
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CItem::~CItem()
{
	CItemManager* pManager = CGame::GetItemManager();

	// �}�l�[�W���[���g�p���̏ꍇ�A������Ɏ��g�̃|�C���^��z�񂩂珜�O����
	if (pManager != nullptr) pManager->DestroyItem(this);
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CItem::Init()
{
	// �����_���ŃA�C�e���̎�ނ�ݒ�
	m_nRand_Type = (Item_Type)(rand() % ITEM_MAX);

	// �A�C�e�����̊i�[��
	char *Item_Name = "\0";

	// �^�C�v�ʂ̐F�̐ݒ�
	switch (m_nRand_Type)
	{
	case CItem::ITEM_SPEEDUP:
		Item_Name = "Data/model/Item/item_rollershoes.x";
		break;
	case CItem::ITEM_KNOCKBACK:
		Item_Name = "Data/model/Item/item_hammer.x";
		break;
	case CItem::ITEM_STACK:
		Item_Name = "Data/model/Item/item_basket.x";
		break;
	default:
		break;
	}
	// ���f���ݒ�
	SetIndex(this->ReadObject(Item_Name));

	m_fRadius = 45.0f;
	m_nLife = 480;
	m_Rot = { 0.0f, 0.0f, 0.0f };
	m_bAlpha = false;

	m_pShadow = CShadow::Create({ m_Pos.x, 5.0f, m_Pos.z }, { 50.0f, 50.0f, 0.0f });

	CObjectX::Init();
	CObjectX::SetSize(m_Size);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CItem::Uninit()
{
	CObjectX::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CItem::Update()
{
	CObjectX::Update();

	// �A�C�e���̎���ɏo��p�[�e�B�N��
	m_emitTime++;
	if ((m_emitTime % 40) == 0) CParticleEmitter::Create("ItemHighlight", m_Pos);

	// �L�����N�^�[�Ƃ̓����蔻��
	CollItem();

	// �_��
	Flash_Item();

	// ���Ԍo�߂ŏ���
	Life();

	// ��]
	m_Rot.y += 0.01f;

	CObjectX::SetPos(m_Pos);
	CObjectX::SetRot(m_Rot);
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CItem::Draw()
{
	CObjectX::Draw();
}

//==============================================================================================
// ��������
//==============================================================================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CItem *pItem = nullptr;

	pItem = new CItem;

	if (pItem != nullptr)
	{
		pItem->SetItemPos(pos);		// �ʒu�̐ݒ�
		pItem->SetItemSize(size);	// �T�C�Y�̐ݒ�
		pItem->Init();
	}

	return pItem;
}

//==============================================================================================
// �L�����N�^�[�Ƃ̓����蔻��
//==============================================================================================
void CItem::CollItem()
{
	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		D3DXVECTOR3 CharPos = { 0.0f,0.0f,0.0f };
		float fCharRadius = 0.0f;

		// �L�����N�^�[�̏��
		CCharacter* pChara = CApplication::GetCharManager()->GetChara(nCnt);

		if (pChara != nullptr)
		{
			CharPos = pChara->GetPos();				// �ʒu
			fCharRadius = pChara->GetRadius();		// �����蔻��̔��a

		// �~�̓����蔻��
			bool Hit = CObject::CollisionCircle(m_Pos, m_fRadius, CharPos, fCharRadius);

			if (Hit)
			{
				// �v���C���[�̈ʒu�Ƀp�[�e�B�N��
				CParticleEmitter::Create("ItemGet", m_Pos);

				// �^�C�v�ʂ̏���
				Type(pChara);

				// �A�C�e��������
				Uninit();

				// �e����������
				if (m_pShadow != nullptr)
				{
					m_pShadow->Uninit();
					m_pShadow = nullptr;
				}

				// �A�C�e���擾���ʂ𔭊�����
				pChara->Item_Effect(m_nRand_Type);
			}
		}
	}
}

//==============================================================================================
// �A�C�e���̃^�C�v�ʂ̏���
//==============================================================================================
void CItem::Type(CCharacter* pChara)
{
	int nItemLevel = 0;

	// �A�C�e���擾SE
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_ITEM);

	switch (m_nRand_Type)
	{
	case CItem::ITEM_SPEEDUP:
		// ���݂̃��x�����擾
		nItemLevel = pChara->GetMoveSpeed_Level();
		nItemLevel++;

		// �ړ����x��i�K�㏸
		pChara->SetMoveSpeed_Level(nItemLevel);
		break;

	case CItem::ITEM_KNOCKBACK:
		// ���݂̃��x�����擾
		nItemLevel = pChara->GetKnockBack_Level();
		nItemLevel++;

		// �m�b�N�o�b�N�̋�����L�΂�
		pChara->SetKnockBack_Level(nItemLevel);
		break;
	case CItem::ITEM_STACK:
		// ���݂̃��x�����擾
		nItemLevel = pChara->GetGemStack_Level();
		nItemLevel++;

		// ��x�Ɏ��Ă�z�΂̗ʂ𑝂₷
		pChara->SetGemStack_Level(nItemLevel);
		break;
	default:
		break;
	}
}

//==============================================================================================
// �A�C�e���̓_��
//==============================================================================================
void CItem::Flash_Item()
{
	if (m_nLife < 260)
	{
		// �_�ł��邩
		if (m_fAlpha >= 1)
			m_bAlpha = false;
		if (m_fAlpha <= 0)
			m_bAlpha = true;

		// �A���t�@�l
		if (!m_bAlpha)
			m_fAlpha -= 0.05f;
		else
			m_fAlpha += 0.05f;

		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			// �A�C�e���̐F
			SetAlpha(m_fAlpha);
		}
	}
}

//==============================================================================================
// ���Ԍo�߂ŏ���
//==============================================================================================
void CItem::Life()
{
	m_nLife--;

	// �̗͂�0�ɂȂ�����
	if (m_nLife <= 0)
	{
		Uninit();
		// �e����������
		if (m_pShadow != nullptr)
		{
			m_pShadow->Uninit();
			m_pShadow = nullptr;
		}
	}
}