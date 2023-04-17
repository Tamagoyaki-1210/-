//=============================================================================
//
// �e
// Author : tanimoto kosuke
//
//=============================================================================
#include "bullet.h"
#include <stdio.h>
#include "matrix.h"
#include "input.h"
#include "application.h"
#include "texture.h"
#include "explosion.h"
#include "player.h"
#include "enemy.h"
#include "ray.h"
#include "obstacle.h"
#include "score.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define PLAYER_BULLET_FILE		"data/.txt/player_bullet.txt"	//�ǂݍ��݃t�@�C��
#define ENEMY_BULLET_FILE		"data/.txt/enemy_bullet.txt"	//�ǂݍ��݃t�@�C��

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CBullet::CBullet() : CObject2D(EFFECT_PRIORITY_CENTER), m_nPenetrat(0)
{
	//�G�L�����|�C���^����������
	for (int nInd = 0; nInd < MAX_ENEMY_HIT; nInd++)
	{
		m_pEnemy[nInd] = nullptr;
	}

	//��Q���|�C���^����������
	for (int nInd = 0; nInd < MAX_OBSTACLE_HIT; nInd++)
	{
		m_pObstacle[nInd] = nullptr;
	}
}

//=====================================
//�f�X�g���N�^
//=====================================
CBullet::~CBullet()
{

}

//=====================================
//����������
//=====================================
void CBullet::Init()
{
	CObject2D::Init();
}

//=====================================
//�I������
//=====================================
void CBullet::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//�X�V����
//=====================================
void CBullet::Update()
{
	CObject2D::Update();

	//UV�ݒ�
	CObject2D::SetUV();

	//�ʒu�擾����
	D3DXVECTOR3 pos = CObject2D::GetPos();

	m_poslast = pos;
	
	//�����擾����
	float fLength = CObject2D::GetLength();

	//�ʒu�X�V
	pos += m_move * m_fSpeed;

	//�ʒu�ݒ菈��
	SetPos(pos);

	//�ʒu�ݒ菈��
	SetLength(fLength);

	//���AI����
	TypeAI(pos);

	//�����蔻��ݒ菈��
	Collision(pos, fLength);

	//�����v�Z
	if (m_nLife <= 0)
	{
		Destroy();
	}
	else
	{//���������炷
		m_nLife--;
		OffScreen(pos);
	}
}

//=====================================
//�`�揈��
//=====================================
void CBullet::Draw()
{
	switch (m_collisionBullet)
	{
	case COLLISION_CIRCLE:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_OBSTACLE_02);
		break;
	case COLLISION_SQUARE:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_BULLET_02);
		break;
	default:
		break;
	}
	CObject2D::Draw();
}

//=====================================
//���AI����
//=====================================
void CBullet::TypeAI(D3DXVECTOR3 pos)
{
	//�v���C���[�p�e
	switch (m_playerBullet)
	{
	case PLAYER_NONE :
		break;
	case PLAYER_NORMAL:
		m_collisionBullet = COLLISION_CIRCLE;
		m_bHitDestroy = true;
		CRay::Create(pos, m_move, CRay::BULLET);
		break;
	case PLAYER_LASER:
		SetColor(D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));
		m_collisionBullet = COLLISION_SQUARE;
		if (m_fWidth < m_fMaxWidth)
		{
			m_fWidth += 5;
			SetObject2D(TYPE_EFFECT, m_fWidth, m_fHeight);
		}
		m_bHitDestroy = false;
		break;
	case PLAYER_BOMM:
		SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
		m_collisionBullet = COLLISION_SQUARE;
		m_bHitDestroy = false;
		break;
	default:
		break;
	}

	//�G�L�����p�e
	switch (m_enemyBullet)
	{
	case ENEMY_NONE:
		break;
	case ENEMY_NORMAL:
		m_collisionBullet = COLLISION_CIRCLE;
		m_bHitDestroy = true;
		CRay::Create(pos, m_move, CRay::BULLET);
		break;
	case ENEMY_LASER:
		m_collisionBullet = COLLISION_SQUARE;
		m_bHitDestroy = false;
		break;

	case ENEMY_SPREAD:
		m_collisionBullet = COLLISION_CIRCLE;
		m_bHitDestroy = true;
		CRay::Create(pos, m_move, CRay::BULLET);

		if (m_fSpeed < 2.0f)
		{
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				CBullet::Create(pos, D3DXVECTOR3(-sinf(D3DX_PI / 5 * nCnt * 2), -cosf(D3DX_PI / 5 * nCnt * 2), 0.0f), CBullet::ENEMY_NORMAL);	//�e��������
			}
			Destroy();
		}
		else
		{
			m_fSpeed *= 0.98f;
		}

		break;
	default:
		break;
	}
}

//=====================================
//��ʊO����
//=====================================
void CBullet::OffScreen(D3DXVECTOR3 pos)
{
	if (pos.x < -m_fWidth / 2 || pos.x > F_SCREEN_WIDTH || pos.y  < -m_fHeight / 2 || pos.y > F_SCREEN_HEIGHT + m_fHeight / 2)
	{
		Uninit();
	}
}

