//=============================================================================
//
// �}�E�X���͏��� [inputmouse.h]
// Author : 
//
//=============================================================================

#ifndef  _INPUTMOUSE_H_
#define _INPUTMOUSE_H_

//�}�E�X�N���X
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
	static LPDIRECTINPUT8	m_pmDinput;//DirectInput�̃|�C���^
	LPDIRECTINPUTDEVICE8	m_pmDevice;//Device�̃|�C���^
	static DIMOUSESTATE2 m_pMouseState;
	static DIMOUSESTATE2 m_MouseState_bak;	// �}�E�X���(�ω����m�p)// �ǎ�O�̒l��ێ����܂�
	static DIMOUSESTATE2 m_pMouseStateTrigger;
	static DIMOUSESTATE2 m_pMouseStateRelease;
	static POINT m_pPoint;//�|�C���g
	static HWND m_phWnd;//�E�B���h�E�n���h��
};
#endif // ! _INPUTMOUSE_H_
