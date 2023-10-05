//=============================================================================
//
// object3D.h
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object3D.h"
#include "object2D.h"
#include "rendering.h"
#include "application.h"

//静的メンバー変数の宣言
//LPDIRECT3DTEXTURE9 CObject_3D::m_pTextureAll[CObject_3D::TextTypeMax] = {};

char* CObject_3D::m_paTextPass[CObject::TEXTURE_TYPE_MAX] =
{

};

//=============================================================================
//コンストラクタ
//=============================================================================
CObject_3D::CObject_3D()
{
	m_pVtxBuff = nullptr;								//頂点バッファ
	m_size = D3DXVECTOR2(0.0f, 0.0f);					//ポリゴンの幅と高さ
	m_dir = Vec3Null;									//ポリゴンの向き
	m_move = Vec3Null;									//移動量
	m_acc = Vec3Null;									//加速
	m_rot = Vec3Null;									//ポリゴンの回転角度
	m_fAngle = 0.0f;									//ポリゴンの回転用の角度
	m_fFrameRot = Vec3Null;								//1フレームの回転角度
	m_pos = Vec3Null;									//ポリゴンの位置
	m_nCountAnim = 0;									//現在のテクスチャパターン
	m_nAnimPattern = 0;									//アニメーションカウンター
	m_nMaxTexPattern = 0;								//テクスチャのパターン数
	m_nMaxTexColumn = 0;								//テクスチャの行数
	m_nTexLine = 0;										//テクスチャの列数
	m_nFirstPattern = 0;								//アニメーションの最初のパターン
	m_nAnimFrame = 0;									//アニメーションパターンの変更フレーム数
	m_textureTranslation = D3DXVECTOR2(0.0f, 0.0f);		//テクスチャの移動量
	m_bFlipX = false;									//テクスチャのX座標が反転しているかどうか
	m_bFlipY = false;									//テクスチャのY座標が反転しているかどうか
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);			//カラー
	m_textType = CObject::TEXTURE_TYPE_MAX;				//テクスチャの種類
	m_pTexture = nullptr;								//テクスチャへのポインタ

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_VtxCol[nCnt] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//頂点カラー
	}
}

CObject_3D::CObject_3D(const int nPriority) : CObject::CObject(nPriority)
{
	m_pVtxBuff = nullptr;								//頂点バッファ
	m_size = D3DXVECTOR2(0.0f, 0.0f);					//ポリゴンの幅と高さ
	m_dir = Vec3Null;									//ポリゴンの向き
	m_move = Vec3Null;									//移動量
	m_acc = Vec3Null;									//加速
	m_rot = Vec3Null;									//ポリゴンの回転角度
	m_fAngle = 0.0f;									//ポリゴンの回転用の角度
	m_fFrameRot = Vec3Null;								//1フレームの回転角度
	m_pos = Vec3Null;									//ポリゴンの位置
	m_nCountAnim = 0;									//現在のテクスチャパターン
	m_nAnimPattern = 0;									//アニメーションカウンター
	m_nMaxTexPattern = 0;								//テクスチャのパターン数
	m_nMaxTexColumn = 0;								//テクスチャの行数
	m_nTexLine = 0;										//テクスチャの列数
	m_nFirstPattern = 0;								//アニメーションの最初のパターン
	m_nAnimFrame = 0;									//アニメーションパターンの変更フレーム数
	m_textureTranslation = D3DXVECTOR2(0.0f, 0.0f);		//テクスチャの移動量
	m_bFlipX = false;									//テクスチャのX座標が反転しているかどうか
	m_bFlipY = false;									//テクスチャのY座標が反転しているかどうか
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);			//カラー
	m_textType = CObject::TEXTURE_TYPE_MAX;				//テクスチャの種類
	m_pTexture = nullptr;								//テクスチャへのポインタ

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_VtxCol[nCnt] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//頂点カラー
	}
}

