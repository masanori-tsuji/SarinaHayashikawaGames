//=============================================================================
//
// 敵の処理 [effect.cpp]
// Author : 林川紗梨夏
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
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
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEffect::m_pTexture[TYPE_MAX] = {};   //テクスチャのポインタ

D3DCOLOR CEffect::m_aColor[COLOR_MAX] = //色パラメータ
{
	D3DCOLOR_RGBA(255,0,0,255),	//赤
	D3DCOLOR_RGBA(255,50,0,255),// オレンジ
	D3DCOLOR_RGBA(255,255,0,255),// 黄色
	D3DCOLOR_RGBA(0,255,0,255),//緑
	D3DCOLOR_RGBA(0,0,255,255),//青
	D3DCOLOR_RGBA(50,0,255,255),//紫
	D3DCOLOR_RGBA(255,0,255,255),//ピンク
};

//=============================================================================
// コンストラクタ
//=============================================================================
CEffect::CEffect(int nPriority) : CScene2D(nPriority)
{
	for (int nCntVertex = 0; nCntVertex < NUM_VERTEX; nCntVertex++)
	{
		m_vPos[nCntVertex] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	m_fLength = 0.0f;		// 対角線の長さ
	m_fAngle = 0.0f;		// 対角線の角度
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 向き
	m_fScale = 0.0f;		// 拡大
	m_fAddScale = 0.0f;	// 拡大加算値
	m_Color = D3DCOLOR_RGBA(255, 255, 255, 255);

	m_TargetDistance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//目標までの距離
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//目標地点
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量

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
// デストラクタ
//=============================================================================
CEffect::~CEffect()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CEffect * CEffect::Create(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, D3DXVECTOR3 move, TYPE Type, int col, int nLife)
{
	CEffect *pEffect;
	pEffect = new CEffect;
	pEffect->Init(pos, fSizeWidth, fSizeHeight, move, Type, col,nLife);
	return pEffect;
}
//=============================================================================
// テクスチャロード
//=============================================================================
HRESULT CEffect::Load(void)
{
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET, &m_pTexture[TYPE_001]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET, &m_pTexture[TYPE_002]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET, &m_pTexture[TYPE_003]);
	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_BULLET, &m_pTexture[TYPE_004]);
	return S_OK;
}
//=============================================================================
// テクスチャアンロード
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
// 初期化処理
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, float fSizeWidth, float fSizeHeight, D3DXVECTOR3 move, TYPE Type, int col, int nLife)
{
	CScene2D::Init(pos, fSizeWidth, fSizeHeight);

	SetObjType(CScene::OBJTYPE_NONE);
	
	m_nType = Type;
	m_Move = move;
	// 対角線、角度の設定
	m_fLength = sqrtf((float)(((fSizeWidth / 2) * (fSizeWidth / 2)) + ((fSizeHeight / 2) * (fSizeHeight / 2))));
	m_fAngle = atan2f((fSizeHeight / 2), (fSizeWidth / 2));
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;
	m_fAddScale = 0.01f;
	m_ColorNum = col;
	m_Color = m_aColor[col];
	m_nLife = nLife;
	//ランダムシード値
	int nRandSeed = rand() % 255;
	srand((unsigned int)time(NULL)*nRandSeed);

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

	// ポリゴンの位置を設定
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


	//色のセット
	SetColor(m_Color);

	//テクスチャ座標のセット
	SetTex(
		0.0f,
		0.0f,
		1.0f,
		1.0f);

	//テクスチャのバインド
	BindTexture(m_pTexture[m_nType]);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect::Uninit(void)
{

	////位置を取得
	//D3DXVECTOR3 pos = CScene2D::GetPosition();
	/*CExplosion::Create(pos, TEXTURE_WIDTH, TEXTURE_HEIGHT);*/
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update(void)
{
	CScene2D::Update();
	//サウンド
	CSound * pSound = CManager::GetSound();

	//位置を取得
	D3DXVECTOR3 pos = CScene2D::GetPosition();

	if (m_nType == TYPE_001)
	{
		//位置更新
		pos += m_Move;
	}
	

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

	m_nCountAnim++;
	CScene2D::SetPosition(pos);

	//if (m_nType == TYPE_003)
	//{
	//	// 拡大透明
	//		m_fScale -= 0.2f;
	//		m_nAlpha -= 0.2f;
	////色のセット
	//SetColor(D3DXCOLOR(m_Color.r, m_Color.g, m_Color.b, m_nAlpha));
	//	//一定時間で破棄
	//	if (m_nCountAnim >= 5)
	//	{
	//		Uninit();
	//		return;
	//	}
	//}
	//if (m_nType == TYPE_004)
	//{
	//	// 拡大透明
	//	m_fScale -= 0.02f;
	//	m_nAlpha -= 0.02f;
	//	//色のセット
	//	SetColor(D3DXCOLOR(m_Color.r, m_Color.g, m_Color.b, m_nAlpha));
	//	//一定時間で破棄
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
		// 拡大透明
		m_fScale -= 1.0f / m_nLife;
		m_nAlpha -= 1.0f / m_nLife;
	}

	//色のセット
	SetColor(D3DXCOLOR(m_Color.r, m_Color.g, m_Color.b, m_nAlpha));
	m_nLife--;
		//一定時間で破棄
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
// 描画処理
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