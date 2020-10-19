//=============================================================================
//
// ランキング画面の処理 [ranking.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include <WinSock2.h>
#include "main.h"
#include "tcp_client.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "ranking.h"
#include "input.h"
#include "joystick.h"
#include "inputkeyboard.h"


LPDIRECT3DTEXTURE9 CRanking::m_apTexture[TYPE_MAX] = {};
CRanking::RankingData CRanking::m_aRankingData[MAX_RANKING] = {};//ランキングデータ
CRanking::CRanking(int nPriority)
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	memset(m_apScene2D, 0, sizeof(m_apScene2D));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSizeWidth = 0.0f;
	m_fSizeHeight = 0.0f;
	m_nPatternAnim = 1;
	m_nCountAnim = 0;
}
CRanking::~CRanking()
{

}

HRESULT CRanking::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BG_RANKING, &m_apTexture[TYPE_001]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BG2_RANKING, &m_apTexture[TYPE_002]);


	return S_OK;
}

void CRanking::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_BG_RANKING; nCnt++)
	{
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

CRanking * CRanking::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CRanking *pResult;

	pResult = new CRanking;
	pResult->Init(pos, fSizeWidth, fSizeHeight);

	return pResult;
}

HRESULT CRanking::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	//メンバ変数にサイズ引数を代入
	m_fSizeWidth = fSizeWidth;
	m_fSizeHeight = fSizeHeight;

	m_apScene2D[TYPE_001] = new CScene2D;
	m_apScene2D[TYPE_001]->Init(pos, fSizeWidth, fSizeHeight);
	m_apScene2D[TYPE_001]->BindTexture(m_apTexture[TYPE_001]);

	// 頂点座標を設定
	m_vPos[0] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
	m_vPos[1] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
	m_vPos[2] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);
	m_vPos[3] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);

	//頂点座標のセット
	m_apScene2D[TYPE_001]->SetVtxPosition(m_vPos);

	//色のセット
	m_apScene2D[TYPE_001]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	//テクスチャ座標のセット
	m_apScene2D[TYPE_001]->SetTex(
		0.0f,
		0.0f,
		1.0f,
		1.0f);

	m_apScene2D[TYPE_002] = new CScene2D;
	m_apScene2D[TYPE_002]->Init(pos, fSizeWidth, RANKING_02_SIZE_HEIGHT);
	m_apScene2D[TYPE_002]->BindTexture(m_apTexture[TYPE_002]);

	// 頂点座標を設定
	m_vPos[0] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y - 300.0f + (-RANKING_02_SIZE_HEIGHT / 2), 0.0f);
	m_vPos[1] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y - 300.0f + (-RANKING_02_SIZE_HEIGHT / 2), 0.0f);
	m_vPos[2] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y - 300.0f + (RANKING_02_SIZE_HEIGHT / 2), 0.0f);
	m_vPos[3] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y - 300.0f + (RANKING_02_SIZE_HEIGHT / 2), 0.0f);

	//頂点座標のセット
	m_apScene2D[TYPE_002]->SetVtxPosition(m_vPos);

	//色のセット
	m_apScene2D[TYPE_002]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	//テクスチャ座標のセット
	m_apScene2D[TYPE_002]->SetTex(
		0.0f,
		0.0f,
		1.0f,
		1.0f);


	CManager::PlayerData pPlayerData = CManager::GetPlayerData();
	
	SetRanking(pPlayerData.nScore, pPlayerData.aName);//ランキング送信リクエスト
	GetRanking(m_aRankingData);//ランキング送信リクエスト
	return S_OK;
}

void CRanking::Uninit(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt] = NULL;

		}
	}
	Release();
}

void CRanking::Update(void)
{
		//キーボードの取得
		CInputKeyboard * pInputKeyboard = CManager::GetInputKeyboard();
#define JS
#ifdef JS
	//ジョイパッド
	CInputJoystick * pInputJoystick = CManager::GetInputJoystick();
	LPDIRECTINPUTDEVICE8 lpDIDevice = CInputJoystick::GetDevice();
	DIJOYSTATE js;
	if (lpDIDevice != NULL)
	{
		lpDIDevice->Poll();
		lpDIDevice->GetDeviceState(sizeof(DIJOYSTATE), &js);
	}

	if (lpDIDevice != NULL && pInputJoystick->GetJoystickTrigger(JS_A) || CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))//ボム
	{
		CManager::StartFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0), 0.05f);
		/*CManager::SetMode(CManager::MODE_TITLE);*/
		/*Uninit();*/
		return;
	}
