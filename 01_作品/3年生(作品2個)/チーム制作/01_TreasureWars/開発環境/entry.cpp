//==============================================================================================
//
// �G���g���[�@�@�@entry.cpp
// tutida ryousei
//
//==============================================================================================
#include"entry.h"
#include"camera.h"
#include"input.h"
#include"fade.h"
#include"file.h"
#include"character.h"
#include "stage_manager.h"
#include "object2D.h"
#include "fontString.h"
#include "charmanager.h"
#include "objectX.h"
#include "skybox.h"
#include "debugproc.h"
#include "stage_select.h"

bool CEntry::m_bEntry[] = {};
bool CEntry::m_bIn[] = {};
bool CEntry::m_bTutorial = false;

const D3DXVECTOR3 CEntry::TVSizeMax = { 920.0f, 520.0f, 0.0f };		// TV��ʂ̍ő�T�C�Y
const float CEntry::TVDisplaySpeed = 0.5f;							// TV��ʂ̕\�����x
const float CEntry::TVStartAnchorAttenuation = 0.1f;				// TV�J�n�A���J�[����
const float CEntry::TVEndAnchorAttenuation = 0.05f;					// TV�I���A���J�[����
//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CEntry::CEntry()
{

}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CEntry::~CEntry()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CEntry::Init()
{
	// �G���g���[���BGM
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_ENTRY);

	// �J�����̃|�C���^
	CCamera* pCamera = CApplication::GetCamera();

	// ���_�A�����_�̐ݒ�
	pCamera->SetPosV({ 75.0f, 145.0f, -500.0f });
	pCamera->SetPosR({ 75.0f, 40.0f, 0.0f });

	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		m_bEntry[nCnt] = false;
		m_bIn[nCnt] = false;
		m_CharPos[nCnt] = { 0.0f,0.0f,500.0f };
		m_pChara[nCnt] = nullptr;
		m_CharPosDest[nCnt] = { 175.0f + (nCnt * 20.0f), 0.0f, 180.0f };
	}

	// �X�J�C�{�b�N�X�̐���
	CSkyBox *pSkyBox = CSkyBox::Create(D3DXVECTOR3(0.0f, -250.0f, 0.0f));
	pSkyBox->SetTexture(CTexture::TEXTURE_WINDOW_OUTSIDE);

	// �X�e�[�W�̐���
	CApplication::GetStageManager()->Create("TEST");

	// �Q�[���X�^�[�g��
	m_pGameStartFont = CFontString::Create({ 720.0f , 50.0f, 0.0f }, { 20.0f, 20.0f, 0.0f }, "START�{�^���ŃQ�[���X�^�[�g!");

	// �`���[�g���A����
	m_apTutorialFont[0] = CFontString::Create({ 900.0f , 280.0f, 0.0f }, { 25.0f, 25.0f, 0.0f }, "�`���[�g���A����");
	m_apTutorialFont[1] = CFontString::Create({ 960.0f , 320.0f, 0.0f }, { 25.0f, 25.0f, 0.0f }, "�͂��߂�");
	m_pABotton = CObject2D::Create({ 960.0f, 380.0f, 0.0f }, { 70.0f, 70.0f, 0.0f });
	m_pABotton->SetTexture(CInput::GetKey()->IsConnected(0) ? CTexture::TEXTURE_BUTTON_A : CTexture::TEXTURE_BUTTON_SPACE);

	// �v���C���[�ԍ��̕\��
	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		m_apPlayerNumber[nCnt] = CObject2D::Create({ 400.0f + (160.0f * nCnt), 600.0f, 0.0f }, { 80.0f, 80.0f, 0.0f });
		m_apPlayerNumber[nCnt]->SetTexture(CTexture::TEXTURE_CHARA_TYPE);
		m_apPlayerNumber[nCnt]->SetTexPos(0.0f, 1.0f, 0.2f + (0.2f * nCnt), 0.0f + (0.2f * nCnt));

		m_apABotton[nCnt] = CObject2D::Create({ 400.0f + (160.0f * nCnt), 680.0f, 0.0f }, { 60.0f, 60.0f, 0.0f });
		m_apABotton[nCnt]->SetTexture(nCnt == 0 ?
			CInput::GetKey()->IsConnected(0) ? CTexture::TEXTURE_BUTTON_A : CTexture::TEXTURE_BUTTON_SPACE
			: CTexture::TEXTURE_BUTTON_A);
	}
	m_nSellect = 0;
	m_nDisplayEndCounter = 0;

	m_bTutorialStart = false;		// �`���[�g���A���̊J�n����
	m_bTutorialEnd = false;			// �e���r��ʂ̕\���̏I������
	m_bTVDisplayStart = false;		// �e���r��ʂ̕\���̊J�n����
	m_bTVDisplayEnd = false;		// �e���r��ʂ̕\���̏I������

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CEntry::Uninit()
{
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CEntry::Update()
{
	// �`���[�g���A�����ł͂Ȃ��ꍇ
	if (m_bTutorial == false)
	{
		// ���͏����̎擾
		CInput* pInput = CInput::GetKey();

		// �R���g���[���[4���̏���
		for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
		{
			// �{�^���������ꂽ��v���C���[�𑝂₷
			if ((pInput->Trigger(JOYPAD_B, nCnt) || pInput->Trigger(JOYPAD_A, nCnt)
				|| ((pInput->Trigger(DIK_P) || pInput->Trigger(DIK_SPACE)) && nCnt == 0)
				&& CFade::GetFade() == CFade::FADE_NONE)
				&& !m_bEntry[nCnt])
			{
				// �G���g���[������
				m_bEntry[nCnt] = true;

				// �Ή�����A�{�^�����I��������
				m_apABotton[nCnt]->Uninit();
				m_apABotton[nCnt] = nullptr;

				// OK����
				m_apOKString[nCnt] = CFontString::Create(D3DXVECTOR3(400.0f + (160.0f * nCnt) - 25.0f, 680.0f, 0.0f), { 50.0f, 50.0f, 0.0f }, "OK");
				m_apOKString[nCnt]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
				m_apOKString[nCnt]->SetScale(true);
			}

			// �G���g���[����Ă���ꍇ
			if (m_bEntry[nCnt])
			{
				// �v���C���[�̓���
				PlayerIn(nCnt);
			}

			// ��ʐ���
			if ((pInput->Trigger(JOYPAD_START, nCnt)
				|| pInput->Trigger(DIK_RETURN))
				&& CFade::GetFade() == CFade::FADE_NONE)
			{
				// ����SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);

				// �X�e�[�W�̃I�u�W�F�N�g�̍폜
				CApplication::GetStageManager()->Clear();

				// ��ʑJ��
				CFade::SetFade(CApplication::MODE_STAGE_SELECT, 0.05f);

				CStage_Select::SetStage_Initiative(nCnt);
				return;
			}
		}

		// �s������
		Act_Limit();

		// �`���[�g���A���̓����蔻��
		TutorialCol();

		// �`���[�g���A���̏I������
		TutorialEnd();
	}
	else
	{
		// �`���[�g���A���̏���
		Tutorial();
	}

}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CEntry::Draw()
{
	
}

//==============================================================================================
// ��������
//==============================================================================================
CEntry *CEntry::Create()
{
	CEntry *pEntry = nullptr;

	pEntry = new CEntry;

	if (pEntry != nullptr)
	{
		pEntry->Init();
	}

	return pEntry;
}

//==============================================================================================
// �`���[�g���A���̓����蔻��
//==============================================================================================
void CEntry::TutorialCol()
{
	bool bColl = false;	// �����蔻��

	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		// �G���g���[���Ă���ꍇ
		if (m_bEntry[nCnt] == true)
		{
			CCharacter* pChara = CApplication::GetCharManager()->GetChara(nCnt);

			// �����蔻��
			if(pChara->CollisionCircle(pChara->GetPos(), pChara->GetRadius(), { 400.0f, 0.0f, -100.0f }, 200))
			{
				// �����蔻���^�ɂ���
				bColl = true;

				// �������g�傳����
				for (int nFont = 0; nFont < 2; nFont++)
				{
					m_apTutorialFont[nFont]->SetScale(true);
				}

				// ���͏����̎擾
				CInput* pInput = CInput::GetKey();

				if (pInput->Trigger(JOYPAD_B, nCnt) || pInput->Trigger(JOYPAD_A, nCnt) || pInput->Trigger(DIK_SPACE))
				{
					// �`���[�g���A�����J�n����
					m_bTutorial = true;
					m_bTutorialStart = true;
					SetEntryUX(false);

					// �e���r_�J�nSE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_TV_START);

					// �`���[�g���A���̎哱����ݒ�
					m_nTutorialInitiative = nCnt;

					return;
				}
			}
		}
	}

	// �N���������Ă��Ȃ��ꍇ
	if (bColl == false)
	{
		// ���������ɖ߂�
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			m_apTutorialFont[nCnt]->SetScale(false);
		}
	}
}

