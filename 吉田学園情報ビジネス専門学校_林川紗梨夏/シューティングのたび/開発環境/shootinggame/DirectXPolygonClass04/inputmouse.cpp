//=============================================================================
//
// �}�E�X�̏��� [inputmouse.cpp]
// Author : �ѐ�ї���
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "input.h"
#include"inputmouse.h"

//*****************************************************************************
// �ÓI�����o�ϐ�������
//*****************************************************************************
DIMOUSESTATE2 CInputMouse::m_MouseState_bak = {};
DIMOUSESTATE2 CInputMouse::m_pMouseState = {};
POINT CInputMouse::m_pPoint = {};
HWND CInputMouse::m_phWnd = NULL;
LPDIRECTINPUT8 CInputMouse::m_pmDinput = NULL;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputMouse::CInputMouse()
{
	m_pmDevice = NULL;
	memset(&m_MouseState_bak, 0, sizeof(m_MouseState_bak));
	memset(&m_pMouseState, 0, sizeof(m_pMouseState));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputMouse::~CInputMouse()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	m_phWnd = hWnd;
	
	// �}�E�X�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	if (FAILED(m_pDinput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}
	
	m_pDevice->Acquire();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CInputMouse::Update(void)
{
	GetCursorPos(&m_pPoint);//�}�E�X�J�[�\���̍��W�擾
	ScreenToClient(m_phWnd, (LPPOINT)&m_pPoint);//�N���C�A���g���W�ɕϊ�

	memcpy(&m_MouseState_bak, &m_pMouseState, sizeof(m_MouseState_bak));//�O�̃}�E�X�̏�Ԃ��i�[

	// �}�E�X�̏�Ԃ��擾���܂�
	HRESULT	hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_pMouseState);
	if (hr == DIERR_INPUTLOST)
	{
		m_pDevice->Acquire();
		hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_pMouseState);
	}

	///*�l���ς������\�����܂�*/
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
// ���W�擾
//=============================================================================
POINT CInputMouse::GetMousePoint(void)
{
	return m_pPoint;
}

//=============================================================================
// ��Ԏ擾
//=============================================================================
DIMOUSESTATE2 CInputMouse::GetMouseState(void)
{
	return m_pMouseState;
}

bool CInputMouse::GetMousePress(int nButton)
{ 
	return m_pMouseState.rgbButtons[nButton] & 0x80 ? true : false; 
}

