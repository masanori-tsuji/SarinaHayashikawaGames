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
#include "battlebg.h"
#include "game.h"


LPDIRECT3DTEXTURE9 CBattlebg::m_apTexture[MAX_BATTLE_BG] = {};

CBattlebg::CBattlebg(int nPriority) : CScene(nPriority)
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
	m_nCountTime = 0;
	m_nAlpha = 0;
	m_fLength = 0.0f;		// 対角線の長さ
	m_fAngle = 0.0f;		// 対角線の角度
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 向き
	m_fScale = 0.0f;		// 拡大
	m_fAddScale = 0.0f;	// 拡大加算値
}
CBattlebg::~CBattlebg()
{

}

HRESULT CBattlebg::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BATTLE_BG2, &m_apTexture[0]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BATTLE_BG3, &m_apTexture[1]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BATTLE_BG, &m_apTexture[2]);

	return S_OK;
}

void CBattlebg::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_BATTLE_BG; nCnt++)
	{
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

CBattlebg * CBattlebg::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CBattlebg *pBg;

	pBg = new CBattlebg;
	pBg->Init(pos, fSizeWidth, fSizeHeight);

	return pBg;
}

HRESULT CBattlebg::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	SetObjType(CScene::OBJTYPE_NONE);

	for (int nCnt = 0; nCnt < MAX_BATTLE_BG; nCnt++)
	{
		m_apScene2D[nCnt] = new CScene2D(2);
		m_apScene2D[nCnt]->Init(pos, fSizeWidth, fSizeHeight);
		m_apScene2D[nCnt]->BindTexture(m_apTexture[nCnt]);
		// 対角線、角度の設定
		m_fLength = sqrtf((float)(((fSizeWidth / 2) * (fSizeWidth / 2)) + ((fSizeHeight / 2) * (fSizeHeight / 2))));
		m_fAngle = atan2f((fSizeHeight / 2), (fSizeWidth / 2));
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_fScale = 1.0f;
		m_fAddScale = 0.0005f;
		//メンバ変数にサイズ引数を代入
		m_fSizeWidth = fSizeWidth;
		m_fSizeHeight = fSizeHeight;
		m_pos = pos;
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
		m_apScene2D[nCnt]->SetVtxPosition(m_vPos);

		//色のセット
		m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));

		//テクスチャ座標のセット
		m_apScene2D[nCnt]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
	}

	return S_OK;
}

void CBattlebg::Uninit(void)
{
	Release();

}