//==============================================================================================
// �`���[�g���A���̏���
//==============================================================================================
void CEntry::Tutorial()
{
	// �`���[�g���A���J�n�������쓮���Ă���ꍇm_pChara
	if (m_bTutorialStart == true)
	{
		// �`���[�g���A���̊J�n����
		TutorialStart();
	}
	// �`���[�g���A���J�n�������쓮���Ă��Ȃ��ꍇ
	else
	{
		// �e���r��ʂ̕\���̊J�n�������쓮���Ă���ꍇ
		if (m_bTVDisplayStart == true)
		{
			// TV��ʂ̕\���̊J�n����
			TVDisplayStart();
		}
		else
		{
			// �e���r��ʂ̕\���̏I���������쓮���Ă���ꍇ
			if (m_bTVDisplayEnd == true)
			{
				// TV��ʂ̕\���̏I������
				TVDisplayEnd();
			}
			else
			{
				// �`���[�g���A���̑I������
				TutorialSelect();
			}
		}

	}
}

//==============================================================================================
// �`���[�g���A���̊J�n����
//==============================================================================================
void CEntry::TutorialStart()
{
	// �ړI�ړ��ʒu
	D3DXVECTOR3 anchorPosV = { 236.0f, 120.0f, 150.0f };
	D3DXVECTOR3 anchorPosR = { 672.0f, 120.0f, 650.0f };

	// �J�����̃|�C���^
	CCamera* pCamera = CApplication::GetCamera();

	// �ړ����������ꍇ
	if (pCamera->GetPosV() == anchorPosV)
	{
		// �`���[�g���A����ʂ��g�p����Ă��Ȃ��ꍇ
		if (m_pTutorial == nullptr)
		{
			m_pTutorial = CObject2D::Create({ 640.0f, 360.0f, 0.0f }, { TVSizeMax.x, 0.0f, 0.0f});
			m_pTutorial->SetTexture(CTexture::TEXTURE_NONE);
		}
		// ��\���ɂ���Ă��ꍇ
		else if (m_pTutorial->GetDrawFlag() == false)
		{
			m_pTutorial->SetDrawFlag(true);
		}
		// �`���[�g���A���摜�̏����T�C�Y
		m_pTutorial->SetSize(TVSizeMax.x, 180.0f);
		// �`���[�g���A���J�n�����̏I��
		m_bTutorialStart = false;
		m_bTVDisplayStart = true;
	}
	// �ړ��������Ă��Ȃ��ꍇ
	else
	{
		// ���̈ʒu�܂ŗ�����l���Œ肳����
		if (((anchorPosV.x - pCamera->GetPosV().x) < 0.1f))
		{
			pCamera->SetPosV(anchorPosV);
			pCamera->SetPosR(anchorPosR);
		}
		else
		{
			D3DXVECTOR3 moveV = (anchorPosV - pCamera->GetPosV()) * TVStartAnchorAttenuation;
			D3DXVECTOR3 moveR = (anchorPosR - pCamera->GetPosR()) * TVStartAnchorAttenuation;

			// ���_�A�����_�̐ݒ�
			pCamera->AddPosV(moveV);
			pCamera->AddPosR(moveR);
		}
	}
}