#endif

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apScene2D[nCnt]->Update();

		m_nCountAnim++;
		if (m_nCountAnim % 3)
		{
			m_nPatternAnim++;
			m_nCountAnim = 0;
		}

		if (nCnt == TYPE_001)
		{
			////テクスチャ座標のセット
			//m_apScene2D[nCnt]->SetTex(
			//	0.0f,
			//	-m_nPatternAnim*BG_UV_MOVE,
			//	1.0f,
			//	-m_nPatternAnim*BG_UV_MOVE + BG_UV_MOVE_2);
		}
		else if (nCnt == TYPE_002)
		{
			//テクスチャ座標のセット
			m_apScene2D[nCnt]->SetTex(
				-m_nPatternAnim*BG_UV_MOVE_01_RANKING,
				0.0f,
				-m_nPatternAnim*BG_UV_MOVE_01_RANKING + BG_UV_MOVE_02_RANKING,
				1.0f);
		}
	}

}

void CRanking::Draw(void)
{

}

void CRanking::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

void CRanking::GetRanking(RankingData * pRankingData)
{
	WSADATA wsaData;

	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);
	//何らかの原因で初期化に失敗した場合
	if (nErr != 0)
	{
		/*printf("WSAStartup failed\n");*/
	}

	CTcpClient *pTcpClient = pTcpClient->Create(SERVER_IP_NUM, SERVER_PORT_NUM);

	if (pTcpClient == NULL)
	{
		////ソケット接続をクローズ
		//pTcpClient->Close();
		return;
	}

	char aSendBuf[BUFFER_NUM];
	memset(aSendBuf, 0, sizeof(aSendBuf));
	aSendBuf[0] = COMMAND_TYPE_GET_RANKING;
	//文字列をサーバに送信
	pTcpClient->Send(aSendBuf, sizeof(int));

	////データをサーバから受け取る
	char aRecvBuffer[BUFFER_NUM];
	memset(aRecvBuffer, 0, sizeof(aRecvBuffer));
	pTcpClient->Recv(aRecvBuffer, sizeof(aRecvBuffer));

	int nIndex = 0;
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		int nTime = *(int*)&aRecvBuffer[nIndex];
		nTime = ntohl(nTime);// エンディアン変更
		pRankingData[nCntRank].nScore = nTime;
		nIndex += sizeof(int);
		memcpy(pRankingData[nCntRank].aName, &aRecvBuffer[nIndex], MAX_NAME);
		nIndex += MAX_NAME;
	}
	pTcpClient->Release();
}

int CRanking::SetRanking(int nScore,char aName[MAX_NAME])
{

	WSADATA wsaData;

	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);
	//何らかの原因で初期化に失敗した場合
	if (nErr != 0)
	{
		/*printf("WSAStartup failed\n");*/
	}

	CTcpClient *pTcpClient = pTcpClient->Create(SERVER_IP_NUM, SERVER_PORT_NUM);

	if (pTcpClient == NULL)
	{
		////ソケット接続をクローズ
		//pTcpClient->Close();
		return 0;
	}
	CManager::PlayerData pPlayerData = CManager::GetPlayerData();

	//ランキング設定リクエストを送信
	char aSendBuf[BUFFER_NUM];
	memset(aSendBuf, 0, sizeof(aSendBuf));
	aSendBuf[0] = COMMAND_TYPE_SET_RANKING;
	//クリアタイムを設定
	nScore = htonl(nScore);
	memcpy(&aSendBuf[1], &nScore, sizeof(int));
	//名前せってい
	memcpy(&aSendBuf[5], pPlayerData.aName, MAX_NAME);

	//送信
	pTcpClient->Send(aSendBuf, 13);

	////データをサーバから受け取る
	char aRecvBuffer[BUFFER_NUM];
	memset(aRecvBuffer, 0, sizeof(aRecvBuffer));
	pTcpClient->Recv(aRecvBuffer, sizeof(aRecvBuffer));

	pTcpClient->Release();
	return nScore;
}
