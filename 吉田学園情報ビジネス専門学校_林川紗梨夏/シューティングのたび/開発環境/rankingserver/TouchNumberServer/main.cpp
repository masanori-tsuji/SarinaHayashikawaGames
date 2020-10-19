//=============================================================================
//
// ���C������ [main.cpp]
// Author : �ѐ�ї���
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include <stdio.h>
#include <winsock2.h>
#include <thread>
#include <mutex>
#include "tcp_listener.h"
#include "tcp_client.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SERVER_PORT_NUM 12345 //�ڑ���T�[�o�̐ڑ���|�[�g
#define MAX_ACCEPT_CONNECTION 5//�ڑ���t�ő吔
#define MAX_RANKING 10
#define MAX_NAME 8

//�\���̒�`
typedef struct
{

	int nClearTime;
	char aName[MAX_NAME];
}RankingData;

//*****************************************************************************
// �񋓌^�錾
//*****************************************************************************
typedef enum
{
	COMMAND_TYPE_NONE = 0,
	COMMAND_TYPE_GET_RANKING,		// �����L���O�ꗗ���擾
	COMMAND_TYPE_SET_RANKING		// �����L���O��ݒ�
} COMMAND_TYPE;

void ThreadServer(CTcpClient *pTcpClient);

//�v���g�^�C�v�錾
void ReadFile(void);
void WriteFile(void);
void WorkerThread(CTcpClient *pTcpClient);
int SetRanking(int nTime, char *pName);
//�O���[�o���ϐ�
RankingData g_aRankingData[MAX_RANKING];
//=============================================================================
// ���C������
//=============================================================================
int main()
{
	//�����L���O�f�[�^��K���ɕύX���ăt�@�C�����X�V����邱�Ƃ��m�F����

	//g_aRankingData[0].nClearTime = 5000;
	//scanf("%s", g_aRankingData[0].aName);
	//WriteFile();

	WSADATA wsaData;

	//winsock����
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);
	if (nErr != 0)
	{
		//���炩�̌����ŏ������Ɏ��s����
		printf("WSAStartup failed\n");
		return 1;
	}

	//�O���[�o���ϐ�������
	memset(g_aRankingData, 0, sizeof(g_aRankingData));
	//�����L���O�t�@�C���ǂݍ���
	ReadFile();

	CTcpListener *pTcpListener = pTcpListener->Create(SERVER_PORT_NUM);

	if (pTcpListener == NULL)
	{
		//�\�P�b�g�ڑ����N���[�Y
		pTcpListener->Close();
		return 1;
	}

	//�\�P�b�g�ڑ����󂯕t����i�󂯕t����܂Ńv���O�����̓u���b�N�����
	while (1)
	{
		CTcpClient*pTcpClient = pTcpListener->Accept();

		if (pTcpClient == NULL)
		{
			continue;
		}

		////thread����
		std::thread WorkerThread(WorkerThread, pTcpClient);
		WorkerThread.detach();
	}

	//�\�P�b�g�ڑ����N���[�Y
	pTcpListener->Release();

	//winsock�I������
	WSACleanup();

	rewind(stdin);
	getchar();

	return 0;

}

////=============================================================================
//// ����M�X���b�h����
////=============================================================================
//void ThreadServer(CTcpClient * pTcpClient)
//{
//	while (1)
//	{
//
//		//���M���e�̎擾
//		char aRecvBuffer[BUFFER_NUM];
//
//		int nNumRecv = pTcpClient->Recv(aRecvBuffer, BUFFER_NUM);
//
//		if (nNumRecv <= 0)
//		{
//			break;
//		}
//
//		printf("[��M�f�[�^]>%s\n", &aRecvBuffer[0]);
//
//		if (strstr(&aRecvBuffer[0], "����ɂ���"))
//		{
//			//send(sock,(char*)&nData,sizeof((char*)&nData), 0); //send(accept�̖߂�l,���M�f�[�^,�f�[�^�T�C�Y, 0);
//			pTcpClient->Send("����ɂ��́I�I�I�I�I�I�I�I�I�I", strlen("����ɂ��́I�I�I�I�I�I�I�I�I�I"));
//		}
//		else if (strstr(&aRecvBuffer[0], "������"))
//		{
//			pTcpClient->Send("�^�X�N�o�[�̉E�[��������������", strlen("�^�X�N�o�[�̉E�[��������������"));
//		}
//		else if (strstr(&aRecvBuffer[0], "�I��"))
//		{
//			pTcpClient->Send("�I��", strlen("�I��"));
//		}
//		else
//		{
//			//send(sock,(char*)&nData,sizeof((char*)&nData), 0); //send(accept�̖߂�l,���M�f�[�^,�f�[�^�T�C�Y, 0);
//			pTcpClient->Send("�킩��܂���", strlen("�킩��܂���"));
//
//		}
//	}
//	//�\�P�b�g�ڑ����N���[�Y
//	pTcpClient->Close();
//}

