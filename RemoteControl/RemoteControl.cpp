// RemoteControl.cpp : Defines the exported functions for the DLL.
//

#include "framework.h"
#include "RemoteControl.h"
#include <cstdio>
#include <WS2tcpip.h>
#include <string>
#include <tchar.h>
#pragma comment(lib, "WS2_32.lib")


// This is an example of an exported variable
REMOTECONTROL_API int nRemoteControl=0;

// This is an example of an exported function.
REMOTECONTROL_API int fnRemoteControl(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CRemoteControl::CRemoteControl()
{
	
}

void CRemoteControl::OnExe(int port, const char* ip)
{
    // 역할을 모르겠음
}

void CRemoteControl::OnKill(int port, const char* ip)
{
	WSADATA data;
	WSAStartup(0x0202, &data);

	SOCKET listen_socket = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in addr_data = { AF_INET, htons(port), };
	InetPton(AF_INET, ip, &addr_data.sin_addr.s_addr);

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	InetPton(AF_INET, ip, &server.sin_addr);

	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	char udp_msg[256];
	ZeroMemory(udp_msg, 256);
	sprintf_s(udp_msg, 256, "k%s", ip);
	sendto(out, udp_msg, 256, 0, (sockaddr*)&server, sizeof(server));

	closesocket(listen_socket);

	WSACleanup();
}

void CRemoteControl::OnShutDown(int port, const char* ip)
{
	WSADATA data;
	WSAStartup(0x0202, &data);

	SOCKET listen_socket = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in addr_data = { AF_INET, htons(port), };
	InetPton(AF_INET, ip, &addr_data.sin_addr.s_addr);

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	InetPton(AF_INET, ip, &server.sin_addr);

	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	char udp_msg[256];
	ZeroMemory(udp_msg, 256);
	sprintf_s(udp_msg, 256, "s%s", ip);
	sendto(out, udp_msg, 256, 0, (sockaddr*)&server, sizeof(server));

	closesocket(listen_socket);

	WSACleanup();
}