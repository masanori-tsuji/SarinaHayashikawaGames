#ifndef _POWER_GAUGE_H_
#define _POWER_GAUGE_H_

class CPowergauge : public CScene
{
public:
	CPowergauge(int nPriority = 5);
	~CPowergauge();
	static HRESULT Load(void);
	static void Unload(void);
	static CPowergauge *Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, int nMaxlife);
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
		TYPE_MAX
	}TYPE;
	static LPDIRECT3DTEXTURE9   m_apTexture[TYPE_MAX];
	CScene2D                  * m_apScene2D[TYPE_MAX];
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //頂点座標
	D3DXVECTOR3                 m_pos;
	float                       m_fSizeWidth;
	float                       m_fSizeHeight;
	int                         m_nMaxPower;//最大ライフ
	int                         m_nPower;//現在のライフ
	int                     m_nPatternAnim; //アニメーションパターン
	int                     m_nCountAnim; //アニメーションパターン
};
#endif // !_BG_H_


