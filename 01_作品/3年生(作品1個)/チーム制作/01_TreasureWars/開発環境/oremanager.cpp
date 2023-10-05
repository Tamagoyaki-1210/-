//==============================================================================
//
// OreManager.cpp	(鉱石マネージャー.cpp)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// インクルード
//==============================================================================
#include <time.h>
#include "application.h"
#include "renderer.h"
#include "oremanager.h"
#include "objectX.h"
#include "input.h"
#include "ore.h"
#include"character.h"
#include"game.h"
#include"fever_time.h"

//==============================================================================
// 静的メンバ変数宣言
//==============================================================================
const float COreManager::Ore_Radius = 50.0f;		// 鉱石の当たり判定の大きさ(半径)
const int COreManager::REAPPEARANCE_MAX = 12;		// 鉱石の再出現
const int COreManager::CREATE_TIME = 500;			// 鉱石が生成されるまでの時間

//==============================================================================
// コンストラクタ
//==============================================================================
COreManager::COreManager()
{
	m_nStageNum = 0;
}

//==============================================================================
// デストラクタ
//==============================================================================
COreManager::~COreManager()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT COreManager::Init()
{
	//起動時に一回だけ行うため初期化に書く	
	srand((unsigned int)time(nullptr));

	//==================================================
	// メンバ変数の初期化
	//==================================================
	m_CreateTime = CREATE_TIME;

	
	InitCreate();
	
	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void COreManager::Uninit()
{
	// リストの削除
	m_pOreList.clear();

	Release();
}

//==============================================================================
// 更新処理
//==============================================================================
void COreManager::Update()
{
	// 入力デバイスの情報
	CInput* pInput = CInput::GetKey();

	m_CreateTime--;
	if (m_CreateTime <= 0)
	{
		RandCreate();
		m_CreateTime = CREATE_TIME;
	}

	// 鉱石のリストを整理する処理
	OreClear();
}

//==============================================================================
// 描画処理
//==============================================================================
void COreManager::Draw()
{

}

//==============================================================================
// 鉱石の当たり判定処理
//==============================================================================
COre* COreManager::OreCollision(D3DXVECTOR3 pos, float radius)
{
	COre* pNearOre = nullptr;	// 距離が一番近い鉱石のポインタ
	float fNearDistance = 0.0f;

	//イテレーターループ
	for (auto itr = m_pOreList.begin(); itr != m_pOreList.end();)
	{
		//イテレーターから鉱石のポインタの代入
		COre* pOre = *itr;

		if (pOre != nullptr)
		{
			// プレイヤーと鉱石が接触している場合 && 鉱石が出現しきっている場合
			if (CollisionCircle(pos, radius, pOre->GetPos(), Ore_Radius) && pOre->GetAppearance())
			{
				// 位置同士の距離の計算用変数
				D3DXVECTOR2 distance = { pOre->GetPos().x - pos.x, pOre->GetPos().z - pos.z };
				float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y));

				// 距離が小さい場合、又は最初の処理の場合確保する
				if (fNearDistance > fDistance || fNearDistance == 0)
				{
					fNearDistance = fDistance;
					pNearOre = pOre;
				}
			}
			itr++;
		}
		//未使用の鉱石ポインタの解放
		else
		{
			pOre->Uninit();
			pOre = nullptr;
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_pOreList.erase(itr);
		}
	}

	// 一番近い鉱石のポインタを返す
	return pNearOre;
}

//==============================================================================
// ランダムで生成する処理
//==============================================================================
void COreManager::RandCreate()
{
	
	for (int nCnt = 0; nCnt < REAPPEARANCE_MAX; nCnt++)
	{
		m_RandX = (float)(rand() % (1400) - 700) + 800;		//xの範囲にランダム
		m_RandY = (float)(rand() % 200) + 100;				//yの範囲にランダム
		m_RandZ = (float)(rand() % (1400) - 700) + 800;		//zの範囲にランダム
		COre *pOre = COre::Create(D3DXVECTOR3(m_RandX, -m_RandY, m_RandZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), m_nStageNum);

		//リストに追加
		m_pOreList.push_back(pOre);
	}
}

//==============================================================================
// 初期化時に生成する処理
//==============================================================================
void COreManager::InitCreate()
{
	for (int nCnt = 0; nCnt < RANDCREATE_MAX; nCnt++)
	{
		m_RandX = (float)(rand() % (1400) - 700) + 800;		//xの範囲にランダム
		m_RandY = (float)(rand() % 200) + 100;				//yの範囲にランダム
		m_RandZ = (float)(rand() % (1400) - 700) + 800;		//zの範囲にランダム
		COre *pOre = COre::Create(D3DXVECTOR3(m_RandX, -m_RandY, m_RandZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), m_nStageNum);

		//リストに追加
		m_pOreList.push_back(pOre);
	}
}

//==============================================================================
// 鉱石のリストを整理する処理
//==============================================================================
void COreManager::OreClear()
{
	// イテレーターループ
	for (auto itr = m_pOreList.begin(); itr != m_pOreList.end();)
	{
		//イテレーターから鉱石のポインタの代入
		COre *pOre = *itr;

		// 鉱石が終了した場合
		if (pOre->GetOreDeath() == true)
		{
			pOre->Uninit();
			pOre = nullptr;
			//次のイテレーターの代入、現在のイテレーターを破棄
			itr = m_pOreList.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

//==============================================================================
// 鉱石削除処理
//==============================================================================
void COreManager::OreDeath(COre* ore)
{
	// 選択した鉱石の終了処理
	ore->Uninit();

	// 選択した鉱石を配列から除外する
	m_pOreList.erase(std::find(m_pOreList.begin(), m_pOreList.end(), ore));
}

//==============================================================================
// 生成処理
//==============================================================================
COreManager *COreManager::Create(int nStageNum)
{
	COreManager *pOreManager = nullptr;

	pOreManager = new COreManager;

	if (pOreManager != nullptr)
	{
		pOreManager->SetStageNum(nStageNum);
		pOreManager->Init();
	}

	return pOreManager;
}
