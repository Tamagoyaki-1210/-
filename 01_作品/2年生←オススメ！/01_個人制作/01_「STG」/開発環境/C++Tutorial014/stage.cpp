//=============================================================================
//
// �X�e�[�W
// Author : tanimoto kosuke
//
//=============================================================================
#include "stage.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "bg.h"
#include "input.h"
#include "mode.h"
#include "fade.h"
#include "obstacle.h"
#include "explosion.h"
#include "score.h"
#include "bullet.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define RESPAWN_OFFSCREEN		(SCREEN_WIDTH + 100.0f)	

//�v15400f(5��20�b��)
#define STAGE_1_WIDTH			(3400)		//�X�e�[�W����1
#define STAGE_2_WIDTH			(2400)		//�X�e�[�W����2
#define STAGE_3_WIDTH			(3200)		//�X�e�[�W����3
#define STAGE_4_WIDTH			(3200)		//�X�e�[�W����4
#define STAGE_BOSS_WIDTH		(3400)
#define STAGE_BOSS_RUSH			(300)
#define STAGE_BOSS_END			(STAGE_BOSS_WIDTH + 400)

int CStage::m_nLoop = 0;
int CStage::m_nGameTimer = 0;
bool CStage::m_bBomm = false;
bool CStage::m_bStageClear = false;
Stage_Mode CStage::m_mode = (Stage_Mode)0;
Stage_Mode CStage::m_modeNext = (Stage_Mode)0;
CBG *CStage::m_pBG = nullptr;

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CStage::CStage() : m_nDirectLoop(0)
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CStage::~CStage()
{

}

//=====================================
//����������
//=====================================
void CStage::Init()
{
	//�w�i
	m_pBG = CBG::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CBG::TYPE_BG01);

	//�v���C���[
	CPlayer::Create(D3DXVECTOR3(200.0f, SCREEN_HEIGHT * 0.5f, 0.0f));

	m_nLoop = m_nDirectLoop;
	m_nGameTimer = 0;
	m_nBlinkCounter = 0;
	m_bStageClear = false;
	m_bBlink = false;
	m_bWall = false;
	m_bChangeColor = false;
	m_bBomm = false;
	m_modeNext = (Stage_Mode)(m_nLoop % Stage_MAX);
	ChangeStage();
}

//=====================================
//�I������
//=====================================
void CStage::Uninit()
{

}

//=====================================
//�X�V����
//=====================================
void CStage::Update()
{
	if (!CGame::GetPause())
	{
		GameManager();
	}
}

//=====================================
//�`�揈��
//=====================================
void CStage::Draw()
{

}

//=====================================
//�Q�[���Ǘ�����
//=====================================
void CStage::GameManager()
{
	ArrayStage();
	//if (m_nLoop >= 6)
	//{
	//	m_nLoop = m_nDirectLoop;
	//	//�����_�̃o�[�W�����ŏI��
	//	CGame::SetEndGame();
	//}

	if (m_nGameTimer == 0)
	{
		m_bBomm = false;
	}

	//�����ɕǂ�ݒu���鏈��
	if (m_bWall == true && m_nGameTimer % 50 == 0)
	{
		CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 0.0f, 0.0f), CObstacle::TYPE_01);
		CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, F_SCREEN_HEIGHT, 0.0f), CObstacle::TYPE_01);
	}
	m_nGameTimer++;
}

