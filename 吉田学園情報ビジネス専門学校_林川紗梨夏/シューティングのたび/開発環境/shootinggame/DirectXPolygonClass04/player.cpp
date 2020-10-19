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
#include "player.h"
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include <time.h>
#include "input.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "explosion.h"
#include "sound.h"
#include "joystick.h"
#include "life.h"
#include "effect.h"
#include "enemy.h"
#include "bomb.h"
#include "msgwindow.h"
#include "continue.h"

//*****************************************************************************
// �ÓI�����o�ϐ�������
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPlayer::m_apTexture[TYPE_MAX] = {};   //�e�N�X�`���̃|�C���^
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority) : CScene2D(nPriority)
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPosTurret[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ړ���
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
	m_fLength = 0.0f;		// �Ίp���̒���
	m_fAngle = 0.0f;		// �Ίp���̊p�x
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// ����
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �ڕW����
	m_nNumContinue = PLAYER_CONTINUE;
	m_fScale = 0.0f;		// �g��
	m_fAddScale = 0.0f;	// �g����Z�l
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
	m_BulletMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�e�̈ړ���

	m_fTurretRot = 0.0f; //�C������
	m_TurretrotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �ق��Ƃ��ڕW����
	m_fTurretLength = 0.0f;		// �Ίp���̒���
	m_fTurretAngle = 0.0f;		// �Ίp���̊p�x
	m_fTurretScale = 0.0f;		// �g��

	m_RandColorR = 255;
	m_RandColorG = 255;
	m_RandColorB = 255;
	m_nLife = 4;
	m_nBomb = 4;
	m_fSpeed = VALUE_MOVE;
	
	m_nPower = 0;
	m_nPatternAnim = 1;
	m_nCountAnim = 0;
	m_nCounterBullet = 1;
	m_CountTime = 0;
	m_nRespawnTime = 0;
	m_CountInvincible = 0;
	m_nExplosionBombCount = 0;
	m_bExplosionBombCount = false;
	m_bInvincible = false;
	m_bShot = false;
	m_bStun = false;

	m_pTurret = NULL;
	
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// ��������
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CPlayer *pPlayer;
	
	pPlayer = new CPlayer;
	pPlayer->Init(pos, fSizeWidth, fSizeHeight);
	
	return pPlayer;
}
//=============================================================================
// �e�N�X�`�����[�h
//=============================================================================
HRESULT CPlayer::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PLAYER, &m_apTexture[TYPE_PLAYER]);
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TURRET, &m_apTexture[TYPE_TURRET]);
	return S_OK;
}
//=============================================================================
// �e�N�X�`���A�����[�h
//=============================================================================
void CPlayer::Unload(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apTexture != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// ���X�|�[��
//=============================================================================
void CPlayer::Respawn(void)
{
	//�F�̃Z�b�g
	SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
	m_pTurret->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
	SetPosition(D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 2 + 200.0f, 0.0f));
	m_bInvincible = true;
	m_bStun = true;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CScene2D::Init(pos, fSizeWidth, fSizeHeight);
	//�{��
	CBomb * pBomb = CGame::GetBomb();
	//���C�t
	CLife * pLife = CGame::GetLife();
	SetObjType(CScene::OBJTYPE_PLAYER);
	// �|���S���̈ʒu��ݒ�
	CScene2D::SetPosition(pos);

	pBomb->SetBomb(m_nBomb);
	pLife->SetLife(m_nLife);
	// �Ίp���A�p�x�̐ݒ�
	m_fLength = sqrtf((float)(((fSizeWidth / 2) * (fSizeWidth / 2)) + ((fSizeHeight / 2) * (fSizeHeight / 2))));
	m_fAngle = atan2f((fSizeHeight / 2), (fSizeWidth / 2));
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 0.8f;
	m_fAddScale = 0.01f;
	//===============================================================
	//�C���̐���
	//===============================================================
    m_pTurret = new CScene2D(4);
	m_pTurret->Init(pos, TURRET_WIDTH, TURRET_HEIGHT);
	m_pTurret->BindTexture(m_apTexture[TYPE_TURRET]);

	// �Ίp���A�p�x�̐ݒ�
	m_fTurretLength = sqrtf((float)(((TURRET_WIDTH / 2) * (TURRET_WIDTH / 2)) + ((TURRET_HEIGHT / 2) * (TURRET_HEIGHT / 2))));
	m_fTurretAngle = atan2f((TURRET_HEIGHT / 2), (TURRET_WIDTH / 2));
	m_fTurretScale = 0.8f;

	//���_���W�̐ݒ�
	m_vPosTurret[0].x = pos.x - cosf(m_fTurretAngle - m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[0].y = pos.y - sinf(m_fTurretAngle - m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[0].z = 0.0f;

	m_vPosTurret[1].x = pos.x + cosf(m_fTurretAngle + m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[1].y = pos.y - sinf(m_fTurretAngle + m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[1].z = 0.0f;

	m_vPosTurret[2].x = pos.x - cosf(m_fTurretAngle + m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[2].y = pos.y + sinf(m_fTurretAngle + m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[2].z = 0.0f;

	m_vPosTurret[3].x = pos.x + cosf(m_fTurretAngle - m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[3].y = pos.y + sinf(m_fTurretAngle - m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[3].z = 0.0f;

	//���_���W�̃Z�b�g
	m_pTurret->SetVtxPosition(m_vPosTurret);
	//���_�J���[�̃Z�b�g
	m_pTurret->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
	//���_�e�N�X�`�̃Z�b�g
	m_pTurret->SetTex(
		0.0f,
		0.0f,
		1.0f,
		1.0f);


	//��]���Ȃ����
#ifdef _NOMAL_POS_

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posPolygon.x + (-POLYGON_SIZE / 2), g_posPolygon.y + (-POLYGON_SIZE / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posPolygon.x + (POLYGON_SIZE / 2), g_posPolygon.y + (-POLYGON_SIZE / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posPolygon.x + (-POLYGON_SIZE / 2), g_posPolygon.y + (POLYGON_SIZE / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posPolygon.x + (POLYGON_SIZE / 2), g_posPolygon.y + (POLYGON_SIZE / 2), 0.0f);


#else

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

#endif

	//�F�̃Z�b�g
	SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
    
	//�e�N�X�`�����W�̃Z�b�g
	SetTex(
		m_nPatternAnim * 0.2f, 
		0.0f,
		m_nPatternAnim * 0.2f + 0.2f,
		1.0f);

	Respawn();
	//�e�N�X�`���̃o�C���h
	BindTexture(m_apTexture[TYPE_PLAYER]);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{

	//m_pTurret->Uninit();
	//m_pTurret = NULL;

	CGame::SetPlayerState(false);
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	CScene2D::Update();

	//���C�t
	CBomb * pBomb = CGame::GetBomb();

	//�ʒu���擾
	m_pos = CScene2D::GetPosition();

	//�T�E���h
	CSound * pSound = CManager::GetSound();

	if (m_bStun == true)//�s���s�\���ǂ���
	{
		for (int nCnt = 0; nCnt < MAX_POLYGON; nCnt++)
		{
			CScene * pScene = GetScene(4, nCnt);
			if (pScene != NULL)
			{
				OBJTYPE objType;
				objType = pScene->GetObjType();
				if (objType == OBJTYPE_BULLET_ENEMY)
				{
					pScene->Uninit();
				}
			}
		}
		m_nRespawnTime++;
		if (m_nRespawnTime == 70)
		{
			CExplosion::Create(m_pos, EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_RESPAWN);
		}
		else if (m_nRespawnTime >= 100)
		{
			m_bStun = false;
			m_nRespawnTime = 0;
			//�F�̃Z�b�g
			SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			CMsgWindow  * pMsg01 = CGame::GetMsgUi01();
			CMsgWindow  * pMsg02 = CGame::GetMsgUi02();
			pMsg01->HitMsgUi(false);
			pMsg02->HitMsgUi(false);
		}
	}
	else//�s���\
	{

#define JS 
//#define KEYBOARD
#ifdef JS
	//�W���C�p�b�h
		CInputJoystick * pInputJoystick = CManager::GetInputJoystick();
		LPDIRECTINPUTDEVICE8 lpDIDevice = CInputJoystick::GetDevice();
		DIJOYSTATE js;
		if (lpDIDevice != NULL)
		{
			lpDIDevice->Poll();
			lpDIDevice->GetDeviceState(sizeof(DIJOYSTATE), &js);
		}
#endif
#if MOUSE

		//�}�E�X���W�擾
		POINT point = CInputMouse::GetMousePoint();

		//�v���C���[�ʒu�Ƀ}�E�X���W����
		pos.x = (float)point.x;
		pos.y = (float)point.y;

		if (CInputMouse::GetMousePress(0))//�N���b�N�ŉ�]
		{
			// ��]
			m_rot.z += D3DX_PI * 0.01f;
			if (m_rot.z >= D3DX_PI)//���̒l�𒴂���Ɖ��Ȃ��Ȃ�̂Ō��Z���Ă���
			{
				m_rot.z -= D3DX_PI * 2.0f;
			}
	}
		// �g��k��
		m_fScale += m_fAddScale;
		if (m_fScale >= 2.0f || m_fScale <= 0.5f)
		{
			m_fAddScale *= -1;
		}
#endif
		
		//�Ȃɂ����ĂȂ��Ƃ�
		m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

#ifdef JS

		//----------------------
		//�ړ�
		//----------------------
		if (lpDIDevice != NULL &&
			js.lRx == 0 &&
			js.lRy == 0
			)//�������Ă��Ȃ��Ƃ�
		{
			m_bShot = false;
			m_nCounterBullet = 9;
			m_fSpeed = VALUE_MOVE;
		}

		//�������Ă��Ȃ��Ƃ��i�L�[�{�[�h�̂݁j
		if (
			lpDIDevice == NULL &&
			CInputKeyboard::GetKeyboardPress(DIK_LEFT) == false &&
			CInputKeyboard::GetKeyboardPress(DIK_RIGHT) == false &&
			CInputKeyboard::GetKeyboardPress(DIK_UP) == false &&
			CInputKeyboard::GetKeyboardPress(DIK_DOWN) == false)
		{
			m_bShot = false;
			m_nCounterBullet = 9;
			m_fSpeed = VALUE_MOVE;
		}

		//�E��Y
		if (lpDIDevice != NULL &&js.lY >= -1000 && js.lY < 0 && js.lX >= 0 && js.lX <= 1000 || CInputKeyboard::GetKeyboardPress(DIK_W) && CInputKeyboard::GetKeyboardPress(DIK_D))
		{
			m_rotDest.z = D3DXToRadian(-45.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * m_fSpeed, sinf(m_rotDest.z) * m_fSpeed, 0.0f);
		}

		//����Y
		if (lpDIDevice != NULL &&js.lY >= -1000 && js.lY < 0 && js.lX <= 0 && js.lX >= -1000 || CInputKeyboard::GetKeyboardPress(DIK_W) && CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_rotDest.z = D3DXToRadian(45.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * -m_fSpeed, sinf(m_rotDest.z) * -m_fSpeed, 0.0f);
		}
		//�E��Y
		if (lpDIDevice != NULL &&js.lY <= 1000 && js.lY > 0 && js.lX >= 0 && js.lX <= 1000 || CInputKeyboard::GetKeyboardPress(DIK_S) && CInputKeyboard::GetKeyboardPress(DIK_D))
		{
			m_rotDest.z = D3DXToRadian(-135.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * -m_fSpeed, sinf(m_rotDest.z) * -m_fSpeed, 0.0f);
		}
		//����Yy
		if (lpDIDevice != NULL &&js.lY <= 1000 && js.lY > 0 && js.lX <= 0 && js.lX >= -1000 || CInputKeyboard::GetKeyboardPress(DIK_S) && CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_rotDest.z = D3DXToRadian(135.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * m_fSpeed, sinf(m_rotDest.z) * m_fSpeed, 0.0f);
		}
		//��
		if (lpDIDevice != NULL &&js.lX >= -1000 && js.lX < 0 && js.lY == 0 || CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_rotDest.z = D3DXToRadian(90.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, 0.0f);
		}
		//�E
		if (lpDIDevice != NULL &&js.lX <= 1000 && js.lX > 0 && js.lY == 0 || CInputKeyboard::GetKeyboardPress(DIK_D))
		{

			m_rotDest.z = D3DXToRadian(270.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, 0.0f);
		}
		//��
		if (lpDIDevice != NULL &&js.lY >= -1000 && js.lY < 0 && js.lX == 0 || CInputKeyboard::GetKeyboardPress(DIK_W))
		{

			m_rotDest.z = D3DXToRadian(0.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, 0.0f);
		}
		//��
		if (lpDIDevice != NULL &&js.lY <= 1000 && js.lY > 0 && js.lX == 0 || CInputKeyboard::GetKeyboardPress(DIK_S))
		{

			m_rotDest.z = D3DXToRadian(180.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, 0.0f);
		}

		//�L�[�{�[�h�̈ړ�
		//��
		if (CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_rotDest.z = D3DXToRadian(90.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, 0.0f);
		}
		//�E
		if (CInputKeyboard::GetKeyboardPress(DIK_D))
		{

			m_rotDest.z = D3DXToRadian(270.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, 0.0f);
		}
		//��
		if (CInputKeyboard::GetKeyboardPress(DIK_W))
		{

			m_rotDest.z = D3DXToRadian(0.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, 0.0f);
		}
		//��
		if (CInputKeyboard::GetKeyboardPress(DIK_S))
		{

			m_rotDest.z = D3DXToRadian(180.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, 0.0f);
		}
		//�E��Y
		if (CInputKeyboard::GetKeyboardPress(DIK_W) && CInputKeyboard::GetKeyboardPress(DIK_D))
		{
			m_rotDest.z = D3DXToRadian(-45.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * m_fSpeed, sinf(m_rotDest.z) * m_fSpeed, 0.0f);
		}

		//����Y
		if (CInputKeyboard::GetKeyboardPress(DIK_W) && CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_rotDest.z = D3DXToRadian(45.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * -m_fSpeed, sinf(m_rotDest.z) * -m_fSpeed, 0.0f);
		}
		//�E��Y
		if (CInputKeyboard::GetKeyboardPress(DIK_S) && CInputKeyboard::GetKeyboardPress(DIK_D))
		{
			m_rotDest.z = D3DXToRadian(-135.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * -m_fSpeed, sinf(m_rotDest.z) * -m_fSpeed, 0.0f);
		}
		//����Yy
		if (CInputKeyboard::GetKeyboardPress(DIK_S) && CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_rotDest.z = D3DXToRadian(135.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * m_fSpeed, sinf(m_rotDest.z) * m_fSpeed, 0.0f);
		}
		//----------------------
		//�p�x�w��V���b�g
		//----------------------
		if (CManager::GetIsTalking() == false)
		{
			//�L�[�{�[�h�̋ʔ���
			//��
			if (CInputKeyboard::GetKeyboardTrigger(DIK_LEFT))
			{

				m_fTurretRot = D3DXToRadian(90.0f);
				m_bShot = true;
			}
			//�E
			if (CInputKeyboard::GetKeyboardTrigger(DIK_RIGHT))
			{

				m_fTurretRot = D3DXToRadian(270.0f);
				m_bShot = true;
			}
			//��
			if (CInputKeyboard::GetKeyboardTrigger(DIK_UP))
			{
				m_TurretrotDest.y = D3DXToRadian(180.0f);
				m_fTurretRot = D3DXToRadian(0.0f);
				m_bShot = true;
			}
			//��
			if (CInputKeyboard::GetKeyboardTrigger(DIK_DOWN))
			{
				m_fTurretRot = D3DXToRadian(0.0f);
				m_fTurretRot = D3DXToRadian(180.0f);
				m_bShot = true;
			}
			//�E��Y
			if (CInputKeyboard::GetKeyboardPress(DIK_UP) && CInputKeyboard::GetKeyboardPress(DIK_RIGHT))
			{
				m_TurretrotDest.y = D3DXToRadian(-135.0f);
				m_fTurretRot = D3DXToRadian(-45.0f);
				m_bShot = true;
			}

			//����Y
			if (CInputKeyboard::GetKeyboardPress(DIK_UP) && CInputKeyboard::GetKeyboardPress(DIK_LEFT))
			{
				m_TurretrotDest.y = D3DXToRadian(135.0f);
				m_fTurretRot = D3DXToRadian(45.0f);
				m_bShot = true;
			}
			//�E��Y
			if (CInputKeyboard::GetKeyboardPress(DIK_DOWN) && CInputKeyboard::GetKeyboardPress(DIK_RIGHT))
			{
				m_TurretrotDest.y = D3DXToRadian(-45.0f);
				m_fTurretRot = D3DXToRadian(-135.0f);
				m_bShot = true;
			}
			//����Yy
			if (CInputKeyboard::GetKeyboardPress(DIK_DOWN) && CInputKeyboard::GetKeyboardPress(DIK_LEFT))
			{
				m_TurretrotDest.y = D3DXToRadian(45.0f);
				m_fTurretRot = D3DXToRadian(135.0f);
				m_bShot = true;
			}

			//�W���C�X�e�B�b�N�̋ʔ���
			//��
			if (lpDIDevice != NULL &&js.lRx == -1000 && js.lRx < 0 && js.lRy == 0)
			{
				
				m_fTurretRot = D3DXToRadian(90.0f);
				m_bShot = true;
			}
			//�E
			if (lpDIDevice != NULL &&js.lRx == 1000 && js.lRx > 0 && js.lRy == 0)
			{
				
				m_fTurretRot = D3DXToRadian(270.0f);
				m_bShot = true;
			}
			//��
			if (lpDIDevice != NULL &&js.lRy == -1000 && js.lRy < 0 && js.lRx == 0)
			{
				m_TurretrotDest.y = D3DXToRadian(180.0f);
				m_fTurretRot = D3DXToRadian(0.0f);
				m_bShot = true;
			}
			//��
			if (lpDIDevice != NULL &&js.lRy == 1000 && js.lRy > 0 && js.lRx == 0)
			{
				m_fTurretRot = D3DXToRadian(0.0f);
				m_fTurretRot = D3DXToRadian(180.0f);
				m_bShot = true;
			}

			if (lpDIDevice != NULL && pInputJoystick->GetJoystickTrigger(JS_LB) || CInputKeyboard::GetKeyboardTrigger(DIK_LSHIFT))//�{��
			{
				//�V���b�g���̍Đ�
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_BOMB);
				ExplosionBomb(m_pos);
			}

			//�ׂ������ˊp�̎w��
			if (lpDIDevice != NULL &&js.lRy > -800 && js.lRy > 0 && js.lRx > -800 && js.lRx < 0 )
			{
				m_fTurretRot = D3DXToRadian(js.lRx / 18 - 180);
				m_bShot = true;
			}
			if (lpDIDevice != NULL &&js.lRy > -800 && js.lRy > 0 && js.lRx < 800 && js.lRx > 0)
			{
				m_fTurretRot = D3DXToRadian(js.lRx / 18 - 180);
				m_bShot = true;
			}
			if (lpDIDevice != NULL &&js.lRy < 800 && js.lRy < 0 && js.lRx > -800 && js.lRx < 0)
			{
				m_fTurretRot = D3DXToRadian(js.lRx / 18 * -1.0f - 360);
				m_bShot = true;
			}
			if (lpDIDevice != NULL &&js.lRy < 800 && js.lRy < 0 && js.lRx < 800 && js.lRx > 0)
			{
				m_fTurretRot = D3DXToRadian(js.lRx / 18 * -1.0f - 360);
				m_bShot = true;
			}

			if (lpDIDevice != NULL &&js.lRx > -800 && js.lRx > 0 && js.lRy > -800 && js.lRy < 0)
			{
				m_fTurretRot = D3DXToRadian(js.lRy / 18 * -1.0f - 90);
				m_bShot = true;
			}
			if (lpDIDevice != NULL &&js.lRx > -800 && js.lRx > 0 && js.lRy < 800 && js.lRy > 0)
			{
				m_fTurretRot = D3DXToRadian(js.lRy / 18 * -1.0f - 90);
				m_bShot = true;
			}
			if (lpDIDevice != NULL &&js.lRx < 800 && js.lRx < 0 && js.lRy > -800 && js.lRy < 0)
			{
				m_fTurretRot = D3DXToRadian(js.lRy / 18 - 270);
				m_bShot = true;
			}
			if (lpDIDevice != NULL &&js.lRx < 800 && js.lRx < 0 && js.lRy < 800 && js.lRy > 0)
			{
				m_fTurretRot = D3DXToRadian(js.lRy / 25 - 270);
				m_bShot = true;
			}

		}
#endif

#ifdef KEYBOARD

		//----------------------
		//�ړ�
		//----------------------
		if (CInputKeyboard::GetKeyboardPress(MAX_KEY) == false)//��
		{
			m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		if (CInputKeyboard::GetKeyboardPress(DIK_A))//��
		{
			m_Move.x = -VALUE_MOVE;
			m_rot.z = D3DXToRadian(90.0f);
		}
		if (CInputKeyboard::GetKeyboardPress(DIK_D))//�E
		{
			m_Move.x = VALUE_MOVE;
			m_rot.z = D3DXToRadian(270.0f);
		}
		if (CInputKeyboard::GetKeyboardPress(DIK_W))//��
		{
			m_Move.y = -VALUE_MOVE;
			m_rot.z = D3DXToRadian(0.0f);
		}
		if (CInputKeyboard::GetKeyboardPress(DIK_S))//��
		{
			m_Move.y = VALUE_MOVE;
			m_rot.z = D3DXToRadian(180.0f);
		}
		//----------------------
		//�p�x�w��V���b�g
		//----------------------

		if (CInputKeyboard::GetKeyboardPress(DIK_LEFT))//��
		{
			m_rot.y = D3DXToRadian(90.0f);
			m_fTurretRot = D3DXToRadian(90.0f);
			m_bShot = true;
		}
		else if (CInputKeyboard::GetKeyboardPress(DIK_RIGHT))//�E
		{
			m_rot.y = D3DXToRadian(270.0f);
			m_fTurretRot = D3DXToRadian(270.0f);
			m_bShot = true;
		}
		else if (CInputKeyboard::GetKeyboardPress(DIK_UP))//��
		{
			m_rot.y = D3DXToRadian(180.0f);
			m_fTurretRot = D3DXToRadian(0.0f);
			m_bShot = true;
		}
		else if (CInputKeyboard::GetKeyboardPress(DIK_DOWN))//��
		{
			m_rot.y = D3DXToRadian(0.0f);
			m_fTurretRot = D3DXToRadian(180.0f);
			m_bShot = true;
		}
		else
		{
			m_bShot = false;
			m_nCounterBullet = 9;
		}

		if (CInputKeyboard::GetKeyboardTrigger(DIK_LSHIFT))//�{��
		{
			pBomb->ExplosionBomb(pos);
		}

#endif
		if (m_bShot == true)
		{
			m_fSpeed = VALUE_MOVE_SLOW;
			m_nCounterBullet++;
		}

		if (m_bExplosionBombCount == true)
		{
			//���邮��e
			if (m_nExplosionBombCount % 4 == 0)
			{

				//�V���b�g���̍Đ�
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
			}
				//�e�̐���
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian((m_nExplosionBombCount * 20) * (360 / 360)))*-20.5f, sinf(D3DXToRadian((m_nExplosionBombCount * 20) * (360 / 360)))*-20.5f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_PINK, CBullet::ATTACK_NORMAL);
				//�e�̐���
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian((m_nExplosionBombCount * 20) * (360 / 360)))*20.5f, sinf(D3DXToRadian((m_nExplosionBombCount * 20) * (360 / 360)))*20.5f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_PINK, CBullet::ATTACK_NORMAL);

			if (m_nExplosionBombCount >= 100)
			{
				m_bExplosionBombCount = false;
				m_nExplosionBombCount = 0;
			}
			m_nExplosionBombCount++;
		}
		//�Ԋu������ăV���b�g
		if (m_nCounterBullet % 2 == 0)
		{
			if (m_nPower < PLAYER_MAXPOWER * 0.33f)//���x��1�̎�
			{
				//�e�̐���
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, sinf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);

			}
			if(m_nPower >= PLAYER_MAXPOWER * 0.33f)//���x���Q
			{
				//�e�̐���
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, sinf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
				//�e�̐���
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * 30.0f, sinf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * 30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);

			}
			if(m_nPower >= PLAYER_MAXPOWER * 0.66f)
			{
				//�e�̐���
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, sinf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
				//�e�̐���
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f + 0.2f) - D3DXToRadian(-90.0f)) * -30.0f, sinf((m_fTurretRot * -1.0f + 0.2f) - D3DXToRadian(-90.0f)) * -30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
				//�e�̐���
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f - 0.2f) - D3DXToRadian(-90.0f)) * -30.0f, sinf((m_fTurretRot * -1.0f - 0.2f) - D3DXToRadian(-90.0f)) * -30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
			}
			if (m_nPower >= PLAYER_MAXPOWER)
			{
				//�e�̐���
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, sinf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
				//�e�̐���
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f + 0.1f) - D3DXToRadian(-90.0f)) * 30.0f, sinf((m_fTurretRot * -1.0f + 0.1f) - D3DXToRadian(-90.0f)) * 30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
				//�e�̐���
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f - 0.1f) - D3DXToRadian(-90.0f)) * 30.0f, sinf((m_fTurretRot * -1.0f - 0.1f) - D3DXToRadian(-90.0f)) * 30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
				//�e�̐���
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * 30.0f, sinf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * 30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
			}


		}
		if (m_nCounterBullet % 10 == 0)
		{
			//�V���b�g���̍Đ�
			pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_PLAYERSHOT);
		}

		//���G���Ԓ�
		if (m_bInvincible == true)
		{
			//�_�ŏ���
			if (m_CountTime >= 10)
			{
				if (m_CountInvincible % 2 == 0)
				{
					//�F�̃Z�b�g
					SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
					//���_�J���[�̃Z�b�g
					m_pTurret->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
				}
				else
				{
					//�F�̃Z�b�g
					SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
					//���_�J���[�̃Z�b�g
					m_pTurret->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
				}
				if (m_CountInvincible >= 20)
				{
					//�F�̃Z�b�g
					SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
					//���_�J���[�̃Z�b�g
					m_pTurret->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
					m_bInvincible = false;
					m_CountInvincible = 0;
				}
				m_CountInvincible++;
				m_CountTime = 0;
			}
			m_CountTime++;
		}

		//��ʊO����
		if (m_pos.x < 0)
		{
			m_pos.x = 0.0f;
		}
		if (m_pos.x > FIELD_WIDTH)
		{
			m_pos.x = FIELD_WIDTH;
		}
		if (m_pos.y < 0)
		{
			m_pos.y = 0.0f;
		}
		if (m_pos.y > FIELD_HEIGHT)
		{
			m_pos.y = FIELD_HEIGHT;
		}

	
		//�ʒu�X�V
		m_pos += m_Move;

		if (m_Move.x != 0.0f || m_Move.y != 0.0f)
		{
			if (m_Move.y <= 0.0f)
			{
				//������
				if (m_bShot == true && m_nCounterBullet > 0)
				{
					//�G�t�F�N�g�����i�G���W���j
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_BLUE, 15);
					//�G�t�F�N�g����
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_BLUE, 15);
					//�G�t�F�N�g�����i�G���W���j
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_PINK, 15);
					//�G�t�F�N�g����
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_PINK, 15);

					//�G�t�F�N�g����
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_004, CEffect::COLOR_BLUE, 20);
					//�G�t�F�N�g����
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_003, CEffect::COLOR_PINK, 20);

				}
				else
				{
					//�G�t�F�N�g�����i�G���W���j
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_RED, 15);
					//�G�t�F�N�g����
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_RED, 15);
					//�G�t�F�N�g�����i�G���W���j
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_ORANGE, 15);
					//�G�t�F�N�g����
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_ORANGE, 15);

					//�G�t�F�N�g����
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_004, CEffect::COLOR_RED, 20);
					//�G�t�F�N�g����
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_003, CEffect::COLOR_ORANGE, 20);
				}
			}
			else //���ړ����Ă�Ƃ��̓G�t�F�N�g�̂炢�ӂ����Ȃ�����
			{
				//������
				if (m_bShot == true && m_nCounterBullet > 0)
				{
					//�G�t�F�N�g�����i�G���W���j
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_BLUE, 5);
					//�G�t�F�N�g����
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_BLUE, 5);
					//�G�t�F�N�g�����i�G���W���j
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_PINK, 5);
					//�G�t�F�N�g����
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_PINK, 5);

					//�G�t�F�N�g����
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_004, CEffect::COLOR_BLUE, 5);
					//�G�t�F�N�g����
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_003, CEffect::COLOR_PINK, 5);

				}
				else
				{
					//�G�t�F�N�g�����i�G���W���j
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_RED, 5);
					//�G�t�F�N�g����
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_RED, 5);
					//�G�t�F�N�g�����i�G���W���j
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_ORANGE, 5);
					//�G�t�F�N�g����
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_ORANGE, 5);

					//�G�t�F�N�g����
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_004, CEffect::COLOR_RED, 5);
					//�G�t�F�N�g����
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_003, CEffect::COLOR_ORANGE, 5);
				}
			}
		}

	}

	//---------------------------
	//���_���W�̐ݒ�
	//---------------------------
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

	//���_���W�Z�b�g
	SetVtxPosition(m_vPos);

	//���_���W�̐ݒ�(�C��)
	m_vPosTurret[0].x = m_pos.x - cosf(m_fTurretAngle - m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[0].y = m_pos.y - sinf(m_fTurretAngle - m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[0].z = 0.0f;

	m_vPosTurret[1].x = m_pos.x + cosf(m_fTurretAngle + m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[1].y = m_pos.y - sinf(m_fTurretAngle + m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[1].z = 0.0f;

	m_vPosTurret[2].x = m_pos.x - cosf(m_fTurretAngle + m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[2].y = m_pos.y + sinf(m_fTurretAngle + m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[2].z = 0.0f;

	m_vPosTurret[3].x = m_pos.x + cosf(m_fTurretAngle - m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[3].y = m_pos.y + sinf(m_fTurretAngle - m_fTurretRot) * m_fTurretLength * m_fTurretScale;
	m_vPosTurret[3].z = 0.0f;

	//���_���W�̃Z�b�g
	m_pTurret->SetVtxPosition(m_vPosTurret);

	SetTex(
		0.0f,
		0.0f,
		1.0f,
		1.0f);

	

	CScene2D::SetPosition(m_pos);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
//���C�t���ǉ�
//=============================================================================
void CPlayer::AddLife(int nLife)
{
	CLife * pLife = CGame::GetLife();
	m_nLife += nLife;
	pLife->SetLife(m_nLife);
}

//=============================================================================
//���C�t�����Z
//=============================================================================
void CPlayer::SubLife(int nLife)
{
	CLife * pLife = CGame::GetLife();
	m_nLife -= nLife;
	pLife->SetLife(m_nLife);

	//�L�����N�^�[UI���_���[�W���[�h�ɂ���
	CMsgWindow  * pMsg01 = CGame::GetMsgUi01();
	CMsgWindow  * pMsg02 = CGame::GetMsgUi02();
	pMsg01->HitMsgUi(true);
	pMsg02->HitMsgUi(true);

	if (m_nLife <= 0)//���񂾂�
	{

		m_nNumContinue--;
		if (m_nNumContinue > 0)
		{
			SubBomb(m_nBomb);
			CGame::SetIsStopUpdateContinue(true);
		}
		else
		{
			CManager::StartFade(CManager::MODE_NAME, D3DCOLOR_RGBA(255, 255, 255, 0), 0.05f);
			Uninit();
			return;
		}
		SubPower(m_nPower / 2);
	}
	else
	{
		SubBomb(m_nBomb);
		AddBomb(4);

		SubPower(m_nPower / 2);
	}
}

//=============================================================================
//�{�����ǉ�
//=============================================================================
void CPlayer::AddBomb(int nBomb)
{
	//�{��
	CBomb * pBomb = CGame::GetBomb();
	if (m_nBomb < MAX_STAR)
	{
		m_nBomb += nBomb;
		pBomb->SetBomb(m_nBomb);
	}
}

//=============================================================================
//�{�������Z
//=============================================================================
void CPlayer::SubBomb(int nBomb)
{
	//�{��
	CBomb * pBomb = CGame::GetBomb();
	if (m_nBomb > 0)
	{
		m_nBomb -= nBomb;
		pBomb->SetBomb(m_nBomb);
	}
}
//=============================================================================
//�p���[�ǉ�
//=============================================================================
void CPlayer::AddPower(int nPow)
{
	if (m_nPower <= PLAYER_MAXPOWER)
	{
		m_nPower += nPow;
	}
	
}

//=============================================================================
//�p���[���Z
//=============================================================================
void CPlayer::SubPower(int nPow)
{
	if (m_nPower > 0)
	{
		m_nPower -= nPow;
	}
}

//=============================================================================
//�{���𔚔j
//=============================================================================
void CPlayer::ExplosionBomb(D3DXVECTOR3 pos)
{
	if (m_nBomb > 0)
	{
		CExplosion::Create(pos, EXPLOSION_SIZE, EXPLOSION_SIZE, CExplosion::TYPE_BOMB);
		SubBomb(1);
		m_bExplosionBombCount = true;
	}
}
