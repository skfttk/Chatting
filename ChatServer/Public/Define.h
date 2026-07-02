#pragma once

#include <WinSock2.h>
#include <MSWSock.h>
#include <Windows.h>
#include <cstdlib>
#include <cstdint>

#pragma comment(lib, "ws2_32.lib")  // Winsock 라이브러리 링크

#include <iostream>
#include <assert.h>
#include <vector>


#define BUF_SIZE 1024
#define OPSZ 4
#define PORT 54000

using namespace std;

// 클라이언트의 서버 연결 요청을 수락할 IOCP
HANDLE AcceptPort = nullptr;

// 연결된 클라이언트가 보낸 send와 서버가 클라에게 전송할 recv를 처리할 IOCP
HANDLE SendRecvPort = nullptr;

static int m_iPort = PORT;
