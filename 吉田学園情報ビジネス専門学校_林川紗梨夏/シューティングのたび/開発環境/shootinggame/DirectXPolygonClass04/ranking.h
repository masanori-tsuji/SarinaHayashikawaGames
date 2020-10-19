//=============================================================================
//
// �����L���O [ranking.h]
// Author : �ѐ�ї���
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#define MAX_RANKING 10				// �����L���O�f�[�^��
#define MAX_NAME 8			// ���O�T�C�Y

#define MAX_BG_RANKING 3
#define BG_UV_MOVE_RANKING 0.001f
#define BG_UV_MOVE_01_RANKING 0.0015f
#define BG_UV_MOVE_02_RANKING 1.0f
#define BG_UV_MOVE_2_RANKING 1.0f

#define RANKING_02_SIZE_HEIGHT 200.0f

#define TEXTURE_BG2_RANKING	     "data/TEXTURE/ranking_001.png"
#define TEXTURE_BG_RANKING	     "data/TEXTURE/result.png"

#define NAME "test"
#define SERVER_IP_NUM "127.0.0.1"
#define SERVER_PORT_NUM 12345
#define BUFFER_NUM 1024

class CRanking : public CScene
{
public:
	typedef struct
	{
		int nScore;
		char aName[MAX_NAME];
	} RankingData;
	typedef enum
	{
		COMMAND_TYPE_NONE = 0,
		COMMAND_TYPE_GET_RANKING,		// �����L���O�ꗗ���擾
		COMMAND_TYPE_SET_RANKING		// �����L���O��ݒ�
	} COMMAND_TYPE;
	CRanking(int nPriority = 1);
	~CRanking();
	static HRESULT Load(void);
	static void Unload(void);
	static CRanking *Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
	static RankingData * GetRankingData(void) { return m_aRankingData; }
	void    GetRanking(RankingData * pRankingData);
	int     SetRanking(int nScore,char aName[MAX_NAME]);
private:
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_001,
		TYPE_002,
		TYPE_MAX,
	}TYPE;
	static LPDIRECT3DTEXTURE9   m_apTexture[TYPE_MAX];
	CScene2D                  * m_apScene2D[TYPE_MAX];
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //���_���W
	D3DXVECTOR3                 m_pos;
	float                       m_fSizeWidth;
	float                       m_fSizeHeight;
	int                     m_nPatternAnim; //�A�j���[�V�����p�^�[��
	int                     m_nCountAnim; //�A�j���[�V�����p�^�[��
	static RankingData              m_aRankingData[MAX_RANKING];//�����L���O�f�[�^
};
#endif // !_BG_H_


