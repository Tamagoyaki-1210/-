//=============================================================================
//
// �v���C���[
// Author : tanimoto kosuke
//
//=============================================================================
#include "player.h"
#include <assert.h>
#include <stdio.h>
#include "matrix.h"
#include "input.h"
#include "bullet.h"
#include "application.h"
#include "texture.h"
#include "explosion.h"
#include "life.h"
#include "gauge.h"
#include "game.h"
#include "obstacle.h"
#include "score.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define PLAYER_FINAL_LIFE		(200)	//�ő�HP
#define PLAYER_MAX_MOVE_SPEED	(3.0f)
#define PLAYER_BLINK_SPEED		(0.2f)
#define PLAYER_MAX_BLINK		(4)
#define PLAYER_MAX_BULLET		(5)		//�e�������L��
#define PLAYER_FILE				"data/.txt/player.txt"		//�ǂݍ��݃t�@�C��

CPlayer *CPlayer::m_pPlayer = nullptr;
//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CPlayer::CPlayer() : CObject2D(OBJECT_PRIORITY_CENTER)
{	

}

//=====================================
//�f�X�g���N�^
//=====================================
CPlayer::~CPlayer()
{

}

//=====================================
//����������
//=====================================
void CPlayer::Init()
{
	CObject2D::Init();

	//�O���t�@�C���ŕϐ��ɒl��������
	Load();

	m_nMaxLife = m_nLife;
	m_nFastMaxLife = m_nMaxLife;
	m_nBlinkLoop = 0;
	m_nBulletCounter = 0;
	m_nHoldBullet = 1;
	m_fBlinkCounter = 0.0f;
	m_bBlink = false;
	m_nDeathTimer = 0;

	//�I�u�W�F�N�g�ݒ菈��
	SetObject2D(CObject::TYPE_PLAYER, m_fWidth, m_fHeight);
}

//=====================================
//�I������
//=====================================
void CPlayer::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//�X�V����
//=====================================
void CPlayer::Update()
{
	if (m_situ != SITU_DEATH)
	{
		CObject2D::Update();

		//UV�ݒ�
		CObject2D::SetUV();

		CObject2D::SetAnim(m_nAnimX, m_nAnimY, 6);

		//�ʒu�擾
		D3DXVECTOR3 pos = CObject2D::GetPos();

		//�O��ʒu�̎擾
		m_posOld = pos;

		//�p�x�擾����
		D3DXVECTOR3 rot = CObject2D::GetRot();

		//���͏���
		Input(pos);

		//����
		m_move -= m_move * 0.15f;

		//�ʒu�X�V
		pos += m_move * m_fSpeed;

		//�ʒu�ݒ菈��
		SetPos(pos);

		//�p�x�擾����
		CObject2D::SetRot(rot);

		OffScreen(pos);

		if (m_nBulletCounter > 0)
		{
			m_nBulletCounter--;
			if (m_nBulletCounter < 0)
			{
				m_nBulletCounter = 0;
			}
		}

		//�����蔻��ݒ菈��
		Collision();
	}
	SituationType();
}

//=====================================
//�`�揈��
//=====================================
void CPlayer::Draw()
{
	if (m_situ != SITU_DEATH)
	{
		CApplication::GetTexture()->TextureType(CTexture::TYPE_PLAYER);
		CObject2D::Draw();
	}
}

//=====================================
//��e����
//=====================================
void CPlayer::Hit(int nDamage)
{
	if (nDamage > 0)
	{//��e����������
		CExplosion::Create(CObject2D::GetPos(), EXPLOSION_WIDTH, EXPLOSION_HEIGHT);
		m_nLife -= nDamage;
		m_pLife->SetLife(m_nLife, m_nMaxLife, m_nFastMaxLife);

		m_fBlinkCounter = 0.0f;
		m_situ = SITU_DAMAGE;
		if (m_nLife <= 0)
		{
			Destroy(CObject2D::GetPos());
		}
	}
}

