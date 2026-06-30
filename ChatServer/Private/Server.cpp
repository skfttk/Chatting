#include "Server.h"

bool Server::Start_Server()
{
	if (!Init_Sock())
		return false;

	if (!Create_TCPSock())
		return false;

	Init_SerAdd();

	if (!Bind_ServerSock())
		return false;	
	
	if (!Listen_ServerSock())
		return false;

	m_iClntAdrSize = sizeof(m_ClntAdr);
	return true;
}

// WinSock을 초기화 시켜주는 함수이다.
// 버전과 WSADATA를 저장할 구조체를 넣어준다.
bool Server::Init_Sock()
{
	if (WSAStartup(MAKEWORD(2, 2), &m_WsaData) != 0)
		return LogError("WSAStartup() error!");

	return true;
}

// 리스닝 소켓을 생성 한다.
// IPv4랑 6중 어떤걸 사용 할 지, TCP인지 UDP인지
// TCP는 SOCK_STREAM, UDP는 SOCK_DRAM이다.
// 마지막 인자는 원래 IPPROTO_TCP, IPPROTO_UDP를 적어야 하지만
// 어차피 SOCK_STRAM이나 SOCK_DRAM은 1개밖에 없어서 0을 적어도 알아서 해줌
bool Server::Create_TCPSock()
{
	m_hServSock = socket(PF_INET, SOCK_STREAM, 0);

	if (m_hServSock == INVALID_SOCKET)
	{
		// 만약 실패를 하면 초기화를 해준다.
		WSACleanup();
		return LogError("socket() error");
	}

	return true;
}

// IP랑 PORT의 주소를 초기화 해주고 저장을 해준다.
void Server::Init_SerAdd()
{
	m_ServAdr = {};
	m_ServAdr.sin_family = AF_INET;
	m_ServAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_ServAdr.sin_port = htons(m_iPort);
}

// 이 함수는 소켓과 IP, PORT를 연결(bind)하는 함수이다.
// bind()의 두 번째 매개변수는 SOCKADDR* 타입을 요구한다.
// 하지만 우리가 사용하는 주소 정보는 SOCKADDR_IN 구조체에 저장되어 있으므로,
// bind()에 전달하기 위해 SOCKADDR*로 형변환하여 넘겨준다.
bool Server::Bind_ServerSock()
{
	if (bind(m_hServSock, reinterpret_cast<SOCKADDR*>(&m_ServAdr), sizeof(m_ServAdr)) == SOCKET_ERROR)
	{
		closesocket(m_hServSock);
		WSACleanup();
		return LogError("bind() error");
	}

	return true;
}

// 생성한 서버 소켓을 연결 요청을 받을 수 있는 상태(리스닝 상태)로 만든다.
// 두 번째 인자인 5는 accept()가 처리하기 전에 운영체제가
// 대기시켜 둘 수 있는 연결 요청의 최대 개수(backlog)이다.
bool Server::Listen_ServerSock()
{
	if (listen(m_hServSock, 5) == SOCKET_ERROR)
		return LogError("listen() error");

	return true;
}

bool Server::Accept_ClntSock()
{
	m_hClntSock = accept(m_hServSock, reinterpret_cast<SOCKADDR*>(&m_ClntAdr), &m_iClntAdrSize);
	if (m_hClntSock == INVALID_SOCKET)
		return LogError("Can't accept a socket! Continuing...");

	return true;
}

void Server::ProcessClinet()
{
	char buf[1024];

	int len = recv(m_hClntSock, buf, sizeof(buf), 0);

	if (len <= 0)
	{
		closesocket(m_hClntSock);
		return;
	}

	cout << buf << endl;

	send(m_hClntSock, buf, len, 0);
}

bool Server::LogError(const char* message)
{
	cerr << message << '\n';
	return false;
}
