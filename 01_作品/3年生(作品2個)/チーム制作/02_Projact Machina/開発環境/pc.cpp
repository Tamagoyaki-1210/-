//=============================================================================
//
// pc.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "pc.h"
#include "application.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "meshfield.h"
#include "energy_gauge.h"
#include "tutorial.h"
#include "locus.h"
#include "player_life_gauge.h"
#include "pause.h"
#include "map.h"
#include "restraint_switch.h"
#include "particle_emitter.h"

#include"player_manager.h"
#include"debugProc.h"

#include "sound.h"

#include "result.h"

//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CPC::CPC()
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CPC::~CPC()
{

}

//============================================================================
// ����������
//============================================================================
HRESULT CPC::Init()
{
	if (CApplication::GetModeType() == CApplication::MODE_GAME)
	{
		SetEnergyGauge(CEnergy_Gauge::Create({ 70,720.0f / 2,0.0f }, { 20.0f,500.0f }));
		SetGaugeManager(CPlayer_Life_Gauge::Create({ 1210.0f,720.0f / 2,0.0f }, { 20.0f,500.0f }));
	}

	CPlayer::Init();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CPC::Uninit()
{
	CPlayer::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CPC::Update()
{
	if (!CApplication::GetCamera()->GetOpening())
		// ���͏���
		Input();

	CPlayer::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CPC::Draw()
{
	CPlayer::Draw();
}

//==============================================================================================
// ���͏���
//==============================================================================================
void CPC::Input()
{
	// ���U���g�ł͂Ȃ��ꍇ && �_���[�W������Ȃ��ꍇ
	if (CApplication::GetModeType() != CApplication::MODE_RESULT && GetCollisionNoneHit() == false)
	{
	//���̓f�o�C�X�̏��
	CInput* pInput = CInput::GetKey();

	// ���g�̔ԍ�
	//const int nIndex = GetCharaIndex();

	// �J�����̊p�x
	D3DXVECTOR3 rotCamera = CApplication::GetCamera()->GetRot();

	// �ړ���
	D3DXVECTOR3 move = GetMove();

	// �G�l���M�[�Q�[�W�̎擾
	CEnergy_Gauge* pGauge = nullptr;
	pGauge = GetEnergy_Gauge();

	D3DXVECTOR3 boostMove = { 1.0f,1.0f,1.0f };

	// �u�[�X�g���͈ړ����x���オ�� && �Q�[�W���c���Ă���ꍇ
	if (GetBoost() && !pGauge->GetConsumption())
		boostMove *= 3.0f;

	// �ړI�̊p�x
	D3DXVECTOR3 rotDest = GetRotDest();

	// �r�p�[�c
	CParts* pLeg = GetParts(PARTS_LEG);

	// ��������
	bool bWalk = false;

	// �����Ă���ꍇ
	if (pInput->MovePress(GAME_MOVE_ALL))
	{
		// ��]������
		Rotation();

		// ���������^�ɂ���
		bWalk = true;
	}

	// �����Ă���ꍇ
	if (bWalk == true && !GetAvoidance())
	{
		//�J�����̌����iY���̂݁j
		float rotY = rotCamera.y;

		//���_�ړ�
		if (pInput->MovePress(GAME_MOVE_UP))
		{//��L�[�������ꂽ
			if (pInput->MovePress(GAME_MOVE_LEFT))
			{
				rotDest.y = rotCamera.y + D3DX_PI * 0.75f;
				move.x = -sinf(rotY + D3DX_PI * 0.75f) * boostMove.x;
				move.z = -cosf(rotY + D3DX_PI * 0.75f) * boostMove.z;
			}
			else if (pInput->MovePress(GAME_MOVE_RIGHT))
			{
				rotDest.y = rotCamera.y + D3DX_PI * -0.75f;
				move.x = -sinf(rotY + D3DX_PI * -0.75f) * boostMove.x;
				move.z = -cosf(rotY + D3DX_PI * -0.75f) * boostMove.z;
			}
			else
			{
				rotDest.y = rotCamera.y + D3DX_PI;
				move.x = sinf(rotY) * boostMove.x;
				move.z = cosf(rotY) * boostMove.z;
			}
		}

		else if (pInput->MovePress(GAME_MOVE_DOWN))
		{//���L�[�������ꂽ
			if (pInput->MovePress(GAME_MOVE_LEFT))
			{
				rotDest.y = rotCamera.y + D3DX_PI * 0.25f;
				move.x = -sinf(rotY + D3DX_PI * 0.25f) * boostMove.x;
				move.z = -cosf(rotY + D3DX_PI * 0.25f) * boostMove.z;
			}
			else if (pInput->MovePress(GAME_MOVE_RIGHT))
			{
				rotDest.y = rotCamera.y + D3DX_PI * -0.25f;
				move.x = -sinf(rotY + D3DX_PI * -0.25f) * boostMove.x;
				move.z = -cosf(rotY + D3DX_PI * -0.25f) * boostMove.z;
			}
			else
			{
				rotDest.y = rotCamera.y;
				move.x = sinf(rotY + D3DX_PI) * boostMove.x;
				move.z = cosf(rotY + D3DX_PI) * boostMove.z;
			}
		}
		else if (pInput->MovePress(GAME_MOVE_LEFT))
		{//���L�[�������ꂽ
			rotDest.y = rotCamera.y + D3DX_PI * 0.5f;
			move.x = sinf(rotY + D3DX_PI * -0.5f) * boostMove.x;
			move.z = cosf(rotY + D3DX_PI * -0.5f) * boostMove.z;
		}
		else if (pInput->MovePress(GAME_MOVE_RIGHT))
		{//�E�L�[�������ꂽ
			rotDest.y = rotCamera.y + D3DX_PI * -0.5f;
			move.x = sinf(rotY + D3DX_PI * 0.5f) * boostMove.x;
			move.z = cosf(rotY + D3DX_PI * 0.5f) * boostMove.z;
		}

		// �ڒn���Ă���ꍇ�ɕ������[�V����
		if (GetGround())
		{
			// ����
			pLeg->SetMotion(MOTION_WALK);

			if (m_nWalk_Counter >= 20)
			{
				// ����SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_PLAYERWALK);
				m_nWalk_Counter = 0;
			}
			else
			{
				m_nWalk_Counter++;
			}
		}
		// ���
		if ((pInput->Trigger(MOUSE_INPUT_RIGHT) || pInput->Trigger(JOYPAD_X))
			&& !pGauge->GetConsumption())
		{
			// ����
			GetParts(PARTS_LEG)->SetMotion(MOTION_WALK);
			SetAvoidanceCount(20);				// ����̍d��
			pGauge->SetAvoidance_amount(200.0f);// ������̃G�l���M�[�����
			pGauge->Avoidance_Energy();			// �G�l���M�[����
			pGauge->Recovery_Pause(50);			// �N�[���^�C��
			SetAvoidance(true);

			// �u�[�X�g�������̑��x�����炷
			if (GetBoost())
				move /= boostMove.x;

			move *= 7.0f;		// ����
			move.y = 0.0f;
		}
	}
	// �O�񃂁[�V�������������[�V�����������ꍇ
	else if (pLeg->GetCurrentMotion() == MOTION_WALK && GetGround())
	{
		// �������I��������
		pLeg->SetMotion(MOTION_NEUTRAL);
		m_nWalk_Counter = 0;
	}

	// �ړ��ʂ��X�V
	CCharacter::SetMove(move);
	// �ړI�̊p�x�̐ݒ�
	CCharacter::SetRotDest(rotDest);

	/*if (pInput->Trigger(DIK_F))
	{
		CMeshField *pMeshField = nullptr;

		if (CApplication::GetModeType() == CApplication::MODE_GAME)
			pMeshField = CGame::GetMeshField();
		else if (CApplication::GetModeType() == CApplication::MODE_TUTORIAL)
			pMeshField = CTutorial::GetMeshField();

		if (pMeshField != nullptr)
			pMeshField->Ground_Broken(CCharacter::GetPos(), 30.0f, 10);
	}*/
	// �W�����v����
	if ((pInput->Press(DIK_SPACE)) || pInput->Press(JOYPAD_A))
	{
		// �W�����v���͎��Ԃ̉��Z
		AddJump_PressCount(1);

		if (GetJump_PressCount() < 20)
			// �v���C���[�̃W�����v����
			JumpStart();
		else
		{
			if (GetJump_Boost() == false)
			{
				// �u�[�X�gSE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_PLAYERBOOST);
			}

			SetJump_Boost(true);
		}
	}
	else
		SetJump_Boost(false);

	if (GetGround())
		// �W�����v���͎��Ԃ̃��Z�b�g
		SetJump_PressCount(0);

	CPause *pPause = nullptr;

		// �|�[�Y�̎擾
		if (CApplication::GetModeType() == CApplication::MODE_GAME)
			pPause = CGame::GetPause();
		else if (CApplication::GetModeType() == CApplication::MODE_TUTORIAL)
			pPause = CTutorial::GetPause();

		// �|�[�Y���ĂȂ��Ƃ�
		if (!pPause->GetPause())
		{
			// ���_����
			Perspective();
		}

		if (pGauge != nullptr)
		{
			// �n��ɂ���ꍇ
			if (GetGround())
			{
				// ����x
				pGauge->SetConsumption_Speed(50.0f);

				// �񕜑��x
				pGauge->SetRecovery_Speed(1000.0f);
			}
			// �󒆂ɂ���ꍇ
			else
			{
				// ����x
				pGauge->SetConsumption_Speed(75.0f);

				// �񕜑��x
				pGauge->SetRecovery_Speed(30.0f);
			}

			// �G�l���M�[���c���Ă�����
			if (!pGauge->GetConsumption())
			{
				// �U������
				if ((pInput->Press(DIK_B)) || pInput->Press(JOYPAD_R2) || pInput->Press(MOUSE_INPUT_LEFT))
				{
					// �v���C���[�̍U������
					if (GetStandbyAttack() == true)
					{
						SetAdditionalAttack(true);
					}
					else
					{
						SetPlayerAttack(true);
					}
				}

				if (GetJump_Boost())
				{
					// �W�����v�u�[�X�g
					JumpBoost();
				}

				// �u�[�X�ƃ_�b�V��
				if ((pInput->Press(DIK_LSHIFT) || pInput->Press(JOYPAD_L1)) && bWalk)
				{
					if (GetBoost() == false)
					{
						// �u�[�X�gSE
						CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_PLAYERBOOST);
					}

					// �u�[�X�g����
					SetBoost(true);

					// �G�l���M�[�������
					pGauge->Consumption_Gauge();
					pLeg->SetMotion(MOTION_BOOST_RUN);
				}
				else
				{
					// �_�b�V���u�[�X�g�̏�����
					SetBoost(false);
				}
			}
		}

		// �p�[�c�ύX����
		if (GetDropContact() == true)
		{
			// E�{�^���ŗ��Ƃ����̎擾
			if (pInput->Trigger(DIK_E) || pInput->Trigger(JOYPAD_R1))
			{
				SetDropGet(true);
			}
		}

		for (int nCnt = 0; nCnt < CRestraint_Switch::SWITHC_NUM; nCnt++)
		{
			// �S���X�C�b�`�̎擾
			CRestraint_Switch *pRestraint = CGame::GetMap()->GetRestraint_Switch(nCnt);

			if (pRestraint != nullptr && pRestraint->GetHit())
			{
				if (pInput->Trigger(DIK_E) || pInput->Trigger(JOYPAD_R1))
				// �{�^�������������̃J�E���g
					pRestraint->AddCountSwitch();
			}
		}
	}
}

//============================================================================
// �j�󏈗�
//============================================================================
void CPC::Destroy()
{
	std::move(CParticleEmitter::Create("burst", GetPos()));

	CPlayer::Destroy();

	CPlayerManager *pPlayerManager = CGame::GetPlayerManager();

	pPlayerManager->SetPlayer({ 0.0f, 0.0f, 0.0f }, CPlayerManager::TYPE_PC, 0);
}

//============================================================================
// ���_�̌����̏���
//============================================================================
void CPC::Perspective()
{
	//���̓f�o�C�X�̏��
	CInput* pInput = CInput::GetKey();
	// �J�����̊p�x
	D3DXVECTOR3 rotCamera = CApplication::GetCamera()->GetRot();

	if (pInput->Press(DIK_UP) || pInput->StickPress(JOYKEY_CROSS_UP, 0, true))
	{	//UP�L�[�������Ă���Ƃ�
		rotCamera.x -= 0.02f;		//�J�����̏�����̉��Z
		if (rotCamera.x <= -0.5f)
		{
			rotCamera.x = -0.5f;
		}

	}
	if (pInput->Press(DIK_DOWN) || pInput->StickPress(JOYKEY_CROSS_DOWN, 0, true))
	{	//DOWN�L�[�������Ă���Ƃ�
		rotCamera.x += 0.02f;		//�J�����̏�����̉��Z
		if (rotCamera.x >= 0.5f)
		{
			rotCamera.x = 0.5f;
		}
	}

	if (pInput->Press(DIK_LEFT) || pInput->StickPress(JOYKEY_CROSS_LEFT, 0, true))
	{	//LEFT�L�[�������Ă���Ƃ�
		rotCamera.y -= 0.02f;		//�J�����̏�����̉��Z
	}

	if (pInput->Press(DIK_RIGHT) || pInput->StickPress(JOYKEY_CROSS_RIGHT, 0, true))
	{	//LEFT�L�[�������Ă���Ƃ�
		rotCamera.y += 0.02f;		//�J�����̏�����̉��Z
	}

	D3DXVECTOR3 MouseMove;
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	MouseMove = pInput->GetMouseMove();

	MouseMove = D3DXVECTOR3(MouseMove.y, MouseMove.x, 0.0f);

	if (D3DXVec3Length(&MouseMove) > 0.25f)
	{
		D3DXVec3Normalize(&MouseMove, &MouseMove);

		rot = MouseMove * (D3DX_PI / 180.0f);

		rot.x *= 3.0f;
		rot.y *= 2.0f;
	}

	rotCamera += rot;

	if (rotCamera.x  > D3DXToRadian(80))
	{
		rotCamera.x = D3DXToRadian(80);
	}
	else if (rotCamera.x  < D3DXToRadian(-50))
	{
		rotCamera.x = D3DXToRadian(-50);
	}

	rot = rotCamera;

	if (rot.x > D3DX_PI)
	{
		rot.x -= D3DX_PI * 2.0f;
	}
	else if (rot.x < -D3DX_PI)
	{
		rot.x += D3DX_PI * 2.0f;
	}

	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	else if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}

	if (rot.z > D3DX_PI)
	{
		rot.z -= D3DX_PI * 2.0f;
	}
	else if (rot.z < -D3DX_PI)
	{
		rot.z += D3DX_PI * 2.0f;
	}

	rotCamera.y = rot.y;
	rotCamera.x = rot.x;

	//�J�����̌����̐ݒ�
	CApplication::GetCamera()->SetRot(rotCamera);
}

//============================================================================
// ��������
//============================================================================
CPC* CPC::Create(const D3DXVECTOR3 pos, const int index)
{
	CPC* pPC = new CPC;

	pPC->SetPos(pos);
	pPC->SetCharaIndex(index);
	pPC->Init();

	return pPC;
}
