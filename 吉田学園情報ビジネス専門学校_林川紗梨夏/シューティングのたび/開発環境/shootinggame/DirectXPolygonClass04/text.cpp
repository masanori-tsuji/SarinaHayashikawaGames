//=============================================================================
//
// テキストの処理 [text.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "joystick.h"
#include "scene.h"
#include "text.h"
#include "result.h"
#include "ranking.h"

char * CText::m_str[512] = {};
char  CText::m_strUiwindow[2][512] = {};
bool CText::m_IsDraw = false;
CText::CText()
{
	m_pFont = NULL;
	m_Rect[0] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	m_Rect[1] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	m_Rect[2] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	m_Rect[3] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	memset(&m_str, 0, sizeof(m_str));
	memset(&m_strUiwindow, 0, sizeof(m_strUiwindow));
	memset(&m_playerData, 0, sizeof(m_playerData));
	m_nCounter = 0;
	m_IsDraw = true;
	posX = 0;
	posY = 0;
}
CText::~CText()
{

}

CText * CText::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CText *pText;

	pText = new CText;
	pText->Init(pos, fSizeWidth, fSizeHeight);

	return pText;
}

HRESULT CText::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// フォントの生成
	D3DXCreateFont(pDevice, 22, 15, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FONT_NAME, &m_pFont);

	posX = SCREEN_WIDTH - 650;
	posY = SCREEN_HEIGHT - 550;
	return S_OK;
}