void ReadFile(void)
{
	FILE *pFile;

	pFile = fopen("RankingData.txt", "r"); //�t�@�C���̓ǂݍ���

	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
		{
			fscanf(pFile, "%d", &g_aRankingData[nCnt].nClearTime);//�N���A�^�C������
			fscanf(pFile, "%s", &g_aRankingData[nCnt].aName);	  //���O����
		}
		fclose(pFile); //�t�@�C�������
		printf("�t�@�C���̓ǂݍ��݂��������܂����B\n"); //�ǂݍ��݊������b�Z�[�W
	}
	else
	{
		//printf("\n");
		printf("�t�@�C�����I�[�v���o���܂���ł���\n�B"); //�G���[���b�Z�[�W
	}
}

void WriteFile(void)
{
	FILE *pFile;

	pFile = fopen("RankingData.txt", "w"); //�t�@�C���̏�������

	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
		{
			fprintf(pFile, "%d\n", g_aRankingData[nCnt].nClearTime);	//�����L���O���o��
			fprintf(pFile, "%s\n", g_aRankingData[nCnt].aName);			//���O���o��
			fprintf(pFile, "\n");
		}
		fclose(pFile); //�t�@�C�������
	}
	else
	{
		printf("\n");
		printf("�t�@�C�����I�[�v���o���܂���ł����B"); //�G���[���b�Z�[�W
	}
}

void WorkerThread(CTcpClient * pTcpClient)
{
	//��M���e�̎擾
	char aRecvBuffer[BUFFER_NUM];
	char aSendBuf[BUFFER_NUM];

	int nNumRecv = pTcpClient->Recv(aRecvBuffer, sizeof(aRecvBuffer));

	if (nNumRecv <= 0)
	{
		//�\�P�b�g�ڑ����N���[�Y
		pTcpClient->Close();
	}

	COMMAND_TYPE command = (COMMAND_TYPE)aRecvBuffer[0];

	int nIndex = 0;
	int nData = 1;
	int nRank = 1;
	int nTime = 0;
	switch (command)
	{
	case COMMAND_TYPE_GET_RANKING:
		//�����L���O�ꗖ�擾���N�G�X�g
		for (int nCount = 0; nCount < MAX_RANKING; nCount++)
		{
			nData = (int)htonl(g_aRankingData[nCount].nClearTime);
			memcpy(&aSendBuf[nIndex], &nData, sizeof(int));
			nIndex += sizeof(int);
			g_aRankingData[nCount].aName[MAX_NAME - 1] = '\0';
			memcpy(&aSendBuf[nIndex], g_aRankingData[nCount].aName, MAX_NAME);
			nIndex += MAX_NAME;
		}
		pTcpClient->Send(aSendBuf, sizeof(aSendBuf));
		printf("���M����\n");
		break;
	case COMMAND_TYPE_SET_RANKING:
		//�����L���O�ݒ胊�N�G�X�g

		//�N���A�^�C�����擾
		nTime = ntohl(*(int*)&aRecvBuffer[1]);

		//���O�擾
		char aName[MAX_NAME];
		memcpy(aName, &aRecvBuffer[5], MAX_NAME);
		aName[MAX_NAME - 1] = '\0';

		//�����L���O�X�V
		nRank = SetRanking(nTime, aName);

		//�����L���O���ʑ��M
		aSendBuf[0] = (char)nRank;
		pTcpClient->Send(aSendBuf, sizeof(aSendBuf));
		break;
	default:
		break;
	}

	WriteFile();

	//�\�P�b�g�ڑ����N���[�Y
	pTcpClient->Release();
}

int SetRanking(int nTime, char * pName)
{
	int nCnt = 0;

	for (nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		//	�����L���O��񂪓����Ă��邩�m�F����
		if (g_aRankingData[nCnt].nClearTime == 0 || nTime > g_aRankingData[nCnt].nClearTime)
		{
			//�ȍ~�̃����L���O�f�[�^�����Ɉړ�
			for (int nCntMove = MAX_RANKING -1;nCntMove > nCnt; nCntMove--)
			{
				g_aRankingData[nCntMove].nClearTime = g_aRankingData[nCntMove - 1].nClearTime;
				strcpy(g_aRankingData[nCntMove].aName, g_aRankingData[nCntMove - 1].aName);
			}
			g_aRankingData[nCnt].nClearTime = nTime;
			strcpy(g_aRankingData[nCnt].aName, pName);
			break;
			
		}
	}
	WriteFile();
	return nCnt + 1;
}
