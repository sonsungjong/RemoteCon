#ifndef _REMOTE_CON_H_
#define _REMOTE_CON_H_
class RemoteCon
{
private:
	int m_port = 40004;
public:
	void ProcessKill(const char* a_process_name);
	void WindowsOff();
	void IPCatch(char* my_ip);
	int GetPort();
};

#endif