//=============================================================================
//
// ���U���g��ʂ̏��� [result.cpp]
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
#include "result.h"
#include "input.h"
#include "joystick.h"
#include"inputkeyboard.h"
#include "ranking.h"


LPDIRECT3DTEXTURE9 CResult::m_apTexture[MAX_BG] = {};
CResult::CResult(int nPriority)
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
	m_pPlayerData = { 0,0,0,0 };
}
CResult::~CResult()
{

}

HRESULT CResult::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT, &m_apTexture[TYPE_001]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT_2, &m_apTexture[TYPE_002]);

	return S_OK;
}

void CResult::Unload(void)
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

CResult * CResult::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CResult *pResult;

	pResult = new CResult;
	pResult->Init(pos, fSizeWidth, fSizeHeight);

	return pResult;
}

HRESULT CResult::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{

	//�����o�ϐ��ɃT�C�Y��������
	m_fSizeWidth = fSizeWidth;
	m_fSizeHeight = fSizeHeight;

		m_apScene2D[TYPE_001] = new CScene2D;
		m_apScene2D[TYPE_001]->Init(pos, fSizeWidth, fSizeHeight);
		m_apScene2D[TYPE_001]->BindTexture(m_apTexture[TYPE_001]);

		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y  + (-fSizeHeight / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y  + (-fSizeHeight / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y  + (fSizeHeight / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y  + (fSizeHeight / 2), 0.0f);

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
	m_apScene2D[TYPE_002]->Init(pos, fSizeWidth, RESULT02_SIZE_HEIGHT);
	m_apScene2D[TYPE_002]->BindTexture(m_apTexture[TYPE_002]);

	// ���_���W��ݒ�
	m_vPos[0] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y - 300.0f + (-RESULT02_SIZE_HEIGHT / 2), 0.0f);
	m_vPos[1] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y - 300.0f + (-RESULT02_SIZE_HEIGHT / 2), 0.0f);
	m_vPos[2] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y - 300.0f + (RESULT02_SIZE_HEIGHT / 2), 0.0f);
	m_vPos[3] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y - 300.0f + (RESULT02_SIZE_HEIGHT / 2), 0.0f);

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

	//�v���C���[�f�[�^���擾
	m_pPlayerData = CManager::GetPlayerData();

	m_pPlayerData.nScore += m_pPlayerData.nBomb * BOMB_BONUS;//�{���{�[�i�X���X�R�A�ɉ��Z
	m_pPlayerData.nScore += m_pPlayerData.nLife * LIFE_BONUS;//���C�t�{�[�i�X���X�R�A�ɉ��Z
	m_pPlayerData.nScore += m_pPlayerData.nGraze * GRAZE_BONUS;//GRAZE�{�[�i�X���X�R�A�ɉ��Z

	CManager::SetScore(m_pPlayerData.nScore);//�X�R�A�f�[�^��manager�ɓn��

	return S_OK;
}

void CResult::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt] = NULL;

		}
	}

	Release();
}

void CResult::Update(void)
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

	if (lpDIDevice != NULL && pInputJoystick->GetJoystickTrigger(JS_A) || CInputKeyboard::GetKeyboardTrigger(DIK_SPACE))//�{��
	{
		CManager::StartFade(CManager::MODE_RANKING, D3DCOLOR_RGBA(0, 0, 0, 0), 0.05f);
		return;
	}
#endif

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
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
		}
		else if (nCnt == TYPE_002)
		{
			//�e�N�X�`�����W�̃Z�b�g
			m_apScene2D[nCnt]->SetTex(
				-m_nPatternAnim*BG_UV_MOVE_01,
				0.0f,
				-m_nPatternAnim*BG_UV_MOVE_01 + BG_UV_MOVE_2,
				1.0f);
		}
	}
}

void CResult::Draw(void)
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

void CResult::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
