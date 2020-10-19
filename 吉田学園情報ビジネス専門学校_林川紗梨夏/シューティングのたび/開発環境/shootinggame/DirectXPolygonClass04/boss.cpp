//=============================================================================
//
// 敵の処理 [enemy.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "game.h"
#include "bossgauge.h"
#include "boss.h"
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "explosion.h"
#include <time.h>
#include "sound.h"
#include "score.h"
#include "item.h"
#include "effect.h"
#include "player.h"
#include "talk.h"
#include "msgwindow.h"
#include "text.h"
#include "battlebg.h"


#define CLAW_SHOT_SPEED 3.0f

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBoss::m_pTexture[TYPE_MAX] = {};   //テクスチャのポインタ
//=============================================================================
// コンストラクタ
//=============================================================================
CBoss::CBoss(int nPriority) : CScene2D(nPriority)
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_fLength = 0.0f;		// 対角線の長さ
	m_fAngle = 0.0f;		// 対角線の角度
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 位置
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 向き
	m_fScale = 0.0f;		// 拡大
	m_fAddScale = 0.0f;	// 拡大加算値

	m_TargetDistance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//目標までの距離
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//目標地点
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量

	m_nPatternAnim = 1;
	m_nCountAnim = 0;
	m_nCountBullet = 0;
	m_nCountTime = 0;
	m_nType = TYPE_MIDDLE_BOSS;
	m_bossmode = MODE_NORMAL;
	m_nLife = 20;
	m_nLifeCount = 0;
	m_nColor = 0;
	m_nConutTime = 0;
	m_nMoveIndex = 10;
	m_fSpeed = 0.0f;
	m_fMovetime = 0.0f;
	m_nBulletShotinterval = 100;
	m_bHit = false;
	m_nHitCount = 0;

}

//=============================================================================
// デストラクタ
//=============================================================================
CBoss::~CBoss()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CBoss * CBoss::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeWidth, float fSizeHeight, TYPE Type, D3DXVECTOR3 location[MAX_LOCATION], int nLife)
{
	CBoss *pEnemy;
	pEnemy = new CBoss;
	pEnemy->Init(pos, move, fSizeWidth, fSizeHeight, Type, location, nLife);
	return pEnemy;
}
//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CBoss::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BOSS_001, &m_pTexture[TYPE_MIDDLE_BOSS]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BOSS_002, &m_pTexture[TYPE_MAIN_BOSS]);
	return S_OK;
}
//=============================================================================
// テクスチャアンロード
//=============================================================================
void CBoss::Unload(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_pTexture != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

void CBoss::SubLife(int SubLife)
{
	//スコア
	CScore * pScore = CGame::GetScore();
	//サウンド
	CSound * pSound = CManager::GetSound();
	m_nLife -= SubLife;
	if (m_bHit != true)
	{
		m_bHit = true;
	}
	if (m_nLife <= 0)
	{
	

		CExplosion::Create(m_Pos, EXPLOSION_SIZE * 10, EXPLOSION_SIZE * 10, CExplosion::TYPE_BOMB);
		m_nLifeCount++;
		m_nCountBullet = 0;//バレットカウントを初期化
		m_nConutTime = 0;
	

		switch (m_nType)
		{

		case TYPE_MIDDLE_BOSS://中ザコボス
			if (m_nLifeCount < 4)
			{
				for (int nCount = 0; nCount < 10; nCount++)
				{
					int nRandSeed = rand() % 200 * nCount;
					srand((unsigned int)nRandSeed);
					float RandPosX = float(rand() % 100 + (-100));
					float RandPosY = float(rand() % 300 + (-300));
					CItem::Create(D3DXVECTOR3(m_Pos.x + RandPosX, m_Pos.y + RandPosY, 0.0f), ITEM_SIZE / 3, ITEM_SIZE / 3, CItem::TYPE_SCORE);
				}
				for (int nCount = 0; nCount < 5; nCount++)
				{
					int nRandSeed = rand() % 300 * nCount;
					srand((unsigned int)nRandSeed);
					float RandPosX = float(rand() % 100 + (-100));
					float RandPosY = float(rand() % 300 + (-300));
					CItem::Create(D3DXVECTOR3(m_Pos.x + RandPosX, m_Pos.y + RandPosY, 0.0f), ITEM_SIZE, ITEM_SIZE, CItem::TYPE_POWER);
				}
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);
				m_nLife = MIDDLEBOSS_LIFE;
			}
			break;
		case TYPE_MAIN_BOSS://ラスボス
			for (int nCount = 0; nCount < 10; nCount++)
			{
				int nRandSeed = rand() % 200 * nCount;
				srand((unsigned int)nRandSeed);
				float RandPosX = float(rand() % 100 + (-100));
				float RandPosY = float(rand() % 300 + (-300));
				CItem::Create(D3DXVECTOR3(m_Pos.x + RandPosX, m_Pos.y + RandPosY, 0.0f), ITEM_SIZE / 3, ITEM_SIZE / 3, CItem::TYPE_SCORE);
			}

			for (int nCount = 0; nCount < 2; nCount++)
			{
				int nRandSeed = rand() % 300 * nCount;
				srand((unsigned int)nRandSeed);
				float RandPosX = float(rand() % 100 + (-100));
				float RandPosY = float(rand() % 300 + (-300));
				CItem::Create(D3DXVECTOR3(m_Pos.x + RandPosX, m_Pos.y + RandPosY, 0.0f), ITEM_SIZE, ITEM_SIZE, CItem::TYPE_BOMB);
			}
			for (int nCount = 0; nCount < 5; nCount++)
			{
				int nRandSeed = rand() % 300 * nCount;
				srand((unsigned int)nRandSeed);
				float RandPosX = float(rand() % 100 + (-100));
				float RandPosY = float(rand() % 300 + (-300));
				CItem::Create(D3DXVECTOR3(m_Pos.x + RandPosX, m_Pos.y + RandPosY, 0.0f), ITEM_SIZE, ITEM_SIZE, CItem::TYPE_POWER);
			}
			if (m_nLifeCount < 5)
			{
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);
				m_nLife = MAINBOSS_LIFE;
			}
			
			break;
		default:
			break;
		}
		
	}

		switch (m_nType)
		{

		case TYPE_MIDDLE_BOSS://中ザコボス
			if (m_nLifeCount >= 4)
			{
				pScore->AddScore(50000);
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_BOSSDEATH);
				CItem::Create(m_Pos, ITEM_SIZE, ITEM_SIZE, CItem::TYPE_POWER);
				CExplosion::Create(m_Pos, EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_NORMAL);
				CGame::m_bBossDeath = true;

				for (int nCount = 0; nCount < 50; nCount++)
				{
					int nRandSeed = rand() % 200 * nCount;
					srand((unsigned int)nRandSeed);
					float RandPosX = float(rand() % 100 + (-100));
					float RandPosY = float(rand() % 300 + (-300));
					CItem::Create(D3DXVECTOR3(m_Pos.x + RandPosX, m_Pos.y + RandPosY, 0.0f), ITEM_SIZE / 3, ITEM_SIZE / 3, CItem::TYPE_SCORE);
				}

				for (int nCount = 0; nCount < 1; nCount++)
				{
					int nRandSeed = rand() % 300 * nCount;
					srand((unsigned int)nRandSeed);
					float RandPosX = float(rand() % 100 + (-100));
					float RandPosY = float(rand() % 300 + (-300));
					CItem::Create(D3DXVECTOR3(m_Pos.x + RandPosX, m_Pos.y + RandPosY, 0.0f), ITEM_SIZE, ITEM_SIZE, CItem::TYPE_LIFE);
				}
				Uninit();
				return;
			}
			break;
		case TYPE_MAIN_BOSS://ラスボス
			if (m_nLifeCount >= 5)
			{
				//スコア加算
				pScore->AddScore(100000);
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_BOSSDEATH);
			
				//アイテムドロップ
				CItem::Create(m_Pos, ITEM_SIZE, ITEM_SIZE, CItem::TYPE_SCORE);
				CExplosion::Create(m_Pos, EXPLOSION_SIZE * 5.0f, EXPLOSION_SIZE * 5.0f, CExplosion::TYPE_NORMAL);
				for (int nCount = 0; nCount < 10; nCount++)
				{
					int nRandSeed = rand() % 200 * nCount;
					srand((unsigned int)nRandSeed);
					float RandPosX = float(rand() % 100 + (-100));
					float RandPosY = float(rand() % 300 + (-300));
					CExplosion::Create(D3DXVECTOR3(m_Pos.x + RandPosX, m_Pos.y + RandPosY, 0.0f), EXPLOSION_SIZE * 5.0f, EXPLOSION_SIZE * 5.0f, CExplosion::TYPE_NORMAL);
				}
				//ボス死亡にする
				CGame::m_bBossDeath = true;
				Uninit();
				return;
			}
			break;
		default:
			pScore->AddScore(1000);
			pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);
			CItem::Create(m_Pos, ITEM_SIZE, ITEM_SIZE, CItem::TYPE_SCORE);
			CExplosion::Create(m_Pos, EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_NORMAL);
			Uninit();
			break;
		}

}

