//=============================================================================
//
// アイテムの処理 [item.h]
// Author : 
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_


#define TEXTURE_ITEM_001 "data/TEXTURE/power.png"
#define TEXTURE_ITEM_002 "data/TEXTURE/life.png"
#define TEXTURE_ITEM_003 "data/TEXTURE/score.png"
#define TEXTURE_ITEM_004 "data/TEXTURE/enemy_004.png"
#define ITEM_SIZE 50
#define ITEM_LIFE 1000

//プレイヤークラス
class CItem : public CScene2D
{
public:
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_POWER,
		TYPE_LIFE,
		TYPE_SCORE,
		TYPE_BOMB,
		TYPE_MAX
	}TYPE;
	CItem(int nPriority = 3);
	~CItem();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, TYPE Type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CItem * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, TYPE Type);
	static HRESULT Load(void);
	static void Unload(void);

private:
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //頂点座標
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
	int                     m_nType;        //敵タイプ
	float                     m_fSize;
	D3DXVECTOR3             m_TargetDistance;//目標までの距離
	D3DXVECTOR3             m_TargetPos;//目標地点
	D3DXVECTOR3             m_Move;//移動量
};
#endif // !_ITEM_H_
