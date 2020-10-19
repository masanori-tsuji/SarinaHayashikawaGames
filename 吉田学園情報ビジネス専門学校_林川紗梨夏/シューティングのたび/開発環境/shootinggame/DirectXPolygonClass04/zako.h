//=============================================================================
//
// ザコ敵の処理 [zako.h](弾撃たない敵)
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

#define MAX_LOCATION 4//移動ポイント最大
//プレイヤークラス
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
};
#endif // !_ENEMY_H_
