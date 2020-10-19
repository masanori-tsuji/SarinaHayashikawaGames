#ifndef _BATTLE_BG_H_
#define _BATTLE_BG_H_

#define MAX_BATTLE_BG 3
#define BATTLE_BG_UV_MOVE 0.001f
#define BATTLE_BG_UV_MOVE_01 0.0005f
#define BATTLE_BG_UV_MOVE_02 0.0017f
#define BATTLE_BG_UV_MOVE_2 1.0f

#define TEXTURE_BATTLE_BG2	     "data/TEXTURE/bg_001.png"
#define TEXTURE_BATTLE_BG3      "data/TEXTURE/bg_0003.png"
#define TEXTURE_BATTLE_BG	     "data/TEXTURE/battlebg001.png"
class CBattlebg : public CScene
{
public:
	CBattlebg(int nPriority = 1);
	~CBattlebg();
	static HRESULT Load(void);
	static void Unload(void);
	static CBattlebg *Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
private:
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_001 = 0,
		TYPE_002 = 1,
		TYPE_003,
	}TYPE;
	static LPDIRECT3DTEXTURE9   m_apTexture[MAX_BATTLE_BG];
	CScene2D                  * m_apScene2D[MAX_BATTLE_BG];

	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //頂点座標
	D3DXVECTOR3                 m_pos;
	float                       m_fSizeWidth;
	float                       m_fSizeHeight;
	int                     m_nPatternAnim; //アニメーションパターン
	int                     m_nCountAnim; //アニメーションパターン
	int                     m_nCountTime;
	int                     m_nAlpha;
	float m_fLength;
	float m_fAngle;
	D3DXVECTOR3 m_rot;
	float m_fScale;
	float m_fAddScale;
};
#endif // !_BG_H_


