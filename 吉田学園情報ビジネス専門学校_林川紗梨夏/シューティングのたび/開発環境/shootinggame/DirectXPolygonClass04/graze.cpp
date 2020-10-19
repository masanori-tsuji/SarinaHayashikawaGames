//=============================================================================
//
// グレイズ処理 [graze.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "graze.h"
#include <cmath>


//=============================================================================
// コンストラクタ
//=============================================================================
CGraze::CGraze(int nPriority) : CScene(nPriority)
{
	memset(m_apNumber, 0, sizeof(m_apNumber));
	m_nGraze = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGraze::~CGraze()
{
}

//=============================================================================
// 初期化処理
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
// 終了処理
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
// 更新処理
//=============================================================================
void CGraze::Update(void)
{
	CManager::SetGraze((int)m_nGraze);
}
//=============================================================================
// 描画処理
//=============================================================================
void CGraze::Draw(void)
{
	for (int nCntNum = 0; nCntNum < MAX_DIGITS; nCntNum++)
	{
		m_apNumber[nCntNum]->Draw();
	}
}
//=============================================================================
// 生成処理
//=============================================================================
CGraze * CGraze::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CGraze *pGraze;

	pGraze = new CGraze;
	pGraze->Init(pos, fSizeWidth, fSizeHeight);

	return pGraze;
}
//=============================================================================
// グレイズ数表示
//=============================================================================
void CGraze::SetGraze(float nGraze)
{
	m_nGraze = nGraze;
	int integer = 0;//計算用変数
	int nCntNumber = 0;

	for (int nCntDigit = 1; nCntDigit <= MAX_DIGITS; nCntDigit++, nCntNumber++)
	{

		integer = int(m_nGraze / std::pow(10, nCntDigit)); //1の位以下を切り捨てる
		integer = int(integer * std::pow(10, nCntDigit));
		m_apNumber[nCntNumber]->SetNumber(int((m_nGraze - integer) / std::pow(10.0, nCntDigit - 1)));
	}

}
//=============================================================================
// グレイズ加算処理
//=============================================================================
void CGraze::AddGraze(float nValue)
{
	m_nGraze += nValue;
	SetGraze(m_nGraze);
}
