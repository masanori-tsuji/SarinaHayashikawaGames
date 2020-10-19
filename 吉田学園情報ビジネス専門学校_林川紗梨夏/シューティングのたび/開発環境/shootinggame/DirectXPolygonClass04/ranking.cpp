//=============================================================================
//
// �����L���O��ʂ̏��� [ranking.cpp]
// Author : �ѐ�ї���
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
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
CRanking::RankingData CRanking::m_aRankingData[MAX_RANKING] = {};//�����L���O�f�[�^
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
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BG_RANKING, &m_apTexture[TYPE_001]);
	// �e�N�X�`���̐���
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
	//�����o�ϐ��ɃT�C�Y��������
	m_fSizeWidth = fSizeWidth;
	m_fSizeHeight = fSizeHeight;

	m_apScene2D[TYPE_001] = new CScene2D;
	m_apScene2D[TYPE_001]->Init(pos, fSizeWidth, fSizeHeight);
	m_apScene2D[TYPE_001]->BindTexture(m_apTexture[TYPE_001]);

	// ���_���W��ݒ�
	m_vPos[0] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
	m_vPos[1] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
	m_vPos[2] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);
	m_vPos[3] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);

	//���_���W�̃Z�b�g
	m_apScene2D[TYPE_001]->SetVtxPosition(m_vPos);

	//�F�̃Z�b�g
	m_apScene2D[TYPE_001]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	//�e�N�X�`�����W�̃Z�b�g
	m_apScene2D[TYPE_001]->SetTex(
		0.0f,
		0.0f,
		1.0f,
		1.0f);

	m_apScene2D[TYPE_002] = new CScene2D;
	m_apScene2D[TYPE_002]->Init(pos, fSizeWidth, RANKING_02_SIZE_HEIGHT);
	m_apScene2D[TYPE_002]->BindTexture(m_apTexture[TYPE_002]);

	// ���_���W��ݒ�
	m_vPos[0] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y - 300.0f + (-RANKING_02_SIZE_HEIGHT / 2), 0.0f);
	m_vPos[1] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y - 300.0f + (-RANKING_02_SIZE_HEIGHT / 2), 0.0f);
	m_vPos[2] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y - 300.0f + (RANKING_02_SIZE_HEIGHT / 2), 0.0f);
	m_vPos[3] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y - 300.0f + (RANKING_02_SIZE_HEIGHT / 2), 0.0f);

	//���_���W�̃Z�b�g
	m_apScene2D[TYPE_002]->SetVtxPosition(m_vPos);

	//�F�̃Z�b�g
	m_apScene2D[TYPE_002]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	//�e�N�X�`�����W�̃Z�b�g
	m_apScene2D[TYPE_002]->SetTex(
		0.0f,
		0.0f,
		1.0f,
		1.0f);


	CManager::PlayerData pPlayerData = CManager::GetPlayerData();
	
	SetRanking(pPlayerData.nScore, pPlayerData.aName);//�����L���O���M���N�G�X�g
	GetRanking(m_aRankingData);//�����L���O���M���N�G�X�g
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
		//�L�[�{�[�h�̎擾
		CInputKeyboard * pInputKeyboard = CManager::GetInputKeyboard();
#define JS
#ifdef JS
	//�W���C�p�b�h
	CInputJoystick * pInputJoystick = CManager::GetInputJoystick();
	LPDIRECTINPUTDEVICE8 lpDIDevice = CInputJoystick::GetDevice();
	DIJOYSTATE js;
	if (lpDIDevice != NULL)
	{
		lpDIDevice->Poll();
		lpDIDevice->GetDeviceState(sizeof(DIJOYSTATE), &js);
	}

	if (lpDIDevice != NULL && pInputJoystick->GetJoystickTrigger(JS_A) || CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))//�{��
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
			////�e�N�X�`�����W�̃Z�b�g
			//m_apScene2D[nCnt]->SetTex(
			//	0.0f,
			//	-m_nPatternAnim*BG_UV_MOVE,
			//	1.0f,
			//	-m_nPatternAnim*BG_UV_MOVE + BG_UV_MOVE_2);
		}
		else if (nCnt == TYPE_002)
		{
			//�e�N�X�`�����W�̃Z�b�g
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
	//���炩�̌����ŏ������Ɏ��s�����ꍇ
	if (nErr != 0)
	{
		/*printf("WSAStartup failed\n");*/
	}

	CTcpClient *pTcpClient = pTcpClient->Create(SERVER_IP_NUM, SERVER_PORT_NUM);

	if (pTcpClient == NULL)
	{
		////�\�P�b�g�ڑ����N���[�Y
		//pTcpClient->Close();
		return;
	}

	char aSendBuf[BUFFER_NUM];
	memset(aSendBuf, 0, sizeof(aSendBuf));
	aSendBuf[0] = COMMAND_TYPE_GET_RANKING;
	//��������T�[�o�ɑ��M
	pTcpClient->Send(aSendBuf, sizeof(int));

	////�f�[�^���T�[�o����󂯎��
	char aRecvBuffer[BUFFER_NUM];
	memset(aRecvBuffer, 0, sizeof(aRecvBuffer));
	pTcpClient->Recv(aRecvBuffer, sizeof(aRecvBuffer));

	int nIndex = 0;
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		int nTime = *(int*)&aRecvBuffer[nIndex];
		nTime = ntohl(nTime);// �G���f�B�A���ύX
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
	//���炩�̌����ŏ������Ɏ��s�����ꍇ
	if (nErr != 0)
	{
		/*printf("WSAStartup failed\n");*/
	}

	CTcpClient *pTcpClient = pTcpClient->Create(SERVER_IP_NUM, SERVER_PORT_NUM);

	if (pTcpClient == NULL)
	{
		////�\�P�b�g�ڑ����N���[�Y
		//pTcpClient->Close();
		return 0;
	}
	CManager::PlayerData pPlayerData = CManager::GetPlayerData();

	//�����L���O�ݒ胊�N�G�X�g�𑗐M
	char aSendBuf[BUFFER_NUM];
	memset(aSendBuf, 0, sizeof(aSendBuf));
	aSendBuf[0] = COMMAND_TYPE_SET_RANKING;
	//�N���A�^�C����ݒ�
	nScore = htonl(nScore);
	memcpy(&aSendBuf[1], &nScore, sizeof(int));
	//���O�����Ă�
	memcpy(&aSendBuf[5], pPlayerData.aName, MAX_NAME);

	//���M
	pTcpClient->Send(aSendBuf, 13);

	////�f�[�^���T�[�o����󂯎��
	char aRecvBuffer[BUFFER_NUM];
	memset(aRecvBuffer, 0, sizeof(aRecvBuffer));
	pTcpClient->Recv(aRecvBuffer, sizeof(aRecvBuffer));

	pTcpClient->Release();
	return nScore;
}
