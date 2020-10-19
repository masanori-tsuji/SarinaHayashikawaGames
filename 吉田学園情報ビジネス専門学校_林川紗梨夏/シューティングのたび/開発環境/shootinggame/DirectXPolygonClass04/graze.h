//=============================================================================
//
// �X�R�A���� [score.h]
// Author : 
//
//=============================================================================
#ifndef _GRAZE_H_
#define _GRAZE_H_
#include "number.h"
#define MAX_DIGITS 10 //�ő包��

//2D�I�u�W�F�N�g�N���X
class CGraze : public CScene
{
public:

	CGraze(int nPriority = 8);
	~CGraze();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGraze * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void SetGraze(float nGraze);
	void AddGraze(float nValue);
	/*CScore * GetScore(void) { return this; };*/

private:
	CNumber                           *m_apNumber[MAX_DIGITS];//�i���o�[�i�ő包�����j
	float                                m_nGraze; //�X�R�A�̐��l
};
#endif // !_SCORE_H_
