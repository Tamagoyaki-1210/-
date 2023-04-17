//============================================================================
//
//ポリゴン描画の処理[main.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include <stdio.h>		//sprintfに必要
#include "main.h"		//作成したmain.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする

#include "fade.h"		//作成したfade.hをインクルードする
#include "title.h"		//作成したtitle.hをインクルードする
#include "game.h"		//作成したgame.hをインクルードする
#include "result.h"		//作成したresult.hをインクルードする
#include "gameover.h"	//作成したgameover.hをインクルードする

#include "camera.h"		//作成したcamera.hをインクルードする
#include "player.h"		//作成したplayer.hをインクルードする
#include "enemy.h"		//作成したenemy.hをインクルードする
#include "item.h"		//作成したitem.hをインクルードする
#include "attack.h"		//作成したattack.hをインクルードする

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM IParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;				//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;		//Direct3Dデバイスへのポインタ
LPD3DXFONT			pFont;						// フォントオブジェクト

MODE g_mode = MODE_TITLE;						//現在のモード
bool g_MeshDebug = false;						//メッシュデバッグ
bool g_TextDebug = true;						//テキストデバッグ

//============================================================================
//メイン関数 (関数ヘッダコメント)
//============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine,int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,							//ウインドウのスタイル
		WindowProc,							//ウインドウプロシージャ
		0,									//0にする(通常は使用しない)
		0,									//0にする(通常は使用しない)
		hInstance,							//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),		//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),			//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			//クライアント領域の背景色
		NULL,								//メニューバー
		CLASS_NAME,							//ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)		//ファイルのアイコン
	};
	HWND hWnd;	//ウインドウハンドル(識別子)
	MSG msg;	//メッセージを格納する変数

	//画面サイズの構造体
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//ウインドクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを作成
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,				//ウインドウの左上X座標
		CW_USEDEFAULT,				//ウインドウの左上Y座標
		(rect.right - rect.left),	//ウインドウの幅
		(rect.bottom - rect.top),	//ウインドウの高さ
		NULL,
		NULL,
		hInstance,
		NULL);

	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExecLastTime;	//最後に処理した時刻

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return -1;
	}

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);		//ウインドウの表示状態を設定
	UpdateWindow(hWnd);				//クライアント領域を更新

	//分解能力を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;				//初期化する
	dwExecLastTime = timeGetTime();	//現在時刻を取得(保存)

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージを抜ける
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();		//現在時刻を取得
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;	//処理開始の時刻[現在時刻]を取得
				Update();	//更新処理
				Draw();		//描画処理
			}
		}
	}
	//終了処理
	Uninit();
	//分解能力を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}
