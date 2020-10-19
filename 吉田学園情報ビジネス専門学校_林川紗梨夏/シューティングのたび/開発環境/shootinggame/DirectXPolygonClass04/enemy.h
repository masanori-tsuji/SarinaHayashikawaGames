//=============================================================================
//
// �G�̏��� [enemy.h]
// Author : 
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_


#define TEXTURE_ENEMY_001 "data/TEXTURE/enemyship001.png"
#define TEXTURE_ENEMY_002 "data/TEXTURE/enemyship001.png"
#define TEXTURE_ENEMY_ZAKO "data/TEXTURE/enemy002.png"
#define ENEMY_SIZE_001_WIDTH 300 / 3
#define ENEMY_SIZE_001_HEIGHT 230 / 3
#define ENEMY_SIZE_001_COLLISION 100

#define MAX_LOCATION 4//�ړ��|�C���g�ő�
//�v���C���[�N���X
class CEnemy : public CScene2D
{
public:
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_001,
		TYPE_002,
		TYPE_003,
		TYPE_ZAKO,
		TYPE_MIDDLE_BOSS,
		TYPE_MAX
	}TYPE;
	CEnemy(int nPriority = 4);
	~CEnemy();
	HRESULT Init(D3DXVECTOR3 pos,D3DXVECTOR3 move, float fSizeWidth, float fSizeHeight, TYPE Type, D3DXVECTOR3 location[MAX_LOCATION],int nLife);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEnemy * Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeWidth, float fSizeHeight, TYPE Type, D3DXVECTOR3 location[MAX_LOCATION], int nLife);
	static HRESULT Load(void);
	static void Unload(void);
	void SubLife(int SubLife) ;
	void SetMove(D3DXVECTOR3 move) { m_Move = move; };
private:
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //���_���W
	D3DXVECTOR3             m_location[MAX_LOCATION];//�ړ��n�_
	D3DXVECTOR3             m_Pos;//�ړ���
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
	int                     m_nConutTime;
	int                     m_nType;        //�G�^�C�v
	int                     m_nCountBullet;
	D3DXVECTOR3             m_TargetDistance;//�ڕW�܂ł̋���
	D3DXVECTOR3             m_TargetPos;//�ڕW�n�_
	D3DXVECTOR3             m_Move;//�ړ���
	int                     m_nColor;//�J���[�ԍ�
	float                   m_fMovetime;//�ړ�����
	bool                    m_bHit;
	int                     m_nHitCount;
};
#endif // !_ENEMY_H_
