//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : 
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//�����_�����O�N���X
class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void);
private:
	#define _DEBUG
#ifdef _DEBUG
	void DrawFPS(void);
#endif // _DEBUG

	LPDIRECT3D9             m_pD3D;
	LPDIRECT3DDEVICE9       m_pD3DDevice;
#ifdef _DEBUG
	LPD3DXFONT				m_pFont;		// �t�H���g�ւ̃|�C���^
	int						m_nCountFPS;	// FPS�J�E���^
#endif

};

#endif // !_RENDERER_H_
