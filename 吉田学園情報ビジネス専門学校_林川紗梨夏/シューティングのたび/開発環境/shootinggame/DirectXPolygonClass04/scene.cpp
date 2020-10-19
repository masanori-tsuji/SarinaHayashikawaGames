//=============================================================================
//
// �V�[������ [scene.cpp]
// Author : �ѐ�ї���
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "renderer.h"
#include "scene2D.h"
#include "manager.h"

//*****************************************************************************
// �ÓI�����o�ϐ�������
//*****************************************************************************
CScene * CScene::m_apScene[PRIORITY_MAX][MAX_POLYGON] = {};
int CScene::m_nNumAll = 0;
//=============================================================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//=============================================================================
CScene::~CScene()
{
}

//=============================================================================
// �X�V�����i�S�ẴV�[���j
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
// �`�揈���i�S�ẴV�[���j
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
// ��������i�S�ẴV�[���j
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
// �������
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
