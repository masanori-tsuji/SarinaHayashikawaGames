//=============================================================================
//
// プレイヤーの処理 [player.h]
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
//プレイヤークラス
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
	void IsInvincible(bool bInvincible) {  m_bInvincible = bInvincible;};//無敵か
	
	void Respawn(void);

private:

	typedef enum
	{
		TYPE_NONE = -1, 
		TYPE_PLAYER,
		TYPE_TURRET,
		TYPE_MAX
	}TYPE;
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //頂点座標
	D3DXVECTOR3               m_vPosTurret[NUM_VERTEX];       //頂点座標
	D3DXVECTOR3               m_pos;
	D3DXVECTOR3               m_TargetPos;//カーソル
	D3DXVECTOR3               m_Move;
	D3DXVECTOR3               m_BulletMove;
	int                       m_nLife; //プレイやーライフ
	int                       m_nBomb; //ボム数
	bool                      m_bExplosionBombCount;
	int                       m_nExplosionBombCount;
	CScene2D                 *m_pTurret;//砲塔ポインタ
	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   //テクスチャのポインタ
	float					m_fLength;		// 対角線の長さ
	float					m_fAngle;		// 対角線の角度
	D3DXVECTOR3				m_rot;			// 向き
	float                   m_fSpeed; //速さ
	D3DXVECTOR3             m_rotDest;
	int                     m_nNumContinue;//残りこんてぃにゅー回数
	float					m_fScale;		// 拡大
	float					m_fAddScale;	// 拡大加算値
	float                   m_fTurretRot; //砲塔向き
	D3DXVECTOR3             m_TurretrotDest;
	float					m_fTurretLength;		// 対角線の長さ
	float					m_fTurretAngle;		// 対角線の角度
	float					m_fTurretScale;		// 拡大
	int                     m_RandColorR;   //ランダムカラーR
	int                     m_RandColorG;   //ランダムカラーG
	int                     m_RandColorB;   //ランダムカラーB
	int                     m_nPatternAnim; //アニメーションパターン
	int                     m_nCountAnim; //アニメーションパターン
	int                     m_nCounterBullet; //弾カウント
	bool                    m_bShot; //射撃しているか
	bool                    m_bInvincible; //無敵か
	int                     m_CountInvincible; //無敵時間
	int                     m_CountTime;
	int                     m_nRespawnTime; //復活までの時間
	bool                    m_bStun; //行動不能か
	int                     m_nPower;
};
#endif // !_PLAYER_H_
