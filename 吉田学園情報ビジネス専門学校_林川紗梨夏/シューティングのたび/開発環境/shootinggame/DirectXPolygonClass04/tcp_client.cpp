#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>
#include "main.h"
#include "tcp_client.h"

//コンストラクタ
CTcpClient::CTcpClient()
{
	m_sock = -1;
}
//デストラクタ
CTcpClient::~CTcpClient()
{

}

//初期化
bool CTcpClient::Init(const char *pHostName, int nPortNum)
{

	//接続ソケットの生成
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET) {
		printf("socket:%d\n", WSAGetLastError());
		return false;
	}

	//接続先情報の設定
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPortNum);
	addr.sin_addr.S_un.S_addr = inet_addr(pHostName);

	//サーバーに接続する
	if (connect(m_sock, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
		printf("connect:%d\n", WSAGetLastError());
		//ソケット接続をクローズ
		closesocket(m_sock);
		return false;
	}
	return true;
}

//送信
int CTcpClient::Send(char *pSendData, int nSendDataSize)
{

	if (m_sock < 0)
	{
		return 0;
	}
	int nSendSize = send(m_sock, pSendData, nSendDataSize, 0);
	return nSendSize;
}

//受信
int CTcpClient::Recv(char *pRecvBuf, int nRecvBufSize)
{
	//バッファを0クリア
	memset(pRecvBuf, 0, nRecvBufSize);

	int nRecvSize = recv(m_sock, pRecvBuf, nRecvBufSize, 0);
	if (nRecvBufSize <= 0)
	{
		closesocket(m_sock);
	}

	printf("[受信データ]>>%s\n", pRecvBuf);//サーバからの受信データを出力

	return nRecvBufSize;
}

void CTcpClient::Close()
{
	if (m_sock < 0)
	{
		return;
	}
	closesocket(m_sock);
	m_sock = -1;
}

CTcpClient *CTcpClient::Create(const char*pHostName, int nPortNum)
{
	CTcpClient *pTcpClient = new CTcpClient;
	if (pTcpClient->Init(pHostName, nPortNum) == false)
	{
		delete pTcpClient;
		pTcpClient = NULL;
	}
	return pTcpClient;
}

void CTcpClient::Release(void)
{
	Close();
	delete this;
}