//=====================================
//�X�e�[�W�؂�ւ�������
//=====================================
void CStage::ArrayStage()
{
	switch (m_mode)
	{
	case Stage_1:
		if (m_nGameTimer == 10)
		{
			//�w�i
			CBG::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CBG::TYPE_Tutorial01);
		}

		//���[�h�ύX(��)
		if (m_nGameTimer == 800)
		{
			//�G�L����
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), 200.0f, 0.0f), CEnemy::TYPE_01);
			}
		}
		if (m_nGameTimer == 1100)
		{
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), F_SCREEN_HEIGHT - 200.0f, 0.0f), CEnemy::TYPE_01);
			}
		}
		if (m_nGameTimer == 1400)
		{
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), 250.0f, 0.0f), CEnemy::TYPE_02);
			}
		}
		if (m_nGameTimer == 1700)
		{
			//��Q��
			CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, (F_SCREEN_HEIGHT / 2 + 150.0f), 0.0f), CObstacle::TYPE_02);

			//��Q��
			CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, (F_SCREEN_HEIGHT / 2 - 150.0f), 0.0f), CObstacle::TYPE_02);

			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), F_SCREEN_HEIGHT - 250.0f, 0.0f), CEnemy::TYPE_02);
			}
		}
		if (m_nGameTimer == 2000)
		{
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), 100.0f, 0.0f), CEnemy::TYPE_02);
			}
		}
		//�����_��
		if (m_nGameTimer > 2200 && m_nGameTimer < STAGE_1_WIDTH - 200)
		{
			if (m_nGameTimer % 200 == 0)
			{
				float nRandHeight = (F_SCREEN_HEIGHT / 2) + (float)(rand() % 400) - 200;
				int nRand = rand() % 2;
				if (nRand == 0)
				{
					for (int nCnt = 0; nCnt < 5; nCnt++)
					{
						CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), nRandHeight, 0.0f), CEnemy::TYPE_01);
					}
				}
				else if (nRand == 1)
				{
					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), nRandHeight, 0.0f), CEnemy::TYPE_02);
					}
				}
			}
		}
		if (m_nGameTimer % STAGE_1_WIDTH >= STAGE_1_WIDTH - 200)
		{
			BlinkStage();
		}
		if (m_nGameTimer == STAGE_1_WIDTH)
		{
			m_nLoop++;
			m_modeNext = (Stage_Mode)(m_nLoop % Stage_MAX);
			ChangeStage();
		}
		break;
	case Stage_2:
		//���[�h�ύX(��)
		if (m_nGameTimer == 10)
		{
			//�w�i
			CBG::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CBG::TYPE_Tutorial02);
		}
		if (m_nGameTimer == 300)
		{
			CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 400.0f, 0.0f), CEnemy::TYPE_03);
		}
		if (m_nGameTimer == 450)
		{
			//�G�L����
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), 200.0f, 0.0f), CEnemy::TYPE_01);
			}
		}
		if (m_nGameTimer == 600)
		{
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), 100.0f, 0.0f), CEnemy::TYPE_02);
			}
		}
		if (m_nGameTimer == 900)
		{
			m_bWall = true;
			//��Q��
			CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, (F_SCREEN_HEIGHT / 2), 0.0f), CObstacle::TYPE_02);

			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), F_SCREEN_HEIGHT - 100.0f, 0.0f), CEnemy::TYPE_02);
			}
			for (int nCnt = 0; nCnt < (rand() % 4) + 8; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (rand() % 300) - 100.0f, (F_SCREEN_HEIGHT / 2) + (rand() % 200) - 100.0f, 0.0f), CEnemy::TYPE_03);
			}
		}
		if (m_nGameTimer == 1200)
		{
			//�㉺��
			for (int i = 0; i < 2; i++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * i, 100.0f, 0.0f), CObstacle::TYPE_01);
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * i, F_SCREEN_HEIGHT - 100.0f, 0.0f), CObstacle::TYPE_01);
			}
		}
		if (m_nGameTimer == 1500)
		{
			m_bWall = false;
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), 100.0f, 0.0f), CEnemy::TYPE_02);
			}
			for (int nCnt = 0; nCnt < (rand() % 4) + 4; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (rand() % 300) - 100.0f, (F_SCREEN_HEIGHT / 2) + (rand() % 200) - 100.0f, 0.0f), CEnemy::TYPE_03);
			}
		}
		if (m_nGameTimer > 1500 && m_nGameTimer < STAGE_2_WIDTH - 200)
		{
			if (m_nGameTimer % 100 == 0)
			{
				float nRandHeight = (F_SCREEN_HEIGHT / 2) + (float)(rand() % 600) - 300;
				int nRand = rand() % 5;
				if (nRand == 0)
				{
					for (int nCnt = 0; nCnt < 5; nCnt++)
					{
						CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), nRandHeight, 0.0f), CEnemy::TYPE_01);
					}
				}
				else if (nRand == 1)
				{
					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), nRandHeight, 0.0f), CEnemy::TYPE_02);
					}
				}
				else if (nRand >= 3)
				{
					for (int nCnt = 0; nCnt < (rand() % 5) + 5; nCnt++)
					{
						CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (rand() % 300) - 100.0f, (F_SCREEN_HEIGHT / 2) + (float)(rand() % 400) - 200, 0.0f), CEnemy::TYPE_03);
					}
				}
			}
		}
		if (m_nGameTimer % STAGE_2_WIDTH >= STAGE_2_WIDTH - 200)
		{
			BlinkStage();
		}
		if (m_nGameTimer == STAGE_2_WIDTH)
		{
			m_nLoop++;
			m_modeNext = (Stage_Mode)(m_nLoop % Stage_MAX);
			ChangeStage();
		}
		break;
	case Stage_3:
		//���[�h�ύX(��)
		if (m_nGameTimer == 10)
		{
			//�w�i
			CBG::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CBG::TYPE_Tutorial03);
		}
		//�����_��
		if (m_nGameTimer > 1500 && m_nGameTimer < STAGE_3_WIDTH - 200)
		{
			if (m_nGameTimer % 200 == 0)
			{
				float nRandHeight = (F_SCREEN_HEIGHT / 2) + (float)(rand() % 600) - 300;
				int nRand = rand() % 5;

				if (nRand == 0)
				{
					for (int nCnt = 0; nCnt < 5; nCnt++)
					{
						CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), nRandHeight, 0.0f), CEnemy::TYPE_01);
					}
				}
				else if (nRand == 1)
				{
					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), nRandHeight, 0.0f), CEnemy::TYPE_02);
					}
				}
				else if (nRand == 2)
				{
					for (int nCnt = 0; nCnt < (rand() % 4) + 4; nCnt++)
					{
						CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (rand() % 300) - 100.0f, (F_SCREEN_HEIGHT / 2) + (float)(rand() % 400) - 200, 0.0f), CEnemy::TYPE_03);
					}
				}
				else if (nRand == 3)
				{
					//������tL��
					for (int i = 0; i < 3; i++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f, F_SCREEN_HEIGHT - 100.0f * i, 0.0f), CObstacle::TYPE_01);
					}
					for (int j = 0; j < 2; j++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f * j, F_SCREEN_HEIGHT, 0.0f), CObstacle::TYPE_01);
					}
					//����
					CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, F_SCREEN_HEIGHT - 100.0f, 0.0f), CEnemy::TYPE_04);
				}
				else if (nRand == 4)
				{
					//�������tL��
					for (int i = 0; i < 3; i++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f, 100.0f * i, 0.0f), CObstacle::TYPE_01);
					}
					for (int j = 0; j < 2; j++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * j, 0.0f, 0.0f), CObstacle::TYPE_01);
					}
					//�㑤
					CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 100.0f, 0.0f), CEnemy::TYPE_04);
				}
			}
		}
		if (m_nGameTimer == 300)
		{
			//������tL��
			for (int i = 0; i < 3; i++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f, F_SCREEN_HEIGHT - 100.0f * i, 0.0f), CObstacle::TYPE_01);
			}
			for (int j = 0; j < 2; j++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f * j, F_SCREEN_HEIGHT, 0.0f), CObstacle::TYPE_01);
			}
			//����
			CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, F_SCREEN_HEIGHT - 100.0f, 0.0f), CEnemy::TYPE_04);
		}
		if (m_nGameTimer == 600)
		{
			//�������tL��
			for (int i = 0; i < 3; i++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f, 100.0f * i, 0.0f), CObstacle::TYPE_01);
			}
			for (int j = 0; j < 2; j++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * j, 0.0f, 0.0f), CObstacle::TYPE_01);
			}
			//�㑤
			CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 100.0f, 0.0f), CEnemy::TYPE_04);
		}
		if (m_nGameTimer == 900)
		{
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), F_SCREEN_HEIGHT - 200.0f, 0.0f), CEnemy::TYPE_01);
			}
		}
		if (m_nGameTimer == 1200)
		{
			for (int nCnt = 0; nCnt < 6; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), (F_SCREEN_HEIGHT / 2), 0.0f), CEnemy::TYPE_02);
			}
		}
		if (m_nGameTimer % STAGE_3_WIDTH >= STAGE_3_WIDTH - 200)
		{
			BlinkStage();
		}
		if (m_nGameTimer == STAGE_3_WIDTH)
		{
			m_nLoop++;
			m_modeNext = (Stage_Mode)(m_nLoop % Stage_MAX);
			ChangeStage();
		}
		break;
	case Stage_4:
		//���[�h�ύX(��)
		//�X�e�[�W1
		if (m_nGameTimer == 300)
		{
			//�G�L����
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), 200.0f, 0.0f), CEnemy::TYPE_01);
			}
		}
		if (m_nGameTimer == 400)
		{
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), F_SCREEN_HEIGHT - 200.0f, 0.0f), CEnemy::TYPE_01);
			}
		}
		if (m_nGameTimer == 500)
		{
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), 250.0f, 0.0f), CEnemy::TYPE_02);
			}
		}
		if (m_nGameTimer == 600)
		{
			//��Q��
			CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, (F_SCREEN_HEIGHT / 2 + 150.0f), 0.0f), CObstacle::TYPE_02);

			//��Q��
			CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, (F_SCREEN_HEIGHT / 2 - 150.0f), 0.0f), CObstacle::TYPE_02);

			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), F_SCREEN_HEIGHT - 250.0f, 0.0f), CEnemy::TYPE_02);
			}
		}
		if (m_nGameTimer == 700)
		{
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), 100.0f, 0.0f), CEnemy::TYPE_02);
			}
		}

		//�X�e�[�W2
		if (m_nGameTimer == 800)
		{
			//�G�L����
			CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 400.0f, 0.0f), CEnemy::TYPE_03);
		}
		if (m_nGameTimer == 900)
		{
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), 200.0f, 0.0f), CEnemy::TYPE_01);
			}
		}
		if (m_nGameTimer == 1000)
		{
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), 100.0f, 0.0f), CEnemy::TYPE_02);
			}
		}
		if (m_nGameTimer == 1100)
		{
			m_bWall = true;
			//��Q��
			CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, (F_SCREEN_HEIGHT / 2), 0.0f), CObstacle::TYPE_02);

			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), F_SCREEN_HEIGHT - 100.0f, 0.0f), CEnemy::TYPE_02);
			}
			for (int nCnt = 0; nCnt < (rand() % 4) + 4; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (rand() % 300) - 100.0f, (F_SCREEN_HEIGHT / 2) + (rand() % 200) - 100.0f, 0.0f), CEnemy::TYPE_03);
			}
		}
		if (m_nGameTimer == 1400)
		{
			//�㉺��
			for (int i = 0; i < 2; i++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * i, 100.0f, 0.0f), CObstacle::TYPE_01);
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * i, F_SCREEN_HEIGHT - 100.0f, 0.0f), CObstacle::TYPE_01);
			}
		}
		if (m_nGameTimer == 1700)
		{
			m_bWall = false;
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), 100.0f, 0.0f), CEnemy::TYPE_02);
			}
			for (int nCnt = 0; nCnt < (rand() % 4) + 8; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (rand() % 300) - 100.0f, (F_SCREEN_HEIGHT / 2) + (rand() % 200) - 100.0f, 0.0f), CEnemy::TYPE_03);
			}
		}

		//�X�e�[�W3
		if (m_nGameTimer == 1900)
		{
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), 200.0f, 0.0f), CEnemy::TYPE_01);
			}
			//������tL��
			for (int i = 0; i < 3; i++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f, F_SCREEN_HEIGHT - 100.0f * i, 0.0f), CObstacle::TYPE_01);
			}
			for (int j = 0; j < 2; j++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f * j, F_SCREEN_HEIGHT, 0.0f), CObstacle::TYPE_01);
			}
			//����
			CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, F_SCREEN_HEIGHT - 100.0f, 0.0f), CEnemy::TYPE_04);
		}
		if (m_nGameTimer == 2050)
		{
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), F_SCREEN_HEIGHT - 200.0f, 0.0f), CEnemy::TYPE_01);
			}
			//�������tL��
			for (int i = 0; i < 3; i++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f, 100.0f * i, 0.0f), CObstacle::TYPE_01);
			}
			for (int j = 0; j < 2; j++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * j, 0.0f, 0.0f), CObstacle::TYPE_01);
			}
			//�㑤
			CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 100.0f, 0.0f), CEnemy::TYPE_04);
		}
		if (m_nGameTimer == 2200)
		{
			for (int nCnt = 0; nCnt < 6; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), (F_SCREEN_HEIGHT / 2), 0.0f), CEnemy::TYPE_02);
			}
			//������tL��
			for (int i = 0; i < 3; i++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f, F_SCREEN_HEIGHT - 100.0f * i, 0.0f), CObstacle::TYPE_01);
			}
			for (int j = 0; j < 2; j++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f * j, F_SCREEN_HEIGHT, 0.0f), CObstacle::TYPE_01);
			}
			//����
			CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, F_SCREEN_HEIGHT - 100.0f, 0.0f), CEnemy::TYPE_04);

			//�������tL��
			for (int i = 0; i < 3; i++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f, 100.0f * i, 0.0f), CObstacle::TYPE_01);
			}
			for (int j = 0; j < 2; j++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * j, 0.0f, 0.0f), CObstacle::TYPE_01);
			}
			//�㑤
			CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 100.0f, 0.0f), CEnemy::TYPE_04);
		}

		//�����_��
		if (m_nGameTimer > 2300 && m_nGameTimer < STAGE_4_WIDTH - 200)
		{
			int nCounter = 100;

			if (m_nGameTimer % nCounter == 0)
			{
				float nRandHeight = (F_SCREEN_HEIGHT / 2) + (float)(rand() % 600) - 300;
				int nRand = rand() % 8;

				if (nRand == 0 || nRand == 5)
				{
					for (int nCnt = 0; nCnt < 5; nCnt++)
					{
						CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), nRandHeight, 0.0f), CEnemy::TYPE_01);
					}
				}
				else if (nRand == 1 || nRand == 6)
				{
					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), nRandHeight, 0.0f), CEnemy::TYPE_02);
					}
				}
				else if (nRand == 2 || nRand == 7)
				{
					for (int nCnt = 0; nCnt < (rand() % 2) + 4; nCnt++)
					{
						CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (rand() % 300) - 100.0f, (F_SCREEN_HEIGHT / 2) + (float)(rand() % 400) - 200, 0.0f), CEnemy::TYPE_03);
					}
				}
				else if (nRand == 3)
				{
					//������tL��
					for (int i = 0; i < 3; i++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f, F_SCREEN_HEIGHT - 100.0f * i, 0.0f), CObstacle::TYPE_01);
					}
					for (int j = 0; j < 2; j++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f * j, F_SCREEN_HEIGHT, 0.0f), CObstacle::TYPE_01);
					}
					//����
					CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, F_SCREEN_HEIGHT - 100.0f, 0.0f), CEnemy::TYPE_04);
				}
				else if (nRand == 4)
				{
					//�������tL��
					for (int i = 0; i < 3; i++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f, 100.0f * i, 0.0f), CObstacle::TYPE_01);
					}
					for (int j = 0; j < 2; j++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * j, 0.0f, 0.0f), CObstacle::TYPE_01);
					}
					//�㑤
					CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 100.0f, 0.0f), CEnemy::TYPE_04);
				}
				if (rand() % 4 > 0)
				{
					//��Q��
					CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, (F_SCREEN_HEIGHT / 2) + (float)(rand() % 600) - 300, 0.0f), CObstacle::TYPE_02);
				}
			}
		}
		if (m_nGameTimer % STAGE_4_WIDTH >= STAGE_4_WIDTH - 200)
		{
			BlinkStage();
		}
		if (m_nGameTimer == STAGE_4_WIDTH)
		{
			m_nLoop++;
			m_modeNext = (Stage_Mode)(m_nLoop % Stage_MAX);
			ChangeStage();
		}
		break;
	case Stage_Boss:
		//�����_��
		if (m_nGameTimer > 100 && m_nGameTimer < STAGE_BOSS_WIDTH - 200)
		{
			int nCounter = 100;

			if (m_nGameTimer > 1000)
			{
				nCounter -= 30;
			}
			if (m_nGameTimer > 2000)
			{
				nCounter -= 30;
			}
			if (m_nGameTimer > STAGE_BOSS_WIDTH - STAGE_BOSS_RUSH - 200)
			{
				nCounter -= 30;
			}

			if (m_nGameTimer % nCounter == 0)
			{
				float nRandHeight = (F_SCREEN_HEIGHT / 2) + (float)(rand() % 600) - 300;
				int nRand = rand() % 8;

				if (nRand == 0 || nRand == 5)
				{
					for (int nCnt = 0; nCnt < 5; nCnt++)
					{
						CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), nRandHeight, 0.0f), CEnemy::TYPE_01);
					}
				}
				else if (nRand == 1 || nRand == 6)
				{
					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), nRandHeight, 0.0f), CEnemy::TYPE_02);
					}
				}
				else if (nRand == 2 || nRand == 7)
				{
					for (int nCnt = 0; nCnt < (rand() % 2) + 4; nCnt++)
					{
						CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (rand() % 300) - 100.0f, (F_SCREEN_HEIGHT / 2) + (float)(rand() % 400) - 200, 0.0f), CEnemy::TYPE_03);
					}
				}
				else if (nRand == 3)
				{
					//������tL��
					for (int i = 0; i < 3; i++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f, F_SCREEN_HEIGHT - 100.0f * i, 0.0f), CObstacle::TYPE_01);
					}
					for (int j = 0; j < 2; j++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f * j, F_SCREEN_HEIGHT, 0.0f), CObstacle::TYPE_01);
					}
					//����
					CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, F_SCREEN_HEIGHT - 100.0f, 0.0f), CEnemy::TYPE_04);
				}
				else if (nRand == 4)
				{
					//�������tL��
					for (int i = 0; i < 3; i++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f, 100.0f * i, 0.0f), CObstacle::TYPE_01);
					}
					for (int j = 0; j < 2; j++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * j, 0.0f, 0.0f), CObstacle::TYPE_01);
					}
					//�㑤
					CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 100.0f, 0.0f), CEnemy::TYPE_04);
				}

				if (rand() % 10 == 0)
				{
					//��Q��
					CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, (F_SCREEN_HEIGHT / 2) + (float)(rand() % 600) - 300, 0.0f), CObstacle::TYPE_02);
				}
			}
		}
		if (m_nGameTimer == STAGE_BOSS_WIDTH - STAGE_BOSS_RUSH)
		{
			CSound::PlaySound(SOUND_LABEL_BGM_EXPLOSION);
		}
		//���[�h�ύX(��)
		if (m_nGameTimer > STAGE_BOSS_WIDTH - STAGE_BOSS_RUSH && m_nGameTimer < STAGE_BOSS_WIDTH)
		{
			if (m_nGameTimer % 10 == 0)
			{
				float nRandWidth = (float)(rand() % SCREEN_WIDTH);
				float nRandHeight = (float)(rand() % SCREEN_HEIGHT);
				float nExplosionX = EXPLOSION_WIDTH + (float)(rand() % EXPLOSION_WIDTH);
				float nExplosionY = EXPLOSION_HEIGHT + (float)(rand() % EXPLOSION_HEIGHT);
				//������������
				CExplosion::Create(D3DXVECTOR3(nRandWidth, nRandHeight, 0.0f), nExplosionX, nExplosionY);
			}
			BlinkStage();
		}
		if (m_nGameTimer == STAGE_BOSS_WIDTH)
		{
			m_bBomm = true;
			CBullet::Create(D3DXVECTOR3(F_SCREEN_WIDTH / 2, F_SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CBullet::PLAYER_BOMM);
			//������������
			CExplosion::Create(D3DXVECTOR3(F_SCREEN_WIDTH / 2, F_SCREEN_HEIGHT / 2, 0.0f), F_SCREEN_WIDTH, F_SCREEN_WIDTH);
			CSound::StopSound(SOUND_LABEL_BGM_EXPLOSION);
			CSound::StopSound(SOUND_LABEL_BGM_BOSS);

			CScore *pScore = CScore::GetScore();
			pScore->AddScore(10000);
			m_bStageClear = true;
		}
		if (m_nGameTimer == STAGE_BOSS_END)
		{
			CGame::SetEndGame();
		}
		break;
	default:
		break;
	}
}

