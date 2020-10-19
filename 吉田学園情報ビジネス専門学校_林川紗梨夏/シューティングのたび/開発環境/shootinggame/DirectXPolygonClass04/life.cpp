//=============================================================================
//
// ライフ処理 [life.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
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
// コンストラクタ
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
// デストラクタ
//=============================================================================
CLife::~CLife()
{
}

//=============================================================================
// 初期化処理
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
// 終了処理
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
// 更新処理
//=============================================================================
void CLife::Update(void)
{
	CManager::SetLife(m_nLife);
}
//=============================================================================
// 描画処理
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
// 生成処理
//=============================================================================
CLife * CLife::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CLife *pScore;

	pScore = new CLife;
	pScore->Init(pos, fSizeWidth, fSizeHeight);

	return pScore;
}
//=============================================================================
// ライフのセット処理
//=============================================================================
void CLife::SetLife(int nLife)
{
	m_nLife = nLife;

	if (m_nLife <= MAX_STAR)
	{
		for (int nCntStar = m_nLife; nCntStar < MAX_STAR; nCntStar++)
		{
			m_apStar[nCntStar]->SetStar(CStar::TYPE_NOGLOW);//ライフの数以降の星を暗くする
		}
		for (int nCntStar = 0; nCntStar < m_nLife; nCntStar++) //ライフの数までの星を光らせる
		{
			m_apStar[nCntStar]->SetStar(CStar::TYPE_GLOW);
		}

	}
}