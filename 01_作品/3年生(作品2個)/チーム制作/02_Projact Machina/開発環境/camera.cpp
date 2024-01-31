//==============================================
//
// �J�����̏���[camera.cpp]
// Author�Ftutida ryousei
// Author�Ftanimoto kosuke
// Author�Fsaito shian
//
//==============================================
#include "camera.h"
#include "input.h"
#include "application.h"
#include "game.h"
#include "tutorial.h"
#include "player_manager.h"
#include "mode.h"
#include "fade.h"
#include"debugProc.h"
#include "utility.h"

//==============================================
//�R���X�g���N�^
//==============================================
CCamera::CCamera()
{

}

//==============================================
//�f�X�g���N�^
//==============================================
CCamera::~CCamera()
{

}

//==============================================
//�J�����̏���������
//==============================================
void CCamera::Init(void)
{
	//���_�E�����_�E�������ݒ肷��
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���_(�I�t�Z�b�g)
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �����_(�I�t�Z�b�g)
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// ������x�N�g��
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �p�x
	m_fRotSpeed = 0.03f;							// ��]���x
	m_CPos = { 0.0f,0.0f,0.0f };					// �L�����N�^�[�̈ʒu
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړI�̎��_
	m_posRDest = D3DXVECTOR3(600.0f, 150.0f, 0.0f);	// �ړI�̒����_
	m_bPerspective = false;
	m_bValue = false;
	m_bOpening = false;								// �I�[�v�j���O�̗L��

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		m_bOpening_step[nCnt] = false;
	}
}

//==============================================
//�J�����̏I������
//==============================================
void CCamera::Uninit(void)
{

}

//==============================================
//�J�����̍X�V����
//==============================================
void CCamera::Update(void)
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	if (((Mode == CApplication::MODE_GAME && CGame::GetPlayerManager()->GetPlayer(0) != nullptr)
		|| (Mode == CApplication::MODE_TUTORIAL && CTutorial::GetPlayerManager()->GetPlayer(0) != nullptr))
		&& !m_bOpening)
	{
		// ���_�ړ�
		Perspective();
	}
	else if (CApplication::GetModeType() == CApplication::MODE_TITLE
		|| CApplication::GetModeType() == CApplication::MODE_CHAR_SELECT
		|| CApplication::GetModeType() == CApplication::MODE_RESULT)
	{
		Matrix(D3DXVECTOR3(-0.5f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 600.0f, 0.0f));
	}

#ifdef _DEBUG
	// �J�����̓��͏���
	DebugInput();
#endif // !_DEBUG

	if ((Mode == CApplication::MODE_GAME || Mode == CApplication::MODE_TUTORIAL))
	{
		// ��ʂ̗h��
		if (m_bTremor)
			Tremor();
		// �I�[�v�j���O
		if (m_bOpening)
			Opening_Move();
	}
}

//==============================================
// �J�����̃Z�b�g����
//==============================================
void CCamera::SetCamera(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X�̐���
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_worldCameraPosV,
		&m_worldCameraPosR,
		&m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̐���
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)1280.0f / (float)720.0f,
		10.0f,
		50000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//==============================================
// �s����g�����J��������
//==============================================
void CCamera::Matrix(D3DXVECTOR3 rot, D3DXVECTOR3 pos)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�̃}�g���b�N�X

	// ���[���h�}�g���b�N�X
	D3DXMatrixIdentity(&m_mtxWorld);

	// �s�����]�ɔ��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �s��Ɉړ��𔽉f(�v���C���[�̈ʒu)
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	D3DXVec3TransformCoord(&m_worldCameraPosV, &m_posV, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldCameraPosR, &m_posR, &m_mtxWorld);
}

//==============================================
// �J�����̓��͏���
//==============================================
void CCamera::DebugInput()
{
	// ���̓f�o�C�X�̏��
	CInput* pInput = CInput::GetKey();

	// �J�������_�؂�ւ�
	if (pInput->Trigger(DIK_F1))
	{
		m_bPerspective = !m_bPerspective;
	}

	// �J�������_��؂�ւ����ꍇ
	if (m_bPerspective == true)
	{
		// �J�����̈ړ�����
		Move();
	}
}

//==============================================
// �J�����̈ړ�
//==============================================
void CCamera::Move()
{
	// ���̓f�o�C�X�̏��
	CInput* pInput = CInput::GetKey();

	// �J�����̈ړ�
	if (pInput->Press(DIK_RIGHT))
	{
		//m_rot.y += m_fRotSpeed;
		m_posV.x += 10.0f;
		m_posR.x += 10.0f;
	}
	if (pInput->Press(DIK_LEFT))
	{
		//m_rot.y -= m_fRotSpeed;
		m_posV.x -= 10.0f;
		m_posR.x -= 10.0f;
	}
	if (pInput->Press(DIK_UP))
	{
		//m_rot.x -= m_fRotSpeed;
		m_posV.z += 10.0f;
		m_posR.z += 10.0f;
	}
	if (pInput->Press(DIK_DOWN))
	{
		//m_rot.x += m_fRotSpeed;
		m_posV.z -= 10.0f;
		m_posR.z -= 10.0f;
	}

	if (pInput->Press(DIK_RSHIFT))
	{
		//m_rot.x -= m_fRotSpeed;
		m_posV.y += 10.0f;
		m_posR.y += 10.0f;
	}
	if (pInput->Press(DIK_RCONTROL))
	{
		//m_rot.x += m_fRotSpeed;
		m_posV.y -= 10.0f;
		m_posR.y -= 10.0f;
	}
	if (pInput->Press(DIK_Q))
	{
		m_rot.y -= m_fRotSpeed;
	}
	if (pInput->Press(DIK_E))
	{
		m_rot.y += m_fRotSpeed;
	}

	// �p�x�̐��K��
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
}

