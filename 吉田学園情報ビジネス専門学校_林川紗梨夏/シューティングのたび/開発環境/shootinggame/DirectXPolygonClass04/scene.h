//=============================================================================
//
// シーン処理 [scene.h]
// Author : 
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_COL_POLYGON 5
#define NUM_ROW_POLYGON 5
//#define MAX_POLYGON NUM_COL_POLYGON * NUM_ROW_POLYGON
#define MAX_POLYGON 20000
#define POLYGON_SIZE 100

#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define TEXTURE_WIDTH	620/ 6
#define TEXTURE_HEIGHT	520 /6
#define TURRET_WIDTH	130/ 6//砲塔サイズ
#define TURRET_HEIGHT	445 /6
#define FIELD_WIDTH 1150
#define FIELD_HEIGHT 1080

#define TEXTURE_PLAYER	 "data/TEXTURE/battleship001.png"
#define TEXTURE_TURRET	 "data/TEXTURE/turret001.png"
#define TEXTURE_UI001	 "data/TEXTURE/ui002.png"
#define TEXTURE_BULLET	 "data/TEXTURE/bullet.png"

#define PRIORITY_MAX 10

//#define MAX_PRIORITY 10 //優先順位最大
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//オブジェクトクラス
class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,
		OBJTYPE_ENEMY,
		OBJTYPE_BULLET_PLAYER,
		OBJTYPE_BULLET_ENEMY,
		OBJTYPE_ITEM,
		OBJTYPE_MAX
	}OBJTYPE;

	CScene(int nPriority = 3);//デフォルト引数３
	virtual ~CScene();
	virtual HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight) = 0;

	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; };
	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseAll(void);
	void SetObjType(OBJTYPE objType) { m_objType = objType; };
	OBJTYPE GetObjType(void) { return m_objType; };
	static CScene * GetScene(int nPriority, int nCnt) { return m_apScene[nPriority][nCnt]; };
	static int GetNumAll(void) { return m_nNumAll; };
	D3DXVECTOR3 GetPosition(void) { return m_pos; };
protected:
	void Release(void);
	//static CScene2D * Create(D3DXVECTOR3 pos);
	//void Release(void);
private:
	static int m_nNumAll;//シーンの総数
	D3DXVECTOR3 m_pos;
	static CScene * m_apScene[PRIORITY_MAX][MAX_POLYGON];//シーン
	int m_nPriority;
	OBJTYPE m_objType;
	int m_nID;//シーンのID
};
#endif // !_SCENE_H_
