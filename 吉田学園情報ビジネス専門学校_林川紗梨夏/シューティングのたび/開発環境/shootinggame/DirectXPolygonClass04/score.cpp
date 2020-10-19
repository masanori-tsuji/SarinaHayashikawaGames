//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : �ѐ�ї���
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "scene.h"

#include "score.h"
#include <cmath>


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScore::CScore(int nPriority) : CScene(nPriority)
{
	memset(m_apNumber, 0, sizeof(m_apNumber));
	m_nScore = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScore::~CScore()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	SetObjType(CScene::OBJTYPE_NONE);
	for (int nCntNum = 0; nCntNum < MAX_DIGITS; nCntNum++)
	{
		m_apNumber[nCntNum] = CNumber::Create(D3DXVECTOR3(pos.x - (fSizeWidth * nCntNum), pos.y, 0.0f), fSizeWidth, fSizeHeight);
	}
	m_nScore = 0;
	CScene::SetPosition(pos);
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntNum = 0; nCntNum < MAX_DIGITS; nCntNum++)
	{
		m_apNumber[nCntNum]->Uninit();
		delete m_apNumber[nCntNum];
		m_apNumber[nCntNum] = NULL;
		
	}
	Release();
}
void CScore::Update(void)
{
	CManager::SetScore(m_nScore);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntNum = 0; nCntNum < MAX_DIGITS; nCntNum++)
	{
		m_apNumber[nCntNum]->Draw();
	}
}
//=============================================================================
// ��������
//=============================================================================
CScore * CScore::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CScore *pScore;

	pScore = new CScore;
	pScore->Init(pos, fSizeWidth, fSizeHeight);

	return pScore;
}
//=============================================================================
// �X�R�A�Z�b�g
//=============================================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;
	int integer = 0;//�v�Z�p�ϐ�
	int nCntNumber = 0;
	
	for (int nCntDigit = 1; nCntDigit <= MAX_DIGITS; nCntDigit++,nCntNumber++)
	{

		integer = int(m_nScore / std::pow(10,nCntDigit)); //1�̈ʈȉ���؂�̂Ă�
		integer = int(integer * std::pow(10, nCntDigit));
		m_apNumber[nCntNumber]->SetNumber(int((m_nScore - integer) / std::pow(10.0, nCntDigit - 1)));
	}

}
//=============================================================================
// �X�R�A���Z
//=============================================================================
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;
	SetScore(m_nScore);
}
