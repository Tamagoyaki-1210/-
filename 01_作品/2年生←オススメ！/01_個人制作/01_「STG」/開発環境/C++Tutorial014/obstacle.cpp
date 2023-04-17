//=============================================================================
//
// ��Q��
// Author : tanimoto kosuke
//
//=============================================================================
#include "obstacle.h"
#include <assert.h>
#include <stdio.h>
#include "matrix.h"
#include "explosion.h"
#include "application.h"
#include "texture.h"
#include "score.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define OBSTACLE_BLINK_SPEED		(0.5f)
#define OBSTACLE_MAX_BLINK			(4)
#define OBSTACLE_MOVE_SPEED			(2.0f)
#define OBSTACLE_FILE				"data/.txt/obstacle.txt"		//�ǂݍ��݃t�@�C��

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CObstacle::CObstacle() : CObject2D(OBJECT_PRIORITY_BACK)
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CObstacle::~CObstacle()
{

}

//=====================================
//����������
//=====================================
void CObstacle::Init()
{
	CObject2D::Init();
}

//=====================================
//�I������
//=====================================
void CObstacle::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//�X�V����
//=====================================
void CObstacle::Update()
{
	CObject2D::Update();

	//UV�ݒ�
	CObject2D::SetUV();

	CObject2D::SetAnim(m_animX, m_animY, 6);

	//�ʒu�擾
	D3DXVECTOR3 pos = CObject2D::GetPos();

	m_posOld = pos;

	SituationType();

	//��ʖ���AI
	TypeAI();

	//�ʒu�X�V
	pos += m_move;

	//�ʒu�ݒ菈��
	SetPos(pos);

	OffScreen(pos);
}

//=====================================
//�`�揈��
//=====================================
void CObstacle::Draw()
{
	//�摜�擾����
	switch (m_type)
	{
	case TYPE_01:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_OBSTACLE_01);
		break;
	case TYPE_02:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_BULLET_01);
		break;
	default:
		break;
	}
	CObject2D::Draw();
}

//=====================================
//��ʖ���AI����
//=====================================
void CObstacle::TypeAI()
{
	switch (m_type)
	{
	case TYPE_01:
		m_move.x = -OBSTACLE_MOVE_SPEED;
		break;
	case TYPE_02:
		m_move.x = -OBSTACLE_MOVE_SPEED;
		break;
	default:
		break;
	}
}

//=====================================
//��e����
//=====================================
void CObstacle::Hit(int nDamage)
{
	m_nLife -= nDamage;
	if (m_nLife <= 0)
	{
		m_nLife = 0;
		Destroy(CObject2D::GetPos());
	}
}
//=====================================
//�ݒ菈��
//=====================================
void CObstacle::SetType()
{
	Load();
	switch (m_type)
	{
	case TYPE_01:
		m_flip = FLIP_SQUARE;
		break;
	case TYPE_02:
		m_flip = FLIP_CIRCLE;
		break;
	default:
		break;
	}
	//�I�u�W�F�N�g�ݒ菈��
	SetObject2D(CObject::TYPE_OBSTACLE, m_fWidth, m_fHeight);
}

//=====================================
//��ʊO����
//=====================================
void CObstacle::OffScreen(D3DXVECTOR3 pos)
{
	if (pos.x + (SCREEN_WIDTH / 2) <= 0.0f || pos.y + (SCREEN_HEIGHT / 2) <= 0.0f || pos.y - (SCREEN_HEIGHT / 2) >= SCREEN_HEIGHT)
	{
		Uninit();
	}
}

//=====================================
//��������
//=====================================
void CObstacle::Destroy(D3DXVECTOR3 pos)
{
	//�j�󂵂��ꍇ10�_���Z
	CScore *pScore = CScore::GetScore();
	pScore->AddScore(50);
	//������������
	CExplosion::Create(pos, m_fWidth, m_fHeight);
	Uninit();
}

//=====================================
//�l�p�`��Q���̒e������
//=====================================
D3DXVECTOR3 CObstacle::FlipCircle(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fLength)
{
	float fSumLengthRad = (GetLength() + fLength) / 2;	//�~�̔��a���m�̘a

	D3DXVECTOR3 posObst = GetPos();
	float fSubX = posObst.x - pos.x;									//X���W�̍���
	float fSubY = posObst.y - pos.y;									//Y���W�̍���
	float fSubLength = sqrtf((fSubX * fSubX) + (fSubY * fSubY));	//���݂�2�_�̋���

	float fSubXOld = m_posOld.x - posOld.x;											//�O��X���W�̍���
	float fSubYOld = m_posOld.y - posOld.y;											//�O��Y���W�̍���

	D3DXVECTOR3 flipPos = posOld;

	//(���݈ʒu���d�Ȃ�A�O��ʒu���d�Ȃ��Ă��Ȃ��ꍇ)
	//�~���a�����̘a������2�_�̋������傫���A�~���a�����̘a���O��2�_�̋�����菬�����ꍇ
	if (fSumLengthRad >= fSubLength)
	{
		if (fSubXOld != 0.0f)
		{
			//�����̐��K��
			fSubXOld = Normalize(fSubXOld) * (OBSTACLE_MOVE_SPEED + 1);
		}
		if (fSubYOld != 0.0f)
		{
			fSubYOld = Normalize(fSubYOld) * (OBSTACLE_MOVE_SPEED + 1);
		}
		//�O��ʒu�ƑO����W�̍����̒����Ō��Z����
		flipPos -= D3DXVECTOR3(fSubXOld, fSubYOld, 0.0f);
	}
	return flipPos;
}

