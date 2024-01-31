//============================
//
// clientê›íË
// Author:hamada ryuuga
//
//============================

#ifndef _TCP_CKIENT_H_
#define _TCP_CKIENT_H_

class CUdp_Client;
#include "model_data.h"


class  CTcp_Client
{
public:
	CTcp_Client();
	~CTcp_Client();
	bool Init(const char*plPAddress, int nPortNum);
	bool Init(SOCKET sock);
	int Send(const char*pSendData, int nSendDataSize);
	int Recv(char*pRecvData, int nRecvDataSize);
	void Uninit();
	bool Connect();
	SOCKET GetSock() { return m_sock; }


	//void ConnectTh();
private:
	std::string m_Ip;
	int m_nPortNum;
	SOCKET m_sock;
	bool m_sockOk;


};

#endif