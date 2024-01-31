//============================
//
// ���C���ݒ�
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
//�R���X�g
//--------------------------
ConnectManager::ConnectManager()
{

}
//--------------------------
//�f�X�g���N�g
//--------------------------
ConnectManager::~ConnectManager()
{
}

//--------------------------
//������
//--------------------------
bool ConnectManager::Init(const char*plPAddress, int nPortNum)
{
	WSADATA wasData;

	int nErr = WSAStartup(WINSOCK_VERSION, &wasData);

	if (nErr != 0)
	{
		printf("was�@error");
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

	// �X���b�h������͂Ȃ�
	ConnectOn.detach();

	m_bRecvOk = false;
	return true;
}

//--------------------------
//�j��
//--------------------------
void ConnectManager::Uninit(void)
{

	if (m_client != nullptr)
	{
		m_client->Uninit();
		// �������̉��
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
		// �X���b�h������͂Ȃ�
		ConnectManagerRecv.detach();
	}
}


//=============================================================================
// �ʐM�ڑ��X���b�h
//=============================================================================
ConnectManager::SConnectCheck ConnectManager::ConnectTh(CClient * Client)
{
	m_bTutorialConnecter = true;
	SConnectCheck ConnectManager;
	ConnectManager.myConnect = false;
	ConnectManager.enemyConnect = false;
	char aRecvData[1024];	// ��M�f�[�^
	int Timer = 0;
	bool myIs = false;
	int ok = 4;
	// �Ȃ���܂Ń��[�v
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
	// �G���Ȃ���܂Ń��[�v
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
// ���V�[�u�X���b�h
//=============================================================================
void ConnectManager::RecvPlayerData(ConnectManager *list)
{
	// �q�����Ă���Ԃ̓��[�v
	while (1)
	{
		list->SetMainConnecter(true);
		if (list->GetIsConnect())
		{
			

			char aRecv[2048];	// ��M�f�[�^

			// ��M
			int nRecvSize = list->GetClient()->Recv(&aRecv[0], sizeof(CReceiveData::SReceiveList), CClient::TYPE_UDP);


			// ��M�f�[�^������������
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
// Send �X���b�g
//=============================================================================
void  ConnectManager::SendPlayerData(CModelData::SSendPack data)
{
	CModelData::SSendPack sendData = data;

	m_client->Send((const char*)&sendData, sizeof(CModelData::SSendPack), CClient::TYPE_UDP);
}

//=============================================================================
// Send �X���b�g
//=============================================================================
void  ConnectManager::CharSend(const char*send,int Size, CClient::CONNECT_TYPE Tipe)
{
	//CModelData::SSendPack sendData = data;

	m_client->Send(send, Size, Tipe);
}

//=============================================================================
// Recv �X���b�g
//=============================================================================
int ConnectManager::CharRecv(char*Recv, int Size, CClient::CONNECT_TYPE Tipe)
{
	//CModelData::SSendPack sendData = data;

	int RecvSize = m_client->Recv(Recv, Size, Tipe);
	return RecvSize;
}