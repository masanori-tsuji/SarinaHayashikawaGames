//=============================================================================
//
// アイテムの処理 [item.cpp]
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
#include "item.h"
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "explosion.h"
#include <time.h>
#include "sound.h"
#include "score.h"
#include "life.h"
#include "effect.h"

#define ITEM_GET_RANGE 4

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CItem::m_pTexture[TYPE_MAX] = {};   //テクスチャのポインタ

//=============================================================================
// コンストラクタ
//=============================================================================
CItem::CItem(int nPriority) : CScene2D(nPriority)
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_fLength = 0.0f;		// 対角線の長さ
	m_fAngle = 0.0f;		// 対角線の角度
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 向き
	m_fScale = 0.0f;		// 拡大
	m_fAddScale = 0.0f;	// 拡大加算値

	m_TargetDistance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//目標までの距離
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//目標地点
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量

	m_RandColorR = 255;
	m_RandColorG = 255;
	m_RandColorB = 255;
	m_fSize = 0.0f;
	m_nPatternAnim = 1;
	m_nCountAnim = 0;
	m_nType = TYPE_NONE;
	m_nLife = ITEM_LIFE;

}

//=============================================================================
// デストラクタ
//=============================================================================
CItem::~CItem()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CItem * CItem::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, TYPE Type)
{
	CItem *pEnemy;
	pEnemy = new CItem;
	pEnemy->Init(pos, fSizeWidth, fSizeHeight, Type);
	return pEnemy;
}
//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CItem::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ITEM_001, &m_pTexture[TYPE_POWER]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ITEM_002, &m_pTexture[TYPE_LIFE]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ITEM_003, &m_pTexture[TYPE_SCORE]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ITEM_004, &m_pTexture[TYPE_BOMB]);
	return S_OK;
}
//=============================================================================
// テクスチャアンロード
//=============================================================================
void CItem::Unload(void)
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

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, TYPE Type)
{
	CScene2D::Init(pos, fSizeWidth, fSizeHeight);

	SetObjType(CScene::OBJTYPE_ITEM);
	// ポリゴンの位置を設定
	CScene2D::SetPosition(pos);

	m_nType = Type;

	// 対角線、角度の設定
	m_fLength = sqrtf((float)(((fSizeWidth / 2) * (fSizeWidth / 2)) + ((fSizeHeight / 2) * (fSizeHeight / 2))));
	m_fAngle = atan2f((fSizeHeight / 2), (fSizeWidth / 2));
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;
	m_fAddScale = 0.01f;
	m_fSize = fSizeWidth; //さいずのほぞん
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
	case TYPE_POWER:
		//テクスチャ座標のセット
		SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
		break;
	case TYPE_LIFE:
		//テクスチャ座標のセット
		SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
		break;
	case TYPE_SCORE:
		//テクスチャ座標のセット
		SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
		break;
	case TYPE_BOMB:
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
	int nRandSeed = rand() % 360 + -360;
	srand((unsigned int)nRandSeed);
	int nRand = rand() % 360 + -360;
	m_rot.y = D3DXToRadian(nRand);
	m_Move = D3DXVECTOR3(cosf(m_rot.y)*5.0f, sinf(m_rot.y)*5.0f, 0.0f);
	//テクスチャのバインド
	BindTexture(m_pTexture[m_nType]);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CItem::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CItem::Update(void)
{
	CScene2D::Update();
	//サウンド
	CSound * pSound = CManager::GetSound();
	//ライフ
	CLife * pLife = CGame::GetLife();
	//スコア
	CScore * pScore = CGame::GetScore();

	//位置を取得
	D3DXVECTOR3 pos = CScene2D::GetPosition();

	switch (m_nType)
	{
		
	case TYPE_POWER:
	
			for (int nCnt = 0; nCnt < GetNumAll(); nCnt++)
			{
				CScene * pScene = GetScene(4,nCnt);
				if (pScene != NULL)
				{
					OBJTYPE objType;
					objType = pScene->GetObjType();
					if (objType == OBJTYPE_PLAYER)
					{
						D3DXVECTOR3 PlayerPos = pScene->GetPosition();

						//近くになると吸い付く
						if (pos.x - BULLET_SIZE / 2 >= PlayerPos.x - TEXTURE_WIDTH * ITEM_GET_RANGE / 2 &&
							pos.x + BULLET_SIZE / 2 < PlayerPos.x + TEXTURE_WIDTH * ITEM_GET_RANGE / 2 &&
							pos.y - BULLET_SIZE / 2 >= PlayerPos.y - TEXTURE_HEIGHT * ITEM_GET_RANGE / 2 &&
							pos.y + BULLET_SIZE / 2 < PlayerPos.y + TEXTURE_HEIGHT * ITEM_GET_RANGE / 2)
						{
							m_TargetDistance = D3DXVECTOR3(PlayerPos.x - pos.x, PlayerPos.y - pos.y, 0.0f);//目標までの距離を算出

							m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
							m_Move = D3DXVECTOR3(cosf(m_rot.y)*7.5f, sinf(m_rot.y)*7.5f, 0.0f);
						}
						else
						{
							//m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//範囲外の時停止
						}

						if (pos.x - BULLET_SIZE / 2 >= PlayerPos.x - TEXTURE_WIDTH / 2 &&
							pos.x + BULLET_SIZE / 2 < PlayerPos.x + TEXTURE_WIDTH / 2 &&
							pos.y - BULLET_SIZE / 2 >= PlayerPos.y - TEXTURE_HEIGHT / 2 &&
							pos.y + BULLET_SIZE / 2 < PlayerPos.y + TEXTURE_HEIGHT / 2)
						{
							if (m_fSize < ITEM_SIZE)
							{
								CPlayer * pPlayer = dynamic_cast<CPlayer*> (pScene);
								pPlayer->AddPower(2);//プレイヤーの残機追加
							}
							else
							{
								CPlayer * pPlayer = dynamic_cast<CPlayer*> (pScene);
								pPlayer->AddPower(10);//プレイヤーの残機追加
							}
							pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_RECOVERY);
							Uninit();
							return;
							break;
						}
					}
				}
			}

			CEffect::Create(pos, ITEM_SIZE, ITEM_SIZE, m_Move, CEffect::TYPE_004, CEffect::COLOR_RED,20);
		break;
	case TYPE_LIFE:
		for (int nCnt = 0; nCnt < GetNumAll(); nCnt++)
		{
			CScene * pScene = GetScene(4, nCnt);
			if (pScene != NULL)
			{
				OBJTYPE objType;
				objType = pScene->GetObjType();
				if (objType == OBJTYPE_PLAYER)
				{
					D3DXVECTOR3 PlayerPos = pScene->GetPosition();

					//近くになると吸い付く
					if (pos.x - BULLET_SIZE / 2 >= PlayerPos.x - TEXTURE_WIDTH * ITEM_GET_RANGE / 2 &&
						pos.x + BULLET_SIZE / 2 < PlayerPos.x + TEXTURE_WIDTH * ITEM_GET_RANGE / 2 &&
						pos.y - BULLET_SIZE / 2 >= PlayerPos.y - TEXTURE_HEIGHT * ITEM_GET_RANGE / 2 &&
						pos.y + BULLET_SIZE / 2 < PlayerPos.y + TEXTURE_HEIGHT * ITEM_GET_RANGE / 2)
					{
						m_TargetDistance = D3DXVECTOR3(PlayerPos.x - pos.x, PlayerPos.y - pos.y, 0.0f);//目標までの距離を算出

						m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
						m_Move = D3DXVECTOR3(cosf(m_rot.y)*7.5f, sinf(m_rot.y)*7.5f, 0.0f);
					}
					else
					{
						//m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//範囲外の時停止
					}

					if (pos.x - BULLET_SIZE / 2 >= PlayerPos.x - TEXTURE_WIDTH / 2 &&
						pos.x + BULLET_SIZE / 2 < PlayerPos.x + TEXTURE_WIDTH / 2 &&
						pos.y - BULLET_SIZE / 2 >= PlayerPos.y - TEXTURE_HEIGHT / 2 &&
						pos.y + BULLET_SIZE / 2 < PlayerPos.y + TEXTURE_HEIGHT / 2)
					{
						CPlayer * pPlayer = dynamic_cast<CPlayer*> (pScene);
						pPlayer->AddLife(1);//プレイヤーの残機追加
						pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_RECOVERY);
						Uninit();
						return;
						break;
					}
				}
			}
		}
		//エフェクト生成
		CEffect::Create(pos, ITEM_SIZE * 0.2f, ITEM_SIZE * 0.2f, m_Move, CEffect::TYPE_001, CEffect::COLOR_YELLOW, 10);
		//エフェクト生成
		CEffect::Create(pos, ITEM_SIZE, ITEM_SIZE, m_Move, CEffect::TYPE_004, CEffect::COLOR_PINK,40);
		break;
	case TYPE_SCORE:
		
			for (int nCnt = 0; nCnt < GetNumAll(); nCnt++)
			{
				CScene * pScene = GetScene(4, nCnt);
				if (pScene != NULL)
				{
					OBJTYPE objType;
					objType = pScene->GetObjType();
					if (objType == OBJTYPE_PLAYER)
					{
						D3DXVECTOR3 PlayerPos = pScene->GetPosition();
	
							m_TargetDistance = D3DXVECTOR3(PlayerPos.x - pos.x, PlayerPos.y - pos.y, 0.0f);//目標までの距離を算出

							m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
							m_Move = D3DXVECTOR3(cosf(m_rot.y)*10.5f, sinf(m_rot.y)*10.5f, 0.0f);

						if (pos.x - BULLET_SIZE / 2 >= PlayerPos.x - TEXTURE_WIDTH / 2 &&
							pos.x + BULLET_SIZE / 2 < PlayerPos.x + TEXTURE_WIDTH / 2 &&
							pos.y - BULLET_SIZE / 2 >= PlayerPos.y - TEXTURE_HEIGHT / 2 &&
							pos.y + BULLET_SIZE / 2 < PlayerPos.y + TEXTURE_HEIGHT / 2)
						{
							pScore->AddScore(100);//スコア追加
							Uninit();
							return;
							break;
						}
					}
				}
			}
		
		break;
	case TYPE_BOMB:
		for (int nCnt = 0; nCnt < GetNumAll(); nCnt++)
		{
			CScene * pScene = GetScene(4, nCnt);
			if (pScene != NULL)
			{
				OBJTYPE objType;
				objType = pScene->GetObjType();
				if (objType == OBJTYPE_PLAYER)
				{
					D3DXVECTOR3 PlayerPos = pScene->GetPosition();

					//近くになると吸い付く
					if (pos.x - BULLET_SIZE / 2 >= PlayerPos.x - TEXTURE_WIDTH * 7 / 2 &&
						pos.x + BULLET_SIZE / 2 < PlayerPos.x + TEXTURE_WIDTH * 7 / 2 &&
						pos.y - BULLET_SIZE / 2 >= PlayerPos.y - TEXTURE_HEIGHT * 7 / 2 &&
						pos.y + BULLET_SIZE / 2 < PlayerPos.y + TEXTURE_HEIGHT * 7 / 2)
					{
						m_TargetDistance = D3DXVECTOR3(PlayerPos.x - pos.x, PlayerPos.y - pos.y, 0.0f);//目標までの距離を算出

						m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
						m_Move = D3DXVECTOR3(cosf(m_rot.y)*7.5f, sinf(m_rot.y)*7.5f, 0.0f);
					}
					else
					{
						//m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//範囲外の時停止
					}

					if (pos.x - BULLET_SIZE / 2 >= PlayerPos.x - TEXTURE_WIDTH / 2 &&
						pos.x + BULLET_SIZE / 2 < PlayerPos.x + TEXTURE_WIDTH / 2 &&
						pos.y - BULLET_SIZE / 2 >= PlayerPos.y - TEXTURE_HEIGHT / 2 &&
						pos.y + BULLET_SIZE / 2 < PlayerPos.y + TEXTURE_HEIGHT / 2)
					{
						CPlayer * pPlayer = dynamic_cast<CPlayer*> (pScene);
						pPlayer->AddBomb(1);//プレイヤーの残機追加
						pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_RECOVERY);
						Uninit();
						return;
						break;
					}
				}
			}

		}
		//エフェクト生成
		CEffect::Create(pos, ITEM_SIZE * 0.3f, ITEM_SIZE * 0.3f, m_Move, CEffect::TYPE_001, CEffect::COLOR_YELLOW, 10);
		//エフェクト生成
		CEffect::Create(pos, ITEM_SIZE, ITEM_SIZE, m_Move, CEffect::TYPE_004, CEffect::COLOR_BLUE,40);
	default:
		break;
	}

	//位置更新
	pos += m_Move;

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

	m_nLife--;
	if (m_nLife <= 100 && m_nLife % 10 == 0)
	{
		//色のセット
		SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
	}
	else
	{
		//色のセット
		SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	if (pos.x < 0)
	{
		m_Move.x = m_Move.x * -1.0f;

	}
	if (pos.x > FIELD_WIDTH)
	{
		m_Move.x = m_Move.x * -1.0f;
		
	}
	if (pos.y < 0)
	{
		m_Move.y = m_Move.y * -1.0f;

	}
	if (pos.y > FIELD_HEIGHT)
	{
		m_Move.y = m_Move.y * -1.0f;
	
	}

	if (m_nLife <= 0)
	{
		Uninit();
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

	CScene2D::SetPosition(pos);
}
//=============================================================================
// 描画処理
//=============================================================================
void CItem::Draw(void)
{
	CScene2D::Draw();
}