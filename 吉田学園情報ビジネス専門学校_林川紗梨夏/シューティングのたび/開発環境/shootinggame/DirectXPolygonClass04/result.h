//=============================================================================
//
// 結果 [result.h]
// Author : 林川紗梨夏
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#define BOMB_BONUS 1000 //残りボム数ボーナス
#define LIFE_BONUS 1000 //残りライフ数ボーナス
#define GRAZE_BONUS 100 //グレイズボーナス

#define MAX_BG 2
#define BG_UV_MOVE 0.001f
#define BG_UV_MOVE_01 0.0015f
#define BG_UV_MOVE_02 0.0017f
#define BG_UV_MOVE_2 1.0f

#define RESULT02_SIZE_HEIGHT 200.0f

#define TEXTURE_RESULT_2	     "data/TEXTURE/result_001.png"
//#define TEXTURE_     "data/TEXTURE/result001.png"
#define TEXTURE_RESULT	     "data/TEXTURE/result.png"


class CResult : public CScene
{
public:
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_001,
		TYPE_002,
		TYPE_MAX
	}TYPE;
	CResult(int nPriority = 1);
	~CResult();
	static HRESULT Load(void);
	static void Unload(void);
	static CResult *Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPosition(D3DXVECTOR3 pos);
private:
	static LPDIRECT3DTEXTURE9   m_apTexture[TYPE_MAX];
	CScene2D                  * m_apScene2D[TYPE_MAX];
	D3DXVECTOR3               m_vPos[NUM_VERTEX];       //頂点座標
	D3DXVECTOR3                 m_pos;
	float                       m_fSizeWidth;
	float                       m_fSizeHeight;
	CManager::PlayerData    m_pPlayerData;
	int                     m_nPatternAnim; //アニメーションパターン
	int                     m_nCountAnim; //アニメーションパターン
};
#endif // !_BG_H_


