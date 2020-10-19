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
#include "enemy.h"
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

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[TYPE_MAX] = {};   //テクスチャのポインタ
//=============================================================================
// コンストラクタ
//=============================================================================
CEnemy::CEnemy(int nPriority) : CScene2D(nPriority)
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_fLength = 0.0f;		// 対角線の長さ
	m_fAngle = 0.0f;		// 対角線の角度
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 向き
	m_fScale = 0.0f;		// 拡大
	m_fAddScale = 0.0f;	// 拡大加算値
	m_bHit = false;
	m_nHitCount = 0;
	m_TargetDistance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//目標までの距離
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//目標地点
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量

	m_RandColorR = 255;
	m_RandColorG = 255;
	m_RandColorB = 255;

	m_nPatternAnim = 1;
	m_nCountAnim = 0;
	m_nCountBullet = 0;
	m_nType = TYPE_001;
	m_nLife = 20;
	m_nColor = 0;
	m_nConutTime = 0;
	m_fMovetime = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeWidth, float fSizeHeight, TYPE Type, D3DXVECTOR3 location[MAX_LOCATION], int nLife)
{
	CEnemy *pEnemy;
	pEnemy = new CEnemy;
	pEnemy->Init(pos,move, fSizeWidth, fSizeHeight,Type, location,nLife);
	return pEnemy;
}
//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CEnemy::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ENEMY_001, &m_pTexture[TYPE_001]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ENEMY_002, &m_pTexture[TYPE_002]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ENEMY_ZAKO, &m_pTexture[TYPE_003]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ENEMY_ZAKO, &m_pTexture[TYPE_ZAKO]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ENEMY_002, &m_pTexture[TYPE_MIDDLE_BOSS]);
	return S_OK;
}
//=============================================================================
// テクスチャアンロード
//=============================================================================
void CEnemy::Unload(void)
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

