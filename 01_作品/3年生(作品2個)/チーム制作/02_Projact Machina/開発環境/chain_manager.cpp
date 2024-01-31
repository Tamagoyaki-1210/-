//==============================================================================================
//
// 鎖マネージャー	　　chain_manager.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"chain_manager.h"
#include"chain.h"
#include"math.h"
#include"objectX.h"
#include "particle_emitter.h"
#include"enemy.h"
#include"enemy_manager.h"
#include"game.h"
#include"cannon.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CChain_Manager::CChain_Manager()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CChain_Manager::~CChain_Manager()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CChain_Manager::Init()
{
	// 鎖の本数
	m_nNumChain = m_fDistance / CHAIN_SIZE_X;
	// 四捨五入
	m_nNumChain = round(m_nNumChain);

	for (int nCnt = 0; nCnt < m_nNumChain; nCnt++)
	{
		D3DXVECTOR3 Pos = m_Pos + (float)nCnt * m_Vec;

		m_pChain[0].emplace_back();
		m_pChain[1].emplace_back();

		// 鎖の生成
		m_pChain[0][nCnt] = CChain::Create(Pos, m_Vec, 0);
		m_pChain[1][nCnt] = CChain::Create(Pos, m_Vec, 1);

		if (nCnt == m_nNumChain - 1)
			// アンカーの生成
			m_pAnchor = CObjectX::Create({ 0.0f,0.0f,0.0f }, { 0.0f,m_fRot,0.0f }, nullptr, "Data/model/Anchor.x");
	}

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CChain_Manager::Uninit()
{
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CChain_Manager::Update()
{
	// 鎖の長さ
	if (CHAIN_SIZE_X > m_fChain_Size)
		m_fChain_Size += 50;
	else if (m_fChain_Size > CHAIN_SIZE_X)
		m_fChain_Size = CHAIN_SIZE_X;

	// 正規化
	D3DXVec3Normalize(&m_Vec, &m_Vec);
	m_Vec *= m_fChain_Size;

	for (int nCnt = 0; nCnt < m_nNumChain; nCnt++)
	{
		if (m_pChain[0][nCnt] != nullptr || m_pChain[1][nCnt] != nullptr)
		{
			// 鎖を伸ばす長さ
			m_pChain[0][nCnt]->SetVec(m_Vec);
			m_pChain[1][nCnt]->SetVec(m_Vec);

			// 次の鎖を付ける位置
			m_pChain[0][nCnt]->SetPos(m_Pos + (float)nCnt * m_Vec);
			m_pChain[1][nCnt]->SetPos(m_Pos + (float)nCnt * m_Vec);
		}
	}

	// アンカーの位置
	D3DXVECTOR3 Anchor_Pos = m_Pos + m_nNumChain * m_Vec;

	// 鎖の先に付けるアンカーの移動
	m_pAnchor->SetPos({ Anchor_Pos.x,Anchor_Pos.y - CHAIN_SIZE_Y / 4,Anchor_Pos.z });

	m_nRestraint_Count++;
	if (m_nRestraint_Count == m_nRestraint_Break)
	{
		for (int nCnt = 0; nCnt < m_nNumChain; nCnt++)
		{
			// 数秒後に消える
			m_pChain[0][nCnt]->Uninit();
			m_pChain[1][nCnt]->Uninit();

			m_pChain[0][nCnt] = nullptr;
			m_pChain[1][nCnt] = nullptr;

			m_pAnchor->Uninit();

			// 拘束を解く
			CCannon::SetRestrain(false);
		}
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CChain_Manager::Draw()
{

}

//==============================================================================================
// 生成処理
//==============================================================================================
CChain_Manager *CChain_Manager::Create(D3DXVECTOR3 pos, D3DXVECTOR3 vec, float Distance, float rot, int breakcount)
{
	CChain_Manager *pChain_Manager = new CChain_Manager;

	if (pChain_Manager != nullptr)
	{
		pChain_Manager->m_Pos = { pos.x,pos.y + 50.0f,pos.z };
		//pChain_Manager->m_Vec = { vec.x,0.0f,vec.z };
		pChain_Manager->m_Vec = vec;
		pChain_Manager->m_fDistance = Distance;
		pChain_Manager->m_fRot = rot;
		pChain_Manager->m_nRestraint_Break = breakcount;
		pChain_Manager->Init();
	}

	return pChain_Manager;
}