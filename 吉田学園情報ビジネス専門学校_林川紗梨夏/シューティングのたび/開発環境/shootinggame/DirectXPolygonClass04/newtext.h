#ifndef _TEXT_H_
#define _TEXT_H_

class CNewText
{
public:
	CNewText();
	~CNewText();
	static HRESULT Load(void);
	static void Unload(void);
	static CNewText *Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DrawResult(void);
private:
	LPD3DXFONT				m_pFont;		// フォントへのポインタ
	RECT                    m_Rect[4];
	int                     posX;
	int                     posY;
	static char                  m_str[256][256];

};
#endif // !_TEXT_H_


