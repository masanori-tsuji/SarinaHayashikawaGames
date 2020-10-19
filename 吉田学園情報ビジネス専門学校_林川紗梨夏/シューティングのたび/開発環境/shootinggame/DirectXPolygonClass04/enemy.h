//=============================================================================
//
// 敵の処理 [enemy.h]
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

#define MAX_LOCATION 4//移動ポイント最大
//プレイヤークラス
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
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //頂点座標
	D3DXVECTOR3             m_location[MAX_LOCATION];//移動地点
	D3DXVECTOR3             m_Pos;//移動量
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];   //テクスチャのポインタ
	int                     m_nLife;
	float					m_fLength;		// 対角線の長さ
	float					m_fAngle;		// 対角線の角度
	D3DXVECTOR3				m_rot;			// 向き
	float					m_fScale;		// 拡大
	float					m_fAddScale;	// 拡大加算値
	int                     m_RandColorR;   //ランダムカラーR
	int                     m_RandColorG;   //ランダムカラーG
	int                     m_RandColorB;   //ランダムカラーB
	int                     m_nPatternAnim; //アニメーションパターン
	int                     m_nCountAnim; //アニメーションパターン
	int                     m_nConutTime;
	int                     m_nType;        //敵タイプ
	int                     m_nCountBullet;
	D3DXVECTOR3             m_TargetDistance;//目標までの距離
	D3DXVECTOR3             m_TargetPos;//目標地点
	D3DXVECTOR3             m_Move;//移動量
	int                     m_nColor;//カラー番号
	float                   m_fMovetime;//移動時間
	bool                    m_bHit;
	int                     m_nHitCount;
};
#endif // !_ENEMY_H_
