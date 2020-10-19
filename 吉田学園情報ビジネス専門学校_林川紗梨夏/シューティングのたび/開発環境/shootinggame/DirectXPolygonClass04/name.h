//=============================================================================
//
// 名前 [name.h]
// Author : 林川紗梨夏
//
//=============================================================================
#ifndef _NAME_H_
#define _NAME_H_
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"
#include "scene2D.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME	"data/TEXTURE/alphabet001.png"
#define TEXTURE_NAME_UI	"data/TEXTURE/nameenter.png"
#define TEXTURE_BG_001	"data/TEXTURE/result.png"
#define TEXTURE_BG_002	"data/TEXTURE/alphabet001.png"
#define MAX_COLUMN 3
#define COLUMN_MAXNUMBER 10
#define MAX_NAME 8

//#define TEXTURE_RESULT2	"data/TEXTURE/bg101.png"
//#define TEXTURE_RESULT3	"data/TEXTURE/bg102.png"

#define BG_UV_MOVE_01_NAME 0.0015f
#define BG_UV_MOVE_02_NAME 1.0f

#define NAME_UI_HEIGHT 200
#define MAX_NAME_TEXTURE 3
#define TEXT_SCALE 2//テキストサイズのスケール
#define TEXT_SIZE 50.0f * TEXT_SCALE
//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CName :public CScene
{
public:
	CName(int nPriority = 0);
	~CName();
	static HRESULT Load(void);
	static void Unload(void);
	static CName * Create(D3DXVECTOR3 pos, float SizeHeight, float SizeWidth);
	HRESULT Init(D3DXVECTOR3 pos, float SizeHeight, float SizeWidth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
	typedef struct
	{
		char aLetter;
		int  LetterIndex;
	}Letter;
	typedef struct
	{
		int nCount;
		bool bTrigger;
	}IsCount;//推されてるか？と押し中カウント
	typedef struct
	{
		IsCount Up;
		IsCount Down;
		IsCount Right;
		IsCount Left;
	}IsPress;//推されてるか？
	
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_NAME_TEXTURE];	//テクスチャ
	//static char m_aLetter[MAX_COLUMN][COLUMN_MAXNUMBER];		//文字格納用
	static Letter m_aLetter[MAX_COLUMN][COLUMN_MAXNUMBER];
	D3DXVECTOR3 m_pos;											//位置
	CScene2D * m_apScene[MAX_POLYGON];							//シーン
	CScene2D * m_apSceneText[MAX_NAME];							//シーン（上のテキスト表示用）
	CScene2D * m_apSceneBg[2];							//シーン背景
	D3DXVECTOR3 m_vPos[NUM_VERTEX];								//頂点座標
	float m_fTextureX;											//テクスチャの幅
	float m_fTextureY;											//テクスチャの高さ
	float m_fMove;
	float m_fWidth;												//幅
	float m_fHeight;											//高さ
	bool m_bName;												//名前の記録状態
	bool m_bCanselName;											//名前の取り消し状態
	char m_aName[MAX_NAME];										//名前格納用
	bool m_bColumnDown;											//下矢印判定
	bool m_bColumnUp;											//上矢印判定
	int m_nColumnCount;											//行数カウンタ
	bool m_bLetterRight;										//右矢印判定
	bool m_bLetterLeft;											//左矢印判定
	bool m_bOK;													//決定判定
	int m_nLetterCount;											//文字数カウンタ
	int m_nNameCount;											//名前の文字数カウンタ
	int m_nPatternAnim;
	int m_nCountAnim;
	IsPress m_IsTrigger;                                         //推されてるかどうか
	int     m_TriggerCount;                                     //押し中のカウンタ
};
#endif