//=============================================================================
//
// スコア処理 [score.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "scene.h"

#include "score.h"
#include <cmath>


//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore(int nPriority) : CScene(nPriority)
{
	memset(m_apNumber, 0, sizeof(m_apNumber));
	m_nScore = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{
}

//=============================================================================
// 初期化処理
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
// 終了処理
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
// 描画処理
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntNum = 0; nCntNum < MAX_DIGITS; nCntNum++)
	{
		m_apNumber[nCntNum]->Draw();
	}
}
//=============================================================================
// 生成処理
//=============================================================================
CScore * CScore::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CScore *pScore;

	pScore = new CScore;
	pScore->Init(pos, fSizeWidth, fSizeHeight);

	return pScore;
}
//=============================================================================
// スコアセット
//=============================================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;
	int integer = 0;//計算用変数
	int nCntNumber = 0;
	
	for (int nCntDigit = 1; nCntDigit <= MAX_DIGITS; nCntDigit++,nCntNumber++)
	{

		integer = int(m_nScore / std::pow(10,nCntDigit)); //1の位以下を切り捨てる
		integer = int(integer * std::pow(10, nCntDigit));
		m_apNumber[nCntNumber]->SetNumber(int((m_nScore - integer) / std::pow(10.0, nCntDigit - 1)));
	}

}
//=============================================================================
// スコア加算
//=============================================================================
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;
	SetScore(m_nScore);
}
