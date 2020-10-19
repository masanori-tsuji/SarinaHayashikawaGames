//=============================================================================
//
// メッセージウィンドウの処理 [msgwindow.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//
// ヘッダファイルのインクルード
//
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "msgwindow.h"


LPDIRECT3DTEXTURE9 CMsgWindow::m_apTexture[TYPE_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CMsgWindow::CMsgWindow(int nPriority) : CScene(nPriority)
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	memset(m_apScene2D, 0, sizeof(m_apScene2D));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fSizeWidth = 0.0f;
	m_fSizeHeight = 0.0f;
	m_nPatternAnim = 0;
	m_nCountAnim = 0;
	m_bHitUi = false;
	m_chara = CHARA_NONE;
}
//=============================================================================
// デストラクタ
//=============================================================================
CMsgWindow::~CMsgWindow()
{

}
//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CMsgWindow::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MSG001, &m_apTexture[TYPE_001]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MSG002, &m_apTexture[TYPE_002]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MSG003, &m_apTexture[TYPE_003]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MSG004, &m_apTexture[TYPE_004]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MSG005, &m_apTexture[TYPE_005]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MSG006, &m_apTexture[TYPE_006]);

	return S_OK;
}
//=============================================================================
// テクスチャアンロード
//=============================================================================
void CMsgWindow::Unload(void)
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

CMsgWindow * CMsgWindow::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CMsgWindow *pUi;

	pUi = new CMsgWindow;
	pUi->Init(pos, fSizeWidth, fSizeHeight);

	return pUi;
}