//=====================================
//�̗͉��Z����
//=====================================
void CPlayer::AddLife(int addLife)
{
	int nAddLife = m_nLife += addLife;

	//���Z���ꂽHP���ő�𒴂����ꍇ
	if (nAddLife >= m_nMaxLife)
	{
		m_nLife = m_nMaxLife;

		CScore *pScore = CScore::GetScore();
		pScore->AddScore(10);
	}
	else
	{
		m_nLife = nAddLife;
		CSound::PlaySound(SOUND_LABEL_SE_ENERGY_1);
	}

	m_pLife->SetLife(m_nLife, m_nMaxLife, m_nFastMaxLife);
}

//=====================================
//�̗͉��Z����
//=====================================
void CPlayer::AddMaxLife(int addMaxLife)
{
	int nAddLife = m_nMaxLife += addMaxLife;

	//���Z���ꂽHP���ő�𒴂����ꍇ
	if (nAddLife >= PLAYER_FINAL_LIFE)
	{
		m_nMaxLife = PLAYER_FINAL_LIFE;
		m_nLife += 10;
		if (m_nLife > m_nMaxLife)
		{
			m_nLife = m_nMaxLife;
		}
		CScore *pScore = CScore::GetScore();
		pScore->AddScore(50);
	}
	else
	{
		CSound::PlaySound(SOUND_LABEL_SE_ENERGY_4);
		m_nMaxLife = nAddLife;
		m_nLife += addMaxLife;
	}
	m_pLife->SetLife(m_nLife, m_nMaxLife, m_nFastMaxLife);
}

//=====================================
//���x���Z����
//=====================================
void CPlayer::AddSpeed(float nSpeed)
{
	//�X�s�[�h���ő�𒴂��Ă��Ȃ��ꍇ
	if (m_fSpeed < PLAYER_MAX_MOVE_SPEED)
	{
		m_fSpeed += nSpeed;
		CSound::PlaySound(SOUND_LABEL_SE_ENERGY_2);
	}
	else
	{
		m_fSpeed = PLAYER_MAX_MOVE_SPEED;
		CScore *pScore = CScore::GetScore();
		pScore->AddScore(20);
	}
}

//=====================================
//�e���L�����Z����
//=====================================
void CPlayer::AddBullet()
{
	//�e���L�����ő�𒴂��Ă��Ȃ��ꍇ
	if (m_nHoldBullet < PLAYER_MAX_BULLET)
	{
		m_nHoldBullet++;
		CSound::PlaySound(SOUND_LABEL_SE_ENERGY_3);
	}
	else
	{
		CScore *pScore = CScore::GetScore();
		pScore->AddScore(30);
	}
}