//=====================================
//�X�e�[�W�؂�ւ�������
//=====================================
void CStage::ChangeStage()
{
	CScore *pScore = CScore::GetScore();
	switch (m_mode)
	{
	case Stage_1:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f));
		CSound::StopSound(SOUND_LABEL_BGM_STAGE1);
		break;
	case Stage_2:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f));
		CSound::StopSound(SOUND_LABEL_BGM_STAGE2);
		break;
	case Stage_3:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f));
		CSound::StopSound(SOUND_LABEL_BGM_STAGE3);
		break;
	case Stage_4:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));
		CSound::StopSound(SOUND_LABEL_BGM_STAGE4);
		break;
	case Stage_Boss:
		//(�{�X�p)
		m_pBG->SetColor(D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f), D3DXCOLOR(0.4f, 1.0f, 0.4f, 0.8f), D3DXCOLOR(0.4f, 0.4f, 1.0f, 0.8f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f));
		CSound::StopSound(SOUND_LABEL_BGM_BOSS);
		break;
	default:
		break;
	}

	m_mode = m_modeNext;	//���̃��[�h�Ɉړ�

	switch (m_mode)
	{
	case Stage_1:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f));
		CSound::PlaySound(SOUND_LABEL_BGM_STAGE1);
		break;
	case Stage_2:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f));
		CSound::PlaySound(SOUND_LABEL_BGM_STAGE2);
		pScore->AddScore(1000);
		break;
	case Stage_3:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f));
		CSound::PlaySound(SOUND_LABEL_BGM_STAGE3);
		pScore->AddScore(2000);
		break;
	case Stage_4:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));
		CSound::PlaySound(SOUND_LABEL_BGM_STAGE4);
		pScore->AddScore(3000);
		break;
	case Stage_Boss:
		//(�{�X�p)
		m_pBG->SetColor(D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f), D3DXCOLOR(0.4f, 1.0f, 0.4f, 0.8f), D3DXCOLOR(0.4f, 0.4f,1.0f, 0.8f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f));
		CSound::PlaySound(SOUND_LABEL_BGM_BOSS);
		pScore->AddScore(5000);
		break;
	default:
		break;
	}
	m_bBomm = true;
	CBullet::Create(D3DXVECTOR3(F_SCREEN_WIDTH / 2, F_SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CBullet::PLAYER_BOMM);
	m_nGameTimer = 0;
	m_bChangeColor = false;
}

