//=============================================================================
//
// レンダラー
// Author : tanimoto kosuke
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "renderer.h"
#include "main.h"
#include "object.h"
#include <stdio.h>

#ifdef _DEBUG
#include <tchar.h> // _T
#include "stage.h"
#endif // _DEBUG

//****************************************************************************
//マクロ定義
//****************************************************************************
#define TEXT_WIDTH			(10)		//テキストの横幅
#define TEXT_HEIGHT			(24)		//テキストの高さ
#define MAX_TEXT			(2)			//テキストの最大数

//-----------------------------------------
//プロトタイプ宣言
//-----------------------------------------
int GetFPS();		//FPS取得処理

//*****************************************************************************
// デバッグ用グローバル変数
//*****************************************************************************
#ifdef _DEBUG
LPD3DXFONT pFont;			// フォントオブジェクト
bool g_MeshDebug = false;	// メッシュデバッグ
bool g_TextDebug = true;	// テキストデバッグ
#endif // _DEBUG

//=====================================
//デフォルトコンストラクタ
//=====================================
CRenderer::CRenderer() : m_pD3D(nullptr), m_pD3DDevice(nullptr), m_pFont(nullptr)
{
	
}

//=====================================
//デストラクタ
//=====================================
CRenderer::~CRenderer()
{

}

//=====================================
//初期化処理
//=====================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));					// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

																// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if ((FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))) &&
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))) &&
		// 上記の設定が失敗したら
		// 描画と頂点処理をCPUで行なう
		(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))))
	{
		// 生成失敗
		return E_FAIL;
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				//カリングの設定
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//アルファブレンドの設定
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//アルファブレンドの設定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//アルファブレンドの設定


	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//テクスチャの大きさに合わせて綺麗に貼る
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	//テクスチャの大きさに合わせて綺麗に貼る
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	//テクスチャを繰り返しに貼る
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	//テクスチャを繰り返しに貼る


	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	//ポリゴンとテクスチャのアルファ値を混ぜる
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	//ポリゴンとテクスチャのアルファ値を混ぜる(テクスチャ指定)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	//ポリゴンとテクスチャのアルファ値を混ぜる(ポリゴン指定)


#ifdef _DEBUG
																			//フォント
	D3DXCreateFont(
		m_pD3DDevice,				// Direct3Dデバイス
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

	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);
#endif
	return S_OK;
}

//=====================================
//終了処理
//=====================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
#endif // _DEBUG

	// デバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//=====================================
//更新処理
//=====================================
void CRenderer::Update(void)
{
	// オブジェクトの更新処理
	CObject::UpdateAll();
}

//=====================================
//描画処理
//=====================================
void CRenderer::Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		//オブジェクトの描画処理
		CObject::DrawAll();

#ifdef _DEBUG
		// FPS表示
		DrawFPS();

		char aStr[MAX_TEXT][256];	//テキストの文字列
									//テキスト集
		sprintf(aStr[0], "<ゲーム時間経過フレーム>\n");
		sprintf(aStr[1], "[%d] : f経過\n", CStage::GetGameTimer());

		for (int nCntText = 0; nCntText < MAX_TEXT; nCntText++)
		{
			//テキスト位置
			RECT rc = {
				0,		// 左上のx座標
				100 + (TEXT_HEIGHT * nCntText),		// 左上のy座標
				SCREEN_WIDTH,	// 右下のx座標
				SCREEN_HEIGHT	// 右下のy座標
			};

			// テキスト描画
			pFont->DrawText(
				NULL,					// NULL
				&aStr[nCntText][0],		// 描画テキスト
				-1,						// 全て表示
				&rc,						// 表示範囲
				DT_LEFT,					// 左寄せ
				D3DCOLOR_XRGB(255, 255, 255)	// 白色
			);
		}

#endif // _DEBUG
		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

#ifdef _DEBUG
//=====================================
//FPS描画処理
//=====================================
void CRenderer::DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	TCHAR str[256];
	int nCountFPS = GetFPS();

	wsprintf(str, _T("FPS : %d\n"), nCountFPS);

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x0f, 0xff, 0xff));
}
#endif // _DEBUG

//=====================================
//デバイス取得処理
//=====================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}