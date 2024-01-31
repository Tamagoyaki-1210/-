//============================
//
// メイン設定
// Author:hamada ryuuga
//
//============================
#include "tcp_client.h"
#include "udp_client.h"

//--------------------------
//コンスト
//--------------------------
CTcp_Client::CTcp_Client()
{
	m_sock = INVALID_SOCKET;
}
//--------------------------
//デストラクト
//--------------------------
CTcp_Client::~CTcp_Client()
{
}

//--------------------------
//初期化
//--------------------------
bool CTcp_Client::Init(const char*plPAddress, int nPortNum)
{

	m_nPortNum = nPortNum;

	m_Ip = plPAddress;

	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		printf("socket　error");
		return false;
	}

	m_sockOk = true;
	return true;
}

//--------------------------
//オーバーロード初期化
//--------------------------
bool CTcp_Client::Init(SOCKET sock)
{
	m_sock = sock;
	return true;
}

//--------------------------
//接続
//--------------------------
int CTcp_Client::Send(const char*pSendData, int nSendDataSize)
{
	if (m_sock == INVALID_SOCKET)
	{
		return 0;
	}
	send(m_sock, &pSendData[0], nSendDataSize, 0);
	return strlen(&pSendData[0]) + 1;
}

//--------------------------
//返信
//--------------------------
int CTcp_Client::Recv(char*pRecvData, int nRecvDataSize)
{
	if (m_sock == INVALID_SOCKET)
	{
		return 0;
	}

	int nRecvByte = recv(m_sock, &pRecvData[0], nRecvDataSize, 0);

	if (nRecvByte <= 0)
	{
		Uninit();
	}
	
	return nRecvByte;
}

//--------------------------
//破棄
//--------------------------
void CTcp_Client::Uninit()
{
	m_sockOk = false;
	closesocket(m_sock);
}


//-------------------------------
// 通信
//-------------------------------
bool CTcp_Client::Connect()
{
	if (!m_sockOk)
	{
		return false;
	}

	//------------------------
	// 接続設定
	//------------------------
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;	// どの通信か
	addr.sin_port = htons(m_nPortNum);	// ポート番号
	addr.sin_addr.S_un.S_addr = inet_addr(m_Ip.c_str());

	//------------------------
	// 接続
	//------------------------
	if (connect(m_sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
	{
		printf("接続できませんでした。\n");
		return false;
	}
	else
	{
	
		printf("接続出来ました。\n");
		return true;
	}
}


