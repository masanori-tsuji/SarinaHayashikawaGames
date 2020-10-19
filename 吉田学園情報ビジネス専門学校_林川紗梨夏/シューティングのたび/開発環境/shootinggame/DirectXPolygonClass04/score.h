//=============================================================================
//
// �X�R�A���� [score.h]
// Author : 
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_
#include "number.h"
#define MAX_DIGITS 10 //�ő包��

//2D�I�u�W�F�N�g�N���X
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
	CNumber                           *m_apNumber[MAX_DIGITS];//�i���o�[�i�ő包�����j
	int                                m_nScore; //�X�R�A�̐��l
};
#endif // !_SCORE_H_
