//=============================================================================
//
// �I����
// Author : tanimoto kosuke
//
//=============================================================================
#include "choice.h"
#include <assert.h>
#include <stdio.h>
#include "application.h"
#include "texture.h"
#include "fade.h"
#include "input.h"
#include "menu.h"
#include "mode.h"
#include "game.h"

int CChoice::m_nNumAll = 0;
int CChoice::m_nSelect = 0;
int CChoice::m_nSkipSelect = 0;
//---------------------------
//�}�N���֐�
//---------------------------
#define CHOICE_FILE		"data/.txt/choice.txt"		//�ǂݍ��݃t�@�C��

//-----------------------------------------
//�v���g�^�C�v�錾
//-----------------------------------------
void ExitExe(void);

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CChoice::CChoice() : CObject2D(UI_PRIORITY_UI), m_nId(0)
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CChoice::~CChoice()
{

}

//=====================================
//����������
//=====================================
void CChoice::Init()
{
	//�O���t�@�C���ŕϐ��ɒl��������
	Load();

	CObject2D::Init();

	//�I�u�W�F�N�g�ݒ菈��
	CObject2D::SetObject2D(TYPE_UI, m_fWidth, m_fHeight);
}

//=====================================
//�I������
//=====================================
void CChoice::Uninit()
{
	CObject2D::Uninit();
	ResetId();
}

//=====================================
//�X�V����
//=====================================
void CChoice::Update()
{
	CObject2D::Update();

	//UV�ݒ�
	CObject2D::SetUV();

	//�ʒu�擾
	D3DXVECTOR3 pos = CObject2D::GetPos();

	//�ʒu�ݒ菈��
	SetPos(pos);

	if (CApplication::GetMode()->GetMode() == CMode::MODE_GAME)
	{
		if (CGame::GetPause() == true)
		{
			InputChoice();
		}
	}
	else
	{
		InputChoice();
	}
}

//=====================================
//�`�揈��
//=====================================
void CChoice::Draw()
{
	//�摜�擾����
	switch (m_type)
	{
	case TITLE_GAMESTART:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_CHOICE_01);
		break;
	case TITLE_EXIT:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_CHOICE_03);
		break;
	case GAME_CONTINUE:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_CHOICE_04);
		break;
	case GAME_EXIT:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_CHOICE_05);
		break;
	default:
		break;
	}
	if (CGame::GetPause() == true)
	{
		CObject2D::Draw();
	}
}

//=====================================
//�I�����̓��͏���
//=====================================
void CChoice::InputChoice(void)
{
	CFade *pFade = CMode::GetFade();

	if (pFade->GetFade() != CFade::FADE_NONE)
	{// �t�F�[�h���Ă���ꍇ
		return;
	}

	CInput *pInput = CApplication::GetInput();

	//���݂�ID
	if (m_nId == m_nSelect)
	{
		//���ݑI�����ꂽID�̂ݐF�ύX
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		if (pInput->GetKeyboardTrigger(DIK_W))
		{// W�L�[�������ꂽ���ǂ���
			if (m_nSkipSelect <= 0)
			{
				//�F������
				SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				m_nSelect--;	//��񂾂��ɂ�����
				m_nSelect = (m_nSelect + m_nNumAll) % m_nNumAll;

				if (m_nSelect == m_nNumAll - 1)
				{
					m_nSkipSelect = 1;
				}
			}
			else
			{
				m_nSkipSelect--;
			}
			CSound::PlaySound(SOUND_LABEL_SE_SELECT);
		}
		else if (pInput->GetKeyboardTrigger(DIK_S))
		{// S�L�[�������ꂽ���ǂ���
		 //���݂�ID
			if (m_nSkipSelect <= 0)
			{
				//�F������
				SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				m_nSelect++;
				m_nSelect = (m_nSelect + m_nNumAll) % m_nNumAll;

				if (m_nSelect != 0)
				{
					m_nSkipSelect = 1;
				}
			}
			else
			{
				m_nSkipSelect--;
			}
			CSound::PlaySound(SOUND_LABEL_SE_SELECT);
		}
		if (pInput->GetKeyboardTrigger(DIK_RETURN) || pInput->GetKeyboardTrigger(DIK_SPACE))
		{//����L�[(ENTER�L�[)�������ꂽ���ǂ���
			CMode *pMode = CApplication::GetMode();

			switch (m_type)
			{
			case TITLE_GAMESTART:	//�^�C�g��_�Q�[���J�n
				pMode->ChangeMode(CMode::MODE_GAME);
				CSound::PlaySound(SOUND_LABEL_SE_YES);
				break;
			case TITLE_EXIT:			//�^�C�g��_�I��
				CSound::PlaySound(SOUND_LABEL_SE_YES);
				ExitExe();
				break;
			case GAME_CONTINUE:		//�Q�[��_�ĊJ
				CGame::SetPause(false);
				CSound::PlaySound(SOUND_LABEL_SE_WHISTLE_START);
				break;
			case GAME_EXIT:			//�Q�[��_�I��
				pMode->ChangeMode(CMode::MODE_TITLE);
				CSound::PlaySound(SOUND_LABEL_SE_WHISTLE_FINISH);
				break;
			default:
				assert(false);
				break;
			}
		}
	}
}

//=====================================
//��������
//=====================================
void CChoice::ResetId()
{
	m_nNumAll = 0;
	m_nSelect = 0;
}

//=====================================
//��������
//=====================================
CChoice *CChoice::Create(D3DXVECTOR3 pos, Choice_Type type)
{
	CChoice* pChoice = new CChoice;	//�G�L�����|�C���^

	if (pChoice != nullptr)
	{
		pChoice->Init();
		pChoice->SetPos(pos);
		pChoice->m_type = type;
		pChoice->m_nId = m_nNumAll;
		if (pChoice->m_nNumAll != 0)
		{
			pChoice->SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}
		pChoice->m_nNumAll++;
	}
	return pChoice;
}

//=====================================
//�ǂݍ��ݏ���
//=====================================
void CChoice::Load()
{
	char s_aString[256];		//�ǂݍ��ޗp������
	int nNumType = 0;			//�ǂݍ��񂾃^�C�v��

								//�t�@�C�����J��
	FILE* pFile = fopen(CHOICE_FILE, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		fscanf(pFile, "%s", s_aString);

		while (strncmp(s_aString, "SCRIPT", 6) != 0)
		{//�X�^�[�g������܂ŋ󔒂�ǂݍ���
			s_aString[0] = {};
			fscanf(pFile, "%s", s_aString);
		}

		while (strncmp(s_aString, "END_SCRIPT", 10) != 0)
		{//������̏������Ɠǂݍ���
			fscanf(pFile, "%s", s_aString);

			if (strncmp(s_aString, "#", 1) == 0)
			{//���̌�ɃR�����g
				fgets(s_aString, sizeof(s_aString), pFile);
				continue;
			}

			if (strcmp(s_aString, "TYPESTATE") == 0)
			{//�����񂪈�v�����ꍇ
				while (strncmp(s_aString, "ENDSTATE", 8) != 0)
				{
					fscanf(pFile, "%s", s_aString);		//=�̓ǂݍ���
					
					if (strcmp(s_aString, "WIDTH") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%f", &m_fWidth);
					}
					else if (strcmp(s_aString, "HEIGHT") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%f", &m_fHeight);
					}
				}
				if (m_type == nNumType)
				{//�w�肵����ʂ̏ꍇ
					break;
				}
				nNumType++;
			}
		}
	}
	//�t�@�C�������
	fclose(pFile);
}