//=============================================================================
//
// �A�C�e���̏��� [item.h]
// Author : 
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_


#define TEXTURE_ITEM_001 "data/TEXTURE/power.png"
#define TEXTURE_ITEM_002 "data/TEXTURE/life.png"
#define TEXTURE_ITEM_003 "data/TEXTURE/score.png"
#define TEXTURE_ITEM_004 "data/TEXTURE/enemy_004.png"
#define ITEM_SIZE 50
#define ITEM_LIFE 1000

//�v���C���[�N���X
class CItem : public CScene2D
{
public:
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_POWER,
		TYPE_LIFE,
		TYPE_SCORE,
		TYPE_BOMB,
		TYPE_MAX
	}TYPE;
	CItem(int nPriority = 3);
	~CItem();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, TYPE Type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CItem * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, TYPE Type);
	static HRESULT Load(void);
	static void Unload(void);

private:
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //���_���W
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];   //�e�N�X�`���̃|�C���^
	int                     m_nLife;
	float					m_fLength;		// �Ίp���̒���
	float					m_fAngle;		// �Ίp���̊p�x
	D3DXVECTOR3				m_rot;			// ����
	float					m_fScale;		// �g��
	float					m_fAddScale;	// �g����Z�l
	int                     m_RandColorR;   //�����_���J���[R
	int                     m_RandColorG;   //�����_���J���[G
	int                     m_RandColorB;   //�����_���J���[B
	int                     m_nPatternAnim; //�A�j���[�V�����p�^�[��
	int                     m_nCountAnim; //�A�j���[�V�����p�^�[��
	int                     m_nType;        //�G�^�C�v
	float                     m_fSize;
	D3DXVECTOR3             m_TargetDistance;//�ڕW�܂ł̋���
	D3DXVECTOR3             m_TargetPos;//�ڕW�n�_
	D3DXVECTOR3             m_Move;//�ړ���
};
#endif // !_ITEM_H_
