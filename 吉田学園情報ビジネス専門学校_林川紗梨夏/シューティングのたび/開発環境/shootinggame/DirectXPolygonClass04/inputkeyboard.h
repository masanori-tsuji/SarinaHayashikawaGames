//=============================================================================
//
// �L�[�{�[�h���͏��� [inputkeyboard.h]
// Author : 
//
//=============================================================================

#ifndef  _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

#define MAX_KEY 256 //�L�[�ő吔
//�L�[�{�[�h�N���X
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	static bool GetKeyboardPress(int nKey) { return m_aKeyState[nKey] & 0x80 ? true : false; };
	static bool GetKeyboardTrigger(int nKey) { return m_aKeyStateTrigger[nKey] & 0x80 ? true : false; };
	static bool GetKeyboardRelease(int nKey) { return m_aKeyStateRelease[nKey] & 0x80 ? true : false; };
private:
	static BYTE m_aKeyState[MAX_KEY];
	static BYTE m_aKeyStateTrigger[MAX_KEY];
	static BYTE m_aKeyStateRelease[MAX_KEY];
};
#endif
