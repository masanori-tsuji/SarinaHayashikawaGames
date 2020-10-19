//=============================================================================
//
// 弾の処理 [bullet.cpp]
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
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "renderer.h"
#include "manager.h"
#include <time.h>
#include "sound.h"
#include "score.h"
#include "life.h"
#include "graze.h"
#include "zako.h"
#include "boss.h"

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBullet::m_pTexture[BULLET_MAX] = {};
bool CBullet::m_bSpread = false; 

D3DCOLOR CBullet::m_aColor[COLOR_MAX] = //色パラメータ
{
	D3DCOLOR_RGBA(255,0,0,255),	//赤
	D3DCOLOR_RGBA(255,128,0,255),// オレンジ
	D3DCOLOR_RGBA(255,255,0,255),// 黄色
	D3DCOLOR_RGBA(0,255,0,255),//緑
	D3DCOLOR_RGBA(0,0,255,255),//青
	D3DCOLOR_RGBA(128,0,255,255),//紫
	D3DCOLOR_RGBA(255,0,255,255),//ピンク
};

//=============================================================================
// コンストラクタ
//=============================================================================
CBullet::CBullet(int nPriority) : CScene2D(nPriority)
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	
	m_move= D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 向き
	m_CPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 向き
	m_fLength = 0.0f;		// 対角線の長さ
	m_fAngle = 0.0f;		// 対角線の角度
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 向き
	m_omega = 0.2f;
	m_fScale = 0.0f;		// 拡大
	m_fAddScale = 0.0f;	// 拡大加算値
	m_Color = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_fRdScale = 1.0f;
	m_RandColorR = 255;
	m_RandColorG = 255;
	m_RandColorB = 255;
	m_nCount = 0;
	m_nAttack = ATTACK_NORMAL;
	m_nColNum = 0;
	m_nEffectCnt = 0;
	m_nColorNum = 0;
	m_nBulletNum = 0;
	m_nPatternAnim = 1;
	m_nCountAnim = 0;
	m_nType = TYPE_NONE;

	m_nLife = BULLET_LIFE;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CBullet * CBullet::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, D3DXVECTOR3 move, BULLET type, BULLETTYPE bullettype, int col, ATTACKTYPE attacktype)
{
	CBullet *pBullet;

	pBullet = new CBullet;
	pBullet->Init(pos, fSizeWidth, fSizeHeight,move,type, bullettype,col, attacktype);

	return pBullet;
}

HRESULT CBullet::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET_001, &m_pTexture[BULLET_DIAMOND]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET_002, &m_pTexture[BULLET_CIRCLE]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET_003, &m_pTexture[BULLET_DOG]);
	return S_OK;
}

