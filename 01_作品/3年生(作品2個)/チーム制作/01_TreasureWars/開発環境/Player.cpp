//==============================================================================================
//
// �v���C���[�@�@�@player.cpp
// tutida ryousei
//
//==============================================================================================
#include"player.h"
#include"application.h"
#include"input.h"
#include"camera.h"
#include"model.h"
#include"game.h"
#include"entry.h"
#include"countdown.h"
#include"time.h"
#include"gauge.h"
#include"charmanager.h"
#include"debugProc.h"
#include"score.h"
#include "particle_emitter.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CPlayer::CPlayer()
{
	m_SizeMax = { -1000.0f,-1000.0f,-1000.0f };
	m_SizeMin = { 1000.0f, 1000.0f, 1000.0f };

	// �U���͂̏����l��ݒ�
	SetMinePower(First_Mine_Power);
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CPlayer::~CPlayer()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CPlayer::Init()
{
	CCharacter::Init();

	// �G���g���[���[�h�̎��̂ݖ��g�p�ɂ���
	m_bEntry = CApplication::GetMode() != CApplication::MODE_ENTRY;

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CPlayer::Uninit()
{
	CCharacter::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CPlayer::Update()
{
	// ���[�V�����ԍ��̐ݒ�
	ChangeMotion();

	// ���[�V����
	Motion();

	// �Q�[�����J�n����Ă���ꍇ ���� �G���g���[��ʂ̏ꍇ ���� �`���[�g���A�����ł͖����ꍇ
	if ((CApplication::GetGameStart()
		|| (CApplication::GetMode() == CApplication::MODE_ENTRY && !CEntry::GetTutorial()))
		|| CApplication::GetMode() == CApplication::MODE_RESULT)
	{
		// �m�b�N�o�b�N����
		bool bKnockBack = CCharacter::GetKnockBack();

		// ����
		bool bIn = CEntry::GetIn(GetIndex());

		// �ړI�̊p�x�̎擾
		m_RotDest = CCharacter::GetRotDest();

		if (!bKnockBack && ((bIn || CApplication::GetGameStart()))
			|| CApplication::GetMode() == CApplication::MODE_RESULT)
		{
			// �A�N�V����
			Action();

			// �ړ�����
			Move();
		}

		// �ړI�̊p�x�̐ݒ�
		CCharacter::SetRotDest(m_RotDest);

		CCharacter::Update();
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CPlayer::Draw()
{
	CCharacter::Draw();
}

//==============================================================================================
// ��������
//==============================================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, int index)
{
	CPlayer *pPlayer = nullptr;

	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->CCharacter::SetPos(pos);
		pPlayer->SetIndex(index);
		pPlayer->Init();
	}

	return pPlayer;
}

//==============================================================================================
// �ړ�����
//==============================================================================================
void CPlayer::Move()
{
	//���̓f�o�C�X�̏��
	CInput* pInput = CInput::GetKey();

	D3DXVECTOR3 CRot = { 0.0f,0.0f,0.0f };

	// �ړ���
	D3DXVECTOR3 move = { 0.0f, 0.0f, 0.0f };

	// �J�����̊p�x
	CRot =  CApplication::GetCamera()->GetRot();

	const int nIndex = GetIndex();	// ���g�̔ԍ�

	if (!GetAttack())
	{
		// �v���C���[�̈ړ�
		if ((pInput->Press(DIK_W) && nIndex == 0) || pInput->Press(JOYPAD_UP, nIndex))
		{
			if ((pInput->Press(DIK_A) && nIndex == 0) || pInput->Press(JOYPAD_LEFT, nIndex))
			{// ���O
				m_RotDest.y = CRot.y + D3DX_PI * 3 / 4;
				move.x = -sinf(CRot.y + D3DX_PI * 3 / 4);
				move.z = -cosf(CRot.y + D3DX_PI * 3 / 4);
			}
			else if ((pInput->Press(DIK_D) && nIndex == 0) || pInput->Press(JOYPAD_RIGHT, nIndex))
			{// �E�O
				m_RotDest.y = CRot.y - D3DX_PI * 3 / 4;
				move.x = sinf(CRot.y + D3DX_PI / 4);
				move.z = cosf(CRot.y + D3DX_PI / 4);
			}
			else
			{// �O
				m_RotDest.y = CRot.y + D3DX_PI;
				move.x = sinf(CRot.y);
				move.z = cosf(CRot.y);
			}
		}
		else if ((pInput->Press(DIK_S) && nIndex == 0) || pInput->Press(JOYPAD_DOWN, nIndex))
		{
			if ((pInput->Press(DIK_A) && nIndex == 0) || pInput->Press(JOYPAD_LEFT, nIndex))
			{// ����
				m_RotDest.y = CRot.y + D3DX_PI / 4;
				move.x = -sinf(CRot.y + D3DX_PI / 4);
				move.z = -cosf(CRot.y + D3DX_PI / 4);
			}
			else if ((pInput->Press(DIK_D) && nIndex == 0) || pInput->Press(JOYPAD_RIGHT, nIndex))
			{// �E��
				m_RotDest.y = CRot.y - D3DX_PI / 4;
				move.x = sinf(CRot.y + D3DX_PI * 3 / 4);
				move.z = cosf(CRot.y + D3DX_PI * 3 / 4);
			}
			else
			{// ��
				m_RotDest.y = CRot.y;
				move.x = -sinf(CRot.y);
				move.z = -cosf(CRot.y);
			}
		}
		else if ((pInput->Press(DIK_A) && nIndex == 0) || pInput->Press(JOYPAD_LEFT, nIndex))
		{// ��
			m_RotDest.y = CRot.y + D3DX_PI / 2;
			move.x = -sinf(CRot.y + D3DX_PI / 2);
			move.z = -cosf(CRot.y + D3DX_PI / 2);
		}
		else if ((pInput->Press(DIK_D) && nIndex == 0) || pInput->Press(JOYPAD_RIGHT, nIndex))
		{// �E
			m_RotDest.y = CRot.y - D3DX_PI / 2;
			move.x = sinf(CRot.y + D3DX_PI / 2);
			move.z = cosf(CRot.y + D3DX_PI / 2);
		}

		if ((pInput->Press(DIK_W) && nIndex == 0)
			|| (pInput->Press(DIK_A) && nIndex == 0)
			|| (pInput->Press(DIK_S) && nIndex == 0)
			|| (pInput->Press(DIK_D) && nIndex == 0)
			|| pInput->Press(JOYPAD_RIGHT, nIndex)
			|| pInput->Press(JOYPAD_LEFT, nIndex)
			|| pInput->Press(JOYPAD_UP, nIndex)
			|| pInput->Press(JOYPAD_DOWN, nIndex))
		{
			// ����
			SetMotion(MOTION_WALK);
			SetMove(true);
		}
		else
		{
			// �������I��������
			SetMotion(MOTION_NEUTRAL);
		}
		// �ړ��ʂ��X�V
		CCharacter::SetMove(move);

		// �ړ�����
		CCharacter::Move();
	}
}

//==============================================================================================
// �A�N�V����
//==============================================================================================
void CPlayer::Action()
{
	//���̓f�o�C�X�̏��
	CInput* pInput = CInput::GetKey();

	// �v���C���[�̔ԍ�
	const int nIndex = GetIndex();

	// �U�����鏈��
	if (((pInput->Trigger(DIK_SPACE) && nIndex == 0)
		|| (pInput->Trigger(JOYPAD_B, nIndex)) || pInput->Trigger(JOYPAD_A, nIndex))
		&& GetMotion_Type() != MOTION_MINE
		&& m_bEntry)
	{
		// �U�����[�V����
		SetMotion(MOTION_MINE);
		
		// �U������
		CCharacter::SetAttack(true);
	}

	// �G���g���[���[�h�̃G���g���[�������I�������g�p����
	if (m_bEntry == false)
	{
		m_bEntry = true;
	}
}