void CBoss::ClearAllBullet(void)
{
	for (int nCnt = 0; nCnt < GetNumAll(); nCnt++)
	{
		CScene * pScene = GetScene(4, nCnt);
		if (pScene != NULL)
		{
			OBJTYPE objType;
			objType = pScene->GetObjType();
			if (objType == OBJTYPE_BULLET_ENEMY)
			{
				pScene->Uninit();
			}
		}
	}
}
//=============================================================================
// フェーズ１
//=============================================================================
void CBoss::PhaseOne(void)
{
	CMsgWindow  * pMsg01 = CGame::GetMsgUi01();
	CMsgWindow  * pMsg02 = CGame::GetMsgUi02();
	char str01[512];
	char str02[512];

	CText * pText = CManager::GetText();
	//サウンド
	CSound * pSound = CManager::GetSound();
	switch (m_nType)
	{

	case TYPE_MIDDLE_BOSS://中ザコボス
						  //目標距離に近づいたら目標を再設定する

			pMsg01->SetMsgUi(0);
			pMsg02->SetMsgUi(1);

			wsprintf(str01, "\0");
			wsprintf(str02, "早く逃げないと\n囲まれる！");
			pText->SetTextUi(0, str01);
			pText->SetTextUi(1, str02);

		if (m_Pos.x >= m_TargetPos.x - 10.0f / 2 &&
			m_Pos.x <  m_TargetPos.x + 10.0f / 2 &&
			m_Pos.y >= m_TargetPos.y - 10.0f / 2 &&
			m_Pos.y <  m_TargetPos.y + 10.0f / 2)
		{
			m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			if (m_nConutTime % 200 == 0)
			{
				//ランダムシード値
				int nRandSeed = rand() % FIELD_HEIGHT;
				srand((unsigned int)nRandSeed);
				m_TargetPos.x = (float)(rand() % (FIELD_WIDTH - 100) + 100);//目標距離の決定
				m_TargetPos.y = (float)(rand() % (FIELD_HEIGHT - 100) + 100);
				m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);//目標までの距離を算出
				m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
				m_Move = D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f);
				m_nConutTime = 0;

			}

		}
		else
		{
			m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);//目標までの距離を算出
			m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
			m_Move = D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f);
		}

		if (m_nCountBullet % 10 == 0 || m_nCountBullet % 11 == 0)
		{
			//ショット音の再生
			pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
			//CPlayer * pPlayer = CGame::GetPlayer();
			//m_TargetPos = pPlayer->GetPosition();

			//////目標までの距離を算出//今回は生成時のプレイヤーに向けての方向指定のみ
			//m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);
			//m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
			int nRandSeed = rand() % 360 + -360;
			srand((unsigned int)nRandSeed);
			int nRand = rand() % 360 + -360;
			m_rot.y = D3DXToRadian(nRand);
			//弾の生成
			CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y)*1.0f, sinf(m_rot.y)*1.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_ORANGE, CBullet::ATTACK_NORMAL);
			//弾の生成
			CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y + 0.1f)*1.0f, sinf(m_rot.y + 0.1f)*1.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_ORANGE, CBullet::ATTACK_NORMAL);
			//弾の生成
			CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y - 0.1f)*1.0f, sinf(m_rot.y - 0.1f)*1.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_ORANGE, CBullet::ATTACK_NORMAL);

		}

		if (m_nCountBullet % 200 == 0)
		{
			CPlayer * pPlayer = CGame::GetPlayer();
			if (pPlayer != NULL)
			{
				m_TargetPos = pPlayer->GetPosition();
			}
			else
			{
				m_TargetPos = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 2, 0.0f);
			}
			

			////目標までの距離を算出//今回は生成時のプレイヤーに向けての方向指定のみ
			m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);
			m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
			int nRand = rand() % 50;
			m_rot.y += D3DXToRadian(nRand);
			for (int nCnt = 0; nCnt < 15; nCnt++)
			{
				CExplosion::Create(D3DXVECTOR3(m_TargetPos.x + 200.0f * cosf(D3DXToRadian(nCnt * (360.0f / 15))), m_TargetPos.y + 200.0f * sinf(D3DXToRadian(nCnt * (360.0f / 15))), 0.0f), EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_RESPAWN);
			}
		}
		if (m_nCountBullet % 215 == 0)
		{
			for (int nCnt = 0; nCnt < 15; nCnt++)
			{
				//ショット音の再生
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
				//弾の生成
				CBullet::Create(D3DXVECTOR3(m_TargetPos.x + 200.0f * cosf(D3DXToRadian(nCnt * (360.0f / 15))), m_TargetPos.y + 200.0f * sinf(D3DXToRadian(nCnt * (360.0f / 15))), 0.0f), BULLET_SIZE, BULLET_SIZE, D3DXVECTOR3(cosf(m_rot.y)*2.5f, sinf(m_rot.y)*2.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_CIRCLE, CEffect::COLOR_PURPLE, CBullet::ATTACK_NORMAL);
			}
		}

		break;
	case TYPE_MAIN_BOSS://ラスボス
		
		pMsg01->SetMsgUi(0);
		pMsg02->SetMsgUi(2);

		wsprintf(str01, "いやぁきれいですなぁ");
		wsprintf(str02, "見とれてる場合\nじゃないぞ！");
		pText->SetTextUi(0, str01);
		pText->SetTextUi(1, str02);
		if (m_nConutTime <= 300)
		{

			if (m_nCountBullet % 20 == 0)
			{
				//ショット音の再生
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
				for (int nCnt = 0; nCnt < 20; nCnt++)
				{
					//弾の生成
					CBullet::Create(m_Pos, BULLET_SIZE, BULLET_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(nCnt * (360 / 20)))*3.5f, sinf(D3DXToRadian(nCnt * (360 / 20)))*3.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, m_nColor, CBullet::ATTACK_AIM);
				}

				m_nColor++;
				if (m_nColor > 6)
				{
					m_nColor = 0;
				}
			}
			if (m_nCountBullet % 15 == 0)
			{
				//ショット音の再生
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
				for (int nCnt = 0; nCnt < 20; nCnt++)
				{
					//弾の生成
					CBullet::Create(m_Pos, BULLET_SIZE, BULLET_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(nCnt * (360 / 20) + 45.0f))*3.5f, sinf(D3DXToRadian(nCnt * (360 / 20) + 45.0f))*3.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, m_nColor, CBullet::ATTACK_AIM);
				}
				m_nCountBullet = 1;
				m_nColor++;
				if (m_nColor > 6)
				{
					m_nColor = 0;
				}
			}

		}
		if (m_nConutTime == 400)
		{
			m_nConutTime = 0;
		}
		break;
	default:
		break;
	}
	
}
//=============================================================================
// フェーズ２
//=============================================================================
void CBoss::PhaseTwo(void)
{
	CMsgWindow  * pMsg01 = CGame::GetMsgUi01();
	CMsgWindow  * pMsg02 = CGame::GetMsgUi02();
	char str01[512];
	char str02[512];

	CText * pText = CManager::GetText();
	//サウンド
	CSound * pSound = CManager::GetSound();

	switch (m_nType)
	{

	case TYPE_MIDDLE_BOSS://中ザコボス

		pMsg01->SetMsgUi(1);
		pMsg02->SetMsgUi(1);

		wsprintf(str01, "にしても\n変なロボットですねぇ");
		wsprintf(str02, "一体持ち主は\nどんな奴なんだ？");
		pText->SetTextUi(0, str01);
		pText->SetTextUi(1, str02);
						  //目標距離に近づいたら目標を再設定する

		if (m_Pos.x >= m_TargetPos.x - 10.0f / 2 &&
			m_Pos.x < m_TargetPos.x + 10.0f / 2 &&
			m_Pos.y >= m_TargetPos.y - 10.0f / 2 &&
			m_Pos.y < m_TargetPos.y + 10.0f / 2)
		{
			m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			if (m_nConutTime % 200 == 0)
			{
				//ランダムシード値
				int nRandSeed = rand() % FIELD_HEIGHT;
				srand((unsigned int)nRandSeed);
				m_TargetPos.x = (float)(rand() % (FIELD_WIDTH - 100) + 100);//目標距離の決定
				m_TargetPos.y = (float)(rand() % (FIELD_HEIGHT - 100) + 100);
				m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);//目標までの距離を算出
				m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
				m_Move = D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f);
				m_nConutTime = 0;

			}

		}
		else
		{
			m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);//目標までの距離を算出
			m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
			m_Move = D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f);
		}

		if (m_nCountBullet % 30 == 0)
		{
			//ショット音の再生
			pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
			for (int nCnt = 0; nCnt < 20; nCnt++)
			{
				//弾の生成
				CBullet::Create(m_Pos, BULLET_SIZE, BULLET_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(nCnt * (360 / 20)))*2.5f, sinf(D3DXToRadian(nCnt * (360 / 20)))*2.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_RED, CBullet::ATTACK_NORMAL);
			}
		}
		break;
	case TYPE_MAIN_BOSS://ラスボス
	
		if (m_nConutTime == 0)
		{
			CTalk::Create(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 85.0f, 0.0f), 40, 70, 41, 49);
			CManager::SetIsTalking(true);
			pSound->CSound::StopSound();
			pSound->CSound::PlaySound(CSound::SOUND_LABEL_BGM_BOSS1);
			m_nConutTime++;
		}
		if (m_nConutTime == 2 && CManager::GetIsTalking() == false)
		{
			CBattlebg::Create(D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 2, 0.0f), FIELD_WIDTH + 50.0f, FIELD_HEIGHT);
			pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_WAON);
			m_bossmode = MODE_MONSTER;
			CExplosion::Create(m_Pos, EXPLOSION_SIZE * 3, EXPLOSION_SIZE * 3, CExplosion::TYPE_RESPAWN);
		}
		if (m_nConutTime > 2 && m_nConutTime < 301)
		{
			if (m_nCountBullet % 10 == 0 || m_nCountBullet % 11 == 0)
			{
				int nRandSeed = rand() % 360 + -360;
				srand((unsigned int)nRandSeed);
				int nRand = rand() % 360 + -360;
				m_rot.y = D3DXToRadian(nRand);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y)*1.0f, sinf(m_rot.y)*1.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_SPREAD);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y + 0.1f)*1.0f, sinf(m_rot.y + 0.1f)*1.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_SPREAD);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y - 0.1f)*1.0f, sinf(m_rot.y - 0.1f)*1.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_SPREAD);

			}
		}
		if (m_nConutTime > 301)
		{
			pMsg01->SetMsgUi(1);
			pMsg02->SetMsgUi(0);

			wsprintf(str01, "ほほぉ");
			wsprintf(str02, "変身した！？");
			pText->SetTextUi(0, str01);
			pText->SetTextUi(1, str02);
	
		}
		if (m_nConutTime > 10 && m_nConutTime < 300)
		{
			
			m_TargetDistance = D3DXVECTOR3(100.0f - m_Pos.x, 100.0f - m_Pos.y, 0.0f);//目標までの距離を算出
			m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
			m_Move = D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f);
		}
		if (m_nConutTime > 300)
		{
			if (m_nConutTime == 302)
			{
				m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				if (m_nMoveIndex > 0)
				{
					m_location[0] = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
					m_location[1] = D3DXVECTOR3(FIELD_WIDTH - 100.0f, 100.0f, 0.0f);
					m_location[2] = D3DXVECTOR3(100.0f, FIELD_HEIGHT - 100.0f, 0.0f);
					m_location[3] = D3DXVECTOR3(FIELD_WIDTH - 100.0f, FIELD_HEIGHT - 100.0f, 0.0f);
				}
				else
				{
					m_location[0] = D3DXVECTOR3(FIELD_WIDTH - 100.0f, FIELD_HEIGHT - 100.0f, 0.0f);
					m_location[1] = D3DXVECTOR3(100.0f, FIELD_HEIGHT - 100.0f, 0.0f);
					m_location[2] = D3DXVECTOR3(FIELD_WIDTH - 100.0f, 100.0f, 0.0f);
					m_location[3] = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
				}
				m_fMovetime = 0.0f;
			}

			if (m_nCountBullet % 10 == 0 || m_nCountBullet % 11 == 0)
			{
				int nRandSeed = rand() % 360 + -360;
				srand((unsigned int)nRandSeed);
				int nRand = rand() % 360 + -360;
				m_rot.y = D3DXToRadian(nRand);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y)*1.0f, sinf(m_rot.y)*1.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_SPREAD);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y + 0.1f)*1.0f, sinf(m_rot.y + 0.1f)*1.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_SPREAD);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y - 0.1f)*1.0f, sinf(m_rot.y - 0.1f)*1.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_SPREAD);

			}
			if (m_nConutTime % 850 == 0)
			{
				/*m_Move.x = 0.0f;*/
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_WAON);
			}
			if (m_nConutTime % 1000 == 0)
			{
				/*m_Move.x = 2.5f;*/
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SYAN);
				for (int nCnt = 0; nCnt < MAX_POLYGON; nCnt++)
				{
					CScene * pScene = GetScene(4, nCnt);
					if (pScene != NULL)
					{
						OBJTYPE objType;
						objType = pScene->GetObjType();
						if (objType == OBJTYPE_BULLET_ENEMY)
						{
							//pScene(CScene)をCEnemyにダウンキャスト
							CBullet * pBullet = dynamic_cast<CBullet*> (pScene);
							pBullet->SpreadBullet(nCnt);
						}
					}
				}
				m_nConutTime = 301;
				m_nMoveIndex = m_nMoveIndex * -1;
			}
		}
	
	
		

		//左移動
		if (m_Pos.x < m_PosOld.x)
		{
			m_nPatternAnim = 0;
		}
		//右移動
		else if (m_Pos.x > m_PosOld.x)
		{
			m_nPatternAnim = 2;
		}
		else if (m_Pos.x < m_PosOld.x + 2.0f &&m_Pos.x > m_PosOld.x - 2.0f)
		{
			m_nPatternAnim = 1;
		}
		break;
	default:
		break;
	}


}
//=============================================================================
// フェーズ３
//=============================================================================
void CBoss::PhaseThree(void)
{
	CMsgWindow  * pMsg01 = CGame::GetMsgUi01();
	CMsgWindow  * pMsg02 = CGame::GetMsgUi02();
	char str01[512];
	char str02[512];

	CText * pText = CManager::GetText();
	//サウンド
	CSound * pSound = CManager::GetSound();
	switch (m_nType)
	{

	case TYPE_MIDDLE_BOSS://中ザコボス

		pMsg01->SetMsgUi(0);
		pMsg02->SetMsgUi(0);

		wsprintf(str01, "もうそろですよぉ！");
		wsprintf(str02, "早く終わってくれ...!!");
		pText->SetTextUi(0, str01);
		pText->SetTextUi(1, str02);
						  //目標距離に近づいたら目標を再設定する
		if (m_Pos.x >= m_TargetPos.x - 10.0f / 2 &&
			m_Pos.x <  m_TargetPos.x + 10.0f / 2 &&
			m_Pos.y >= m_TargetPos.y - 10.0f / 2 &&
			m_Pos.y <  m_TargetPos.y + 10.0f / 2)
		{
			m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			if (m_nConutTime % 200 == 0)
			{
				//ランダムシード値
				int nRandSeed = rand() % FIELD_HEIGHT;
				srand((unsigned int)nRandSeed);
				m_TargetPos.x = (float)(rand() % (FIELD_WIDTH - 100) + 100);//目標距離の決定
				m_TargetPos.y = (float)(rand() % (FIELD_HEIGHT - 100) + 100);
				m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);//目標までの距離を算出
				m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
				m_Move = D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f);
				m_nConutTime = 0;

			}

		}
		else
		{
			m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);//目標までの距離を算出
			m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
			m_Move = D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f);
		}
		if (m_nCountBullet % 21 == 0)
		{
			//ショット音の再生
			pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
			for (int nCnt = 0; nCnt < 20; nCnt++)
			{
				//弾の生成
				CBullet::Create(m_Pos, BULLET_SIZE, BULLET_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(nCnt * (360 / 20)))*5.5f, sinf(D3DXToRadian(nCnt * (360 / 20)))*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_RED, CBullet::ATTACK_NORMAL);
			}
		}
		if (m_nCountBullet % 41 == 0)
		{
			//ショット音の再生
			pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
			for (int nCnt = 0; nCnt < 20; nCnt++)
			{
				//弾の生成
				CBullet::Create(m_Pos, BULLET_SIZE, BULLET_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(nCnt * (360 / 25)))*5.5f, sinf(D3DXToRadian(nCnt * (360 / 25)))*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_YELLOW, CBullet::ATTACK_NORMAL);
			}
		}
		if (m_nCountBullet > 600)
		{
			//ボスからの発射玉
			if (m_nCountBullet % 80 == 0)
			{
				//ショット音の再生
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_HIKKAKI);
				CPlayer * pPlayer = CGame::GetPlayer();
				D3DXVECTOR3 TargetPos = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 2, 0.0f);
				if (pPlayer != NULL)
				{
					TargetPos = pPlayer->GetPosition();
				}
				else
				{
					TargetPos = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 2, 0.0f);
				}

				////目標までの距離を算出//今回は生成時のプレイヤーに向けての方向指定のみ
				D3DXVECTOR3 TargetDistance = D3DXVECTOR3(TargetPos.x - m_Pos.x, TargetPos.y - m_Pos.y, 0.0f);
				m_rot.y = atan2f(TargetDistance.y, TargetDistance.x);

				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y)*7.0f, sinf(m_rot.y)*7.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_YELLOW, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y + 0.1f)*6.8f, sinf(m_rot.y + 0.1f)*6.8f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_YELLOW, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y + 0.2f)*6.5f, sinf(m_rot.y + 6.5f)*6.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_ORANGE, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y - 0.1f)*6.8f, sinf(m_rot.y - 0.1f)*6.8f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_YELLOW, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y - 0.2f)*6.5f, sinf(m_rot.y - 0.2f)*6.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_ORANGE, CBullet::ATTACK_NORMAL);
				

			}
		}

		break;
	case TYPE_MAIN_BOSS://中ザコボス
						//目標距離に近づいたら目標を再設定する
		pMsg01->SetMsgUi(0);
		pMsg02->SetMsgUi(0);

		wsprintf(str01, "格子状の弾幕が\n厄介ですねぇ！");
		wsprintf(str02, "しかもめちゃくちゃ\n撃ってくる！");
		pText->SetTextUi(0, str01);
		pText->SetTextUi(1, str02);
		if (m_Pos.x >= m_TargetPos.x - 10.0f / 2 &&
			m_Pos.x <  m_TargetPos.x + 10.0f / 2 &&
			m_Pos.y >= m_TargetPos.y - 10.0f / 2 &&
			m_Pos.y <  m_TargetPos.y + 10.0f / 2)
		{
			m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			if (m_nConutTime % 200 == 0)
			{
				//ランダムシード値
				int nRandSeed = rand() % FIELD_HEIGHT;
				srand((unsigned int)nRandSeed);
				m_TargetPos.x = (float)(rand() % (FIELD_WIDTH - 100) + 100);//目標距離の決定
				m_TargetPos.y = (float)(rand() % (FIELD_HEIGHT - 100) + 100);
				m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);//目標までの距離を算出
				m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
				m_Move = D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f);
				m_nConutTime = 0;

			}

		}
		else
		{
			m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);//目標までの距離を算出
			m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
			m_Move = D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f);
		}
		//発射間隔を狭めていく
		if (m_nCountBullet % 70 == 0)
		{
			m_nBulletShotinterval -= 8;
		}
		if (m_nBulletShotinterval > 0)
		{
			//ボスからの発射玉
			if (m_nCountBullet % m_nBulletShotinterval == 0)
			{
				CPlayer * pPlayer = CGame::GetPlayer();
				D3DXVECTOR3 TargetPos = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 2, 0.0f);
				if (pPlayer != NULL)
				{
					TargetPos = pPlayer->GetPosition();
				}
				else
				{
					TargetPos = D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 2, 0.0f);
				}
				//ショット音の再生
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
				////目標までの距離を算出//今回は生成時のプレイヤーに向けての方向指定のみ
				D3DXVECTOR3 TargetDistance = D3DXVECTOR3(TargetPos.x - m_Pos.x, TargetPos.y - m_Pos.y, 0.0f);
				m_rot.y = atan2f(TargetDistance.y, TargetDistance.x);

				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y)*CLAW_SHOT_SPEED, sinf(m_rot.y)*CLAW_SHOT_SPEED, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_YELLOW, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y + 0.1f)*CLAW_SHOT_SPEED - 0.2f, sinf(m_rot.y + 0.1f)*CLAW_SHOT_SPEED - 0.2f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_YELLOW, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y + 0.2f)*CLAW_SHOT_SPEED - 0.5f, sinf(m_rot.y + 0.2f)*CLAW_SHOT_SPEED - 0.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_ORANGE, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y + 0.3f)*CLAW_SHOT_SPEED - 0.7f, sinf(m_rot.y + 0.3f)*CLAW_SHOT_SPEED - 0.7f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_RED, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y - 0.1f)*CLAW_SHOT_SPEED - 0.2f, sinf(m_rot.y - 0.1f)*CLAW_SHOT_SPEED - 0.2f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_YELLOW, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y - 0.2f)*CLAW_SHOT_SPEED - 0.5f, sinf(m_rot.y - 0.2f)*CLAW_SHOT_SPEED - 0.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_ORANGE, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y - 0.3f)*CLAW_SHOT_SPEED - 0.7f, sinf(m_rot.y - 0.3f)*CLAW_SHOT_SPEED - 0.7f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_RED, CBullet::ATTACK_NORMAL);

			}
		}
		else
		{
			m_nBulletShotinterval = 100;//間隔を戻す
		}

		if (m_nCountBullet % 50 == 0)
		{
			//ショット音の再生
			pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
			for (int nCount = 0; nCount < 20; nCount++)
			{
				//弾の生成
				CBullet::Create(D3DXVECTOR3(nCount * 200.0f, 0.0f, 0.0f), BULLET_SIZE * 0.5f, BULLET_SIZE * 0.5f, D3DXVECTOR3(0.0f, 2.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_CIRCLE, CEffect::COLOR_BLUE, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(D3DXVECTOR3(0.0f, nCount * 200.0f, 0.0f), BULLET_SIZE * 0.5f, BULLET_SIZE * 0.5f, D3DXVECTOR3(2.0f, 0.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_CIRCLE, CEffect::COLOR_BLUE, CBullet::ATTACK_NORMAL);
			}

		}




		//左移動
		if (m_Pos.x < m_PosOld.x)
		{
			m_nPatternAnim = 0;
		}
		//右移動
		else if (m_Pos.x > m_PosOld.x)
		{
			m_nPatternAnim = 2;
		}
		else if (m_Pos.x < m_PosOld.x + 2.0f &&m_Pos.x > m_PosOld.x - 2.0f)
		{
			m_nPatternAnim = 1;
		}

		break;
	default:
		break;
	}
	

}
//=============================================================================
// フェーズ４
//=============================================================================
void CBoss::PhaseFour(void)
{
	CMsgWindow  * pMsg01 = CGame::GetMsgUi01();
	CMsgWindow  * pMsg02 = CGame::GetMsgUi02();
	char str01[512];
	char str02[512];

	CText * pText = CManager::GetText();
	//サウンド
	CSound * pSound = CManager::GetSound();
	switch (m_nType)
	{

	case TYPE_MIDDLE_BOSS://中ザコボス

		break;
	case TYPE_MAIN_BOSS://中ザコボス
		pMsg01->SetMsgUi(0);
		pMsg02->SetMsgUi(0);

		wsprintf(str01, "悪いわんこには\nお仕置きですよぉ！");
		wsprintf(str02, "お仕置きされる側には\nならないようにな！！");
		pText->SetTextUi(0, str01);
		pText->SetTextUi(1, str02);
		if (m_Pos.x >= m_TargetPos.x - 10.0f / 2 &&
			m_Pos.x <  m_TargetPos.x + 10.0f / 2 &&
			m_Pos.y >= m_TargetPos.y - 10.0f / 2 &&
			m_Pos.y <  m_TargetPos.y + 10.0f / 2)
		{
			m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			if (m_nConutTime % 200 == 0)
			{
				//ランダムシード値
				int nRandSeed = rand() % FIELD_HEIGHT;
				srand((unsigned int)nRandSeed);
				m_TargetPos.x = (float)(rand() % (FIELD_WIDTH - 100) + 100);//目標距離の決定
				m_TargetPos.y = (float)(rand() % (FIELD_HEIGHT - 100) + 100);
				m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);//目標までの距離を算出
				m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
				m_Move = D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f);
				m_nConutTime = 0;

			}

		}
		else
		{
			m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);//目標までの距離を算出
			m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
			m_Move = D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f);
		}
		if (m_nCountBullet % 70 == 0)
		{
			//ショット音の再生
			pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
			int nRandSeed = rand() % 360 + -360;
			srand((unsigned int)nRandSeed);
			int nRand = rand() % 360 + -360;
			m_rot.y = D3DXToRadian(nRand);
			//弾の生成
			CBullet::Create(D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f), BULLET_SIZE, BULLET_SIZE, D3DXVECTOR3(cosf(m_rot.y)*5.0f, sinf(m_rot.y)*5.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_RED, CBullet::ATTACK_BOUND);

		}
		if (m_nCountBullet % 200 == 0)
		{
			//ショット音の再生
			pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SYAN);
			int nRandSeed = rand() % 360 + -360;
			srand((unsigned int)nRandSeed);
			int nRand = rand() % 360 + -360;
			m_rot.y = D3DXToRadian(nRand);
			//弾の生成
			CBullet::Create(D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f), BULLET_SIZE * 2, BULLET_SIZE * 2, D3DXVECTOR3(cosf(m_rot.y)*7.0f, sinf(m_rot.y)*7.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DOG, CEffect::COLOR_RED, CBullet::ATTACK_BOTOBOTO);

		}

		if (m_nCountBullet > 600)
		{

			if (m_nCountBullet % 200 == 0)
			{
				//ショット音の再生
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SYAN);
				//弾の生成
				CBullet::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), BULLET_SIZE * 2, BULLET_SIZE * 2, D3DXVECTOR3(cosf(D3DXToRadian(45.0f))*7.0f, sinf(D3DXToRadian(45.0f))*7.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DOG, CEffect::COLOR_RED, CBullet::ATTACK_BOTOBOTO);

			}
		}
		if (m_nCountBullet > 800)
		{
	
			if (m_nCountBullet % 200 == 0)
			{
				//ショット音の再生
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SYAN);
				//弾の生成
				CBullet::Create(D3DXVECTOR3(FIELD_WIDTH, 0.0f, 0.0f), BULLET_SIZE * 2, BULLET_SIZE * 2, D3DXVECTOR3(cosf(D3DXToRadian(135.0f))*7.0f, sinf(D3DXToRadian(135.0f))*7.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DOG, CEffect::COLOR_RED, CBullet::ATTACK_BOTOBOTO);

			}
		}
		if (m_nCountBullet > 1000)
		{
	
			if (m_nCountBullet % 200 == 0)
			{
				//ショット音の再生
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SYAN);
				//弾の生成
				CBullet::Create(D3DXVECTOR3(FIELD_WIDTH / 2, 0.0f, 0.0f), BULLET_SIZE * 2, BULLET_SIZE * 2, D3DXVECTOR3(0.0f, 7.0f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DOG, CEffect::COLOR_RED, CBullet::ATTACK_BOTOBOTO);

			}
		}

		//左移動
		if (m_Pos.x < m_PosOld.x)
		{
			m_nPatternAnim = 0;
		}
		//右移動
		else if (m_Pos.x > m_PosOld.x)
		{
			m_nPatternAnim = 2;
		}
		else if (m_Pos.x < m_PosOld.x + 2.0f &&m_Pos.x > m_PosOld.x - 2.0f)
		{
			m_nPatternAnim = 1;
		}

		break;
	default:
		break;
	}

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBoss::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeWidth, float fSizeHeight, TYPE Type, D3DXVECTOR3 location[MAX_LOCATION], int nLife)
{
	CScene2D::Init(pos, fSizeWidth, fSizeHeight);

	for (int nCount = 0; nCount < MAX_LOCATION; nCount++)
	{
		m_location[nCount] = location[nCount];
	}

	m_Move = move;
	SetObjType(CScene::OBJTYPE_ENEMY);
	// ポリゴンの位置を設定
	CScene2D::SetPosition(pos);

	m_nType = Type;
	m_nLife = nLife;
	m_Pos = pos;
	// 対角線、角度の設定
	m_fLength = sqrtf((float)(((fSizeWidth / 2) * (fSizeWidth / 2)) + ((fSizeHeight / 2) * (fSizeHeight / 2))));
	m_fAngle = atan2f((fSizeHeight / 2), (fSizeWidth / 2));
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;
	m_fAddScale = 0.01f;

	//ランダムシード値
	int nRandSeed = rand() % 255;
	srand((unsigned int)nRandSeed);

	m_TargetPos.x = float(rand() % FIELD_WIDTH);
	m_TargetPos.y = float(rand() % FIELD_HEIGHT);

	//頂点座標の設定
	m_vPos[0].x = pos.x - cosf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	m_vPos[0].y = pos.y - sinf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	m_vPos[0].z = 0.0f;

	m_vPos[1].x = pos.x + cosf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	m_vPos[1].y = pos.y - sinf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	m_vPos[1].z = 0.0f;

	m_vPos[2].x = pos.x - cosf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	m_vPos[2].y = pos.y + sinf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	m_vPos[2].z = 0.0f;

	m_vPos[3].x = pos.x + cosf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	m_vPos[3].y = pos.y + sinf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	m_vPos[3].z = 0.0f;

	//頂点座標セット
	SetVtxPosition(m_vPos);

	//色のセット
	SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	switch (m_nType)
	{
	case TYPE_MIDDLE_BOSS:
		//テクスチャ座標のセット
		SetTex(
			m_nPatternAnim * 0.3333f,
			m_bossmode * 0.5f,
			m_nPatternAnim * 0.3333f + 0.3333f,
			m_bossmode * 0.5f + 0.5f);
		break;
	case TYPE_MAIN_BOSS:
		//テクスチャ座標のセット
		SetTex(
			m_nPatternAnim * 0.3333f,
			m_bossmode * 0.5f,
			m_nPatternAnim * 0.3333f + 0.3333f,
			m_bossmode * 0.5f + 0.5f);
		break;
	default:
		break;
	}

	//テクスチャのバインド
	BindTexture(m_pTexture[m_nType]);



	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBoss::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBoss::Update(void)
{
	if (CManager::GetIsTalking() == false)
	{



		if (m_nConutTime % 100 == 0)
		{
			m_PosOld = m_Pos;
		}

		CScene2D::Update();
		//サウンド
		CSound * pSound = CManager::GetSound();
		//位置を取得
		m_Pos = CScene2D::GetPosition();

		//位置更新
		m_Pos += m_Move;

		if (m_fMovetime < 1)
		{
			//タイム加算
			m_fMovetime += 0.005f;

			//ベジェ曲線計算
			m_Pos.x = (1.0f - m_fMovetime) * (1.0f - m_fMovetime) * (1.0f - m_fMovetime) * m_location[0].x +
				3.0f * (1.0f - m_fMovetime) *(1.0f - m_fMovetime) * m_fMovetime * m_location[1].x +
				3.0f * (1.0f - m_fMovetime) * m_fMovetime * m_fMovetime * m_location[2].x +
				m_fMovetime * m_fMovetime * m_fMovetime * m_location[3].x;

			m_Pos.y = (1.0f - m_fMovetime) * (1.0f - m_fMovetime) * (1.0f - m_fMovetime) * m_location[0].y +
				3.0f * (1.0f - m_fMovetime) *(1.0f - m_fMovetime) * m_fMovetime * m_location[1].y +
				3.0f * (1.0f - m_fMovetime) * m_fMovetime * m_fMovetime * m_location[2].y +
				m_fMovetime * m_fMovetime * m_fMovetime * m_location[3].y;
		}


		//頂点座標の設定
		m_vPos[0].x = m_Pos.x - cosf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
		m_vPos[0].y = m_Pos.y - sinf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
		m_vPos[0].z = 0.0f;

		m_vPos[1].x = m_Pos.x + cosf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
		m_vPos[1].y = m_Pos.y - sinf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
		m_vPos[1].z = 0.0f;

		m_vPos[2].x = m_Pos.x - cosf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
		m_vPos[2].y = m_Pos.y + sinf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
		m_vPos[2].z = 0.0f;

		m_vPos[3].x = m_Pos.x + cosf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
		m_vPos[3].y = m_Pos.y + sinf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
		m_vPos[3].z = 0.0f;

		//頂点座標セット
		SetVtxPosition(m_vPos);

		//色のセット
		SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));



		switch (m_nType)
		{
		case TYPE_MIDDLE_BOSS://中ボス０１

							  //スタート時の攻撃パターン
			if (m_nLifeCount == 0)
			{
				if (m_nCountBullet % 7 == 0)
				{
					//ショット音の再生
					pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
					//弾の生成
					CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*-5.5f, sinf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*-5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_NORMAL);
					//弾の生成
					CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*5.5f, sinf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_NORMAL);
				}

				if (m_nCountBullet > 500)
				{
					if (m_nCountBullet % 10 == 0)
					{
						//ショット音の再生
						pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
						for (int nCnt = 0; nCnt < 5; nCnt++)
						{
							//弾の生成
							CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(m_nCountBullet + 210.0f + (nCnt * 5)))*5.5f, sinf(D3DXToRadian(m_nCountBullet + 210.0f + (nCnt * 10)))*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, m_nColor, CBullet::ATTACK_NORMAL);
						}
						for (int nCnt = 0; nCnt < 5; nCnt++)
						{
							//弾の生成
							CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(m_nCountBullet + 150.0f - (nCnt * 5)))*5.5f, sinf(D3DXToRadian(m_nCountBullet + 150.0f - (nCnt * 10)))*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, m_nColor, CBullet::ATTACK_NORMAL);
						}
					}
				}
			}
			//フェーズ１の攻撃パターン
			if (m_nLifeCount == 1)
			{
				PhaseOne();
			}
			//フェーズ２のこうどうパターン
			if (m_nLifeCount == 2)
			{
				PhaseTwo();
			}
			//フェーズ２のこうどうパターン
			if (m_nLifeCount == 3)
			{
				PhaseThree();
			}
			//フェーズ２のこうどうパターン
			if (m_nLifeCount == 4)
			{
				PhaseFour();
			}
			m_nConutTime++;
			m_nCountBullet++;
			break;
		case TYPE_MAIN_BOSS://中ボス０１

							//スタート時の攻撃パターン
			if (m_nLifeCount == 0)
			{
				if (m_nCountBullet % 7 == 0)
				{
					//ショット音の再生
					pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
					//弾の生成
					CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*-5.5f, sinf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*-5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_NORMAL);
					//弾の生成
					CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*5.5f, sinf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_NORMAL);
				}

				if (m_nCountBullet > 500)
				{
					if (m_nCountBullet % 10 == 0)
					{
						//ショット音の再生
						pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
						for (int nCnt = 0; nCnt < 5; nCnt++)
						{
							//弾の生成
							CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(m_nCountBullet + 210.0f + (nCnt * 5)))*5.5f, sinf(D3DXToRadian(m_nCountBullet + 210.0f + (nCnt * 10)))*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, m_nColor, CBullet::ATTACK_NORMAL);
						}
						for (int nCnt = 0; nCnt < 5; nCnt++)
						{
							//弾の生成
							CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(m_nCountBullet + 150.0f - (nCnt * 5)))*5.5f, sinf(D3DXToRadian(m_nCountBullet + 150.0f - (nCnt * 10)))*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, m_nColor, CBullet::ATTACK_NORMAL);
						}
					}
				}
			}
			//フェーズ１の攻撃パターン
			if (m_nLifeCount == 1)
			{
				PhaseOne();
			}
			//フェーズ２のこうどうパターン
			if (m_nLifeCount == 2)
			{
				PhaseTwo();
			}
			//フェーズ２のこうどうパターン
			if (m_nLifeCount == 3)
			{
				PhaseThree();
			}
			//フェーズ２のこうどうパターン
			if (m_nLifeCount == 4)
			{
				PhaseFour();
			}
			m_nConutTime++;
			m_nCountBullet++;
			break;
		default:
			break;
		}
		switch (m_nType)
		{
		case TYPE_MIDDLE_BOSS:
			//テクスチャ座標のセット
			SetTex(
				0.0f,
				0.0f,
				1.0f,
				1.0f);
			break;
		case TYPE_MAIN_BOSS:
			//テクスチャ座標のセット
			SetTex(
				m_nPatternAnim * 0.3333f,
				m_bossmode * 0.5f,
				m_nPatternAnim * 0.3333f + 0.3333f,
				m_bossmode * 0.5f + 0.5f);
			break;
		default:
			break;
		}

		CScene2D::SetPosition(m_Pos);

		if (m_Pos.y < 0.0f ||
			m_Pos.y > FIELD_HEIGHT ||
			m_Pos.x < 0.0f ||
			m_Pos.x > FIELD_WIDTH)
		{
			Uninit();
			return;
		}

	

	}
	//ヒット中か
	if (m_bHit == true)
	{
		m_nHitCount++;
		if (m_nHitCount >= 20)
		{
			m_bHit = false;
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CBoss::Draw(void)
{
	CScene2D::Draw();
	//ヒット中白く加算する
	if(m_bHit == true)
	{
		CRenderer * pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		CScene2D::Draw();
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}