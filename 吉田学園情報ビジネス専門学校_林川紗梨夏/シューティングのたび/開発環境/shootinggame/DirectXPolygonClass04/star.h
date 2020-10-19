#ifndef _STAR_H_
#define _STAR_H_

//#define NUM_VERTEX 4
//#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//#define NUM_POLYGON 2

#define TEXTURE_STAR	 "data/TEXTURE/star.png"

class CStar
{
public:
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_GLOW,
		TYPE_NOGLOW,
		TYPE_MAX
	}TYPE;

	CStar();
	~CStar();
	static CStar * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetStar(TYPE type);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9   m_pVtxBuff;
	int                       m_nPatternAnim;
};

#endif // !_NUMBER_H_