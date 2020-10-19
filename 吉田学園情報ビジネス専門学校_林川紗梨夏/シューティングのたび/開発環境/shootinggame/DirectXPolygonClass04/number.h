#ifndef _NUMBER_H_
#define _NUMBER_H_

//#define NUM_VERTEX 4
//#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//#define NUM_POLYGON 2

#define TEXTURE_NUMBER	 "data/TEXTURE/number001.png"

class CNumber
{
public:
	CNumber();
	~CNumber();
	static CNumber * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumber(int nNumber);
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9   m_pVtxBuff;
	int                       m_nPatternAnim;
};

#endif // !_NUMBER_H_