//=============================================================================
//
// シーン処理 [scene.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "renderer.h"
#include "scene2D.h"
#include "manager.h"

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
CScene * CScene::m_apScene[PRIORITY_MAX][MAX_POLYGON] = {};
int CScene::m_nNumAll = 0;
//=============================================================================
// コンストラクタ
//=============================================================================
CScene::CScene(int nPriority)
{
	m_nPriority = nPriority;
	m_objType = OBJTYPE_NONE;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nID = 0;
	for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	{
		if (m_apScene[nPriority][nCntScene] == NULL)
		{
			m_apScene[nPriority][nCntScene] = this;
			m_nNumAll++;
			m_nID = nCntScene;
			break;
		}
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene::~CScene()
{
}

//=============================================================================
// 更新処理（全てのシーン）
//=============================================================================
void CScene::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				m_apScene[nCntPriority][nCntScene]->Update();
			}
		}
	}
}
//=============================================================================
// 描画処理（全てのシーン）
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				m_apScene[nCntPriority][nCntScene]->Draw();
			}
		}
	}
}
//=============================================================================
// 解放処理（全てのシーン）
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				m_apScene[nCntPriority][nCntScene]->Uninit();
			}
		}
	}
}
//=============================================================================
// 解放処理
//=============================================================================
void CScene::Release(void)
{
	if (m_apScene[m_nID] != NULL)
	{
		int nID = m_nID;
		int nPriority = m_nPriority;
 		delete m_apScene[nPriority][nID];
		m_apScene[nPriority][nID] = NULL;
 		m_nNumAll--;
	}
}