void CEnemy::SubLife(int SubLife)
{
	//スコア
	CScore * pScore = CGame::GetScore();
	//サウンド
	CSound * pSound = CManager::GetSound();
	m_nLife -= SubLife; 
	//ヒット中にする
	if (m_bHit != true)
	{
		m_bHit = true;
	}
	if (m_nLife <= 0)
	{
		
		
		switch (m_nType)
		{
		case TYPE_MIDDLE_BOSS://中ザコボス
			pScore->AddScore(50000);
			pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);
			CItem::Create(m_Pos, ITEM_SIZE, ITEM_SIZE, CItem::TYPE_LIFE);

			for (int nCount = 0; nCount < 20; nCount++)
			{
				int nRandSeed = rand() % 200 * nCount;
				srand((unsigned int)nRandSeed);
				float RandPosX = float(rand() % 100 + (-100));
				float RandPosY = float(rand() % 300 + (-300));
				CItem::Create(D3DXVECTOR3(m_Pos.x + RandPosX, m_Pos.y + RandPosY,0.0f), ITEM_SIZE / 3, ITEM_SIZE / 3, CItem::TYPE_SCORE);
			}

			for (int nCount = 0; nCount < 1; nCount++)
			{
				int nRandSeed = rand() % 300 * nCount;
				srand((unsigned int)nRandSeed);
				float RandPosX = float(rand() % 100 + (-100));
				float RandPosY = float(rand() % 300 + (-300));
				CItem::Create(D3DXVECTOR3(m_Pos.x + RandPosX, m_Pos.y + RandPosY, 0.0f), ITEM_SIZE / 2, ITEM_SIZE / 2, CItem::TYPE_LIFE);
			}

			for (int nCount = 0; nCount < 5; nCount++)
			{
				int nRandSeed = rand() % 300 * nCount;
				srand((unsigned int)nRandSeed);
				float RandPosX = float(rand() % 100 + (-100));
				float RandPosY = float(rand() % 300 + (-300));
				CItem::Create(D3DXVECTOR3(m_Pos.x + RandPosX, m_Pos.y + RandPosY, 0.0f), ITEM_SIZE / 2, ITEM_SIZE / 2, CItem::TYPE_POWER);
			}

			CExplosion::Create(m_Pos, EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_NORMAL);
	
			Uninit();
			return;
			break;
		default:
			pScore->AddScore(1000);
			pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);
			CItem::Create(m_Pos, ITEM_SIZE, ITEM_SIZE, CItem::TYPE_SCORE);
			CExplosion::Create(m_Pos, EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_NORMAL);

			for (int nCount = 0; nCount < 10; nCount++)
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
				CItem::Create(D3DXVECTOR3(m_Pos.x + RandPosX, m_Pos.y + RandPosY, 0.0f), ITEM_SIZE / 2, ITEM_SIZE / 2, CItem::TYPE_BOMB);
			}
			Uninit();
			return;
			break;
		}
		
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeWidth, float fSizeHeight, TYPE Type, D3DXVECTOR3 location[MAX_LOCATION], int nLife)
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

	//ランダムな色の数値取得
	m_RandColorR = rand() % 255;
	m_RandColorG = rand() % 255;
	m_RandColorB = rand() % 255;

	//色のセット
	SetColor(D3DCOLOR_RGBA(m_RandColorR, m_RandColorG, m_RandColorB, 255));

	switch (m_nType)
	{
	case TYPE_001:
		//テクスチャ座標のセット
		SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
		break;
	case TYPE_002:
		//テクスチャ座標のセット
		SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
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
void CEnemy::Uninit(void)
{

	////位置を取得
	//D3DXVECTOR3 pos = CScene2D::GetPosition();
	/*CExplosion::Create(pos, TEXTURE_WIDTH, TEXTURE_HEIGHT);*/
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	CScene2D::Update();
	//サウンド
	CSound * pSound = CManager::GetSound();
	
	m_nConutTime++;

	//位置を取得
	m_Pos = CScene2D::GetPosition();
		//// 回転
		//m_rot.z += D3DX_PI * 0.01f;
		//if (m_rot.z >= D3DX_PI)//一定の値を超えると回らなくなるので減算していく
		//{
		//	m_rot.z -= D3DX_PI * 2.0f;
		//}

		//エフェクト生成
	CEffect::Create(m_Pos, ENEMY_SIZE_001_HEIGHT, ENEMY_SIZE_001_HEIGHT, m_Move, CEffect::TYPE_003,CEffect::COLOR_RED,2);

	////目標距離に近づいたら目標を再設定する
	//if (m_Pos.x >= m_TargetPos.x - ENEMY_SIZE_001_WIDTH / 2 &&
	//	m_Pos.x <  m_TargetPos.x + ENEMY_SIZE_001_WIDTH / 2 &&
	//	m_Pos.y >= m_TargetPos.y - ENEMY_SIZE_001_HEIGHT / 2 &&
	//	m_Pos.y <  m_TargetPos.y + ENEMY_SIZE_001_HEIGHT / 2)
	//{
	//	//ランダムシード値
	//	int nRandSeed = rand() % 255;
	//	srand((unsigned int)time(NULL)*nRandSeed);
	//	m_TargetPos.x = 0 + (float)(rand() * (FIELD_WIDTH - 0 + 1.0) / (1.0 + RAND_MAX));//目標距離の決定
	//	m_TargetPos.y = 0 + (float)(rand() * (FIELD_HEIGHT - 0 + 1.0) / (1.0 + RAND_MAX));
	//}
	//else
	//{
	//	m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);//目標までの距離を算出
	//	m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
	//	m_Move = D3DXVECTOR3(cosf(m_rot.y)*0.5f, sinf(m_rot.y)*0.5f, 0.0f);
	//}



	/*if (m_nConutTime == 200)
	{
		m_Move.y = m_Move.y*-1.0f;
	}*/
		////位置更新
		//m_Pos += m_Move;

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
		////３６０度円形射撃
		case TYPE_001:
			/*if (m_fMovetime <= 0.0f)
			{*/
				if (m_nCountBullet == 300)
				{
					m_location[0] = m_Pos;
					m_location[1] = D3DXVECTOR3(m_Pos.x, m_Pos.y - 100.0f, 0.0f);
					m_location[2] = D3DXVECTOR3(m_Pos.x, m_Pos.y - 200.0f, 0.0f);
					m_location[3] = D3DXVECTOR3(m_Pos.x, 0.0f, 0.0f);
					m_fMovetime = 0.0f;
				}
				else
				{
					if (int(m_Pos.y) == int(m_location[3].y))
					{
						if (m_nCountBullet % 20 == 0)
						{
							//ショット音の再生
							pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								//弾の生成
								CBullet::Create(m_Pos, BULLET_SIZE, BULLET_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(nCnt * (360 / 20)))*5.5f, sinf(D3DXToRadian(nCnt * (360 / 20)))*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_CIRCLE, m_nColor, CBullet::ATTACK_NORMAL);
							}

							m_nColor++;
							if (m_nColor > 6)
							{
								m_nColor = 0;
							}
						}
						if (m_nCountBullet % 41 == 0)
						{
							//ショット音の再生
							pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
							for (int nCnt = 0; nCnt < 20; nCnt++)
							{
								//弾の生成
								CBullet::Create(m_Pos, BULLET_SIZE, BULLET_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(nCnt * (360 / 20) + 45.0f))*5.5f, sinf(D3DXToRadian(nCnt * (360 / 20) + 45.0f))*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_CIRCLE, m_nColor, CBullet::ATTACK_NORMAL);
							}
							m_nColor++;
							if (m_nColor > 6)
							{
								m_nColor = 0;
							}
						}
					}
				}
			
		
			break;
		////ぐるぐる射撃
		case TYPE_002:
			//ぐるぐる弾
			if (m_nCountBullet % 5 == 0)
			{
				
				//ショット音の再生
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*-5.5f, sinf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*-5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, m_nColor, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*5.5f, sinf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, m_nColor, CBullet::ATTACK_NORMAL);
			//カラー番号のカウント
				m_nColor++;
				if (m_nColor > 6)
				{
					m_nColor = 0;
				}
			}
			break;
		case TYPE_003://ただ出す
			if (m_nCountBullet % 10 == 0)
			{
				CPlayer * pPlayer = CGame::GetPlayer();
				m_TargetPos = pPlayer->GetPosition();

				////目標までの距離を算出//今回は生成時のプレイヤーに向けての方向指定のみ
				m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);
				m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
	
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, m_nColor, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, m_nColor, CBullet::ATTACK_NORMAL);
			}
			break;
		case TYPE_MIDDLE_BOSS://中ボス０

			if (m_nCountBullet < 500)
			{
				//ぐるぐる弾
				if (m_nCountBullet % 7 == 0)
				{

					//ショット音の再生
					pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
					//弾の生成
					CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*-5.5f, sinf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*-5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_NORMAL);
					//弾の生成
					CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*5.5f, sinf(D3DXToRadian(m_nCountBullet / 5 * (360 / 20)))*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_NORMAL);
				}
				if (m_nCountBullet % 20 == 0)
				{
					CPlayer * pPlayer = CGame::GetPlayer();
					m_TargetPos = pPlayer->GetPosition();

					////目標までの距離を算出//今回は生成時のプレイヤーに向けての方向指定のみ
					m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);
					m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);

					//弾の生成
					CBullet::Create(m_Pos, BULLET_SIZE, BULLET_SIZE, D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_CIRCLE, m_nColor, CBullet::ATTACK_NORMAL);
					//弾の生成
					CBullet::Create(m_Pos, BULLET_SIZE, BULLET_SIZE, D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_CIRCLE, m_nColor, CBullet::ATTACK_NORMAL);
				}
			}
			if (m_nCountBullet > 500)
			{
				if (m_nCountBullet % 10 == 0)
				{
					for (int nCnt = 0; nCnt < 5; nCnt++)
					{
						//弾の生成
						CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(m_nCountBullet  + 210.0f + (nCnt * 5)))*5.5f, sinf(D3DXToRadian(m_nCountBullet  + 210.0f + (nCnt * 10)))*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, m_nColor, CBullet::ATTACK_NORMAL);
					}
					for (int nCnt = 0; nCnt < 5; nCnt++)
					{
						//弾の生成
						CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian(m_nCountBullet  + 150.0f - (nCnt * 5)))*5.5f, sinf(D3DXToRadian(m_nCountBullet  + 150.0f - (nCnt * 10)))*5.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, m_nColor, CBullet::ATTACK_NORMAL);
					}
				}
			}

			break;
	default:
		break;
	}

	m_nCountBullet++;

	switch (m_nType)
	{
	case TYPE_001:
		//テクスチャ座標のセット
		SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
		break;
	case TYPE_002:
		//テクスチャ座標のセット
		SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
		break;
	default:
		break;
	}

	//switch (m_nType)
	//{
	//case TYPE_001:
	//	//テクスチャアニメーション更新
	//	if (m_nCountAnim >= 8)
	//	{
	//		m_nCountAnim = 0;
	//		m_nPatternAnim++;
	//		if (m_nPatternAnim > 5)
	//		{
	//			m_nPatternAnim = 1;
	//		}
	//	}
	//	m_nCountAnim++;
	//	//テクスチャ座標のセット
	//	SetTex(
	//		m_nPatternAnim * 0.2f,
	//		0.0f,
	//		m_nPatternAnim * 0.2f + 0.2f,
	//		1.0f);
	//	break;
	//case TYPE_002:
	//	//テクスチャ座標のセット
	//	SetTex(
	//		0.0f,
	//		0.0f,
	//		1.0f,
	//		1.0f);
	//	break;
	//default:
	//	break;
	//}

	CScene2D::SetPosition(m_Pos);

	if (m_Pos.y < 0.0f ||
		m_Pos.y > FIELD_HEIGHT ||
		m_Pos.x < 0.0f ||
		m_Pos.x > FIELD_WIDTH)
	{
		Uninit();
		return;
	}

	if (CManager::GetIsTalking() == true)
	{
		SubLife(9999);
	}

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
void CEnemy::Draw(void)
{

		CScene2D::Draw();
	//ヒット中は白くする
	if (m_bHit == true)
	{
		CRenderer * pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		CScene2D::Draw();
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}