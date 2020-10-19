//=============================================================================
//
// シーン2D処理 [scene2D.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include <time.h>
#include "input.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "bg.h"

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CScene2D::m_pSceneTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPriority) : CScene(nPriority)
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
		m_pTexture = NULL;
		m_pVtxBuff = NULL;
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Color = D3DCOLOR_RGBA(255,255,255,255);
		memset(m_vPos, 0, sizeof(m_vPos));
		m_fTexX = 0.0f;
		m_fTexX2 = 1.0f;
		m_fTexY = 0.0f;
		m_fTexY2 = 1.0f;

}

//=============================================================================
// デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	SetObjType(CScene::OBJTYPE_NONE);
	// 頂点バッファの生成
 	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&m_pVtxBuff,				// 頂点バッファへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報を設定
	/*pVtx[0].pos = D3DXVECTOR3(m_fPosX,  m_fPosY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fPosX2, m_fPosY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_fPosX,  m_fPosY2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fPosX2, m_fPosY2, 0.0f);*/

	pVtx[0].pos = m_vPos[0];
	pVtx[1].pos = m_vPos[1];
	pVtx[2].pos = m_vPos[2];
	pVtx[3].pos = m_vPos[3];

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	pVtx[0].tex = D3DXVECTOR2(m_fTexX, m_fTexY);
	pVtx[1].tex = D3DXVECTOR2(m_fTexX2, m_fTexY);
	pVtx[2].tex = D3DXVECTOR2(m_fTexX, m_fTexY2);
	pVtx[3].tex = D3DXVECTOR2(m_fTexX2, m_fTexY2);

	CScene::SetPosition(pos);
	m_pVtxBuff->Unlock();
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	Release();
}
void CScene2D::Update(void)
{
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報を設定
	pVtx[0].pos = m_vPos[0];
	pVtx[1].pos = m_vPos[1];
	pVtx[2].pos = m_vPos[2];
	pVtx[3].pos = m_vPos[3];

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	pVtx[0].tex = D3DXVECTOR2(m_fTexX, m_fTexY);
	pVtx[1].tex = D3DXVECTOR2(m_fTexX2, m_fTexY);
	pVtx[2].tex = D3DXVECTOR2(m_fTexX, m_fTexY2);
	pVtx[3].tex = D3DXVECTOR2(m_fTexX2, m_fTexY2);

	CScene::SetPosition(m_pos);
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	pDevice->SetTexture(0, m_pTexture);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

CScene2D * CScene2D::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CScene2D *pPolygon;

	pPolygon = new CScene2D;
	pPolygon->SetPosition(pos);
	pPolygon->Init(D3DXVECTOR3(SCREEN_CENTER_X,SCREEN_CENTER_Y,0.0f),SCREEN_WIDTH,SCREEN_HEIGHT);

	return pPolygon;
}

void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

HRESULT CScene2D::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BG, &m_pSceneTexture);
	return S_OK;
}

void CScene2D::Unload(void)
{
	if (m_pSceneTexture != NULL)
	{
		m_pSceneTexture->Release();
		m_pSceneTexture = NULL;
	}
}

void CScene2D::SetVertex(VERTEX_2D * pSVtx)
{
	VERTEX_2D *pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pVtx = pSVtx;
	m_pVtxBuff->Unlock();

}

void CScene2D::SetTex(float fTexX, float fTexY, float fTexX2, float fTexY2)
{
	m_fTexX = fTexX;
	m_fTexY = fTexY;
	m_fTexX2 = fTexX2;
	m_fTexY2 = fTexY2;
}

void CScene2D::SetColor(D3DCOLOR col)
{
	m_Color = col;
}

void CScene2D::SetVtxPosition(D3DXVECTOR3 vPos[NUM_VERTEX])
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = vPos[nCntVertex];
	}
}
