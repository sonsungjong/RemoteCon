#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdio>
#pragma comment (lib, "WS2_32.lib")

#define PORT 40004

BOOL ProcessKill(const char* a_process_name);
void WindowsOff3();
void IPCatch(char* my_ip)
{
	WORD wVersionRequested = MAKEWORD(0x02, 0x02);
	WSADATA wsaData;

	//char my_ip[256];
	//ZeroMemory(my_ip, 256);

	if (0 == WSAStartup(wVersionRequested, &wsaData))
	{
		char ucHostName[255];
		if (0 == gethostname(ucHostName, sizeof(ucHostName)))
		{
			PHOSTENT phostInfo = gethostbyname(ucHostName);
			if (NULL != phostInfo)
			{
				for (int i = 0; phostInfo->h_addr_list[i] != NULL; i++)
				{
					int iIP[4] = { 0, };
					in_addr* pAddr = (in_addr*)phostInfo->h_addr_list[i];
					iIP[0] = pAddr->S_un.S_un_b.s_b1;
					iIP[1] = pAddr->S_un.S_un_b.s_b2;
					iIP[2] = pAddr->S_un.S_un_b.s_b3;
					iIP[3] = pAddr->S_un.S_un_b.s_b4;
					sprintf_s(my_ip, 256, "%d.%d.%d.%d", iIP[0], iIP[1], iIP[2], iIP[3]);
				}
			}
		}
	}
	//printf("%s", my_ip);
}

int main()
{
	WSADATA data;
	WSAStartup(0x0202, &data);

	SOCKET listen_socket = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in addr_data = { AF_INET, htons(PORT), };
	InetPtonA(AF_INET, "127.0.0.1", &addr_data.sin_addr.s_addr);

	bind(listen_socket, (sockaddr*)&addr_data, sizeof(addr_data));
	
	sockaddr_in recv_socket;
	ZeroMemory(&recv_socket, sizeof(recv_socket));
	int recv_len = sizeof(recv_socket);

	char buf[1024];
	ZeroMemory(buf, 1024);

	while (true)
	{
		ZeroMemory(buf, 1024);

		char serverIP[256];
		ZeroMemory(serverIP, 256);
		IPCatch(serverIP);

		printf("%s", serverIP);

		int byesIn = recvfrom(listen_socket, buf, 1024, 0, (sockaddr*)&recv_socket, &recv_len);
		
		char clientIP[256];
		ZeroMemory(clientIP, 256);
		inet_ntop(AF_INET, &recv_socket.sin_addr, clientIP, 256);
		printf("Message rect from %s : %s", clientIP, buf);
		if (0 == strcmp(buf, serverIP))
		{
			ProcessKill("vmplayer.exe");
			Sleep(10);
			WindowsOff3();
		}
	}

	closesocket(listen_socket);

	WSACleanup();

	return 0;
}

BOOL ProcessKill(const char* a_process_name)
{
	HANDLE sj_handle_first = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	DWORD_PTR dwsma = GetLastError();
	HANDLE sj_handle_second;
	DWORD dwExitCode = 0;

	PROCESSENTRY32 procEntry = { 0 };
	procEntry.dwSize = sizeof(PROCESSENTRY32);
	Process32First(sj_handle_first, &procEntry);

	while (1)
	{
		if (!strcmp(procEntry.szExeFile, a_process_name))
		{
			sj_handle_second = OpenProcess(PROCESS_ALL_ACCESS, 0, procEntry.th32ProcessID);
			if (GetExitCodeProcess(sj_handle_second, &dwExitCode))
			{
				if (TerminateProcess(sj_handle_second, dwExitCode))
				{
					return TRUE;
				}
			}
		}
		if (!Process32Next(sj_handle_first, &procEntry))
		{
			return FALSE;
		}
	}
	return FALSE;
}

void WindowsOff3()
{
	// 20초후 윈도우즈 종료
	system("shutdown -s -f -t 20");
}

