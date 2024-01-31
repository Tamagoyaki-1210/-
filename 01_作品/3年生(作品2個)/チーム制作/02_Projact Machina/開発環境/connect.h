
//============================
//
// client設定
// Author:hamada ryuuga
//
//============================

#ifndef _CONNECT_H_
#define _CONNECT_H_

#include "model_data.h"
#include "client.h"

//構造体


class CDataPack;
class  ConnectManager
{
	struct SConnectCheck
	{
		int myConnect;
		bool enemyConnect;
	};
public:
	const int MAX_PL = 4;
public:
	ConnectManager();
	~ConnectManager();
	bool Init(const char*plPAddress, int nPortNum);
	void Uninit();
	void Update();
	static SConnectCheck ConnectTh(CClient * Client);

	static void RecvPlayerData(ConnectManager *list);
	void SendPlayerData(CModelData::SSendPack Data);

	int CharRecv(char*Recv, int Size, CClient::CONNECT_TYPE Tipe);
	void CharSend(const char*send, int Size, CClient::CONNECT_TYPE Tipe);


	bool GetIsConnect() { return m_myConnect.enemyConnect; }
	CClient *GetClient() { return m_client; }

	static bool GetTutorialConnecter() { return m_bTutorialConnecter; }
	static bool GetMainConnecter() { return m_bMainConnecter; }

	static void SetTutorialConnecter(bool bTutorialConnecter) { m_bTutorialConnecter = bTutorialConnecter; }
	static void SetMainConnecter(bool bMainConnecter) { m_bMainConnecter = bMainConnecter; }

private:
	static bool m_bTutorialConnecter;
	static bool m_bMainConnecter;
	bool m_bRecvOk;
	CClient *m_client;				//クライアント統合マネージャー
	SConnectCheck m_myConnect;		//自分に接続する
	CReceiveData::SReceiveList m_player;	//保存するプレイヤーデータの保存先
	


};

#endif