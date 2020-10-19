//=============================================================================
//
// �G�t�F�N�g�̏��� [effect.h]
// Author : 
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#define TEXTURE_EFFECT_001 "data/TEXTURE/slime001.png"
#define TEXTURE_EFFECT_002 "data/TEXTURE/ball_cat.png"

//�v���C���[�N���X
class CEffect : public CScene2D
{
public:
	typedef enum
	{
		COLOR_NONE = -1,
		COLOR_RED,
		COLOR_ORANGE,
		COLOR_YELLOW,
		COLOR_GREEN,
		COLOR_BLUE,
		COLOR_PURPLE,
		COLOR_PINK,
		COLOR_MAX
	}COLOR;//7�F�J���[�v���Z�b�g

	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_001,
		TYPE_002,
		TYPE_003,
		TYPE_004,
		TYPE_MAX
	}TYPE;
	CEffect(int nPriority = 3);
	~CEffect();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, D3DXVECTOR3 move, TYPE Type,int col, int nLife);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEffect * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, D3DXVECTOR3 move, TYPE Type, int col,int nLife);
	static HRESULT Load(void);
	static void Unload(void);

private:
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //���_���W
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];   //�e�N�X�`���̃|�C���^
	float					m_fLength;		// �Ίp���̒���
	float					m_fAngle;		// �Ίp���̊p�x
	D3DXVECTOR3				m_rot;			// ����
	float					m_fScale;		// �g��
	float					m_fAddScale;	// �g����Z�l
	int                     m_RandColorR;   //�����_���J���[R
	int                     m_RandColorG;   //�����_���J���[G
	int                     m_RandColorB;   //�����_���J���[B
	float                   m_nAlpha;       //�����x
	int                     m_nPatternAnim; //�A�j���[�V�����p�^�[��
	int                     m_nCountAnim; //�A�j���[�V�����p�^�[��
	int                     m_nType;        //�G�^�C�v
	int                     m_nCountBullet;
	D3DXVECTOR3             m_TargetDistance;//�ڕW�܂ł̋���
	D3DXVECTOR3             m_TargetPos;//�ڕW�n�_
	D3DXVECTOR3             m_Move;//�ړ���
	static D3DCOLOR         m_aColor[COLOR_MAX];//7�F�v���Z�b�g
	D3DXCOLOR                m_Color;//�F
	int                     m_ColorNum; //�J���[�ԍ�
	int                     m_nCountDraw;
	int                     m_nLife;
};
#endif // !_EFFECT_H_
