//==============================================================================
//
// Ore.cpp	(�z��.cpp)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// �C���N���[�h
//==============================================================================
#include "application.h"
#include "renderer.h"
#include "ore.h"
#include "scoreitem.h"
#include "particle_emitter.h"
#include <assert.h>

//==============================================================================
// �ÓI�����o�ϐ��錾
//==============================================================================
const float COre::MOVING_DISTANCE = 2.5f;
const int COre::UNINIT_TIME = 500;
const int COre::Ore_Blink_Speed = 10;		// �z�΂̓_�ł��鑬�x
const float COre::Ore_Blink_Timer = 180.0f;	// �z�΂��_�ł��n�߂鎞��

const char* COre::STAGE_MODEL_01[] =
{
	"Data\\model\\Collecting_Point\\Stage_01\\ore_01.x",
	"Data\\model\\Collecting_Point\\Stage_01\\ore_02.x"
};
const char* COre::STAGE_MODEL_02[] =
{
	"Data\\model\\Collecting_Point\\Stage_02\\Fossil_01.x",
	"Data\\model\\Collecting_Point\\Stage_02\\Fossil_02.x"
};
const char* COre::STAGE_MODEL_03[] =
{
	"Data\\model\\Collecting_Point\\Stage_03\\Artificial_Satellite_01.x",
	"Data\\model\\Collecting_Point\\Stage_03\\Artificial_Satellite_02.x"
};

//==============================================================================
// �R���X�g���N�^
//==============================================================================
COre::COre()
{
	m_nLife = Normal_Ore_Life;
	m_UninitTime = UNINIT_TIME;
	m_bOnce = true;
	m_bAppearance = false;
	m_bOreDeath = false;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
COre::~COre()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT COre::Init()
{
	//objectX�̏���������
	CObjectX::Init();

	//==================================================
	// �����o�ϐ��̏�����
	//==================================================
	m_move = D3DXVECTOR3(0.0f, MOVING_DISTANCE, 0.0f);

	
	// �z��(��)����g��
	SetIndex(m_nBigOreModelIndex);

	return S_OK;
}

//==============================================================================
// ����������
//==============================================================================
HRESULT COre::Init(int nStageNum)
{
	// �z�΃��f���̔ԍ��̐ݒ�
	switch (nStageNum)
	{
	case 0:
		m_nOreModelIndex = ReadObject(STAGE_MODEL_01[0]);
		m_nBigOreModelIndex = ReadObject(STAGE_MODEL_01[1]);
		break;
	case 1:
		m_nOreModelIndex = ReadObject(STAGE_MODEL_02[0]);
		m_nBigOreModelIndex = ReadObject(STAGE_MODEL_02[1]);
		break;
	case 2:
		m_nOreModelIndex = ReadObject(STAGE_MODEL_03[0]);
		m_nBigOreModelIndex = ReadObject(STAGE_MODEL_03[1]);
		break;
	default:
		assert(false);
		break;
	}

	//������
	Init();

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void COre::Uninit()
{
	//objectX�̏I������
	CObjectX::Uninit();
}

//==============================================================================
// �X�V����
//==============================================================================
void COre::Update()
{
	//�o���������������ǂ���
	if (m_bAppearance)
	{
		if (m_bOnce)
		{
			// �z�Ώo�����p�[�e�B�N��
			CParticleEmitter::Create("PopOre", GetPos());
			m_bOnce = false;
		}
	}

	// ���ݒl
	D3DXVECTOR3 pos = GetPos();

	//�n�ʂɏo����
	if (pos.y >= 0.0f)
	{
		//�o������������
		m_bAppearance = true;
		m_UninitTime--;
	}
	//�o�����������Ă��Ȃ�
	if (m_bAppearance == false)
	{
		pos += m_move;
	}
	
	// �_�ŊJ�n���ԂɒB�����ꍇ
	if (m_UninitTime < Ore_Blink_Timer)
	{
		// �z�΂̓_�ŏ���
		AddAlpha(m_UninitTime % (Ore_Blink_Speed * 2) < Ore_Blink_Speed ? 1.0f / Ore_Blink_Speed : -1.0f / Ore_Blink_Speed);
	}

	//���Ԃ�0�ɂȂ�����
	if (m_UninitTime <= 0)
	{
		// �z�΂��I��������t���O
		m_bOreDeath = true;
	}

	//�ݒ菈��
	SetPos(pos);

	//objectX�̍X�V����
	CObjectX::Update();
}

//==============================================================================================
// �_���[�W����
//==============================================================================================
int COre::Damage(int damage)
{
	// �z�΂�����������
	SetIndex(m_nOreModelIndex);

	// ������܂ł̎��Ԃ�������
	m_UninitTime = UNINIT_TIME;

	return m_nLife -= damage;
}

//==============================================================================================
// ��������
//==============================================================================================
COre *COre::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, int nStageNum)
{
	COre *pOre = new COre;

	if (pOre != nullptr)
	{
		pOre->SetPos(pos);
		pOre->SetRot(rot);
		pOre->SetSize(size);
		pOre->Init(nStageNum);
	}

	return pOre;
}