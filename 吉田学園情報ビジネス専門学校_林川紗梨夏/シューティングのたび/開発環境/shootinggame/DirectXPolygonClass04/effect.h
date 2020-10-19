//=============================================================================
//
// エフェクトの処理 [effect.h]
// Author : 
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#define TEXTURE_EFFECT_001 "data/TEXTURE/slime001.png"
#define TEXTURE_EFFECT_002 "data/TEXTURE/ball_cat.png"

//プレイヤークラス
class CEffect : public CScene2D
{
public:
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
	}COLOR;//7色カラープリセット

	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_001,
		TYPE_002,
		TYPE_003,
		TYPE_004,
		TYPE_MAX
	}TYPE;
	CEffect(int nPriority = 3);
	~CEffect();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, D3DXVECTOR3 move, TYPE Type,int col, int nLife);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEffect * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, D3DXVECTOR3 move, TYPE Type, int col,int nLife);
	static HRESULT Load(void);
	static void Unload(void);

private:
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //頂点座標
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];   //テクスチャのポインタ
	float					m_fLength;		// 対角線の長さ
	float					m_fAngle;		// 対角線の角度
	D3DXVECTOR3				m_rot;			// 向き
	float					m_fScale;		// 拡大
	float					m_fAddScale;	// 拡大加算値
	int                     m_RandColorR;   //ランダムカラーR
	int                     m_RandColorG;   //ランダムカラーG
	int                     m_RandColorB;   //ランダムカラーB
	float                   m_nAlpha;       //透明度
	int                     m_nPatternAnim; //アニメーションパターン
	int                     m_nCountAnim; //アニメーションパターン
	int                     m_nType;        //敵タイプ
	int                     m_nCountBullet;
	D3DXVECTOR3             m_TargetDistance;//目標までの距離
	D3DXVECTOR3             m_TargetPos;//目標地点
	D3DXVECTOR3             m_Move;//移動量
	static D3DCOLOR         m_aColor[COLOR_MAX];//7色プリセット
	D3DXCOLOR                m_Color;//色
	int                     m_ColorNum; //カラー番号
	int                     m_nCountDraw;
	int                     m_nLife;
};
#endif // !_EFFECT_H_
