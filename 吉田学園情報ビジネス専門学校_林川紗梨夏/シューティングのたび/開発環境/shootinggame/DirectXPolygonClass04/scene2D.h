//=============================================================================
//
// シーン処理 [scene2D.h]
// Author : 
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//2Dオブジェクトクラス
class CScene2D : public CScene
{
public:
	typedef struct
	{
		float                              x;
		float                              x2;
		float                              x3;
		float                              x4;
		float                              y;
		float                              y2;
		float                              y3;
		float                              y4;
	}VERTEX_POS;

	CScene2D(int nPriority = 3);
	~CScene2D();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; };
	D3DXVECTOR3 GetPosition(void) { return m_pos; };
	static CScene2D * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	static HRESULT Load(void);
	static void Unload(void);
	void SetVertex(VERTEX_2D * pSVtx);
	void SetTex(float fTexX, float fTexY,float fTexX2, float fTexY2);
	void SetColor(D3DCOLOR col);
	void SetVtxPosition(D3DXVECTOR3 vPos[NUM_VERTEX]);
protected:
	LPDIRECT3DTEXTURE9                 m_pTexture;    //テクスチャポインタ
	LPDIRECT3DVERTEXBUFFER9            m_pVtxBuff;    //頂点バッファポインタ
private:
	D3DXVECTOR3                        m_vPos[NUM_VERTEX];       //頂点座標
	D3DXVECTOR3                        m_pos;        //位置

	float                              m_fTexX;      //テクスチャX座標
	float                              m_fTexX2;      //テクスチャX2座標
	float                              m_fTexY;      //テクスチャY座標
	float                              m_fTexY2;      //テクスチャY座標
	D3DXCOLOR                          m_Color;      //いろ
	static LPDIRECT3DTEXTURE9          m_pSceneTexture; //シーンのテクスチャ
};
#endif // !_SCENE2D_H_
