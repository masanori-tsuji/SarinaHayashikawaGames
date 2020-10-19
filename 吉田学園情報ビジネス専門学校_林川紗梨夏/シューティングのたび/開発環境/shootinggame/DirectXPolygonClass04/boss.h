//=============================================================================
//
// �G�̏��� [enemy.h]
// Author : 
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_


#define TEXTURE_BOSS_001 "data/TEXTURE/middle_boss.png"
#define TEXTURE_BOSS_002 "data/TEXTURE/bosschara001game.png"
#define TEXTURE_BOSS_003 "data/TEXTURE/enemy002.png"
#define BOSS_SIZE_001_WIDTH 200
#define BOSS_SIZE_001_HEIGHT 200
#define BOSS_SIZE_001_COLLISION 100
#define MIDDLEBOSS_LIFE 3000
#define MAINBOSS_LIFE 6000
#define MAX_LOCATION 4//�ړ��|�C���g�ő�
//�{�X�N���X
class CBoss : public CScene2D
{
public:
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_MIDDLE_BOSS,
		TYPE_MAIN_BOSS,
		TYPE_MAX
	}TYPE;
	typedef enum
	{
		MODE_NONE = -1,
		MODE_NORMAL,//�m�[�}�����
		MODE_MONSTER,//���������
		MODE_MAX
	}BOSSMODE;
	CBoss(int nPriority = 4);
	~CBoss();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeWidth, float fSizeHeight, TYPE Type, D3DXVECTOR3 location[MAX_LOCATION], int nLife);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBoss * Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeWidth, float fSizeHeight, TYPE Type, D3DXVECTOR3 location[MAX_LOCATION], int nLife);
	static HRESULT Load(void);
	static void Unload(void);
	void SubLife(int SubLife);
	int GetLife(void) { return m_nLife; };
	void SetMove(D3DXVECTOR3 move) { m_Move = move; };
private:
	void ClearAllBullet(void);
	void PhaseOne(void);
	void PhaseTwo(void);
	void PhaseThree(void);
	void PhaseFour(void);
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //���_���W
	D3DXVECTOR3             m_location[MAX_LOCATION];//�ړ��n�_
	D3DXVECTOR3             m_Pos;//�ړ���
	D3DXVECTOR3             m_PosOld;//�ړ���1�t���[���O
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];   //�e�N�X�`���̃|�C���^
	int                     m_nLife;
	int                     m_nLifeCount; //�t�F�[�Y�ɂ���Ẵ��C�t����ϐ�
	float					m_fLength;		// �Ίp���̒���
	float					m_fAngle;		// �Ίp���̊p�x
	D3DXVECTOR3				m_rot;			// ����
	float					m_fScale;		// �g��
	float					m_fAddScale;	// �g����Z�l
	int                     m_nPatternAnim; //�A�j���[�V�����p�^�[��
	int                     m_nCountAnim; //�A�j���[�V�����p�^�[��
	int                     m_nConutTime;
	int                     m_nMoveIndex;//�ړ����������߂�
	float                    m_fSpeed;
	int                     m_nType;        //�G�^�C�v
	int                     m_nCountBullet;
	int                     m_nBulletShotinterval;//���˂܂ł̃C���^�[�o��
	int                     m_nCountTime;
	BOSSMODE                m_bossmode;//���݂̃{�X�`��
	D3DXVECTOR3             m_TargetDistance;//�ڕW�܂ł̋���
	D3DXVECTOR3             m_TargetPos;//�ڕW�n�_
	D3DXVECTOR3             m_Move;//�ړ���
	int                     m_nColor;//�J���[�ԍ�
	float                   m_fMovetime;//�ړ�����        
	bool                    m_bHit;
	int                     m_nHitCount;
};
#endif // !_ENEMY_H_
