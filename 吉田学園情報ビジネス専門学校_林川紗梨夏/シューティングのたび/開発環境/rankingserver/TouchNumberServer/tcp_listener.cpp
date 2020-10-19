//=============================================================================
//
// TCP�ڑ���t�̏��� [tcp_listener.cpp]
// Author : �ѐ�ї���
//
//=============================================================================
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "tcp_listener.h"
#include "tcp_client.h"
#include "main.h"
#include <stdio.h>
#include <winsock2.h>
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ACCEPT_CONN 100			// �ڑ���t�ő吔

//=============================================================================
// CTcpListener����
//=============================================================================
CTcpListener* CTcpListener::Create(int nPortNum)
{
	// TCP�ڑ���t�N���X�̃C���X�^���X����
	CTcpListener *pTcpListener = new CTcpListener;

	if (pTcpListener->Init(nPortNum, MAX_ACCEPT_CONN) == false)
	{
		delete pTcpListener;
		pTcpListener = NULL;
	}
	return pTcpListener;
}

//=============================================================================
// CTcpListener�R���X�g���N�^
//=============================================================================
CTcpListener::CTcpListener()
{
	m_sockServer = -1;
}

//=============================================================================
// CTcpListener�f�X�g���N�^
//=============================================================================
CTcpListener::~CTcpListener()
{
}

//=============================================================================
// ����������
//=============================================================================
bool CTcpListener::Init(int nPortNum, int nAcceptConnNum)
{
	// �ڑ���t�p�\�P�b�g�̐��� (socket)
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	//�ڑ�����̐ݒ�
	struct sockaddr_in addr;

	// �ڑ���t���̐ݒ�
	addr.sin_family = AF_INET;               //AF_INET = IPV4
	addr.sin_port = htons(nPortNum);            //�|�[�g�ԍ�
	addr.sin_addr.S_un.S_addr = INADDR_ANY;  //INADDR_ANY = ���ׂĂ̐ڑ����󂯕t����

											 // �\�P�b�g��IP�A�h���X�ƃ|�[�g�ԍ���ݒ肷�� (bind)
	bind(m_sockServer, (struct sockaddr*)&addr, sizeof(addr));

	// �ڑ��v����t���J�n���� (listen)
	listen(m_sockServer, MAX_ACCEPT_CONN);//�ڑ��\��

	return true;
}

//=============================================================================
// �ڑ���t����
//=============================================================================
CTcpClient * CTcpListener::Accept()
{
	struct sockaddr_in client;

	if (m_sockServer < 0) {
		return NULL;
	}

	// �ڑ���t����
	int nAddrLength = sizeof(client);
	SOCKET sock = accept(m_sockServer, (struct sockaddr*)&client, &nAddrLength);
	CTcpClient *pTcpClient = CTcpClient::Create(sock);
	return pTcpClient;
}

//=============================================================================
// �j������
//=============================================================================
void CTcpListener::Release()
{
	// �ؒf����
	Close();

	// �C���X�^���X�폜
	delete this;
}

//=============================================================================
// �N���[�Y����
//=============================================================================
void CTcpListener::Close()
{
	if (m_sockServer < 0) {
		return;
	}

	// �\�P�b�g�ڑ����N���[�Y
	closesocket(m_sockServer);
}
