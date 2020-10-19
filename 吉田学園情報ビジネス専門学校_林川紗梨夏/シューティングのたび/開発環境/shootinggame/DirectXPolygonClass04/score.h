//=============================================================================
//
// スコア処理 [score.h]
// Author : 
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_
#include "number.h"
#define MAX_DIGITS 10 //最大桁数

//2Dオブジェクトクラス
class CScore : public CScene
{
public:

	CScore(int nPriority = 8);
	~CScore();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CScore * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void SetScore(int nScore);
	void AddScore(int nValue);
	/*CScore * GetScore(void) { return this; };*/

private:
	CNumber                           *m_apNumber[MAX_DIGITS];//ナンバー（最大桁数分）
	int                                m_nScore; //スコアの数値
};
#endif // !_SCORE_H_
