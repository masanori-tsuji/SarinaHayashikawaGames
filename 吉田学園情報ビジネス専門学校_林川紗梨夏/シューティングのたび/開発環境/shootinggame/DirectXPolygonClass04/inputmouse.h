//=============================================================================
//
// マウス入力処理 [inputmouse.h]
// Author : 
//
//=============================================================================

#ifndef  _INPUTMOUSE_H_
#define _INPUTMOUSE_H_

//マウスクラス
class CInputMouse : public CInput
{
public:
	CInputMouse();
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	/*void GetMouseState();*/
	static POINT GetMousePoint(void);
	static DIMOUSESTATE2 GetMouseState(void);
	static bool GetMousePress(int nButton);
	/*static bool GetMousePress(int nButton);*/
private:
	static LPDIRECTINPUT8	m_pmDinput;//DirectInputのポインタ
	LPDIRECTINPUTDEVICE8	m_pmDevice;//Deviceのポインタ
	static DIMOUSESTATE2 m_pMouseState;
	static DIMOUSESTATE2 m_MouseState_bak;	// マウス情報(変化検知用)// 読取前の値を保持します
	static DIMOUSESTATE2 m_pMouseStateTrigger;
	static DIMOUSESTATE2 m_pMouseStateRelease;
	static POINT m_pPoint;//ポイント
	static HWND m_phWnd;//ウィンドウハンドル
};
#endif // ! _INPUTMOUSE_H_
