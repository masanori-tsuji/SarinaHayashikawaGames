
#ifndef _CONTINUE_H_
#define _CONTINUE_H_

#define TEXTURE_CONTINUE	 "data/TEXTURE/result.png" //背景
#define TEXTURE_BUTTON_CONTINUE	 "data/TEXTURE/continueyes.png" //コンティニュー
#define TEXTURE_BUTTON_END	 "data/TEXTURE/exit.png" //タイトルへ
#define TEXTURE_CONTINUE_UI	 "data/TEXTURE/continueui.png" //UI

class CContinue
{
public:
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_BG,
		TYPE_BUTTON_001,
		TYPE_BUTTON_002,
		TYPE_UI,
		TYPE_MAX
	}TYPE;//UI種類

	CContinue();
	~CContinue();
	static CContinue * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
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
	}IsPress;//押されてるか？
	IsPress m_IsTrigger;                                         //押されてるかどうか
	int     m_TriggerCount;                                     //押し中のカウンタ
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];
	LPDIRECT3DVERTEXBUFFER9   m_pVtxBuff;
	int                       m_nPatternAnim;//テクスチャパターン
	int                       m_nButtonPos;//ボタン位置
	int                       m_aButtonAlpha[TYPE_MAX];//ボタン透明度
};
#endif // !_PAUSE_H_