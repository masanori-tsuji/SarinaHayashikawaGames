//=============================================================================
//
// TCP接続受付の処理 [tcp_listener.cpp]
// Author : 林川紗梨夏
//
//=============================================================================
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "tcp_listener.h"
#include "tcp_client.h"
#include "main.h"
#include <stdio.h>
#include <winsock2.h>
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ACCEPT_CONN 100			// 接続受付最大数

//=============================================================================
// CTcpListener生成
//=============================================================================
CTcpListener* CTcpListener::Create(int nPortNum)
{
	// TCP接続受付クラスのインスタンス生成
	CTcpListener *pTcpListener = new CTcpListener;

	if (pTcpListener->Init(nPortNum, MAX_ACCEPT_CONN) == false)
	{
		delete pTcpListener;
		pTcpListener = NULL;
	}
	return pTcpListener;
}

//=============================================================================
// CTcpListenerコンストラクタ
//=============================================================================
CTcpListener::CTcpListener()
{
	m_sockServer = -1;
}

//=============================================================================
// CTcpListenerデストラクタ
//=============================================================================
CTcpListener::~CTcpListener()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
bool CTcpListener::Init(int nPortNum, int nAcceptConnNum)
{
	// 接続受付用ソケットの生成 (socket)
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	//接続先情報の設定
	struct sockaddr_in addr;

	// 接続受付情報の設定
	addr.sin_family = AF_INET;               //AF_INET = IPV4
	addr.sin_port = htons(nPortNum);            //ポート番号
	addr.sin_addr.S_un.S_addr = INADDR_ANY;  //INADDR_ANY = すべての接続を受け付ける

											 // ソケットにIPアドレスとポート番号を設定する (bind)
	bind(m_sockServer, (struct sockaddr*)&addr, sizeof(addr));

	// 接続要求受付を開始する (listen)
	listen(m_sockServer, MAX_ACCEPT_CONN);//接続可能数

	return true;
}

//=============================================================================
// 接続受付処理
//=============================================================================
CTcpClient * CTcpListener::Accept()
{
	struct sockaddr_in client;

	if (m_sockServer < 0) {
		return NULL;
	}

	// 接続受付処理
	int nAddrLength = sizeof(client);
	SOCKET sock = accept(m_sockServer, (struct sockaddr*)&client, &nAddrLength);
	CTcpClient *pTcpClient = CTcpClient::Create(sock);
	return pTcpClient;
}

//=============================================================================
// 破棄処理
//=============================================================================
void CTcpListener::Release()
{
	// 切断処理
	Close();

	// インスタンス削除
	delete this;
}

//=============================================================================
// クローズ処理
//=============================================================================
void CTcpListener::Close()
{
	if (m_sockServer < 0) {
		return;
	}

	// ソケット接続をクローズ
	closesocket(m_sockServer);
}
