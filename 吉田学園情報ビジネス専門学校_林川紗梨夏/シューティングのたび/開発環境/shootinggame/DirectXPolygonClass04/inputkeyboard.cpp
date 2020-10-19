//=============================================================================
//
// キーボードの処理 [inputkeyboard.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "input.h"
#include"inputkeyboard.h"

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
BYTE CInputKeyboard::m_aKeyState[MAX_KEY] = {};
BYTE CInputKeyboard::m_aKeyStateTrigger[MAX_KEY] = {};
BYTE CInputKeyboard::m_aKeyStateRelease[MAX_KEY] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	memset(m_aKeyState, 0, sizeof(m_aKeyState));
	memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
	memset(m_aKeyStateRelease, 0, sizeof(m_aKeyStateRelease));
}

//=============================================================================
// デストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	// キーボード用にデバイスオブジェクトを作成
	if (FAILED(m_pDinput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	// モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	m_pDevice->Acquire();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[MAX_KEY];
	int nCntKey;

	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) &aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}
