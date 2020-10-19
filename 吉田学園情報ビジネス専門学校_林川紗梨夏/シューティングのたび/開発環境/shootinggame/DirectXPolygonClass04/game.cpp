//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "joystick.h"
#include "scene.h"
#include "game.h"
#include "scene2D.h"
#include "title.h"
#include "bg.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "zako.h"
#include "score.h"
#include "life.h"
#include "number.h"
#include "effect.h"
#include "ui.h"
#include "item.h"
#include "star.h"
#include "bomb.h"
#include "msgwindow.h"
#include "graze.h"
#include "pause.h"
#include "boss.h"
#include "bossgauge.h"
#include "talk.h"
#include "text.h"
#include "powergauge.h"
#include "battlebg.h"
#include "inputkeyboard.h"
//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
CScore * CGame::m_pScore = NULL;
CGraze * CGame::m_pGraze = NULL;
CLife * CGame::m_pLife = NULL;
CBomb * CGame::m_pBomb = NULL;
CMsgWindow * CGame::m_pMsgUi01 = NULL;
CMsgWindow * CGame::m_pMsgUi02 = NULL;
CPlayer * CGame::m_pPlayer = NULL;
CBoss * CGame::m_pBoss = NULL;
bool CGame::m_bPlayerUse = true;
bool CGame::m_bIsStopUpdate = false;

bool CGame::m_bBossDeath = true;
bool CGame::m_bIsStopUpdateContinue = false;


//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame()
{
	memset(m_pZako, 0, sizeof(m_pZako));
	memset(m_pEnemy, 0, sizeof(m_pEnemy));
	memset(m_aEnemyLocation, 0, sizeof(m_aEnemyLocation));
	m_nEnemyIndex = 0;

	m_pBg = NULL;
	m_pUi = NULL;
	m_nGameCount = 0;
	m_nBGMCount = 0;
}
//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{

}

