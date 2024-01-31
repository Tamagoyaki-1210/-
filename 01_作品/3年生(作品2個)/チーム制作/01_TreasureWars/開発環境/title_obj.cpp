//==============================================================================
//
// title_obj.cpp	(タイトルのオブジェクト.cpp)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// インクルード
//==============================================================================
#include <time.h>
#include "application.h"
#include "renderer.h"
#include "title_obj.h"
#include "objectX.h"
#include "ore.h"
#include "particle_emitter.h"

//==============================================================================
// 静的メンバ変数宣言
//==============================================================================
const float CTitleObj::MOVING_ORESPEED = 10.0f;
const float CTitleObj::MOVING_PILEOFBILLSSPEED = 2.0f;
const int CTitleObj::CREATE_TIME = 100;

//==============================================================================
// コンストラクタ
//==============================================================================
CTitleObj::CTitleObj()
{

}


//==============================================================================
// デストラクタ
//==============================================================================
CTitleObj::~CTitleObj()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CTitleObj::Init()
{
	//起動時に一回だけ行うため初期化に書く	
	srand((unsigned int)time(nullptr));

	//objectXの初期化処理
	CObjectX::Init();

	//==================================================
	// メンバ変数の初期化
	//==================================================
	m_PileofbillsMove = D3DXVECTOR3(0.0f, MOVING_PILEOFBILLSSPEED, 0.0f);
	m_OreMove = D3DXVECTOR3(0.0f, MOVING_ORESPEED, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PileofbillsTime = CREATE_TIME;
	m_RandRotX = 0.0f;
	m_RandX = 0.0f;
	m_RandY = 0.0f;

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CTitleObj::Uninit()
{
	//objectXの終了処理
	CObjectX::Uninit();
}

//==============================================================================
// 更新処理
//==============================================================================
void CTitleObj::Update()
{
	D3DXVECTOR3 pos = GetPos();
	if(m_PileofbillsTime > 0) m_PileofbillsTime--;

	if (m_ObjType == ObjType_Ore)
	{
		if (pos.y <= -500.0f)
		{
			m_RandX = (float)(rand() % 1500 - 750);		// xの範囲にランダム
			m_RandY = (float)(rand() % 1000 + 500);		// yの範囲にランダム
			m_RandRotX = (float)(rand() % 30 - 15);

			pos = D3DXVECTOR3(m_RandX, m_RandY, 0.0f);
		}

		m_rot.x += m_RandRotX / 100;

		pos -= m_OreMove;

		SetRot(m_rot);
	}

	if (m_ObjType == ObjType_Pileofbills && m_PileofbillsTime <= 0)
	{
		D3DXVECTOR3 rot = GetRot();
		rot.y += D3DXToRadian(0.2f);
		if (-100.0f >= pos.y)
		{
			pos += m_PileofbillsMove;

		}
		SetRot(rot);

		static int emitTime = 0;
		emitTime++;
		if (emitTime > 0 && (emitTime % 250) == 0)
		{
			CParticleEmitter::Create("Coin", D3DXVECTOR3(pos.x, -300.0f, pos.z + 750.0f));
		}
	}

	//設定
	SetPos(pos);
	

	//objectXの更新処理
	CObjectX::Update();
}

//==============================================================================
// 生成処理
//==============================================================================
CTitleObj *CTitleObj::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, char *Xfilename, ObjType type)
{
	CTitleObj *pTitleObj = nullptr;

	pTitleObj = new CTitleObj;

	if (pTitleObj != nullptr)
	{
		pTitleObj->SetPos(pos); 
		pTitleObj->SetRot(rot);
		pTitleObj->SetSize(size);
		pTitleObj->SetIndex(pTitleObj->ReadObject(Xfilename));
		pTitleObj->SetObjType(type);
		pTitleObj->Init();
	}

	return pTitleObj;
}