//=====================================
//�l�p�`��Q���̒e������
//=====================================
D3DXVECTOR3 CObstacle::FlipSquare(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fWidth, float fHeight)
{
	float fLeft = pos.x - (fWidth / 2);				//P�l�p�`����
	float fRight = pos.x + (fWidth / 2);			//P�l�p�`�E��
	float fUpp = pos.y - (fHeight / 2);				//P�l�p�`�㑤
	float fLow = pos.y + (fHeight / 2);				//P�l�p�`����

	float fLeftOld = posOld.x - (fWidth / 2);		//P�O��l�p�`����
	float fRightOld = posOld.x + (fWidth / 2);		//P�O��l�p�`�E��
	float fUppOld = posOld.y - (fHeight / 2);		//P�O��l�p�`�㑤
	float fLowOld = posOld.y + (fHeight / 2);		//P�O��l�p�`����

	D3DXVECTOR3 posObst = GetPos();
	float fWidthObst = m_fWidth / 2;
	float fHeightObst = m_fHeight / 2;

	float fLeftObst = posObst.x - fWidthObst;		//O�l�p�`����
	float fRightObst = posObst.x + fWidthObst;		//O�l�p�`�E��
	float fUppObst = posObst.y - fHeightObst;		//O�l�p�`�㑤
	float fLowObst = posObst.y + fHeightObst;		//O�l�p�`����

	float fLeftObstOld = m_posOld.x - fWidthObst;	//O�O��l�p�`����
	float fRightObstOld = m_posOld.x + fWidthObst;	//O�O��l�p�`�E��
	float fUppObstOld = m_posOld.y - fHeightObst;	//O�O��l�p�`�㑤
	float fLowObstOld = m_posOld.y + fHeightObst;	//O�O��l�p�`����

	D3DXVECTOR3 flipPos;
	//P���������蔻��@P������O�E����荶�ɍ݂�ꍇ
	if (fLeft < fRightObst && fLeftOld >= fRightObstOld)
	{
		flipPos = D3DXVECTOR3(fRightObst + (fWidth / 2), pos.y, 0.0f);
	}
	//P�E�������蔻�� P�E����O�������E�ɍ݂�ꍇ
	if (fRight > fLeftObst && fRightOld <= fLeftObstOld)
	{
		flipPos = D3DXVECTOR3(fLeftObst - (fWidth / 2), pos.y, 0.0f);
	}
	//P�㑤�����蔻�� P�㑤��O��������ɍ݂�ꍇ
	if (fUpp < fLowObst && fUppOld >= fLowObstOld)
	{
		flipPos = D3DXVECTOR3(pos.x, fLowObst + (fHeight / 2), 0.0f);
	}
	//P���������蔻�� P������O�㑤��艺�ɍ݂�ꍇ
	if (fLow > fUppObst && fLowOld <= fUppObstOld)
	{
		flipPos = D3DXVECTOR3(pos.x, fUppObst - (fHeight / 2), 0.0f);
	}

	return flipPos;
}


//=====================================
//�_�ŐF����
//=====================================
void CObstacle::BlinkColor()
{
	if (m_bBlink)
	{
		m_fBlinkCounter--;
	}
	else
	{
		m_fBlinkCounter++;
	}

	if (m_fBlinkCounter >= 1.0f / OBSTACLE_BLINK_SPEED)
	{
		m_bBlink = true;
		m_fBlinkCounter = 1.0f / OBSTACLE_BLINK_SPEED;
		m_nBlinkLoop++;
	}
	else if (m_fBlinkCounter <= 0.0f)
	{
		m_bBlink = false;
		m_fBlinkCounter = 0.0f;
	}

	//�_�ł����镔���̐F���v�Z
	D3DXCOLOR colBlink = D3DXCOLOR(1.0f, 0.6f, 0.6f, m_fBlinkCounter * OBSTACLE_BLINK_SPEED);

	SetColor(colBlink);

	if (m_nBlinkLoop >= OBSTACLE_MAX_BLINK)
	{
		m_nBlinkLoop = 0;
		m_situ = SITU_NONE;
		//�F������
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//=====================================
//��Q����ԏ���
//=====================================
void CObstacle::SituationType()
{
	switch (m_situ)
	{
	case CObstacle::SITU_NONE:
		break;
	case CObstacle::SITU_DAMAGE:
		BlinkColor();
		break;
	default:
		break;
	}
}

//=====================================
//��������
//=====================================
CObstacle *CObstacle::Create(D3DXVECTOR3 pos, Obstacle_Type type)
{
	CObstacle* pObstacle = new CObstacle;	//�G�L�����|�C���^

	if (pObstacle != nullptr)
	{
		pObstacle->Init();
		pObstacle->SetPos(pos);
		pObstacle->m_type = type;
		pObstacle->SetType();
	}

	return pObstacle;
}

//=====================================
//�ǂݍ��ݏ���
//=====================================
void CObstacle::Load()
{
	char s_aString[256];		//�ǂݍ��ޗp������
	int nNumType = 0;			//�ǂݍ��񂾃^�C�v��

								//�t�@�C�����J��
	FILE* pFile = fopen(OBSTACLE_FILE, "r");

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

				assert(TYPE_MAX != nNumType);	//�z���葽���t�@�C���̓ǂݍ���

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
					else if (strcmp(s_aString, "ANIM_X") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_animX);
					}
					else if (strcmp(s_aString, "ANIM_Y") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_animY);
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