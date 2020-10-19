//=============================================================================
//
// �X�R�A���� [life.h]
// Author : 
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_
#include "star.h"
#define MAX_STAR 10 //�ő包��
#define PLAYER_LIFE 1 //�v���C���[�̃��C�t
//2D�I�u�W�F�N�g�N���X
class CLife : public CScene
{
public:

	CLife(int nPriority = 8);
	~CLife();
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CLife * Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void SetLife(int nLife);
	/*CScore * GetScore(void) { return this; };*/

private:
	CStar                           *m_apStar[MAX_STAR];//�i���o�[�i�ő包�����j
	D3DXVECTOR3                        m_Pos;
	int                                m_nLife; //�X�R�A�̐��l
	float                              m_fSizeWidth;
	float                              m_fSizeHeight;
};
#endif // !_SCORE_H_
