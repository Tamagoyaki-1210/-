//=============================================================================
//
// scoreitem_stack.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include <time.h>
#include "scoreitem_stack.h"
#include "application.h"
#include "renderer.h"
#include "game.h"
#include "time.h"
#include "receipt.h"
#include "shop.h"
#include "character.h"
#include "stage_select.h"

// �X�R�A�A�C�e���̐F�ꗗ
const D3DXCOLOR CScoreItemStack::Col_Yellow = D3DCOLOR_RGBA(255, 255, 16, 255);
const D3DXCOLOR CScoreItemStack::Col_Green = D3DCOLOR_RGBA(34, 255, 25, 255);
const D3DXCOLOR CScoreItemStack::Col_Bule = D3DCOLOR_RGBA(40, 72, 255, 255);
const D3DXCOLOR CScoreItemStack::Col_Red = D3DCOLOR_RGBA(252, 20, 20, 255);
const D3DXCOLOR CScoreItemStack::Col_Gray = D3DCOLOR_RGBA(224, 224, 224, 255);
const D3DXCOLOR CScoreItemStack::Col_White = D3DCOLOR_RGBA(255, 255, 255, 255);

const float CScoreItemStack::ScoreItem_Stack_Size = 30.0f;	// �X�R�A�A�C�e���摜�̃T�C�Y

//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CScoreItemStack::CScoreItemStack()
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CScoreItemStack::~CScoreItemStack()
{

}

//============================================================================
// ����������
//============================================================================
HRESULT CScoreItemStack::Init()
{
	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CScoreItemStack::Uninit()
{
	// �̎����̏I������
	if (m_pReceipt != nullptr)
	{
		m_pReceipt->Uninit();
		m_pReceipt = nullptr;
	}

	// �z��̒��g�̑S�č폜����
	m_listScoreItem.clear();

	// UI�̏I������
	CUi::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CScoreItemStack::Update()
{
	CUi::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CScoreItemStack::Draw()
{

}

//============================================================================
// ��΂̕�������
//============================================================================
void CScoreItemStack::LostScoreItem(const int lost, const D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < lost; nCnt++)
	{
		// �z����g�p���Ă���ꍇ
		if (!m_listScoreItem.empty())
		{
			// �z��̈�Ԍ��̏��
			ScoreItem gem = m_listScoreItem.back();

			// �X�R�A�A�C�e���̐���
			CScoreItem::Create(pos, gem.nType);

			// �X�R�A�A�C�e���̃f�t�H���g�摜��`�悷��
			m_apScoreItem[m_listScoreItem.size() - 1]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_NONE);

			// �z��̈�Ԍ��̍폜
			m_listScoreItem.pop_back();
		}
		else
		{
			break;
		}
	}
}

//============================================================================
// �X�R�A�A�C�e���̔��p����
//============================================================================
int CScoreItemStack::SaleScoreItem()
{
	// ��΂̒l�i�̕Ԃ�l�p�̕ϐ�
	int nValue = 0;

	// �z����g�p���Ă���ꍇ
	if (!m_listScoreItem.empty())
	{
		// �z��̈�Ԍ��̏��
		ScoreItem scoreItem = m_listScoreItem.back();

		// �X�R�A�A�C�e���̃f�t�H���g�摜��`�悷��
		m_apScoreItem[m_listScoreItem.size() - 1]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_NONE);

		// �z��̈�Ԍ��̍폜
		m_listScoreItem.pop_back();

		// ������̕ϐ�
		std::string str;
		std::string strCol;

		strCol += std::to_string(scoreItem.col.r * 255) + "," + std::to_string(scoreItem.col.g * 255) + "," + std::to_string(scoreItem.col.b * 255) + "," + std::to_string(scoreItem.col.a * 255) + ",";
		str += "<c=" + strCol + ">" + scoreItem.name + "</>";
		nValue = scoreItem.value;

		// ������char*�^�ɕϊ�str += "<c=0,0,0,255,>$" + std::to_string(value) + "</>";
		str += "<c=255,255,255,255,>$</><c=255,255,255,255,>" + std::to_string(nValue) + "</>";

		// �̎����ɕ�������L�q(�ݒ�)����
		m_pReceipt->SetTotalValue(nValue);
		m_pReceipt->SetString(str);
	}

	// �X�R�A�A�C�e���̒l�i�̕Ԃ�l
	return nValue;
}

