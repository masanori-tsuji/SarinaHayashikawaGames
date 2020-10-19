//=============================================================================
//
// 背景の処理 [ui.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "newtext.h"

char CNewText::m_str[256][256] = {};
CNewText::CNewText()
{
	m_pFont = NULL;
	m_Rect[0] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	m_Rect[1] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	m_Rect[2] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	m_Rect[3] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	memset(m_str, 0, sizeof(m_str));
	posX = 0;
	posY = 0;
}
CNewText::~CNewText()
{

}

CNewText * CNewText::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CNewText *pText;

	pText = new CNewText;
	pText->Init(pos, fSizeWidth, fSizeHeight);

	return pText;
}

HRESULT CNewText::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// フォントの生成
	D3DXCreateFont(pDevice, 22, 15, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus12", &m_pFont);

	posX = SCREEN_WIDTH - 650;
	posY = SCREEN_HEIGHT - 550;
	return S_OK;
}

void CNewText::Uninit(void)
{
	// フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

void CNewText::Update(void)
{


}

void CNewText::Draw(void)
{
	m_Rect[0] = { posX + 2, posY + 2 , posX + 2 + 400, posY + 2 + 100 };
	m_Rect[1] = { posX, posY, posX + 400, posY + 100 };

	//m_Rect[0] = { posX + 2, posY + 2 , posX + 2 + 400, posY + 2 + 100 };
	//m_Rect[1] = { posX, posY, posX + 400, posY + 100 };
	//wsprintf(m_str, &m_str[0]);
	// テキスト描画
	wsprintf(m_str[0], "テストテストテストテスト\naaaaaaaaaaaaaaaaaaa");
	m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
	m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

void CNewText::DrawResult(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// フォントの生成
	D3DXCreateFont(pDevice, 22 * 3, 15 * 3, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "PixelMplus12", &m_pFont);

	//wsprintf(m_str[0], "ゲームスコア　：　11111111\n");
	//wsprintf(m_str[1], "残りボムボーナス　：　11111111\n");
	//wsprintf(m_str[2], "残機ボーナス　：　11111111\n");
	//wsprintf(m_str[3], "グレイズボーナス　：　11111111\n");
	//wsprintf(m_str[4], "合計スコア　：　11111111\n");

	posX = 200;
	posY = 500;
	////m_Rect[0] = { posX + 2, posY + 2 , posX + 2 + 400, posY + 2 + 100 };
	////m_Rect[1] = { posX, posY, posX + 400, posY + 100 };
	//for (int nCnt = 0; nCnt < 4; nCnt++)
	//{
	//	m_Rect[0] = { posX + 2, posY + 2 + (nCnt * 100) , posX + 2 + 400 + (nCnt * 100), posY + 2 + 100 };
	//	m_Rect[1] = { posX, posY + (nCnt * 100), posX + 400 + (nCnt * 100), posY + 100 };
	//	m_pFont->DrawText(NULL, m_str[nCnt], -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
	//	m_pFont->DrawText(NULL, m_str[nCnt], -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	//}
	wsprintf(m_str[0], "ゲームスコア　：　11111111\n");
	m_Rect[0] = { posX + 2, posY + 2 + (0 * 100) , SCREEN_WIDTH, SCREEN_HEIGHT };
	m_Rect[1] = { posX, posY + (0 * 100), SCREEN_WIDTH, SCREEN_HEIGHT };
	m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
	m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	wsprintf(m_str[1], "残りボムボーナス　：　11111111\n");
	m_Rect[0] = { posX + 2, posY + 2 + (1 * 100) , SCREEN_WIDTH, SCREEN_HEIGHT };
	m_Rect[1] = { posX, posY + (1 * 100), SCREEN_WIDTH, SCREEN_HEIGHT };
	m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
	m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	wsprintf(m_str[2], "残機ボーナス　：　11111111\n");
	m_Rect[0] = { posX + 2, posY + 2 + (2 * 100) , SCREEN_WIDTH, SCREEN_HEIGHT };
	m_Rect[1] = { posX, posY + (2 * 100), SCREEN_WIDTH, SCREEN_HEIGHT };
	m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
	m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	wsprintf(m_str[2], "グレイズボーナス　：　11111111\n");
	m_Rect[0] = { posX + 2, posY + 2 + (3 * 100) , SCREEN_WIDTH, SCREEN_HEIGHT };
	m_Rect[1] = { posX, posY + (3 * 100), SCREEN_WIDTH, SCREEN_HEIGHT };
	m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
	m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	wsprintf(m_str[2], "合計スコア　：　11111111\n");
	m_Rect[0] = { posX + 2, posY + 2 + (4 * 100) , SCREEN_WIDTH, SCREEN_HEIGHT };
	m_Rect[1] = { posX, posY + (4 * 100), SCREEN_WIDTH, SCREEN_HEIGHT };
	m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
	m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}
