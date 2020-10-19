#ifndef _UI_H_
#define _UI_H_

#define MAX_UI 1
class CUi : public CScene
{
public:
	CUi(int nPriority = 5);
	~CUi();
	static HRESULT Load(void);
	static void Unload(void);
	static CUi *Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
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
		TYPE_002 = 1
	}TYPE;
	static LPDIRECT3DTEXTURE9   m_apTexture[MAX_UI];
	CScene2D                  * m_apScene2D[MAX_UI];
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //���_���W
	D3DXVECTOR3                 m_pos;
	float                       m_fSizeWidth;
	float                       m_fSizeHeight;
	int                     m_nPatternAnim; //�A�j���[�V�����p�^�[��
	int                     m_nCountAnim; //�A�j���[�V�����p�^�[��
};
#endif // !_BG_H_


