//============================
//
// clientÝ’è
// Author:hamada ryuuga
//
//============================

#ifndef _CKIENT_H_
#define _CKIENT_H_

//\‘¢‘Ì

class  CUdp_Client;
class  CTcp_Client;
class  CClient
{
public:
	enum CONNECT_TYPE
	{
		TYPE_UDP = 0,		// UDP
		TYPE_TUP,			// TCP
		MAX_TYPE,			// Å‘å”
	};
	struct CClientList
	{
		CUdp_Client* m_udp;
		CTcp_Client* m_tcp;
	};

	CClient();
	~CClient();
	bool Init(const char*plPAddress, int nPortNum); 
	bool Init(CClientList* List);
	void Uninit(void);
	int Send(const char*pSendData, int nSendDataSize, CONNECT_TYPE type);
	int Recv(char*pRecvData, int nRecvDataSize, CONNECT_TYPE type);


	void SetTcp(CTcp_Client *UdpSocket) { m_Socket->m_tcp = UdpSocket; }
	CTcp_Client* GetTcp() { return m_Socket->m_tcp; }

	void SetUdp(CUdp_Client *UdpSocket) { m_Socket->m_udp = UdpSocket; }
	CUdp_Client* GetUdp() { return m_Socket->m_udp; }

private:

	CClientList *m_Socket;
};

#endif