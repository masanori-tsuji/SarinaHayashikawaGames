//=============================================================================
//
// マウスの処理 [inputmouse.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "input.h"
#include"inputmouse.h"

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
DIMOUSESTATE2 CInputMouse::m_MouseState_bak = {};
DIMOUSESTATE2 CInputMouse::m_pMouseState = {};
POINT CInputMouse::m_pPoint = {};
HWND CInputMouse::m_phWnd = NULL;
LPDIRECTINPUT8 CInputMouse::m_pmDinput = NULL;
//=============================================================================
// コンストラクタ
//=============================================================================
CInputMouse::CInputMouse()
{
	m_pmDevice = NULL;
	memset(&m_MouseState_bak, 0, sizeof(m_MouseState_bak));
	memset(&m_pMouseState, 0, sizeof(m_pMouseState));
}

//=============================================================================
// デストラクタ
//=============================================================================
CInputMouse::~CInputMouse()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	m_phWnd = hWnd;
	
	// マウス用にデバイスオブジェクトを作成
	if (FAILED(m_pDinput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}
	
	m_pDevice->Acquire();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CInputMouse::Update(void)
{
	GetCursorPos(&m_pPoint);//マウスカーソルの座標取得
	ScreenToClient(m_phWnd, (LPPOINT)&m_pPoint);//クライアント座標に変換

	memcpy(&m_MouseState_bak, &m_pMouseState, sizeof(m_MouseState_bak));//前のマウスの状態を格納

	// マウスの状態を取得します
	HRESULT	hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_pMouseState);
	if (hr == DIERR_INPUTLOST)
	{
		m_pDevice->Acquire();
		hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_pMouseState);
	}

	///*値が変わったら表示します*/
	//if (memcmp(&m_MouseState_bak, &m_pMouseState, sizeof(m_MouseState_bak)) != 0)
	//{

	//	char buf[128];
	//	wsprintf(buf, "(%5d, %5d, %5d) %s %s %s\n",
	//		m_pMouseState.lX, m_pMouseState.lY, m_pMouseState.lZ,
	//		(m_pMouseState.rgbButtons[0] & 0x80) ? "Left" : "--",
	//		(m_pMouseState.rgbButtons[1] & 0x80) ? "Right" : "--",
	//		(m_pMouseState.rgbButtons[2] & 0x80) ? "Center" : "--");
	//	OutputDebugString(buf);
	//
	//}
}

//=============================================================================
// 座標取得
//=============================================================================
POINT CInputMouse::GetMousePoint(void)
{
	return m_pPoint;
}

//=============================================================================
// 状態取得
//=============================================================================
DIMOUSESTATE2 CInputMouse::GetMouseState(void)
{
	return m_pMouseState;
}

bool CInputMouse::GetMousePress(int nButton)
{ 
	return m_pMouseState.rgbButtons[nButton] & 0x80 ? true : false; 
}

