//=============================================================================
//
// �G�̏��� [effect.cpp]
// Author : �ѐ�ї���
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "effect.h"
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "explosion.h"
#include <time.h>
#include "sound.h"

//*****************************************************************************
// �ÓI�����o�ϐ�������
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEffect::m_pTexture[TYPE_MAX] = {};   //�e�N�X�`���̃|�C���^

D3DCOLOR CEffect::m_aColor[COLOR_MAX] = //�F�p�����[�^
{
	D3DCOLOR_RGBA(255,0,0,255),	//��
	D3DCOLOR_RGBA(255,50,0,255),// �I�����W
	D3DCOLOR_RGBA(255,255,0,255),// ���F
	D3DCOLOR_RGBA(0,255,0,255),//��
	D3DCOLOR_RGBA(0,0,255,255),//��
	D3DCOLOR_RGBA(50,0,255,255),//��
	D3DCOLOR_RGBA(255,0,255,255),//�s���N
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffect::CEffect(int nPriority) : CScene2D(nPriority)
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	m_fLength = 0.0f;		// �Ίp���̒���
	m_fAngle = 0.0f;		// �Ίp���̊p�x
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// ����
	m_fScale = 0.0f;		// �g��
	m_fAddScale = 0.0f;	// �g����Z�l
	m_Color = D3DCOLOR_RGBA(255, 255, 255, 255);

	m_TargetDistance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ڕW�܂ł̋���
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ڕW�n�_
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���

	m_RandColorR = 255;
	m_RandColorG = 255;
	m_RandColorB = 255;
	m_nLife = 0;
	m_nAlpha = 1.0f;
	m_ColorNum = 0;

	m_nPatternAnim = 1;
	m_nCountAnim = 0;
	m_nCountBullet = 0;
	m_nType = TYPE_NONE;
	m_nCountDraw = 0;

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{
}

//=============================================================================
// ��������
//=============================================================================
CEffect * CEffect::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, D3DXVECTOR3 move, TYPE Type, int col, int nLife)
{
	CEffect *pEffect;
	pEffect = new CEffect;
	pEffect->Init(pos, fSizeWidth, fSizeHeight, move, Type, col,nLife);
	return pEffect;
}
//=============================================================================
// �e�N�X�`�����[�h
//=============================================================================
HRESULT CEffect::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET, &m_pTexture[TYPE_001]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET, &m_pTexture[TYPE_002]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET, &m_pTexture[TYPE_003]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET, &m_pTexture[TYPE_004]);
	return S_OK;
}
//=============================================================================
// �e�N�X�`���A�����[�h
//=============================================================================
void CEffect::Unload(void)
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

//=============================================================================
// ����������
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, D3DXVECTOR3 move, TYPE Type, int col, int nLife)
{
	CScene2D::Init(pos, fSizeWidth, fSizeHeight);

	SetObjType(CScene::OBJTYPE_NONE);
	
	m_nType = Type;
	m_Move = move;
	// �Ίp���A�p�x�̐ݒ�
	m_fLength = sqrtf((float)(((fSizeWidth / 2) * (fSizeWidth / 2)) + ((fSizeHeight / 2) * (fSizeHeight / 2))));
	m_fAngle = atan2f((fSizeHeight / 2), (fSizeWidth / 2));
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;
	m_fAddScale = 0.01f;
	m_ColorNum = col;
	m_Color = m_aColor[col];
	m_nLife = nLife;
	//�����_���V�[�h�l
	int nRandSeed = rand() % 255;
	srand((unsigned int)time(NULL)*nRandSeed);

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

	// �|���S���̈ʒu��ݒ�
	CScene2D::SetPosition(pos);

	if (m_nType == TYPE_001)
	{
		int nRandSeed = rand() % 720;
		srand((int)nRandSeed);
	/*	int Rand = 0 + (int)(rand() * (720 - 0 + 1.0) / (1.0 + RAND_MAX));*/
		int nRand = rand() % 720;
		m_rot.y = nRand;
		m_Move = D3DXVECTOR3(cosf(m_rot.y)*5.0f, sinf(m_rot.y)*5.0f, 0.0f);
	}


	//�F�̃Z�b�g
	SetColor(m_Color);

	//�e�N�X�`�����W�̃Z�b�g
	SetTex(
		0.0f,
		0.0f,
		1.0f,
		1.0f);

	//�e�N�X�`���̃o�C���h
	BindTexture(m_pTexture[m_nType]);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffect::Uninit(void)
{

	////�ʒu���擾
	//D3DXVECTOR3 pos = CScene2D::GetPosition();
	/*CExplosion::Create(pos, TEXTURE_WIDTH, TEXTURE_HEIGHT);*/
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffect::Update(void)
{
	CScene2D::Update();
	//�T�E���h
	CSound * pSound = CManager::GetSound();

	//�ʒu���擾
	D3DXVECTOR3 pos = CScene2D::GetPosition();

	if (m_nType == TYPE_001)
	{
		//�ʒu�X�V
		pos += m_Move;
	}
	

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

	m_nCountAnim++;
	CScene2D::SetPosition(pos);

	//if (m_nType == TYPE_003)
	//{
	//	// �g�哧��
	//		m_fScale -= 0.2f;
	//		m_nAlpha -= 0.2f;
	////�F�̃Z�b�g
	//SetColor(D3DXCOLOR(m_Color.r, m_Color.g, m_Color.b, m_nAlpha));
	//	//��莞�ԂŔj��
	//	if (m_nCountAnim >= 5)
	//	{
	//		Uninit();
	//		return;
	//	}
	//}
	//if (m_nType == TYPE_004)
	//{
	//	// �g�哧��
	//	m_fScale -= 0.02f;
	//	m_nAlpha -= 0.02f;
	//	//�F�̃Z�b�g
	//	SetColor(D3DXCOLOR(m_Color.r, m_Color.g, m_Color.b, m_nAlpha));
	//	//��莞�ԂŔj��
	//	if (m_nCountAnim >= 50)
	//	{
	//		Uninit();
	//		return;
	//	}
	//}

	if (m_nType == TYPE_001)
	{

		m_nAlpha -= 1.0f / m_nLife;
	}
	else
	{
		// �g�哧��
		m_fScale -= 1.0f / m_nLife;
		m_nAlpha -= 1.0f / m_nLife;
	}

	//�F�̃Z�b�g
	SetColor(D3DXCOLOR(m_Color.r, m_Color.g, m_Color.b, m_nAlpha));
	m_nLife--;
		//��莞�ԂŔj��
		if (m_nLife < 0)
		{
			Uninit();
			return;
		}
	

	if (pos.y < 0.0f ||
		pos.y > FIELD_HEIGHT ||
		pos.x < 0.0f ||
		pos.x > FIELD_WIDTH)
	{
		/*CExplosion::Create(pos, EXPLOSION_SIZE, EXPLOSION_SIZE);
		pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);*/
		Uninit();
		return;
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void CEffect::Draw(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		CScene2D::Draw();
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		/*m_nCountDraw = 0;*/

}