//=====================================
//�X�e�[�W�_�ŏ���
//=====================================
void CStage::BlinkStage()
{
	float fAlpha = (m_nBlinkCounter * 0.005f);

	if (fAlpha > 0.2f)
	{
		m_bBlink = true;
		fAlpha = 0.2f;
		if (m_bChangeColor == false)
		{
			m_bChangeColor = true;
		}
		else
		{
			m_bChangeColor = false;
		}
	}
	else if (fAlpha < 0.0f)
	{
		m_bBlink = false;
		fAlpha = 0.0f;
	}

	switch (m_mode)
	{
	case Stage_1:
		//(��)
		if (m_bChangeColor == false)
		{
			m_pBG->SetColor(D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f - fAlpha), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f - fAlpha), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f - fAlpha), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f - fAlpha));
		}
		else
		{
			m_pBG->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f - fAlpha), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f - fAlpha), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f - fAlpha), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f - fAlpha));
		}
		break;
	case Stage_2:
		//(��)
		if (m_bChangeColor == false)
		{
			m_pBG->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f - fAlpha), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f - fAlpha), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f - fAlpha), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f - fAlpha));
		}
		else
		{
			m_pBG->SetColor(D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f - fAlpha), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f - fAlpha), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f - fAlpha), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f - fAlpha));
		}
		break;
	case Stage_3:
		//(��)
		if (m_bChangeColor == false)
		{
			m_pBG->SetColor(D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f - fAlpha), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f - fAlpha), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f - fAlpha), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f - fAlpha));
		}
		else
		{
			m_pBG->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f - fAlpha), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f - fAlpha), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f - fAlpha), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f - fAlpha));
		}
		break;
	case Stage_4:
		//(��)
		if (m_bChangeColor == false)
		{
			m_pBG->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f - fAlpha), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f - fAlpha), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f - fAlpha), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f - fAlpha));
		}
		else
		{
			m_pBG->SetColor(D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f - fAlpha), D3DXCOLOR(0.4f, 1.0f, 0.4f, 0.8f - fAlpha), D3DXCOLOR(0.4f, 0.4f, 1.0f, 0.8f - fAlpha), D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f - fAlpha));
		}
		break;
	case Stage_Boss:
		//(��)
		if (m_bChangeColor == false)
		{
			m_pBG->SetColor(D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f - fAlpha), D3DXCOLOR(0.4f, 1.0f, 0.4f, 0.8f - fAlpha), D3DXCOLOR(0.4f, 0.4f, 1.0f, 0.8f - fAlpha), D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f - fAlpha));
		}
		else
		{
			m_pBG->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f - fAlpha), D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.8f - fAlpha), D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f - fAlpha), D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f - fAlpha));
		}
		break;
	default:
		break;
	}

	if (m_bBlink == true)
	{
		m_nBlinkCounter--;
	}
	else
	{
		m_nBlinkCounter++;
	}
}