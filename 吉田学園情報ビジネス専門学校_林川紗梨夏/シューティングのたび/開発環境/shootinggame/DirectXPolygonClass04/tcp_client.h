#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_


class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();
	static CTcpClient*Create(const char* pHostName, int nPortNum);
	void Release(void);
	bool Init(const char *pHostName, int nPortNum);//(IP�A�h���X,�|�[�g�ԍ�),
	int Send(char *pSendData, int nSendDataSize);
	int Recv(char *pRecvBuf, int nRecvBufSize);
	void Close();
private:
	SOCKET m_sock;
};
#endif // !_TCP_CLIENT_H_
