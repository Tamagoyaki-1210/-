//=============================================================================
//
// �i���o�[
// Author : tanimoto kosuke
//
//=============================================================================
#include "number.h"
#include <stdio.h>
#include "score.h"
#include "application.h"
#include "texture.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define NUMBER_FILE		"data/.txt/number.txt"		//�ǂݍ��݃t�@�C��

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CNumber::CNumber() : CObject2D(UI_PRIORITY_UI)
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CNumber::~CNumber()
{

}

//=====================================
//����������
//=====================================
void CNumber::Init()
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
void CNumber::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//�X�V����
//=====================================
void CNumber::Update()
{
	CObject2D::Update();

	//UV�ݒ菈��
	CObject2D::SetUV();
}

//=====================================
//�`�揈��
//=====================================
void CNumber::Draw()
{
	CApplication::GetTexture()->TextureType(CTexture::TYPE_NUMBER);

	CObject2D::Draw();
}

//=====================================
//�_���̐ݒ菈��
//=====================================
void CNumber::SetNumber(int nScore)
{
	SetTex(nScore, BASE_NUMBER);
}

//=====================================
//�X�R�A��������
//=====================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, Number_Type type)
{
	CNumber* pNumber = new CNumber;		//�X�R�A�|�C���^

	if (pNumber != nullptr)
	{
		pNumber->m_type = type;
		pNumber->Init();
		pNumber->SetPos(pos);
	}
	return pNumber;
}

//=====================================
//�ǂݍ��ݏ���
//=====================================
void CNumber::Load()
{
	char s_aString[256];		//�ǂݍ��ޗp������
	int nNumType = 0;			//�ǂݍ��񂾃^�C�v��

								//�t�@�C�����J��
	FILE* pFile = fopen(NUMBER_FILE, "r");

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