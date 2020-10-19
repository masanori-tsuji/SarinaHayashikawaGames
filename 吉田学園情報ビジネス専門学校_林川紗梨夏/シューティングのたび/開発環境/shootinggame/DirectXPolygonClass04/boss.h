//=============================================================================
//
// 敵の処理 [enemy.h]
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
#define MAX_LOCATION 4//移動ポイント最大
//ボスクラス
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
		MODE_NORMAL,//ノーマル状態
		MODE_MONSTER,//化け物状態
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
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //頂点座標
	D3DXVECTOR3             m_location[MAX_LOCATION];//移動地点
	D3DXVECTOR3             m_Pos;//移動量
	D3DXVECTOR3             m_PosOld;//移動量1フレーム前
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];   //テクスチャのポインタ
	int                     m_nLife;
	int                     m_nLifeCount; //フェーズによってのライフ制御変数
	float					m_fLength;		// 対角線の長さ
	float					m_fAngle;		// 対角線の角度
	D3DXVECTOR3				m_rot;			// 向き
	float					m_fScale;		// 拡大
	float					m_fAddScale;	// 拡大加算値
	int                     m_nPatternAnim; //アニメーションパターン
	int                     m_nCountAnim; //アニメーションパターン
	int                     m_nConutTime;
	int                     m_nMoveIndex;//移動方向をきめる
	float                    m_fSpeed;
	int                     m_nType;        //敵タイプ
	int                     m_nCountBullet;
	int                     m_nBulletShotinterval;//発射までのインターバル
	int                     m_nCountTime;
	BOSSMODE                m_bossmode;//現在のボス形態
	D3DXVECTOR3             m_TargetDistance;//目標までの距離
	D3DXVECTOR3             m_TargetPos;//目標地点
	D3DXVECTOR3             m_Move;//移動量
	int                     m_nColor;//カラー番号
	float                   m_fMovetime;//移動時間        
	bool                    m_bHit;
	int                     m_nHitCount;
};
#endif // !_ENEMY_H_