CGame * CGame::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CGame *pGame;
	pGame = new CGame;
	pGame->Init(pos, fSizeWidth, fSizeHeight);
	return pGame;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{

	//背景のテクスチャロード
	CBg::Load();
	//プレイヤーのテクスチャロード
	CPlayer::Load();
	//敵のテクスチャロード
	CEnemy::Load();
	CZako::Load();
	//エフェクトのテクスチャロード
	CEffect::Load();
	//弾のテクスチャロード
	CBullet::Load();
	//爆発のテクスチャロード
	CExplosion::Load();
	//スコア用数値のテクスチャロード
	CNumber::Load();
	//アイテムテクスチャロード
	CStar::Load();
	//UIテクスチャロード
	CUi::Load();
	//UIテクスチャロード
	CBossgauge::Load();
	//UIテクスチャロード
	CPowergauge::Load();
	//アイテムテクスチャロード
	CItem::Load();
	//メッセージウィンドウUIロード
	CMsgWindow::Load();
	//\ボスロード
	CBoss::Load();
	//\ボスロード
	CBattlebg::Load();

	m_pBg = CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
	m_pUi = CUi::Create(D3DXVECTOR3(1545.0f, SCREEN_CENTER_Y, 0.0f), 750, 1080);
	m_pMsgUi01 = CMsgWindow::Create(D3DXVECTOR3(SCREEN_WIDTH -380.0f, 600.0f, 0.0f), 1, SCREEN_HEIGHT);
	m_pMsgUi02 = CMsgWindow::Create(D3DXVECTOR3(SCREEN_WIDTH - 380.0f, 770.0f, 0.0f), 0, SCREEN_HEIGHT);


	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 85.0f, 0.0f), 40, 70);
	m_pGraze = CGraze::Create(D3DXVECTOR3(SCREEN_WIDTH - 170.0f, 155.0f, 0.0f), 40 * 0.8f, 70 * 0.8f);
	m_pLife = CLife::Create(D3DXVECTOR3(SCREEN_WIDTH - 470.0f, 220.0f, 0.0f), 50, 50);
	m_pBomb = CBomb::Create(D3DXVECTOR3(SCREEN_WIDTH - 470.0f, 300.0f, 0.0f), 50, 50);

	m_pPlayer = CPlayer::Create(D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 2 + 200.0f, 0.0f), TEXTURE_WIDTH, TEXTURE_HEIGHT);

	CPowergauge::Create(D3DXVECTOR3(FIELD_WIDTH + 320.0f, 380.0f, 0.0f), 400.0f, 40.0f, 0);
	//enemy_location[]
	/*m_pEnemy[0] = CEnemy::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), TEXTURE_WIDTH, TEXTURE_HEIGHT, CEnemy::TYPE_002);*/
	//CEnemy::Create(D3DXVECTOR3(300.0f * 3, 100.0f, 0.0f), TEXTURE_WIDTH, TEXTURE_HEIGHT, CEnemy::TYPE_002);
	//CEnemy::Create(D3DXVECTOR3(50.0f * 3, 100.0f, 0.0f), TEXTURE_WIDTH, TEXTURE_HEIGHT, CEnemy::TYPE_003);
	//サウンド

	CSound * pSound = CManager::GetSound();
	pSound->CSound::StopSound();
	pSound->CSound::PlaySound(CSound::SOUND_LABEL_BGM_DOCYU);
	m_pScore->SetScore(0);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	//////全て破棄する
	//CScene::ReleaseAll();
	//背景のテクスチャアンロード
	CBg::Unload();
	//プレイヤーのテクスチャアンロード
	CPlayer::Unload();
	//敵のテクスチャロード
	CEnemy::Unload();
	CZako::Unload();
	//エフェクトのテクスチャあんロード
	CEffect::Unload();
	//弾のテクスチャアンロード
	CBullet::Unload();
	//爆発のテクスチャアンロード
	CExplosion::Unload();
	//スコア用数値のテクスチャアンロード
	CNumber::Unload();
	//アイテムテクスチャアンロード
	CStar::Unload();
	//UIテクスチャアンロード
	CUi::Unload();
	//UIテクスチャアンロード
	CBossgauge::Unload();
	//UIテクスチャアンロード
	CPowergauge::Unload();
	//アイテムテクスチャロード
	CItem::Unload();
	//メッセージウィンドウUIanロード
	CMsgWindow::Unload();
	//ボスアンロード
	CBoss::Unload();
	//\ボスロード
	CBattlebg::Unload();

	CManager::SetIsTalking(false);
	m_bBossDeath = true;
	Release();
	
}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{

	//サウンド
	CSound * pSound = CManager::GetSound();
	CMsgWindow  * pMsg01 = CGame::GetMsgUi01();
	CMsgWindow  * pMsg02 = CGame::GetMsgUi02();

	char str01[512];
	char str02[512];

	CText * pText = CManager::GetText();
	if (m_bIsStopUpdate == false)
	{
		if (m_bIsStopUpdateContinue != true)
		{

#define JS
#ifdef JS
			//キーボードの取得
			CInputKeyboard * pInputKeyboard = CManager::GetInputKeyboard();
			//ジョイパッド
			CInputJoystick * pInputJoystick = CManager::GetInputJoystick();
			LPDIRECTINPUTDEVICE8 lpDIDevice = CInputJoystick::GetDevice();
			DIJOYSTATE js;
			if (lpDIDevice != NULL)
			{
				lpDIDevice->Poll();
				lpDIDevice->GetDeviceState(sizeof(DIJOYSTATE), &js);
			}

			if (m_bIsStopUpdate == false)
			{
				if (lpDIDevice != NULL && pInputJoystick->GetJoystickTrigger(7) || pInputKeyboard->GetKeyboardTrigger(DIK_P))//ポーズ
				{
					m_bIsStopUpdate = true;
				}
			}


#endif
			//==================================================================
			//道中1
			//==================================================================

			if (m_nGameCount == 0)
			{
				pMsg01->SetMsgUi(1);
				pMsg02->SetMsgUi(0);
			

				wsprintf(str01, "右スティック任意の方向で\nショット、\nRBボタンでボムが使えます");
				wsprintf(str02, "一休みしたければ\nSTARTでポーズできる");
				pText->SetTextUi(0, str01);
				pText->SetTextUi(1, str02);
			}
			if (m_nGameCount > 150 && m_nGameCount < 300)
			{
				if (m_nEnemyIndex < 20)//１０匹スポーン
				{
					if (m_nGameCount % 15 == 0)
					{
						m_aEnemyLocation[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//開始地点
						m_aEnemyLocation[1] = D3DXVECTOR3(300.0f, 600.0f, 0.0f);
						m_aEnemyLocation[2] = D3DXVECTOR3(FIELD_WIDTH - 300.0f, 600.0f, 0.0f);
						m_aEnemyLocation[3] = D3DXVECTOR3(FIELD_WIDTH, 0.0f, 0.0f);//終了地点
						m_pZako[m_nEnemyIndex] = CZako::Create(m_aEnemyLocation[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), ZAKO_SIZE_001_WIDTH, ZAKO_SIZE_001_HEIGHT, CZako::TYPE_002, m_aEnemyLocation);

						m_nEnemyIndex++;

						m_aEnemyLocation[0] = D3DXVECTOR3(FIELD_WIDTH, FIELD_HEIGHT / 2, 0.0f);//開始地点
						m_aEnemyLocation[1] = D3DXVECTOR3(FIELD_WIDTH - 300.0f, 300.0f, 0.0f);
						m_aEnemyLocation[2] = D3DXVECTOR3(300.0f, 300.0f, 0.0f);
						m_aEnemyLocation[3] = D3DXVECTOR3(0.0f, FIELD_HEIGHT / 2, 0.0f);//終了地点
						m_pZako[m_nEnemyIndex] = CZako::Create(m_aEnemyLocation[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), ZAKO_SIZE_001_WIDTH, ZAKO_SIZE_001_HEIGHT, CZako::TYPE_002, m_aEnemyLocation);

						m_nEnemyIndex++;
					}
				}
				else
				{
					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						m_pZako[m_nEnemyIndex] = NULL;
					}
					m_nEnemyIndex = 0;
				}
			}
			if (m_nGameCount > 600 && m_nGameCount < 1000)
			{
			
				if (m_nGameCount > 600 && m_nGameCount < 700)
				{
					pMsg01->SetMsgUi(1);
					pMsg02->SetMsgUi(0);


					wsprintf(str01, "いやぁ、いい運転手を見つけて\n僕うれしーですよぉ");
					wsprintf(str02, "で、どこに行きたいか\n聞いてなかったんだが");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nGameCount > 800 && m_nGameCount < 1000)
				{
					pMsg01->SetMsgUi(0);
					pMsg02->SetMsgUi(0);


					wsprintf(str01, "ちょっくら\n宇宙役所に用がありましてね");
					wsprintf(str02, "");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				
				if (m_nEnemyIndex < 20)//１０匹スポーン
				{
					//if (m_nGameCount % 15 == 0)
					//{
					//	m_aEnemyLocation[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//開始地点
					//	m_aEnemyLocation[1] = D3DXVECTOR3(600.0f, 600.0f, 0.0f);
					//	m_aEnemyLocation[2] = D3DXVECTOR3(600.0f, FIELD_HEIGHT - 600.0f, 0.0f);
					//	m_aEnemyLocation[3] = D3DXVECTOR3(0.0f, FIELD_HEIGHT, 0.0f);//終了地点
					//	m_pEnemy[m_nEnemyIndex] = CEnemy::Create(m_aEnemyLocation[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), TEXTURE_WIDTH, TEXTURE_HEIGHT, CEnemy::TYPE_003, m_aEnemyLocation,30);
					//	m_nEnemyIndex++;
					//}
					if (m_nGameCount % 15 == 0)
					{
						//ランダムな色の数値取得
						float RandPosX = float(rand() % FIELD_WIDTH);

						m_aEnemyLocation[0] = D3DXVECTOR3(RandPosX, 0.0f, 0.0f);//開始地点
						m_aEnemyLocation[1] = D3DXVECTOR3(RandPosX + 300.0f, 300.0f, 0.0f);
						m_aEnemyLocation[2] = D3DXVECTOR3(RandPosX, FIELD_HEIGHT - 300.0f, 0.0f);
						m_aEnemyLocation[3] = D3DXVECTOR3(RandPosX + 300.0f, FIELD_HEIGHT, 0.0f);//終了地点
						m_pZako[m_nEnemyIndex] = CZako::Create(m_aEnemyLocation[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), ZAKO_SIZE_001_WIDTH, ZAKO_SIZE_001_HEIGHT, CZako::TYPE_003, m_aEnemyLocation);
						m_nEnemyIndex++;
					}

				}
				else
				{
					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						m_pZako[m_nEnemyIndex] = NULL;
					}
					m_nEnemyIndex = 0;
				}

			}

			if (m_nGameCount > 1100 && m_nGameCount < 2000)
			{
				if (m_nGameCount > 1100 && m_nGameCount < 1300)
				{
					pMsg01->SetMsgUi(1);
					pMsg02->SetMsgUi(0);


					wsprintf(str01, "そちらまで\nよろしくおねがいしますぅ");
					wsprintf(str02, "");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nGameCount > 1300 && m_nGameCount < 1500)
				{
					pMsg01->SetMsgUi(0);
					pMsg02->SetMsgUi(1);


					wsprintf(str01, "\0");
					wsprintf(str02, "というか、周りが\n危険すぎるぞ！");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nGameCount > 1500 && m_nGameCount < 1800)
				{
					pMsg01->SetMsgUi(0);
					pMsg02->SetMsgUi(0);


					wsprintf(str01, "\0");
					wsprintf(str02, "俺たちに向けて\n弾撃ってきてないか？");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nGameCount > 1900 && m_nGameCount < 2300)
				{
					pMsg01->SetMsgUi(2);
					pMsg02->SetMsgUi(0);

					wsprintf(str01, "うーむ、\nならば撃ち落とすまでですよぉ");
					wsprintf(str02, "\0");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nEnemyIndex < 20)//2０匹スポーン
				{
					if (m_nGameCount % 15 == 0)
					{
						//ランダムな色の数値取得
						float RandPosX = float(rand() % FIELD_WIDTH);

						m_aEnemyLocation[0] = D3DXVECTOR3(RandPosX, 0.0f, 0.0f);//開始地点
						m_aEnemyLocation[1] = D3DXVECTOR3(RandPosX + 300.0f, 300.0f, 0.0f);
						m_aEnemyLocation[2] = D3DXVECTOR3(RandPosX, FIELD_HEIGHT - 300.0f, 0.0f);
						m_aEnemyLocation[3] = D3DXVECTOR3(RandPosX + 300.0f, FIELD_HEIGHT, 0.0f);//終了地点
						m_pZako[m_nEnemyIndex] = CZako::Create(m_aEnemyLocation[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), ZAKO_SIZE_001_WIDTH, ZAKO_SIZE_001_HEIGHT, CZako::TYPE_004, m_aEnemyLocation);
						m_nEnemyIndex++;
					}
				}
				else
				{
					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						m_pZako[m_nEnemyIndex] = NULL;
					}
					m_nEnemyIndex = 0;
				}
			}
			if (m_nGameCount > 2000 && m_nGameCount < 2500)
			{
				if (m_nGameCount > 2000 && m_nGameCount < 2500)
				{
					pMsg01->SetMsgUi(0);
					pMsg02->SetMsgUi(0);

					wsprintf(str01, "\0");
					wsprintf(str02, "\0");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nEnemyIndex < 10)//１０匹スポーン
				{
					if (m_nGameCount % 100 == 0)
					{
						//ランダム数値取得
						float RandPosX = float(rand() % FIELD_WIDTH);

						m_aEnemyLocation[0] = D3DXVECTOR3(RandPosX, 0.0f, 0.0f);//開始地点
						m_aEnemyLocation[1] = D3DXVECTOR3(RandPosX, 500.0f, 0.0f);
						m_aEnemyLocation[2] = D3DXVECTOR3(RandPosX, 550.0f, 0.0f);
						m_aEnemyLocation[3] = D3DXVECTOR3(RandPosX, 600.0f, 0.0f);//終了地点

						m_pEnemy[m_nEnemyIndex] = CEnemy::Create(D3DXVECTOR3(RandPosX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), TEXTURE_WIDTH, TEXTURE_HEIGHT, CEnemy::TYPE_001, m_aEnemyLocation, 100);

						m_nEnemyIndex++;
					}
				}
				else
				{
					for (int nCnt = 0; nCnt < 10; nCnt++)
					{
						m_pEnemy[m_nEnemyIndex] = NULL;
					}
					m_nEnemyIndex = 0;
				}
			}
			if (m_nGameCount > 2700 && m_nGameCount < 3200)
			{
				if (m_nGameCount > 2750 && m_nGameCount < 3000)
				{
					pMsg01->SetMsgUi(1);
					pMsg02->SetMsgUi(1);

					wsprintf(str01, "こいつは\nちょっと強めですかねぇ");
					wsprintf(str02, "\0");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nGameCount > 3000 && m_nGameCount < 3200)
				{
					pMsg01->SetMsgUi(1);
					pMsg02->SetMsgUi(0);

					wsprintf(str01, "回避は頼みましたよぉ");
					wsprintf(str02, "こんな命懸けな\n運転したくないぞ...");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nGameCount == 2750)
				{
					m_aEnemyLocation[0] = D3DXVECTOR3(FIELD_WIDTH / 2, 0.0f, 0.0f);//開始地点
					m_aEnemyLocation[1] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 4.0f, 0.0f);
					m_aEnemyLocation[2] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 3.5f, 0.0f);
					m_aEnemyLocation[3] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 3.0f, 0.0f);//終了地点
					m_pEnemy[m_nEnemyIndex] = CEnemy::Create(m_aEnemyLocation[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), TEXTURE_WIDTH, TEXTURE_HEIGHT, CEnemy::TYPE_MIDDLE_BOSS, m_aEnemyLocation, 3000);
				}
			}

			if (m_nGameCount > 4500 && m_nGameCount < 4700)
			{
				if (m_nGameCount > 4500 && m_nGameCount < 4700)
				{
					pMsg01->SetMsgUi(0);
					pMsg02->SetMsgUi(0);

					wsprintf(str01, "\0");
					wsprintf(str02, "\0");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nGameCount == 4501)
				{
	
					CTalk::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 85.0f, 0.0f), 40, 70, 0, 11);
					//CTalk::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 85.0f, 0.0f), 40, 70, 12, 40);
					//CTalk::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 85.0f, 0.0f), 40, 70, 41, 49);
				}
				if (m_nGameCount == 4502)
				{
					CManager::SetIsTalking(true);
					m_nGameCount++;
				}
				if (m_nGameCount == 4550)
				{
					m_bBossDeath = false;
					m_nGameCount++;
					m_aEnemyLocation[0] = D3DXVECTOR3(FIELD_WIDTH / 2, 0.0f, 0.0f);//開始地点
					m_aEnemyLocation[1] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 4.0f, 0.0f);
					m_aEnemyLocation[2] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 3.5f, 0.0f);
					m_aEnemyLocation[3] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 3.0f, 0.0f);//終了地点
					m_pBoss = CBoss::Create(m_aEnemyLocation[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), BOSS_SIZE_001_WIDTH, BOSS_SIZE_001_HEIGHT, CBoss::TYPE_MIDDLE_BOSS, m_aEnemyLocation, MIDDLEBOSS_LIFE);
					CBossgauge::Create(D3DXVECTOR3(100.0f, 100.0f, 0.0f), 800.0f, 20.0f, 0);
				}
				if (m_nGameCount > 4560)
				{
					/*m_pBoss = NULL;*/
				}
			}
			if (m_nGameCount > 4700 && m_nGameCount < 5000)
			{
				if (m_nGameCount == 4701)
				{
					//CTalk::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 85.0f, 0.0f), 40, 70, 0, 11);
					CTalk::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 85.0f, 0.0f), 40, 70, 12, 40);
					/*CTalk::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 85.0f, 0.0f), 40, 70, 41, 49);*/
				}
				if (m_nGameCount == 4702)
				{
					CManager::SetIsTalking(true);
					m_nGameCount++;
					
				}
				if (m_nGameCount == 4703)
				{
					m_nBGMCount++;
				}
				if (m_nBGMCount == 1000 || (m_nBGMCount < 1000 && CManager::GetIsTalking() == false && m_nGameCount == 4703))
				{
					pSound->CSound::StopSound();
					pSound->CSound::PlaySound(CSound::SOUND_LABEL_BGM_BOSS2);
				}
				if (m_nGameCount == 4750)
				{
			
					m_bBossDeath = false;
					m_nGameCount++;
					m_aEnemyLocation[0] = D3DXVECTOR3(FIELD_WIDTH / 2, 0.0f, 0.0f);//開始地点
					m_aEnemyLocation[1] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 4.0f, 0.0f);
					m_aEnemyLocation[2] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 3.5f, 0.0f);
					m_aEnemyLocation[3] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 3.0f, 0.0f);//終了地点
					m_pBoss = CBoss::Create(m_aEnemyLocation[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), BOSS_SIZE_001_WIDTH, BOSS_SIZE_001_HEIGHT, CBoss::TYPE_MAIN_BOSS, m_aEnemyLocation, MAINBOSS_LIFE);
					CBossgauge::Create(D3DXVECTOR3(100.0f, 100.0f, 0.0f), 800.0f, 20.0f, 0);
				}
				if (m_nGameCount == 4800 && m_bBossDeath == true)
				{
					CTalk::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 85.0f, 0.0f), 40, 70, 50, 55);
					CManager::SetIsTalking(true);
					m_nGameCount++;
				}
				if (m_nGameCount == 4820 && CManager::GetIsTalking() == false)
				{
					pMsg01->SetMsgUi(1);
					pMsg02->SetMsgUi(0);


					wsprintf(str01, "\0");
					wsprintf(str02, "\0");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
					pSound->CSound::StopSound();
					int nContinue = m_pPlayer->GetContinue();
					if (nContinue >= PLAYER_CONTINUE)
					{
						CManager::SetIsTalking(false);
						CManager::StartFade(CManager::MODE_TRUEEND,D3DCOLOR_RGBA(255,255,255,0),0.01f);//クリアさせる
					}
					if (nContinue < PLAYER_CONTINUE)
					{
						CManager::SetIsTalking(false);
						CManager::StartFade(CManager::MODE_BADEND, D3DCOLOR_RGBA(255, 255, 255, 0),0.01f);//クリアさせる
					}
					
				}
			}
			if (CManager::GetIsTalking() == false)
			{
				if (m_bBossDeath == true)
				{
					m_nGameCount++;
				}

			}
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// スコア取得
//=============================================================================
CScore * CGame::GetScore(void)
{
	return m_pScore;
}
CGraze * CGame::GetGraze(void)
{
	return m_pGraze;
}
CLife * CGame::GetLife(void)
{
	return m_pLife;
}
CBomb * CGame::GetBomb(void)
{
	return m_pBomb;
}
