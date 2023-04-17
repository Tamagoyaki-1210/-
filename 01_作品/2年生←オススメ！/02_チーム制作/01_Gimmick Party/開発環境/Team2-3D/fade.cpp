//=============================================================================
//
// フェード
// Author : tanimoto kosuke
//
//=============================================================================
#include "fade.h"
#include "application.h"
#include "rendering.h"

//---------------------------------------------------------
// マクロ定義
//---------------------------------------------------------
#define ALPHA_CHANGE	(0.03f)	// フェードのa値の変化量

//=====================================
// デフォルトコンストラクタ
//=====================================
CFade::CFade() : m_pVtxBuff(nullptr)
{

}

//=====================================
// デストラクタ
//=====================================
CFade::~CFade()
{

}

//=====================================
// 初期化処理
//=====================================
HRESULT CFade::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos.x = 0.0f;
	pVtx[0].pos.y = 0.0f;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = SCREEN_WIDTH;
	pVtx[1].pos.y = 0.0f;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = 0.0f;
	pVtx[2].pos.y = SCREEN_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = SCREEN_WIDTH;
	pVtx[3].pos.y = SCREEN_HEIGHT;
	pVtx[3].pos.z = 0.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	m_fAlpha = 1.0f;	// ポリゴンを不透明にする

	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CFade::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=====================================
// 更新処理
//=====================================
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{// 何もしていない状態ではない場合
		switch (m_fade)
		{
			// フェードアウト状態の場合
		case FADE_OUT:
			m_fAlpha += ALPHA_CHANGE;	// ポリゴンを不透明にしていく

			if (m_fAlpha >= 1.0f)
			{// 完全に不透明になった場合
				m_fAlpha = 1.0f;
				m_fade = FADE_CHANGE;  // フェード切り替え状態
			}
			break;
			// フェードイン状態の場合
		case FADE_IN:
			m_fAlpha -= ALPHA_CHANGE;	// ポリゴンを徐々に透明にする

			if (m_fAlpha <= 0.0f)
			{// 完全に透明になった場合
				m_fAlpha = 0.0f;
				m_fade = FADE_NONE;	// フェード切り替え状態
			}
			break;
			// フェード切り替え状態の場合
		case FADE_CHANGE:
			//ポリゴンが完全に不透明な場合
			if (m_fAlpha == 1.0f)
			{
				m_fade = FADE_IN;		// フェードイン状態
			}
			break;
		default:
			break;
		}
		SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fAlpha));
	}
}

//=====================================
// 描画処理
//=====================================
void CFade::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャを未使用にする
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//=====================================
//色設定処理(全体)
//=====================================
void CFade::SetColor(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=====================================
// フェード設定処理
//=====================================
void CFade::SetFade()
{
	//ポリゴンが完全に不透明な場合
	if (m_fAlpha == 1.0f)
	{
		m_fade = FADE_IN;		// フェードイン状態
	}
	//ポリゴンが完全に透明な場合
	else if (m_fAlpha == 0.0f)
	{
		m_fade = FADE_OUT;		// 何もしていない状態
	}
}

//=====================================
// フェード生成処理
//=====================================
CFade *CFade::Create()
{
	CFade* pFade = new CFade;

	if (pFade != nullptr)
	{
		if (FAILED(pFade->Init()))
		{
			return nullptr;
		}
	}
	return pFade;
}