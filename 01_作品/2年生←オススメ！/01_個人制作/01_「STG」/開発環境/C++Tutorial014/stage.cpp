//=============================================================================
//
// ステージ
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
//マクロ定義
//****************************************************************************
#define RESPAWN_OFFSCREEN		(SCREEN_WIDTH + 100.0f)	

//計15400f(5分20秒程)
#define STAGE_1_WIDTH			(3400)		//ステージ長さ1
#define STAGE_2_WIDTH			(2400)		//ステージ長さ2
#define STAGE_3_WIDTH			(3200)		//ステージ長さ3
#define STAGE_4_WIDTH			(3200)		//ステージ長さ4
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
//デフォルトコンストラクタ
//=====================================
CStage::CStage() : m_nDirectLoop(0)
{

}

//=====================================
//デストラクタ
//=====================================
CStage::~CStage()
{

}

//=====================================
//初期化処理
//=====================================
void CStage::Init()
{
	//背景
	m_pBG = CBG::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CBG::TYPE_BG01);

	//プレイヤー
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
//終了処理
//=====================================
void CStage::Uninit()
{

}

//=====================================
//更新処理
//=====================================
void CStage::Update()
{
	if (!CGame::GetPause())
	{
		GameManager();
	}
}

//=====================================
//描画処理
//=====================================
void CStage::Draw()
{

}

//=====================================
//ゲーム管理処理
//=====================================
void CStage::GameManager()
{
	ArrayStage();
	//if (m_nLoop >= 6)
	//{
	//	m_nLoop = m_nDirectLoop;
	//	//現時点のバージョンで終了
	//	CGame::SetEndGame();
	//}

	if (m_nGameTimer == 0)
	{
		m_bBomm = false;
	}

	//横一列に壁を設置する処理
	if (m_bWall == true && m_nGameTimer % 50 == 0)
	{
		CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 0.0f, 0.0f), CObstacle::TYPE_01);
		CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, F_SCREEN_HEIGHT, 0.0f), CObstacle::TYPE_01);
	}
	m_nGameTimer++;
}

