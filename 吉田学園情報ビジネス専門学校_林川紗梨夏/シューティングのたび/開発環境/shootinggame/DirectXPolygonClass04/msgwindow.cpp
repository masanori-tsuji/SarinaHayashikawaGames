//=============================================================================
//
// ���b�Z�[�W�E�B���h�E�̏��� [msgwindow.cpp]
// Author : �ѐ�ї���
//
//=============================================================================

//
// �w�b�_�t�@�C���̃C���N���[�h
//
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "msgwindow.h"


LPDIRECT3DTEXTURE9 CMsgWindow::m_apTexture[TYPE_MAX] = {};

//=============================================================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//=============================================================================
CMsgWindow::~CMsgWindow()
{

}
//=============================================================================
// �e�N�X�`�����[�h
//=============================================================================
HRESULT CMsgWindow::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MSG001, &m_apTexture[TYPE_001]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MSG002, &m_apTexture[TYPE_002]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MSG003, &m_apTexture[TYPE_003]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MSG004, &m_apTexture[TYPE_004]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MSG005, &m_apTexture[TYPE_005]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_MSG006, &m_apTexture[TYPE_006]);

	return S_OK;
}
//=============================================================================
// �e�N�X�`���A�����[�h
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

		//�����t���[��

		m_apScene2D[TYPE_001] = new CScene2D(5);
		m_apScene2D[TYPE_001]->Init(pos, 662, 286);
		m_apScene2D[TYPE_001]->BindTexture(m_apTexture[TYPE_001]);

		//�����o�ϐ��ɃT�C�Y��������
		m_fSizeWidth = fSizeWidth;
		m_fSizeHeight = fSizeHeight;

		
		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (-286 / 2), aaa);
		m_vPos[1] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (-286 / 2), aaa);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (286 / 2), aaa);
		m_vPos[3] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (286 / 2), aaa);

		//���_���W�̃Z�b�g
		m_apScene2D[TYPE_001]->SetVtxPosition(m_vPos);

		//�F�̃Z�b�g
		m_apScene2D[TYPE_001]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_001]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);


		////��

		m_apScene2D[TYPE_002] = new CScene2D(5);
		m_apScene2D[TYPE_002]->Init(pos, 570, 418);
		m_apScene2D[TYPE_002]->BindTexture(m_apTexture[TYPE_002]);

		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(pos.x + (-CHARA_SIZE_WIDTH / 2) + 200.0f, pos.y + (-CHARA_SIZE_HEIGHT / 2) + 55.0f, 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (CHARA_SIZE_WIDTH / 2) + 200.0f, pos.y + (-CHARA_SIZE_HEIGHT / 2) + 55.0f, 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-CHARA_SIZE_WIDTH / 2) + 200.0f, pos.y + (CHARA_SIZE_HEIGHT / 2) + 55.0f, 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (CHARA_SIZE_WIDTH / 2) + 200.0f, pos.y + (CHARA_SIZE_HEIGHT / 2) + 55.0f, 0.0f);

		//���_���W�̃Z�b�g
		m_apScene2D[TYPE_002]->SetVtxPosition(m_vPos);

		//�F�̃Z�b�g
		m_apScene2D[TYPE_002]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_002]->SetTex(
			m_nPatternAnim * 0.2f,
			0.0f,
			m_nPatternAnim * 0.2f + 0.2f,
			1.0f);

		////��t���[��

		m_apScene2D[TYPE_003] = new CScene2D(5);
		m_apScene2D[TYPE_003]->Init(pos, 662, 286);
		m_apScene2D[TYPE_003]->BindTexture(m_apTexture[TYPE_003]);



		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (-286 / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (-286 / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (286 / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (286 / 2), 0.0f);

		//���_���W�̃Z�b�g
		m_apScene2D[TYPE_003]->SetVtxPosition(m_vPos);

		//�F�̃Z�b�g
		m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_003]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);

		break;
	case CHARA_002:

		//�����t���[��

		m_apScene2D[TYPE_001] = new CScene2D(5);
		m_apScene2D[TYPE_001]->Init(pos, 662, 286);
		m_apScene2D[TYPE_001]->BindTexture(m_apTexture[TYPE_004]);

		//�����o�ϐ��ɃT�C�Y��������
		m_fSizeWidth = fSizeWidth;
		m_fSizeHeight = fSizeHeight;

		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (-286 / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (-286 / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (286 / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (286 / 2), 0.0f);

		//���_���W�̃Z�b�g
		m_apScene2D[TYPE_001]->SetVtxPosition(m_vPos);

		//�F�̃Z�b�g
		m_apScene2D[TYPE_001]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_001]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);


		////��

		m_apScene2D[TYPE_002] = new CScene2D(5);
		m_apScene2D[TYPE_002]->Init(pos, 570, 418);
		m_apScene2D[TYPE_002]->BindTexture(m_apTexture[TYPE_005]);

		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(pos.x + (-CHARA_SIZE_WIDTH / 2) - 200.0f, pos.y + (-CHARA_SIZE_HEIGHT02 / 2) + 55.0f, 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (CHARA_SIZE_WIDTH / 2) - 200.0f, pos.y + (-CHARA_SIZE_HEIGHT02 / 2) + 55.0f, 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-CHARA_SIZE_WIDTH / 2) - 200.0f, pos.y + (CHARA_SIZE_HEIGHT02 / 2) + 55.0f, 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (CHARA_SIZE_WIDTH / 2) - 200.0f, pos.y + (CHARA_SIZE_HEIGHT02 / 2) + 55.0f, 0.0f);

		//���_���W�̃Z�b�g
		m_apScene2D[TYPE_002]->SetVtxPosition(m_vPos);

		//�F�̃Z�b�g
		m_apScene2D[TYPE_002]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_002]->SetTex(
			m_nPatternAnim * 0.2f,
			0.0f,
			m_nPatternAnim * 0.2f + 0.2f,
			1.0f);

		////��t���[��

		m_apScene2D[TYPE_003] = new CScene2D(5);
		m_apScene2D[TYPE_003]->Init(pos, 662, 286);
		m_apScene2D[TYPE_003]->BindTexture(m_apTexture[TYPE_006]);



		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (-286 / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (-286 / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(pos.x + (-662 / 2), pos.y + (286 / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(pos.x + (662 / 2), pos.y + (286 / 2), 0.0f);

		//���_���W�̃Z�b�g
		m_apScene2D[TYPE_003]->SetVtxPosition(m_vPos);

		//�F�̃Z�b�g
		m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//�e�N�X�`�����W�̃Z�b�g
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
// �I������
//=============================================================================
void CMsgWindow::Uninit(void)
{
	
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CMsgWindow::Update(void)
{
	//�����t���[��
	float aaa = -1.0f;
	switch (m_chara)
	{
	case CHARA_001:

	m_apScene2D[TYPE_001]->Update();
	
	// ���_���W��ݒ�
	m_vPos[0] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (-286 / 2), 0.0f);
	m_vPos[1] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (-286 / 2), 0.0f);
	m_vPos[2] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (286 / 2), 0.0f);
	m_vPos[3] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (286 / 2), 0.0f);

	//���_���W�̃Z�b�g
	m_apScene2D[TYPE_001]->SetVtxPosition(m_vPos);

	//�F�̃Z�b�g
	m_apScene2D[TYPE_001]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	//�e�N�X�`�����W�̃Z�b�g
	m_apScene2D[TYPE_001]->SetTex(
		0.0f,
		0.0f,
		1.0f,
		1.0f);


	//��

	// ���_���W��ݒ�
	m_vPos[0] = D3DXVECTOR3(m_pos.x + (-CHARA_SIZE_WIDTH / 2) + 200.0f, m_pos.y + (-CHARA_SIZE_HEIGHT / 2) + 55.0f, aaa);
	m_vPos[1] = D3DXVECTOR3(m_pos.x + (CHARA_SIZE_WIDTH / 2) + 200.0f, m_pos.y + (-CHARA_SIZE_HEIGHT / 2) + 55.0f, aaa);
	m_vPos[2] = D3DXVECTOR3(m_pos.x + (-CHARA_SIZE_WIDTH / 2) + 200.0f, m_pos.y + (CHARA_SIZE_HEIGHT / 2) + 55.0f, aaa);
	m_vPos[3] = D3DXVECTOR3(m_pos.x + (CHARA_SIZE_WIDTH / 2) + 200.0f, m_pos.y + (CHARA_SIZE_HEIGHT / 2) + 55.0f, aaa);

	m_apScene2D[TYPE_002]->Update();

	//���_���W�̃Z�b�g
	m_apScene2D[TYPE_002]->SetVtxPosition(m_vPos);

	//�F�̃Z�b�g
	m_apScene2D[TYPE_002]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 220));

	//�e�N�X�`�����W�̃Z�b�g
	m_apScene2D[TYPE_002]->SetTex(
		m_nPatternAnim * 0.2f,
		0.0f,
		m_nPatternAnim * 0.2f + 0.2f,
		1.0f);

	////��t���[��

	m_apScene2D[TYPE_003]->Update();

	// ���_���W��ݒ�
	m_vPos[0] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (-286 / 2), 0.0f);
	m_vPos[1] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (-286 / 2), 0.0f);
	m_vPos[2] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (286 / 2), 0.0f);
	m_vPos[3] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (286 / 2), 0.0f);

	//���_���W�̃Z�b�g
	m_apScene2D[TYPE_003]->SetVtxPosition(m_vPos);

	//�F�̃Z�b�g
	m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(0, 230, 200, 255));

	//�e�N�X�`�����W�̃Z�b�g
	m_apScene2D[TYPE_003]->SetTex(
		0.0f,
		0.0f,
		1.0f,
		1.0f);

	if (m_bHitUi == true)
	{
		//�F�̃Z�b�g
		m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(255, 0, 0, 255));

		m_apScene2D[TYPE_002]->SetTex(
			4 * 0.2f,
			0.0f,
			4 * 0.2f + 0.2f,
			1.0f);
	}
	else
	{
		//�F�̃Z�b�g
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

		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (-286 / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (-286 / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (286 / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (286 / 2), 0.0f);

		//���_���W�̃Z�b�g
		m_apScene2D[TYPE_001]->SetVtxPosition(m_vPos);

		//�F�̃Z�b�g
		m_apScene2D[TYPE_001]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_001]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);


		//��

		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(m_pos.x + (-CHARA_SIZE_WIDTH / 2) - 200.0f, m_pos.y + (-CHARA_SIZE_HEIGHT02 / 2) - 20.0f, 0.0f);
		m_vPos[1] = D3DXVECTOR3(m_pos.x + (CHARA_SIZE_WIDTH / 2) - 200.0f, m_pos.y + (-CHARA_SIZE_HEIGHT02 / 2) - 20.0f, 0.0f);
		m_vPos[2] = D3DXVECTOR3(m_pos.x + (-CHARA_SIZE_WIDTH / 2) - 200.0f, m_pos.y + (CHARA_SIZE_HEIGHT02 / 2) - 20.0f, 0.0f);
		m_vPos[3] = D3DXVECTOR3(m_pos.x + (CHARA_SIZE_WIDTH / 2) - 200.0f, m_pos.y + (CHARA_SIZE_HEIGHT02 / 2) - 20.0f, 0.0f);

		m_apScene2D[TYPE_002]->Update();

		//���_���W�̃Z�b�g
		m_apScene2D[TYPE_002]->SetVtxPosition(m_vPos);

		//�F�̃Z�b�g
		m_apScene2D[TYPE_002]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 220));

		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_002]->SetTex(
			m_nPatternAnim * 0.2f,
			0.0f,
			m_nPatternAnim * 0.2f + 0.2f,
			1.0f);

		////��t���[��

		m_apScene2D[TYPE_003]->Update();

		// ���_���W��ݒ�
		m_vPos[0] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (-286 / 2), 0.0f);
		m_vPos[1] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (-286 / 2), 0.0f);
		m_vPos[2] = D3DXVECTOR3(m_pos.x + (-662 / 2), m_pos.y + (286 / 2), 0.0f);
		m_vPos[3] = D3DXVECTOR3(m_pos.x + (662 / 2), m_pos.y + (286 / 2), 0.0f);

		//���_���W�̃Z�b�g
		m_apScene2D[TYPE_003]->SetVtxPosition(m_vPos);

		//�F�̃Z�b�g
		m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(0, 230, 200, 255));

		//�e�N�X�`�����W�̃Z�b�g
		m_apScene2D[TYPE_003]->SetTex(
			0.0f,
			0.0f,
			1.0f,
			1.0f);

		if (m_bHitUi == true)
		{
			//�F�̃Z�b�g
			m_apScene2D[TYPE_003]->SetColor(D3DCOLOR_RGBA(255, 0, 0, 255));

			m_apScene2D[TYPE_002]->SetTex(
				2 * 0.25f,
				0.0f,
				2 * 0.25f + 0.25f,
				1.0f);
		}
		else
		{
			//�F�̃Z�b�g
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
// �`�揈��
//=============================================================================
void CMsgWindow::Draw(void)
{

}
//=============================================================================
// �ʒu�Z�b�g
//=============================================================================
void CMsgWindow::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//=============================================================================
// �L�����N�^�̕\��Z�b�g
//=============================================================================
void CMsgWindow::SetChara(CTYPE type)
{
	m_chara = type;
}
//=============================================================================
// �q�b�g�����Ƃ��̕\������
//=============================================================================
void CMsgWindow::HitMsgUi(bool bHit)
{
	m_bHitUi = bHit;

}
//=============================================================================
// �t���[���̕\���ύX
//=============================================================================
void CMsgWindow::SetMsgUi( int UiIndex)
{
	m_nPatternAnim = UiIndex;
}