//============================================================================
// �X�R�A�A�C�e���̐ݒ菈��
//============================================================================
void CScoreItemStack::SetScoreItem(const int type)
{
	// �ő吔�𒴂��Ȃ��ꍇ
	if ((int)m_listScoreItem.size() < CCharacter::Max_GemStack)
	{
		// �z��̍ő吔
		int listSize = (int)m_listScoreItem.size();

		// ��΍\���̂̏�����
		ScoreItem scoreItem = {};

		// ��΍\���̂ɏ���������
		scoreItem.getTime = CGame::GetTime()->GetTime();
		scoreItem.nType = type;

		// �X�R�A�A�C�e���̉摜��ݒ肷��
		switch (CStage_Select::GetStage_Number())
		{
		case CStage_Select::STAGE_1:
			// ��ޖ��̏���
			switch (scoreItem.nType)
			{
			case GEM_DIAMOND:
				scoreItem.name = "�_�C�������h";
				scoreItem.col = Col_Gray;
				scoreItem.value = 500;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_DIAMOND);
				break;
			case GEM_RUBY:
				scoreItem.name = "���r�[�@�@�@";
				scoreItem.col = Col_Red;
				scoreItem.value = 400;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_RUBY);
				break;
			case GEM_SAPPHIRE:
				scoreItem.name = "�T�t�@�C�A�@";
				scoreItem.col = Col_Bule;
				scoreItem.value = 300;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_SAPPHIRE);
				break;
			case GEM_EMERALD:
				scoreItem.name = "�G�������h�@";
				scoreItem.col = Col_Green;
				scoreItem.value = 200;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_EMERALD);
				break;
			case GEM_TOPAZ:
				scoreItem.name = "�g�p�[�Y�@�@";
				scoreItem.col = Col_Yellow;
				scoreItem.value = 100;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_TOPAZ);
				break;
			default:
				break;
			}
			break;
		case CStage_Select::STAGE_2:
			// ��ޖ��̏���
			switch (scoreItem.nType)
			{
			case FOSSIL_HEAD:
				scoreItem.name = "�����܁@�@�@";
				scoreItem.col = Col_Gray;
				scoreItem.value = 1200;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_HEAD);
				break;
			case FOSSIL_AMBER:
				scoreItem.name = "�R�n�N�@�@�@";
				scoreItem.col = Col_Yellow;
				scoreItem.value = 800;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_AMBER);
				break;
			case FOSSIL_BONE:
				scoreItem.name = "�z�l�@�@�@�@";
				scoreItem.col = Col_Red;
				scoreItem.value = 500;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_BONE);
				break;
			case FOSSIL_FOOTPRINS:
				scoreItem.name = "�������Ɓ@�@";
				scoreItem.col = Col_Green;
				scoreItem.value = 300;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_FOOTPRINTS);
				break;
			case FOSSIL_AMMONITE:
				scoreItem.name = "�A�����i�C�g";
				scoreItem.col = Col_Bule;
				scoreItem.value = 200;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_AMMONITE);
				break;
			default:
				break;
			}
			break;
		case CStage_Select::STAGE_3:
			// ��ޖ��̏���
			switch (scoreItem.nType)
			{
			case DEBRIS_CPUCHIP:
				scoreItem.name = "CPU�`�b�v";
				scoreItem.col = Col_Gray;
				scoreItem.value = 2677;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_CPU);
				break;
			case DEBRIS_PANEL:
				scoreItem.name = "�p�l���@�@�@";
				scoreItem.col = Col_Bule;
				scoreItem.value = 1453;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_SOLARPANEL);
				break;
			case DEBRIS_ANTENNA:
				scoreItem.name = "�A���e�i�@�@";
				scoreItem.col = Col_Green;
				scoreItem.value = 997;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_ANTENNA);
				break;
			case DEBRIS_ENGINE:
				scoreItem.name = "�G���W���@�@";
				scoreItem.col = Col_Red;
				scoreItem.value = 521;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_ENGINE);
				break;
			case DEBRIS_OXYGENBONB:
				scoreItem.name = "���񂻃{���x";
				scoreItem.col = Col_Yellow;
				scoreItem.value = 53;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_OXYGENTANK);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		// �z��̈�Ԍ��̎��̏ꏊ�ɒǉ�����
		m_listScoreItem.push_back(scoreItem);
	}
}

//============================================================================
// �v���C���[����ő及�������l��
//============================================================================
void CScoreItemStack::SetDefault(const int num)
{
	// ���ݖ��g�p�̃X�^�b�N����ǉ�
	for (int nCnt = 0; nCnt < num; nCnt++)
	{
		m_apScoreItem[nCnt]->SetDrawFlag(true);
	}
}

//============================================================================
// ��������
//============================================================================
CScoreItemStack* CScoreItemStack::Create(const D3DXVECTOR3 pos, const int index)
{
	CScoreItemStack* pScoreItemStack = new CScoreItemStack;

	if (FAILED(pScoreItemStack->Init()))
	{
		return nullptr;
	}

	pScoreItemStack->m_pos = pos;
	pScoreItemStack->m_pReceipt = CReceipt::Create(index);	// �X�R�A�A�C�e���X�^�b�N�̃��V�[�g

	for (int nCnt = 0; nCnt < 20; nCnt++)
	{
		pScoreItemStack->m_apScoreItem[nCnt] = CObject2D::Create(pos +
			D3DXVECTOR3(
			ScoreItem_Stack_Size * 0.5f * (nCnt % (CCharacter::Max_GemStack / 2)) + ((int)(nCnt / (CCharacter::Max_GemStack / 2)) * ScoreItem_Stack_Size * 0.25f),
			ScoreItem_Stack_Size * 0.5f * (int)(nCnt / (CCharacter::Max_GemStack / 2)),
			0.0f),
			{ ScoreItem_Stack_Size, ScoreItem_Stack_Size, 0.0f }, 5);
		pScoreItemStack->m_apScoreItem[nCnt]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_NONE);
		pScoreItemStack->m_apScoreItem[nCnt]->SetDrawFlag(false);
	}

	return pScoreItemStack;
}
