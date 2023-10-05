//==============================================================================================
//
// オブジェクト　　　area.cpp
// koduna hirohito
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include "area.h"
#include "texture.h"
#include "application.h"
#include <assert.h>
#include "input.h"

//==============================================================================================
// 静的メンバ変数宣言
//==============================================================================================
const D3DXVECTOR3 CArea::AREA_POS = D3DXVECTOR3(800.0f, 2.0f, 800.0f);
const D3DXVECTOR2 CArea::AREA_UV_SIZE_INIT = D3DXVECTOR2(0.25f, 0.25f);
const D3DXVECTOR2 CArea::EACH_PLAYER_DIRECTION[] = { 
	D3DXVECTOR2(-0.5f, -0.5f) ,
	D3DXVECTOR2(0.5f, -0.5f) ,
	D3DXVECTOR2(-0.5f, 0.5f) ,
	D3DXVECTOR2(0.5f, 0.5f) };
const float CArea::AREA_UV_MOVE = AREA_UV_SIZE_INIT.x / 4.0f;
const float CArea::AREA_UV_MOVE_OBLIQUE = sqrtf(AREA_UV_SIZE_INIT.x * AREA_UV_SIZE_INIT.x + AREA_UV_SIZE_INIT.y * AREA_UV_SIZE_INIT.y) / 4.0f;
const float CArea::AREA_SIZE = 800.0f;
const float CArea::AREA_FLOATING_DAMPING_RATE = 0.04f;
const float CArea::ANGLE_EACH_EACH_CHARACTER[] = {
	D3DXToRadian(45.0f),
	D3DXToRadian(315.0f),
	D3DXToRadian(135.0f),
	D3DXToRadian(225.0f)
};

