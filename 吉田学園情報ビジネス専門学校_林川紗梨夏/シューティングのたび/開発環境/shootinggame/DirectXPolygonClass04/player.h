//=============================================================================
//
// �v���C���[�̏��� [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#define VALUE_MOVE 10.0f
#define VALUE_MOVE_SLOW 4.0f
#define PLAYER_COLLISION 35.0f
#define PLAYER_INITLIFE 4
#define PLAYER_MAXPOWER 100
#define PLAYER_CONTINUE 3
//�v���C���[�N���X
class CPlayer : public CScene2D
{
public:

	CPlayer(int nPriority = 4);
	~CPlayer();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPlayer * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	static HRESULT Load(void);
	static void Unload(void);
	void AddLife(int nLife);
	void SubLife(int nLife);
	void AddBomb(int nBomb);
	void SubBomb(int nBomb);
	void AddPower(int nPow);
	void SubPower(int nPow);
	void ExplosionBomb(D3DXVECTOR3 pos);
	int GetPower(void) { return m_nPower; };
	int GetContinue(void) {
		return m_nNumContinue;
	};

	D3DXVECTOR3 GetPosition(void) { return m_pos; };
	bool IsInvincible() { return m_bInvincible; };
	void IsInvincible(bool bInvincible) {  m_bInvincible = bInvincible;};//���G��
	
	void Respawn(void);

private:

	typedef enum
	{
		TYPE_NONE = -1, 
		TYPE_PLAYER,
		TYPE_TURRET,
		TYPE_MAX
	}TYPE;
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //���_���W
	D3DXVECTOR3               m_vPosTurret[NUM_VERTEX];       //���_���W
	D3DXVECTOR3               m_pos;
	D3DXVECTOR3               m_TargetPos;//�J�[�\��
	D3DXVECTOR3               m_Move;
	D3DXVECTOR3               m_BulletMove;
	int                       m_nLife; //�v���C��[���C�t
	int                       m_nBomb; //�{����
	bool                      m_bExplosionBombCount;
	int                       m_nExplosionBombCount;
	CScene2D                 *m_pTurret;//�C���|�C���^
	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   //�e�N�X�`���̃|�C���^
	float					m_fLength;		// �Ίp���̒���
	float					m_fAngle;		// �Ίp���̊p�x
	D3DXVECTOR3				m_rot;			// ����
	float                   m_fSpeed; //����
	D3DXVECTOR3             m_rotDest;
	int                     m_nNumContinue;//�c�肱��Ă��ɂ�[��
	float					m_fScale;		// �g��
	float					m_fAddScale;	// �g����Z�l
	float                   m_fTurretRot; //�C������
	D3DXVECTOR3             m_TurretrotDest;
	float					m_fTurretLength;		// �Ίp���̒���
	float					m_fTurretAngle;		// �Ίp���̊p�x
	float					m_fTurretScale;		// �g��
	int                     m_RandColorR;   //�����_���J���[R
	int                     m_RandColorG;   //�����_���J���[G
	int                     m_RandColorB;   //�����_���J���[B
	int                     m_nPatternAnim; //�A�j���[�V�����p�^�[��
	int                     m_nCountAnim; //�A�j���[�V�����p�^�[��
	int                     m_nCounterBullet; //�e�J�E���g
	bool                    m_bShot; //�ˌ����Ă��邩
	bool                    m_bInvincible; //���G��
	int                     m_CountInvincible; //���G����
	int                     m_CountTime;
	int                     m_nRespawnTime; //�����܂ł̎���
	bool                    m_bStun; //�s���s�\��
	int                     m_nPower;
};
#endif // !_PLAYER_H_
