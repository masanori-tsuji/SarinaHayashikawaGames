//=============================================================================
//
// zako�G�̏��� [zako.cpp]
// Author : �ѐ�ї���
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "game.h"
#include "zako.h"
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "explosion.h"
#include <time.h>
#include "sound.h"
#include "score.h"
#include "life.h"
#include "item.h"
#include "effect.h"
#include "player.h"

//*****************************************************************************
// �ÓI�����o�ϐ�������
//*****************************************************************************
LPDIRECT3DTEXTURE9 CZako::m_pTexture[TYPE_MAX] = {};   //�e�N�X�`���̃|�C���^
														//=============================================================================
														// �R���X�g���N�^
														//=============================================================================
CZako::CZako(int nPriority) : CScene2D(nPriority)
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_fLength = 0.0f;		// �Ίp���̒���
	m_fAngle = 0.0f;		// �Ίp���̊p�x
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// ����
	m_fScale = 0.0f;		// �g��
	m_fAddScale = 0.0f;	// �g����Z�l

	m_TargetDistance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ڕW�܂ł̋���
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ڕW�n�_
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���

	m_nPatternAnim = 1;
	m_nCountAnim = 0;
	m_nCountBullet = 0;
	m_nType = TYPE_001;
	m_nLife = 20;
	m_nColor = 0;
	m_nConutTime = 0;
	m_fMovetime = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CZako::~CZako()
{
}

