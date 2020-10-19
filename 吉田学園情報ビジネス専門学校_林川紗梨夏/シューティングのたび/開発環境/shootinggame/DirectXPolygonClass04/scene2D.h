//=============================================================================
//
// �V�[������ [scene2D.h]
// Author : 
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//2D�I�u�W�F�N�g�N���X
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
	LPDIRECT3DTEXTURE9                 m_pTexture;    //�e�N�X�`���|�C���^
	LPDIRECT3DVERTEXBUFFER9            m_pVtxBuff;    //���_�o�b�t�@�|�C���^
private:
	D3DXVECTOR3                        m_vPos[NUM_VERTEX];       //���_���W
	D3DXVECTOR3                        m_pos;        //�ʒu

	float                              m_fTexX;      //�e�N�X�`��X���W
	float                              m_fTexX2;      //�e�N�X�`��X2���W
	float                              m_fTexY;      //�e�N�X�`��Y���W
	float                              m_fTexY2;      //�e�N�X�`��Y���W
	D3DXCOLOR                          m_Color;      //����
	static LPDIRECT3DTEXTURE9          m_pSceneTexture; //�V�[���̃e�N�X�`��
};
#endif // !_SCENE2D_H_