//==============================================================================================
// �`���[�g���A���̏I������
//==============================================================================================
void CEntry::TutorialEnd()
{
	// �`���[�g���A���̏I���������ǂݍ��܂�Ă���ꍇ
	if (m_bTutorialEnd == true)
	{
		// �ړI�ړ��ʒu
		D3DXVECTOR3 anchorPosV = { 75.0f, 145.0f, -500.0f };
		D3DXVECTOR3 anchorPosR = { 75.0f, 40.0f, 0.0f };

		// �J�����̃|�C���^
		CCamera* pCamera = CApplication::GetCamera();

		// �ړ��������Ă��Ȃ��ꍇ
		if (pCamera->GetPosV() != anchorPosV)
		{
			// ���̈ʒu�܂ŗ�����l���Œ肳����
			if (((anchorPosV.x - pCamera->GetPosV().x) > -0.1f))
			{
				pCamera->SetPosV(anchorPosV);
				pCamera->SetPosR(anchorPosR);
			}
			else
			{
				D3DXVECTOR3 moveV = (anchorPosV - pCamera->GetPosV()) * TVEndAnchorAttenuation;
				D3DXVECTOR3 moveR = (anchorPosR - pCamera->GetPosR()) * TVEndAnchorAttenuation;

				// ���_�A�����_�̐ݒ�
				pCamera->AddPosV(moveV);
				pCamera->AddPosR(moveR);
			}
		}
		// �ړ����������ꍇ
		else
		{
			m_bTutorialEnd = false;	// �I���������I������
		}
	}
}

