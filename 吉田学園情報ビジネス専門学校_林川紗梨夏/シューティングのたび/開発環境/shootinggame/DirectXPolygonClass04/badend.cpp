//=============================================================================
//
// ED��ʂ̏��� [badend.cpp]
// Author : �ѐ�ї���
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "badend.h"
#include "input.h"
#include "joystick.h"
#include"inputkeyboard.h"
#include "ranking.h"
#include "talk.h"


LPDIRECT3DTEXTURE9 CBadend::m_apTexture[TYPE_MAX] = {};
CBadend::CBadend(int nPriority)
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
CBadend::~CBadend()
{

}

HRESULT CBadend::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BADEND, &m_apTexture[TYPE_001]);

	return S_OK;
}

void CBadend::Unload(void)
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

CBadend * CBadend::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CBadend *pResult;

	pResult = new CBadend;
	pResult->Init(pos, fSizeWidth, fSizeHeight);

	return pResult;
}

HRESULT CBadend::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
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

	return S_OK;
}

void CBadend::Uninit(void)
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

void CBadend::Update(void)
{

	//	//�L�[�{�[�h�̎擾
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

	if (m_nCountAnim == 100)
	{
		CTalk::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 85.0f, 0.0f), 40, 70, 56, 60);
		CManager::SetIsTalking(true);
	}
	if (m_nCountAnim > 100 && CManager::GetIsTalking() == false)
	{
		if (lpDIDevice != NULL && pInputJoystick->GetJoystickTrigger(JS_A) || CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))//�{��
		{
			CManager::StartFade(CManager::MODE_NAME, D3DCOLOR_RGBA(0, 0, 0, 0), 0.05f);
			/*CManager::SetMode(CManager::MODE_TITLE);*/
			/*Uninit();*/
			return;
		}
	}

#endif
	m_nCountAnim++;
}

void CBadend::Draw(void)
{
	//CRenderer * pRenderer = CManager::GetRenderer();
	//LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//// Direct3D�ɂ��`��̊J�n
	//if (SUCCEEDED(pDevice->BeginScene()))
	//{
	/*m_pText->DrawResult();*/
	//// Direct3D�ɂ��`��̏I��
	//pDevice->EndScene();
	//}
	//// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	//pDevice->Present(NULL, NULL, NULL, NULL);
}

void CBadend::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
