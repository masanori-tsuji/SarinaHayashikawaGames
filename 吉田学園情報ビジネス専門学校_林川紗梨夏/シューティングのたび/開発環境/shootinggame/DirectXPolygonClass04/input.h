//=============================================================================
//
// ���͏��� [input.h]
// Author : 
//
//=============================================================================

#ifndef  _INPUT_H_
#define _INPUT_H_

//�C���v�b�g�N���X
class CInput
{
public:
	CInput();
	virtual~CInput();
	virtual HRESULT Init(HINSTANCE hInstance,HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	//virtual bool GetKeyboardPress(int nKey)= 0;
	//virtual bool GetKeyboardTrigger(int nKey) = 0;
	//virtual bool GetKeyboardRelease(int nKey) = 0;
protected:
	static LPDIRECTINPUT8	m_pDinput;//DirectInput�̃|�C���^
	LPDIRECTINPUTDEVICE8	m_pDevice;//Device�̃|�C���^
};
#endif // ! _INPUT_H_
