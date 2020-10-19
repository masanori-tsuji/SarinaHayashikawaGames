//=============================================================================
//
// プレイヤーの処理 [player.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "explosion.h"
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "bomb.h"
#include "item.h"
#include <time.h>
#include "enemy.h"
#include "boss.h"
#include "zako.h"

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;   //テクスチャのポインタ

//=============================================================================
// コンストラクタ
//=============================================================================
CExplosion::CExplosion(int nPriority): CScene2D(nPriority)
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_RandColorR = 255;
	m_RandColorG = 255;
	m_RandColorB = 255;
	m_fLength = 0.0f;		// 対角線の長さ
	m_fAngle = 0.0f;		// 対角線の角度
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 向き
	m_fScale = 0.0f;		// 拡大
		
	m_nPatternAnim = 1;
	m_nCountAnim = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CExplosion::~CExplosion()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CExplosion * CExplosion::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, TYPE type)
{
	CExplosion *pExplosion;
	
	pExplosion = new CExplosion;
	pExplosion->Init(pos, fSizeWidth, fSizeHeight,type);

	return pExplosion;
}
//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CExplosion::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EXPLOSION, &m_pTexture);
	return S_OK;
}
//=============================================================================
// テクスチャアンロード
//=============================================================================
void CExplosion::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, TYPE type)
{
	CScene2D::Init(pos, fSizeWidth, fSizeHeight);

	SetObjType(CScene::OBJTYPE_NONE);
	//位置のセット
	SetPosition(pos);

	// 対角線、角度の設定
	m_fLength = sqrtf((float)(((fSizeWidth / 2) * (fSizeWidth / 2)) + ((fSizeHeight / 2) * (fSizeHeight / 2))));
	m_fAngle = atan2f((fSizeHeight / 2), (fSizeWidth / 2));
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;
	m_fAddScale = 0.01f;

	m_Type = type;

	switch (m_Type)
	{
	case TYPE_NORMAL:
		m_nPatternAnim = 0;
		break;
	case TYPE_RESPAWN:
		m_nPatternAnim = 7;
		break;
	case TYPE_BOMB:
		m_nPatternAnim = 6;
	default:
		break;
	}

	//メンバ変数にサイズ引数を代入
	m_fSizeWidth = fSizeWidth;
	m_fSizeHeight = fSizeHeight;

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

	//頂点座標のセット
	SetVtxPosition(m_vPos);

	int nRandSeed = 0;//ランダムシード値（時間毎変更ではなくクリック毎変更にするため）
	nRandSeed = rand() % 255;
	srand((unsigned int)time(NULL)*nRandSeed);

	//ランダムな色の数値取得
	m_RandColorR = rand() % 255;
	m_RandColorG = rand() % 255;
	m_RandColorB = rand() % 255;

	//色のセット
	SetColor(D3DCOLOR_RGBA(m_RandColorR, m_RandColorG, m_RandColorB, 255));

	//テクスチャ座標のセット
	SetTex(
		m_nPatternAnim * 0.125f,
		0.0f,
		m_nPatternAnim * 0.125f + 0.125f,
		1.0f);

    //テクスチャの割り当て
	BindTexture(m_pTexture);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CExplosion::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CExplosion::Update(void)
{
	CScene2D::Update();

	D3DXVECTOR3 pos = CScene2D::GetPosition();

	SetPosition(pos);

	//色のセット
	SetColor(D3DCOLOR_RGBA(m_RandColorR, m_RandColorG, m_RandColorB, 255));

	switch (m_Type)
	{
	case TYPE_NORMAL:
		if (m_nCountAnim >= 5)
		{
			m_nCountAnim = 0;
			m_nPatternAnim++;
		}

		if (m_nPatternAnim > 7)
		{
			Uninit();
			return;
		}

		break;
	case TYPE_RESPAWN:
		if (m_nCountAnim >= 5)//フレームを逆に再生する
		{
			m_nCountAnim = 0;
			m_nPatternAnim--;
		}
		if (m_nPatternAnim <= 0)
		{
			Uninit();
			return;
		}
		break;
	case TYPE_BOMB:
	
			m_nPatternAnim = 6;
			m_fScale += 0.1f;
			m_fSizeWidth += 20.0f;
			m_fSizeHeight += 20.0f;
			
				for (int nCnt = 0; nCnt < GetNumAll(); nCnt++)
				{
					CScene * pScene = GetScene(4, nCnt);
					if (pScene != NULL)
					{
						OBJTYPE objType;
						objType = pScene->GetObjType();
						if (objType == OBJTYPE_BULLET_ENEMY)
						{
							D3DXVECTOR3 BulletPos = pScene->GetPosition();
							//if (pos.x - (m_fSizeWidth / 2) <= BulletPos.x - (30.0f / 2) &&
							//	pos.x + (m_fSizeWidth / 2) > BulletPos.x + (30.0f / 2) &&
							//	pos.y - (m_fSizeHeight / 2) <= BulletPos.y - (30.0f / 2) &&
							//	pos.y + (m_fSizeHeight / 2) > BulletPos.y + (30.0f / 2))
							//{
							//	pScene->Uninit();
							//	CExplosion::Create(BulletPos, EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_NORMAL);
							//	CItem::Create(BulletPos, 20, 20, CItem::TYPE_SCORE);
							//	/*break;*/
							//}

							float posX = pos.x - BulletPos.x;
							float posY = pos.y - BulletPos.y;
							float Rd = float(sqrt(posX * posX + posY * posY)); //ポリゴン１の位置からポリゴン２までの距離

							if (Rd <= m_fSizeWidth / 2 + 15.0f)//ポリゴン１とポリゴン２の半径の合計より位置が小さいと衝突
							{
									pScene->Uninit();
									CExplosion::Create(BulletPos, EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_NORMAL);
									CItem::Create(BulletPos, 20, 20, CItem::TYPE_SCORE);
							}
						}
					}
				}
			
			if (m_nCountAnim >= 100)
			{
				Uninit();
				return;
			}
	
		break;

	default:
		break;
	}

	m_nCountAnim++;

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

	//頂点座標のセット
	SetVtxPosition(m_vPos);

	//テクスチャ座標のセット
	SetTex(
		m_nPatternAnim * 0.125f,
		0.0f,
		m_nPatternAnim * 0.125f + 0.125f,
		1.0f);
	

}
//=============================================================================
// 描画処理
//=============================================================================
void CExplosion::Draw(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	CScene2D::Draw();
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}