//=====================================
//���͏���
//=====================================
void CPlayer::Input(D3DXVECTOR3 pos)
{
	//�L�[�{�[�h�擾
	CInput *pCInput = CApplication::GetInput();

	//�v���C���[�ړ�
	if (pCInput->GetKeyboardPress(DIK_D))
	{//�E�ړ�
		if (pCInput->GetKeyboardPress(DIK_S))
		{//���ړ�
			m_move += NormalizeLength(1.0f, 1.0f);
		}
		else if (pCInput->GetKeyboardPress(DIK_W))
		{//��ړ�
			m_move += NormalizeLength(1.0f, -1.0f);
		}
		else
		{
			m_move.x += 1.0f;
		}
	}
	else if (pCInput->GetKeyboardPress(DIK_A))
	{//���ړ�
		if (pCInput->GetKeyboardPress(DIK_S))
		{//���ړ�
			m_move += NormalizeLength(-1.0f, 1.0f);
		}
		else if (pCInput->GetKeyboardPress(DIK_W))
		{//��ړ�
			m_move += NormalizeLength(-1.0f, -1.0f);
		}
		else
		{
			m_move.x += -1.0f;
		}
	}
	else if (pCInput->GetKeyboardPress(DIK_S))
	{//���ړ�
		m_move.y += 1.0f;
	}
	else if (pCInput->GetKeyboardPress(DIK_W))
	{//��ړ�
		m_move.y += -1.0f;
	}

	if (m_nBulletCounter <= 0)
	{
		if (pCInput->GetKeyboardPress(DIK_B))
		{
			//���[�U�[��������
			CBullet::Create(D3DXVECTOR3(pos.x + (m_fWidth / 2), pos.y, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), CBullet::PLAYER_LASER);
			m_nBulletCounter = 25 - (m_nHoldBullet * 3);
		}
		else if (pCInput->GetKeyboardPress(DIK_SPACE))
		{
			if (m_nHoldBullet >= 5)
			{
				//�e��������
				CBullet::Create(D3DXVECTOR3(pos.x + (m_fWidth / 2), pos.y, 0.0f), D3DXVECTOR3(sinf((float)D3DX_PI / 8), cosf((float)D3DX_PI / 8), 0.0f), CBullet::PLAYER_NORMAL);
			}
			if (m_nHoldBullet >= 4)
			{
				//�e��������
				CBullet::Create(D3DXVECTOR3(pos.x + (m_fWidth / 2), pos.y, 0.0f), D3DXVECTOR3(sinf((float)D3DX_PI / 8 * 7), cosf((float)D3DX_PI / 8 * 7), 0.0f), CBullet::PLAYER_NORMAL);
			}
			if (m_nHoldBullet >= 3)
			{
				//�e��������
				CBullet::Create(D3DXVECTOR3(pos.x + (m_fWidth / 2), pos.y, 0.0f), D3DXVECTOR3(sinf((float)D3DX_PI / 8 * 3), cosf((float)D3DX_PI / 8 * 3), 0.0f), CBullet::PLAYER_NORMAL);
			}
			if (m_nHoldBullet >= 2)
			{
				//�e��������
				CBullet::Create(D3DXVECTOR3(pos.x + (m_fWidth / 2), pos.y, 0.0f), D3DXVECTOR3(sinf((float)D3DX_PI / 8 * 5), cosf((float)D3DX_PI / 8 * 5), 0.0f), CBullet::PLAYER_NORMAL);
			}
			//�e��������
			CBullet::Create(D3DXVECTOR3(pos.x + (m_fWidth / 2), pos.y, 0.0f), D3DXVECTOR3(sinf((float)D3DX_PI / 2), cosf((float)D3DX_PI / 2), 0.0f), CBullet::PLAYER_NORMAL);
			m_nBulletCounter = 10;
		}
	}
	else
	{
		//�e����
		if (pCInput->GetKeyboardTrigger(DIK_SPACE))
		{
			if (m_nHoldBullet >= 5)
			{
				//�e��������
				CBullet::Create(pos, D3DXVECTOR3(sinf((float)D3DX_PI / 8), cosf((float)D3DX_PI / 8), 0.0f), CBullet::PLAYER_NORMAL);
			}
			if (m_nHoldBullet >= 4)
			{
				//�e��������
				CBullet::Create(pos, D3DXVECTOR3(sinf((float)D3DX_PI / 8 * 7), cosf((float)D3DX_PI / 8 * 7), 0.0f), CBullet::PLAYER_NORMAL);
			}
			if (m_nHoldBullet >= 3)
			{
				//�e��������
				CBullet::Create(pos, D3DXVECTOR3(sinf((float)D3DX_PI / 8 * 3), cosf((float)D3DX_PI / 8 * 3), 0.0f), CBullet::PLAYER_NORMAL);
			}
			if (m_nHoldBullet >= 2)
			{
				//�e��������
				CBullet::Create(pos, D3DXVECTOR3(sinf((float)D3DX_PI / 8 * 5), cosf((float)D3DX_PI / 8 * 5), 0.0f), CBullet::PLAYER_NORMAL);
			}
			//�e��������
			CBullet::Create(pos, D3DXVECTOR3(1.0f, 0.0f, 0.0f), CBullet::PLAYER_NORMAL);
			m_nBulletCounter = 25 - (m_nHoldBullet * 3);
		}
	}
}

