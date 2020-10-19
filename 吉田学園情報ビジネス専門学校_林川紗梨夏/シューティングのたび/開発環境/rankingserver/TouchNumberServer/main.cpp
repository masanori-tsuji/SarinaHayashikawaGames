//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 林川紗梨夏
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
// マクロ定義
//*****************************************************************************
#define SERVER_PORT_NUM 12345 //接続先サーバの接続先ポート
#define MAX_ACCEPT_CONNECTION 5//接続受付最大数
#define MAX_RANKING 10
#define MAX_NAME 8

//構造体定義
typedef struct
{

	int nClearTime;
	char aName[MAX_NAME];
}RankingData;

//*****************************************************************************
// 列挙型宣言
//*****************************************************************************
typedef enum
{
	COMMAND_TYPE_NONE = 0,
	COMMAND_TYPE_GET_RANKING,		// ランキング一覧を取得
	COMMAND_TYPE_SET_RANKING		// ランキングを設定
} COMMAND_TYPE;

void ThreadServer(CTcpClient *pTcpClient);

//プロトタイプ宣言
void ReadFile(void);
void WriteFile(void);
void WorkerThread(CTcpClient *pTcpClient);
int SetRanking(int nTime, char *pName);
//グローバル変数
RankingData g_aRankingData[MAX_RANKING];
//=============================================================================
// メイン処理
//=============================================================================
int main()
{
	//ランキングデータを適当に変更してファイルが更新されることを確認する

	//g_aRankingData[0].nClearTime = 5000;
	//scanf("%s", g_aRankingData[0].aName);
	//WriteFile();

	WSADATA wsaData;

	//winsock初期
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);
	if (nErr != 0)
	{
		//何らかの原因で初期化に失敗した
		printf("WSAStartup failed\n");
		return 1;
	}

	//グローバル変数初期化
	memset(g_aRankingData, 0, sizeof(g_aRankingData));
	//ランキングファイル読み込み
	ReadFile();

	CTcpListener *pTcpListener = pTcpListener->Create(SERVER_PORT_NUM);

	if (pTcpListener == NULL)
	{
		//ソケット接続をクローズ
		pTcpListener->Close();
		return 1;
	}

	//ソケット接続を受け付ける（受け付けるまでプログラムはブロックされる
	while (1)
	{
		CTcpClient*pTcpClient = pTcpListener->Accept();

		if (pTcpClient == NULL)
		{
			continue;
		}

		////thread生成
		std::thread WorkerThread(WorkerThread, pTcpClient);
		WorkerThread.detach();
	}

	//ソケット接続をクローズ
	pTcpListener->Release();

	//winsock終了処理
	WSACleanup();

	rewind(stdin);
	getchar();

	return 0;

}

////=============================================================================
//// 送受信スレッド処理
////=============================================================================
//void ThreadServer(CTcpClient * pTcpClient)
//{
//	while (1)
//	{
//
//		//送信内容の取得
//		char aRecvBuffer[BUFFER_NUM];
//
//		int nNumRecv = pTcpClient->Recv(aRecvBuffer, BUFFER_NUM);
//
//		if (nNumRecv <= 0)
//		{
//			break;
//		}
//
//		printf("[受信データ]>%s\n", &aRecvBuffer[0]);
//
//		if (strstr(&aRecvBuffer[0], "こんにちは"))
//		{
//			//send(sock,(char*)&nData,sizeof((char*)&nData), 0); //send(acceptの戻り値,送信データ,データサイズ, 0);
//			pTcpClient->Send("こんにちは！！！！！！！！！！", strlen("こんにちは！！！！！！！！！！"));
//		}
//		else if (strstr(&aRecvBuffer[0], "今何時"))
//		{
//			pTcpClient->Send("タスクバーの右端をご覧ください", strlen("タスクバーの右端をご覧ください"));
//		}
//		else if (strstr(&aRecvBuffer[0], "終了"))
//		{
//			pTcpClient->Send("終了", strlen("終了"));
//		}
//		else
//		{
//			//send(sock,(char*)&nData,sizeof((char*)&nData), 0); //send(acceptの戻り値,送信データ,データサイズ, 0);
//			pTcpClient->Send("わかりません", strlen("わかりません"));
//
//		}
//	}
//	//ソケット接続をクローズ
//	pTcpClient->Close();
//}

void ReadFile(void)
{
	FILE *pFile;

	pFile = fopen("RankingData.txt", "r"); //ファイルの読み込み

	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
		{
			fscanf(pFile, "%d", &g_aRankingData[nCnt].nClearTime);//クリアタイム入力
			fscanf(pFile, "%s", &g_aRankingData[nCnt].aName);	  //名前入力
		}
		fclose(pFile); //ファイルを閉じる
		printf("ファイルの読み込みが完了しました。\n"); //読み込み完了メッセージ
	}
	else
	{
		//printf("\n");
		printf("ファイルがオープン出来ませんでした\n。"); //エラーメッセージ
	}
}

void WriteFile(void)
{
	FILE *pFile;

	pFile = fopen("RankingData.txt", "w"); //ファイルの書き込み

	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
		{
			fprintf(pFile, "%d\n", g_aRankingData[nCnt].nClearTime);	//ランキングを出力
			fprintf(pFile, "%s\n", g_aRankingData[nCnt].aName);			//名前を出力
			fprintf(pFile, "\n");
		}
		fclose(pFile); //ファイルを閉じる
	}
	else
	{
		printf("\n");
		printf("ファイルがオープン出来ませんでした。"); //エラーメッセージ
	}
}

void WorkerThread(CTcpClient * pTcpClient)
{
	//受信内容の取得
	char aRecvBuffer[BUFFER_NUM];
	char aSendBuf[BUFFER_NUM];

	int nNumRecv = pTcpClient->Recv(aRecvBuffer, sizeof(aRecvBuffer));

	if (nNumRecv <= 0)
	{
		//ソケット接続をクローズ
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
		//ランキング一蘭取得リクエスト
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
		printf("送信完了\n");
		break;
	case COMMAND_TYPE_SET_RANKING:
		//ランキング設定リクエスト

		//クリアタイムを取得
		nTime = ntohl(*(int*)&aRecvBuffer[1]);

		//名前取得
		char aName[MAX_NAME];
		memcpy(aName, &aRecvBuffer[5], MAX_NAME);
		aName[MAX_NAME - 1] = '\0';

		//ランキング更新
		nRank = SetRanking(nTime, aName);

		//ランキング順位送信
		aSendBuf[0] = (char)nRank;
		pTcpClient->Send(aSendBuf, sizeof(aSendBuf));
		break;
	default:
		break;
	}

	WriteFile();

	//ソケット接続をクローズ
	pTcpClient->Release();
}

int SetRanking(int nTime, char * pName)
{
	int nCnt = 0;

	for (nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		//	ランキング情報が入っているか確認する
		if (g_aRankingData[nCnt].nClearTime == 0 || nTime > g_aRankingData[nCnt].nClearTime)
		{
			//以降のランキングデータを後ろに移動
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
