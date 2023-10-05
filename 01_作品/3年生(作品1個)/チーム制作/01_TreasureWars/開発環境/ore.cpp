//==============================================================================
//
// Ore.cpp	(鉱石.cpp)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// インクルード
//==============================================================================
#include "application.h"
#include "renderer.h"
#include "ore.h"
#include "scoreitem.h"
#include "particle_emitter.h"
#include <assert.h>

//==============================================================================
// 静的メンバ変数宣言
//==============================================================================
const float COre::MOVING_DISTANCE = 2.5f;
const int COre::UNINIT_TIME = 500;
const int COre::Ore_Blink_Speed = 10;		// 鉱石の点滅する速度
const float COre::Ore_Blink_Timer = 180.0f;	// 鉱石が点滅し始める時間

const char* COre::STAGE_MODEL_01[] =
{
	"Data\\model\\Collecting_Point\\Stage_01\\ore_01.x",
	"Data\\model\\Collecting_Point\\Stage_01\\ore_02.x"
};
const char* COre::STAGE_MODEL_02[] =
{
	"Data\\model\\Collecting_Point\\Stage_02\\Fossil_01.x",
	"Data\\model\\Collecting_Point\\Stage_02\\Fossil_02.x"
};
const char* COre::STAGE_MODEL_03[] =
{
	"Data\\model\\Collecting_Point\\Stage_03\\Artificial_Satellite_01.x",
	"Data\\model\\Collecting_Point\\Stage_03\\Artificial_Satellite_02.x"
};

//==============================================================================
// コンストラクタ
//==============================================================================
COre::COre()
{
	m_nLife = Normal_Ore_Life;
	m_UninitTime = UNINIT_TIME;
	m_bOnce = true;
	m_bAppearance = false;
	m_bOreDeath = false;
}

//==============================================================================
// デストラクタ
//==============================================================================
COre::~COre()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT COre::Init()
{
	//objectXの初期化処理
	CObjectX::Init();

	//==================================================
	// メンバ変数の初期化
	//==================================================
	m_move = D3DXVECTOR3(0.0f, MOVING_DISTANCE, 0.0f);

	
	// 鉱石(大)から使う
	SetIndex(m_nBigOreModelIndex);

	return S_OK;
}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT COre::Init(int nStageNum)
{
	// 鉱石モデルの番号の設定
	switch (nStageNum)
	{
	case 0:
		m_nOreModelIndex = ReadObject(STAGE_MODEL_01[0]);
		m_nBigOreModelIndex = ReadObject(STAGE_MODEL_01[1]);
		break;
	case 1:
		m_nOreModelIndex = ReadObject(STAGE_MODEL_02[0]);
		m_nBigOreModelIndex = ReadObject(STAGE_MODEL_02[1]);
		break;
	case 2:
		m_nOreModelIndex = ReadObject(STAGE_MODEL_03[0]);
		m_nBigOreModelIndex = ReadObject(STAGE_MODEL_03[1]);
		break;
	default:
		assert(false);
		break;
	}

	//初期化
	Init();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void COre::Uninit()
{
	//objectXの終了処理
	CObjectX::Uninit();
}

//==============================================================================
// 更新処理
//==============================================================================
void COre::Update()
{
	//出現が完了したかどうか
	if (m_bAppearance)
	{
		if (m_bOnce)
		{
			// 鉱石出現時パーティクル
			CParticleEmitter::Create("PopOre", GetPos());
			m_bOnce = false;
		}
	}

	// 現在値
	D3DXVECTOR3 pos = GetPos();

	//地面に出たら
	if (pos.y >= 0.0f)
	{
		//出現が完了した
		m_bAppearance = true;
		m_UninitTime--;
	}
	//出現が完了していない
	if (m_bAppearance == false)
	{
		pos += m_move;
	}
	
	// 点滅開始時間に達した場合
	if (m_UninitTime < Ore_Blink_Timer)
	{
		// 鉱石の点滅処理
		AddAlpha(m_UninitTime % (Ore_Blink_Speed * 2) < Ore_Blink_Speed ? 1.0f / Ore_Blink_Speed : -1.0f / Ore_Blink_Speed);
	}

	//時間が0になったら
	if (m_UninitTime <= 0)
	{
		// 鉱石を終了させるフラグ
		m_bOreDeath = true;
	}

	//設定処理
	SetPos(pos);

	//objectXの更新処理
	CObjectX::Update();
}

//==============================================================================================
// ダメージ処理
//==============================================================================================
int COre::Damage(int damage)
{
	// 鉱石を小さくする
	SetIndex(m_nOreModelIndex);

	// 消えるまでの時間を初期化
	m_UninitTime = UNINIT_TIME;

	return m_nLife -= damage;
}

//==============================================================================================
// 生成処理
//==============================================================================================
COre *COre::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, int nStageNum)
{
	COre *pOre = new COre;

	if (pOre != nullptr)
	{
		pOre->SetPos(pos);
		pOre->SetRot(rot);
		pOre->SetSize(size);
		pOre->Init(nStageNum);
	}

	return pOre;
}