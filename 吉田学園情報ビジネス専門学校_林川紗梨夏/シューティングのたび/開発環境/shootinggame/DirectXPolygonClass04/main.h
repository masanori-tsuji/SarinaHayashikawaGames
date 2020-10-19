//=============================================================================
//
// メイン処理 [main.h]
// Author : 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define DIRECTINPUT_VERSION		0x0800		// DirectInputのバージョン情報
//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include <windows.h>
#include <dinput.h>
#include "d3dx9.h"
//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment (lib,"ws2_32.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	WINDOW_POS_X	(0)
#define	WINDOW_POS_Y	(0)
#define SCREEN_WIDTH	(1920)
#define SCREEN_HEIGHT	(1080)
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_2D;
int GetFPS(void);
#endif