//=============================================================================
//
// TCP�ڑ���t�̏��� [tcp_listener.h]
// Author : 
//
//=============================================================================
#ifndef _TCPLISTENER_H_
#define _TCPLISTENER_H_

#include <winsock2.h>
//*********************************************************
// �O���錾
//*********************************************************
class CTcpClient;
//*********************************************************
// TCP�ڑ���t�N���X
//*********************************************************
class CTcpListener
{
public:
	CTcpListener();
	~CTcpListener();

	static CTcpListener * Create(int nPortNum);
	bool Init(int nPortNum, int nAcceptConnNum);
	CTcpClient * Accept(void);
	void Release(void);
	void Close(void);
private:
	SOCKET m_sockServer;
};

#endif
