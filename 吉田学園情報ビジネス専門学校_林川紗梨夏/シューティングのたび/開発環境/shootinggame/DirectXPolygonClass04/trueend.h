//=============================================================================
//
// ED [trueend.h]
// Author : 林川紗梨夏
//
//=============================================================================
#ifndef _TRUE_END_H_
#define _TRUE_END_H_

//#define TEXTURE_RESULT_2	     "data/TEXTURE/result_001.png"
//#define TEXTURE_     "data/TEXTURE/result001.png"
#define TEXTURE_TRUEEND     "data/TEXTURE/trueend001.png"

class CTrueend : public CScene
{
public:
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_001,
		TYPE_MAX
	}TYPE;
	CTrueend(int nPriority = 1);
	~CTrueend();
	static HRESULT Load(void);
	static void Unload(void);
	static CTrueend *Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
private:
	static LPDIRECT3DTEXTURE9   m_apTexture[TYPE_MAX];
	CScene2D                  * m_apScene2D[TYPE_MAX];
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //頂点座標
	D3DXVECTOR3                 m_pos;
	float                       m_fSizeWidth;
	float                       m_fSizeHeight;
	int                     m_nPatternAnim; //アニメーションパターン
	int                     m_nCountAnim; //アニメーションパターン
};
#endif // !_BG_H_


