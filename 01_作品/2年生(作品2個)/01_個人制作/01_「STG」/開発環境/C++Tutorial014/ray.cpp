//=============================================================================
//
// �����G�t�F�N�g
// Author : tanimoto kosuke
//
//=============================================================================
#include "ray.h"
#include <stdio.h>
#include "application.h"
#include "texture.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define RAY_FILE			"data/.txt/ray.txt"	//�ǂݍ��݃t�@�C��
#define SHRINK_EFFECT		(0.2f)				// �G�t�F�N�g�k�����x
#define RAND_EFFECT			(2.0f)				// �G�t�F�N�g�̐U�ꕝ
#define BLINK_EFFECT_SPEED	(0.02f)				//�G�t�F�N�g�_�ő��x
#define BLINK_EFFECT_MAX	(0.6f)				//�G�t�F�N�g�_�ōő�l
#define BLINK_EFFECT_MIN	(0.2f)				//�G�t�F�N�g�_�ōŏ��l

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CRay::CRay() : CObject2D(EFFECT_PRIORITY_BACK), m_fAlphaCounter(BLINK_EFFECT_MAX), m_bBlink(false)
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CRay::~CRay()
{

}

//=====================================
//����������
//=====================================
void CRay::Init()
{
	CObject2D::Init();
}

//=====================================
//�I������
//=====================================
void CRay::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//�X�V����
//=====================================
void CRay::Update()
{
	CObject2D::Update();

	//UV�ݒ�
	CObject2D::SetUV();

	//�ʒu�擾����
	D3DXVECTOR3 pos = CObject2D::GetPos();

	//��ʍX�V����
	UpdateType();

	//�ʒu�X�V
	pos += m_move * m_fSpeed;

	//�ʒu�ݒ菈��
	SetPos(pos);

	//��������
	Decline();
}

//=====================================
//�`�揈��
//=====================================
void CRay::Draw()
{
	//�摜�擾����
	CApplication::GetTexture()->TextureType(CTexture::TYPE_RAY_01);
	CObject2D::Draw();
}

//=====================================
//��ʏ���������
//=====================================
void CRay::InitType()
{
	switch (m_type)
	{
	case CRay::NONE:
		break;
	case CRay::BULLET:
		Rand();
		CObject2D::SetColor(D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.8f, 0.8f, 1.0f));
		break;
	default:
		break;
	}
}

//=====================================
//��ʍX�V����
//=====================================
void CRay::UpdateType()
{
	//�����擾����
	float fLength = CObject2D::GetLength();

	switch (m_type)
	{
	case CRay::NONE:
		break;
	case CRay::BULLET:
		if (m_bBlink == false)
		{
			if (m_fAlphaCounter < BLINK_EFFECT_MAX)
			{
				m_fAlphaCounter += BLINK_EFFECT_SPEED;
			}
			else
			{
				m_fAlphaCounter = BLINK_EFFECT_MAX;
				m_bBlink = true;
			}
		}
		else
		{
			if (m_fAlphaCounter > BLINK_EFFECT_MIN)
			{
				m_fAlphaCounter -= BLINK_EFFECT_SPEED;
			}
			else
			{
				m_fAlphaCounter = BLINK_EFFECT_MIN;
				m_bBlink = false;
			}
		}
		CObject2D::SetColor(D3DXCOLOR(0.5f, 0.0f, 0.0f, m_fAlphaCounter), D3DXCOLOR(1.0f, 0.2f, 0.0f, m_fAlphaCounter), D3DXCOLOR(1.0f, 1.0f, 0.0f, m_fAlphaCounter), D3DXCOLOR(1.0f, 0.8f, 0.8f, m_fAlphaCounter));

		fLength -= fLength * SHRINK_EFFECT;
		SetLength(fLength);
		break;
	default:
		break;
	}
}

//=====================================
//��������
//=====================================
void CRay::Destroy()
{
	CObject2D::Uninit();
}

//=====================================
//�����_���ݒ菈��
//=====================================
void CRay::Rand()
{
	D3DXVECTOR3 pos = GetPos();

	float randX = (rand() / (float)RAND_MAX) * (RAND_EFFECT * 2) - RAND_EFFECT;
	float randY = (rand() / (float)RAND_MAX) * (RAND_EFFECT * 2) - RAND_EFFECT;

	pos.x += randX;
	pos.y += randY;
	m_move.x *= randX;
	m_move.y *= -randY;

	//�ʒu�ݒ菈��
	SetPos(pos);
}

//=====================================
//��������
//=====================================
void CRay::Decline()
{
	//�����v�Z
	if (m_nLife <= 0)
	{
		Destroy();
	}
	else
	{//���������炷
		m_nLife--;
	}
}

//=====================================
//�v���C���[�e��������
//=====================================
CRay *CRay::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, Ray_Type type)
{
	CRay* pRay = new CRay;	//�e�̃|�C���^

	if (pRay != nullptr)
	{
		pRay->Init();
		//�I�u�W�F�N�g�ݒ菈��
		pRay->m_move = move;
		pRay->SetPos(pos - move * 10.0f);
		pRay->m_type = type;
		pRay->LoadRay();
		pRay->InitType();
		pRay->SetObject2D(TYPE_EFFECT, pRay->m_fWidth, pRay->m_fHeight);
	}
	return pRay;
}

//=====================================
//�ǂݍ��ݏ���(�v���C���[�e)
//=====================================
void CRay::LoadRay()
{
	char s_aString[256];		//�ǂݍ��ޗp������
	int nNumType = 0;			//�ǂݍ��񂾃^�C�v��

								//�t�@�C�����J��
	FILE* pFile = fopen(RAY_FILE, "r");

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

					if (strcmp(s_aString, "LIFE") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_nLife);
					}
					else if (strcmp(s_aString, "WIDTH") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%f", &m_fWidth);
					}
					else if (strcmp(s_aString, "HEIGHT") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%f", &m_fHeight);
					}
					else if (strcmp(s_aString, "SPEED") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%f", &m_fSpeed);
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