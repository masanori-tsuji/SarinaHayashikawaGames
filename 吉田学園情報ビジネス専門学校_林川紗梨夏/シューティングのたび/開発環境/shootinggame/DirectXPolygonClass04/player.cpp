//=============================================================================
//
// プレイヤーの処理 [player.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
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
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPlayer::m_apTexture[TYPE_MAX] = {};   //テクスチャのポインタ
//=============================================================================
// コンストラクタ
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
	
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
	m_fLength = 0.0f;		// 対角線の長さ
	m_fAngle = 0.0f;		// 対角線の角度
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 向き
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 目標向き
	m_nNumContinue = PLAYER_CONTINUE;
	m_fScale = 0.0f;		// 拡大
	m_fAddScale = 0.0f;	// 拡大加算値
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
	m_BulletMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//弾の移動量

	m_fTurretRot = 0.0f; //砲塔向き
	m_TurretrotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// ほうとう目標向き
	m_fTurretLength = 0.0f;		// 対角線の長さ
	m_fTurretAngle = 0.0f;		// 対角線の角度
	m_fTurretScale = 0.0f;		// 拡大

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
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CPlayer *pPlayer;
	
	pPlayer = new CPlayer;
	pPlayer->Init(pos, fSizeWidth, fSizeHeight);
	
	return pPlayer;
}
//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CPlayer::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PLAYER, &m_apTexture[TYPE_PLAYER]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TURRET, &m_apTexture[TYPE_TURRET]);
	return S_OK;
}
//=============================================================================
// テクスチャアンロード
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
// リスポーン
//=============================================================================
void CPlayer::Respawn(void)
{
	//色のセット
	SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
	m_pTurret->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
	SetPosition(D3DXVECTOR3(FIELD_WIDTH / 2, FIELD_HEIGHT / 2 + 200.0f, 0.0f));
	m_bInvincible = true;
	m_bStun = true;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight)
{
	CScene2D::Init(pos, fSizeWidth, fSizeHeight);
	//ボム
	CBomb * pBomb = CGame::GetBomb();
	//ライフ
	CLife * pLife = CGame::GetLife();
	SetObjType(CScene::OBJTYPE_PLAYER);
	// ポリゴンの位置を設定
	CScene2D::SetPosition(pos);

	pBomb->SetBomb(m_nBomb);
	pLife->SetLife(m_nLife);
	// 対角線、角度の設定
	m_fLength = sqrtf((float)(((fSizeWidth / 2) * (fSizeWidth / 2)) + ((fSizeHeight / 2) * (fSizeHeight / 2))));
	m_fAngle = atan2f((fSizeHeight / 2), (fSizeWidth / 2));
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 0.8f;
	m_fAddScale = 0.01f;
	//===============================================================
	//砲塔の生成
	//===============================================================
    m_pTurret = new CScene2D(4);
	m_pTurret->Init(pos, TURRET_WIDTH, TURRET_HEIGHT);
	m_pTurret->BindTexture(m_apTexture[TYPE_TURRET]);

	// 対角線、角度の設定
	m_fTurretLength = sqrtf((float)(((TURRET_WIDTH / 2) * (TURRET_WIDTH / 2)) + ((TURRET_HEIGHT / 2) * (TURRET_HEIGHT / 2))));
	m_fTurretAngle = atan2f((TURRET_HEIGHT / 2), (TURRET_WIDTH / 2));
	m_fTurretScale = 0.8f;

	//頂点座標の設定
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

	//頂点座標のセット
	m_pTurret->SetVtxPosition(m_vPosTurret);
	//頂点カラーのセット
	m_pTurret->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
	//頂点テクスチのセット
	m_pTurret->SetTex(
		0.0f,
		0.0f,
		1.0f,
		1.0f);


	//回転しないやつ
#ifdef _NOMAL_POS_

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(g_posPolygon.x + (-POLYGON_SIZE / 2), g_posPolygon.y + (-POLYGON_SIZE / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posPolygon.x + (POLYGON_SIZE / 2), g_posPolygon.y + (-POLYGON_SIZE / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posPolygon.x + (-POLYGON_SIZE / 2), g_posPolygon.y + (POLYGON_SIZE / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posPolygon.x + (POLYGON_SIZE / 2), g_posPolygon.y + (POLYGON_SIZE / 2), 0.0f);


#else

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

	//頂点座標セット
	SetVtxPosition(m_vPos);

#endif

	//色のセット
	SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
    
	//テクスチャ座標のセット
	SetTex(
		m_nPatternAnim * 0.2f, 
		0.0f,
		m_nPatternAnim * 0.2f + 0.2f,
		1.0f);

	Respawn();
	//テクスチャのバインド
	BindTexture(m_apTexture[TYPE_PLAYER]);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{

	//m_pTurret->Uninit();
	//m_pTurret = NULL;

	CGame::SetPlayerState(false);
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	CScene2D::Update();

	//ライフ
	CBomb * pBomb = CGame::GetBomb();

	//位置を取得
	m_pos = CScene2D::GetPosition();

	//サウンド
	CSound * pSound = CManager::GetSound();

	if (m_bStun == true)//行動不能かどうか
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
			//色のセット
			SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			CMsgWindow  * pMsg01 = CGame::GetMsgUi01();
			CMsgWindow  * pMsg02 = CGame::GetMsgUi02();
			pMsg01->HitMsgUi(false);
			pMsg02->HitMsgUi(false);
		}
	}
	else//行動可能
	{

#define JS 
//#define KEYBOARD
#ifdef JS
	//ジョイパッド
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

		//マウス座標取得
		POINT point = CInputMouse::GetMousePoint();

		//プレイヤー位置にマウス座標を代入
		pos.x = (float)point.x;
		pos.y = (float)point.y;

		if (CInputMouse::GetMousePress(0))//クリックで回転
		{
			// 回転
			m_rot.z += D3DX_PI * 0.01f;
			if (m_rot.z >= D3DX_PI)//一定の値を超えると回らなくなるので減算していく
			{
				m_rot.z -= D3DX_PI * 2.0f;
			}
	}
		// 拡大縮小
		m_fScale += m_fAddScale;
		if (m_fScale >= 2.0f || m_fScale <= 0.5f)
		{
			m_fAddScale *= -1;
		}
#endif
		
		//なにもしてないとき
		m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

#ifdef JS

		//----------------------
		//移動
		//----------------------
		if (lpDIDevice != NULL &&
			js.lRx == 0 &&
			js.lRy == 0
			)//何もしていないとき
		{
			m_bShot = false;
			m_nCounterBullet = 9;
			m_fSpeed = VALUE_MOVE;
		}

		//何もしていないとき（キーボードのみ）
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

		//右上Y
		if (lpDIDevice != NULL &&js.lY >= -1000 && js.lY < 0 && js.lX >= 0 && js.lX <= 1000 || CInputKeyboard::GetKeyboardPress(DIK_W) && CInputKeyboard::GetKeyboardPress(DIK_D))
		{
			m_rotDest.z = D3DXToRadian(-45.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * m_fSpeed, sinf(m_rotDest.z) * m_fSpeed, 0.0f);
		}

		//左上Y
		if (lpDIDevice != NULL &&js.lY >= -1000 && js.lY < 0 && js.lX <= 0 && js.lX >= -1000 || CInputKeyboard::GetKeyboardPress(DIK_W) && CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_rotDest.z = D3DXToRadian(45.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * -m_fSpeed, sinf(m_rotDest.z) * -m_fSpeed, 0.0f);
		}
		//右下Y
		if (lpDIDevice != NULL &&js.lY <= 1000 && js.lY > 0 && js.lX >= 0 && js.lX <= 1000 || CInputKeyboard::GetKeyboardPress(DIK_S) && CInputKeyboard::GetKeyboardPress(DIK_D))
		{
			m_rotDest.z = D3DXToRadian(-135.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * -m_fSpeed, sinf(m_rotDest.z) * -m_fSpeed, 0.0f);
		}
		//左下Yy
		if (lpDIDevice != NULL &&js.lY <= 1000 && js.lY > 0 && js.lX <= 0 && js.lX >= -1000 || CInputKeyboard::GetKeyboardPress(DIK_S) && CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_rotDest.z = D3DXToRadian(135.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * m_fSpeed, sinf(m_rotDest.z) * m_fSpeed, 0.0f);
		}
		//左
		if (lpDIDevice != NULL &&js.lX >= -1000 && js.lX < 0 && js.lY == 0 || CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_rotDest.z = D3DXToRadian(90.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, 0.0f);
		}
		//右
		if (lpDIDevice != NULL &&js.lX <= 1000 && js.lX > 0 && js.lY == 0 || CInputKeyboard::GetKeyboardPress(DIK_D))
		{

			m_rotDest.z = D3DXToRadian(270.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, 0.0f);
		}
		//上
		if (lpDIDevice != NULL &&js.lY >= -1000 && js.lY < 0 && js.lX == 0 || CInputKeyboard::GetKeyboardPress(DIK_W))
		{

			m_rotDest.z = D3DXToRadian(0.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, 0.0f);
		}
		//下
		if (lpDIDevice != NULL &&js.lY <= 1000 && js.lY > 0 && js.lX == 0 || CInputKeyboard::GetKeyboardPress(DIK_S))
		{

			m_rotDest.z = D3DXToRadian(180.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, 0.0f);
		}

		//キーボードの移動
		//左
		if (CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_rotDest.z = D3DXToRadian(90.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, 0.0f);
		}
		//右
		if (CInputKeyboard::GetKeyboardPress(DIK_D))
		{

			m_rotDest.z = D3DXToRadian(270.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * -m_fSpeed, 0.0f);
		}
		//上
		if (CInputKeyboard::GetKeyboardPress(DIK_W))
		{

			m_rotDest.z = D3DXToRadian(0.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, 0.0f);
		}
		//下
		if (CInputKeyboard::GetKeyboardPress(DIK_S))
		{

			m_rotDest.z = D3DXToRadian(180.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, sinf(m_rotDest.z - D3DXToRadian(90.0f)) * m_fSpeed, 0.0f);
		}
		//右上Y
		if (CInputKeyboard::GetKeyboardPress(DIK_W) && CInputKeyboard::GetKeyboardPress(DIK_D))
		{
			m_rotDest.z = D3DXToRadian(-45.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * m_fSpeed, sinf(m_rotDest.z) * m_fSpeed, 0.0f);
		}

		//左上Y
		if (CInputKeyboard::GetKeyboardPress(DIK_W) && CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_rotDest.z = D3DXToRadian(45.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * -m_fSpeed, sinf(m_rotDest.z) * -m_fSpeed, 0.0f);
		}
		//右下Y
		if (CInputKeyboard::GetKeyboardPress(DIK_S) && CInputKeyboard::GetKeyboardPress(DIK_D))
		{
			m_rotDest.z = D3DXToRadian(-135.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * -m_fSpeed, sinf(m_rotDest.z) * -m_fSpeed, 0.0f);
		}
		//左下Yy
		if (CInputKeyboard::GetKeyboardPress(DIK_S) && CInputKeyboard::GetKeyboardPress(DIK_A))
		{
			m_rotDest.z = D3DXToRadian(135.0f);
			m_Move = D3DXVECTOR3(cosf(m_rotDest.z) * m_fSpeed, sinf(m_rotDest.z) * m_fSpeed, 0.0f);
		}
		//----------------------
		//角度指定ショット
		//----------------------
		if (CManager::GetIsTalking() == false)
		{
			//キーボードの玉発射
			//左
			if (CInputKeyboard::GetKeyboardTrigger(DIK_LEFT))
			{

				m_fTurretRot = D3DXToRadian(90.0f);
				m_bShot = true;
			}
			//右
			if (CInputKeyboard::GetKeyboardTrigger(DIK_RIGHT))
			{

				m_fTurretRot = D3DXToRadian(270.0f);
				m_bShot = true;
			}
			//上
			if (CInputKeyboard::GetKeyboardTrigger(DIK_UP))
			{
				m_TurretrotDest.y = D3DXToRadian(180.0f);
				m_fTurretRot = D3DXToRadian(0.0f);
				m_bShot = true;
			}
			//下
			if (CInputKeyboard::GetKeyboardTrigger(DIK_DOWN))
			{
				m_fTurretRot = D3DXToRadian(0.0f);
				m_fTurretRot = D3DXToRadian(180.0f);
				m_bShot = true;
			}
			//右上Y
			if (CInputKeyboard::GetKeyboardPress(DIK_UP) && CInputKeyboard::GetKeyboardPress(DIK_RIGHT))
			{
				m_TurretrotDest.y = D3DXToRadian(-135.0f);
				m_fTurretRot = D3DXToRadian(-45.0f);
				m_bShot = true;
			}

			//左上Y
			if (CInputKeyboard::GetKeyboardPress(DIK_UP) && CInputKeyboard::GetKeyboardPress(DIK_LEFT))
			{
				m_TurretrotDest.y = D3DXToRadian(135.0f);
				m_fTurretRot = D3DXToRadian(45.0f);
				m_bShot = true;
			}
			//右下Y
			if (CInputKeyboard::GetKeyboardPress(DIK_DOWN) && CInputKeyboard::GetKeyboardPress(DIK_RIGHT))
			{
				m_TurretrotDest.y = D3DXToRadian(-45.0f);
				m_fTurretRot = D3DXToRadian(-135.0f);
				m_bShot = true;
			}
			//左下Yy
			if (CInputKeyboard::GetKeyboardPress(DIK_DOWN) && CInputKeyboard::GetKeyboardPress(DIK_LEFT))
			{
				m_TurretrotDest.y = D3DXToRadian(45.0f);
				m_fTurretRot = D3DXToRadian(135.0f);
				m_bShot = true;
			}

			//ジョイスティックの玉発射
			//左
			if (lpDIDevice != NULL &&js.lRx == -1000 && js.lRx < 0 && js.lRy == 0)
			{
				
				m_fTurretRot = D3DXToRadian(90.0f);
				m_bShot = true;
			}
			//右
			if (lpDIDevice != NULL &&js.lRx == 1000 && js.lRx > 0 && js.lRy == 0)
			{
				
				m_fTurretRot = D3DXToRadian(270.0f);
				m_bShot = true;
			}
			//上
			if (lpDIDevice != NULL &&js.lRy == -1000 && js.lRy < 0 && js.lRx == 0)
			{
				m_TurretrotDest.y = D3DXToRadian(180.0f);
				m_fTurretRot = D3DXToRadian(0.0f);
				m_bShot = true;
			}
			//下
			if (lpDIDevice != NULL &&js.lRy == 1000 && js.lRy > 0 && js.lRx == 0)
			{
				m_fTurretRot = D3DXToRadian(0.0f);
				m_fTurretRot = D3DXToRadian(180.0f);
				m_bShot = true;
			}

			if (lpDIDevice != NULL && pInputJoystick->GetJoystickTrigger(JS_LB) || CInputKeyboard::GetKeyboardTrigger(DIK_LSHIFT))//ボム
			{
				//ショット音の再生
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_BOMB);
				ExplosionBomb(m_pos);
			}

			//細かい発射角の指定
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
		//移動
		//----------------------
		if (CInputKeyboard::GetKeyboardPress(MAX_KEY) == false)//下
		{
			m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		if (CInputKeyboard::GetKeyboardPress(DIK_A))//左
		{
			m_Move.x = -VALUE_MOVE;
			m_rot.z = D3DXToRadian(90.0f);
		}
		if (CInputKeyboard::GetKeyboardPress(DIK_D))//右
		{
			m_Move.x = VALUE_MOVE;
			m_rot.z = D3DXToRadian(270.0f);
		}
		if (CInputKeyboard::GetKeyboardPress(DIK_W))//上
		{
			m_Move.y = -VALUE_MOVE;
			m_rot.z = D3DXToRadian(0.0f);
		}
		if (CInputKeyboard::GetKeyboardPress(DIK_S))//下
		{
			m_Move.y = VALUE_MOVE;
			m_rot.z = D3DXToRadian(180.0f);
		}
		//----------------------
		//角度指定ショット
		//----------------------

		if (CInputKeyboard::GetKeyboardPress(DIK_LEFT))//左
		{
			m_rot.y = D3DXToRadian(90.0f);
			m_fTurretRot = D3DXToRadian(90.0f);
			m_bShot = true;
		}
		else if (CInputKeyboard::GetKeyboardPress(DIK_RIGHT))//右
		{
			m_rot.y = D3DXToRadian(270.0f);
			m_fTurretRot = D3DXToRadian(270.0f);
			m_bShot = true;
		}
		else if (CInputKeyboard::GetKeyboardPress(DIK_UP))//上
		{
			m_rot.y = D3DXToRadian(180.0f);
			m_fTurretRot = D3DXToRadian(0.0f);
			m_bShot = true;
		}
		else if (CInputKeyboard::GetKeyboardPress(DIK_DOWN))//下
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

		if (CInputKeyboard::GetKeyboardTrigger(DIK_LSHIFT))//ボム
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
			//ぐるぐる弾
			if (m_nExplosionBombCount % 4 == 0)
			{

				//ショット音の再生
				pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_SHOT);
			}
				//弾の生成
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian((m_nExplosionBombCount * 20) * (360 / 360)))*-20.5f, sinf(D3DXToRadian((m_nExplosionBombCount * 20) * (360 / 360)))*-20.5f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_PINK, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf(D3DXToRadian((m_nExplosionBombCount * 20) * (360 / 360)))*20.5f, sinf(D3DXToRadian((m_nExplosionBombCount * 20) * (360 / 360)))*20.5f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_PINK, CBullet::ATTACK_NORMAL);

			if (m_nExplosionBombCount >= 100)
			{
				m_bExplosionBombCount = false;
				m_nExplosionBombCount = 0;
			}
			m_nExplosionBombCount++;
		}
		//間隔を取ってショット
		if (m_nCounterBullet % 2 == 0)
		{
			if (m_nPower < PLAYER_MAXPOWER * 0.33f)//レベル1の時
			{
				//弾の生成
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, sinf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);

			}
			if(m_nPower >= PLAYER_MAXPOWER * 0.33f)//レベル２
			{
				//弾の生成
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, sinf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * 30.0f, sinf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * 30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);

			}
			if(m_nPower >= PLAYER_MAXPOWER * 0.66f)
			{
				//弾の生成
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, sinf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f + 0.2f) - D3DXToRadian(-90.0f)) * -30.0f, sinf((m_fTurretRot * -1.0f + 0.2f) - D3DXToRadian(-90.0f)) * -30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f - 0.2f) - D3DXToRadian(-90.0f)) * -30.0f, sinf((m_fTurretRot * -1.0f - 0.2f) - D3DXToRadian(-90.0f)) * -30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
			}
			if (m_nPower >= PLAYER_MAXPOWER)
			{
				//弾の生成
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, sinf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * -30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f + 0.1f) - D3DXToRadian(-90.0f)) * 30.0f, sinf((m_fTurretRot * -1.0f + 0.1f) - D3DXToRadian(-90.0f)) * 30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f - 0.1f) - D3DXToRadian(-90.0f)) * 30.0f, sinf((m_fTurretRot * -1.0f - 0.1f) - D3DXToRadian(-90.0f)) * 30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
				//弾の生成
				CBullet::Create(m_pos, BULLET_DIAMOND_SIZE, BULLET_DIAMOND_SIZE, D3DXVECTOR3(cosf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * 30.0f, sinf((m_fTurretRot * -1.0f) - D3DXToRadian(-90.0f)) * 30.0f, 0.0f), CBullet::TYPE_PLAYER, CBullet::BULLET_DIAMOND, CBullet::COLOR_BLUE, CBullet::ATTACK_NORMAL);
			}


		}
		if (m_nCounterBullet % 10 == 0)
		{
			//ショット音の再生
			pSound->CSound::PlaySound(CSound::SOUND_LABEL_SE_PLAYERSHOT);
		}

		//無敵時間中
		if (m_bInvincible == true)
		{
			//点滅処理
			if (m_CountTime >= 10)
			{
				if (m_CountInvincible % 2 == 0)
				{
					//色のセット
					SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
					//頂点カラーのセット
					m_pTurret->SetColor(D3DCOLOR_RGBA(255, 255, 255, 0));
				}
				else
				{
					//色のセット
					SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
					//頂点カラーのセット
					m_pTurret->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
				}
				if (m_CountInvincible >= 20)
				{
					//色のセット
					SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
					//頂点カラーのセット
					m_pTurret->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
					m_bInvincible = false;
					m_CountInvincible = 0;
				}
				m_CountInvincible++;
				m_CountTime = 0;
			}
			m_CountTime++;
		}

		//画面外処理
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

	
		//位置更新
		m_pos += m_Move;

		if (m_Move.x != 0.0f || m_Move.y != 0.0f)
		{
			if (m_Move.y <= 0.0f)
			{
				//減速中
				if (m_bShot == true && m_nCounterBullet > 0)
				{
					//エフェクト生成（エンジン）
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_BLUE, 15);
					//エフェクト生成
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_BLUE, 15);
					//エフェクト生成（エンジン）
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_PINK, 15);
					//エフェクト生成
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_PINK, 15);

					//エフェクト生成
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_004, CEffect::COLOR_BLUE, 20);
					//エフェクト生成
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_003, CEffect::COLOR_PINK, 20);

				}
				else
				{
					//エフェクト生成（エンジン）
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_RED, 15);
					//エフェクト生成
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_RED, 15);
					//エフェクト生成（エンジン）
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_ORANGE, 15);
					//エフェクト生成
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_ORANGE, 15);

					//エフェクト生成
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_004, CEffect::COLOR_RED, 20);
					//エフェクト生成
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_003, CEffect::COLOR_ORANGE, 20);
				}
			}
			else //後ろ移動してるときはエフェクトのらいふを少なくする
			{
				//減速中
				if (m_bShot == true && m_nCounterBullet > 0)
				{
					//エフェクト生成（エンジン）
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_BLUE, 5);
					//エフェクト生成
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_BLUE, 5);
					//エフェクト生成（エンジン）
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_PINK, 5);
					//エフェクト生成
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_PINK, 5);

					//エフェクト生成
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_004, CEffect::COLOR_BLUE, 5);
					//エフェクト生成
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_003, CEffect::COLOR_PINK, 5);

				}
				else
				{
					//エフェクト生成（エンジン）
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_RED, 5);
					//エフェクト生成
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 8.0f, 8.0f, m_Move, CEffect::TYPE_004, CEffect::COLOR_RED, 5);
					//エフェクト生成（エンジン）
					CEffect::Create(D3DXVECTOR3(m_vPos[2].x, m_vPos[2].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_ORANGE, 5);
					//エフェクト生成
					CEffect::Create(D3DXVECTOR3(m_vPos[3].x, m_vPos[3].y, 0.0f), 10.0f, 10.0f, m_Move, CEffect::TYPE_003, CEffect::COLOR_ORANGE, 5);

					//エフェクト生成
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_004, CEffect::COLOR_RED, 5);
					//エフェクト生成
					CEffect::Create(m_pos, ENEMY_SIZE_001_HEIGHT * 0.2f, ENEMY_SIZE_001_HEIGHT * 0.2f, m_Move, CEffect::TYPE_003, CEffect::COLOR_ORANGE, 5);
				}
			}
		}

	}

	//---------------------------
	//頂点座標の設定
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

	//頂点座標セット
	SetVtxPosition(m_vPos);

	//頂点座標の設定(砲塔)
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

	//頂点座標のセット
	m_pTurret->SetVtxPosition(m_vPosTurret);

	SetTex(
		0.0f,
		0.0f,
		1.0f,
		1.0f);

	

	CScene2D::SetPosition(m_pos);
}
//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
//ライフ数追加
//=============================================================================
void CPlayer::AddLife(int nLife)
{
	CLife * pLife = CGame::GetLife();
	m_nLife += nLife;
	pLife->SetLife(m_nLife);
}

