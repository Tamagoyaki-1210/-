//==============================================================================================
//
// トロッコマネージャー　　　trolley_manager.cpp
// tutida ryousei
//
//==============================================================================================
#include"gimmick_model.h"
#include"trolley_manager.h"
#include"debugProc.h"
#include"file.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CTrolley_Manager::CTrolley_Manager()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CTrolley_Manager::~CTrolley_Manager()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CTrolley_Manager::Init()
{
	// トロッコの設定
	SetTrolley();

	// 初期位置の設定
	SetStartPos();

	// 生成
	m_pGimmick_Model = CGimmick_Model::Create(m_Pos, m_nIndex, m_FileName);

	m_nCoolTime_Count = m_nCoolTime;

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CTrolley_Manager::Uninit()
{
	Release();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CTrolley_Manager::Update()
{
	// トロッコを動かしていない場合
	if (!m_bMove)
	{
		// クールタイムに達した場合
		if (m_nCoolTime_Count <= 0)
		{
			// 初期位置の設定
			SetStartPos();

			m_nCoolTime_Count = m_nCoolTime;
		}
		// クールタイムをカウントする
		m_nCoolTime_Count--;
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CTrolley_Manager::Draw()
{
}

//==============================================================================================
// 生成処理
//==============================================================================================
CTrolley_Manager *CTrolley_Manager::Create(int type, int index, const char *Xfilename)
{
	CTrolley_Manager *pTrolley = nullptr;

	pTrolley = new CTrolley_Manager;

	if (pTrolley != nullptr)
	{
		pTrolley->SetType(type);
		pTrolley->SetIndex(index);
		pTrolley->SetFileName(Xfilename);
		pTrolley->Init();
	}

	return pTrolley;
}

//==============================================================================================
// トロッコの削除
//==============================================================================================
void CTrolley_Manager::TrolleyDeath()
{
	// トロッコを止める
	m_bMove = false;

	// トロッコが使用中の場合
	if (m_pGimmick_Model != nullptr)
	{
		m_pGimmick_Model->Uninit();
		m_pGimmick_Model = nullptr;
	}
}

//==============================================================================================
// トロッコの設定
//==============================================================================================
void CTrolley_Manager::SetTrolley()
{
	// ファイルポインタ
	FILE *pFile = nullptr;

	// ファイル名
	pFile = fopen("Data/text/trolley.txt", "r");

	m_nStartNum = 0;		// 初期位置の数
	m_nTurnNum = 0;			// 巡回地点の数
	m_nFirstNum = 0;		// 最初の目的地
	int nSpeedNum = 0;		// スピードのレベル

	int nType = 0;			// タイプ

	char m_aString[0xff];
	memset(m_aString, 0, sizeof(m_aString));

	// ファイルが開いた場合
	if (pFile != NULL)
	{
		while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", &m_aString[0]);

			// SCRIPTが一致
			if (strcmp(&m_aString[0], "SCRIPT") == 0)
			{
				while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &m_aString[0]);

					// タイプ
					if (strcmp(&m_aString[0], "TYPE") == 0)
					{
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%d", &nType);
					}

					if (nType == m_nType)
					{
						// 初期位置
						if (strcmp(&m_aString[0], "START_POS") == 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);
							fscanf(pFile, "%f", &m_Start_Pos[m_nStartNum].x);
							fscanf(pFile, "%f", &m_Start_Pos[m_nStartNum].y);
							fscanf(pFile, "%f", &m_Start_Pos[m_nStartNum].z);

							m_nStartNum++;
						}

						// 巡回地点
						if (strcmp(&m_aString[0], "TURN_POS") == 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);
							fscanf(pFile, "%f", &m_Turn_Pos[m_nTurnNum].x);
							fscanf(pFile, "%f", &m_Turn_Pos[m_nTurnNum].y);
							fscanf(pFile, "%f", &m_Turn_Pos[m_nTurnNum].z);

							m_nTurnNum++;
						}

						// 最初の目的地
						if (strcmp(&m_aString[0], "FIRST_INDEX") == 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);
							fscanf(pFile, "%d", &m_nFirst_Index[m_nFirstNum][0]);
							fscanf(pFile, "%d", &m_nFirst_Index[m_nFirstNum][1]);

							m_nFirstNum++;
						}

						// クールタイム
						if (strcmp(&m_aString[0], "COOL_TIME") == 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);
							fscanf(pFile, "%d", &m_nCoolTime);
						}

						// 速度
						if (strcmp(&m_aString[0], "SPEED") == 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);
							fscanf(pFile, "%f", &m_fSpeed[nSpeedNum]);
							nSpeedNum++;
						}

						// 当たり判定の半径
						if (strcmp(&m_aString[0], "RADIUS") == 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);
							fscanf(pFile, "%f", &m_fRadius);
						}
					}
				}
			}
		}
	}
}

//==============================================================================================
// 初期位置の設定
//==============================================================================================
void CTrolley_Manager::SetStartPos()
{
	m_Pos = { 0.0f,0.0f,0.0 };		// 生成する位置
	m_nRand_StartPos = 0;		// 初期位置

	// 初期位置をランダムで決める
	m_nRand_StartPos = rand() % m_nStartNum;
	m_Pos = m_Start_Pos[m_nRand_StartPos];

	for (int nCnt = 0; nCnt < m_nFirstNum; nCnt++)
	{
		if (m_nRand_StartPos == m_nFirst_Index[nCnt][0])
			m_nStartTurn_Index = m_nFirst_Index[nCnt][1];
	}

	// トロッコがすでに存在する場合
	if (m_pGimmick_Model != nullptr)
	{
		// トロッコの位置を設定する
		m_pGimmick_Model->SetPos(m_Pos);
		m_pGimmick_Model->SetTrolleyInfomation();
	}

	// トロッコを動かす
	m_bMove = true;
}