//=====================================
//��ʊO����
//=====================================
void CPlayer::OffScreen(D3DXVECTOR3 pos)
{
	//��ʓ������ʊO�ɏo���ꍇ
	if (pos.x + (m_fWidth / 2) > SCREEN_WIDTH)
	{//��ʉE�[�܂ŗ������̏���
		pos.x = SCREEN_WIDTH - (m_fWidth / 2);
		SetPos(pos);
	}
	if (pos.x - (m_fWidth / 2) < 0.0f)
	{//��ʍ��[�܂ŗ������̏���
		pos.x = 0.0f + (m_fWidth / 2);
		SetPos(pos);
	}
	if (pos.y + (m_fHeight / 2) > SCREEN_HEIGHT)
	{//��ʏ�[�܂ŗ������̏���
		pos.y = SCREEN_HEIGHT - (m_fHeight / 2);
		SetPos(pos);
	}
	if (pos.y - (m_fHeight / 2) < 0.0f)
	{//��ʉ��[�܂ŗ������̏���
		pos.y = 0.0f + (m_fHeight / 2);
		SetPos(pos);
	}
}

//=====================================
//��������
//=====================================
void CPlayer::Destroy(D3DXVECTOR3 pos)
{
	//������������
	CExplosion::Create(pos, m_fWidth, m_fHeight);

	m_nLife = 0;
	m_pLife->SetLife(m_nLife, m_nMaxLife, m_nFastMaxLife);
	m_situ = SITU_DEATH;
}

//=====================================
//�����蔻�菈��
//=====================================
void CPlayer::Collision()
{
	CObject *pCenter = (CObject*)CObject2D::GetTop(OBJECT_PRIORITY_CENTER);	//�I�u�W�F�N�g�̐擪�|�C���^
	if (pCenter != nullptr)
	{
		//���݈ʒu�|�C���^�ɐ擪�|�C���^����
		CObject *pObject = pCenter;

		//���݈ʒu�|�C���^���g�p���̏ꍇ
		while (pObject != nullptr)
		{
			CObject::Object_Type objtype = pObject->GetType();

			if (objtype == CObject::TYPE_ENEMY)
			{//�G�L�����Ŗ����ꍇ
				if (CollisionCircle(GetPos(), GetLength(), pObject->GetPos(), pObject->GetLength()))
				{//�d�Ȃ����ꍇ
					if (m_situ == SITU_NONE)
					{//��ԕω����̕����������������
						Hit(20);
					}
				}
			}
			//���݈ʒu�|�C���^�Ɏ���|�C���^��������(�|�C���^��i�߂鏈��)
			pObject = pObject->GetNext();
		}
	}

	CObject *pBack = (CObject*)CObject2D::GetTop(OBJECT_PRIORITY_BACK);	//�I�u�W�F�N�g�̐擪�|�C���^
	if (pBack != nullptr)
	{
		//���݈ʒu�|�C���^�ɐ擪�|�C���^����
		CObject *pObject = pBack;

		//���݈ʒu�|�C���^���g�p���̏ꍇ
		while (pObject != nullptr)
		{
			CObject::Object_Type objtype = pObject->GetType();

			if (objtype == CObject::TYPE_OBSTACLE)
			{//��Q���̏ꍇ
				CObstacle *pObstacle = (CObstacle*)pObject;

				if (pObstacle->GetFlip() == CObstacle::FLIP_CIRCLE)
				{
					if (CollisionCircle(GetPos(), GetLength(), pObject->GetPos(), pObject->GetLength()))
					{//�d�Ȃ����ꍇ
						SetPos(pObstacle->FlipCircle(GetPos(), m_posOld, GetLength()));
						if (GetPos().x - (m_fWidth / 2) <= 0.0f)
						{
							Destroy(GetPos());
						}
						if (m_situ == SITU_NONE)
						{//��ԕω����̕����������������
							Hit(20);
						}
					}
				}
				else if (pObstacle->GetFlip() == CObstacle::FLIP_SQUARE)
				{
					if (CollisionSquare(GetPos(), pObject->GetPos(), m_fWidth, m_fHeight, pObject->GetWidth(), pObject->GetHeight()))
					{//�d�Ȃ����ꍇ����
						SetPos(pObstacle->FlipSquare(GetPos(), m_posOld, m_fWidth, m_fHeight));
						if (GetPos().x - (m_fWidth / 2) <= 0.0f
							&& GetPos().y - (m_fHeight / 2) < pObject->GetPos().y + (pObject->GetHeight() / 2)
							&& GetPos().y + (m_fHeight / 2) > pObject->GetPos().y - (pObject->GetHeight() / 2))
						{
							Destroy(GetPos());
						}
						if (m_situ == SITU_NONE)
						{//��ԕω����̕����������������
							Hit(20);
						}
					}
				}
			}
			//���݈ʒu�|�C���^�Ɏ���|�C���^��������(�|�C���^��i�߂鏈��)
			pObject = pObject->GetNext();
		}
	}
}

