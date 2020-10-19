//=============================================================================
//
// 爆発の処理 [explosion.h]
// Author : 
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
#define TEXTURE_EXPLOSION "data/TEXTURE/explosion001.png"
#define EXPLOSION_SIZE 200
//プレイヤークラス
class CExplosion : public CScene2D
{
public:
	typedef enum 
	{
		TYPE_NONE = 0,
		TYPE_NORMAL,
		TYPE_BOMB,
		TYPE_RESPAWN,
	}TYPE;
	CExplosion(int nPriority = 3);
	~CExplosion();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight,TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CExplosion * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, TYPE type);
	static HRESULT Load(void);
	static void Unload(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;   //テクスチャのポインタ
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //頂点座標
	float                   m_fSizeWidth;
	float                   m_fSizeHeight;
	int                     m_RandColorR;   //ランダムカラーR
	int                     m_RandColorG;   //ランダムカラーG
	int                     m_RandColorB;   //ランダムカラーB
	int                     m_nPatternAnim; //アニメーションパターン
	int                     m_nCountAnim; //アニメーションパターン
	TYPE                    m_Type; //爆発種類

	float                   m_fLength;		// 対角線の長さ
	float                   m_fAngle;		// 対角線の角度
	D3DXVECTOR3             m_Pos;// 位置
	D3DXVECTOR3             m_rot;// 向き
	float                   m_fScale;		// 拡大
	float                   m_fAddScale;	// 拡大加算値
};
#endif // !_EXPLOSION_H_