HRESULT CMsgWindow::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	SetObjType(CScene::OBJTYPE_NONE);

	m_pos = pos;

	if (fSizeWidth > 0)
	{
		m_chara = CHARA_001;
	}
	else
	{
		m_chara = CHARA_002;
	}
	float aaa = -1.0f;
	switch (m_chara)
	{
	case CHARA_001:

		//したフレーム

		m_apScene2D[TYPE_001] = new CScene2D(5);
		m_apScene2D[TYPE_001]->Init(pos, 662, 286);
		m_apScene2D[TYPE_001]->BindTexture(m_apTexture[TYPE_001]);

		//メンバ変数にサイズ引数を代入
		m_fSizeWidth = fSizeWidth;
		m_fSizeHeight = fSizeHeight;

		
		// 頂点座標を設定
		m_vPos[0] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (-286 / 2), aaa);
		m_vPos[1] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (-286 / 2), aaa);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (286 / 2), aaa);
		m_vPos[3] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (286 / 2), aaa);

		//頂点座標のセット
		m_apScene2D[TYPE_001]->SetVtxPosition(m_vPos);

		//色のセット
		m_apScene2D[TYPE_001]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//テクスチャ座標のセット
		m_apScene2D[TYPE_001]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);


		////顔

		m_apScene2D[TYPE_002] = new CScene2D(5);
		m_apScene2D[TYPE_002]->Init(pos, 570, 418);
		m_apScene2D[TYPE_002]->BindTexture(m_apTexture[TYPE_002]);

		// 頂点座標を設定
		m_vPos[0] = D3DXVECTOR3(pos.x + (-CHARA_SIZE_WIDTH / 2) + 200.0f, pos.y + (-CHARA_SIZE_HEIGHT / 2) + 55.0f, 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (CHARA_SIZE_WIDTH / 2) + 200.0f, pos.y + (-CHARA_SIZE_HEIGHT / 2) + 55.0f, 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-CHARA_SIZE_WIDTH / 2) + 200.0f, pos.y + (CHARA_SIZE_HEIGHT / 2) + 55.0f, 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (CHARA_SIZE_WIDTH / 2) + 200.0f, pos.y + (CHARA_SIZE_HEIGHT / 2) + 55.0f, 0.0f);

		//頂点座標のセット
		m_apScene2D[TYPE_002]->SetVtxPosition(m_vPos);

		//色のセット
		m_apScene2D[TYPE_002]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//テクスチャ座標のセット
		m_apScene2D[TYPE_002]->SetTex(
			m_nPatternAnim * 0.2f,
			0.0f,
			m_nPatternAnim * 0.2f + 0.2f,
			1.0f);

		////上フレーム

		m_apScene2D[TYPE_003] = new CScene2D(5);
		m_apScene2D[TYPE_003]->Init(pos, 662, 286);
		m_apScene2D[TYPE_003]->BindTexture(m_apTexture[TYPE_003]);



		// 頂点座標を設定
		m_vPos[0] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (-286 / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (-286 / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (286 / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (286 / 2), 0.0f);

		//頂点座標のセット
		m_apScene2D[TYPE_003]->SetVtxPosition(m_vPos);

		//色のセット
		m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//テクスチャ座標のセット
		m_apScene2D[TYPE_003]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);

		break;
	case CHARA_002:

		//したフレーム

		m_apScene2D[TYPE_001] = new CScene2D(5);
		m_apScene2D[TYPE_001]->Init(pos, 662, 286);
		m_apScene2D[TYPE_001]->BindTexture(m_apTexture[TYPE_004]);

		//メンバ変数にサイズ引数を代入
		m_fSizeWidth = fSizeWidth;
		m_fSizeHeight = fSizeHeight;

		// 頂点座標を設定
		m_vPos[0] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (-286 / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (-286 / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (286 / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (286 / 2), 0.0f);

		//頂点座標のセット
		m_apScene2D[TYPE_001]->SetVtxPosition(m_vPos);

		//色のセット
		m_apScene2D[TYPE_001]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//テクスチャ座標のセット
		m_apScene2D[TYPE_001]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);


		////顔

		m_apScene2D[TYPE_002] = new CScene2D(5);
		m_apScene2D[TYPE_002]->Init(pos, 570, 418);
		m_apScene2D[TYPE_002]->BindTexture(m_apTexture[TYPE_005]);

		// 頂点座標を設定
		m_vPos[0] = D3DXVECTOR3(pos.x + (-CHARA_SIZE_WIDTH / 2) - 200.0f, pos.y + (-CHARA_SIZE_HEIGHT02 / 2) + 55.0f, 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (CHARA_SIZE_WIDTH / 2) - 200.0f, pos.y + (-CHARA_SIZE_HEIGHT02 / 2) + 55.0f, 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-CHARA_SIZE_WIDTH / 2) - 200.0f, pos.y + (CHARA_SIZE_HEIGHT02 / 2) + 55.0f, 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (CHARA_SIZE_WIDTH / 2) - 200.0f, pos.y + (CHARA_SIZE_HEIGHT02 / 2) + 55.0f, 0.0f);

		//頂点座標のセット
		m_apScene2D[TYPE_002]->SetVtxPosition(m_vPos);

		//色のセット
		m_apScene2D[TYPE_002]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//テクスチャ座標のセット
		m_apScene2D[TYPE_002]->SetTex(
			m_nPatternAnim * 0.2f,
			0.0f,
			m_nPatternAnim * 0.2f + 0.2f,
			1.0f);

		////上フレーム

		m_apScene2D[TYPE_003] = new CScene2D(5);
		m_apScene2D[TYPE_003]->Init(pos, 662, 286);
		m_apScene2D[TYPE_003]->BindTexture(m_apTexture[TYPE_006]);



		// 頂点座標を設定
		m_vPos[0] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (-286 / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (-286 / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (286 / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (286 / 2), 0.0f);

		//頂点座標のセット
		m_apScene2D[TYPE_003]->SetVtxPosition(m_vPos);

		//色のセット
		m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//テクスチャ座標のセット
		m_apScene2D[TYPE_003]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);

		break;
	default:
		break;
	}
	

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CMsgWindow::Uninit(void)
{
	
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CMsgWindow::Update(void)
{
	//したフレーム
	float aaa = -1.0f;
	switch (m_chara)
	{
	case CHARA_001:

	m_apScene2D[TYPE_001]->Update();
	
	// 頂点座標を設定
	m_vPos[0] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (-286 / 2), 0.0f);
	m_vPos[1] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (-286 / 2), 0.0f);
	m_vPos[2] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (286 / 2), 0.0f);
	m_vPos[3] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (286 / 2), 0.0f);

	//頂点座標のセット
	m_apScene2D[TYPE_001]->SetVtxPosition(m_vPos);

	//色のセット
	m_apScene2D[TYPE_001]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	//テクスチャ座標のセット
	m_apScene2D[TYPE_001]->SetTex(
		0.0f,
		0.0f,
		1.0f,
		1.0f);


	//顔

	// 頂点座標を設定
	m_vPos[0] = D3DXVECTOR3(m_pos.x + (-CHARA_SIZE_WIDTH / 2) + 200.0f, m_pos.y + (-CHARA_SIZE_HEIGHT / 2) + 55.0f, aaa);
	m_vPos[1] = D3DXVECTOR3(m_pos.x + (CHARA_SIZE_WIDTH / 2) + 200.0f, m_pos.y + (-CHARA_SIZE_HEIGHT / 2) + 55.0f, aaa);
	m_vPos[2] = D3DXVECTOR3(m_pos.x + (-CHARA_SIZE_WIDTH / 2) + 200.0f, m_pos.y + (CHARA_SIZE_HEIGHT / 2) + 55.0f, aaa);
	m_vPos[3] = D3DXVECTOR3(m_pos.x + (CHARA_SIZE_WIDTH / 2) + 200.0f, m_pos.y + (CHARA_SIZE_HEIGHT / 2) + 55.0f, aaa);

	m_apScene2D[TYPE_002]->Update();

	//頂点座標のセット
	m_apScene2D[TYPE_002]->SetVtxPosition(m_vPos);

	//色のセット
	m_apScene2D[TYPE_002]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 220));

	//テクスチャ座標のセット
	m_apScene2D[TYPE_002]->SetTex(
		m_nPatternAnim * 0.2f,
		0.0f,
		m_nPatternAnim * 0.2f + 0.2f,
		1.0f);

	////上フレーム

	m_apScene2D[TYPE_003]->Update();

	// 頂点座標を設定
	m_vPos[0] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (-286 / 2), 0.0f);
	m_vPos[1] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (-286 / 2), 0.0f);
	m_vPos[2] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (286 / 2), 0.0f);
	m_vPos[3] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (286 / 2), 0.0f);

	//頂点座標のセット
	m_apScene2D[TYPE_003]->SetVtxPosition(m_vPos);

	//色のセット
	m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(0, 230, 200, 255));

	//テクスチャ座標のセット
	m_apScene2D[TYPE_003]->SetTex(
		0.0f,
		0.0f,
		1.0f,
		1.0f);

	if (m_bHitUi == true)
	{
		//色のセット
		m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(255, 0, 0, 255));

		m_apScene2D[TYPE_002]->SetTex(
			4 * 0.2f,
			0.0f,
			4 * 0.2f + 0.2f,
			1.0f);
	}
	else
	{
		//色のセット
		m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(0, 230, 200, 255));

		m_apScene2D[TYPE_002]->SetTex(
			m_nPatternAnim * 0.2f,
			0.0f,
			m_nPatternAnim * 0.2f + 0.2f,
			1.0f);
	}
	break;
	case CHARA_002:
		m_apScene2D[TYPE_001]->Update();

		// 頂点座標を設定
		m_vPos[0] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (-286 / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (-286 / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (286 / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (286 / 2), 0.0f);

		//頂点座標のセット
		m_apScene2D[TYPE_001]->SetVtxPosition(m_vPos);

		//色のセット
		m_apScene2D[TYPE_001]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//テクスチャ座標のセット
		m_apScene2D[TYPE_001]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);


		//顔

		// 頂点座標を設定
		m_vPos[0] = D3DXVECTOR3(m_pos.x + (-CHARA_SIZE_WIDTH / 2) - 200.0f, m_pos.y + (-CHARA_SIZE_HEIGHT02 / 2) - 20.0f, 0.0f);
		m_vPos[1] = D3DXVECTOR3(m_pos.x + (CHARA_SIZE_WIDTH / 2) - 200.0f, m_pos.y + (-CHARA_SIZE_HEIGHT02 / 2) - 20.0f, 0.0f);
		m_vPos[2] = D3DXVECTOR3(m_pos.x + (-CHARA_SIZE_WIDTH / 2) - 200.0f, m_pos.y + (CHARA_SIZE_HEIGHT02 / 2) - 20.0f, 0.0f);
		m_vPos[3] = D3DXVECTOR3(m_pos.x + (CHARA_SIZE_WIDTH / 2) - 200.0f, m_pos.y + (CHARA_SIZE_HEIGHT02 / 2) - 20.0f, 0.0f);

		m_apScene2D[TYPE_002]->Update();

		//頂点座標のセット
		m_apScene2D[TYPE_002]->SetVtxPosition(m_vPos);

		//色のセット
		m_apScene2D[TYPE_002]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 220));

		//テクスチャ座標のセット
		m_apScene2D[TYPE_002]->SetTex(
			m_nPatternAnim * 0.2f,
			0.0f,
			m_nPatternAnim * 0.2f + 0.2f,
			1.0f);

		////上フレーム

		m_apScene2D[TYPE_003]->Update();

		// 頂点座標を設定
		m_vPos[0] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (-286 / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (-286 / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (286 / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (286 / 2), 0.0f);

		//頂点座標のセット
		m_apScene2D[TYPE_003]->SetVtxPosition(m_vPos);

		//色のセット
		m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(0, 230, 200, 255));

		//テクスチャ座標のセット
		m_apScene2D[TYPE_003]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);

		if (m_bHitUi == true)
		{
			//色のセット
			m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(255, 0, 0, 255));

			m_apScene2D[TYPE_002]->SetTex(
				2 * 0.25f,
				0.0f,
				2 * 0.25f + 0.25f,
				1.0f);
		}
		else
		{
			//色のセット
			m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(0, 230, 200, 255));

			m_apScene2D[TYPE_002]->SetTex(
				m_nPatternAnim * 0.25f,
				0.0f,
				m_nPatternAnim * 0.25f + 0.25f,
				1.0f);
		}
		break;
	default:
		break;
	}

}
//=============================================================================
// 描画処理
//=============================================================================
void CMsgWindow::Draw(void)
{

}
//=============================================================================
// 位置セット
//=============================================================================
void CMsgWindow::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//=============================================================================
// キャラクタの表情セット
//=============================================================================
void CMsgWindow::SetChara(CTYPE type)
{
	m_chara = type;
}
//=============================================================================
// ヒットしたときの表示処理
//=============================================================================
void CMsgWindow::HitMsgUi(bool bHit)
{
	m_bHitUi = bHit;

}
//=============================================================================
// フレームの表示変更
//=============================================================================
void CMsgWindow::SetMsgUi( int UiIndex)
{
	m_nPatternAnim = UiIndex;
}
