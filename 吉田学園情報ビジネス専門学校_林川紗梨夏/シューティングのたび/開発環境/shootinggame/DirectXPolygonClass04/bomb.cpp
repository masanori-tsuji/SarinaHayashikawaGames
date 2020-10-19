//=============================================================================
//
// ボムUI処理 [bomb.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "explosion.h"
#include "bomb.h"
#include <cmath>

//=============================================================================
// コンストラクタ
//=============================================================================
CBomb::CBomb(int nPriority) : CScene(nPriority)
{
	memset(m_apStar, 0, sizeof(m_apStar));
	m_nBomb = NUM_BOMB;
	m_fSizeWidth = 0;
	m_fSizeHeight = 0;
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CBomb::~CBomb()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBomb::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	SetObjType(CScene::OBJTYPE_NONE);

	m_fSizeWidth = fSizeWidth;
	m_fSizeHeight = fSizeHeight;
	m_Pos = pos;
	for (int nCntNum = 0; nCntNum < MAX_STAR; nCntNum++)
	{
		m_apStar[nCntNum] = CStar::Create(D3DXVECTOR3(pos.x + (fSizeWidth * nCntNum), pos.y, 0.0f), fSizeWidth, fSizeHeight);
	}
	SetBomb(m_nBomb);
	CScene::SetPosition(pos);
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CBomb::Uninit(void)
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
void CBomb::Update(void)
{
	CManager::SetBomb(m_nBomb);
}
//=============================================================================
// 描画処理
//=============================================================================
void CBomb::Draw(void)
{
	for (int nCntNum = 0; nCntNum < MAX_STAR; nCntNum++)
	{
		if (m_apStar[nCntNum] != NULL)
		{
			m_apStar[nCntNum]->Draw();
		}
	}
}

CBomb * CBomb::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CBomb *pScore;

	pScore = new CBomb;
	pScore->Init(pos, fSizeWidth, fSizeHeight);

	return pScore;
}

void CBomb::SetBomb(int nBomb)
{
	m_nBomb = nBomb;

	if (m_nBomb <= MAX_STAR)
	{
		for (int nCntStar = m_nBomb; nCntStar < MAX_STAR; nCntStar++)
		{
			m_apStar[nCntStar]->SetStar(CStar::TYPE_NOGLOW);//ライフの数以降の星を暗くする
		}
		for (int nCntStar = 0; nCntStar < m_nBomb; nCntStar++) //ライフの数までの星を光らせる
		{
			m_apStar[nCntStar]->SetStar(CStar::TYPE_GLOW);
		}

	}
}
