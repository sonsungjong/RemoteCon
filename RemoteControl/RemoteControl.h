
#ifdef REMOTECONTROL_EXPORTS
#define REMOTECONTROL_API __declspec(dllexport)
#else
#define REMOTECONTROL_API __declspec(dllimport)
#endif

// This class is exported from the dll
class REMOTECONTROL_API CRemoteControl {
public:
	CRemoteControl(void);
	// TODO: add your methods here.
	void OnExe(int port, const char* ip);
	void OnKill(int port, const char* ip);
	void OnShutDown(int port, const char* ip);
};

extern REMOTECONTROL_API int nRemoteControl;

REMOTECONTROL_API int fnRemoteControl(void);
