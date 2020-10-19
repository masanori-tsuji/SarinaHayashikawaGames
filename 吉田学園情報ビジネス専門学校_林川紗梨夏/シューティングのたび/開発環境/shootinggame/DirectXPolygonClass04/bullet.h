//=============================================================================
//
// 弾の処理 [bullet.h]
// Author : 
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#define BULLET_LIFE 100
#define BULLET_SIZE 40
#define BULLET_DIAMOND_SIZE 30
#define GRAZE_SIZE 50

#define TEXTURE_BULLET_001	 "data/TEXTURE/bullet_001.png"//ダイヤ弾
#define TEXTURE_BULLET_002	 "data/TEXTURE/bullet_maru.png"//まる弾
#define TEXTURE_BULLET_003	 "data/TEXTURE/middle_boss.png"//まる弾
//弾クラス
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
		BULLET_DIAMOND,//ダイヤ弾
		BULLET_CIRCLE,//丸玉
		BULLET_DOG,//丸玉
		BULLET_MAX
	}BULLETTYPE;
	typedef enum
	{
		ATTACK_NONE = 0,
		ATTACK_NORMAL,//ノーマル弾
		ATTACK_AIM,//時間差でエイム
		ATTACK_SPREAD,//時間差でバラバラ
		ATTACK_ROTATION, //回転
		ATTACK_BOUND, //跳ね返り弾
		ATTACK_BOTOBOTO, //ぼとぼと落とす弾
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
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //頂点座標
	static LPDIRECT3DTEXTURE9 m_pTexture[BULLET_MAX];   //テクスチャのポインタ
	D3DXVECTOR3             m_move;         // 移動量
	D3DXVECTOR3             m_CPos;         // 中心座標
	int                     m_nLife;        //弾の残存時間
	float                   m_fSizeWidth;
	float                   m_fSizeHeight;
	float					m_fLength;		// 対角線の長さ
	float					m_fAngle;		// 対角線の角度
	D3DXVECTOR3				m_rot;			// 向き
	float                   m_nColNum;
	float                   m_omega; //一回の移動で変化する角度
	float                   m_fRdScale;
	float					m_fScale;		// 拡大
	float					m_fAddScale;	// 拡大加算値
	int                     m_RandColorR;   // ランダムカラーR
	int                     m_RandColorG;   // ランダムカラーG
	int                     m_RandColorB;   // ランダムカラーB
	int                     m_nPatternAnim; // アニメーションパターン
	int                     m_nBulletNum;   // アニメーションパターン
	int                     m_nCountAnim;   // アニメーションパターン
	BULLET                  m_nType;        //弾タイプ
	ATTACKTYPE              m_nAttack;      //攻撃タイプ
	int                     m_nCount;
	int                     m_nEffectCnt;
	static bool                    m_bSpread;
	static D3DCOLOR         m_aColor[COLOR_MAX];//7色プリセット
	D3DCOLOR                m_Color;//色
	int                     m_nColorNum; //カラー番号
};
#endif // !_BULLET_H_
