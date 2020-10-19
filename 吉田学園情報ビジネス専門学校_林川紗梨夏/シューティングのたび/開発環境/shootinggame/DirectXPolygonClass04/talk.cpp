//=============================================================================
//
// ��b�̏��� [talk.cpp]
// Author : �ѐ�ї���
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
//
// �w�b�_�t�@�C���̃C���N���[�h
//
#include "main.h"
#include <stdio.h>
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "game.h"
#include "input.h"
#include "joystick.h"
#include "inputkeyboard.h"
#include "talk.h"
#include "text.h"


LPDIRECT3DTEXTURE9 CTalk::m_apTexture[TYPE_MAX] = {};

CTalk::CTalk(int nPriority) : CScene(nPriority)
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	memset(m_apScene2D, 0, sizeof(m_apScene2D));
	memset(m_aText, 0, sizeof(m_aText));
	memset(m_aChara, 0, sizeof(m_aChara));
	memset(m_aPattern, 0, sizeof(m_aPattern));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSizeWidth = 0.0f;
	m_fSizeHeight = 0.0f;
	m_nPatternAnim = 0;
	m_nCountAnim = 0;
	m_bHitUi = false;
	m_chara = CHARA_NONE;
	m_nTalkIndex = 0;
	m_nTalkEndIndex = 0;
	m_nAlpha = 255;
	m_nAddAlpha = 10;
}
CTalk::~CTalk()
{

}

HRESULT CTalk::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_chara001, &m_apTexture[TYPE_001]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_chara002, &m_apTexture[TYPE_002]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_chara003, &m_apTexture[TYPE_003]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, NULL, &m_apTexture[TYPE_004]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TALKUI, &m_apTexture[TYPE_005]);


	return S_OK;
}

void CTalk::Unload(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

CTalk * CTalk::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, int nTalkIndex,int nTalkEndIndex)
{
	CTalk *pTalk;

	pTalk = new CTalk;
	pTalk->Init(pos, fSizeWidth, fSizeHeight);
	pTalk->m_nTalkIndex = nTalkIndex;
	pTalk->m_nTalkEndIndex = nTalkEndIndex;
	return pTalk;
}

