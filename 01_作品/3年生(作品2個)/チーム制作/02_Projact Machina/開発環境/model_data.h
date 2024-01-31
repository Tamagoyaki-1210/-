//============================
//
// 座標があるモデル
// Author:hamada ryuuga
//
//============================

#ifndef _MODEL_DATA_H_
#define _MODEL_DATA_H_


static const int Max = 1024;
static const int MaxModel = 20;

static const int P1 = 1345;
static const int P2 = 2333;
static const int P3 = 3204;
static const int P4 = 4650;


class CModelData
{

public:
	struct item_data
	{
		int itemData[5];
	};

	struct abnormal_data
	{
		int abnormalData[5];
	};

	//にんずう
	static const int MAX_P = 4;

	//ーーーーーーー
	//プレイヤー
	//ーーーーーーー
	struct SPlayerList
	{
		D3DXVECTOR3 m_pos;
		D3DXVECTOR3 m_rot;
		int m_motion;
		int m_haveItemRightId;
		int m_haveItemLeftId;
		bool m_pushBomComands;
	};

	//ーーーーーーー
	//エネミー
	//ーーーーーーー
	struct SEnemyData
	{
		bool isUse;
		bool isDiscovery;
		bool isMap;
		D3DXVECTOR3 Pos;
	};

	//ーーーーーーー
	//ギミック
	//ーーーーーーー
	struct SGimmickData
	{
		bool isUse;
		bool isMap;
	};
	struct SSendData
	{
		enum EDATA
		{
			ESENDDATA_ENEMY,
		};


		SSendData(int id) :mId(id) {}
		int mId;
	};
	struct SSendPack : public SSendData
	{
	public:
		SPlayerList m_PlayData;
		int m_popEnemy;
		int m_popGimmick;
		SEnemyData m_isPopEnemy[MaxModel];
		SGimmickData m_isPopGimmick[MaxModel];
		abnormal_data m_haveAbnormal;
		item_data m_haveItem;
		int m_addscore;
		int m_score;
		bool m_IsGame;
		int m_MyId;
		int m_log;
		SSendPack() : SSendData(ESENDDATA_ENEMY)
		{
			for (int i = 0; i < MaxModel; i++)
			{
				m_isPopEnemy[i].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_isPopEnemy[i].isDiscovery = false;
				m_isPopEnemy[i].isUse = false;
				m_isPopGimmick[i].isMap = false;
				m_isPopGimmick[i].isUse = false;
			}
		}
		void SetList(std::string modelList)
		{

			if (modelList.size() != Max)
			{
				return;
			}
			m_PlayData.m_pos = D3DXVECTOR3(modelList[0], modelList[1], modelList[2]);
			m_PlayData.m_rot = D3DXVECTOR3(modelList[3], modelList[4], modelList[5]);
			m_PlayData.m_motion = modelList[6];
		}

	};
	struct SPlayerData
	{
		SSendPack Player;
		void SetPlayerClear();
		void SetPlayer(SSendPack data)
		{
			Player = data;
		}
		void SetPlayerCast(const char* data)
		{
			Player = *((SSendPack*)data);
		}
	};
public:

	CModelData();
	~CModelData();

	SPlayerData* GetCommu() { return &m_CommuData; };
	void SetCommu(SPlayerData Set) { m_CommuData = Set; };
	bool* GetUse() { return &IsUse; };
	void SetUse(bool Set) { IsUse = Set; };
	bool IsUse;
	SPlayerData m_CommuData;
};

struct SSendMap
{
	int sendMap;
	bool isMap;
};

//ーーーーーーー
//送り返すデータ
//ーーーーーーー
class CReceiveData
{
public:
	struct SReceiveList
	{
		CModelData::SPlayerList m_PlayrData[CModelData::MAX_P];
		CModelData::SEnemyData m_isPopEnemy[MaxModel];
		CModelData::SGimmickData m_isPopGimmick[MaxModel];
		int m_score;
		int m_log;
		bool m_IsGame;
		void Init();
		void SetListPlayr(CModelData::SPlayerList player, int playerId);
		void SetListEnemy(CModelData::SEnemyData* Enemy);
		void SetListGimmick(CModelData::SGimmickData* Gimmick);
	};

};
#endif