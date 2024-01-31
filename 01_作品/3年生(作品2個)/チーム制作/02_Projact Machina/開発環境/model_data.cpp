//====================================================
//
// モデルData
// Author : 浜田
//
//====================================================
//-------------------------------
// インクルード
//-------------------------------
#include "model_data.h"

//-------------------------------
// デフォルトコンストラクタ
//-------------------------------
CModelData::CModelData()
{
	IsUse = false;
	m_CommuData.Player.m_log = 0;
	m_CommuData.Player.m_PlayData.m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_CommuData.Player.m_PlayData.m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_CommuData.Player.m_PlayData.m_motion = 0;
	m_CommuData.Player.m_PlayData.m_haveItemRightId = false;
	m_CommuData.Player.m_PlayData.m_haveItemLeftId = false;
	m_CommuData.Player.m_popEnemy = 0;
	m_CommuData.Player.m_popGimmick = 0;
	m_CommuData.Player.m_PlayData.m_pushBomComands = false;
	m_CommuData.Player.m_IsGame = false;
	m_CommuData.Player.m_MyId = P1;
	for (int i = 0; i < MaxModel; i++)
	{
		m_CommuData.Player.m_isPopEnemy[i].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_CommuData.Player.m_isPopEnemy[i].isDiscovery = false;
		m_CommuData.Player.m_isPopEnemy[i].isUse = true;
		m_CommuData.Player.m_isPopGimmick[i].isUse = false;
		m_CommuData.Player.m_isPopGimmick[i].isMap = false;
	}
}

//-------------------------------
// デフォルトデストラクタ
//-------------------------------
CModelData::~CModelData()
{

}

void CModelData::SPlayerData::SetPlayerClear()
{
	Player.m_log = 0;
	Player.m_PlayData.m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Player.m_PlayData.m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Player.m_PlayData.m_motion = 0;
	Player.m_PlayData.m_haveItemRightId = 0;
	Player.m_PlayData.m_haveItemLeftId = 0;
	Player.m_popEnemy = 0;
	Player.m_popGimmick = 0;
	Player.m_addscore = 0;
	Player.m_PlayData.m_pushBomComands = false;
	Player.m_IsGame = false;
	Player.m_MyId = 0;
	for (int i = 0; i < MaxModel; i++)
	{
		Player.m_isPopEnemy[i].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Player.m_isPopEnemy[i].isDiscovery = false;
		Player.m_isPopEnemy[i].isUse = true;
		Player.m_isPopGimmick[i].isUse = false;
		Player.m_isPopGimmick[i].isMap = false;
	}
}

void CReceiveData::SReceiveList::Init()
{

	for (int i = 0; i < CModelData::MAX_P; i++)
	{
		m_PlayrData[i].m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_PlayrData[i].m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_PlayrData[i].m_haveItemLeftId = 0;
		m_PlayrData[i].m_motion = 0;
		m_PlayrData[i].m_haveItemRightId = 0;
		m_PlayrData[i].m_pushBomComands = false;
	}
	for (int i = 0; i < MaxModel; i++)
	{
		m_isPopGimmick[i].isUse = false;
		m_isPopGimmick[i].isMap = false;
		m_isPopEnemy[i].isUse = false;
		m_isPopEnemy[i].isMap = false;
		m_isPopEnemy[i].isDiscovery = false;
		m_isPopEnemy[i].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}




}

void CReceiveData::SReceiveList::SetListPlayr(CModelData::SPlayerList player, int playerId)
{
	m_PlayrData[playerId] = player;
}

void CReceiveData::SReceiveList::SetListEnemy(CModelData::SEnemyData* Enemy)
{
	for (int i = 0; i < MaxModel; i++)
	{
		if (Enemy[i].isUse)
		{
			m_isPopEnemy[i].isUse = Enemy[i].isUse;
		}
		if (Enemy[i].isMap)
		{
			m_isPopEnemy[i].isMap = Enemy[i].isMap;
		}
		if (Enemy[i].isDiscovery)
		{
			m_isPopEnemy[i].isDiscovery = Enemy[i].isDiscovery;
		}
		m_isPopEnemy[i].Pos = Enemy[i].Pos;
	}

}

void CReceiveData::SReceiveList::SetListGimmick(CModelData::SGimmickData* Gimmick)
{
	for (int i = 0; i < MaxModel; i++)
	{
		if (Gimmick[i].isUse)
		{
			m_isPopGimmick[i].isUse = Gimmick[i].isUse;
		}
		if (Gimmick[i].isMap)
		{
			m_isPopGimmick[i].isMap = Gimmick[i].isMap;
		}
	}
}
