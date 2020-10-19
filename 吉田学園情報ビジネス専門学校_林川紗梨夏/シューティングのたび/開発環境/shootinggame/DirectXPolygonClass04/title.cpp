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
#include "title.h"
#include "input.h"
#include "joystick.h"
#include "inputkeyboard.h"
#include "fade.h"


LPDIRECT3DTEXTURE9 CTitle::m_apTexture[TYPE_MAX] = {};

CTitle::CTitle(int nPriority)
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
CTitle::~CTitle()
{

}

HRESULT CTitle::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BG, &m_apTexture[0]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BG2_TITLE, &m_apTexture[1]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BG3, &m_apTexture[2]);

	return S_OK;
}

void CTitle::Unload(void)
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

CTitle * CTitle::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CTitle *pTitle;

	pTitle = new CTitle;
	pTitle->Init(pos, fSizeWidth, fSizeHeight);
	
	return pTitle;
}

HRESULT CTitle::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{

		//背景
		m_apScene2D[TYPE_001] = new CScene2D;
		m_apScene2D[TYPE_001]->Init(pos, fSizeWidth, fSizeHeight);
		m_apScene2D[TYPE_001]->BindTexture(m_apTexture[TYPE_001]);

		//メンバ変数にサイズ引数を代入
		m_fSizeWidth = fSizeWidth;
		m_fSizeHeight = fSizeHeight;

		// 頂点座標を設定
		m_vPos[0] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);

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

		m_apScene2D[TYPE_002] = new CScene2D;
		m_apScene2D[TYPE_002]->Init(pos, fSizeWidth, fSizeHeight);
		m_apScene2D[TYPE_002]->BindTexture(m_apTexture[TYPE_002]);

		//メンバ変数にサイズ引数を代入
		m_fSizeWidth = fSizeWidth;
		m_fSizeHeight = fSizeHeight;

		// 頂点座標を設定
		m_vPos[0] = D3DXVECTOR3(pos.x + (-SCREEN_WIDTH / 2), 850.0f + (-234 / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (SCREEN_WIDTH / 2), 850.0f + (-234 / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-SCREEN_WIDTH / 2), 850.0f + (234 / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (SCREEN_WIDTH / 2), 850.0f + (234 / 2), 0.0f);

		//頂点座標のセット
		m_apScene2D[TYPE_002]->SetVtxPosition(m_vPos);

		//色のセット
		m_apScene2D[TYPE_002]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//テクスチャ座標のセット
		m_apScene2D[TYPE_002]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);

		m_apScene2D[TYPE_003] = new CScene2D;
		m_apScene2D[TYPE_003]->Init(pos, fSizeWidth, fSizeHeight);
		m_apScene2D[TYPE_003]->BindTexture(m_apTexture[TYPE_003]);

		//メンバ変数にサイズ引数を代入
		m_fSizeWidth = fSizeWidth;
		m_fSizeHeight = fSizeHeight;

		// 頂点座標を設定
		m_vPos[0] = D3DXVECTOR3(pos.x + (-1368 / 2), 350.0f + (-710 / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (1368 / 2), 350.0f + (-710 / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-1368 / 2), 350.0f + (710 / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (1368 / 2), 350.0f + (710 / 2), 0.0f);

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

	return S_OK;
}

void CTitle::Uninit(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt] = NULL;

		}
	}
	
	Release();
}

void CTitle::Update(void)
{

		//キーボードの取得
		CInputKeyboard * pInputKeyboard = CManager::GetInputKeyboard();
#define JS
#ifdef JS
	//ジョイパッド
	CInputJoystick * pInputJoystick = CManager::GetInputJoystick();
	LPDIRECTINPUTDEVICE8 lpDIDevice = CInputJoystick::GetDevice();
	DIJOYSTATE js;
	if (lpDIDevice != NULL)
	{
		lpDIDevice->Poll();
		lpDIDevice->GetDeviceState(sizeof(DIJOYSTATE), &js);
	}

	
#endif

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
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
			////テクスチャ座標のセット
			//m_apScene2D[nCnt]->SetTex(
			//	0.0f,
			//	-m_nPatternAnim*BG_UV_MOVE,
			//	1.0f,
			//	-m_nPatternAnim*BG_UV_MOVE + BG_UV_MOVE_2);
		}
		else if (nCnt == TYPE_002)
		{
			//テクスチャ座標のセット
			m_apScene2D[nCnt]->SetTex(
				-m_nPatternAnim*BG_UV_MOVE,
				0.0f,
				-m_nPatternAnim*BG_UV_MOVE + BG_UV_MOVE_2,
				1.0f);
		}
		else if (nCnt == TYPE_003)
		{
			////テクスチャ座標のセット
			//m_apScene2D[nCnt]->SetTex(
			//	0.0f,
			//	-m_nPatternAnim*BG_UV_MOVE_02,
			//	1.0f,
			//	-m_nPatternAnim*BG_UV_MOVE_02 + BG_UV_MOVE_2);
		}
	}

	if (lpDIDevice != NULL && pInputJoystick->GetJoystickTrigger(JS_A)|| CInputKeyboard::GetKeyboardTrigger(DIK_SPACE))//ボム
	{
		CManager::StartFade(CManager::MODE_GAME, D3DCOLOR_RGBA(0, 0, 0, 0), 0.05f);
		/*CManager::SetMode(CManager::MODE_GAME);*/
		/* Uninit();*/
		return;
	}
}

void CTitle::Draw(void)
{

}

void CTitle::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
