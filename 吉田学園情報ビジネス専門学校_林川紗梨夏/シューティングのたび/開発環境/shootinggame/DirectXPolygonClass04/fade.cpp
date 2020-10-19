//---------------------------------------------------------------
//	fadeの処理
//	fade.cpp
//	Authur 林川紗梨夏
//---------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "fade.h"
#include "star.h"
#include "text.h"
//--------------------------------------------------------
//コンストラクタ
//--------------------------------------------------------
CFade::CFade()
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nSizeX = 0;
	m_nSizeY = 0;
	m_fade = FADE_NONE;
	m_ModeNext = CManager::MODE_NONE;
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_fFadelate = FADE_RATE;
}
//--------------------------------------------------------
//デストラクタ
//--------------------------------------------------------
CFade::~CFade()
{
}
//--------------------------------------------------------
//生成
//--------------------------------------------------------
CFade * CFade::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int sizeX, int sizeY, CManager::MODE mode)
{
	CFade * pFade;
	//メモリ確保
	pFade = new CFade;
	//初期化
	pFade->Init(pos, rot, sizeX, sizeY, mode);
	return pFade;
}
//--------------------------------------------------------
//初期化
//--------------------------------------------------------
HRESULT CFade::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int sizeX, int sizeY, CManager::MODE mode)
{
	//初期化
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	//頂点情報を設定
	VERTEX_2D *pVtx;
	m_pos = pos;
	m_nSizeX = sizeX;
	m_nSizeY = sizeY;
	m_ModeNext = mode;
	m_fade = FADE_IN;
	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-m_nSizeX / 2), m_pos.y + (-m_nSizeY / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_nSizeX / 2), m_pos.y + (-m_nSizeY / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-m_nSizeX / 2), m_pos.y + (m_nSizeY / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_nSizeX / 2), m_pos.y + (m_nSizeY / 2), 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定 
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
	return S_OK;
}
//--------------------------------------------------------
//終了
//--------------------------------------------------------
void CFade::Uninit(void)
{
	//終了
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}
//--------------------------------------------------------
//更新
//--------------------------------------------------------
void CFade::Update(void)
{
	//頂点情報を設定
	VERTEX_2D *pVtx;
	//モード
	m_ModeNext = CManager::GetMode();
	//更新
	if (m_fade != FADE_NONE)
	{
		//フェードイン
		if (m_fade == FADE_IN)
		{
			//a値を加算
			m_colorFade.a += m_fFadelate;
			//画面が黒くなったら
			if (m_colorFade.a >= 1.0f)
			{
				//タイトルに遷移
				CManager::SetMode(m_ModeNext);
				//フェード終了処理
				m_colorFade.a = 1.0f;
				m_fade = FADE_OUT;
			}
		}
		//フェードアウト
		if (m_fade == FADE_OUT)
		{
			//α値を減算
			m_colorFade.a -= m_fFadelate;
			//画面の透過
			if (m_colorFade.a <= 0.0f)
			{
				//フェード処理切り替え
				m_colorFade.a = 0.0f;
				//モード設定
				m_fade = FADE_IN;
				CManager::StopFade();
				

			}
		}
		//頂点バッファをロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		//頂点カラーの設定 
		pVtx[0].col = m_colorFade;
		pVtx[1].col = m_colorFade;
		pVtx[2].col = m_colorFade;
		pVtx[3].col = m_colorFade;
		//頂点バッファのアンロック
		m_pVtxBuff->Unlock();
	}
}
//--------------------------------------------------------
//描画
//--------------------------------------------------------
void CFade::Draw(void)
{
	//描画
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0, NULL);
	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		NUM_POLYGON);
}

void CFade::SetColor(D3DXCOLOR col)
{
	m_colorFade.r = col.r;
	m_colorFade.g = col.g;
	m_colorFade.b = col.b;
}

void CFade::SetFadelate(float fadelate)
{
	m_fFadelate = fadelate;
}

//#include "main.h"
//#include "manager.h"
//#include "scene.h"
//#include "renderer.h"
//#include "fade.h"
//#include "star.h"
//
//LPDIRECT3DTEXTURE9 CFade::m_pTexture = NULL;
//CFade::CFade()
//{
//	m_pVtxBuff = NULL;
//	m_nPatternAnim = TYPE_NOGLOW;
//}
//
//CFade::~CFade()
//{
//}
//
//CFade * CFade::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, CManager::MODE mode)
//{
//	CFade *pFade;
//
//	pFade = new CFade;
//	pFade->Init(pos, fSizeWidth, fSizeHeight);
//
//	return pFade;
//}
//
//HRESULT CFade::Load(void)
//{
//	CRenderer * pRenderer = CManager::GetRenderer();
//	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
//	// テクスチャの生成
//	D3DXCreateTextureFromFile(pDevice, TEXTURE_STAR, &m_pTexture);
//	return S_OK;
//}
//
//void CFade::Unload(void)
//{
//	if (m_pTexture != NULL)
//	{
//		m_pTexture->Release();
//		m_pTexture = NULL;
//	}
//}
//
//HRESULT CFade::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, CManager::MODE mode)
//{
//	CRenderer * pRenderer = CManager::GetRenderer();
//	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
//
//	// 頂点バッファの生成
//	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
//		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
//		FVF_VERTEX_2D,				// 使用する頂点フォーマット
//		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
//		&m_pVtxBuff,				// 頂点バッファへのポインタ
//		NULL)))						// NULLに設定
//	{
//		return E_FAIL;
//	}
//
//	VERTEX_2D *pVtx;
//
//	// 頂点バッファをロックし、頂点情報へのポインタを取得
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	pVtx[0].pos = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
//	pVtx[1].pos = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
//	pVtx[2].pos = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);
//	pVtx[3].pos = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);
//
//	pVtx[0].rhw = 1.0f;
//	pVtx[1].rhw = 1.0f;
//	pVtx[2].rhw = 1.0f;
//	pVtx[3].rhw = 1.0f;
//
//	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//
//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//	m_pVtxBuff->Unlock();
//	return S_OK;
//}
//
//void CFade::Uninit(void)
//{
//	if (m_pVtxBuff != NULL)
//	{
//		m_pVtxBuff->Release();
//		m_pVtxBuff = NULL;
//	}
//}
//
//void CFade::Update(void)
//{
//}
//
//void CFade::Draw(void)
//{
//	CRenderer *pRenderer = CManager::GetRenderer();
//	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
//
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
//
//	pDevice->SetTexture(0, m_pTexture);
//
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
//}
//
//void CFade::SetFade(CManager::MODE mode)
//{
//
//	VERTEX_2D *pVtx;
//
//	// 頂点バッファをロックし、頂点情報へのポインタを取得
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	pVtx[0].tex = D3DXVECTOR2(type * 0.5f, 0.0f);
//	pVtx[1].tex = D3DXVECTOR2(type * 0.5f + 0.5f, 0.0f);
//	pVtx[2].tex = D3DXVECTOR2(type * 0.5f, 1.0f);
//	pVtx[3].tex = D3DXVECTOR2(type * 0.5f + 0.5f, 1.0f);
//
//	m_pVtxBuff->Unlock();
//}
