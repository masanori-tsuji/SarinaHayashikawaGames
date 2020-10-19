//=============================================================================
//
// �v���C���[�̏��� [player.cpp]
// Author : �ѐ�ї���
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "explosion.h"
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "bomb.h"
#include "item.h"
#include <time.h>
#include "enemy.h"
#include "boss.h"
#include "zako.h"

//*****************************************************************************
// �ÓI�����o�ϐ�������
//*****************************************************************************
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;   //�e�N�X�`���̃|�C���^

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CExplosion::CExplosion(int nPriority): CScene2D(nPriority)
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_RandColorR = 255;
	m_RandColorG = 255;
	m_RandColorB = 255;
	m_fLength = 0.0f;		// �Ίp���̒���
	m_fAngle = 0.0f;		// �Ίp���̊p�x
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// ����
	m_fScale = 0.0f;		// �g��
		
	m_nPatternAnim = 1;
	m_nCountAnim = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CExplosion::~CExplosion()
{
}

//=============================================================================
// ��������
//=============================================================================
CExplosion * CExplosion::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, TYPE type)
{
	CExplosion *pExplosion;
	
	pExplosion = new CExplosion;
	pExplosion->Init(pos, fSizeWidth, fSizeHeight,type);

	return pExplosion;
}
//=============================================================================
// �e�N�X�`�����[�h
//=============================================================================
HRESULT CExplosion::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EXPLOSION, &m_pTexture);
	return S_OK;
}
//=============================================================================
// �e�N�X�`���A�����[�h
//=============================================================================
void CExplosion::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, TYPE type)
{
	CScene2D::Init(pos, fSizeWidth, fSizeHeight);

	SetObjType(CScene::OBJTYPE_NONE);
	//�ʒu�̃Z�b�g
	SetPosition(pos);

	// �Ίp���A�p�x�̐ݒ�
	m_fLength = sqrtf((float)(((fSizeWidth / 2) * (fSizeWidth / 2)) + ((fSizeHeight / 2) * (fSizeHeight / 2))));
	m_fAngle = atan2f((fSizeHeight / 2), (fSizeWidth / 2));
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;
	m_fAddScale = 0.01f;

	m_Type = type;

	switch (m_Type)
	{
	case TYPE_NORMAL:
		m_nPatternAnim = 0;
		break;
	case TYPE_RESPAWN:
		m_nPatternAnim = 7;
		break;
	case TYPE_BOMB:
		m_nPatternAnim = 6;
	default:
		break;
	}

	//�����o�ϐ��ɃT�C�Y��������
	m_fSizeWidth = fSizeWidth;
	m_fSizeHeight = fSizeHeight;

	//���_���W�̐ݒ�
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

	//���_���W�̃Z�b�g
	SetVtxPosition(m_vPos);

	int nRandSeed = 0;//�����_���V�[�h�l�i���Ԗ��ύX�ł͂Ȃ��N���b�N���ύX�ɂ��邽�߁j
	nRandSeed = rand() % 255;
	srand((unsigned int)time(NULL)*nRandSeed);

	//�����_���ȐF�̐��l�擾
	m_RandColorR = rand() % 255;
	m_RandColorG = rand() % 255;
	m_RandColorB = rand() % 255;

	//�F�̃Z�b�g
	SetColor(D3DCOLOR_RGBA(m_RandColorR, m_RandColorG, m_RandColorB, 255));

	//�e�N�X�`�����W�̃Z�b�g
	SetTex(
		m_nPatternAnim * 0.125f,
		0.0f,
		m_nPatternAnim * 0.125f + 0.125f,
		1.0f);

    //�e�N�X�`���̊��蓖��
	BindTexture(m_pTexture);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CExplosion::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CExplosion::Update(void)
{
	CScene2D::Update();

	D3DXVECTOR3 pos = CScene2D::GetPosition();

	SetPosition(pos);

	//�F�̃Z�b�g
	SetColor(D3DCOLOR_RGBA(m_RandColorR, m_RandColorG, m_RandColorB, 255));

	switch (m_Type)
	{
	case TYPE_NORMAL:
		if (m_nCountAnim >= 5)
		{
			m_nCountAnim = 0;
			m_nPatternAnim++;
		}

		if (m_nPatternAnim > 7)
		{
			Uninit();
			return;
		}

		break;
	case TYPE_RESPAWN:
		if (m_nCountAnim >= 5)//�t���[�����t�ɍĐ�����
		{
			m_nCountAnim = 0;
			m_nPatternAnim--;
		}
		if (m_nPatternAnim <= 0)
		{
			Uninit();
			return;
		}
		break;
	case TYPE_BOMB:
	
			m_nPatternAnim = 6;
			m_fScale += 0.1f;
			m_fSizeWidth += 20.0f;
			m_fSizeHeight += 20.0f;
			
				for (int nCnt = 0; nCnt < GetNumAll(); nCnt++)
				{
					CScene * pScene = GetScene(4, nCnt);
					if (pScene != NULL)
					{
						OBJTYPE objType;
						objType = pScene->GetObjType();
						if (objType == OBJTYPE_BULLET_ENEMY)
						{
							D3DXVECTOR3 BulletPos = pScene->GetPosition();
							//if (pos.x - (m_fSizeWidth / 2) <= BulletPos.x - (30.0f / 2) &&
							//	pos.x + (m_fSizeWidth / 2) > BulletPos.x + (30.0f / 2) &&
							//	pos.y - (m_fSizeHeight / 2) <= BulletPos.y - (30.0f / 2) &&
							//	pos.y + (m_fSizeHeight / 2) > BulletPos.y + (30.0f / 2))
							//{
							//	pScene->Uninit();
							//	CExplosion::Create(BulletPos, EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_NORMAL);
							//	CItem::Create(BulletPos, 20, 20, CItem::TYPE_SCORE);
							//	/*break;*/
							//}

							float posX = pos.x - BulletPos.x;
							float posY = pos.y - BulletPos.y;
							float Rd = float(sqrt(posX * posX + posY * posY)); //�|���S���P�̈ʒu����|���S���Q�܂ł̋���

							if (Rd <= m_fSizeWidth / 2 + 15.0f)//�|���S���P�ƃ|���S���Q�̔��a�̍��v���ʒu���������ƏՓ�
							{
									pScene->Uninit();
									CExplosion::Create(BulletPos, EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_NORMAL);
									CItem::Create(BulletPos, 20, 20, CItem::TYPE_SCORE);
							}
						}
					}
				}
			
			if (m_nCountAnim >= 100)
			{
				Uninit();
				return;
			}
	
		break;

	default:
		break;
	}

	m_nCountAnim++;

	//���_���W�̐ݒ�
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

	//���_���W�̃Z�b�g
	SetVtxPosition(m_vPos);

	//�e�N�X�`�����W�̃Z�b�g
	SetTex(
		m_nPatternAnim * 0.125f,
		0.0f,
		m_nPatternAnim * 0.125f + 0.125f,
		1.0f);
	

}
//=============================================================================
// �`�揈��
//=============================================================================
void CExplosion::Draw(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	CScene2D::Draw();
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}