//=====================================
//ステージ切り替え時処理
//=====================================
void CStage::ArrayStage()
{
	switch (m_mode)
	{
	case Stage_1:
		if (m_nGameTimer == 10)
		{
			//背景
			CBG::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CBG::TYPE_Tutorial01);
		}

		//モード変更(緑)
		if (m_nGameTimer == 800)
		{
			//敵キャラ
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
			//障害物
			CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, (F_SCREEN_HEIGHT / 2 + 150.0f), 0.0f), CObstacle::TYPE_02);

			//障害物
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
		//ランダム
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
		//モード変更(赤)
		if (m_nGameTimer == 10)
		{
			//背景
			CBG::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CBG::TYPE_Tutorial02);
		}
		if (m_nGameTimer == 300)
		{
			CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 400.0f, 0.0f), CEnemy::TYPE_03);
		}
		if (m_nGameTimer == 450)
		{
			//敵キャラ
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
			//障害物
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
			//上下壁
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
		//モード変更(青)
		if (m_nGameTimer == 10)
		{
			//背景
			CBG::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CBG::TYPE_Tutorial03);
		}
		//ランダム
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
					//上向き逆L字
					for (int i = 0; i < 3; i++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f, F_SCREEN_HEIGHT - 100.0f * i, 0.0f), CObstacle::TYPE_01);
					}
					for (int j = 0; j < 2; j++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f * j, F_SCREEN_HEIGHT, 0.0f), CObstacle::TYPE_01);
					}
					//下側
					CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, F_SCREEN_HEIGHT - 100.0f, 0.0f), CEnemy::TYPE_04);
				}
				else if (nRand == 4)
				{
					//下向き逆L字
					for (int i = 0; i < 3; i++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f, 100.0f * i, 0.0f), CObstacle::TYPE_01);
					}
					for (int j = 0; j < 2; j++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * j, 0.0f, 0.0f), CObstacle::TYPE_01);
					}
					//上側
					CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 100.0f, 0.0f), CEnemy::TYPE_04);
				}
			}
		}
		if (m_nGameTimer == 300)
		{
			//上向き逆L字
			for (int i = 0; i < 3; i++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f, F_SCREEN_HEIGHT - 100.0f * i, 0.0f), CObstacle::TYPE_01);
			}
			for (int j = 0; j < 2; j++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f * j, F_SCREEN_HEIGHT, 0.0f), CObstacle::TYPE_01);
			}
			//下側
			CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, F_SCREEN_HEIGHT - 100.0f, 0.0f), CEnemy::TYPE_04);
		}
		if (m_nGameTimer == 600)
		{
			//下向き逆L字
			for (int i = 0; i < 3; i++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f, 100.0f * i, 0.0f), CObstacle::TYPE_01);
			}
			for (int j = 0; j < 2; j++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * j, 0.0f, 0.0f), CObstacle::TYPE_01);
			}
			//上側
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
		//モード変更(白)
		//ステージ1
		if (m_nGameTimer == 300)
		{
			//敵キャラ
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
			//障害物
			CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, (F_SCREEN_HEIGHT / 2 + 150.0f), 0.0f), CObstacle::TYPE_02);

			//障害物
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

		//ステージ2
		if (m_nGameTimer == 800)
		{
			//敵キャラ
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
			//障害物
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
			//上下壁
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

		//ステージ3
		if (m_nGameTimer == 1900)
		{
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), 200.0f, 0.0f), CEnemy::TYPE_01);
			}
			//上向き逆L字
			for (int i = 0; i < 3; i++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f, F_SCREEN_HEIGHT - 100.0f * i, 0.0f), CObstacle::TYPE_01);
			}
			for (int j = 0; j < 2; j++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f * j, F_SCREEN_HEIGHT, 0.0f), CObstacle::TYPE_01);
			}
			//下側
			CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, F_SCREEN_HEIGHT - 100.0f, 0.0f), CEnemy::TYPE_04);
		}
		if (m_nGameTimer == 2050)
		{
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), F_SCREEN_HEIGHT - 200.0f, 0.0f), CEnemy::TYPE_01);
			}
			//下向き逆L字
			for (int i = 0; i < 3; i++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f, 100.0f * i, 0.0f), CObstacle::TYPE_01);
			}
			for (int j = 0; j < 2; j++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * j, 0.0f, 0.0f), CObstacle::TYPE_01);
			}
			//上側
			CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 100.0f, 0.0f), CEnemy::TYPE_04);
		}
		if (m_nGameTimer == 2200)
		{
			for (int nCnt = 0; nCnt < 6; nCnt++)
			{
				CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + (50.0f * nCnt), (F_SCREEN_HEIGHT / 2), 0.0f), CEnemy::TYPE_02);
			}
			//上向き逆L字
			for (int i = 0; i < 3; i++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f, F_SCREEN_HEIGHT - 100.0f * i, 0.0f), CObstacle::TYPE_01);
			}
			for (int j = 0; j < 2; j++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f * j, F_SCREEN_HEIGHT, 0.0f), CObstacle::TYPE_01);
			}
			//下側
			CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, F_SCREEN_HEIGHT - 100.0f, 0.0f), CEnemy::TYPE_04);

			//下向き逆L字
			for (int i = 0; i < 3; i++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f, 100.0f * i, 0.0f), CObstacle::TYPE_01);
			}
			for (int j = 0; j < 2; j++)
			{
				CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * j, 0.0f, 0.0f), CObstacle::TYPE_01);
			}
			//上側
			CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 100.0f, 0.0f), CEnemy::TYPE_04);
		}

		//ランダム
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
					//上向き逆L字
					for (int i = 0; i < 3; i++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f, F_SCREEN_HEIGHT - 100.0f * i, 0.0f), CObstacle::TYPE_01);
					}
					for (int j = 0; j < 2; j++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f * j, F_SCREEN_HEIGHT, 0.0f), CObstacle::TYPE_01);
					}
					//下側
					CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, F_SCREEN_HEIGHT - 100.0f, 0.0f), CEnemy::TYPE_04);
				}
				else if (nRand == 4)
				{
					//下向き逆L字
					for (int i = 0; i < 3; i++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f, 100.0f * i, 0.0f), CObstacle::TYPE_01);
					}
					for (int j = 0; j < 2; j++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * j, 0.0f, 0.0f), CObstacle::TYPE_01);
					}
					//上側
					CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 100.0f, 0.0f), CEnemy::TYPE_04);
				}
				if (rand() % 4 > 0)
				{
					//障害物
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
		//ランダム
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
					//上向き逆L字
					for (int i = 0; i < 3; i++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f, F_SCREEN_HEIGHT - 100.0f * i, 0.0f), CObstacle::TYPE_01);
					}
					for (int j = 0; j < 2; j++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f * j, F_SCREEN_HEIGHT, 0.0f), CObstacle::TYPE_01);
					}
					//下側
					CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, F_SCREEN_HEIGHT - 100.0f, 0.0f), CEnemy::TYPE_04);
				}
				else if (nRand == 4)
				{
					//下向き逆L字
					for (int i = 0; i < 3; i++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN - 100.0f, 100.0f * i, 0.0f), CObstacle::TYPE_01);
					}
					for (int j = 0; j < 2; j++)
					{
						CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN + 100.0f * j, 0.0f, 0.0f), CObstacle::TYPE_01);
					}
					//上側
					CEnemy::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, 100.0f, 0.0f), CEnemy::TYPE_04);
				}

				if (rand() % 10 == 0)
				{
					//障害物
					CObstacle::Create(D3DXVECTOR3(RESPAWN_OFFSCREEN, (F_SCREEN_HEIGHT / 2) + (float)(rand() % 600) - 300, 0.0f), CObstacle::TYPE_02);
				}
			}
		}
		if (m_nGameTimer == STAGE_BOSS_WIDTH - STAGE_BOSS_RUSH)
		{
			CSound::PlaySound(SOUND_LABEL_BGM_EXPLOSION);
		}
		//モード変更(白)
		if (m_nGameTimer > STAGE_BOSS_WIDTH - STAGE_BOSS_RUSH && m_nGameTimer < STAGE_BOSS_WIDTH)
		{
			if (m_nGameTimer % 10 == 0)
			{
				float nRandWidth = (float)(rand() % SCREEN_WIDTH);
				float nRandHeight = (float)(rand() % SCREEN_HEIGHT);
				float nExplosionX = EXPLOSION_WIDTH + (float)(rand() % EXPLOSION_WIDTH);
				float nExplosionY = EXPLOSION_HEIGHT + (float)(rand() % EXPLOSION_HEIGHT);
				//爆発生成処理
				CExplosion::Create(D3DXVECTOR3(nRandWidth, nRandHeight, 0.0f), nExplosionX, nExplosionY);
			}
			BlinkStage();
		}
		if (m_nGameTimer == STAGE_BOSS_WIDTH)
		{
			m_bBomm = true;
			CBullet::Create(D3DXVECTOR3(F_SCREEN_WIDTH / 2, F_SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CBullet::PLAYER_BOMM);
			//爆発生成処理
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
//ステージ切り替え時処理
//=====================================
void CStage::ChangeStage()
{
	CScore *pScore = CScore::GetScore();
	switch (m_mode)
	{
	case Stage_1:
		//(緑)
		m_pBG->SetColor(D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f));
		CSound::StopSound(SOUND_LABEL_BGM_STAGE1);
		break;
	case Stage_2:
		//(赤)
		m_pBG->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f));
		CSound::StopSound(SOUND_LABEL_BGM_STAGE2);
		break;
	case Stage_3:
		//(青)
		m_pBG->SetColor(D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f));
		CSound::StopSound(SOUND_LABEL_BGM_STAGE3);
		break;
	case Stage_4:
		//(白)
		m_pBG->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));
		CSound::StopSound(SOUND_LABEL_BGM_STAGE4);
		break;
	case Stage_Boss:
		//(ボス用)
		m_pBG->SetColor(D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f), D3DXCOLOR(0.4f, 1.0f, 0.4f, 0.8f), D3DXCOLOR(0.4f, 0.4f, 1.0f, 0.8f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f));
		CSound::StopSound(SOUND_LABEL_BGM_BOSS);
		break;
	default:
		break;
	}

	m_mode = m_modeNext;	//次のモードに移動

	switch (m_mode)
	{
	case Stage_1:
		//(緑)
		m_pBG->SetColor(D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f));
		CSound::PlaySound(SOUND_LABEL_BGM_STAGE1);
		break;
	case Stage_2:
		//(赤)
		m_pBG->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f));
		CSound::PlaySound(SOUND_LABEL_BGM_STAGE2);
		pScore->AddScore(1000);
		break;
	case Stage_3:
		//(青)
		m_pBG->SetColor(D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f));
		CSound::PlaySound(SOUND_LABEL_BGM_STAGE3);
		pScore->AddScore(2000);
		break;
	case Stage_4:
		//(白)
		m_pBG->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));
		CSound::PlaySound(SOUND_LABEL_BGM_STAGE4);
		pScore->AddScore(3000);
		break;
	case Stage_Boss:
		//(ボス用)
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
//ステージ点滅処理
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
		//(緑)
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
		//(赤)
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
		//(青)
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
		//(白)
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
		//(白)
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