void CText::Uninit(void)
{
	// フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

void CText::Update(void)
{

	
}

void CText::Draw(void)
{

	if (m_IsDraw == true)
	{
		switch (CManager::GetMode())
		{
		case CManager::MODE_GAME:
			//ジョイパッド
			
			char str[256];

			m_Rect[0] = { posX + 2, posY + 2 , posX + 2 + 400, posY + 2 + 100 };
			m_Rect[1] = { posX, posY, posX + 400, posY + 100 };

			posX = SCREEN_WIDTH - 650;
			posY = SCREEN_HEIGHT - 550;

			m_Rect[0] = { posX + 2, posY + 2 , posX + 2 + 400, posY + 2 + 100 };
			m_Rect[1] = { posX, posY, posX + 400, posY + 100 };

			 //テキスト描画

			if (m_str[0] != NULL)
			{
				wsprintf(m_str[0], m_str[0]);
				m_Rect[0] = { posX + -1000 + 2, posY + 420 + 2 + (-1 * 100) , SCREEN_WIDTH, SCREEN_HEIGHT };
				m_Rect[1] = { posX + -1000, posY + 420 + (-1 * 100), SCREEN_WIDTH, SCREEN_HEIGHT };
				m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
				m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
			}

			if (m_strUiwindow[0] != NULL)
			{
				wsprintf(m_strUiwindow[0], m_strUiwindow[0]);
				m_Rect[0] = { posX + 2, posY + 2 , posX + 2 + 400, posY + 2 + 100 };
				m_Rect[1] = { posX, posY, posX + 400, posY + 100 };
				m_pFont->DrawText(NULL, m_strUiwindow[0], -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
				m_pFont->DrawText(NULL, m_strUiwindow[0], -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
			}
			if (m_strUiwindow[1] != NULL)
			{
				wsprintf(m_strUiwindow[1], m_strUiwindow[1]);
				m_Rect[0] = { posX + 200 + 2, posY + 250 + 2 , posX + 2 + 550, posY + 2 + 600 };
				m_Rect[1] = { posX + 200, posY + 250, posX + 550, posY + 600 };
				m_pFont->DrawText(NULL, m_strUiwindow[1], -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
				m_pFont->DrawText(NULL, m_strUiwindow[1], -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
			}
	
		break;
		case CManager::MODE_RESULT:
			
				posX = 200;
				posY = 500;
				m_playerData = CManager::GetPlayerData();
				wsprintf(str, "お名前: %s \n", &m_playerData.aName);
				m_Rect[0] = { posX + 2, posY + 2 + (-1 * 100) , SCREEN_WIDTH, SCREEN_HEIGHT };
				m_Rect[1] = { posX, posY + (-1 * 100), SCREEN_WIDTH, SCREEN_HEIGHT };
				m_pFont->DrawText(NULL, str, -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
				m_pFont->DrawText(NULL, str, -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

				wsprintf(str, "ゲームスコア　    ：　%d\n", m_playerData.nScore - ((m_playerData.nGraze * GRAZE_BONUS) + (m_playerData.nLife * LIFE_BONUS) + (m_playerData.nBomb * BOMB_BONUS)));
				m_Rect[0] = { posX + 2, posY + 2 + (0 * 100) , SCREEN_WIDTH, SCREEN_HEIGHT };
				m_Rect[1] = { posX, posY + (0 * 100), SCREEN_WIDTH, SCREEN_HEIGHT };
				m_pFont->DrawText(NULL, str, -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
				m_pFont->DrawText(NULL, str, -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

				wsprintf(str, "残りボムボーナス　：　%d × %d\n", m_playerData.nBomb, BOMB_BONUS);
				m_Rect[0] = { posX + 2, posY + 2 + (1 * 100) , SCREEN_WIDTH, SCREEN_HEIGHT };
				m_Rect[1] = { posX, posY + (1 * 100), SCREEN_WIDTH, SCREEN_HEIGHT };
				m_pFont->DrawText(NULL, str, -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
				m_pFont->DrawText(NULL, str, -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

				wsprintf(str, "残機ボーナス　    ：　%d × %d\n", m_playerData.nLife, LIFE_BONUS);
				m_Rect[0] = { posX + 2, posY + 2 + (2 * 100) , SCREEN_WIDTH, SCREEN_HEIGHT };
				m_Rect[1] = { posX, posY + (2 * 100), SCREEN_WIDTH, SCREEN_HEIGHT };
				m_pFont->DrawText(NULL, str, -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
				m_pFont->DrawText(NULL, str, -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

				wsprintf(str, "グレイズボーナス　：　%d × %d\n", m_playerData.nGraze, GRAZE_BONUS);
				m_Rect[0] = { posX + 2, posY + 2 + (3 * 100) , SCREEN_WIDTH, SCREEN_HEIGHT };
				m_Rect[1] = { posX, posY + (3 * 100), SCREEN_WIDTH, SCREEN_HEIGHT };
				m_pFont->DrawText(NULL, str, -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
				m_pFont->DrawText(NULL, str, -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

				wsprintf(str, "合計スコア　      ：　%d\n ", m_playerData.nScore);
				m_Rect[0] = { posX + 2, posY + 2 + (4 * 100) , SCREEN_WIDTH, SCREEN_HEIGHT };
				m_Rect[1] = { posX, posY + (4 * 100), SCREEN_WIDTH, SCREEN_HEIGHT };
				m_pFont->DrawText(NULL, str, -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
				m_pFont->DrawText(NULL, str, -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

		

			break;
		case CManager::MODE_RANKING:
	
				for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
				{
					posX = 200;
					posY = 300;
					RECT rect = { posX, posY + (nCnt + 1) * 70, SCREEN_WIDTH, SCREEN_HEIGHT };
					char str[256];
					CRanking::RankingData * RankingData = CRanking::GetRankingData();

					wsprintf(str, "%d位　名前 : %s スコア:%d\n", nCnt, RankingData[nCnt].aName, RankingData[nCnt].nScore);

					// テキスト描画
					m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
				}
				break;
		case CManager::MODE_NAME:
			posX = 200;
			posY = 400;
			str[256];
			wsprintf(str, "名前入力：Xボタンで入力\nYで一文字戻る\nSTARTで入力完了");
			m_Rect[0] = { posX + 2, posY + 2 + (-1 * 100) , SCREEN_WIDTH, SCREEN_HEIGHT };
			m_Rect[1] = { posX, posY + (-1 * 100), SCREEN_WIDTH, SCREEN_HEIGHT };
			m_pFont->DrawText(NULL, str, -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
			m_pFont->DrawText(NULL, str, -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
			break;
		case CManager::MODE_BADEND:
			//ジョイパッド

			m_Rect[0] = { posX + 2, posY + 2 , posX + 2 + 400, posY + 2 + 100 };
			m_Rect[1] = { posX, posY, posX + 400, posY + 100 };

			posX = SCREEN_WIDTH - 650;
			posY = SCREEN_HEIGHT - 550;

			m_Rect[0] = { posX + 2, posY + 2 , posX + 2 + 400, posY + 2 + 100 };
			m_Rect[1] = { posX, posY, posX + 400, posY + 100 };

			if (m_str[0] != NULL)
			{
				wsprintf(m_str[0], m_str[0]);
				m_Rect[0] = { posX + -1000 + 2, posY + 420 + 2 + (-1 * 100) , SCREEN_WIDTH, SCREEN_HEIGHT };
				m_Rect[1] = { posX + -1000, posY + 420 + (-1 * 100), SCREEN_WIDTH, SCREEN_HEIGHT };
				m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
				m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
			}

			break;
		case CManager::MODE_TRUEEND:
			//会話

			m_Rect[0] = { posX + 2, posY + 2 , posX + 2 + 400, posY + 2 + 100 };
			m_Rect[1] = { posX, posY, posX + 400, posY + 100 };

			posX = SCREEN_WIDTH - 650;
			posY = SCREEN_HEIGHT - 550;

			m_Rect[0] = { posX + 2, posY + 2 , posX + 2 + 400, posY + 2 + 100 };
			m_Rect[1] = { posX, posY, posX + 400, posY + 100 };

			if (m_str[0] != NULL)
			{
				wsprintf(m_str[0], m_str[0]);
				m_Rect[0] = { posX + -1000 + 2, posY + 420 + 2 + (-1 * 100) , SCREEN_WIDTH, SCREEN_HEIGHT };
				m_Rect[1] = { posX + -1000, posY + 420 + (-1 * 100), SCREEN_WIDTH, SCREEN_HEIGHT };
				m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[0], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 100));
				m_pFont->DrawText(NULL, m_str[0], -1, &m_Rect[1], DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
			}


			break;
		default:
			break;
	}
}
	
	
}

void CText::SetText(char aText[512])
{
	m_str[0] = &aText[0];
}

void CText::SetTextUi(int nNum,char aText[512])
{
	for (int nCount = 0; nCount < 512; nCount++)
	{
		m_strUiwindow[nNum][nCount] = aText[nCount];
	}
	
}