//==============================================================================================
// �`���[�g���A���̑I������
//==============================================================================================
void CEntry::TutorialSelect()
{
	// �I���󋵂̊i�[��
	int nSellect = m_nSellect;

	// ���͏����̎擾
	CInput* pInput = CInput::GetKey();

	if ((pInput->Trigger(DIK_D)
		|| pInput->Trigger(JOYPAD_RIGHT, m_nTutorialInitiative))
		&& nSellect < MaxTutorialPage)
	{
		// �y�[�W��i�߂�
		nSellect++;
		m_apArrow[0]->SetDrawFlag(true);
		if (nSellect == MaxTutorialPage) m_apArrow[1]->SetDrawFlag(false);
		// �I��SE
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
	}
	else if ((pInput->Trigger(DIK_A)
		|| pInput->Trigger(JOYPAD_LEFT, m_nTutorialInitiative))
		&& nSellect > 0)
	{
		// �y�[�W��߂�
		nSellect--;
		m_apArrow[1]->SetDrawFlag(true);
		if (nSellect == 0) m_apArrow[0]->SetDrawFlag(false);
		// �I��SE
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
	}

	// �`���[�g���A�����I�����鏈��
	if (pInput->Trigger(JOYPAD_B, m_nTutorialInitiative) || pInput->Trigger(JOYPAD_A, m_nTutorialInitiative) || pInput->Trigger(DIK_SPACE))
	{
		// �e���r��ʂ̕\���̏I���������n�߂�
		m_bTVDisplayEnd = true;
		m_pTutorial->SetTexture(CTexture::TEXTURE_NONE);
		for (int nArrow = 0; nArrow < 2; nArrow++) m_apArrow[nArrow]->SetDrawFlag(false);
		nSellect = 0;
		m_nSellect = nSellect;
	}

	// �I���ɍX�V���������ꍇ
	if (m_nSellect != nSellect)
	{
		switch (m_nSellect = nSellect)
		{
		case 0:
			m_pTutorial->SetTexture(CTexture::TEXTURE_TUTORIAL_01);
			break;
		case 1:
			m_pTutorial->SetTexture(CTexture::TEXTURE_TUTORIAL_02);
			break;
		case 2:
			m_pTutorial->SetTexture(CTexture::TEXTURE_TUTORIAL_03);
			break;
		case 3:
			m_pTutorial->SetTexture(CTexture::TEXTURE_TUTORIAL_04);
			break;
		default:
			break;
		}
	}
}

