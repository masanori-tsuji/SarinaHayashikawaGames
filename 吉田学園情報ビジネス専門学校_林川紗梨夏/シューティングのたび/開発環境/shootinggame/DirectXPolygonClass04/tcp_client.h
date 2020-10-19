#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_


class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();
	static CTcpClient*Create(const char* pHostName, int nPortNum);
	void Release(void);
	bool Init(const char *pHostName, int nPortNum);//(IPアドレス,ポート番号),
	int Send(char *pSendData, int nSendDataSize);
	int Recv(char *pRecvBuf, int nRecvBufSize);
	void Close();
private:
	SOCKET m_sock;
};
#endif // !_TCP_CLIENT_H_
