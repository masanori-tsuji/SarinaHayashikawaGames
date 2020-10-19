//=============================================================================
//
// ���C�t���� [life.cpp]
// Author : �ѐ�ї���
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "life.h"
#include "score.h"
#include <cmath>
#include "fade.h"
#include "msgwindow.h"
#include "game.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLife::CLife(int nPriority) : CScene(nPriority)
{
	memset(m_apStar, 0, sizeof(m_apStar));
	m_nLife = PLAYER_LIFE;
	m_fSizeWidth = 0;
	m_fSizeHeight = 0;
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLife::~CLife()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLife::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	SetObjType(CScene::OBJTYPE_NONE);

	m_fSizeWidth = fSizeWidth;
	m_fSizeHeight = fSizeHeight;
	m_Pos = pos;
	for (int nCntNum = 0; nCntNum < MAX_STAR; nCntNum++)
	{
		m_apStar[nCntNum] = CStar::Create(D3DXVECTOR3(pos.x + (fSizeWidth * nCntNum), pos.y, 0.0f), fSizeWidth, fSizeHeight);
	}
	SetLife(m_nLife);
	CScene::SetPosition(pos);
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CLife::Uninit(void)
{
	for (int nCntNum = 0; nCntNum < MAX_STAR; nCntNum++)
	{
		if (m_apStar[nCntNum] != NULL)
		{
			m_apStar[nCntNum]->Uninit();
			delete m_apStar[nCntNum];
			m_apStar[nCntNum] = NULL;
		}
	}
	
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CLife::Update(void)
{
	CManager::SetLife(m_nLife);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CLife::Draw(void)
{
	for (int nCntNum = 0; nCntNum < MAX_STAR; nCntNum++)
	{
		if (m_apStar[nCntNum] != NULL)
		{
			m_apStar[nCntNum]->Draw();
		}
	}
}
//=============================================================================
// ��������
//=============================================================================
CLife * CLife::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CLife *pScore;

	pScore = new CLife;
	pScore->Init(pos, fSizeWidth, fSizeHeight);

	return pScore;
}
//=============================================================================
// ���C�t�̃Z�b�g����
//=============================================================================
void CLife::SetLife(int nLife)
{
	m_nLife = nLife;

	if (m_nLife <= MAX_STAR)
	{
		for (int nCntStar = m_nLife; nCntStar < MAX_STAR; nCntStar++)
		{
			m_apStar[nCntStar]->SetStar(CStar::TYPE_NOGLOW);//���C�t�̐��ȍ~�̐����Â�����
		}
		for (int nCntStar = 0; nCntStar < m_nLife; nCntStar++) //���C�t�̐��܂ł̐������点��
		{
			m_apStar[nCntStar]->SetStar(CStar::TYPE_GLOW);
		}

	}
}