using Mono.Cecil;
using System;
using System.IO;
using System.Net.Sockets;
using System.Text;
using UnityEngine;

public class TCPClient
{

    // C++에서는 SOCKET부터 다 선언을 해줘야 하지만
    // C#에서는 TcpClient라는 클래스안에 다 감싸져 있음
    public TcpClient m_Client;

    // 이거는 C++에 없지만 C#에서 socket의 send랑 recv를 호출하는 래퍼이다.
    public NetworkStream m_Stream;

    public void Connect(string ip, int port)
    {
        m_Client = new TcpClient();

        // 우선 C++에서 socket(IPv4(6), SOCK_STREAM, 0) 이라고 생각하면 된다.
        // C#은 IP를 보고 자동으로 판단해서 맞는 Socket을 생성 해준다.
        // C++에서 cennect함수로 3 way - handshake를 수행 해준다.
        m_Client.Connect(ip, port);

        // C++에서는 없지만 C#에선 socket을 직접 사용하지 않고 stream으로 감싸서 사용한다.
        m_Stream = m_Client.GetStream();

        if (m_Client.Connected)
            Debug.Log("연결 성공");
    }

    public void send(string message)
    {
        byte[] buffer = Encoding.UTF8.GetBytes(message);
        byte[] len = BitConverter.GetBytes(buffer.Length);

        m_Stream.Write(len, 0, 4);
        // C++의 send랑 동일한 기능이다.
        m_Stream.Write(buffer, 0, buffer.Length);
    }

    public string Receive()
    {
        byte[] buffer = new byte[1024];

        // C++의 recv랑 동일한 기능이다.
        int size = m_Stream.Read(buffer, 0, buffer.Length);

        // C++에서는 받은 buffer를 직접 변환해서 처리해야 하지만
        // C#에서는 문자열 변환까지 라이브러리로 지원이 된다.
        return Encoding.UTF8.GetString(buffer, 0, size);
    }

    public bool HasData()
    {
        return m_Stream != null && m_Stream.DataAvailable;
    }

    public void Disconnect()
    {
        // C#에선 WSACleanup를 자동으로 처리해준다.
        // C++에서 closesocket이랑 동일한 기능이다.
        m_Stream?.Close();

        // C++에서는 소켓을 닫으면 끝나지면 C#은 stream이랑 client둘다 지워야 한다.
        m_Client?.Close();
    }
}
