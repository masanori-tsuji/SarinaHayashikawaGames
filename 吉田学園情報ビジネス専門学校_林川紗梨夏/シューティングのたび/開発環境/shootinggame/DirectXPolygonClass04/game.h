//=============================================================================
//
// ゲームシーン処理 [game.h]
// Author : 
//=============================================================================
//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CInput;
class CInputKeyboard;
class CInputMouse;
class CInputJoystick;
class CRenderer;
class CScene;
class CScene2D;
class CTitle;
class CTalk;
class CPlayer;
class CEnemy;
class CZako;
class CSound;
class CBg;
class CUi;
class CScore;
class CLife;
class CBomb;
class CMsgWindow;
class CGraze;
class CBoss;
class CBossgauge;


#define MAX_ENEMY 500

//マネージャークラス
class CGame : public CScene
{
public:
	CGame();
	~CGame();
	static CGame *Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	HRESULT Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CScore * GetScore(void);
	static CGraze * GetGraze(void);
	static CLife * GetLife(void);
	static CBomb * GetBomb(void);
	
	static CInputJoystick * GetInputJoystick(void);
	static CPlayer * GetPlayer(void) { return m_pPlayer; };
	static CBoss * GetBoss(void) { return m_pBoss; };
	static CMsgWindow * GetMsgUi01(void) { return m_pMsgUi01; };
	static CMsgWindow * GetMsgUi02(void) { return m_pMsgUi02; };
	CScene2D * GetScene(void) { return m_pScene; };
	static CSound * GetSound(void) { return m_pSound; };
	CGame * GetManager(void) { return this; };
	static void SetPlayerState(bool bPlayerUse) { m_bPlayerUse = bPlayerUse; };
	static bool GetIsStopUpdate(void) { return m_bIsStopUpdate; };
	static void SetIsStopUpdate(bool bIsStopUpdate) { m_bIsStopUpdate = bIsStopUpdate; };
	static bool GetIsStopUpdateContinue(void) { return m_bIsStopUpdateContinue; };
	static void SetIsStopUpdateContinue(bool bIsStopUpdateContinue) { m_bIsStopUpdateContinue = bIsStopUpdateContinue; };

	static bool m_bBossDeath;
private:
	static CRenderer * m_pRenderer;
	static CInputKeyboard * m_pInputKeyboard;
	static CInputMouse * m_pInputMouse;
	static CInputJoystick * m_pInputJoystick;
	static CSound * m_pSound;
	static CGraze * m_pGraze;
	static CScore * m_pScore;
	static CLife * m_pLife;
	static CBomb * m_pBomb;
	static CTitle * m_pTitle;
	static CGame * m_pGame;
	static CPlayer * m_pPlayer;
	static CBoss * m_pBoss;
	CEnemy * m_pEnemy[MAX_ENEMY];
	CZako * m_pZako[MAX_ENEMY];
	int         m_nEnemyIndex;
	D3DXVECTOR3 m_aEnemyLocation[4];
	CBg * m_pBg;
	CUi * m_pUi;
	static CMsgWindow * m_pMsgUi01;
	static CMsgWindow * m_pMsgUi02;
	int    m_nGameCount;
	int    m_nBGMCount;
	CScene2D * m_pScene;
	static bool m_bPlayerUse;
	static bool m_bIsStopUpdate;
	static bool m_bIsStopUpdateContinue;

};