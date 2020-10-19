//=============================================================================
//
// ポーズ [pause.h]
// Author : 林川紗梨夏
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//#define NUM_VERTEX 4
//#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//#define NUM_POLYGON 2

#define TEXTURE_PAUSE	 "data/TEXTURE/result.png" //背景
#define TEXTURE_BUTTON_BACK	 "data/TEXTURE/backgame.png" //ゲームに戻る
#define TEXTURE_BUTTON_RESTART	 "data/TEXTURE/restart.png" //最初から
#define TEXTURE_BUTTON_EXIT	 "data/TEXTURE/exit.png" //タイトルへ
#define TEXTURE_PAUSE_UI	 "data/TEXTURE/pauseui.png" //UI

class CPause
{
public:
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_BG,
		TYPE_BUTTON_001,
		TYPE_BUTTON_002,
		TYPE_BUTTON_003,
		TYPE_004,
		TYPE_MAX
	}TYPE;

	CPause();
	~CPause();
	static CPause * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
	typedef struct
	{
		int nCount;
		bool bTrigger;
	}IsCount;//推されてるか？と押し中カウント
	typedef struct
	{
		IsCount Up;
		IsCount Down;
		IsCount Right;
		IsCount Left;
	}IsPress;//推されてるか？
	IsPress m_IsTrigger;                                         //推されてるかどうか
	int     m_TriggerCount;                                     //押し中のカウンタ
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];
	LPDIRECT3DVERTEXBUFFER9   m_pVtxBuff;
	int                       m_nPatternAnim;
	int                       m_nButtonPos;
	int                       m_aButtonAlpha[TYPE_MAX];
};
#endif // !_PAUSE_H_