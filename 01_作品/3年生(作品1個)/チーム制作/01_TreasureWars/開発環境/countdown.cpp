//==============================================================================================
//
// �J�E���g�_�E���@�@�@countdown.cpp
// tutida ryousei
//
//==============================================================================================
#include"countdown.h"
#include"number.h"
#include "application.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CCountdown::CCountdown() : CObject(3)
{
	m_nCountTime = 0;			// �n�܂�܂ł̎���
	m_nCountFrame = 0;			// ���Ԃ̃J�E���g
	m_nStart_Count = 0;			// �X�^�[�g��\�����鎞��
	m_pObject2D = nullptr;		// �X�^�[�g
	m_Start_Size = { 0.0f,0.0f,0.0f };	// �X�^�[�g�̃T�C�Y
	m_bDisplay_Start = false;	// �X�^�[�g��\��������
	m_bStartScale = true;	// �X�^�[�g�̊g�唻��
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CCountdown::~CCountdown()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CCountdown::Init()
{
	m_nCountTime = 3;		// �n�܂�܂ł̎���
	m_Start_Size = { 250.0f,0.0f,0.0f };	// �X�^�[�g�̃T�C�Y
	m_pObject2D = nullptr;	// �X�^�[�g

	// �i���o�[�̐���
	m_pNumber = CNumber::Create({ 1280.0f / 2,720.0f / 2,0.0f }, 100.0f, 100.0f);

	// �i���o�[�̐F
	m_pNumber->SetCol({ 1.0f,1.0f,1.0f,1.0f });

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CCountdown::Uninit()
{
	Release();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CCountdown::Update()
{
	// �J�E���g�_�E�����I������܂�
	if (m_bDisplay_Start == false)
	{
		// 1�b�o����
		if (m_nCountFrame % 60 == 0)
		{
			// �ŏ��ł͂Ȃ��ꍇ
			if (m_nCountFrame != 0)
			{
				// ��b���炷
				m_nCountTime--;
			}

			// �J�E���g��0�ɂȂ���
			if (m_nCountTime <= 0)
			{
				// �Q�[���J�nSE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_GAMESTART);

				// �i���o�[���I��������
				m_pNumber->Uninit();
				m_pNumber = nullptr;

				// �X�^�[�g
				CApplication::SetGameStart(true);

				// �X�^�[�g��\��
				m_pObject2D = CObject2D::Create({ 1280.0f / 2,720.0f / 2,0.0f }, m_Start_Size);
				m_pObject2D->SetTexture(CTexture::TEXTURE_START);

				// �X�^�[�g��\��������
				m_bDisplay_Start = true;
			}
			// �J�E���g��0�ł͂Ȃ��ꍇ
			else
			{
				// �J�E���g�_�E���̐ݒ�
				SetCountdown(m_nCountTime);
			}
		}
		// 60�t���[�����J�E���g����
		m_nCountFrame++;
	}
	else
	{
		// �g��̔���
		if (m_bStartScale == true)
		{
			m_Start_Size.y += 2;

			// �X�^�[�g�̃T�C�Y��ς���
			if (m_Start_Size.y > 70.0f)
			{
				m_bStartScale = false;
			}
		}
		else
		{
			m_Start_Size.y -= 2;
		}

		// �傫����0�ȉ��ɂȂ��Ă��܂����ꍇ
		if (m_Start_Size.y <= 0.0f)
		{
			// �X�^�[�g�̏I������
			m_pObject2D->Uninit();
			m_pObject2D = nullptr;

			// ���g�̏I������
			CCountdown::Uninit();
		}
		// �傫�����c���Ă���ꍇ
		else
		{
			// �T�C�Y�̍X�V
			m_pObject2D->SetSize(m_Start_Size.x, m_Start_Size.y);
		}
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CCountdown::Draw()
{

}

//==============================================================================================
// ��������
//==============================================================================================
CCountdown *CCountdown::Create()
{
	CCountdown *pCountdown = nullptr;

	pCountdown = new CCountdown;

	if (pCountdown != nullptr)
	{
		pCountdown->Init();
	}

	return pCountdown;
}

//==============================================================================================
// �J�E���g�_�E���̐ݒ�
//==============================================================================================
void CCountdown::SetCountdown(int countTime)
{
	// �J�E���g�_�E��SE
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNTDOWN);

	// �e�N�X�`�����W�̐ݒ�
	m_pNumber->SetTexPos(0.0f, 1.0f, 0.1f * countTime + 0.1f, countTime * 0.1f);
}