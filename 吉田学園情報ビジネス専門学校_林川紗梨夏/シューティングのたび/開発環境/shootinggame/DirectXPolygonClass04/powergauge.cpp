//=============================================================================
//
// パワーゲージの処理 [powergauge.cpp]
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
#include "powergauge.h"
#include "player.h"

LPDIRECT3DTEXTURE9 CPowergauge::m_apTexture[TYPE_MAX] = {};
//=============================================================================
// コンストラクタ
//=============================================================================
CPowergauge::CPowergauge(int nPriority) : CScene(nPriority)
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
	m_nPower = 0;
	m_nMaxPower = PLAYER_MAXPOWER;
}
//=============================================================================
// デストラクタ
//=============================================================================
CPowergauge::~CPowergauge()
{

}
//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CPowergauge::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAUGE, &m_apTexture[0]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, NULL, &m_apTexture[1]);

	return S_OK;
}
//=============================================================================
// テクスチャアンロード
//=============================================================================
void CPowergauge::Unload(void)
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
//=============================================================================
// 生成処理
//=============================================================================
CPowergauge * CPowergauge::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, int nMaxlife)
{
	CPowergauge *pPowergauge;

	pPowergauge = new CPowergauge;
	pPowergauge->Init(pos, fSizeWidth, fSizeHeight);
	return pPowergauge;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPowergauge::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	SetObjType(CScene::OBJTYPE_NONE);
	CPlayer * pPlayer = CGame::GetPlayer();
	m_nPower = pPlayer->GetPower();
	m_pos = pos;
	

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
			m_vPos[1] = D3DXVECTOR3(m_pos.x + fSizeWidth * (m_nPower / m_nMaxPower), m_pos.y + (-m_fSizeHeight / 2), 0.0f);
			m_vPos[2] = D3DXVECTOR3(m_pos.x, m_pos.y + (m_fSizeHeight / 2), 0.0f);
			m_vPos[3] = D3DXVECTOR3(m_pos.x + fSizeWidth * (m_nPower / m_nMaxPower), m_pos.y + (m_fSizeHeight / 2), 0.0f);

			//色のセット
			m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(80, 150, 255, 255));
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
//=============================================================================
// 終了処理
//=============================================================================
void CPowergauge::Uninit(void)
{
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CPowergauge::Update(void)
{

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apScene2D[nCnt]->Update();

		if (nCnt == TYPE_001)
		{
			CPlayer * pPlayer = CGame::GetPlayer();
			m_nPower = pPlayer->GetPower();
			float fGauge = float(m_nPower) / float(m_nMaxPower);
			// 頂点座標を設定
			m_vPos[0] = D3DXVECTOR3(m_pos.x, m_pos.y + (-m_fSizeHeight / 2), 0.0f);
			m_vPos[1] = D3DXVECTOR3(m_pos.x + m_fSizeWidth * fGauge, m_pos.y + (-m_fSizeHeight / 2), 0.0f);
			m_vPos[2] = D3DXVECTOR3(m_pos.x, m_pos.y + (m_fSizeHeight / 2), 0.0f);
			m_vPos[3] = D3DXVECTOR3(m_pos.x + m_fSizeWidth * fGauge, m_pos.y + (m_fSizeHeight / 2), 0.0f);

			//頂点座標のセット
			m_apScene2D[nCnt]->SetVtxPosition(m_vPos);
			if (m_nPower >= PLAYER_MAXPOWER  * 0.33)
			{
				m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(0, 255, 100, 255));
			}
			if (m_nPower >= PLAYER_MAXPOWER  * 0.66)
			{
				m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
			}
			if (m_nPower >= PLAYER_MAXPOWER)
			{
				m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 0, 0, 255));
			}
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
}
//=============================================================================
// 描画処理
//=============================================================================
void CPowergauge::Draw(void)
{

}
//=============================================================================
// 位置セット
//=============================================================================
void CPowergauge::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
