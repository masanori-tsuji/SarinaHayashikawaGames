//=============================================================================
//
// �U�R�G�̏��� [zako.h](�e�����Ȃ��G)
// Author : 
//
//=============================================================================
#ifndef _ZAKO_H_
#define _ZAKO_H_


#define TEXTURE_ENEMY_ZAKO_001 "data/TEXTURE/enemy002.png"

#define TEXTURE_ENEMY_ZAKO_002 "data/TEXTURE/enemy003.png"
#define TEXTURE_ENEMY_ZAKO "data/TEXTURE/enemy002.png"
#define ZAKO_SIZE_001_WIDTH 200 / 3
#define ZAKO_SIZE_001_HEIGHT 200 / 3
#define ZAKO_SIZE_001_COLLISION 30

#define MAX_LOCATION 4//�ړ��|�C���g�ő�
//�v���C���[�N���X
class CZako : public CScene2D
{
public:
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_001,
		TYPE_002,
		TYPE_003,
		TYPE_004,
		TYPE_MAX
	}TYPE;
	CZako(int nPriority = 4);
	~CZako();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeWidth, float fSizeHeight, TYPE Type, D3DXVECTOR3 location[MAX_LOCATION]);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CZako * Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeWidth, float fSizeHeight, TYPE Type, D3DXVECTOR3 location[MAX_LOCATION]);
	static HRESULT Load(void);
	static void Unload(void);
	void SubLife(int SubLife);
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
};
#endif // !_ENEMY_H_
