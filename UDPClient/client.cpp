#include <cstdio>
#include <WS2tcpip.h>
#include <string>
#include <tchar.h>
#pragma comment(lib, "WS2_32.lib")

#define IP _T("127.0.0.1")
#define PORT			40004

void main()
{
	WSADATA data;
	WSAStartup(0x0202, &data);

	SOCKET listen_socket = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in addr_data = { AF_INET, htons(PORT), };
	InetPton(AF_INET, _T("127.0.0.1"), &addr_data.sin_addr.s_addr);

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	InetPton(AF_INET, IP, &server.sin_addr);

	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	char ss[256];
	printf("전송할 문자열 >>");
	scanf_s("%s", &ss, 256);
	sendto(out, ss, 256, 0, (sockaddr*)&server, sizeof(server));

	closesocket(listen_socket);

	WSACleanup();
}