//=====================================
//��������
//=====================================
void CBullet::Destroy()
{
	CBullet::Uninit();
}

//=====================================
//�����蔻��ݒ菈��
//=====================================
void CBullet::Collision(D3DXVECTOR3 pos, float fLength)
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

			//�v���C���[�e�̏ꍇ
			if (m_playerBullet != PLAYER_NONE)
			{
				//�Ώۂ��G�L�����̏ꍇ
				if (objtype == CObject::TYPE_ENEMY)
				{
					CEnemy *pEnemy = (CEnemy*)pObject;

					//�~�`�����蔻��̏ꍇ
					if (m_collisionBullet == COLLISION_CIRCLE)
					{//�d�Ȃ��Ă���ꍇ
						if (CollisionCircle(pos, fLength, pObject->GetPos(), pObject->GetLength()))
						{
							//������������
							CExplosion::Create(pos, EXPLOSION_WIDTH, EXPLOSION_HEIGHT);

							//��e����
							pEnemy->Hit(m_nPower);

							//10�_���Z
							CScore *pScore = CScore::GetScore();
							pScore->AddScore(20);

							//�e�j��
							Destroy();
						}
					}
					//�l�p�`�����蔻��̏ꍇ
					else if (m_collisionBullet == COLLISION_SQUARE)
					{//�d�Ȃ��Ă���ꍇ
						if (CollisionSquare(pos, pObject->GetPos(), m_fWidth, m_fHeight, pObject->GetWidth(), pObject->GetHeight()))
						{
							//�e�ƃI�u�W�F�N�g��������������
							bool bHit = false;

							for (int nIndEnemy = 0; nIndEnemy < MAX_ENEMY_HIT; nIndEnemy++)
							{
								//�G�L�����̃|�C���^����v�����ꍇ
								if (m_pEnemy[nIndEnemy] == pEnemy)
								{
									bHit = true;
									break;
								}
							}
							//�e���q�b�g�O�̏ꍇ
							if (!bHit)
							{
								if (pEnemy->GetType() != CEnemy::TYPE_03)
								{
									pEnemy->SetSitu(CEnemy::SITU_DAMAGE);
								}

								if (m_playerBullet != PLAYER_BOMM)
								{
									CSound::PlaySound(SOUND_LABEL_SE_BULLET_LASER_DAMAGE);
								}
								//��e����
								pEnemy->Hit(m_nPower);

								CScore *pScore = CScore::GetScore();
								if (m_playerBullet == PLAYER_BOMM)
								{
									if (m_nPenetrat < 5)
									{
										m_nPenetrat++;
									}
									pScore->AddScore(100 * m_nPenetrat);
								}
								else
								{
									pScore->AddScore(10);
								}

								for (int nIndEnemy = 0; nIndEnemy < MAX_ENEMY_HIT; nIndEnemy++)
								{
									//���g�����݂��Ȃ��ꍇ
									if (m_pEnemy[nIndEnemy] == nullptr)
									{
										//Hit�����I�u�W�F�N�g�̃|�C���^�𒆐g�ɓo�^����
										m_pEnemy[nIndEnemy] = pEnemy;
										break;
									}
								}
							}
						}
					}
				}
			}
			//�G�L�����e�̏ꍇ
			else if (m_enemyBullet != ENEMY_NONE)
			{	//�v���C���[�̏ꍇ
				if (objtype == CObject::TYPE_PLAYER)
				{
					CPlayer *pPlayer = (CPlayer*)pObject;

					//�v���C���[���ʏ��Ԃ̏ꍇ
					if (pPlayer->GetSitu() == CPlayer::SITU_NONE)
					{
						//�~�`�̏ꍇ
						if (m_collisionBullet == COLLISION_CIRCLE)
						{//�d�Ȃ��Ă���ꍇ
							if (CollisionCircle(pos, fLength, pObject->GetPos(), pObject->GetLength()))
							{
								//�e�j��
								Destroy();

								//��e����
								pPlayer->Hit(m_nPower);
							}
						}
						//�l�p�`�̏ꍇ
						else if (m_collisionBullet == COLLISION_SQUARE)
						{//�d�Ȃ��Ă���ꍇ
							if (CollisionSquare(pos, pObject->GetPos(), m_fWidth, m_fHeight, pObject->GetWidth(), pObject->GetHeight()))
							{
								//�e�j��
								Destroy();

								//��e����
								pPlayer->Hit(m_nPower);
							}
						}
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
			//�v���C���[�e�̏ꍇ
			if (m_playerBullet != PLAYER_NONE)
			{
				//��Q���̏ꍇ
				if (objtype == CObject::TYPE_OBSTACLE)
				{
					CObstacle *pObstacle = (CObstacle*)pObject;

					//�~�`�̏ꍇ
					if (m_collisionBullet == COLLISION_CIRCLE)
					{
						//�d�Ȃ��Ă���ꍇ
						if (CollisionCircle(pos, fLength, pObject->GetPos(), pObject->GetLength()))
						{
							//������������
							CExplosion::Create(pos, EXPLOSION_WIDTH, EXPLOSION_HEIGHT);
							//�e�j��
							Destroy();
							//��e����
							pObstacle->Hit(m_nPower);
						}
					}
					//�l�p�`�̏ꍇ
					else if (m_collisionBullet == COLLISION_SQUARE)
					{
						//�d�Ȃ��Ă���ꍇ����
						if (CollisionSquare(pos, pObject->GetPos(), m_fWidth, m_fHeight, pObject->GetWidth(), pObject->GetHeight()))
						{
							//�e�ƃI�u�W�F�N�g��������������
							bool bHit = false;

							for (int nIndObstacle = 0; nIndObstacle < MAX_OBSTACLE_HIT; nIndObstacle++)
							{
								//�G�L�����̃|�C���^����v�����ꍇ
								if (m_pObstacle[nIndObstacle] == pObstacle)
								{
									bHit = true;
									break;
								}
							}
							//�e���q�b�g�O�̏ꍇ
							if (!bHit)
							{
								pObstacle->SetSitu(CObstacle::SITU_DAMAGE);

								if (m_playerBullet != PLAYER_BOMM)
								{
									CSound::PlaySound(SOUND_LABEL_SE_BULLET_LASER_DAMAGE);
								}
								//��e����
								pObstacle->Hit(m_nPower * 5);

								CScore *pScore = CScore::GetScore();
								if (m_playerBullet == PLAYER_BOMM)
								{
									if (m_nPenetrat < 5)
									{
										m_nPenetrat++;
									}
									pScore->AddScore(100 * m_nPenetrat);
								}

								for (int nIndObstacle = 0; nIndObstacle < MAX_OBSTACLE_HIT; nIndObstacle++)
								{
									//���g�����݂��Ȃ��ꍇ
									if (m_pObstacle[nIndObstacle] == nullptr)
									{
										//Hit�����I�u�W�F�N�g�̃|�C���^�𒆐g�ɓo�^����
										m_pObstacle[nIndObstacle] = pObstacle;
										break;
									}
								}
							}
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
//�v���C���[�e��������
//=====================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, Bullet_Player playerBullet)
{
	CBullet* pBullet = new CBullet;	//�e�̃|�C���^

	if (pBullet != nullptr)
	{
		pBullet->Init();
		pBullet->SetPos(pos);
		pBullet->m_move = move;	//�e�i�s����
		pBullet->m_playerBullet = playerBullet;
		pBullet->LoadPlayerBullet();

		switch (pBullet->m_playerBullet)
		{
		case PLAYER_NORMAL:
			CSound::PlaySound(SOUND_LABEL_SE_BULLET_NORMAL);
			break;
		case PLAYER_LASER:
			CSound::PlaySound(SOUND_LABEL_SE_BULLET_LASER);
			pBullet->m_fMaxWidth = pBullet->m_fWidth;
			pBullet->m_fWidth = 20.0f;
			break;
		default:
			break;
		}
		//�I�u�W�F�N�g�ݒ菈��
		pBullet->SetObject2D(TYPE_EFFECT, pBullet->m_fWidth, pBullet->m_fHeight);
	}

	return pBullet;
}

//=====================================
//�G�L�����e��������
//=====================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, Bullet_Enemy enemyBullet)
{
	CBullet* pBullet = new CBullet;	//�e�̃|�C���^

	if (pBullet != nullptr)
	{
		pBullet->Init();
		pBullet->SetPos(pos);
		pBullet->m_move = move;	//�e�i�s����
		pBullet->m_enemyBullet = enemyBullet;
		pBullet->LoadEnemyBullet();
		pBullet->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));	//���F
		//�I�u�W�F�N�g�ݒ菈��
		pBullet->SetObject2D(TYPE_EFFECT, pBullet->m_fWidth, pBullet->m_fHeight);
	}
	return pBullet;
}

//=====================================
//�ǂݍ��ݏ���(�v���C���[�e)
//=====================================
void CBullet::LoadPlayerBullet()
{
	char s_aString[256];		//�ǂݍ��ޗp������
	int nNumType = 0;			//�ǂݍ��񂾃^�C�v��

								//�t�@�C�����J��
	FILE* pFile = fopen(PLAYER_BULLET_FILE, "r");

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
					else if (strcmp(s_aString, "POWER") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_nPower);
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
				if (m_playerBullet == nNumType)
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


//=====================================
//�ǂݍ��ݏ���(�G�L�����e)
//=====================================
void CBullet::LoadEnemyBullet()
{
	char s_aString[256];		//�ǂݍ��ޗp������
	int nNumType = 0;			//�ǂݍ��񂾃^�C�v��

								//�t�@�C�����J��
	FILE* pFile = fopen(ENEMY_BULLET_FILE, "r");

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
					else if (strcmp(s_aString, "POWER") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_nPower);
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
				if (m_enemyBullet == nNumType)
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