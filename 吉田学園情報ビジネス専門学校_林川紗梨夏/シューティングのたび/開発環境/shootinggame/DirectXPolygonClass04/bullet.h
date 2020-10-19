//=============================================================================
//
// �e�̏��� [bullet.h]
// Author : 
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#define BULLET_LIFE 100
#define BULLET_SIZE 40
#define BULLET_DIAMOND_SIZE 30
#define GRAZE_SIZE 50

#define TEXTURE_BULLET_001	 "data/TEXTURE/bullet_001.png"//�_�C���e
#define TEXTURE_BULLET_002	 "data/TEXTURE/bullet_maru.png"//�܂�e
#define TEXTURE_BULLET_003	 "data/TEXTURE/middle_boss.png"//�܂�e
//�e�N���X
class CBullet : public CScene2D
{
public:
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_ENEMY,
		TYPE_PLAYER,
		TYPE_MAX
	}BULLET;
	typedef enum
	{
		BULLET_NONE = 0,
		BULLET_DIAMOND,//�_�C���e
		BULLET_CIRCLE,//�ۋ�
		BULLET_DOG,//�ۋ�
		BULLET_MAX
	}BULLETTYPE;
	typedef enum
	{
		ATTACK_NONE = 0,
		ATTACK_NORMAL,//�m�[�}���e
		ATTACK_AIM,//���ԍ��ŃG�C��
		ATTACK_SPREAD,//���ԍ��Ńo���o��
		ATTACK_ROTATION, //��]
		ATTACK_BOUND, //���˕Ԃ�e
		ATTACK_BOTOBOTO, //�ڂƂڂƗ��Ƃ��e
		ATTACK_MAX
	}ATTACKTYPE;

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
	}COLOR;

	CBullet(int nPriority = 4);
	~CBullet();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, D3DXVECTOR3 move, BULLET type, BULLETTYPE bullettype, int col, ATTACKTYPE attacktype);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBullet * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight,D3DXVECTOR3 move, BULLET type, BULLETTYPE bullettype,int col, ATTACKTYPE attacktype);
	static HRESULT Load(void);
	static void Unload(void);
	void SpreadBullet(int nRandSeed);

private:
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //���_���W
	static LPDIRECT3DTEXTURE9 m_pTexture[BULLET_MAX];   //�e�N�X�`���̃|�C���^
	D3DXVECTOR3             m_move;         // �ړ���
	D3DXVECTOR3             m_CPos;         // ���S���W
	int                     m_nLife;        //�e�̎c������
	float                   m_fSizeWidth;
	float                   m_fSizeHeight;
	float					m_fLength;		// �Ίp���̒���
	float					m_fAngle;		// �Ίp���̊p�x
	D3DXVECTOR3				m_rot;			// ����
	float                   m_nColNum;
	float                   m_omega; //���̈ړ��ŕω�����p�x
	float                   m_fRdScale;
	float					m_fScale;		// �g��
	float					m_fAddScale;	// �g����Z�l
	int                     m_RandColorR;   // �����_���J���[R
	int                     m_RandColorG;   // �����_���J���[G
	int                     m_RandColorB;   // �����_���J���[B
	int                     m_nPatternAnim; // �A�j���[�V�����p�^�[��
	int                     m_nBulletNum;   // �A�j���[�V�����p�^�[��
	int                     m_nCountAnim;   // �A�j���[�V�����p�^�[��
	BULLET                  m_nType;        //�e�^�C�v
	ATTACKTYPE              m_nAttack;      //�U���^�C�v
	int                     m_nCount;
	int                     m_nEffectCnt;
	static bool                    m_bSpread;
	static D3DCOLOR         m_aColor[COLOR_MAX];//7�F�v���Z�b�g
	D3DCOLOR                m_Color;//�F
	int                     m_nColorNum; //�J���[�ԍ�
};
#endif // !_BULLET_H_
