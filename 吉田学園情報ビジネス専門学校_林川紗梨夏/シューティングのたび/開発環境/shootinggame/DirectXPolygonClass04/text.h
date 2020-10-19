#ifndef _TEXT_H_
#define _TEXT_H_

#define MAX_TEXT 100
#define FONT_NAME "PixelMplus12"
class CText
{
public:
	
	CText();
	~CText();
	static HRESULT Load(void);
	static void Unload(void);
	static CText *Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetText(char aText[512]);
	void SetTextUi(int nNum, char aText[512]);
	static void IsDrawText(bool bDraw) { m_IsDraw = bDraw; };
private:
	LPD3DXFONT				m_pFont;		// フォントへのポインタ
	RECT                    m_Rect[4];
	int                     posX;
	int                     posY;
	static char           * m_str[512];
	static char            m_strUiwindow[2][512];
	int                     m_nCounter;
	static bool             m_IsDraw;//切り替え
	CManager::PlayerData m_playerData;//プレイヤーデータ
	                  
};
#endif // !_TEXT_H_


