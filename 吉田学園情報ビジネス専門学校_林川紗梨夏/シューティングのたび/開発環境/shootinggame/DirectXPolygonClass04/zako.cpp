//=============================================================================
//
// zako敵の処理 [zako.cpp]
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
#include "zako.h"
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "explosion.h"
#include <time.h>
#include "sound.h"
#include "score.h"
#include "life.h"
#include "item.h"
#include "effect.h"
#include "player.h"

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CZako::m_pTexture[TYPE_MAX] = {};   //テクスチャのポインタ
														//=============================================================================
														// コンストラクタ
														//=============================================================================
CZako::CZako(int nPriority) : CScene2D(nPriority)
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

	m_TargetDistance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//目標までの距離
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//目標地点
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量

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
CZako::~CZako()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CZako * CZako::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeWidth, float fSizeHeight, TYPE Type, D3DXVECTOR3 location[MAX_LOCATION])
{
	CZako *pEnemy;
	pEnemy = new CZako;
	pEnemy->Init(pos, move, fSizeWidth, fSizeHeight, Type, location);
	return pEnemy;
}
//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CZako::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ENEMY_ZAKO_002, &m_pTexture[TYPE_001]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ENEMY_ZAKO_001, &m_pTexture[TYPE_002]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ENEMY_ZAKO_002, &m_pTexture[TYPE_003]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ENEMY_ZAKO_002, &m_pTexture[TYPE_004]);
	return S_OK;
}
//=============================================================================
// テクスチャアンロード
//=============================================================================
void CZako::Unload(void)
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

