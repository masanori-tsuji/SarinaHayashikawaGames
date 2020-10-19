//=============================================================================
//
// スコア処理 [life.h]
// Author : 
//
//=============================================================================
#ifndef _BOMB_H_
#define _BOMB_H_
#include "star.h"
#define MAX_STAR 10 //最大桁数
#define NUM_BOMB 5 //初期ボム数

class CBomb : public CScene
{
public:

	CBomb(int nPriority = 8);
	~CBomb();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBomb * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void SetBomb(int nBomb);
	
	/*CScore * GetScore(void) { return this; };*/

private:
	CStar                           *m_apStar[MAX_STAR];//ナンバー（最大桁数分）
	D3DXVECTOR3                        m_Pos;
	int                                m_nBomb; //スコアの数値
	float                              m_fSizeWidth;
	float                              m_fSizeHeight;
};
#endif // !_SCORE_H_