//=============================================================================
//ライフ数減算
//=============================================================================
void CPlayer::SubLife(int nLife)
{
	CLife * pLife = CGame::GetLife();
	m_nLife -= nLife;
	pLife->SetLife(m_nLife);

	//キャラクターUIをダメージモードにする
	CMsgWindow  * pMsg01 = CGame::GetMsgUi01();
	CMsgWindow  * pMsg02 = CGame::GetMsgUi02();
	pMsg01->HitMsgUi(true);
	pMsg02->HitMsgUi(true);

	if (m_nLife <= 0)//死んだら
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
//ボム数追加
//=============================================================================
void CPlayer::AddBomb(int nBomb)
{
	//ボム
	CBomb * pBomb = CGame::GetBomb();
	if (m_nBomb < MAX_STAR)
	{
		m_nBomb += nBomb;
		pBomb->SetBomb(m_nBomb);
	}
}

//=============================================================================
//ボム数減算
//=============================================================================
void CPlayer::SubBomb(int nBomb)
{
	//ボム
	CBomb * pBomb = CGame::GetBomb();
	if (m_nBomb > 0)
	{
		m_nBomb -= nBomb;
		pBomb->SetBomb(m_nBomb);
	}
}
//=============================================================================
//パワー追加
//=============================================================================
void CPlayer::AddPower(int nPow)
{
	if (m_nPower <= PLAYER_MAXPOWER)
	{
		m_nPower += nPow;
	}
	
}

//=============================================================================
//パワー減算
//=============================================================================
void CPlayer::SubPower(int nPow)
{
	if (m_nPower > 0)
	{
		m_nPower -= nPow;
	}
}

//=============================================================================
//ボムを爆破
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
