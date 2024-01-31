//============================
//
// メイン設定
// Author:hamada ryuuga
//
//============================
#include "client.h"
#include "connect.h"
#include "player_manager.h"
#include "player.h"
#include "tcp_client.h"
#include "udp_client.h"
#include "application.h"
#include <thread>

bool ConnectManager::m_bTutorialConnecter = false;
bool ConnectManager::m_bMainConnecter = false;

//--------------------------
//コンスト
//--------------------------
ConnectManager::ConnectManager()
{

}
//--------------------------
//デストラクト
//--------------------------
ConnectManager::~ConnectManager()
{
}

//--------------------------
//初期化
//--------------------------
bool ConnectManager::Init(const char*plPAddress, int nPortNum)
{
	WSADATA wasData;

	int nErr = WSAStartup(WINSOCK_VERSION, &wasData);

	if (nErr != 0)
	{
		printf("was　error");
	}
	m_client = new CClient;

	if (!m_client->Init(plPAddress, nPortNum))
	{
		printf("error");
	}
	else
	{

	}


	std::thread ConnectOn([&] {m_myConnect= ConnectTh(m_client);});

	// スレッドをきりはなす
	ConnectOn.detach();

	m_bRecvOk = false;
	return true;
}

//--------------------------
//破棄
//--------------------------
void ConnectManager::Uninit(void)
{

	if (m_client != nullptr)
	{
		m_client->Uninit();
		// メモリの解放
		delete m_client;
		m_client = nullptr;
	}

	
	WSACleanup();
}

void ConnectManager::Update()
{
	if (!m_bRecvOk&&m_myConnect.enemyConnect)
	{

		m_bRecvOk = true;
		std::thread ConnectManagerRecv(RecvPlayerData, this);
		// スレッドをきりはなす
		ConnectManagerRecv.detach();
	}
}


//=============================================================================
// 通信接続スレッド
//=============================================================================
ConnectManager::SConnectCheck ConnectManager::ConnectTh(CClient * Client)
{
	m_bTutorialConnecter = true;
	SConnectCheck ConnectManager;
	ConnectManager.myConnect = false;
	ConnectManager.enemyConnect = false;
	char aRecvData[1024];	// 受信データ
	int Timer = 0;
	bool myIs = false;
	int ok = 4;
	// つながるまでループ
	while (!myIs)
	{

		m_bTutorialConnecter = true;
		myIs = Client->GetTcp()->Connect();
		Client->GetUdp()->Connect();
		Timer++;
		if (Timer >= 1)
		{
			m_bTutorialConnecter = false;
			break;
		}
		if (myIs)
		{
			Client->Send((const char*)&ok, 4, CClient::TYPE_TUP);
		}

		char sChar[64] = {};
		int nCharRecvSize = CApplication::GetClient()->CharRecv(sChar, sizeof(int), CClient::TYPE_TUP);

		memcpy(&nCharRecvSize, &sChar[0], (int)sizeof(int));
		m_bTutorialConnecter = false;
	}
	// 敵がつながるまでループ
	while (!ConnectManager.enemyConnect)
	{
		m_bTutorialConnecter = true;
		int isRecv = Client->Recv(&aRecvData[0], sizeof(bool), CClient::TYPE_TUP);
		if (isRecv == sizeof(SConnectCheck))
		{
			memcpy(&ConnectManager, &aRecvData[0], sizeof(SConnectCheck));
		}
		else if(isRecv == -1)
		{
			m_bTutorialConnecter = false;
			break;
		}
		m_bTutorialConnecter = false;
	}
	
	m_bTutorialConnecter = false;
	return ConnectManager;
}

//=============================================================================
// レシーブスレッド
//=============================================================================
void ConnectManager::RecvPlayerData(ConnectManager *list)
{
	// 繋がっている間はループ
	while (1)
	{
		list->SetMainConnecter(true);
		if (list->GetIsConnect())
		{
			

			char aRecv[2048];	// 受信データ

			// 受信
			int nRecvSize = list->GetClient()->Recv(&aRecv[0], sizeof(CReceiveData::SReceiveList), CClient::TYPE_UDP);


			// 受信データが無かったら
			if (nRecvSize < 0)
			{
				return;
			}
			CReceiveData::SReceiveList List;
			memcpy(&list->m_player, &aRecv[0], (int)sizeof(CReceiveData::SReceiveList));
		
		}
		list->SetMainConnecter(false);
	}
}

//=============================================================================
// Send スレット
//=============================================================================
void  ConnectManager::SendPlayerData(CModelData::SSendPack data)
{
	CModelData::SSendPack sendData = data;

	m_client->Send((const char*)&sendData, sizeof(CModelData::SSendPack), CClient::TYPE_UDP);
}

//=============================================================================
// Send スレット
//=============================================================================
void  ConnectManager::CharSend(const char*send,int Size, CClient::CONNECT_TYPE Tipe)
{
	//CModelData::SSendPack sendData = data;

	m_client->Send(send, Size, Tipe);
}

//=============================================================================
// Recv スレット
//=============================================================================
int ConnectManager::CharRecv(char*Recv, int Size, CClient::CONNECT_TYPE Tipe)
{
	//CModelData::SSendPack sendData = data;

	int RecvSize = m_client->Recv(Recv, Size, Tipe);
	return RecvSize;
}