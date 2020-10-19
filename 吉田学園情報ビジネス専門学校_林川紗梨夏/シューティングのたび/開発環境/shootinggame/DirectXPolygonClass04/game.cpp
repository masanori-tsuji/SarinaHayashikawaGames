//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : �ѐ�ї���
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
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
// �ÓI�����o�ϐ�������
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
// �R���X�g���N�^
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
// �f�X�g���N�^
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
// ����������
//=============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{

	//�w�i�̃e�N�X�`�����[�h
	CBg::Load();
	//�v���C���[�̃e�N�X�`�����[�h
	CPlayer::Load();
	//�G�̃e�N�X�`�����[�h
	CEnemy::Load();
	CZako::Load();
	//�G�t�F�N�g�̃e�N�X�`�����[�h
	CEffect::Load();
	//�e�̃e�N�X�`�����[�h
	CBullet::Load();
	//�����̃e�N�X�`�����[�h
	CExplosion::Load();
	//�X�R�A�p���l�̃e�N�X�`�����[�h
	CNumber::Load();
	//�A�C�e���e�N�X�`�����[�h
	CStar::Load();
	//UI�e�N�X�`�����[�h
	CUi::Load();
	//UI�e�N�X�`�����[�h
	CBossgauge::Load();
	//UI�e�N�X�`�����[�h
	CPowergauge::Load();
	//�A�C�e���e�N�X�`�����[�h
	CItem::Load();
	//���b�Z�[�W�E�B���h�EUI���[�h
	CMsgWindow::Load();
	//\�{�X���[�h
	CBoss::Load();
	//\�{�X���[�h
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
	//�T�E���h

	CSound * pSound = CManager::GetSound();
	pSound->CSound::StopSound();
	pSound->CSound::PlaySound(CSound::SOUND_LABEL_BGM_DOCYU);
	m_pScore->SetScore(0);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGame::Uninit(void)
{
	//////�S�Ĕj������
	//CScene::ReleaseAll();
	//�w�i�̃e�N�X�`���A�����[�h
	CBg::Unload();
	//�v���C���[�̃e�N�X�`���A�����[�h
	CPlayer::Unload();
	//�G�̃e�N�X�`�����[�h
	CEnemy::Unload();
	CZako::Unload();
	//�G�t�F�N�g�̃e�N�X�`�����񃍁[�h
	CEffect::Unload();
	//�e�̃e�N�X�`���A�����[�h
	CBullet::Unload();
	//�����̃e�N�X�`���A�����[�h
	CExplosion::Unload();
	//�X�R�A�p���l�̃e�N�X�`���A�����[�h
	CNumber::Unload();
	//�A�C�e���e�N�X�`���A�����[�h
	CStar::Unload();
	//UI�e�N�X�`���A�����[�h
	CUi::Unload();
	//UI�e�N�X�`���A�����[�h
	CBossgauge::Unload();
	//UI�e�N�X�`���A�����[�h
	CPowergauge::Unload();
	//�A�C�e���e�N�X�`�����[�h
	CItem::Unload();
	//���b�Z�[�W�E�B���h�EUIan���[�h
	CMsgWindow::Unload();
	//�{�X�A�����[�h
	CBoss::Unload();
	//\�{�X���[�h
	CBattlebg::Unload();

	CManager::SetIsTalking(false);
	m_bBossDeath = true;
	Release();
	
}

//=============================================================================
// �X�V����
//=============================================================================
void CGame::Update(void)
{

	//�T�E���h
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
			//�L�[�{�[�h�̎擾
			CInputKeyboard * pInputKeyboard = CManager::GetInputKeyboard();
			//�W���C�p�b�h
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
				if (lpDIDevice != NULL && pInputJoystick->GetJoystickTrigger(7) || pInputKeyboard->GetKeyboardTrigger(DIK_P))//�|�[�Y
				{
					m_bIsStopUpdate = true;
				}
			}


