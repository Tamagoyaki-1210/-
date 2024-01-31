//============================
//
// 背景
// Author:hamada ryuuga
//
//============================
#ifndef _UDP_CLIENT_H_
#define _UDP_CLIENT_H_

//**************************************************
// インクルード
//**************************************************

class CModel;
//**************************************************
// クラス
//**************************************************
// ソケット通信クラス
class CUdp_Client
{
public:
	CUdp_Client();
	~CUdp_Client();

	bool Init(const char*plPAddress, int nPortNum);

	void Init(SOCKET sockfd) { m_sock = sockfd; }

	void Uninit();
	void UpData();

	void SetAddr(sockaddr_in addr) { m_clientAddr = addr; }
	sockaddr_in GetAddr() { return m_clientAddr; }
	void Send(const char *pSendData, int Size);
	int Recv(char *recvBuffer, int Size);
	SOCKET GetSock() { return m_sock; }
	bool CUdp_Client::Connect();

private:
	SOCKET m_sock;
	sockaddr_in m_serverAddr;
	sockaddr_in m_clientAddr;
};

#endif
