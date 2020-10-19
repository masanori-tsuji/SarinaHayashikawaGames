//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : 
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "joystick.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "text.h"
#include "ranking.h"
#include "name.h"
#include "pause.h"
#include "continue.h"
#include "trueend.h"
#include "badend.h"
#include "talk.h"
//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
CRenderer * CManager::m_pRenderer = NULL;
CInputKeyboard * CManager::m_pInputKeyboard = NULL;
CInputMouse * CManager::m_pInputMouse = NULL;
CSound * CManager::m_pSound = NULL;
CInputJoystick * CManager::m_pInputJoystick = NULL;
CTitle * CManager::m_pTitle = NULL;
CGame * CManager::m_pGame = NULL;
CRanking * CManager::m_pRanking = NULL;
CResult * CManager::m_pResult = NULL;
CName * CManager::m_pName = NULL;
CTrueend * CManager::m_pTrueend = NULL;
CBadend * CManager::m_pBadend = NULL;
CText * CManager::m_pText = NULL;
CFade * CManager::m_pFade = NULL;
CPause * CManager::m_pPause = NULL;
CContinue * CManager::m_pContinue = NULL;
CManager::MODE CManager::m_mode = MODE_TITLE;
CManager::PlayerData CManager::m_playerData = {0,0,0,0};
bool CManager::m_bIsTalking = false;
bool CManager::m_bUseFade = false;
//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{
	m_pScene = NULL;
}
//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HWND hWnd, bool bWindow, HINSTANCE hInstance)
{
	m_playerData.nBomb = 0;
	m_playerData.nGraze = 0;
	m_playerData.nScore = 0;
	m_playerData.nLife = 0;

	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;
		// 初期化処理
		if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
		{
			return -1;

		}
	}
	if (m_pInputKeyboard == NULL)
	{
		m_pInputKeyboard = new CInputKeyboard;
		// 初期化処理
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{
			return -1;
		}
	}
	if (m_pInputMouse == NULL)
	{
		m_pInputMouse = new CInputMouse;
		// 初期化処理
		if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
		{
			return -1;
		}
	}
	if (m_pInputJoystick == NULL)
	{
		m_pInputJoystick = new CInputJoystick;
		// 初期化処理
		if (FAILED(m_pInputJoystick->Init(hInstance, hWnd)))
		{
			return -1;

		}
	}
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;
		// 初期化処理
		if (FAILED(m_pSound->Init(hWnd)))
		{
			return -1;
		}
	}

	if (m_pText == NULL)
	{
		m_pText = new CText;

		if (FAILED(m_pText->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100, 100)))
		{
			return -1;
		}
	}
	//テクスチャロード
	CTitle::Load();
	CResult::Load();
	CRanking::Load();
	CTrueend::Load();
	CBadend::Load();
	CName::Load();
	CPause::Load();
	CContinue::Load();
	CTalk::Load();

	//フェード生成
	m_pFade = CFade::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, m_mode);
	SetMode(MODE_TITLE);
	
	//ポーズ生成
	m_pPause= CPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
	//ポーズ生成
	m_pContinue = CContinue::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// 終了処理
	m_pRenderer->Uninit();
	if (m_pRenderer != NULL)
	{
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	m_pInputKeyboard->Uninit();
	if (m_pInputKeyboard != NULL)
	{
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	m_pInputMouse->Uninit();
	if (m_pInputMouse != NULL)
	{
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	m_pInputJoystick->Uninit();
	if (m_pInputJoystick != NULL)
	{
		delete m_pInputJoystick;
		m_pInputJoystick = NULL;
	}

	m_pSound->CSound::StopSound();
	m_pSound->Uninit();
	if (m_pSound != NULL)
	{
		delete m_pSound;
		m_pSound = NULL;
	}

	m_pText->Uninit();
	if (m_pText != NULL)
	{
		delete m_pText;
		m_pText = NULL;
	}

	//フェード終了
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}

	//ポーズ終了
	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = NULL;
	}
	//こんてにゅ終了
	if (m_pContinue != NULL)
	{
		m_pContinue->Uninit();
		delete m_pContinue;
		m_pContinue = NULL;
	}
	//全て破棄する

	//テクスチャアンロード
	CScene::ReleaseAll();
	CTitle::Unload();
	CResult::Unload();
	CName::Unload();
	CRanking::Unload();
	CPause::Unload();
	CTrueend::Unload();
	CBadend::Unload();
	CContinue::Unload();
	CTalk::Unload();
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();// 更新処理
	}

	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Update();
	}

	if (m_pInputJoystick != NULL)
	{
		m_pInputJoystick->Update();
	}

	if (m_pText != NULL)
	{
		m_pText->Update();
	}
	if (CGame::GetIsStopUpdate() == true)
	{
		if (m_pPause != NULL)
		{
			m_pPause->Update();
		}
	}
	if (CGame::GetIsStopUpdateContinue() == true)
	{
		if (m_pContinue != NULL)
		{
			m_pContinue->Update();
		}
	}

	//フェード更新
	if (m_bUseFade == true)
	{
		if (m_pFade != NULL)
		{
			m_pFade->Update();
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	m_pRenderer->Draw();// 描画処理
}

//=============================================================================
// レンダリング取得
//=============================================================================
CRenderer * CManager::GetRenderer(void)
{
	return m_pRenderer;
}
//=============================================================================
// キーボード取得
//=============================================================================
CInputKeyboard * CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}
//=============================================================================
// マウス取得
//=============================================================================
CInput * CManager::GetInputMouse(void)
{
	return m_pInputMouse;
}

//=============================================================================
// ジョイパッド取得
//=============================================================================
CInputJoystick * CManager::GetInputJoystick(void)
{
	return m_pInputJoystick;
}

void CManager::SetMode(MODE mode)
{

	CScene::ReleaseAll();

	m_mode = mode;

	switch (m_mode)
	{
	case MODE_TITLE:
			m_pTitle = CTitle::Create(D3DXVECTOR3(SCREEN_CENTER_X,SCREEN_CENTER_Y,0.0f),SCREEN_WIDTH,SCREEN_HEIGHT);
		break;
	case MODE_GAME:
			m_pGame = CGame::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
	case MODE_RANKING:
		m_pRanking = CRanking::Create(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
	case MODE_RESULT:
		m_pResult = CResult::Create(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
	case MODE_NAME:
		m_pName = CName::Create(D3DXVECTOR3(SCREEN_WIDTH / 4, 500.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
	case MODE_TRUEEND:
		m_pTrueend = CTrueend::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
	case MODE_BADEND:
		m_pBadend = CBadend::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
		break;
	default:
		break;
	}
}

CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

//----------------------------------------------------
//フェードの開始
//----------------------------------------------------
void CManager::StartFade(MODE mode,D3DXCOLOR col, float fadelate)
{
	m_pFade->SetColor(col);
	m_pFade->SetFadelate(fadelate);
	m_mode = mode;
	m_bUseFade = true;
	CText::IsDrawText(false);//テキストは描画させない
}
void CManager::SetPlayerName(int nCount, char cName)
{
	//名前格納
	m_playerData.aName[nCount] = cName;
}
//----------------------------------------------------
//フェードの停止
//----------------------------------------------------
void CManager::StopFade(void)
{
	m_bUseFade = false;
	CText::IsDrawText(true);
}