//============================================================================
//ウインドウプロシージャの処理
//============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM IParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:	//ウインドウ破棄のメッセージ
						//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:	//[ESC]キーが押された時の処理
						//第1引数をhWndに指定した場合、ウインドウが親(オーナー)になる
			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				//ウインドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:	//閉じるボタン押下のメッセージ

		nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			//ウインドウを破棄する
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;	//0を返さないと終了してしまう
		}
		break;
	case WM_LBUTTONDOWN:	//マウス左クリックのメッセージ

		//ウインドウにフォーカスを合わせる
		SetFocus(hWnd);		//クリックしたウインドウがアクティブになる処理
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, IParam);
}
//============================================================================
//初期化の処理
//============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;		//ゲーム画面サイズの横幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		//ゲーム画面サイズの高さ
	d3dpp.BackBufferFormat = d3ddm.Format;		//バックバッファの形式
	d3dpp.BackBufferCount = 1;					//バックバッファの個数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil = TRUE;		//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;					//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//インターバル

	//Direct3Dデバイスの生成(描写処理と頂点処理をハードウェアで行なう)
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//Direct3Dデバイスの生成(描写処理はハードウェア、頂点処理はCPUで行なう)
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{//Direct3Dデバイスの生成(描写処理と頂点処理をCPUで行なう)
			if (FAILED(g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//フォント
	D3DXCreateFont(
		g_pD3DDevice,				// Direct3Dデバイス
		TEXT_HEIGHT,				// 高さ
		TEXT_WIDTH,					// 幅
		FW_REGULAR,					// フォントの太さ 普通
		NULL,						// 下線
		FALSE,						// 斜体
		SHIFTJIS_CHARSET,			// 文字セット
		OUT_DEFAULT_PRECIS,			// 出力制度は普通
		PROOF_QUALITY,				// 文字品質を重視
		FIXED_PITCH | FF_SCRIPT,	// ピッチとファミリ
		TEXT("ＭＳＰゴシック"),	// フォント名
		&pFont);					// ID3DXFontポインタ

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//カリングの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//モードの設定
	SetMode(g_mode);

	//フェードの初期化設定
	InitFade(g_mode);	

	return S_OK;
}
//============================================================================
//終了処理
//============================================================================
void Uninit(void)
{
	UninitTitle();		//タイトル画面の終了処理
	UninitGame();		//ゲーム画面の終了処理
	UninitResult();		//リザルトの終了処理
	UninitFade();		//フェードの終了処理
	UninitGameOver();	//ゲームオーバーの終了処理

	UninitKeyboard();	//キーボードの終了処理

	if (g_pD3DDevice != NULL)
	{//Direct3Dデバイスの破棄
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	if (g_pD3D != NULL)
	{//Direct3Dオブジェクトの破棄
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}
//============================================================================
//更新処理
//============================================================================
void Update(void)
{
	FADE fade;
	fade = GetFade();

	UpdateKeyboard();	//キーボードの更新処理

	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();	//タイトル画面
		break;
	case MODE_GAME:
		UpdateGame();	//ゲーム画面
		break;
	case MODE_RESULT:
		UpdateResult();	//リザルト画面
		break;
	case MODE_GAMEOVER:
		UpdateGameOver();	//ゲームオーバー画面
		break;
	}

	UpdateFade();	//フェードの更新処理

	if (GetKeyboardTrigger(DIK_F2) == true)
	{
		g_TextDebug = !g_TextDebug;
		//if (g_TextDebug == false)
		//{
		//	//ワイヤーフレームモードの設定(追加文章)
		//	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//}
		//else
		//{
		//	//元に戻す(ワイヤーフレームモード)
		//	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		//}
		//g_MeshDebug = !g_MeshDebug;
		//if (g_MeshDebug == false)
		//{
		//	//ワイヤーフレームモードの設定(追加文章)
		//	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//}
		//else
		//{
		//	//元に戻す(ワイヤーフレームモード)
		//	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		//}
	}
}
//============================================================================
//描画処理
//============================================================================
void Draw(void)
{
	//画面クリア(バックバッファ&Zバッファのクリア)
	g_pD3DDevice->Clear(
		0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が開始が成功した場合
		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();	//タイトル画面の描画処理
			break;
		case MODE_GAME:
			DrawGame();		//ゲーム画面も描画処理
			break;
		case MODE_RESULT:
			DrawResult();	//リザルト画面の描画処理
			break;
		case MODE_GAMEOVER:
			DrawGameOver();	//ゲームオーバー画面の描画処理
			break;
		}

		DrawFade();	//フェードの描画処理

	char aStr[MAX_TEXT][256];	//テキストの文字列
	PLAYER *pPlayer = GetPlayer();
	ENEMY *pEnemy = GetEnemy();
	CAMERA *pCamera = GetCamera();

		//テキスト集
		sprintf(aStr[0], "<操作説明>\n");
		sprintf(aStr[1], "プレイヤー移動 :	[W]\n");
		sprintf(aStr[2], "------------[A] [S] [D]\n");
		sprintf(aStr[3], "ジャンプ\n");
		sprintf(aStr[4], "[SPACE]\n");
		sprintf(aStr[5], "------------\n");
		sprintf(aStr[6], "攻撃\n");
		sprintf(aStr[7], "[ENTER]\n");
		sprintf(aStr[8], "------------\n");
		sprintf(aStr[9], "カメラ移動 :	[↑]\n");
		sprintf(aStr[10], "-------[←] [↓] [→]\n");
		sprintf(aStr[11], "又は :	[U] [I] [O]\n");
		sprintf(aStr[12], "------[J] [K] [L]\n");
		sprintf(aStr[13], "操作説明の閉じ方 : [F2]\n");
	//sprintf(aStr[0], "<テキスト>\n");
	//sprintf(aStr[1], "[F3] : カメラ追従\n");
	////sprintf(aStr[2], "プレイヤー_x座標:%.2f\n", pPlayer->pos.x);
	////sprintf(aStr[3], "プレイヤー_y座標:%.2f\n", pPlayer->pos.y);
	////sprintf(aStr[4], "プレイヤー_z座標:%.2f\n", pPlayer->pos.z);
	////sprintf(aStr[2], "プレイヤー_x移動量:%.2f\n", pPlayer->move.x);
	////sprintf(aStr[3], "プレイヤー_y移動量:%.2f\n", pPlayer->move.y);
	////sprintf(aStr[4], "プレイヤー_z移動量:%.2f\n", pPlayer->move.z);
	////sprintf(aStr[2], "敵キャラ1_x座標:%.2f\n", pEnemy->pos.x);
	////sprintf(aStr[3], "敵キャラ1_y座標:%.2f\n", pEnemy->pos.y);
	////sprintf(aStr[4], "敵キャラ1_z座標:%.2f\n", pEnemy->pos.z);
	//sprintf(aStr[2], "敵キャラ1_x移動量:%.2f\n", pEnemy->move.x);
	//sprintf(aStr[3], "敵キャラ1_y移動量:%.2f\n", pEnemy->move.y);
	//sprintf(aStr[4], "敵キャラ1_z移動量:%.2f\n", pEnemy->move.z);
	//sprintf(aStr[5], "視点の座標:(%.2f,%.2f,%.2f)\n", pCamera->posV.x, pCamera->posV.y, pCamera->posV.z);
	//sprintf(aStr[6], "注視点の座標:(%.2f,%.2f,%.2f)\n", pCamera->posR.x, pCamera->posR.y, pCamera->posR.z);
	//sprintf(aStr[7], "カメラの角度:(%.2f,%.2f,%.2f)\n", pCamera->rot.x, pCamera->rot.y, pCamera->rot.z);
	//sprintf(aStr[8], "プレイヤー接地判定[%d]\n", pPlayer->bGround);
	//sprintf(aStr[9], "プレイヤーモーション[%d]\n", pPlayer->motion);
	//sprintf(aStr[10], "プレイヤー経過フレーム数[%.0f]\n", pPlayer->fFlame);
	//sprintf(aStr[11], "プレイヤーループ判定[%d]\n", pPlayer->bLoop);
	//sprintf(aStr[12], "パーツ1_x角度:%.2f\n", pPlayer->aParts->rot.x);
	//sprintf(aStr[13], "プレイヤーライフ:[%d]\n", pPlayer->nLife);

	for (int nCntText = 0; nCntText < MAX_TEXT; nCntText++)
	{
		//テキスト位置
		RECT rc = {
			0,		// 左上のx座標
			100 + (TEXT_HEIGHT * nCntText),		// 左上のy座標
			SCREEN_WIDTH,	// 右下のx座標
			SCREEN_HEIGHT	// 右下のy座標
		};

		if (g_TextDebug == true)
		{
			// テキスト描画
			pFont->DrawText(
				NULL,					// NULL
				&aStr[nCntText][0],		// 描画テキスト
				-1,						// 全て表示
				&rc,						// 表示範囲
				DT_LEFT,					// 左寄せ
				D3DCOLOR_XRGB(0, 255, 0)	//緑色
				//D3DCOLOR_XRGB(255, 255, 255)	// 白色
			);
		}
		else
		{
			// テキスト描画
			pFont->DrawText(
				NULL,					// NULL
				&aStr[nCntText][0],		// 描画テキスト
				0,						// 全て非表示
				&rc,						// 表示範囲
				DT_LEFT,					// 左寄せ
				D3DCOLOR_XRGB(0, 255, 0)	//緑色
			);
		}
	}

		//描画終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//============================================================================
//モード設定処理
//============================================================================
void SetMode(MODE mode)
{
	//現在画面(モード)の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();	//タイトル画面の終了処理
		break;
	case MODE_GAME:
		UninitGame();	//ゲーム画面の終了処理
		break;
	case MODE_RESULT:
		UninitResult();	//リザルト画面の終了処理
		break;
	case MODE_GAMEOVER:
		UninitGameOver();	//ゲームオーバー画面の終了処理
		break;
	}

	g_mode = mode;		//現在の画面(モード)を切り替える
						//新画面(モード)の初期化処理
	switch (mode)
	{
	case MODE_TITLE:	
		InitTitle();	//タイトル画面の初期化処理
		break;

	case MODE_GAME:		
		InitGame();		//ゲーム画面の初期化処理
		break;

	case MODE_RESULT:	
		InitResult();	//リザルト画面の初期化処理
		break;
	case MODE_GAMEOVER:
		InitGameOver();	//ゲームオーバー画面の初期化処理
		break;
	}
}
//============================================================================
//デバイスの取得
//============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//============================================================================
//モード取得処理
//============================================================================
MODE GetMode(void)
{
	return g_mode;
}