//==============================================
// �J�����̎��_
//==============================================
void CCamera::Perspective()
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	CPlayer* pPlayer = nullptr;

	// ���[�h���Ƀv���C���[��ǂݍ���
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayerManager()->GetPlayer(0);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pPlayer = CGame::GetPlayerManager()->GetPlayer(0);
	}

	if (m_bPerspective == false)
	{	// �v���C���[���g���Ă�������s

		//�擾����
		m_PPos = pPlayer->GetPos();
		m_bValue = false;
	}
	else if (m_bPerspective == true && m_bValue == false)
	{	// �v���C���[���g���Ă�������s

		//�擾����
		m_PPos = pPlayer->GetPos();
		m_bValue = true;
	}
	// �s����g�����J��������
	Matrix(m_rot, { m_PPos.x, m_PPos.y, m_PPos.z });
}

//==============================================
// �I�[�v�j���O
//==============================================
void CCamera::Opening_Move()
{
	// ���̓f�o�C�X�̏��
	CInput* pInput = CInput::GetKey();

	// �X�L�b�v
	if (pInput->Trigger(DIK_RETURN) || pInput->Trigger(JOYPAD_START))
	{
		m_posV = { 0.0f, 225.0f, -450.0f };
		m_posR = { 0.0f, 112.5f, 450.0f };

		// �I�[�v�j���O�I��
		m_bOpening = false;
	}

	// �s����g�����J��������
	Matrix(m_rot, { 0.0f,0.0f,0.0f });

	if (!m_bOpening_step[0])
	{// �X�e�b�v0
		m_nChange_Count++;

		// �����_�̈ړ�
		if (m_posR.y > 80.0f)
			m_posR.y -= 29.0f;
		else if(m_posR.y <= 80.0f && !m_bTremor)
		{
			// �J�����̗h��
			m_Base_PosV = m_posV;
			m_Base_PosR = m_posR;

			m_fMax_Scale = 15.0f;
			m_bTremor = true;
		}

		if (m_nChange_Count == 300)
		{
			// ���̃X�e�b�v�ɐi�߂�
			m_nChange_Count = 0;
			m_bOpening_step[0] = true;

			m_posV = { 1500.0f,1000.0f,-5000.0f };
			m_posR = { 0.0f,1000.0f,0.0f };

			m_bTremor = false;
		}
	}
	else if (m_bOpening_step[0] && !m_bOpening_step[1])
	{// �X�e�b�v1
		m_nChange_Count++;

		float f = D3DX_PI * 2 / 0.05f;

		if (m_nChange_Count < f)
			m_rot.y -= 0.05f;

		// 1�t���[���̈ړ���
		float X = 1500 / f;
		float Z = 4000 / f;
		float Y = 500 / f;

		// ���_�A�����_�̈ړ�
		if (m_posV.x > 0)
			m_posV.x -= X;
		if (m_posV.z < -1000)
			m_posV.z += Z;
		if (m_posV.y < 1500)
		{
			m_posV.y += Y;
			m_posR.y += Y;
		}

		if (m_nChange_Count == 160)
		{
			// ���̃X�e�b�v�ɐi�߂�
			m_nChange_Count = 0;
			m_bOpening_step[1] = true;

			m_posV = { 0.0f,1500.0f,-1000.0f };
			m_posR = { 0.0f,1500.0f,0.0f };
			m_rot.y = 0.0f;
		}
	}
	else if (m_bOpening_step[1] && !m_bOpening_step[2])
	{// �X�e�b�v2
		m_nChange_Count++;

		// 1�t���[���̈ړ���
		float X = 4000 / 20;
		float Y = 1400 / 20;

		// ���_�̈ړ�
		if (m_posV.z > -5000)
			m_posV.z -= X;
		if (m_posV.y > 100)
			m_posV.y -= Y;

		if (m_nChange_Count == 100)
		{
			// ���̃X�e�b�v�ɐi�߂�
			m_nChange_Count = 0;
			m_bOpening_step[2] = true;

			m_posV = { 0.0f, 225.0f, -450.0f };
			m_posR = { 0.0f, 112.5f, 450.0f };

			// �I�[�v�j���O�I��
			m_bOpening = false;
		}
	}
}

//==============================================
// ��ʂ̗h��
//==============================================
void CCamera::Tremor()
{
	// �����_���Ȑ��l
	float fRand_TreX = utility::Random<float>(m_fMax_Scale, -m_fMax_Scale);
	float fRand_TreY = utility::Random<float>(m_fMax_Scale, -m_fMax_Scale);

	// ���_�̈ړ�
	m_posV.x = m_Base_PosV.x + fRand_TreX;
	m_posV.y = m_Base_PosV.y + fRand_TreY;

	// �����_�̈ړ�
	m_posR.x = m_Base_PosR.x + fRand_TreX;
	m_posR.y = m_Base_PosR.y + fRand_TreY;

	// �h�������������������
	if (m_fMax_Scale > 0)
		m_fMax_Scale -= 0.1f;
}