void CBattlebg::Update(void)
{


		if (m_nAlpha < 255)
		{
			if (m_nCountAnim % 5 == 0)
			{
				m_nAlpha++;
			}
			m_nCountAnim++;
		}

		for (int nCnt = 0; nCnt < MAX_BATTLE_BG; nCnt++)
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
					-m_nPatternAnim*BATTLE_BG_UV_MOVE,
					1.0f,
					-m_nPatternAnim*BATTLE_BG_UV_MOVE + BATTLE_BG_UV_MOVE_2);
				//色のセット
				m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 0, 50, m_nAlpha));
			}
			else if (nCnt == TYPE_002)
			{
				//テクスチャ座標のセット
				m_apScene2D[nCnt]->SetTex(
					0.0f,
					-m_nPatternAnim*BATTLE_BG_UV_MOVE_01,
					1.0f,
					-m_nPatternAnim*BATTLE_BG_UV_MOVE_01 + BATTLE_BG_UV_MOVE_2);
				//色のセット
			/*	if (m_nAlpha <= 110)
				{
					m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));

				}
				else
				{
					m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha - 110));
				}*/
				m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
			}
			else if (nCnt == TYPE_003)
			{
				// 拡大縮小
				m_fScale += m_fAddScale;
				if (m_fScale >= 1.2f || m_fScale <= 0.8f)
				{
					m_fAddScale *= -1;
				}
				////テクスチャ座標のセット
				//m_apScene2D[nCnt]->SetTex(
				//	0.0f,
				//	-m_nPatternAnim*BG_UV_MOVE_02,
				//	1.0f,
				//	-m_nPatternAnim*BG_UV_MOVE_02 + BG_UV_MOVE_2);
				////色のセット
				//m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
				if (m_nAlpha <= 110)
				{
					m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(100, 100, 255, 0));

				}
				else
				{
					m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(100, 100, 255, m_nAlpha - 110));
				}


				//頂点座標の設定
				m_vPos[0].x = m_pos.x - cosf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
				m_vPos[0].y = m_pos.y - sinf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
				m_vPos[0].z = 0.0f;

				m_vPos[1].x = m_pos.x + cosf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
				m_vPos[1].y = m_pos.y - sinf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
				m_vPos[1].z = 0.0f;

				m_vPos[2].x = m_pos.x - cosf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
				m_vPos[2].y = m_pos.y + sinf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
				m_vPos[2].z = 0.0f;

				m_vPos[3].x = m_pos.x + cosf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
				m_vPos[3].y = m_pos.y + sinf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
				m_vPos[3].z = 0.0f;

				//頂点座標のセット
				m_apScene2D[nCnt]->SetVtxPosition(m_vPos);
			}
			/*m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));*/
		}
	//}
	//else
	//{
	//	if (m_nAlpha > 0)
	//	{
	//		if (m_nCountAnim % 5 == 0)
	//		{
	//			m_nAlpha -= 20;
	//		}
	//		m_nCountAnim++;
	//
	//	}
	//	else
	//	{
	//		Uninit();
	//		return;
	//	}

	//	for (int nCnt = 0; nCnt < MAX_BATTLE_BG; nCnt++)
	//	{
	//		if (nCnt == TYPE_001)
	//		{
	//			//テクスチャ座標のセット
	//			m_apScene2D[nCnt]->SetTex(
	//				0.0f,
	//				-m_nPatternAnim*BATTLE_BG_UV_MOVE,
	//				1.0f,
	//				-m_nPatternAnim*BATTLE_BG_UV_MOVE + BATTLE_BG_UV_MOVE_2);
	//			//色のセット
	//			m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 0, 50, m_nAlpha));
	//		}
	//		else if (nCnt == TYPE_002)
	//		{
	//			//テクスチャ座標のセット
	//			m_apScene2D[nCnt]->SetTex(
	//				0.0f,
	//				-m_nPatternAnim*BATTLE_BG_UV_MOVE_01,
	//				1.0f,
	//				-m_nPatternAnim*BATTLE_BG_UV_MOVE_01 + BATTLE_BG_UV_MOVE_2);
	//			//色のセット
	//			/*	if (m_nAlpha <= 110)
	//			{
	//			m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));

	//			}
	//			else
	//			{
	//			m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha - 110));
	//			}*/
	//			m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 255, 255, m_nAlpha));
	//		}
	//		else if (nCnt == TYPE_003)
	//		{
	//			// 拡大縮小
	//			m_fScale += m_fAddScale;
	//			if (m_fScale >= 1.2f || m_fScale <= 0.8f)
	//			{
	//				m_fAddScale *= -1;
	//			}
	//			////テクスチャ座標のセット
	//			//m_apScene2D[nCnt]->SetTex(
	//			//	0.0f,
	//			//	-m_nPatternAnim*BG_UV_MOVE_02,
	//			//	1.0f,
	//			//	-m_nPatternAnim*BG_UV_MOVE_02 + BG_UV_MOVE_2);
	//			////色のセット
	//			//m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	//			if (m_nAlpha <= 110)
	//			{
	//				m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(100, 100, 255, 0));

	//			}
	//			else
	//			{
	//				m_apScene2D[nCnt]->SetColor(D3DCOLOR_RGBA(100, 100, 255, m_nAlpha - 110));
	//			}


	//			//頂点座標の設定
	//			m_vPos[0].x = m_pos.x - cosf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	//			m_vPos[0].y = m_pos.y - sinf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	//			m_vPos[0].z = 0.0f;

	//			m_vPos[1].x = m_pos.x + cosf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	//			m_vPos[1].y = m_pos.y - sinf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	//			m_vPos[1].z = 0.0f;

	//			m_vPos[2].x = m_pos.x - cosf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	//			m_vPos[2].y = m_pos.y + sinf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	//			m_vPos[2].z = 0.0f;

	//			m_vPos[3].x = m_pos.x + cosf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	//			m_vPos[3].y = m_pos.y + sinf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	//			m_vPos[3].z = 0.0f;

	//			//頂点座標のセット
	//			m_apScene2D[nCnt]->SetVtxPosition(m_vPos);
	//		}
	//	}
	//}
}

void CBattlebg::Draw(void)
{

}

void CBattlebg::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
