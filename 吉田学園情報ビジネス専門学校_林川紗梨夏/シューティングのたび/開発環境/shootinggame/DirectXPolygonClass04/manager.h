//=============================================================================
//
// マネージャー処理 [manager.h]
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
class CGame;
class CScene2D;
class CTitle;
class CResult;
class CRanking;
class CSound;
class CText;
class CFade;
class CName;
class CPause;
class CContinue;
class CTrueend; 
class CBadend;
//マネージャークラス
class CManager
{
public:
	typedef enum
	{
		MODE_NONE = -1,
		MODE_TITLE,
		MODE_GAME,
		MODE_RANKING,
		MODE_RESULT,
		MODE_NAME,
		MODE_TRUEEND,
		MODE_BADEND,
		MODE_MAX
	}MODE;
	typedef struct
	{
		char aName[9];//名前
		int nScore;//スコア
		int nBomb;//ボム数
		int nLife;//残機
		int nGraze;//グレイズ
	}PlayerData;//プレイヤーデータ
	CManager();
	~CManager();
	HRESULT Init(HWND hWnd, bool bWindow, HINSTANCE hInstance);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CRenderer * GetRenderer(void);
	static CInputKeyboard * GetInputKeyboard(void);
	static CInput * GetInputMouse(void);
	static CInputJoystick * GetInputJoystick(void);
	CScene2D * GetScene(void) { return m_pScene; };
	static CSound * GetSound(void) { return m_pSound; };
	static CFade * GetFade(void) { return m_pFade; };
	static CText * GetText(void) { return m_pText; };
	static CPause * GetPause(void) { return m_pPause; };
	static CContinue * GetContinue(void) { return m_pContinue; };
	CManager * GetManager(void) { return this; };
	static void SetMode(MODE mode);
	static MODE GetMode(void);
	static void StopFade(void);
	static void StartFade(MODE mode, D3DXCOLOR col,float fadelate);
	static void SetScore(int nScore) { m_playerData.nScore = nScore; };
	static void SetBomb(int nBomb) { m_playerData.nBomb = nBomb; };
	static void SetLife(int nLife) { m_playerData.nLife = nLife; };
	static void SetGraze(int nGraze) { m_playerData.nGraze = nGraze; };
	static void SetPlayerName(int nCount, char cName);
	static PlayerData GetPlayerData(void) { return m_playerData; };
	static bool GetIsTalking(void) { return m_bIsTalking; };
	static void SetIsTalking(bool bIsTalking) { m_bIsTalking = bIsTalking; };
private:
	static bool m_bIsTalking;
	static CRenderer * m_pRenderer;
	static CInputKeyboard * m_pInputKeyboard;
	static CInputMouse * m_pInputMouse;
	static CInputJoystick * m_pInputJoystick;
	static CSound * m_pSound;
	static CTitle * m_pTitle;
	static CGame * m_pGame;
	static CRanking * m_pRanking;
	static CResult * m_pResult;
	static CTrueend * m_pTrueend;
	static CBadend * m_pBadend;
	static CName * m_pName;
	static CText * m_pText;
	static MODE m_mode;
	static CFade * m_pFade;
	static CPause * m_pPause;
	static CContinue * m_pContinue;
	static PlayerData m_playerData;//プレイヤーデータ
	static bool  m_bUseFade;

	CScene2D * m_pScene;
};