HRESULT CTalk::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	SetObjType(CScene::OBJTYPE_NONE);

	m_pos = pos;

	LoadFile();

		//�L�����N�^�[�����G01�ǂ炲

		m_apScene2D[TYPE_001] = new CScene2D(4);
		m_apScene2D[TYPE_001]->Init(D3DXVECTOR3(CHARA001_POS_WIDTH, CHARA_POS_HEIGHT,0.0f), CHARA001_SIZE_WIDTH, CHARA001_SIZE_HEIGHT);
		m_apScene2D[TYPE_001]->BindTexture(m_apTexture[TYPE_001]);

		//�����o�ϐ��ɃT�C�Y��������
		m_fSizeWidth = fSizeWidth;
		m_fSizeHeight = fSizeHeight;

		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(CHARA001_POS_WIDTH + (-CHARA001_SIZE_WIDTH / 2), CHARA_POS_HEIGHT + (-CHARA001_SIZE_HEIGHT / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(CHARA001_POS_WIDTH + (CHARA001_SIZE_WIDTH / 2), CHARA_POS_HEIGHT + (-CHARA001_SIZE_HEIGHT / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(CHARA001_POS_WIDTH + (-CHARA001_SIZE_WIDTH / 2), CHARA_POS_HEIGHT + (CHARA001_SIZE_HEIGHT / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(CHARA001_POS_WIDTH + (CHARA001_SIZE_WIDTH / 2), CHARA_POS_HEIGHT + (CHARA001_SIZE_HEIGHT / 2), 0.0f);

		//���_���W�̃Z�b�g
		m_apScene2D[TYPE_001]->SetVtxPosition(m_vPos);

		//�F�̃Z�b�g
		m_apScene2D[TYPE_001]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));

		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_001]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);


		//�L�����N�^�[�����G01�Ƃ�

		m_apScene2D[TYPE_002] = new CScene2D(4);
		m_apScene2D[TYPE_002]->Init(D3DXVECTOR3(CHARA002_POS_WIDTH, CHARA_POS_HEIGHT, 0.0f), CHARA002_SIZE_WIDTH, CHARA002_SIZE_HEIGHT);
		m_apScene2D[TYPE_002]->BindTexture(m_apTexture[TYPE_002]);

		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(CHARA002_POS_WIDTH + (-CHARA002_SIZE_WIDTH / 2), CHARA_POS_HEIGHT + (-CHARA002_SIZE_HEIGHT / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(CHARA002_POS_WIDTH + (CHARA002_SIZE_WIDTH / 2), CHARA_POS_HEIGHT + (-CHARA002_SIZE_HEIGHT / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(CHARA002_POS_WIDTH + (-CHARA002_SIZE_WIDTH / 2), CHARA_POS_HEIGHT + (CHARA002_SIZE_HEIGHT / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(CHARA002_POS_WIDTH + (CHARA002_SIZE_WIDTH / 2), CHARA_POS_HEIGHT + (CHARA002_SIZE_HEIGHT / 2), 0.0f);

		//���_���W�̃Z�b�g
		m_apScene2D[TYPE_002]->SetVtxPosition(m_vPos);

		//�F�̃Z�b�g
		m_apScene2D[TYPE_002]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));

		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_002]->SetTex(
			m_nPatternAnim * 0.2f,
			0.0f,
			m_nPatternAnim * 0.2f + 0.2f,
			1.0f);

		//�L�����N�^�[�����G01��������

		m_apScene2D[TYPE_003] = new CScene2D(4);
		m_apScene2D[TYPE_003]->Init(D3DXVECTOR3(CHARA003_POS_WIDTH, CHARA_POS_HEIGHT, 0.0f), CHARA003_SIZE_WIDTH, CHARA003_SIZE_HEIGHT);
		m_apScene2D[TYPE_003]->BindTexture(m_apTexture[TYPE_003]);

		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(CHARA003_POS_WIDTH + (-CHARA003_SIZE_WIDTH / 2), CHARA_POS_HEIGHT + (-CHARA003_SIZE_HEIGHT / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(CHARA003_POS_WIDTH + (CHARA003_SIZE_WIDTH / 2), CHARA_POS_HEIGHT + (-CHARA003_SIZE_HEIGHT / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(CHARA003_POS_WIDTH + (-CHARA003_SIZE_WIDTH / 2), CHARA_POS_HEIGHT + (CHARA003_SIZE_HEIGHT / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(CHARA003_POS_WIDTH + (CHARA003_SIZE_WIDTH / 2), CHARA_POS_HEIGHT + (CHARA003_SIZE_HEIGHT / 2), 0.0f);

		//���_���W�̃Z�b�g
		m_apScene2D[TYPE_003]->SetVtxPosition(m_vPos);

		//�F�̃Z�b�g
		m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));

		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_003]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);

		//���b�Z�[�W�E�B���h�E

		m_apScene2D[TYPE_004] = new CScene2D(4);
		m_apScene2D[TYPE_004]->Init(D3DXVECTOR3(FIELD_WIDTH / 2, 900.0f, 0.0f), WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);
		m_apScene2D[TYPE_004]->BindTexture(m_apTexture[TYPE_004]);

		//�����o�ϐ��ɃT�C�Y��������
		m_fSizeWidth = fSizeWidth;
		m_fSizeHeight = fSizeHeight;

		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(FIELD_WIDTH / 2 + 50.0f + (-WINDOW_SIZE_WIDTH / 2), 900.0f + (-WINDOW_SIZE_HEIGHT / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(FIELD_WIDTH / 2 + 50.0f + (WINDOW_SIZE_WIDTH / 2), 900.0f + (-WINDOW_SIZE_HEIGHT / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(FIELD_WIDTH / 2 + 50.0f + (-WINDOW_SIZE_WIDTH / 2), 900.0f + (WINDOW_SIZE_HEIGHT / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(FIELD_WIDTH / 2 + 50.0f + (WINDOW_SIZE_WIDTH / 2), 900.0f + (WINDOW_SIZE_HEIGHT / 2), 0.0f);

		//���_���W�̃Z�b�g
		m_apScene2D[TYPE_004]->SetVtxPosition(m_vPos);

		//�F�̃Z�b�g
		m_apScene2D[TYPE_004]->SetColor(D3DCOLOR_RGBA(0, 0, 0, 100));

		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_004]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);

		//���b�Z�[�W�E�B���h�E

		m_apScene2D[TYPE_005] = new CScene2D(4);
		m_apScene2D[TYPE_005]->Init(D3DXVECTOR3(FIELD_WIDTH / 2, 900.0f, 0.0f), TALKUI_SIZE_WIDTH, TALKUI_SIZE_HEIGHT);
		m_apScene2D[TYPE_005]->BindTexture(m_apTexture[TYPE_005]);

		//�����o�ϐ��ɃT�C�Y��������
		m_fSizeWidth = fSizeWidth;
		m_fSizeHeight = fSizeHeight;

		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(FIELD_WIDTH / 2 + 450.0f + (-TALKUI_SIZE_WIDTH / 2), 900.0f + (-TALKUI_SIZE_HEIGHT / 2) + 50.0f, 0.0f);
		m_vPos[1] = D3DXVECTOR3(FIELD_WIDTH / 2 + 450.0f + (TALKUI_SIZE_WIDTH / 2), 900.0f + (-TALKUI_SIZE_HEIGHT / 2) + 50.0f, 0.0f);
		m_vPos[2] = D3DXVECTOR3(FIELD_WIDTH / 2 + 450.0f + (-TALKUI_SIZE_WIDTH / 2), 900.0f + (TALKUI_SIZE_HEIGHT / 2) + 50.0f, 0.0f);
		m_vPos[3] = D3DXVECTOR3(FIELD_WIDTH / 2 + 450.0f + (TALKUI_SIZE_WIDTH / 2), 900.0f + (TALKUI_SIZE_HEIGHT / 2) + 50.0f, 0.0f);

		//���_���W�̃Z�b�g
		m_apScene2D[TYPE_005]->SetVtxPosition(m_vPos);

		//�F�̃Z�b�g
		m_apScene2D[TYPE_005]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_005]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);

	return S_OK;
}

void CTalk::Uninit(void)
{
	for (int nCntNum = 0; nCntNum < 5; nCntNum++)
	{
		if (m_apScene2D[nCntNum] != NULL)
		{
			m_apScene2D[nCntNum]->Uninit();
		}
	}
	Release();
}

void CTalk::Update(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard * pInputKeyboard = CManager::GetInputKeyboard();
	//�W���C�p�b�h
	CInputJoystick * pInputJoystick = CManager::GetInputJoystick();
	LPDIRECTINPUTDEVICE8 lpDIDevice = CInputJoystick::GetDevice();
	DIJOYSTATE js;

	CText * pText = CManager::GetText();
	if (lpDIDevice != NULL)
	{
		lpDIDevice->Poll();
		lpDIDevice->GetDeviceState(sizeof(DIJOYSTATE), &js);
	}

	if (lpDIDevice != NULL && pInputJoystick->GetJoystickTrigger(JS_A) || pInputKeyboard->GetKeyboardTrigger(DIK_SPACE))//��b����
	{
		m_nTalkIndex++;
	}

	//��b���̃L�����N�^�[�ȊO�͓��߂�����
	for (int nCount = 0; nCount < CHARA_MAX; nCount++)
	{
		m_apScene2D[nCount]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
	}

	//��b���̃L�����N�^�[��s�����ɂ����ĕ\����ύX������
	switch (m_aChara[m_nTalkIndex])
	{
	case CHARA_001:
		//�F�̃Z�b�g
		m_apScene2D[TYPE_001]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_001]->SetTex(
			m_aPattern[m_nTalkIndex] * 0.3333f,
			0.0f,
			m_aPattern[m_nTalkIndex] * 0.3333f + 0.3333f,
			1.0f);
		break;
	case CHARA_002:
		//�F�̃Z�b�g
		m_apScene2D[TYPE_002]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_002]->SetTex(
			m_aPattern[m_nTalkIndex] * 0.25f,
			0.0f,
			m_aPattern[m_nTalkIndex] * 0.25f + 0.25f,
			1.0f);
		break;
	case CHARA_003:
		//�F�̃Z�b�g
		m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_003]->SetTex(
			m_aPattern[m_nTalkIndex] * 0.25f,
			0.0f,
			m_aPattern[m_nTalkIndex] * 0.25f + 0.25f,
			1.0f);
		break;
	default:
		//��b���̃L�����N�^�[�ȊO�͓��߂�����
		for (int nCount = 0; nCount < CHARA_MAX; nCount++)
		{
			m_apScene2D[nCount]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
		}
		break;
	}

	//�{�^����UI�_�ŏ���
	if (m_nAlpha >= 255)
	{
		m_nAddAlpha = m_nAddAlpha * -1;
	}
	else if (m_nAlpha <= 100)
	{
		m_nAddAlpha = m_nAddAlpha * -1;
	}
	m_nAlpha += m_nAddAlpha;
	m_apScene2D[TYPE_005]->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
	//�e�L�X�g�̃Z�b�g
	pText->SetText(&m_aText[m_nTalkIndex][0]);

	if (m_nTalkIndex > m_nTalkEndIndex)//�I����uninit
	{
	/*	��b���̃L�����N�^�[�ȊO�͓��߂�����
		for (int nCount = 0; nCount < TYPE_MAX; nCount++)
		{
			m_apScene2D[nCount]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
		}*/
		char str[1];
		wsprintf(str, "\0");
		pText->SetText(str);
		CManager::SetIsTalking(false);
		Uninit();
		return;
	}
}

void CTalk::Draw(void)
{
}

void CTalk::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

void CTalk::SetChara(CTYPE type)
{
	m_chara = type;
}
//=============================================================================
// ��b�t�@�C���ǂݍ���
//=============================================================================
void CTalk::LoadFile(void)
{
	FILE * pFile = fopen("data/talk.txt", "r");

	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_TEXT; nCnt++)
		{
			
			fscanf(pFile, "%d", &m_aChara[nCnt]);
			fscanf(pFile, "%d", &m_aPattern[nCnt]);
			fscanf(pFile, "%s", &m_aText[nCnt]);
		}
		fclose(pFile);
	}//���z��i�[����
	else
	{
		printf("���s");
	}//�G���[�̏ꍇ
}