void CZako::SubLife(int SubLife)
{
	//スコア
	CScore * pScore = CGame::GetScore();
	//サウンド
	CSound * pSound = CManager::GetSound();
	m_nLife -= SubLife;

	if (m_nLife <= 0)
	{
		pScore->AddScore(1);
		pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);
		CItem::Create(m_Pos, ITEM_SIZE, ITEM_SIZE, CItem::TYPE_SCORE);
		CExplosion::Create(m_Pos, EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_NORMAL);

		for (int nCount = 0; nCount < 5; nCount++)
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
			CItem::Create(D3DXVECTOR3(m_Pos.x + RandPosX, m_Pos.y + RandPosY, 0.0f), ITEM_SIZE / 2, ITEM_SIZE / 2, CItem::TYPE_POWER);
		}

		Uninit();
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CZako::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeWidth, float fSizeHeight, TYPE Type, D3DXVECTOR3 location[MAX_LOCATION])
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

	m_Pos = pos;
	// 対角線、角度の設定
	m_fLength = sqrtf((float)(((fSizeWidth / 2) * (fSizeWidth / 2)) + ((fSizeHeight / 2) * (fSizeHeight / 2))));
	m_fAngle = atan2f((fSizeHeight / 2), (fSizeWidth / 2));
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;
	m_fAddScale = 0.01f;

	//ランダムシード値
	int nRandSeed = rand() % 255;
	srand((unsigned int)time(NULL)*nRandSeed);

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
void CZako::Uninit(void)
{

	////位置を取得
	//D3DXVECTOR3 pos = CScene2D::GetPosition();
	/*CExplosion::Create(pos, TEXTURE_WIDTH, TEXTURE_HEIGHT);*/
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CZako::Update(void)
{
	CScene2D::Update();
	//サウンド
	CSound * pSound = CManager::GetSound();
	//ライフ
	CLife * pLife = CGame::GetLife();

	m_nConutTime++;

	//位置を取得
	m_Pos = CScene2D::GetPosition();
	// 回転
	m_rot.z += D3DX_PI * 0.01f;
	if (m_rot.z >= D3DX_PI)//一定の値を超えると回らなくなるので減算していく
	{
		m_rot.z -= D3DX_PI * 2.0f;
	}

	//エフェクト生成
	CEffect::Create(m_Pos, ZAKO_SIZE_001_HEIGHT, ZAKO_SIZE_001_HEIGHT, m_Move, CEffect::TYPE_003, CEffect::COLOR_RED,10);

	/*if (m_nConutTime == 200)
	{
		m_Move.y = m_Move.y*-1.0f;
	}*/
	////位置更新
	//m_Pos += m_Move;

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

	for (int nCnt = 0; nCnt < GetNumAll(); nCnt++)
	{
		CScene * pScene = GetScene(4, nCnt);
		if (pScene != NULL)
		{
			OBJTYPE objType;
			objType = pScene->GetObjType();
			if (objType == OBJTYPE_PLAYER)
			{
				D3DXVECTOR3 playerPos = pScene->GetPosition();

				float posX = m_Pos.x - playerPos.x;
				float posY = m_Pos.y - playerPos.y;
				float Rd = float(sqrt(posX * posX + posY * posY)); //ポリゴン１の位置からポリゴン２までの距離

				if (Rd <= (BULLET_SIZE) / 2 + 15.0f)//ポリゴン１とポリゴン２の半径の合計より位置が小さいと衝突
				{
					CPlayer * pPlayer = dynamic_cast<CPlayer*> (pScene);
					if (pPlayer->IsInvincible() == false)
					{
						CExplosion::Create(playerPos, EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_NORMAL);
						pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);
						pPlayer->Respawn();
						pPlayer->SubLife(1);
					}
				}
			}
		}
	}

	//ショット
	switch (m_nType)
	{
		////３６０度円形射撃
	case TYPE_001:
		break;
		////ぐるぐる射撃
	case TYPE_002: //何もしない（ザコ）
		
		break;
	case TYPE_003://ただ出す
		if (m_nCountBullet % 50 == 0)
		{
			CPlayer * pPlayer = CGame::GetPlayer();
			m_TargetPos = pPlayer->GetPosition();

			////目標までの距離を算出//今回は生成時のプレイヤーに向けての方向指定のみ
			m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);
			m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
			int nRand = rand() % 50;
			m_rot.y += D3DXToRadian(nRand);
			//弾の生成
			CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y)*4.5f, sinf(m_rot.y)*4.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_NORMAL);
			//弾の生成
			CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y + 0.1f)*4.5f, sinf(m_rot.y + 0.1f)*4.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_NORMAL);
			//弾の生成
			CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y - 0.1f)*4.5f, sinf(m_rot.y - 0.1f)*4.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_NORMAL);
		}
		break;
	case TYPE_004://ポンデリング弾
		if (m_nCountBullet % 105 == 0)
		{
			CPlayer * pPlayer = CGame::GetPlayer();
			m_TargetPos = pPlayer->GetPosition();

			////目標までの距離を算出//今回は生成時のプレイヤーに向けての方向指定のみ
			m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);
			m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
			int nRand = rand() % 50;
			m_rot.y += D3DXToRadian(nRand);
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				//弾の生成
				CBullet::Create(D3DXVECTOR3(m_Pos.x + 30.0f * cosf(D3DXToRadian(nCnt * (360.0f / 5))), m_Pos.y + 30.0f * sinf(D3DXToRadian(nCnt * (360.0f / 5))), 0.0f), BULLET_SIZE, BULLET_SIZE, D3DXVECTOR3(cosf(m_rot.y)*3.5f, sinf(m_rot.y)*3.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_CIRCLE, CEffect::COLOR_PINK, CBullet::ATTACK_NORMAL);
			}
		}
		if (m_nCountBullet % 202 == 0)
		{
			CPlayer * pPlayer = CGame::GetPlayer();
			m_TargetPos = pPlayer->GetPosition();

			////目標までの距離を算出//今回は生成時のプレイヤーに向けての方向指定のみ
			m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);
			m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
			int nRand = rand() % 50;
			m_rot.y += D3DXToRadian(nRand);
			for (int nCnt = 0; nCnt < 10; nCnt++)
			{
				//弾の生成
				CBullet::Create(D3DXVECTOR3(m_Pos.x + 30.0f * cosf(D3DXToRadian(nCnt * (360.0f / 10))), m_Pos.y + 30.0f * sinf(D3DXToRadian(nCnt * (360.0f / 10))), 0.0f), BULLET_SIZE, BULLET_SIZE, D3DXVECTOR3(cosf(m_rot.y)*3.5f, sinf(m_rot.y)*3.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_CIRCLE, CEffect::COLOR_ORANGE, CBullet::ATTACK_NORMAL);
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
}
//=============================================================================
// 描画処理
//=============================================================================
void CZako::Draw(void)
{
	CScene2D::Draw();
}