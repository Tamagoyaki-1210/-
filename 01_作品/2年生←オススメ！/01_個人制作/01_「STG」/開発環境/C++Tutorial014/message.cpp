//=============================================================================
//
// ���b�Z�[�W
// Author : tanimoto kosuke
//
//=============================================================================
#include "message.h"
#include <stdio.h>
#include "score.h"
#include "application.h"
#include "texture.h"
#include "game.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define MESSAGE_FILE		"data/.txt/message.txt"		//�ǂݍ��݃t�@�C��

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CMessage::CMessage() : CObject2D(UI_PRIORITY_UI)
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CMessage::~CMessage()
{

}

//=====================================
//����������
//=====================================
void CMessage::Init()
{
	CObject2D::Init();

	//�O���t�@�C���ŕϐ��ɒl����
	Load();

	//�I�u�W�F�N�g�ݒ菈��
	CObject2D::SetObject2D(TYPE_UI, m_fWidth, m_fHeight);
}

//=====================================
//�I������
//=====================================
void CMessage::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//�X�V����
//=====================================
void CMessage::Update()
{
	CObject2D::Update();

	//UV�ݒ菈��
	CObject2D::SetUV();
}

//=====================================
//�`�揈��
//=====================================
void CMessage::Draw()
{
	switch (m_type)
	{
	case CMessage::TYPE_THISNUMBER:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_THISNUMBER);
		break;
	case CMessage::TYPE_TITLE:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_TITLE);
		break;
	case CMessage::TYPE_PAUSE:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_PAUSE);
		break;
	default:
		break;
	}
	if (m_type != TYPE_PAUSE || CGame::GetPause() == true)
	{
		CObject2D::Draw();
	}
}

//=====================================
//�X�R�A��������
//=====================================
CMessage *CMessage::Create(D3DXVECTOR3 pos, Message_Type type)
{
	CMessage* pMessage = new CMessage;		//�X�R�A�|�C���^

	if (pMessage != nullptr)
	{
		pMessage->m_type = type;
		pMessage->Init();
		pMessage->SetPos(pos);
	}
	return pMessage;
}

//=====================================
//�ǂݍ��ݏ���
//=====================================
void CMessage::Load()
{
	char s_aString[256];		//�ǂݍ��ޗp������
	int nNumType = 0;			//�ǂݍ��񂾃^�C�v��

								//�t�@�C�����J��
	FILE* pFile = fopen(MESSAGE_FILE, "r");

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