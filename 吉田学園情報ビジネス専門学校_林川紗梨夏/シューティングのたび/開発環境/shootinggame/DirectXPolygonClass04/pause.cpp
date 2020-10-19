//=============================================================================
//
// �|�[�Y����[pause.cpp]
// Author : �ѐ�ї���
//
//=============================================================================
#include "main.h"
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "pause.h"
#include "input.h"
#include "joystick.h"
#include "game.h"
#include "fade.h"
#include "sound.h"
#include "inputkeyboard.h"

#define COUNT_TRIGGER_PAUSE 4

LPDIRECT3DTEXTURE9 CPause::m_pTexture[TYPE_MAX] = {};
CPause::CPause()
{
	m_pVtxBuff = NULL;
	m_nButtonPos = 1;
	m_nPatternAnim = 1;
	memset(m_aButtonAlpha, 0, sizeof(m_aButtonAlpha));

	m_IsTrigger.Down.bTrigger = false;//������Ă邩�ǂ���
	m_IsTrigger.Up.bTrigger = false;
	m_IsTrigger.Left.bTrigger = false;
	m_IsTrigger.Right.bTrigger = false;
	m_IsTrigger.Down.nCount = 0;//������Ă邩�ǂ���
	m_IsTrigger.Up.nCount = 0;
	m_IsTrigger.Left.nCount = 0;
	m_IsTrigger.Right.nCount = 0;
	m_TriggerCount = 0;//�������̃J�E���^
}

CPause::~CPause()
{
}

CPause * CPause::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CPause *pPause;

	pPause = new CPause;
	pPause->Init(pos, fSizeWidth, fSizeHeight);

	return pPause;
}

HRESULT CPause::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PAUSE, &m_pTexture[TYPE_BG]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BUTTON_BACK, &m_pTexture[TYPE_BUTTON_001]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BUTTON_RESTART, &m_pTexture[TYPE_BUTTON_002]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BUTTON_EXIT, &m_pTexture[TYPE_BUTTON_003]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PAUSE_UI, &m_pTexture[TYPE_004]);
	return S_OK;
}

