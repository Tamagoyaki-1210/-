//==============================================================================
//
// result_obj.cpp	(リザルトのオブジェクト.cpp)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// インクルード
//==============================================================================
#include <time.h>
#include "application.h"
#include "renderer.h"
#include "result_obj.h"
#include "objectX.h"
#include "ore.h"
#include "particle_emitter.h"
#include "charmanager.h"

//==============================================================================
// 静的メンバ変数宣言
//==============================================================================
const float CResultObj::Moving_OreSpeed = 10.0f;

//==============================================================================
// コンストラクタ
//==============================================================================
CResultObj::CResultObj()
{

}


//==============================================================================
// デストラクタ
//==============================================================================
CResultObj::~CResultObj()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CResultObj::Init()
{
	//起動時に一回だけ行うため初期化に書く	
	srand((unsigned int)time(nullptr));

	//objectXの初期化処理
	CObjectX::Init();

	//==================================================
	// メンバ変数の初期化
	//==================================================
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_OreMove = D3DXVECTOR3(0.0f, Moving_OreSpeed, 0.0f);
	m_LogoMove = 3.0f;

	return S_OK;
}

//==============================================================================
// 更新処理
//==============================================================================
void CResultObj::Update()
{
	//鉱石の挙動
	ObjectBehavior();

	//objectXの更新処理
	CObjectX::Update();
}

//==============================================================================
// オブジェクトの挙動処理
//==============================================================================
void CResultObj::ObjectBehavior()
{
	//位置の取得
	D3DXVECTOR3 pos = GetPos();

	if (pos.y <= -500.0f)
	{
		m_RandX = (float)(rand() % 1500 - 750);		//xの範囲にランダム
		m_RandY = (float)(rand() % 1000 + 500);		//yの範囲にランダム
		m_RandRotX = (float)(rand() % 30 - 15);		//回転ランダム

		pos = D3DXVECTOR3(m_RandX, m_RandY, 0.0f);
	}
	if (pos.y == -50.0f)
	{
		CParticleEmitter::Create("Coin", pos);
	}

	//回転
	m_rot.x += m_RandRotX / 100;

	//移動量
	pos -= m_OreMove;

	//設定
	SetRot(m_rot);
	SetPos(pos);
}

//==============================================================================
// 生成処理
//==============================================================================
CResultObj *CResultObj::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, int index)
{
	CResultObj *pResultObj = nullptr;

	pResultObj = new CResultObj;

	if (pResultObj != nullptr)
	{
		pResultObj->SetPos(pos);
		pResultObj->SetRot(rot);
		pResultObj->SetSize(size);
		pResultObj->SetIndex(index);
		pResultObj->Init();
	}

	return pResultObj;
}
