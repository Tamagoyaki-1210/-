//============================================================================
//
//ポリゴン描画の処理[main.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _MAIN_H_						//このマクロ定義がされてなかったら
#define _MAIN_H_						//2連インクルード防止のマクロ定義
#include <windows.h>
#include <tchar.h>						//文字表示に必要
#include "d3dx9.h"						//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の整数処理マクロ
#include "dinput.h"						//入力処理に必要
#include "xaudio2.h"					//サウンド処理に必要

//****************************************************************************
//ライブラリのリンク
//****************************************************************************
#pragma comment(lib,"d3d9.lib")			//描画処理に必要
#pragma comment(lib,"d3dx9.lib")		//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")		//DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"winmm.lib")		//システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")		//入力処理に必要
//****************************************************************************
//マクロ定義
//****************************************************************************
#define CLASS_NAME		"WindowClass"				//ウインドクラスの名前
#define WINDOW_NAME		"DANGO_HAZARD"		//ウインドウの名前
#define SCREEN_WIDTH		(1280)		//ウインドウの横幅
#define SCREEN_HEIGHT		(720)		//ウインドウの高さ
#define TEXT_WIDTH			(10)		//テキストの横幅
#define TEXT_HEIGHT			(24)		//テキストの高さ
#define MAX_TEXT			(14)		//テキストの最大数
//頂点フォーマット[2D]
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	

#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//座標・法線・カラー

//****************************************************************************
//構造体定義
//****************************************************************************
//頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ標準
}VERTEX_3D;

//頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//座標変換用係数(1.0fで固定)
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ標準
}VERTEX_2D;

//画面モードの種類
typedef enum
{
	MODE_TITLE = 0,	//タイトル画面	
	MODE_GAME,		//ゲーム画面
	MODE_RESULT,	//リザルト画面
	MODE_GAMEOVER,	//ゲームオーバー画面
	MODE_MAX
}MODE;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);

void SetMode(MODE mode);
MODE GetMode(void);

#endif 


