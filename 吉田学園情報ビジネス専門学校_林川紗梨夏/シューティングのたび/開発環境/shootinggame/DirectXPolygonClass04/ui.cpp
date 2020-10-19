//=============================================================================
//
// �w�i�̏��� [ui.cpp]
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
#include "ui.h"


LPDIRECT3DTEXTURE9 CUi::m_apTexture[MAX_UI] = {};

CUi::CUi(int nPriority) : CScene(nPriority)
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
CUi::~CUi()
{

}

HRESULT CUi::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_UI001, &m_apTexture[0]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_UI001, &m_apTexture[1]);

	return S_OK;
}

void CUi::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

CUi * CUi::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CUi *pUi;

	pUi = new CUi;
	pUi->Init(pos, fSizeWidth, fSizeHeight);

	return pUi;
}

HRESULT CUi::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	SetObjType(CScene::OBJTYPE_NONE);

	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		m_apScene2D[nCnt] = new CScene2D(5);
		m_apScene2D[nCnt]->Init(pos, fSizeWidth, fSizeHeight);
		m_apScene2D[nCnt]->BindTexture(m_apTexture[nCnt]);

		//�����o�ϐ��ɃT�C�Y��������
		m_fSizeWidth = fSizeWidth;
		m_fSizeHeight = fSizeHeight;

		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);

		//���_���W�̃Z�b�g
		m_apScene2D[nCnt]->SetVtxPosition(m_vPos);

		//�F�̃Z�b�g
		m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[nCnt]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
	}

	return S_OK;
}

void CUi::Uninit(void)
{
	Release();
}

void CUi::Update(void)
{
	
}

void CUi::Draw(void)
{

}

void CUi::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