//=====================================
//�_�ŐF����
//=====================================
void CPlayer::BlinkColor()
{
	if (m_bBlink)
	{
		m_fBlinkCounter--;
	}
	else
	{
		m_fBlinkCounter++;
	}

	if (m_fBlinkCounter >= 1.0f / PLAYER_BLINK_SPEED)
	{
		m_bBlink = true;
		m_fBlinkCounter = 1.0f / PLAYER_BLINK_SPEED;
		m_nBlinkLoop++;
	}
	else if(m_fBlinkCounter <= 0.0f)
	{
		m_bBlink = false;
		m_fBlinkCounter = 0.0f;
	}

	//�_�ł����镔���̐F���v�Z
	D3DXCOLOR colBlink = D3DXCOLOR(1.0f, 0.6f, 0.6f, m_fBlinkCounter * PLAYER_BLINK_SPEED);

	SetColor(colBlink);

	if (m_nBlinkLoop >= PLAYER_MAX_BLINK)
	{
		m_nBlinkLoop = 0;
		m_situ = SITU_NONE;
		//�F������
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//=====================================
//�v���C���[��ԏ���
//=====================================
void CPlayer::SituationType()
{
	switch (m_situ)
	{
	case CPlayer::SITU_NONE:
		break;
	case CPlayer::SITU_DAMAGE:
		BlinkColor();
		break;
	case CPlayer::SITU_DEATH:

		if (m_nDeathTimer > 120)
		{
			CGame::SetEndGame();
			Uninit();
		}
		else
		{
			m_nDeathTimer++;
		}
		break;
	default:
		break;
	}
}

//=====================================
//�v���C���[��������
//=====================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	m_pPlayer = new CPlayer;		//�v���C���[�|�C���^

	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Init();
		m_pPlayer->SetPos(pos);

		m_pPlayer->m_pLife = CLife::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 50.0f, 0.0f));
	}
	return m_pPlayer;
}

//=====================================
//�ǂݍ��ݏ���
//=====================================
void CPlayer::Load()
{
	char s_aString[256];		//�ǂݍ��ޗp������
	int nNumType = 0;			//�ǂݍ��񂾃^�C�v��

								//�t�@�C�����J��
	FILE* pFile = fopen(PLAYER_FILE, "r");

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

				//assert(PLAYER_TYPE_MAX != nNumType);	//�z���葽���t�@�C���̓ǂݍ���

				while (strncmp(s_aString, "ENDSTATE", 8) != 0)
				{
					fscanf(pFile, "%s", s_aString);		//=�̓ǂݍ���

					if (strcmp(s_aString, "LIFE") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_nLife);
					}
					else if (strcmp(s_aString, "ANIM_X") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_nAnimX);
					}
					else if (strcmp(s_aString, "ANIM_Y") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_nAnimY);
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
				nNumType++;
			}
		}
	}
	//�t�@�C�������
	fclose(pFile);
}