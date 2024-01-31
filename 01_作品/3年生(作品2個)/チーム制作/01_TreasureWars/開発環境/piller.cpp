//==============================================================================
//
// piller.cpp	(���̃I�u�W�F�N�g.cpp)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// �C���N���[�h
//==============================================================================
#include <time.h>
#include "application.h"
#include "renderer.h"
#include "piller.h"
#include "objectX.h"
#include "ore.h"
#include "ranking.h"
#include "score.h"
#include "result.h"
#include"character.h"
#include "charmanager.h"
#include "stage_select.h"

//==============================================================================
// �ÓI�����o�ϐ��錾
//==============================================================================
const float CPiller::MOVING_PILLERSPEED = 3.0f;
int CPiller::m_nScore = 0;
int CPiller::m_anScore[4] = {};

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CPiller ::CPiller()
{

}


//==============================================================================
// �f�X�g���N�^
//==============================================================================
CPiller::~CPiller()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CPiller::Init()
{
	//objectX�̏���������
	CObjectX::Init();

	//==================================================
	// �����o�ϐ��̏�����
	//==================================================
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, MOVING_PILLERSPEED, 0.0f);	
	m_no1 = 0;

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CPiller::Uninit()
{
	//objectX�̏I������
	CObjectX::Uninit();
}

//==============================================================================
// �X�V����
//==============================================================================
void CPiller::Update()
{
	//�ʒu�̎擾
	D3DXVECTOR3 pos = GetPos();

	for (int i = 0; i < 4; i++)
	{
		//�ő�l
		if (m_no1 < m_anScore[i])
		{//�X�R�A��0���傫��������
			m_no1 = m_anScore[i];			//�ő�l�i�[�ϐ����̃X�R�A���i�[
		}
		if (m_no1 == m_anScore[i] && pos.y >= CResult::Moving_Piller[0])
		{
			// �I���t���O��^�ɂ���
			CResult::SetFlag(true);
		}
	}

	// �w�肳�ꂽ�ʒu��艺�̏ꍇ
	if (pos.y <= m_fMaxMoving)
	{
		pos.y += m_move.y;
	}
	//  �w�肳�ꂽ�ʒu�ȏ�̏ꏊ�ɒB�����ꍇ
	else if (pos.y >= m_fMaxMoving)
	{
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	//�ʒu�̐ݒ�
	SetPos(pos);

	// �L�����̈ړ�
	m_pResult_Char->SetPos(pos);
	
	//objectX�̍X�V����
	CObjectX::Update();
}

//==============================================================================
// ��������
//==============================================================================
CPiller *CPiller::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, int index, float moving, int num)
{
	CPiller *pPiller = nullptr;

	pPiller = new CPiller;

	if (pPiller != nullptr)
	{
		pPiller->SetPos(pos);
		pPiller->SetRot(rot);
		pPiller->SetSize(size);
		pPiller->SetIndex(index);
		pPiller->m_fMaxMoving = moving;	// �c�̍ő�l�̐ݒ�

		// ���U���g�p�̃L����
		pPiller->m_pResult_Char = CApplication::GetCharManager()->EntryChara(pos, CCharManager::TYPE_RESULT_CHAR, num, CStage_Select::GetStage_Number());
		pPiller->Init();
	}

	return pPiller;
}
