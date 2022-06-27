#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <TlHelp32.h>
//#include <tchar.h>
#include <cstdio>
#pragma comment (lib, "WS2_32.lib")

void WindowsOff1()
{
    // 20초후 윈도우즈 종료
    system("shutdown -s -f -t 20");
}

void WindowsOff2(int a_second)
{
    char sysoff[128];
    sprintf_s(sysoff, 128, "C:\\Windows\\System32\\shutdown.exe -s -f -t %d", a_second);

    // a_second초후 윈도우즈 종료
    system(sysoff);
}

void IPCatch33()
{
    WORD wVersionRequested = MAKEWORD(0x02, 0x02);
    WSADATA wsaData;
    char my_ip[256];
    ZeroMemory(my_ip, 256);
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
    printf("%s", my_ip);
}

BOOL ProcessKill1(const TCHAR* a_process_name)
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

int main()
{
    //if (ProcessKill1("vmplayer.exe"))
    //{
    //    printf("종료 성공1\n");
    //}
    //else {
    //    printf("종료 안됨1\n");
    //}
    //Sleep(10);
    //WindowsOff2(20);
    IPCatch33();
}