//=============================================================================
// ��������
//=============================================================================
CZako * CZako::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeWidth, float fSizeHeight, TYPE Type, D3DXVECTOR3 location[MAX_LOCATION])
{
	CZako *pEnemy;
	pEnemy = new CZako;
	pEnemy->Init(pos, move, fSizeWidth, fSizeHeight, Type, location);
	return pEnemy;
}
//=============================================================================
// �e�N�X�`�����[�h
//=============================================================================
HRESULT CZako::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ENEMY_ZAKO_002, &m_pTexture[TYPE_001]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ENEMY_ZAKO_001, &m_pTexture[TYPE_002]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ENEMY_ZAKO_002, &m_pTexture[TYPE_003]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_ENEMY_ZAKO_002, &m_pTexture[TYPE_004]);
	return S_OK;
}
//=============================================================================
// �e�N�X�`���A�����[�h
//=============================================================================
void CZako::Unload(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_pTexture != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

void CZako::SubLife(int SubLife)
{
	//�X�R�A
	CScore * pScore = CGame::GetScore();
	//�T�E���h
	CSound * pSound = CManager::GetSound();
	m_nLife -= SubLife;

	if (m_nLife <= 0)
	{
		pScore->AddScore(1);
		pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);
		CItem::Create(m_Pos, ITEM_SIZE, ITEM_SIZE, CItem::TYPE_SCORE);
		CExplosion::Create(m_Pos, EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_NORMAL);

		for (int nCount = 0; nCount < 5; nCount++)
		{
			int nRandSeed = rand() % 200 * nCount;
			srand((unsigned int)nRandSeed);
			float RandPosX = float(rand() % 100 + (-100));
			float RandPosY = float(rand() % 300 + (-300));
			CItem::Create(D3DXVECTOR3(m_Pos.x + RandPosX, m_Pos.y + RandPosY, 0.0f), ITEM_SIZE / 3, ITEM_SIZE / 3, CItem::TYPE_SCORE);
		}

		for (int nCount = 0; nCount < 1; nCount++)
		{
			int nRandSeed = rand() % 300 * nCount;
			srand((unsigned int)nRandSeed);
			float RandPosX = float(rand() % 100 + (-100));
			float RandPosY = float(rand() % 300 + (-300));
			CItem::Create(D3DXVECTOR3(m_Pos.x + RandPosX, m_Pos.y + RandPosY, 0.0f), ITEM_SIZE / 2, ITEM_SIZE / 2, CItem::TYPE_POWER);
		}

		Uninit();
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CZako::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fSizeWidth, float fSizeHeight, TYPE Type, D3DXVECTOR3 location[MAX_LOCATION])
{
	CScene2D::Init(pos, fSizeWidth, fSizeHeight);

	for (int nCount = 0; nCount < MAX_LOCATION; nCount++)
	{
		m_location[nCount] = location[nCount];
	}

	m_Move = move;
	SetObjType(CScene::OBJTYPE_ENEMY);
	// �|���S���̈ʒu��ݒ�
	CScene2D::SetPosition(pos);

	m_nType = Type;

	m_Pos = pos;
	// �Ίp���A�p�x�̐ݒ�
	m_fLength = sqrtf((float)(((fSizeWidth / 2) * (fSizeWidth / 2)) + ((fSizeHeight / 2) * (fSizeHeight / 2))));
	m_fAngle = atan2f((fSizeHeight / 2), (fSizeWidth / 2));
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;
	m_fAddScale = 0.01f;

	//�����_���V�[�h�l
	int nRandSeed = rand() % 255;
	srand((unsigned int)time(NULL)*nRandSeed);

	m_TargetPos.x = float(rand() % FIELD_WIDTH);
	m_TargetPos.y = float(rand() % FIELD_HEIGHT);

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

	//���_���W�Z�b�g
	SetVtxPosition(m_vPos);

	//�F�̃Z�b�g
	SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	switch (m_nType)
	{
	case TYPE_001:
		//�e�N�X�`�����W�̃Z�b�g
		SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
		break;
	case TYPE_002:
		//�e�N�X�`�����W�̃Z�b�g
		SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
		break;
	default:
		break;
	}

	//�e�N�X�`���̃o�C���h
	BindTexture(m_pTexture[m_nType]);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CZako::Uninit(void)
{

	////�ʒu���擾
	//D3DXVECTOR3 pos = CScene2D::GetPosition();
	/*CExplosion::Create(pos, TEXTURE_WIDTH, TEXTURE_HEIGHT);*/
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CZako::Update(void)
{
	CScene2D::Update();
	//�T�E���h
	CSound * pSound = CManager::GetSound();
	//���C�t
	CLife * pLife = CGame::GetLife();

	m_nConutTime++;

	//�ʒu���擾
	m_Pos = CScene2D::GetPosition();
	// ��]
	m_rot.z += D3DX_PI * 0.01f;
	if (m_rot.z >= D3DX_PI)//���̒l�𒴂���Ɖ��Ȃ��Ȃ�̂Ō��Z���Ă���
	{
		m_rot.z -= D3DX_PI * 2.0f;
	}

	//�G�t�F�N�g����
	CEffect::Create(m_Pos, ZAKO_SIZE_001_HEIGHT, ZAKO_SIZE_001_HEIGHT, m_Move, CEffect::TYPE_003, CEffect::COLOR_RED,10);

	/*if (m_nConutTime == 200)
	{
		m_Move.y = m_Move.y*-1.0f;
	}*/
	////�ʒu�X�V
	//m_Pos += m_Move;

	//�^�C�����Z
	m_fMovetime += 0.005f;

	//�x�W�F�Ȑ��v�Z
	m_Pos.x = (1.0f - m_fMovetime) * (1.0f - m_fMovetime) * (1.0f - m_fMovetime) * m_location[0].x +
		3.0f * (1.0f - m_fMovetime) *(1.0f - m_fMovetime) * m_fMovetime * m_location[1].x +
		3.0f * (1.0f - m_fMovetime) * m_fMovetime * m_fMovetime * m_location[2].x +
		m_fMovetime * m_fMovetime * m_fMovetime * m_location[3].x;

	m_Pos.y = (1.0f - m_fMovetime) * (1.0f - m_fMovetime) * (1.0f - m_fMovetime) * m_location[0].y +
		3.0f * (1.0f - m_fMovetime) *(1.0f - m_fMovetime) * m_fMovetime * m_location[1].y +
		3.0f * (1.0f - m_fMovetime) * m_fMovetime * m_fMovetime * m_location[2].y +
		m_fMovetime * m_fMovetime * m_fMovetime * m_location[3].y;

	//���_���W�̐ݒ�
	m_vPos[0].x = m_Pos.x - cosf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	m_vPos[0].y = m_Pos.y - sinf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	m_vPos[0].z = 0.0f;

	m_vPos[1].x = m_Pos.x + cosf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	m_vPos[1].y = m_Pos.y - sinf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	m_vPos[1].z = 0.0f;

	m_vPos[2].x = m_Pos.x - cosf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	m_vPos[2].y = m_Pos.y + sinf(m_fAngle + m_rot.z) * m_fLength * m_fScale;
	m_vPos[2].z = 0.0f;

	m_vPos[3].x = m_Pos.x + cosf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	m_vPos[3].y = m_Pos.y + sinf(m_fAngle - m_rot.z) * m_fLength * m_fScale;
	m_vPos[3].z = 0.0f;

	//���_���W�Z�b�g
	SetVtxPosition(m_vPos);

	//�F�̃Z�b�g
	SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	for (int nCnt = 0; nCnt < GetNumAll(); nCnt++)
	{
		CScene * pScene = GetScene(4, nCnt);
		if (pScene != NULL)
		{
			OBJTYPE objType;
			objType = pScene->GetObjType();
			if (objType == OBJTYPE_PLAYER)
			{
				D3DXVECTOR3 playerPos = pScene->GetPosition();

				float posX = m_Pos.x - playerPos.x;
				float posY = m_Pos.y - playerPos.y;
				float Rd = float(sqrt(posX * posX + posY * posY)); //�|���S���P�̈ʒu����|���S���Q�܂ł̋���

				if (Rd <= (BULLET_SIZE) / 2 + 15.0f)//�|���S���P�ƃ|���S���Q�̔��a�̍��v���ʒu���������ƏՓ�
				{
					CPlayer * pPlayer = dynamic_cast<CPlayer*> (pScene);
					if (pPlayer->IsInvincible() == false)
					{
						CExplosion::Create(playerPos, EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_NORMAL);
						pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);
						pPlayer->Respawn();
						pPlayer->SubLife(1);
					}
				}
			}
		}
	}

	//�V���b�g
	switch (m_nType)
	{
		////�R�U�O�x�~�`�ˌ�
	case TYPE_001:
		break;
		////���邮��ˌ�
	case TYPE_002: //�������Ȃ��i�U�R�j
		
		break;
	case TYPE_003://�����o��
		if (m_nCountBullet % 50 == 0)
		{
			CPlayer * pPlayer = CGame::GetPlayer();
			m_TargetPos = pPlayer->GetPosition();

			////�ڕW�܂ł̋������Z�o//����͐������̃v���C���[�Ɍ����Ă̕����w��̂�
			m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);
			m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
			int nRand = rand() % 50;
			m_rot.y += D3DXToRadian(nRand);
			//�e�̐���
			CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y)*4.5f, sinf(m_rot.y)*4.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_NORMAL);
			//�e�̐���
			CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y + 0.1f)*4.5f, sinf(m_rot.y + 0.1f)*4.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_NORMAL);
			//�e�̐���
			CBullet::Create(m_Pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(m_rot.y - 0.1f)*4.5f, sinf(m_rot.y - 0.1f)*4.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_DIAMOND, CEffect::COLOR_GREEN, CBullet::ATTACK_NORMAL);
		}
		break;
	case TYPE_004://�|���f�����O�e
		if (m_nCountBullet % 105 == 0)
		{
			CPlayer * pPlayer = CGame::GetPlayer();
			m_TargetPos = pPlayer->GetPosition();

			////�ڕW�܂ł̋������Z�o//����͐������̃v���C���[�Ɍ����Ă̕����w��̂�
			m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);
			m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
			int nRand = rand() % 50;
			m_rot.y += D3DXToRadian(nRand);
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				//�e�̐���
				CBullet::Create(D3DXVECTOR3(m_Pos.x + 30.0f * cosf(D3DXToRadian(nCnt * (360.0f / 5))), m_Pos.y + 30.0f * sinf(D3DXToRadian(nCnt * (360.0f / 5))), 0.0f), BULLET_SIZE, BULLET_SIZE, D3DXVECTOR3(cosf(m_rot.y)*3.5f, sinf(m_rot.y)*3.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_CIRCLE, CEffect::COLOR_PINK, CBullet::ATTACK_NORMAL);
			}
		}
		if (m_nCountBullet % 202 == 0)
		{
			CPlayer * pPlayer = CGame::GetPlayer();
			m_TargetPos = pPlayer->GetPosition();

			////�ڕW�܂ł̋������Z�o//����͐������̃v���C���[�Ɍ����Ă̕����w��̂�
			m_TargetDistance = D3DXVECTOR3(m_TargetPos.x - m_Pos.x, m_TargetPos.y - m_Pos.y, 0.0f);
			m_rot.y = atan2f(m_TargetDistance.y, m_TargetDistance.x);
			int nRand = rand() % 50;
			m_rot.y += D3DXToRadian(nRand);
			for (int nCnt = 0; nCnt < 10; nCnt++)
			{
				//�e�̐���
				CBullet::Create(D3DXVECTOR3(m_Pos.x + 30.0f * cosf(D3DXToRadian(nCnt * (360.0f / 10))), m_Pos.y + 30.0f * sinf(D3DXToRadian(nCnt * (360.0f / 10))), 0.0f), BULLET_SIZE, BULLET_SIZE, D3DXVECTOR3(cosf(m_rot.y)*3.5f, sinf(m_rot.y)*3.5f, 0.0f), CBullet::TYPE_ENEMY, CBullet::BULLET_CIRCLE, CEffect::COLOR_ORANGE, CBullet::ATTACK_NORMAL);
			}
		}
		break;
	default:
		break;
	}
	m_nCountBullet++;

	switch (m_nType)
	{
	case TYPE_001:
		//�e�N�X�`�����W�̃Z�b�g
		SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
		break;
	case TYPE_002:
		//�e�N�X�`�����W�̃Z�b�g
		SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);
		break;
	default:
		break;
	}

	//switch (m_nType)
	//{
	//case TYPE_001:
	//	//�e�N�X�`���A�j���[�V�����X�V
	//	if (m_nCountAnim >= 8)
	//	{
	//		m_nCountAnim = 0;
	//		m_nPatternAnim++;
	//		if (m_nPatternAnim > 5)
	//		{
	//			m_nPatternAnim = 1;
	//		}
	//	}
	//	m_nCountAnim++;
	//	//�e�N�X�`�����W�̃Z�b�g
	//	SetTex(
	//		m_nPatternAnim * 0.2f,
	//		0.0f,
	//		m_nPatternAnim * 0.2f + 0.2f,
	//		1.0f);
	//	break;
	//case TYPE_002:
	//	//�e�N�X�`�����W�̃Z�b�g
	//	SetTex(
	//		0.0f,
	//		0.0f,
	//		1.0f,
	//		1.0f);
	//	break;
	//default:
	//	break;
	//}

	CScene2D::SetPosition(m_Pos);

	if (m_Pos.y < 0.0f ||
		m_Pos.y > FIELD_HEIGHT ||
		m_Pos.x < 0.0f ||
		m_Pos.x > FIELD_WIDTH)
	{
		Uninit();
		return;
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CZako::Draw(void)
{
	CScene2D::Draw();
}