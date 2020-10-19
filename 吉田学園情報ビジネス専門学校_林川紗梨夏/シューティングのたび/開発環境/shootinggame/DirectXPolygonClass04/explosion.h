//=============================================================================
//
// �����̏��� [explosion.h]
// Author : 
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
#define TEXTURE_EXPLOSION "data/TEXTURE/explosion001.png"
#define EXPLOSION_SIZE 200
//�v���C���[�N���X
class CExplosion : public CScene2D
{
public:
	typedef enum 
	{
		TYPE_NONE = 0,
		TYPE_NORMAL,
		TYPE_BOMB,
		TYPE_RESPAWN,
	}TYPE;
	CExplosion(int nPriority = 3);
	~CExplosion();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight,TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CExplosion * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, TYPE type);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;   //�e�N�X�`���̃|�C���^
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //���_���W
	float                   m_fSizeWidth;
	float                   m_fSizeHeight;
	int                     m_RandColorR;   //�����_���J���[R
	int                     m_RandColorG;   //�����_���J���[G
	int                     m_RandColorB;   //�����_���J���[B
	int                     m_nPatternAnim; //�A�j���[�V�����p�^�[��
	int                     m_nCountAnim; //�A�j���[�V�����p�^�[��
	TYPE                    m_Type; //�������

	float                   m_fLength;		// �Ίp���̒���
	float                   m_fAngle;		// �Ίp���̊p�x
	D3DXVECTOR3             m_Pos;// �ʒu
	D3DXVECTOR3             m_rot;// ����
	float                   m_fScale;		// �g��
	float                   m_fAddScale;	// �g����Z�l
};
#endif // !_EXPLOSION_H_
