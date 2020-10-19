//=============================================================================
//
// ED [badend.h]
// Author : 林川紗梨夏
//
//=============================================================================
#ifndef _BAD_END_H_
#define _BAD_END_H_

#define TEXTURE_BADEND     "data/TEXTURE/badend001.png"

class CBadend : public CScene
{
public:
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_001,
		TYPE_MAX
	}TYPE;
	CBadend(int nPriority = 1);
	~CBadend();
	static HRESULT Load(void);
	static void Unload(void);
	static CBadend *Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
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