//=============================================================================
//デストラクタ
//=============================================================================
CObject_3D::~CObject_3D()
{

}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT CObject_3D::Init(void)
{
	CRenderer* Render = CApplication::GetRenderer();			//レンディングインスタンスの取得処理
	LPDIRECT3DDEVICE9 pDevice = Render->GetDevice();			//デバイスの取得処理

	m_size = D3DXVECTOR2(0.0f, 0.0f);							//ポリゴンの幅と高さの設定
	m_dir = D3DXVECTOR3(1.0f, 0.0f, 0.0f);						//ポリゴンの向き
	m_move = Vec3Null;											//移動量
	m_acc = Vec3Null;											//加速
	m_rot = Vec3Null;											//回転角度の設定
	m_fAngle = 0.0f;											//回転用の角度を計算する
	m_fFrameRot = Vec3Null;										//1フレームの回転角度
	m_pos = Vec3Null;											//位置の設定
	m_nCountAnim = 0;											//現在のテクスチャパターン
	m_nAnimPattern = 0;											//アニメーションカウンター
	m_nMaxTexPattern = 1;										//テクスチャのパターン数
	m_nMaxTexColumn = 1;										//テクスチャの行数
	m_nTexLine = 1;												//テクスチャの列数
	m_nFirstPattern = 0;										//アニメーションの最初のパターン
	m_textureTranslation = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャの移動量
	m_bFlipX = false;											//テクスチャのX座標が反転しているかどうか
	m_bFlipY = false;											//テクスチャのY座標が反転しているかどうか
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);					//カラー
	m_nAnimFrame = 0;											//アニメーションパターンの変更フレーム数
	m_pTexture = nullptr;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_VtxCol[nCnt] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//頂点カラー
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D* pVtx = nullptr;					//頂点情報へのポインタ

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		// 法線の設定
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// 頂点カラーの設定
		pVtx[nCnt].col = m_col;
	}

	SetTexture(TEXTURE_NULL);						//テクスチャの設定
	SetTextureParameter(1, 1, 1, INT_MAX);			//テクスチャパラメータの設定

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了
//=============================================================================
void CObject_3D::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//テクスチャへのポインタをnullにする
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}
}

//=============================================================================
// ポリゴンの更新
//=============================================================================
void CObject_3D::Update()
{
	if (m_textureTranslation != Vec2Null)
	{
		VERTEX_3D* pVtx = nullptr;					//頂点情報へのポインタ
		VERTEX_3D  Vtx = {};

		//頂点バッファのロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			Vtx = pVtx[nCnt];
			Vtx.tex += m_textureTranslation;
			pVtx[nCnt] = Vtx;
		}

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// ポリゴンの描画
//=============================================================================
void CObject_3D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();							//デバイスの取得処理
	D3DXMATRIX	mtxRot, mtxTrans;					//計算用マトリックス

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ワルドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワルドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//四角形を描画する
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//テクスチャの設定
	pDevice->SetTexture(0, nullptr);
}

