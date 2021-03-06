#ifndef _BG_H_
#define _BG_H_

#define MAX_BG 3
#define BG_UV_MOVE 0.001f
#define BG_UV_MOVE_01 0.0015f
#define BG_UV_MOVE_02 0.0017f
#define BG_UV_MOVE_2 1.0f

#define TEXTURE_BG2	     "data/TEXTURE/bg_0002.png"
#define TEXTURE_BG3      "data/TEXTURE/bg_0003.png"
#define TEXTURE_BG	     "data/TEXTURE/bg_001.png"
class CBg : public CScene
{
public:
	CBg(int nPriority = 1);
	~CBg();
	static HRESULT Load(void);
	static void Unload(void);
	static CBg *Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
private:
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_001  = 0,
		TYPE_002  = 1,
		TYPE_003,
	}TYPE;
	static LPDIRECT3DTEXTURE9   m_apTexture[MAX_BG];
	CScene2D                  * m_apScene2D[MAX_BG];
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //頂点座標
	D3DXVECTOR3                 m_pos;
	float                       m_fSizeWidth;
	float                       m_fSizeHeight;
	int                     m_nPatternAnim; //アニメーションパターン
	int                     m_nCountAnim; //アニメーションパターン
};
#endif // !_BG_H_


