//=============================================================================
//
// �O���C�Y���� [graze.cpp]
// Author : �ѐ�ї���
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "graze.h"
#include <cmath>


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGraze::CGraze(int nPriority) : CScene(nPriority)
{
	memset(m_apNumber, 0, sizeof(m_apNumber));
	m_nGraze = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGraze::~CGraze()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGraze::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	SetObjType(CScene::OBJTYPE_NONE);
	for (int nCntNum = 0; nCntNum < MAX_DIGITS; nCntNum++)
	{
		m_apNumber[nCntNum] = CNumber::Create(D3DXVECTOR3(pos.x - (fSizeWidth * nCntNum), pos.y, 0.0f), fSizeWidth, fSizeHeight);
	}
	m_nGraze = 0;
	CScene::SetPosition(pos);
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CGraze::Uninit(void)
{
	
	for (int nCntNum = 0; nCntNum < MAX_DIGITS; nCntNum++)
	{
		m_apNumber[nCntNum]->Uninit();
		delete m_apNumber[nCntNum];
		m_apNumber[nCntNum] = NULL;

	}
	
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CGraze::Update(void)
{
	CManager::SetGraze((int)m_nGraze);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CGraze::Draw(void)
{
	for (int nCntNum = 0; nCntNum < MAX_DIGITS; nCntNum++)
	{
		m_apNumber[nCntNum]->Draw();
	}
}
//=============================================================================
// ��������
//=============================================================================
CGraze * CGraze::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CGraze *pGraze;

	pGraze = new CGraze;
	pGraze->Init(pos, fSizeWidth, fSizeHeight);

	return pGraze;
}
//=============================================================================
// �O���C�Y���\��
//=============================================================================
void CGraze::SetGraze(float nGraze)
{
	m_nGraze = nGraze;
	int integer = 0;//�v�Z�p�ϐ�
	int nCntNumber = 0;

	for (int nCntDigit = 1; nCntDigit <= MAX_DIGITS; nCntDigit++, nCntNumber++)
	{

		integer = int(m_nGraze / std::pow(10, nCntDigit)); //1�̈ʈȉ���؂�̂Ă�
		integer = int(integer * std::pow(10, nCntDigit));
		m_apNumber[nCntNumber]->SetNumber(int((m_nGraze - integer) / std::pow(10.0, nCntDigit - 1)));
	}

}
//=============================================================================
// �O���C�Y���Z����
//=============================================================================
void CGraze::AddGraze(float nValue)
{
	m_nGraze += nValue;
	SetGraze(m_nGraze);
}
