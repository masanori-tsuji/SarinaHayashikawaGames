#ifndef _MSGWINDOW_H_
#define _MSGWINDOW_H_

#define TEXTURE_MSG001	 "data/TEXTURE/window01_down.png"
#define TEXTURE_MSG002	 "data/TEXTURE/charaicon1.png"
#define TEXTURE_MSG003	 "data/TEXTURE/window01_up.png"
#define TEXTURE_MSG004	 "data/TEXTURE/window02_down.png"
#define TEXTURE_MSG005	 "data/TEXTURE/charaicon2.png"
#define TEXTURE_MSG006	 "data/TEXTURE/window02_up.png"

#define CHARA_SIZE_WIDTH 570 / 3
#define CHARA_SIZE_HEIGHT 418 /3

#define CHARA_SIZE_HEIGHT02 502 /3

#define MAX_MSGUI 5
class CMsgWindow : public CScene
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
		TYPE_006,
		TYPE_MAX
	}TYPE;//テクスチャタイプ
	CMsgWindow(int nPriority = 5);
	~CMsgWindow();
	static HRESULT Load(void);
	static void Unload(void);
	static CMsgWindow *Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
	void SetChara(CTYPE type);
	void HitMsgUi(bool bHit);
	void SetMsgUi(int UiIndex);
private:
	
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
};
#endif // !_BG_H_


