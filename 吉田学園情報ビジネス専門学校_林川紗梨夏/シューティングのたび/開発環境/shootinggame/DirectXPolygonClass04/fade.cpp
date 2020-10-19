//---------------------------------------------------------------
//	fade�̏���
//	fade.cpp
//	Authur �ѐ�ї���
//---------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "fade.h"
#include "star.h"
#include "text.h"
//--------------------------------------------------------
//�R���X�g���N�^
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
//�f�X�g���N�^
//--------------------------------------------------------
CFade::~CFade()
{
}
//--------------------------------------------------------
//����
//--------------------------------------------------------
CFade * CFade::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int sizeX, int sizeY, CManager::MODE mode)
{
	CFade * pFade;
	//�������m��
	pFade = new CFade;
	//������
	pFade->Init(pos, rot, sizeX, sizeY, mode);
	return pFade;
}
//--------------------------------------------------------
//������
//--------------------------------------------------------
HRESULT CFade::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int sizeX, int sizeY, CManager::MODE mode)
{
	//������
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	//���_����ݒ�
	VERTEX_2D *pVtx;
	m_pos = pos;
	m_nSizeX = sizeX;
	m_nSizeY = sizeY;
	m_ModeNext = mode;
	m_fade = FADE_IN;
	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + (-m_nSizeX / 2), m_pos.y + (-m_nSizeY / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_nSizeX / 2), m_pos.y + (-m_nSizeY / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + (-m_nSizeX / 2), m_pos.y + (m_nSizeY / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_nSizeX / 2), m_pos.y + (m_nSizeY / 2), 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ� 
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();
	return S_OK;
}
//--------------------------------------------------------
//�I��
//--------------------------------------------------------
void CFade::Uninit(void)
{
	//�I��
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}
//--------------------------------------------------------
//�X�V
//--------------------------------------------------------
void CFade::Update(void)
{
	//���_����ݒ�
	VERTEX_2D *pVtx;
	//���[�h
	m_ModeNext = CManager::GetMode();
	//�X�V
	if (m_fade != FADE_NONE)
	{
		//�t�F�[�h�C��
		if (m_fade == FADE_IN)
		{
			//a�l�����Z
			m_colorFade.a += m_fFadelate;
			//��ʂ������Ȃ�����
			if (m_colorFade.a >= 1.0f)
			{
				//�^�C�g���ɑJ��
				CManager::SetMode(m_ModeNext);
				//�t�F�[�h�I������
				m_colorFade.a = 1.0f;
				m_fade = FADE_OUT;
			}
		}
		//�t�F�[�h�A�E�g
		if (m_fade == FADE_OUT)
		{
			//���l�����Z
			m_colorFade.a -= m_fFadelate;
			//��ʂ̓���
			if (m_colorFade.a <= 0.0f)
			{
				//�t�F�[�h�����؂�ւ�
				m_colorFade.a = 0.0f;
				//���[�h�ݒ�
				m_fade = FADE_IN;
				CManager::StopFade();
				

			}
		}
		//���_�o�b�t�@�����b�N
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		//���_�J���[�̐ݒ� 
		pVtx[0].col = m_colorFade;
		pVtx[1].col = m_colorFade;
		pVtx[2].col = m_colorFade;
		pVtx[3].col = m_colorFade;
		//���_�o�b�t�@�̃A�����b�N
		m_pVtxBuff->Unlock();
	}
}
//--------------------------------------------------------
//�`��
//--------------------------------------------------------
void CFade::Draw(void)
{
	//�`��
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
	// �|���S���̕`��
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
//	// �e�N�X�`���̐���
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
//	// ���_�o�b�t�@�̐���
//	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
//		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
//		FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
//		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
//		&m_pVtxBuff,				// ���_�o�b�t�@�ւ̃|�C���^
//		NULL)))						// NULL�ɐݒ�
//	{
//		return E_FAIL;
//	}
//
//	VERTEX_2D *pVtx;
//
//	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
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
//	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	pVtx[0].tex = D3DXVECTOR2(type * 0.5f, 0.0f);
//	pVtx[1].tex = D3DXVECTOR2(type * 0.5f + 0.5f, 0.0f);
//	pVtx[2].tex = D3DXVECTOR2(type * 0.5f, 1.0f);
//	pVtx[3].tex = D3DXVECTOR2(type * 0.5f + 0.5f, 1.0f);
//
//	m_pVtxBuff->Unlock();
//}
