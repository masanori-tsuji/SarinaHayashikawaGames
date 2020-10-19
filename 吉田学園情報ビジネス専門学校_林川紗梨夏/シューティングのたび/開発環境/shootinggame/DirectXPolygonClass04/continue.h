
#ifndef _CONTINUE_H_
#define _CONTINUE_H_

#define TEXTURE_CONTINUE	 "data/TEXTURE/result.png" //�w�i
#define TEXTURE_BUTTON_CONTINUE	 "data/TEXTURE/continueyes.png" //�R���e�B�j���[
#define TEXTURE_BUTTON_END	 "data/TEXTURE/exit.png" //�^�C�g����
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
	}TYPE;//UI���

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
	}IsCount;//������Ă邩�H�Ɖ������J�E���g
	typedef struct
	{
		IsCount Up;
		IsCount Down;
		IsCount Right;
		IsCount Left;
	}IsPress;//������Ă邩�H
	IsPress m_IsTrigger;                                         //������Ă邩�ǂ���
	int     m_TriggerCount;                                     //�������̃J�E���^
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];
	LPDIRECT3DVERTEXBUFFER9   m_pVtxBuff;
	int                       m_nPatternAnim;//�e�N�X�`���p�^�[��
	int                       m_nButtonPos;//�{�^���ʒu
	int                       m_aButtonAlpha[TYPE_MAX];//�{�^�������x
};
#endif // !_PAUSE_H_