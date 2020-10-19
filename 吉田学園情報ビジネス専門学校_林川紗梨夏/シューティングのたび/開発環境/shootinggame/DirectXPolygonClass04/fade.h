
#ifndef _FADE_H_
#define _FADE_H_
#define FADE_RATE 0.05f
//---------------------------------------
//前方宣言
//---------------------------------------
class CManager;
//---------------------------------------
//クラス
//---------------------------------------
class CFade
{
public:
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_MAX
	}FADE;
	CFade();
	~CFade();
	static CFade * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int sizeX, int sizeY, CManager::MODE mode);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int sizeX, int sizeY, CManager::MODE mode);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetColor(D3DXCOLOR col);
	void SetFadelate(float fadelate);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//バッファ
	D3DXVECTOR3 m_pos;
	int m_nSizeX;
	int m_nSizeY;
	FADE m_fade;	//フェード
	CManager::MODE m_ModeNext;//次のモード
	D3DXCOLOR m_colorFade;	//カラー
	float     m_fFadelate;
};
#endif