#endif
			//==================================================================
			//����1
			//==================================================================

			if (m_nGameCount == 0)
			{
				pMsg01->SetMsgUi(1);
				pMsg02->SetMsgUi(0);
			

				wsprintf(str01, "�E�X�e�B�b�N�C�ӂ̕�����\n�V���b�g�A\nRB�{�^���Ń{�����g���܂�");
				wsprintf(str02, "��x�݂��������\nSTART�Ń|�[�Y�ł���");
				pText->SetTextUi(0, str01);
				pText->SetTextUi(1, str02);
			}
			if (m_nGameCount > 150 && m_nGameCount < 300)
			{
				if (m_nEnemyIndex < 20)//�P�O�C�X�|�[��
				{
					if (m_nGameCount % 15 == 0)
					{
						m_aEnemyLocation[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�J�n�n�_
						m_aEnemyLocation[1] = D3DXVECTOR3(300.0f, 600.0f, 0.0f);
						m_aEnemyLocation[2] = D3DXVECTOR3(FIELD_WIDTH - 300.0f, 600.0f, 0.0f);
						m_aEnemyLocation[3] = D3DXVECTOR3(FIELD_WIDTH, 0.0f, 0.0f);//�I���n�_
						m_pZako[m_nEnemyIndex] = CZako::Create(m_aEnemyLocation[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), ZAKO_SIZE_001_WIDTH, ZAKO_SIZE_001_HEIGHT, CZako::TYPE_002, m_aEnemyLocation);

						m_nEnemyIndex++;

						m_aEnemyLocation[0] = D3DXVECTOR3(FIELD_WIDTH, FIELD_HEIGHT / 2, 0.0f);//�J�n�n�_
						m_aEnemyLocation[1] = D3DXVECTOR3(FIELD_WIDTH - 300.0f, 300.0f, 0.0f);
						m_aEnemyLocation[2] = D3DXVECTOR3(300.0f, 300.0f, 0.0f);
						m_aEnemyLocation[3] = D3DXVECTOR3(0.0f, FIELD_HEIGHT / 2, 0.0f);//�I���n�_
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


					wsprintf(str01, "���₟�A�����^�]���������\n�l���ꂵ�[�ł��悧");
					wsprintf(str02, "�ŁA�ǂ��ɍs��������\n�����ĂȂ������񂾂�");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nGameCount > 800 && m_nGameCount < 1000)
				{
					pMsg01->SetMsgUi(0);
					pMsg02->SetMsgUi(0);


					wsprintf(str01, "���������\n�F�������ɗp������܂��Ă�");
					wsprintf(str02, "");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				
				if (m_nEnemyIndex < 20)//�P�O�C�X�|�[��
				{
					//if (m_nGameCount % 15 == 0)
					//{
					//	m_aEnemyLocation[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�J�n�n�_
					//	m_aEnemyLocation[1] = D3DXVECTOR3(600.0f, 600.0f, 0.0f);
					//	m_aEnemyLocation[2] = D3DXVECTOR3(600.0f, FIELD_HEIGHT - 600.0f, 0.0f);
					//	m_aEnemyLocation[3] = D3DXVECTOR3(0.0f, FIELD_HEIGHT, 0.0f);//�I���n�_
					//	m_pEnemy[m_nEnemyIndex] = CEnemy::Create(m_aEnemyLocation[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), TEXTURE_WIDTH, TEXTURE_HEIGHT, CEnemy::TYPE_003, m_aEnemyLocation,30);
					//	m_nEnemyIndex++;
					//}
					if (m_nGameCount % 15 == 0)
					{
						//�����_���ȐF�̐��l�擾
						float RandPosX = float(rand() % FIELD_WIDTH);

						m_aEnemyLocation[0] = D3DXVECTOR3(RandPosX, 0.0f, 0.0f);//�J�n�n�_
						m_aEnemyLocation[1] = D3DXVECTOR3(RandPosX + 300.0f, 300.0f, 0.0f);
						m_aEnemyLocation[2] = D3DXVECTOR3(RandPosX, FIELD_HEIGHT - 300.0f, 0.0f);
						m_aEnemyLocation[3] = D3DXVECTOR3(RandPosX + 300.0f, FIELD_HEIGHT, 0.0f);//�I���n�_
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


					wsprintf(str01, "������܂�\n��낵�����˂������܂���");
					wsprintf(str02, "");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nGameCount > 1300 && m_nGameCount < 1500)
				{
					pMsg01->SetMsgUi(0);
					pMsg02->SetMsgUi(1);


					wsprintf(str01, "\0");
					wsprintf(str02, "�Ƃ������A���肪\n�댯�����邼�I");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nGameCount > 1500 && m_nGameCount < 1800)
				{
					pMsg01->SetMsgUi(0);
					pMsg02->SetMsgUi(0);


					wsprintf(str01, "\0");
					wsprintf(str02, "�������Ɍ�����\n�e�����Ă��ĂȂ����H");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nGameCount > 1900 && m_nGameCount < 2300)
				{
					pMsg01->SetMsgUi(2);
					pMsg02->SetMsgUi(0);

					wsprintf(str01, "���[�ށA\n�Ȃ�Ό������Ƃ��܂łł��悧");
					wsprintf(str02, "\0");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nEnemyIndex < 20)//2�O�C�X�|�[��
				{
					if (m_nGameCount % 15 == 0)
					{
						//�����_���ȐF�̐��l�擾
						float RandPosX = float(rand() % FIELD_WIDTH);

						m_aEnemyLocation[0] = D3DXVECTOR3(RandPosX, 0.0f, 0.0f);//�J�n�n�_
						m_aEnemyLocation[1] = D3DXVECTOR3(RandPosX + 300.0f, 300.0f, 0.0f);
						m_aEnemyLocation[2] = D3DXVECTOR3(RandPosX, FIELD_HEIGHT - 300.0f, 0.0f);
						m_aEnemyLocation[3] = D3DXVECTOR3(RandPosX + 300.0f, FIELD_HEIGHT, 0.0f);//�I���n�_
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
				if (m_nEnemyIndex < 10)//�P�O�C�X�|�[��
				{
					if (m_nGameCount % 100 == 0)
					{
						//�����_�����l�擾
						float RandPosX = float(rand() % FIELD_WIDTH);

						m_aEnemyLocation[0] = D3DXVECTOR3(RandPosX, 0.0f, 0.0f);//�J�n�n�_
						m_aEnemyLocation[1] = D3DXVECTOR3(RandPosX, 500.0f, 0.0f);
						m_aEnemyLocation[2] = D3DXVECTOR3(RandPosX, 550.0f, 0.0f);
						m_aEnemyLocation[3] = D3DXVECTOR3(RandPosX, 600.0f, 0.0f);//�I���n�_

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

					wsprintf(str01, "������\n������Ƌ��߂ł����˂�");
					wsprintf(str02, "\0");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nGameCount > 3000 && m_nGameCount < 3200)
				{
					pMsg01->SetMsgUi(1);
					pMsg02->SetMsgUi(0);

					wsprintf(str01, "����͗��݂܂����悧");
					wsprintf(str02, "����Ȗ�������\n�^�]�������Ȃ���...");
					pText->SetTextUi(0, str01);
					pText->SetTextUi(1, str02);
				}
				if (m_nGameCount == 2750)
				{
					m_aEnemyLocation[0] = D3DXVECTOR3(FIELD_WIDTH / 2, 0.0f, 0.0f);//�J�n�n�_
					m_aEnemyLocation[1] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 4.0f, 0.0f);
					m_aEnemyLocation[2] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 3.5f, 0.0f);
					m_aEnemyLocation[3] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 3.0f, 0.0f);//�I���n�_
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
					m_aEnemyLocation[0] = D3DXVECTOR3(FIELD_WIDTH / 2, 0.0f, 0.0f);//�J�n�n�_
					m_aEnemyLocation[1] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 4.0f, 0.0f);
					m_aEnemyLocation[2] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 3.5f, 0.0f);
					m_aEnemyLocation[3] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 3.0f, 0.0f);//�I���n�_
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
					m_aEnemyLocation[0] = D3DXVECTOR3(FIELD_WIDTH / 2, 0.0f, 0.0f);//�J�n�n�_
					m_aEnemyLocation[1] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 4.0f, 0.0f);
					m_aEnemyLocation[2] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 3.5f, 0.0f);
					m_aEnemyLocation[3] = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 3.0f, 0.0f);//�I���n�_
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
						CManager::StartFade(CManager::MODE_TRUEEND,D3DCOLOR_RGBA(255,255,255,0),0.01f);//�N���A������
					}
					if (nContinue < PLAYER_CONTINUE)
					{
						CManager::SetIsTalking(false);
						CManager::StartFade(CManager::MODE_BADEND, D3DCOLOR_RGBA(255, 255, 255, 0),0.01f);//�N���A������
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
// �`�揈��
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// �X�R�A�擾
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