//==============================================================================================
// TV��ʂ̕\���̊J�n����
//==============================================================================================
void CEntry::TVDisplayStart()
{
	// �e���r��ʂ̃T�C�Y
	D3DXVECTOR3 size = m_pTutorial->GetSize();

	// �ő�T�C�Y�ɒB���Ă���ꍇ
	if (size.y == TVSizeMax.y)
	{
		// ��󂪖��g�p�̏ꍇ
		if (m_apArrow[0] == nullptr)
		{
			m_apArrow[0] = CObject2D::Create({ 100.0f, 360.0f, 0.0f }, { 120.0f, 120.0f, 0.0f });
			m_apArrow[0]->SetTexture(CTexture::TEXTURE_TUTORIAL_ALLOW);
			m_apArrow[0]->SetTexPos(0.0f, 1.0f, 0.5f, 0.0f);
			m_apArrow[0]->SetDrawFlag(false);

			m_apArrow[1] = CObject2D::Create({ 1180.0f, 360.0f, 0.0f }, { 120.0f, 120.0f, 0.0f });
			m_apArrow[1]->SetTexture(CTexture::TEXTURE_TUTORIAL_ALLOW);
			m_apArrow[1]->SetTexPos(0.0f, 1.0f, 1.0f, 0.5f);
		}
		// �`���[�g���A���摜��\��t����
		m_pTutorial->SetTexture(CTexture::TEXTURE_TUTORIAL_01);

		// ������`�悷��
		m_apArrow[1]->SetDrawFlag(true);

		// �e���r_�\��SE
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_TV_DISPLAY);

		// �e���r��ʂ̕\���̊J�n�̏I������
		m_bTVDisplayStart = false;
	}
	else
	{
		// �T�C�Y�����Z
		size.y += size.y * TVDisplaySpeed;

		// �ő�T�C�Y�𒴂����ꍇ
		if (size.y > TVSizeMax.y)
		{
			// �T�C�Y�s�b�^���ɐݒ�
			size.y = TVSizeMax.y;
		}
		// �`���[�g���A���摜�̃T�C�Y��ݒ肷��
		m_pTutorial->SetSize(size.x, size.y);
	}
}

//==============================================================================================
// TV��ʂ̕\���̏I������
//==============================================================================================
void CEntry::TVDisplayEnd()
{
	// �e���r��ʂ̃T�C�Y
	D3DXVECTOR3 size = m_pTutorial->GetSize();

	// �Œ�T�C�Y�ɒB���Ă���ꍇ
	if (size.y == 0.0f)
	{
		// TV��ʂ̕\���̏I�����ԂɒB�����ꍇ
		if (m_nDisplayEndCounter > TVDisplayEndTimer)
		{
			m_nDisplayEndCounter = 0;
			// �`���[�g���A�����I������
			m_bTutorial = false;
			m_bTVDisplayEnd = false;
			m_bTutorialEnd = true;
			SetEntryUX(true);
			m_pTutorial->SetDrawFlag(false);
			// �e���r_�I��SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_TV_FINISH);
		}
		else
		{
			m_nDisplayEndCounter++;
		}
	}
	else
	{
		// �T�C�Y�����Z
		size.y -= size.y * TVDisplaySpeed;

		// �Œ�T�C�Y�𒴂����ꍇ
		if (size.y < 0.01f)
		{
			// �T�C�Y�s�b�^���ɐݒ�
			size.y = 0.0f;
		}
		// �`���[�g���A���摜�̃T�C�Y��ݒ肷��
		m_pTutorial->SetSize(size.x, size.y);
	}
}

