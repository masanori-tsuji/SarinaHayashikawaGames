//=============================================================================
//
// ��UI [star.cpp]
// Author : �ѐ�ї���
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "star.h"

LPDIRECT3DTEXTURE9 CStar::m_pTexture = NULL;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CStar::CStar()
{
	m_pVtxBuff = NULL;
	m_nPatternAnim = TYPE_NOGLOW;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CStar::~CStar()
{
}
//=============================================================================
// ����
//=============================================================================
CStar * CStar::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CStar *pStar;

	pStar = new CStar;
	pStar->Init(pos, fSizeWidth, fSizeHeight);

	return pStar;
}
//=============================================================================
// �e�N�X�`�����[�h
//=============================================================================
HRESULT CStar::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_STAR, &m_pTexture);
	return S_OK;
}
//=============================================================================
// �e�N�X�`���A�����[�h
//=============================================================================
void CStar::Unload(void)
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
HRESULT CStar::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&m_pVtxBuff,				// ���_�o�b�t�@�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (-fSizeHeight / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x + (-fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + (fSizeWidth / 2), pos.y + (fSizeHeight / 2), 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(m_nPatternAnim * 0.5f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_nPatternAnim * 0.5f + 0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_nPatternAnim * 0.5f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_nPatternAnim * 0.5f + 0.5f, 1.0f);

	m_pVtxBuff->Unlock();
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CStar::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void CStar::Update(void)
{
}
//=============================================================================
// �`�揈��
//=============================================================================
void CStar::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	pDevice->SetTexture(0, m_pTexture);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}
//=============================================================================
// ���Z�b�g����
//=============================================================================
void CStar::SetStar(TYPE type)
{

	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(type * 0.5f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(type * 0.5f + 0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(type * 0.5f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(type * 0.5f + 0.5f, 1.0f);

	m_pVtxBuff->Unlock();
}
