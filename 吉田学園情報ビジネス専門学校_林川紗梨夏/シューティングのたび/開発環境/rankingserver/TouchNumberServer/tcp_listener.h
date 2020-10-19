//=============================================================================
//
// TCP接続受付の処理 [tcp_listener.h]
// Author : 
//
//=============================================================================
#ifndef _TCPLISTENER_H_
#define _TCPLISTENER_H_

#include <winsock2.h>
//*********************************************************
// 前方宣言
//*********************************************************
class CTcpClient;
//*********************************************************
// TCP接続受付クラス
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