//==============================================================================================
// �v���C���[�̓���
//==============================================================================================
void CEntry::PlayerIn(int index)
{
	if (m_pChara[index] == nullptr)
	{
		// �L�����N�^�[�̐���
		m_pChara[index] = CApplication::GetCharManager()->EntryChara(m_CharPos[index], CCharManager::TYPE_PLAYER, index, 0);
	}
	else if (!m_bIn[index])
	{
		D3DXVECTOR3 Vec[CCharManager::Max_Character] = {};
		float fCharSpeed = m_pChara[index]->GetMoveSpeed();

		// ���݂̈ʒu����ړI�̈ʒu�܂ł̃x�N�g��
		D3DXVECTOR3 DestPos_Vec = m_CharPosDest[index] - m_CharPos[index];
		Vec[index] = DestPos_Vec;

		// �x�N�g���̐��K��
		D3DXVec3Normalize(&DestPos_Vec, &DestPos_Vec);

		// �ړI�̈ʒu�܂Ŏ����ňړ�
		m_CharPos[index] += DestPos_Vec * fCharSpeed;

		// �ʒu�̐ݒ�
		m_pChara[index]->SetPos(m_CharPos[index]);

		// �ړI�̈ʒu�܂ł̊p�x
		float fAngle = (float)atan2(m_CharPos[index].x - m_CharPosDest[index].x, m_CharPos[index].z - m_CharPosDest[index].z);

		// �p�x�̐ݒ�
		m_pChara[index]->SetRotDest({ 0.0f,fAngle,0.0f });

		// ���[�V�����̐ݒ�
		m_pChara[index]->SetMotion(CCharacter::MOTION_WALK);

		// �ړI�̈ʒu�ɂ�����~�܂�
		if (Vec[index].x < fCharSpeed && Vec[index].x > -fCharSpeed
			&& Vec[index].z < fCharSpeed && Vec[index].z > -fCharSpeed)
		{
			m_bIn[index] = true;

			// ���[�V�����̐ݒ�
			m_pChara[index]->SetMotion(CCharacter::MOTION_NEUTRAL);
		}
	}
}

//==============================================================================================
// �s������
//==============================================================================================
void CEntry::Act_Limit()
{
	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		if (m_pChara[nCnt] != nullptr
			&& m_bIn[nCnt])
		{
			// �ʒu�̎擾
			D3DXVECTOR3 CharPos = m_pChara[nCnt]->GetPos();

			// X���̐���
			if (CharPos.x > 250)
				CharPos.x = 250;
			else if (CharPos.x < -100)
				CharPos.x = -100;

			// Z���̐���
			if (CharPos.z >	150)
				CharPos.z = 150;
			else if (CharPos.z < -300)
				CharPos.z = -300;

			// �ʒu�̐ݒ�
			m_pChara[nCnt]->SetPos(CharPos);
		}
	}
}

//==============================================================================================
// �G���g���[��ʂ̑S�Ă�UX�̐ݒ菈��
//==============================================================================================
void CEntry::SetEntryUX(const bool use)
{
	// �`���[�g���A�����J�n���鎞��UX����������
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_apTutorialFont[nCnt] != nullptr) for(auto apTutorialFont : m_apTutorialFont[nCnt]->GetFontAll()) apTutorialFont->SetDrawFlag(use);
	}

	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		if (m_apPlayerNumber[nCnt] != nullptr) m_apPlayerNumber[nCnt]->SetDrawFlag(use);
		if (m_apABotton[nCnt] != nullptr) m_apABotton[nCnt]->SetDrawFlag(use);
		if (m_apOKString[nCnt] != nullptr) for(auto apOKString : m_apOKString[nCnt]->GetFontAll()) apOKString->SetDrawFlag(use);
	}

	if (m_pGameStartFont != nullptr) for (auto pGameStartFont : m_pGameStartFont->GetFontAll()) pGameStartFont->SetDrawFlag(use);
	if (m_pABotton != nullptr) m_pABotton->SetDrawFlag(use);
}
