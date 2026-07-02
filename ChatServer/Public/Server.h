#pragma once

#include "Define.h"

class Server
{
public:
	bool Start_Server();
	bool Accept_ClntSock();
	void ProcessClinet();

private:
	bool Init_Sock();
	bool Create_TCPSock();
	void Init_SerAdd();
	bool Bind_ServerSock();
	bool Listen_ServerSock();
	bool LogError(const char* message);

private:
	WSADATA		m_WsaData;
	SOCKET		m_hServSock;
	SOCKET		m_hClntSock;
	SOCKADDR_IN	m_ServAdr;
	SOCKADDR_IN	m_ClntAdr;
	int			m_iresult, m_iopndCnt;
	int			m_irecvCnt, m_irecvLen;
	int			m_iClntAdrSize;
};

