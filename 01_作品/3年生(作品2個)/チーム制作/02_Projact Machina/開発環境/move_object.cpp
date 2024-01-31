//==============================================================================================
//
// �����I�u�W�F�N�g�@�@�@move_object.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"move_object.h"
#include "application.h"
#include "tutorial.h"
#include "game.h"
#include "collision_manager.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CMove_Object::CMove_Object(const PRIORITY priority) : CObject(priority)
{
	m_tag = TAG_NONE;
	m_bPlayerSide = false;
	m_bCollision_Delay = false;		// �����蔻��̒x���ݒu����
	m_bCollision_NoneHit = false;	// �����蔻��̃q�b�g������ǂݍ��܂Ȃ�����
	m_CenterPos = { 0.0f, 0.0f, 0.0f };
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CMove_Object::~CMove_Object()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CMove_Object::Init()
{
	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CMove_Object::Uninit()
{
	if (m_pCollision != nullptr)
	{
		m_pCollision->Uninit();
		m_pCollision = nullptr;
	}

	Release();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CMove_Object::Update()
{
	// �����蔻���x�����Đݒ肷�鏈��
	DelayCollision();

	if (m_pCollision != nullptr)
		m_pCollision->SetNoneHit(m_bCollision_NoneHit);
}

//==============================================================================================
// �����蔻��|�C���^����������
//==============================================================================================
void CMove_Object::DelayCollision()
{
	// �x�����Đݒ肷��ꍇ
	if (m_bCollision_Delay == true)
	{
		SetCollision();
	}
}

//==============================================================================================
// �����蔻��|�C���^����������
//==============================================================================================
void CMove_Object::CollisionDestroy()
{
	if (m_pCollision != nullptr)
	{
		m_pCollision = nullptr;
	}
}

//==============================================================================================
// �����蔻��̐���
//==============================================================================================
void CMove_Object::SetCollision()
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	CCollision_Manager* pManager = nullptr;

	// �������Ɏ��g�̃|�C���^�𓖂��蔻��}�l�[�W���[�ɐݒ�
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pManager = CTutorial::GetCollision_Manager();
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pManager = CGame::GetCollision_Manager();
	}

	// �����蔻��}�l�[�W���[���g�p����Ă���ꍇ && �����蔻�肪�g�p����Ă��Ȃ��ꍇ
	if (pManager != nullptr && m_pCollision == nullptr)
	{
		m_pCollision = CCollision::Create(this);
		m_pCollision->SetNoneHit(m_bCollision_NoneHit);
	}
}