//=============================================================================
//位置の設定処理
//=============================================================================
void CObject_3D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//ポリゴンの幅と高さの設定処理
void CObject_3D::SetSize(const D3DXVECTOR2 dim)
{
	m_size = dim;
	m_fAngle = atan2f(m_size.y, m_size.x);

	VERTEX_3D* pVtx = nullptr;					//頂点情報へのポインタ

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-dim.x, dim.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(dim.x, dim.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-dim.x, -dim.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(dim.x, -dim.y, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//ポリゴンの幅と高さの設定処理
void CObject_3D::SetSize(const float x, const float y)
{
	m_size.x = x;
	m_size.y = y;
	m_fAngle = atan2f(m_size.y, m_size.x);

	VERTEX_3D* pVtx = nullptr;					//頂点情報へのポインタ

												//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-x, y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(x, y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-x, -y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(x, -y, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}


//速度の取得処理
const D3DXVECTOR3 CObject_3D::GetMove(void)
{
	return m_move;
}

//速度の設定処理
void CObject_3D::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//加速の取得処理
const D3DXVECTOR3 CObject_3D::GetAcceleration(void)
{
	return m_acc;
}

//加速の設定処理
void CObject_3D::SetAcceleration(const D3DXVECTOR3 acc)
{
	m_acc = acc;
}

//サイズの取得処理
const D3DXVECTOR2 CObject_3D::GetSize(void)
{
	return m_size;
}

//回転の設定処理
void CObject_3D::SetRotation(const D3DXVECTOR3 frameRot)
{
	m_fFrameRot = frameRot;
}

//回転速度の加算処理
void CObject_3D::AddAngularSpeed(const D3DXVECTOR3 speed)
{
	m_fFrameRot += speed;
}

//回転速度の取得処理
const D3DXVECTOR3 CObject_3D::GetAngularSpeed(void)
{
	return m_fFrameRot;
}

//回転角度の初期値の取得処理
void CObject_3D::SetStartingRot(const D3DXVECTOR3 fStartRot)
{
	m_rot = fStartRot;
}

//回転角度の取得処理 
const D3DXVECTOR3 CObject_3D::GetRot(void)
{
	return m_rot;
}

//オブジェクトの位置取得処理
const D3DXVECTOR3 CObject_3D::GetPos(void)
{
	return m_pos;
}

//テクスチャアニメーション用のパラメータ設定処理
void CObject_3D::SetTextureParameter(const int MaxPattern, const int LinePattern, const int ColumnPattern, const int nAnimFrame)
{
	m_nMaxTexColumn = ColumnPattern;				//行数の設定
	m_nTexLine = LinePattern;						//列数の設定
	m_nMaxTexPattern = MaxPattern;					//アニメーションパターンの最大数の設定
	m_nAnimFrame = nAnimFrame;						//アニメーションフレーム数の設定

	VERTEX_3D* pVtx = nullptr;

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	UpdateTexture();

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//現在のアニメーションパターンの取得処理
const int CObject_3D::GetPresentAnimPattern(void)
{
	return m_nAnimPattern;				//アニメーションパターンを返す
}

void CObject_3D::SetAnimPattern(const int PatternNum)
{
	m_nAnimPattern = PatternNum;
}

bool CObject_3D::GetFlipX(void)
{
	return m_bFlipX;
}

void CObject_3D::FlipX(void)
{
	m_bFlipX = !m_bFlipX;

	VERTEX_3D* pVtx = nullptr;
	VERTEX_3D Vtx;

	ZeroMemory(&Vtx, sizeof(VERTEX_2D));

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	Vtx.tex = pVtx[0].tex;
	pVtx[0].tex = pVtx[1].tex;
	pVtx[1].tex = Vtx.tex;
	Vtx.tex = pVtx[2].tex;
	pVtx[2].tex = pVtx[3].tex;
	pVtx[3].tex = Vtx.tex;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CObject_3D::FlipY(void)
{
	m_bFlipY = !m_bFlipY;

	VERTEX_3D* pVtx = nullptr;
	VERTEX_3D Vtx;

	ZeroMemory(&Vtx, sizeof(VERTEX_2D));

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	Vtx.tex = pVtx[0].tex;
	pVtx[0].tex = pVtx[2].tex;
	pVtx[2].tex = Vtx.tex;
	Vtx.tex = pVtx[1].tex;
	pVtx[1].tex = pVtx[3].tex;
	pVtx[3].tex = Vtx.tex;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//カーラーの取得処理
const D3DXCOLOR CObject_3D::GetColor(void)
{
	return m_col;
}

//カーラーの設定処理
void CObject_3D::SetColor(const D3DXCOLOR col)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxbuff();		//頂点バッファの取得処理
	VERTEX_3D* pVtx = nullptr;								//頂点情報へのポインタ

	m_col = col;											//カーラーの設定

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_VtxCol[nCnt] = col;
	}

	//頂点バッファのロック
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//頂点１つのカーラー設定処理
void CObject_3D::SetVtxColor(const int nVtxIdx, const D3DXCOLOR col)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxbuff();		//頂点バッファの取得処理
	VERTEX_3D* pVtx = nullptr;								//頂点情報へのポインタ

	m_VtxCol[nVtxIdx] = col;

	//頂点バッファのロック
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//カラーの設定
	pVtx[nVtxIdx].col = m_VtxCol[nVtxIdx];

	//頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//テクスチャの移動量の設定処理
void CObject_3D::MoveTexCoordinates(const D3DXVECTOR2 move)
{
	m_textureTranslation = move;
}

//テクスチャの種類の設定処理
void CObject_3D::SetTexture(TextType textType)
{
	LPDIRECT3DTEXTURE9 text = CObject_2D::GetTexturePointer(textType);
	m_pTexture = text;														//テクスチャの種類の設定
}

//アニメーションの最初のパターンの設定処理
void CObject_3D::SetAnimationBase(const int FirstPattern)
{
	m_nFirstPattern = FirstPattern;
}

//頂点バッファの取得処理
LPDIRECT3DVERTEXBUFFER9 CObject_3D::GetVtxbuff(void)
{
	return m_pVtxBuff;
}

//生成処理
CObject_3D* CObject_3D::Create(void)
{
	CObject_3D* pObj3D = nullptr;

	pObj3D = new CObject_3D;

	if (FAILED(pObj3D->Init()))
	{
		if (pObj3D != nullptr)
		{
			delete pObj3D;
			pObj3D = nullptr;
		}

		return nullptr;
	}

	return pObj3D;
}


void CObject_3D::UpdateTexture(void)
{
	VERTEX_3D* pVtx = nullptr;					//頂点情報へのポインタ

												//頂点バッファをロックする
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fX = 1.0f / m_nTexLine;
	float fY = 1.0f / m_nMaxTexColumn;

	if (!m_bFlipX && !m_bFlipY)
	{
		pVtx[0].tex = D3DXVECTOR2(m_textureTranslation.x + 0.0f + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + 0.0f + fY * (m_nAnimPattern / m_nTexLine));
		pVtx[1].tex = D3DXVECTOR2(m_textureTranslation.x + fX + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + 0.0f + fY * (m_nAnimPattern / m_nTexLine));
		pVtx[2].tex = D3DXVECTOR2(m_textureTranslation.x + 0.0f + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + fY + fY * (m_nAnimPattern / m_nTexLine));
		pVtx[3].tex = D3DXVECTOR2(m_textureTranslation.x + fX + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + fY + fY * (m_nAnimPattern / m_nTexLine));
	}
	else if (m_bFlipX && !m_bFlipY)
	{
		pVtx[1].tex = D3DXVECTOR2(m_textureTranslation.x + 0.0f + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + 0.0f + fY * (m_nAnimPattern / m_nTexLine));
		pVtx[0].tex = D3DXVECTOR2(m_textureTranslation.x + fX + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + 0.0f + fY * (m_nAnimPattern / m_nTexLine));
		pVtx[3].tex = D3DXVECTOR2(m_textureTranslation.x + 0.0f + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + fY + fY * (m_nAnimPattern / m_nTexLine));
		pVtx[2].tex = D3DXVECTOR2(m_textureTranslation.x + fX + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + fY + fY * (m_nAnimPattern / m_nTexLine));
	}
	else if (!m_bFlipX && m_bFlipY)
	{
		pVtx[2].tex = D3DXVECTOR2(m_textureTranslation.x + 0.0f + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + 0.0f + fY * (m_nAnimPattern / m_nTexLine));
		pVtx[3].tex = D3DXVECTOR2(m_textureTranslation.x + fX + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + 0.0f + fY * (m_nAnimPattern / m_nTexLine));
		pVtx[0].tex = D3DXVECTOR2(m_textureTranslation.x + 0.0f + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + fY + fY * (m_nAnimPattern / m_nTexLine));
		pVtx[1].tex = D3DXVECTOR2(m_textureTranslation.x + fX + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + fY + fY * (m_nAnimPattern / m_nTexLine));
	}
	else
	{
		pVtx[3].tex = D3DXVECTOR2(m_textureTranslation.x + 0.0f + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + 0.0f + fY * (m_nAnimPattern / m_nTexLine));
		pVtx[2].tex = D3DXVECTOR2(m_textureTranslation.x + fX + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + 0.0f + fY * (m_nAnimPattern / m_nTexLine));
		pVtx[1].tex = D3DXVECTOR2(m_textureTranslation.x + 0.0f + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + fY + fY * (m_nAnimPattern / m_nTexLine));
		pVtx[0].tex = D3DXVECTOR2(m_textureTranslation.x + fX + fX * (m_nAnimPattern % m_nTexLine), m_textureTranslation.y + fY + fY * (m_nAnimPattern / m_nTexLine));
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}