//==============================================================================================
// コンストラクタ
//==============================================================================================
CArea::CArea()
{
	
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CArea::~CArea()
{
}

//==============================================================================================
// 初期化
//==============================================================================================
HRESULT CArea::Init()
{

	//親の初期化
	if (FAILED(C3DPolygon::Init()))
	{
		return E_FAIL;
	}

	//テクスチャの設定
	SetTextIndex(CTexture::TEXTURE_AREA);

	//エリア拡張の回数
	m_nAreaExtensionX = 0;
	m_nAreaExtensionZ = 0;
	m_nOldAreaExtensionX = 0;
	m_nOldAreaExtensionZ = 0;

	//十字の位置
	m_CrossPos = AREA_POS;
	m_CrossPos.y = 0.0f;

	//エリア変動カウントの初期化
	m_nAreaFloatingTime = 0;

	return S_OK;
}

//==============================================================================================
// 終了
//==============================================================================================
//void CArea::Uninit()
//{
//}

//==============================================================================================
// 更新
//==============================================================================================
void CArea::Update()
{
	//エリア変動カウント
	m_nAreaFloatingTime++;

	//カウントチェック
	if (m_nAreaFloatingTime > AREA_FLOATING_TIME)
	{
		//プレイヤーの順位確認
		PlayerRankConfirmation();
		//タイムのリセット
		m_nAreaFloatingTime = 0;
	}
	

	//現在のUVの位置の取得
	D3DXVECTOR2 UV = GetUV();

	if (UV.x < AREA_UV_SIZE_INIT.x)
	{
		m_DestUVPos = D3DXVECTOR2(0.5f, 0.5f);
	}

	//目的のとの距離の計算
	UV = m_DestUVPos - UV;

	//減衰計算
	UV *= AREA_FLOATING_DAMPING_RATE;

	//加算
	AddUVPos(UV);

	//各情報の反映
	SetPolygon();

	//更新されたのUVの位置の再取得
	UV = GetUV();

	//UV割合の計算
	D3DXVECTOR2 Ratio;

	//X
	Ratio.x = (UV.x - AREA_UV_SIZE_INIT.x) / (AREA_UV_SIZE_INIT.x * 2.0f);
	//Y
	Ratio.y = (UV.y - AREA_UV_SIZE_INIT.y) / (AREA_UV_SIZE_INIT.y * 2.0f);

	//十字の位置の計算(3次元対応のためYはZとする)
	m_CrossPos.x = AREA_POS.x * 2.0f - AREA_POS.x * 2.0f * Ratio.x;
	m_CrossPos.z = AREA_POS.z * 2.0f * Ratio.y;

}

//==============================================================================================
// 描画
//==============================================================================================
void CArea::Draw()
{
	
	// デバイスのポインタを取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// レンダリングステートの設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//親の描画
	C3DPolygon::Draw();

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zバッファの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// 設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==============================================================================================
//プレイヤーの順位確認
//==============================================================================================
void CArea::PlayerRankConfirmation()
{
	//エリア変動を行うかどうか
	if (m_nTopCharaNum >= 0)
	{
		AreaFluctuation(m_nTopCharaNum);
	}
}

//==============================================================================================
//エリア変動
//==============================================================================================
void CArea::AreaFluctuation(int nPLNum)
{
	//エリア拡張カウント
	if (nPLNum <= 1)
	{
		m_nAreaExtensionZ--;
	}
	else
	{
		m_nAreaExtensionZ++;
	}

	if (nPLNum % 2 == 0)
	{
		m_nAreaExtensionX--;
	}
	else
	{
		m_nAreaExtensionX++;
	}

	//Xの限界値の判定
	if (m_nAreaExtensionX > AREA_EXTENSION)
	{
		m_nAreaExtensionX = AREA_EXTENSION;
	}
	else if (m_nAreaExtensionX < -AREA_EXTENSION)
	{
		m_nAreaExtensionX = -AREA_EXTENSION;
	}

	//Zの限界値の判定
	if (m_nAreaExtensionZ > AREA_EXTENSION)
	{
		m_nAreaExtensionZ = AREA_EXTENSION;
	}
	else if (m_nAreaExtensionZ < -AREA_EXTENSION)
	{
		m_nAreaExtensionZ = -AREA_EXTENSION;
	}

	//前回との差分
	int nAreaExtensionDifferenceX, nAreaExtensionDifferenceZ;

	//Xの差分
	nAreaExtensionDifferenceX = m_nAreaExtensionX - m_nOldAreaExtensionX;

	//Zの差分
	nAreaExtensionDifferenceZ = m_nAreaExtensionZ - m_nOldAreaExtensionZ;

	//更新対象かどうか
	if (nAreaExtensionDifferenceX == 0 && nAreaExtensionDifferenceZ == 0)
	{
		return;
	}

	//長さ
	float fUVMove = AREA_UV_MOVE;

	//方向にとって長さを変える
	if (nAreaExtensionDifferenceX != 0 && nAreaExtensionDifferenceZ != 0)
	{
		fUVMove = AREA_UV_MOVE_OBLIQUE;
	}

	D3DXVECTOR2 ExtendedDirection = D3DXVECTOR2((float)nAreaExtensionDifferenceX,(float)nAreaExtensionDifferenceZ);

	//長さを1にする
	D3DXVec2Normalize(&ExtendedDirection, &ExtendedDirection);

	//目的位置の設定
	m_DestUVPos += ExtendedDirection * fUVMove;

	//Oldの更新
	m_nOldAreaExtensionX = m_nAreaExtensionX;
	m_nOldAreaExtensionZ = m_nAreaExtensionZ;

}

//==============================================================================================
// 陣地内かどうか
//==============================================================================================
bool CArea::InCheckTerritory(D3DXVECTOR3 Pos, int nCharacterNum)
{
	return CheckInAngle(Pos, ANGLE_EACH_EACH_CHARACTER[nCharacterNum],90.0f, m_CrossPos);
}

//==============================================================================================
// 生成処理
//==============================================================================================
CArea *CArea::Create()
{
	//ポインタ変数宣言
	CArea *pArea = nullptr;

	//情報の確保
	pArea = new CArea;

	//NULLチェック
	if (pArea == nullptr)
	{
		assert(false);
	}

	//初期化
	if (FAILED(pArea->Init()))
	{
		assert(false);
	}

	//各情報の登録
	pArea->SetPos(AREA_POS);
	pArea->SetRot(D3DXVECTOR3(0.0f,0.0f,0.0f));
	pArea->SetDiagonalLine(AREA_SIZE, AREA_SIZE);
	pArea->SetColor(D3DXCOLOR(1.0f,1.0f,1.0f,0.3f));
	pArea->SetUVSize(AREA_UV_SIZE_INIT);
	pArea->SetPolygon();
	pArea->SetDestUVPos(pArea->GetUV());

	return pArea;
}