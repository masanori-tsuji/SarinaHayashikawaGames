//=============================================================================
//
// ボスゲージの処理 [bossgauge.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "game.h"
#include "bossgauge.h"
#include "boss.h"

LPDIRECT3DTEXTURE9 CBossgauge::m_apTexture[TYPE_MAX] = {};

CBossgauge::CBossgauge(int nPriority) : CScene(nPriority)
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	memset(m_apScene2D, 0, sizeof(m_apScene2D));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSizeWidth = 0.0f;
	m_fSizeHeight = 0.0f;
	m_nPatternAnim = 1;
	m_nCountAnim = 0;
	m_nLife = 0;
	m_nMaxLife = 0;
}
CBossgauge::~CBossgauge()
{

}

HRESULT CBossgauge::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAUGE, &m_apTexture[0]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, NULL, &m_apTexture[1]);

	return S_OK;
}

void CBossgauge::Unload(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

CBossgauge * CBossgauge::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, int nMaxlife)
{
	CBossgauge *pBossgauge;

	pBossgauge = new CBossgauge;
	pBossgauge->Init(pos, fSizeWidth, fSizeHeight);
	return pBossgauge;
}

HRESULT CBossgauge::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	SetObjType(CScene::OBJTYPE_NONE);
	CBoss * pBoss = CGame::GetBoss();
	m_nMaxLife = pBoss->GetLife();
	m_pos = pos;
	m_nLife = m_nMaxLife;

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apScene2D[nCnt] = new CScene2D(5);
		m_apScene2D[nCnt]->Init(pos, fSizeWidth, fSizeHeight);
		m_apScene2D[nCnt]->BindTexture(m_apTexture[nCnt]);

		//メンバ変数にサイズ引数を代入
		m_fSizeWidth = fSizeWidth;
		m_fSizeHeight = fSizeHeight;

		if (nCnt == TYPE_001)
		{
			// 頂点座標を設定
			m_vPos[0] = D3DXVECTOR3(m_pos.x, m_pos.y + (-m_fSizeHeight / 2), 0.0f);
			m_vPos[1] = D3DXVECTOR3(m_pos.x + fSizeWidth * (m_nLife / m_nMaxLife), m_pos.y + (-m_fSizeHeight / 2), 0.0f);
			m_vPos[2] = D3DXVECTOR3(m_pos.x, m_pos.y + (m_fSizeHeight / 2), 0.0f);
			m_vPos[3] = D3DXVECTOR3(m_pos.x + fSizeWidth * (m_nLife / m_nMaxLife), m_pos.y + (m_fSizeHeight / 2), 0.0f);

			//色のセット
			m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 150, 150, 255));
		}
		else if (nCnt == TYPE_002)
		{
			// 頂点座標を設定
			m_vPos[0] = D3DXVECTOR3(m_pos.x, m_pos.y + (-m_fSizeHeight / 2), 0.0f);
			m_vPos[1] = D3DXVECTOR3(m_pos.x, m_pos.y + (-m_fSizeHeight / 2), 0.0f);
			m_vPos[2] = D3DXVECTOR3(m_pos.x, m_pos.y + (m_fSizeHeight / 2), 0.0f);
			m_vPos[3] = D3DXVECTOR3(m_pos.x, m_pos.y + (m_fSizeHeight / 2), 0.0f);

			//色のセット
			m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(0, 255, 255, 100));
		}
		//頂点座標のセット
		m_apScene2D[nCnt]->SetVtxPosition(m_vPos);

	

		//テクスチャ座標のセット
		m_apScene2D[nCnt]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
	}

	return S_OK;
}

void CBossgauge::Uninit(void)
{
	/*for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
	if (m_apScene2D[nCnt] != NULL)
	{
	m_apScene2D[nCnt]->Uninit();
	m_apScene2D[nCnt] = NULL;
	}
	}*/
	Release();
}

void CBossgauge::Update(void)
{

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apScene2D[nCnt]->Update();

		if (nCnt == TYPE_001)
		{
			CBoss * pBoss = CGame::GetBoss();
			m_nLife = pBoss->GetLife();
			float fGauge = float(m_nLife) / float(m_nMaxLife);
			// 頂点座標を設定
			m_vPos[0] = D3DXVECTOR3(m_pos.x, m_pos.y + (-m_fSizeHeight / 2), 0.0f);
			m_vPos[1] = D3DXVECTOR3(m_pos.x + m_fSizeWidth * fGauge, m_pos.y + (-m_fSizeHeight / 2), 0.0f);
			m_vPos[2] = D3DXVECTOR3(m_pos.x, m_pos.y + (m_fSizeHeight / 2), 0.0f);
			m_vPos[3] = D3DXVECTOR3(m_pos.x + m_fSizeWidth * fGauge, m_pos.y + (m_fSizeHeight / 2), 0.0f);

			//頂点座標のセット
			m_apScene2D[nCnt]->SetVtxPosition(m_vPos);
		}
		else if (nCnt == TYPE_002)
		{
			////テクスチャ座標のセット
			//m_apScene2D[nCnt]->SetTex(
			//	m_nPatternAnim*-BG_UV_MOVE,
			//	0.0f,
			//	m_nPatternAnim*-BG_UV_MOVE + BG_UV_MOVE_2,
			//	1.0f);
		}
	}

	if (m_nLife <= 0)
	{
		Uninit();
	}
}

void CBossgauge::Draw(void)
{

}

void CBossgauge::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
