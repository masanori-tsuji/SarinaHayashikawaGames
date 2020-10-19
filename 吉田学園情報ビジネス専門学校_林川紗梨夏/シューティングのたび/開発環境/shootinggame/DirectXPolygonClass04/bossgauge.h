#ifndef _BOSS_GAUGE_H_
#define _BOSS_GAUGE_H_

#define TEXTURE_GAUGE	 "data/TEXTURE/gauge.png"
#define TEXTURE_GAUGE_002 "data/TEXTURE/star.png"


class CBossgauge : public CScene
{
public:
	CBossgauge(int nPriority = 5);
	~CBossgauge();
	static HRESULT Load(void);
	static void Unload(void);
	static CBossgauge *Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight,int nMaxlife);
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
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //���_���W
	D3DXVECTOR3                 m_pos;
	float                       m_fSizeWidth;
	float                       m_fSizeHeight;
	int                         m_nMaxLife;//�ő僉�C�t
	int                         m_nLife;//���݂̃��C�t
	int                     m_nPatternAnim; //�A�j���[�V�����p�^�[��
	int                     m_nCountAnim; //�A�j���[�V�����p�^�[��
};
#endif // !_BG_H_


