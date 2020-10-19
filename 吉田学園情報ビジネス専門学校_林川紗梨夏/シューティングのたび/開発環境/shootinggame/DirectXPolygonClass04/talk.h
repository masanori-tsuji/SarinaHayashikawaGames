#ifndef _TALK_H_
#define _TALK_H_

#define TEXTURE_chara001	 "data/TEXTURE/chara001_pose.png"
#define TEXTURE_chara002	 "data/TEXTURE/chara002_pose.png"
#define TEXTURE_chara003	 "data/TEXTURE/bosschara001.png"
#define TEXTURE_TALKUI	 "data/TEXTURE/talkui.png"
#define CHARA_POS_HEIGHT 800
#define CHARA001_POS_WIDTH 200
#define CHARA001_SIZE_WIDTH 800 * 0.8
#define CHARA001_SIZE_HEIGHT 1200 * 0.8

#define CHARA002_POS_WIDTH 200
#define CHARA002_SIZE_WIDTH 800 * 0.8
#define CHARA002_SIZE_HEIGHT 800 * 0.8

#define CHARA003_POS_WIDTH 950
#define CHARA003_SIZE_WIDTH 890 * 0.6
#define CHARA003_SIZE_HEIGHT 1400 * 0.6

#define WINDOW_POS_WIDTH 200
#define WINDOW_SIZE_WIDTH 900
#define WINDOW_SIZE_HEIGHT 180

#define TALKUI_SIZE_WIDTH 80
#define TALKUI_SIZE_HEIGHT 80

#define MAX_MSGUI 5
#define MAX_TEXT 100
class CTalk : public CScene
{
public:
	typedef enum
	{
		CHARA_NONE = -1,
		CHARA_001,
		CHARA_002,
		CHARA_003,
		CHARA_MAX
	}CTYPE;//キャラタイプ
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_001,
		TYPE_002,
		TYPE_003,
		TYPE_004,
		TYPE_005,
		TYPE_MAX
	}TYPE;//テクスチャタイプ
	CTalk(int nPriority = 4);
	~CTalk();
	static HRESULT Load(void);
	static void Unload(void);
	static CTalk *Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, int nTalkIndex, int nTalkEndIndex);
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
	void SetChara(CTYPE type);
private:
	void LoadFile(void);
	static LPDIRECT3DTEXTURE9   m_apTexture[TYPE_MAX];
	CScene2D                  * m_apScene2D[TYPE_MAX];
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //頂点座標
	D3DXVECTOR3                 m_pos;
	float                       m_fSizeWidth;
	float                       m_fSizeHeight;
	int                     m_nPatternAnim; //アニメーションパターン
	int                     m_nCountAnim; //アニメーションパターン
	bool                     m_bHitUi;
	CTYPE                    m_chara;//キャラクタータイプ
	char                     m_aText[MAX_TEXT][512];
	int                     m_aChara[MAX_TEXT];
	int                     m_aPattern[MAX_TEXT];
	int                      m_nTalkIndex;
	int                      m_nTalkEndIndex;
	int                      m_nAlpha;
	int                      m_nAddAlpha;
};
#endif // !_BG_H_


