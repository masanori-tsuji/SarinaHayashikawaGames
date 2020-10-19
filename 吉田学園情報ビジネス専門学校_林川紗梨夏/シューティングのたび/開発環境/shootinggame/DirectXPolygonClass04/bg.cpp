//=============================================================================
//
// 背景の処理 [bg.cpp]
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
#include "bg.h"


LPDIRECT3DTEXTURE9 CBg::m_apTexture[MAX_BG] = {};

CBg::CBg(int nPriority) : CScene(nPriority)
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
}
CBg::~CBg()
{

}

HRESULT CBg::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BG, &m_apTexture[0]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BG2, &m_apTexture[1]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BG3, &m_apTexture[2]);

	return S_OK;
}

void CBg::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

CBg * CBg::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CBg *pBg;

		pBg = new CBg;
		pBg->Init(pos, fSizeWidth, fSizeHeight);

	return pBg;
}

HRESULT CBg::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	SetObjType(CScene::OBJTYPE_NONE);

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		m_apScene2D[nCnt] = new CScene2D(1);
		m_apScene2D[nCnt]->Init(pos, fSizeWidth, fSizeHeight);
		m_apScene2D[nCnt]->BindTexture(m_apTexture[nCnt]);

		//メンバ変数にサイズ引数を代入
		m_fSizeWidth = fSizeWidth;
		m_fSizeHeight = fSizeHeight;

		// 頂点座標を設定
		m_vPos[0] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);

		//頂点座標のセット
		m_apScene2D[nCnt]->SetVtxPosition(m_vPos);

		//色のセット
		m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//テクスチャ座標のセット
		m_apScene2D[nCnt]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
	}

	return S_OK;
}

void CBg::Uninit(void)
{
	/*for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt] = NULL;
			
		}
	}*/
	Release();
	
}

void CBg::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		m_apScene2D[nCnt]->Update();

		m_nCountAnim++;
		if (m_nCountAnim % 3)
		{
			m_nPatternAnim++;
			m_nCountAnim = 0;
		}
	
		if (nCnt == TYPE_001)
		{
			//テクスチャ座標のセット
			m_apScene2D[nCnt]->SetTex(
				0.0f,
				-m_nPatternAnim*BG_UV_MOVE,
				1.0f,
				-m_nPatternAnim*BG_UV_MOVE + BG_UV_MOVE_2);
	
		}
		else if (nCnt == TYPE_002)
		{
			//テクスチャ座標のセット
			m_apScene2D[nCnt]->SetTex(
				0.0f,
				-m_nPatternAnim*BG_UV_MOVE_01,
				1.0f,
				-m_nPatternAnim*BG_UV_MOVE_01 + BG_UV_MOVE_2);
	
		}
		else if (nCnt == TYPE_003)
		{
			//テクスチャ座標のセット
			m_apScene2D[nCnt]->SetTex(
				0.0f,
				-m_nPatternAnim*BG_UV_MOVE_02,
				1.0f,
				-m_nPatternAnim*BG_UV_MOVE_02 + BG_UV_MOVE_2);
			//色のセット
			m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 150));
		}
	}
}

void CBg::Draw(void)
{
	//CRenderer * pRenderer = CManager::GetRenderer();
	//LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	//for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	//{
	//	if (nCnt == TYPE_002 || nCnt == TYPE_003)
	//	{
	//		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//		m_apScene2D[nCnt]->CScene2D::Draw();
	//		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//	}
	//	else
	//	{
	//		m_apScene2D[nCnt]->CScene2D::Draw();
	//	}

	//}
}

void CBg::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