void CBullet::Unload(void)
{
	for (int nCount = 0; nCount < BULLET_MAX; nCount++)
	{
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
	
}

void CBullet::SpreadBullet(int nRandSeed)
{
	if (m_nAttack == ATTACK_SPREAD)
	{
		int randSeed = rand() % 100 * nRandSeed;

		srand((unsigned int)randSeed);
		m_rot.y = D3DXToRadian(rand() % 360);
		m_move = D3DXVECTOR3(cosf(m_rot.y)*1.5f, sinf(m_rot.y)*1.5f, 0.0f);
		BindTexture(m_pTexture[BULLET_CIRCLE]);
		int nRandCol = rand() % 3;
		//テクスチャ座標のセット
		m_nColorNum = nRandCol;
		m_fScale = float(rand() % 15 + 5) / 10;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, D3DXVECTOR3 move, BULLET type, BULLETTYPE bullettype, int col, ATTACKTYPE attacktype)
{
	CScene2D::Init(pos, fSizeWidth, fSizeHeight);
	if (type == TYPE_PLAYER)
	{
		SetObjType(CScene::OBJTYPE_BULLET_PLAYER);
	}
	else
	{
		SetObjType(CScene::OBJTYPE_BULLET_ENEMY);
	}
	
	m_CPos = pos;

	// 対角線、角度の設定
	m_fLength = sqrtf((float)(((fSizeWidth / 2) * (fSizeWidth / 2)) + ((fSizeHeight / 2) * (fSizeHeight / 2))));
	m_fAngle = atan2f((fSizeHeight / 2), (fSizeWidth / 2));
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;
	m_fAddScale = 0.01f;
	
	m_nBulletNum = bullettype;
	m_nColorNum = col;
	m_Color = m_aColor[col];
	m_fSizeWidth = fSizeWidth;
	m_fSizeHeight = fSizeHeight;
	m_nAttack = attacktype;
	m_nType = type;
	m_move = move;
	//位置をセット
	SetPosition(pos);

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

	if (m_nBulletNum == BULLET_CIRCLE || m_nBulletNum == BULLET_DIAMOND)
	{
		//テクスチャ座標のセット
		SetTex(
			col * 0.14285f,
			0.0f,
			col * 0.14285f + 0.14285f,
			1.0f);
	}
	else
	{
		//テクスチャ座標のセット
		SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
	}

	m_pVtxBuff->Unlock();

	BindTexture(m_pTexture[bullettype]);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update(void)
{
	//ライフ
	CLife * pLife = CGame::GetLife();
	
	CScene2D::Update();

	CSound * pSound = CManager::GetSound();
	//位置を取得
	D3DXVECTOR3 pos = CScene2D::GetPosition();

	//グレイズ
	CGraze * pGraze = CGame::GetGraze();

	//進行方向に向きを合わせる
	m_rot.z = atan2f((pos.x + m_move.x) - pos.x, (pos.y + m_move.y) - pos.y);
	

	switch (m_nAttack)
	{
	case ATTACK_NORMAL:

		break;
	case ATTACK_AIM:

		if (m_nCount == 100)
		{
			CPlayer * pPlayer = CGame::GetPlayer();
			D3DXVECTOR3 m_TargetPos = pPlayer->GetPosition();

			////目標までの距離を算出//今回は生成時のプレイヤーに向けての方向指定のみ
			D3DXVECTOR3 m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - pos.x, m_TargetPos.y - pos.y, 0.0f);
			m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
			m_move = D3DXVECTOR3(cosf(m_rot.y)*5.5f, sinf(m_rot.y)*5.5f, 0.0f);

		}
		m_nCount++;
		break;
	case ATTACK_SPREAD:

		break;
	case ATTACK_ROTATION:
		m_fRdScale += 0.1f;
		m_rot.y += m_omega;
		pos.x = m_CPos.x + 50.0f * m_fRdScale*cosf(m_rot.y);
		pos.y = m_CPos.y + 50.0f * m_fRdScale*sinf(m_rot.y);
		
		m_move.x = -50.0f*m_omega*sinf(m_rot.y);
		m_move.y = 50.0f*m_omega*cosf(m_rot.y);
		break;
	case ATTACK_BOUND:
		//エフェクト生成
		CEffect::Create(pos, BULLET_SIZE * 0.8, BULLET_SIZE * 0.8, m_move, CEffect::TYPE_004, m_nColorNum,20);
		if (pos.x < 0)
		{
			m_move.x = m_move.x * -1.0f;
			m_nColorNum++;
		}
		if (pos.x > FIELD_WIDTH)
		{
			m_move.x = m_move.x * -1.0f;
			m_nColorNum++;
		}
		if (pos.y < 0)
		{
			m_move.y = m_move.y * -1.0f;
			m_nColorNum++;
		}
		if (pos.y > FIELD_HEIGHT)
		{
			m_move.y = m_move.y * -1.0f;
			m_nColorNum++;
		}

		if (m_nColorNum > 6)
		{
			Uninit();
			return;
		}
		break;
	case ATTACK_BOTOBOTO:
		if (m_nCount % 15 == 0)
		{
			int nRandSeed = rand() % 360 + -360;
			srand((unsigned int)nRandSeed);
			int nRand = rand() % 360 + -360;
			m_rot.y = D3DXToRadian(nRand);
			//弾の生成
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f), BULLET_SIZE * 0.5f, BULLET_SIZE * 0.5f, D3DXVECTOR3(cosf(m_rot.y)*1.5f, sinf(m_rot.y)*1.8f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_CIRCLE, CEffect::COLOR_BLUE, CBullet::ATTACK_NORMAL);
			//弾の生成
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f), BULLET_SIZE * 0.5f, BULLET_SIZE * 0.5f, D3DXVECTOR3(cosf(m_rot.y)*-1.5f, sinf(m_rot.y)*-1.8f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_CIRCLE, CEffect::COLOR_BLUE, CBullet::ATTACK_NORMAL);
		}
		if (m_nCount % 2 == 0)
		{
			//エフェクト生成
			CEffect::Create(pos, BULLET_SIZE*1.2, BULLET_SIZE*1.2, m_move, CEffect::TYPE_004, CEffect::COLOR_PURPLE,40);
			//エフェクト生成
			CEffect::Create(pos, BULLET_SIZE, BULLET_SIZE, m_move, CEffect::TYPE_004, CEffect::COLOR_PINK,15);
		}
		if (pos.x < 0)
		{
			m_move.x = m_move.x * -1.0f;
			m_nColorNum++;
		}
		if (pos.x > FIELD_WIDTH)
		{
			m_move.x = m_move.x * -1.0f;
			m_nColorNum++;
		}
		if (pos.y < 0)
		{
			m_move.y = m_move.y * -1.0f;
			m_nColorNum++;
		}
		if (pos.y > FIELD_HEIGHT)
		{
			m_move.y = m_move.y * -1.0f;
			m_nColorNum++;
		}

		if (m_nColorNum > 6)
		{
			Uninit();
			return;
		}
		m_nCount++;
		break;
	default:
		break;
	}

	//位置更新
	pos += m_move;





	//m_nEffectCnt++;
	//	if (m_nEffectCnt >= 2)
	//	{
	//		//エフェクト生成
	//		CEffect::Create(pos, BULLET_SIZE*1.2, BULLET_SIZE*1.2, m_move, CEffect::TYPE_003, m_nColorNum);
	//		m_nEffectCnt = 0;
	//	}
	

		////テクスチャ座標のセット
		//SetTex(
		//	m_nColorNum * 0.14285f,
		//	0.0f,
		//	m_nColorNum * 0.14285f + 0.14285f,
		//	1.0f);

	if (pos.y < -50.0f ||
		pos.y > FIELD_HEIGHT + 50.0f ||
		pos.x < -50.0f ||
		pos.x > FIELD_WIDTH + 50.0f)
	{
		Uninit();
		return;
	}
	else
	{

		switch (m_nType)
		{
		case TYPE_PLAYER:

				for (int nCnt = 0; nCnt < GetNumAll(); nCnt++)
				{
					CScene * pScene = GetScene(4,nCnt);
					if (pScene != NULL)
					{
						OBJTYPE objType;
						objType = pScene->GetObjType();
						if (objType == OBJTYPE_ENEMY)
						{
							D3DXVECTOR3 enemyPos = pScene->GetPosition();

							float posX = pos.x - enemyPos.x;
							float posY = pos.y - enemyPos.y;
							float Rd = float(sqrt(posX * posX + posY * posY)); //ポリゴン１の位置からポリゴン２までの距離

						
								
								//pScene(CScene)をCEnemyにダウンキャスト
								CEnemy * pEnemy = dynamic_cast<CEnemy*> (pScene);

								if (pEnemy != NULL)
								{
									if (Rd <= m_fSizeWidth / 2 + 15.0f)//ポリゴン１とポリゴン２の半径の合計より位置が小さいと衝突
									{
										pEnemy->SubLife(10);
										Uninit();
										return;
									}
									
								}

								CZako * pZako = dynamic_cast<CZako*> (pScene);
								if (pZako != NULL)
								{
									if (Rd <= m_fSizeWidth / 2 + 15.0f)//ポリゴン１とポリゴン２の半径の合計より位置が小さいと衝突
									{
										pZako->SubLife(10);
										Uninit();
										return;
									}
									
								}
								CBoss * pBoss = dynamic_cast<CBoss*> (pScene);
								if (pBoss != NULL)
								{
									if (Rd <= m_fSizeWidth / 2 + 50.0f)//ポリゴン１とポリゴン２の半径の合計より位置が小さいと衝突
									{
										pBoss->SubLife(10);
										Uninit();
										return;
									}
									
								}
							}
			
						}
					}
			break;
		case TYPE_ENEMY:

			/*for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
			{*/
				for (int nCnt = 0; nCnt < GetNumAll(); nCnt++)
				{
					CScene * pScene = GetScene(4,nCnt);
					if (pScene != NULL)
					{
						OBJTYPE objType;
						objType = pScene->GetObjType();
						if (objType == OBJTYPE_PLAYER)
						{
							D3DXVECTOR3 playerPos = pScene->GetPosition();


							float posX = pos.x - playerPos.x;
							float posY = pos.y - playerPos.y;
							float Rd = float(sqrt(posX * posX + posY * posY)); //ポリゴン１の位置からポリゴン２までの距離

							if (Rd <= (m_fSizeWidth + GRAZE_SIZE) / 2 + 15.0f)//ポリゴン１とポリゴン２の半径の合計より位置が小さいと衝突
							{
								pGraze->AddGraze(0.05f);
							}
							
							if (Rd <= m_fSizeWidth / 2 + 2.0f)//ポリゴン１とポリゴン２の半径の合計より位置が小さいと衝突
							{
								Uninit();
								CPlayer * pPlayer = dynamic_cast<CPlayer*> (pScene);
								if (pPlayer->IsInvincible() == false)
								{
									CExplosion::Create(pos, EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_NORMAL);
									pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);
									pPlayer->Respawn();
									pPlayer->SubLife(1);
								}
								return;
							}
							
						}
					}
				}
			/*}*/
			break;
		default:
			break;
		}


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

		if (m_nBulletNum == BULLET_CIRCLE || m_nBulletNum == BULLET_DIAMOND)
		{
			//テクスチャ座標のセット
			SetTex(
				m_nColorNum * 0.14285f,
				0.0f,
				m_nColorNum * 0.14285f + 0.14285f,
				1.0f);
		}
		else
		{
			//テクスチャ座標のセット
			SetTex(
				0.0f,
				0.0f,
				1.0f,
				1.0f);
		}

		//位置のセット
		CScene2D::SetPosition(pos);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBullet::Draw(void)
{
	CScene2D::Draw();
}