void CPause::Unload(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

HRESULT CPause::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * TYPE_MAX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&m_pVtxBuff,				// ���_�o�b�t�@�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	//BG
	pVtx[0].pos = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//�Q�[���ɖ߂�{�^��
	pVtx[0].pos = D3DXVECTOR3(pos.x + (-300 / 2), 500.0f + (-100 / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + (300 / 2), 500.0f + (-100 / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x + (-300 / 2), 500.0f + (100 / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + (300 / 2), 500.0f + (100 / 2), 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//�ŏ�����{�^��
	pVtx[0].pos = D3DXVECTOR3(pos.x + (-300 / 2), 550.0f + (-100 / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + (300 / 2), 550.0f + (-100 / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x + (-300 / 2), 550.0f + (100 / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + (300 / 2), 550.0f + (100 / 2), 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//�^�C�g���ɖ߂�{�^��
	pVtx[0].pos = D3DXVECTOR3(pos.x + (-300 / 2), 600.0f + (-100 / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + (300 / 2), 600.0f + (-100 / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x + (-300 / 2), 600.0f + (100 / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + (300 / 2), 600.0f + (100 / 2), 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//UI
	//�^�C�g���ɖ߂�{�^��
	pVtx[0].pos = D3DXVECTOR3(pos.x + (-500 / 2), 400.0f + (-100 / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + (500 / 2), 400.0f + (-100 / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x + (-500 / 2), 400.0f + (100 / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + (500 / 2), 400.0f + (100 / 2), 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff->Unlock();
	return S_OK;
}

void CPause::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

void CPause::Update(void)
{
		//�L�[�{�[�h�̎擾
		CInputKeyboard * pInputKeyboard = CManager::GetInputKeyboard();
	//�T�E���h
	CSound * pSound = CManager::GetSound();
	if (CGame::GetIsStopUpdate() == true)
	{
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

		if (lpDIDevice != NULL &&js.lY == -1000 || pInputKeyboard->GetKeyboardPress(DIK_W))//��
		{
			m_IsTrigger.Up.nCount++;
		}
		if (lpDIDevice != NULL &&js.lY == 1000 || pInputKeyboard->GetKeyboardPress(DIK_S))//��
		{
			m_IsTrigger.Down.nCount++;
		}

		if (m_IsTrigger.Up.nCount > COUNT_TRIGGER_PAUSE)
		{
			m_IsTrigger.Up.bTrigger = true;
			m_IsTrigger.Up.nCount = COUNT_TRIGGER_PAUSE - 5;
		}
		if (m_IsTrigger.Down.nCount > COUNT_TRIGGER_PAUSE)
		{
			m_IsTrigger.Down.bTrigger = true;
			m_IsTrigger.Down.nCount = COUNT_TRIGGER_PAUSE - 5;
		}

#endif

		//��������󂪉�����炽��
		if (m_IsTrigger.Down.bTrigger == true)
		{
	
			m_IsTrigger.Down.bTrigger = false;
			if (m_nButtonPos < 3)
			{
				//�V���b�g���̍Đ�
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SELECT);
				m_nButtonPos++;
			}
		}

		//�������󂪉������
		if (m_IsTrigger.Up.bTrigger == true)
		{
	
			m_IsTrigger.Up.bTrigger = false;
			if (m_nButtonPos > 1)
			{
				//�V���b�g���̍Đ�
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SELECT);
				m_nButtonPos--;
			}
		}

		if (lpDIDevice != NULL && pInputJoystick->GetJoystickTrigger(JS_A) || CInputKeyboard::GetKeyboardTrigger(DIK_SPACE))//����
		{
			//�V���b�g���̍Đ�
			pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_ENTER);
			switch (m_nButtonPos)
			{
			case 1://�Q�[���߂�{�^��
				CGame::SetIsStopUpdate(false);
				m_nButtonPos = 1;
				break;
			case 2://�ŏ�����{�^��
				CManager::StartFade(CManager::MODE_GAME, D3DCOLOR_RGBA(0, 0, 0, 0), 0.05f);
				CGame::SetIsStopUpdate(false);
				m_nButtonPos = 1;
				break;
			case 3://�^�C�g���ɂ��ǂ�
				CManager::StartFade(CManager::MODE_TITLE, D3DCOLOR_RGBA(0, 0, 0, 0), 0.05f);
				CGame::SetIsStopUpdate(false);
				m_nButtonPos = 1;
				break;
			default:
				break;
			}
		}

		for (int nCnt = 1; nCnt <= 3; nCnt++)
		{
			m_aButtonAlpha[nCnt] = 100;
		}
		switch (m_nButtonPos)
		{
		case 1://�Q�[���߂�{�^��
			m_aButtonAlpha[TYPE_BUTTON_001] = 255;
			break;
		case 2://�ŏ�����{�^��
			m_aButtonAlpha[TYPE_BUTTON_002] = 255;
			break;
		case 3://�^�C�g���ɂ��ǂ�
			m_aButtonAlpha[TYPE_BUTTON_003] = 255;
			break;
		default:
			break;
		}

		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4;

		//�Q�[���ɖ߂�{�^��
		pVtx[0].pos = D3DXVECTOR3(SCREEN_CENTER_X + (-300 / 2), 500.0f + (-100 / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_CENTER_X + (300 / 2), 500.0f + (-100 / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_CENTER_X + (-300 / 2), 500.0f + (100 / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_CENTER_X + (300 / 2), 500.0f + (100 / 2), 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, m_aButtonAlpha[TYPE_BUTTON_001]);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, m_aButtonAlpha[TYPE_BUTTON_001]);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, m_aButtonAlpha[TYPE_BUTTON_001]);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, m_aButtonAlpha[TYPE_BUTTON_001]);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;

		//�ŏ�����{�^��
		pVtx[0].pos = D3DXVECTOR3(SCREEN_CENTER_X + (-300 / 2), 550.0f + (-100 / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_CENTER_X + (300 / 2), 550.0f + (-100 / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_CENTER_X + (-300 / 2), 550.0f + (100 / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_CENTER_X + (300 / 2), 550.0f + (100 / 2), 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, m_aButtonAlpha[TYPE_BUTTON_002]);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, m_aButtonAlpha[TYPE_BUTTON_002]);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, m_aButtonAlpha[TYPE_BUTTON_002]);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, m_aButtonAlpha[TYPE_BUTTON_002]);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;

		//�^�C�g���ɖ߂�{�^��
		pVtx[0].pos = D3DXVECTOR3(SCREEN_CENTER_X + (-300 / 2), 600.0f + (-100 / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_CENTER_X + (300 / 2), 600.0f + (-100 / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_CENTER_X + (-300 / 2), 600.0f + (100 / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_CENTER_X + (300 / 2), 600.0f + (100 / 2), 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, m_aButtonAlpha[TYPE_BUTTON_003]);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, m_aButtonAlpha[TYPE_BUTTON_003]);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, m_aButtonAlpha[TYPE_BUTTON_003]);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, m_aButtonAlpha[TYPE_BUTTON_003]);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		m_pVtxBuff->Unlock();
	}
}

void CPause::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		pDevice->SetTexture(0, m_pTexture[nCnt]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCnt * 4), NUM_POLYGON);
	}
}
