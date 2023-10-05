//=============================================================================
//
// object2D.h
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object2D.h"
#include "rendering.h"
#include "application.h"

//静的メンバー変数の宣言
LPDIRECT3DTEXTURE9 CObject_2D::m_pTextureAll[CObject_2D::TEXTURE_TYPE_MAX] = {};

char* CObject_2D::m_paTextPass[CObject::TEXTURE_TYPE_MAX] =
{
	" ",													//TextureNull

	"data\\TEXTURE\\Block.png",								//TEXTURE_BLOCK,
	"data\\TEXTURE\\sky002.jpg",							//TEXTURE_SKY,
	"data\\TEXTURE\\89_m.jpg",								//TEXTURE_GRASS,
	"data\\TEXTURE\\MeshField001.png",						//TEXTURE_ROCK,
	"data\\TEXTURE\\MagmaArea.png",							//TEXTURE_MAGMA,
	"data\\TEXTURE\\snow.png",								//TEXTURE_SNOW,
	"data\\TEXTURE\\snowarea.png",							//TEXTURE_SNOW_AREA,
	"data\\TEXTURE\\Characters.png",						//TEXTURE_CHARACTERS,
	"data\\TEXTURE\\Alphabet.png",							//TEXTURE_LETTERS,
	"data\\TEXTURE\\Numbers.png",							//TEXTURE_NUMBERS,
	"data\\TEXTURE\\Japanese2000unicode.png",				//TEXTURE_JAPANESE,
	"data\\TEXTURE\\sele.png",								//TEXTURE_CHARASET_FRAM,
	"data\\TEXTURE\\UI_Debug_Start.png",					//TEXTURE_MESSAGE_START,
	"data\\TEXTURE\\UI_Debug_Goal.png",						//TEXTURE_MESSAGE_GOAL,
	"data\\TEXTURE\\UI_PlayerNumber_1P.png",				//TEXTURE_1P_WIN,
	"data\\TEXTURE\\UI_PlayerNumber_2P.png",				//TEXTURE_2P_WIN,
	"data\\TEXTURE\\UI_PlayerNumber_3P.png",				//TEXTURE_3P_WIN,
	"data\\TEXTURE\\UI_PlayerNumber_4P.png",				//TEXTURE_4P_WIN,
	"data\\TEXTURE\\UI_GameResult_Win.png",					//TEXTURE_WINNER,
	"data\\TEXTURE\\UI_GameResult_Draw.png",				//TEXTURE_DRAW,
	"data\\TEXTURE\\Frame.png",								//TEXTURE_SELECT_FRAME,
	"data\\TEXTURE\\Lava.jpg",								//TEXTURE_LAVA,
	"data\\TEXTURE\\ice.png",								//TEXTURE_ICE,
	"data\\TEXTURE\\se.png",								//TEXTURE_BACKGROUND,
	"data\\TEXTURE\\Particle02.png",						//TEXTURE_PARTICLE_EFFECT,
	"data\\TEXTURE\\StageSelect\\stage01.png",				//TEXTURE_STAGESEL01,
	"data\\TEXTURE\\StageSelect\\stage02.png",				//TEXTURE_STAGESEL02,
	"data\\TEXTURE\\StageSelect\\stage03.png",				//TEXTURE_STAGESEL03,
	"data\\TEXTURE\\StageSelect\\stageR.png",				//TEXTURE_STAGESELR,
	"data\\TEXTURE\\UI_OK.png",								//TEXTURE_OK_UI,
	"data\\TEXTURE\\UI_TitleLogo.png",						//TEXTURE_TITLE_UI,
	"data\\TEXTURE\\unve.png",								//TEXTURE_STAGE_SELECT_BG,
	"data\\TEXTURE\\ButtonPress.png",						//TEXTURE_BUTTON_SELECT_UI,
	"data\\TEXTURE\\TutorialMove.png",						//TEXTURE_TUTORIAL_MOVE,
	"data\\TEXTURE\\TutorialButton.png",					//TEXTURE_TUTORIAL_BUTTON,
	"data\\TEXTURE\\PlayerSelCtrl.png",						//TEXTURE_PLAYER_SEL_CTRL,
};

//=============================================================================
//コンストラクタ
//=============================================================================
CObject_2D::CObject_2D()
{
	//メンバー変数をクリアする
	m_pVtxBuff = nullptr;								//頂点バッファ
	m_size = D3DXVECTOR2(0.0f, 0.0f);					//ポリゴンの幅と高さ
	m_dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//ポリゴンの向き
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//移動量
	m_acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//加速
	m_fRot = 0.0f;										//ポリゴンの回転角度
	m_fAngle = 0.0f;									//ポリゴンの回転用の角度
	m_fFrameRot = 0.0f;									//1フレームの回転角度
	m_revolutionCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//公転の中心点
	m_fRevolutionSpeed = 0.0f;							//公転のスピード
	m_fRevolutionAngle = 0.0f;							//公転の現在角度
	m_fRevolutionRadius = 0.0f;							//公転の半径
	m_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//ポリゴンの位置
	m_nCountAnim = 0;									//現在のテクスチャパターン
	m_nAnimPattern = 0;									//アニメーションカウンター
	m_nMaxTexPattern = 0;								//テクスチャのパターン数
	m_nMaxTexColumn = 0;								//テクスチャの行数
	m_nTexLine = 0;										//テクスチャの列数
	m_nFirstPattern = 0;								//最初のアニメーションパターン
	m_nAnimFrame = 0;									//アニメーションパターンの変更フレーム数
	m_textureTranslation = D3DXVECTOR2(0.0f, 0.0f);		//テクスチャの移動量
	m_textureAnimSpeed = Vec2Null;						//テクスチャの移動速度
	m_bFlipX = false;									//テクスチャのX座標が反転しているかどうか
	m_bFlipY = false;									//テクスチャのY座標が反転しているかどうか
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);			//頂点カーラー
	m_textType = CObject::TEXTURE_TYPE_MAX;				//テクスチャの種類

	//頂点カラー
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_VtxCol[nCnt] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
}	

CObject_2D::CObject_2D(const int nPriority) : CObject::CObject(nPriority)
{
	//メンバー変数をクリアする
	m_pVtxBuff = nullptr;								//頂点バッファ
	m_size = D3DXVECTOR2(0.0f, 0.0f);					//ポリゴンの幅と高さ
	m_dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//ポリゴンの向き
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//移動量
	m_acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//加速
	m_fRot = 0.0f;										//ポリゴンの回転角度
	m_fAngle = 0.0f;									//ポリゴンの回転用の角度
	m_fFrameRot = 0.0f;									//1フレームの回転角度
	m_revolutionCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//公転の中心点
	m_fRevolutionSpeed = 0.0f;							//公転のスピード
	m_fRevolutionAngle = 0.0f;							//公転の現在角度
	m_fRevolutionRadius = 0.0f;							//公転の半径
	m_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//ポリゴンの位置
	m_nCountAnim = 0;									//現在のテクスチャパターン
	m_nAnimPattern = 0;									//アニメーションカウンター
	m_nMaxTexPattern = 0;								//テクスチャのパターン数
	m_nMaxTexColumn = 0;								//テクスチャの行数
	m_nTexLine = 0;										//テクスチャの列数
	m_nFirstPattern = 0;								//最初のアニメーションパターン
	m_nAnimFrame = 0;									//アニメーションパターンの変更フレーム数
	m_textureTranslation = D3DXVECTOR2(0.0f, 0.0f);		//テクスチャの移動量
	m_textureAnimSpeed = Vec2Null;						//テクスチャの移動速度
	m_bFlipX = false;									//テクスチャのX座標が反転しているかどうか
	m_bFlipY = false;									//テクスチャのY座標が反転しているかどうか
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);			//頂点カラー
	m_textType = CObject::TEXTURE_TYPE_MAX;				//テクスチャの種類

	//頂点カラー
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_VtxCol[nCnt] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
//デストラクタ
//=============================================================================
CObject_2D::~CObject_2D()
{

}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT CObject_2D::Init(void)
{
	//初期化処理
	CRenderer* Render = CApplication::GetRenderer();			//レンディングインスタンスの取得処理
	LPDIRECT3DDEVICE9 pDevice = Render->GetDevice();			//デバイスの取得処理

	m_size = D3DXVECTOR2(0.0f, 0.0f);							//ポリゴンの幅と高さの設定
	m_dir = D3DXVECTOR3(1.0f, 0.0f, 0.0f);						//ポリゴンの向き
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//移動量
	m_acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//加速
	m_fRot = 0.0f;												//回転角度の設定
	m_fAngle = 0.0f;											//回転用の角度を計算する
	m_fFrameRot = 0.0f;											//1フレームの回転角度
	m_revolutionCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//公転の中心点
	m_fRevolutionSpeed = 0.0f;									//公転のスピード
	m_fRevolutionAngle = 0.0f;									//公転の現在角度
	m_fRevolutionRadius = 0.0f;									//公転の半径
	m_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//位置の設定
	m_nCountAnim = 0;											//現在のテクスチャパターン
	m_nAnimPattern = 0;											//アニメーションカウンター
	m_nMaxTexPattern = 1;										//テクスチャのパターン数
	m_nMaxTexColumn = 1;										//テクスチャの行数
	m_nTexLine = 1;												//テクスチャの列数
	m_nFirstPattern = 0;										//最初のアニメーションパターン
	m_textureTranslation = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャの移動量
	m_textureAnimSpeed = Vec2Null;								//テクスチャの移動速度
	m_bFlipX = false;											//テクスチャのX座標が反転しているかどうか
	m_bFlipY = false;											//テクスチャのY座標が反転しているかどうか
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);					//ポリゴンのカラー
	m_nAnimFrame = 0;											//アニメーションパターンの変更フレーム数

	//頂点カラー
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_VtxCol[nCnt] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D* pVtx = nullptr;					//頂点情報へのポインタ

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos.x = (m_posPolygon.x) + sinf(m_fRot + (m_fAngle + 2.0f * (D3DX_PI * 0.5f - m_fAngle))) * (m_size.x);
	pVtx[0].pos.y = (m_posPolygon.y) + cosf(m_fRot + (m_fAngle + 2.0f * (D3DX_PI * 0.5f - m_fAngle))) * (m_size.x);
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = (m_posPolygon.x) + sinf(m_fRot + m_fAngle) * (m_size.x);
	pVtx[1].pos.y = (m_posPolygon.y) + cosf(m_fRot + m_fAngle) * (m_size.x);
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = (m_posPolygon.x) + sinf(m_fRot - (m_fAngle + 2.0f * (D3DX_PI * 0.5f - m_fAngle))) * (m_size.x);
	pVtx[2].pos.y = (m_posPolygon.y) + cosf(m_fRot - (m_fAngle + 2.0f * (D3DX_PI * 0.5f - m_fAngle))) * (m_size.x);
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = (m_posPolygon.x) + sinf(m_fRot - m_fAngle) * (m_size.x);
	pVtx[3].pos.y = (m_posPolygon.y) + cosf(m_fRot - m_fAngle) * (m_size.x);
	pVtx[3].pos.z = 0.0f;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		// rhwの設定
		pVtx[nCnt].rhw = 1.0f;
		// 頂点カラーの設定
		pVtx[nCnt].col = m_col;
	}

	SetTexture(TEXTURE_NULL);					//テクスチャの設定
	SetTextureParameter(1, 1, 1, INT_MAX);		//テクスチャパラメータの設定

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了
//=============================================================================
void CObject_2D::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================================
// ポリゴンの更新
//=============================================================================
void CObject_2D::Update()
{
	VERTEX_2D* pVtx = nullptr;					//頂点情報へのポインタ

	//頂点バッファをロックする
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_nCountAnim++;								//アニメーションカウンターの更新

	if (m_nCountAnim >= m_nAnimFrame)
	{
		m_nCountAnim = 0;						//アニメーションカウンターを0に戻す
		m_nAnimPattern++;						//アニメーションパターンの更新

		if (m_nAnimPattern >= m_nFirstPattern + m_nMaxTexPattern)
		{//アニメーションの最後のパターンを超えた場合
			m_nAnimPattern = m_nFirstPattern;					//アニメーションパターンを戻す
		}
	}

	if (m_fRevolutionRadius != 0.0f)
	{//公転速度が0ではなかったら
		m_revolutionCenter += GetMove();			//中心点座標の更新

		//位置の更新
		m_posPolygon.x = m_revolutionCenter.x + (cosf(m_fRevolutionAngle) * (m_fRevolutionRadius));
		m_posPolygon.y = m_revolutionCenter.y + (sinf(m_fRevolutionAngle) * (m_fRevolutionRadius));
	}

	float dim = sqrtf((m_size.x * m_size.x) + (m_size.y * m_size.y));

	//頂点座標の更新
	pVtx[0].pos.x = (m_posPolygon.x) + sinf(D3DX_PI * 0.5f + m_fRot + (m_fAngle + 2.0f * (D3DX_PI * 0.5f - m_fAngle))) * (dim);
	pVtx[0].pos.y = (m_posPolygon.y) + cosf(D3DX_PI * 0.5f + m_fRot + (m_fAngle + 2.0f * (D3DX_PI * 0.5f - m_fAngle))) * (dim);
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = (m_posPolygon.x) + sinf(D3DX_PI * 0.5f + m_fRot + m_fAngle) * (dim);
	pVtx[1].pos.y = (m_posPolygon.y) + cosf(D3DX_PI * 0.5f + m_fRot + m_fAngle) * (dim);
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = (m_posPolygon.x) + sinf(D3DX_PI * 0.5f + m_fRot - (m_fAngle + 2.0f * (D3DX_PI * 0.5f - m_fAngle))) * (dim);
	pVtx[2].pos.y = (m_posPolygon.y) + cosf(D3DX_PI * 0.5f + m_fRot - (m_fAngle + 2.0f * (D3DX_PI * 0.5f - m_fAngle))) * (dim);
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = (m_posPolygon.x) + sinf(D3DX_PI * 0.5f + m_fRot - m_fAngle) * (dim);
	pVtx[3].pos.y = (m_posPolygon.y) + cosf(D3DX_PI * 0.5f + m_fRot - m_fAngle) * (dim);
	pVtx[3].pos.z = 0.0f;

	m_fRevolutionAngle += m_fRevolutionSpeed;

	if (m_textureAnimSpeed != Vec2Null)
	{
		m_textureTranslation += m_textureAnimSpeed;
	}

	UpdateTexture();				//テクスチャの更新

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	m_posPolygon += m_move;
	m_move += m_acc;

	//回転角度の更新
	m_fRot += m_fFrameRot;
	if (m_fRot >= D3DX_PI * 2.0f)
	{
		m_fRot -= D3DX_PI * 2.0f;
	}
}

//=============================================================================
// ポリゴンの描画
//=============================================================================
void CObject_2D::Draw()
{
	CRenderer* Render = CApplication::GetRenderer();							//レンディングインスタンスの取得処理
	LPDIRECT3DDEVICE9 pDevice = Render->GetDevice();							//デバイスの取得処理

	//頂点バッファをデータストリームに設定する
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTextureAll[m_textType]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//テクスチャの設定
	pDevice->SetTexture(0, nullptr);
}

//=============================================================================
//位置の設定処理
//=============================================================================
void CObject_2D::SetPos(const D3DXVECTOR3 pos)
{
	m_posPolygon = pos;
}

//ポリゴンの幅と高さの設定処理
void CObject_2D::SetSize(const D3DXVECTOR2 dim)
{
	m_size = dim;
	m_fAngle = atan2f(m_size.y, m_size.x);
}

//ポリゴンの幅と高さの設定処理
void CObject_2D::SetSize(const float x, const float y)
{
	m_size.x = x;
	m_size.y = y;
	m_fAngle = atan2f(m_size.y, m_size.x);
}

//速度の取得処理
const D3DXVECTOR3 CObject_2D::GetMove(void)
{
	return m_move;
}

//速度の設定処理
void CObject_2D::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//加速の取得処理
const D3DXVECTOR3 CObject_2D::GetAcceleration(void)
{
	return m_acc;
}

//加速の設定処理
void CObject_2D::SetAcceleration(const D3DXVECTOR3 acc)
{
	m_acc = acc;
}

//サイズの取得処理
const D3DXVECTOR2 CObject_2D::GetSize(void)
{
	return m_size;
}

//回転の設定処理
void CObject_2D::SetRotation(const float fFrameRot)
{
	m_fFrameRot = fFrameRot;
}

//回転速度の加算処理
void CObject_2D::AddAngularSpeed(const float fSpeed)
{
	m_fFrameRot += fSpeed;
}

//回転速度の取得処理
const float CObject_2D::GetAngularSpeed(void)
{
	return m_fFrameRot;
}

//回転角度の初期値の取得処理
void CObject_2D::SetStartingRot(const float fStartRot)
{
	m_fRot = fStartRot;
}

//回転角度の取得処理 
const float CObject_2D::GetRot(void)
{
	return m_fRot;
}

//公転の設定処理
void CObject_2D::SetRevolution(const D3DXVECTOR3 Center,
	const float fAngularSpeed, const float fRadius)
{
	m_revolutionCenter = Center;
	m_fRevolutionSpeed = fAngularSpeed;
	m_fRevolutionRadius = fRadius;
}

//公転の中心点の設定処理
void CObject_2D::SetRevolutionCenter(const D3DXVECTOR3 Center)
{
	m_revolutionCenter = Center;
}

void CObject_2D::SetPresentRevolutionAngle(const float fAngle)
{
	m_fRevolutionAngle = fAngle;
}

//公転の中心点の取得処理
const D3DXVECTOR3 CObject_2D::GetRevolutionCenter(void)
{
	return m_revolutionCenter;
}

void CObject_2D::AddRevolutionSpeed(const float fSpeed)
{
	m_fRevolutionSpeed += fSpeed;
}

//オブジェクトの位置取得処理
const D3DXVECTOR3 CObject_2D::GetPos(void)
{
	return m_posPolygon;
}

//テクスチャアニメーション用のパラメータ設定処理
void CObject_2D::SetTextureParameter(const int MaxPattern, const int LinePattern, const int ColumnPattern, const int nAnimFrame)
{
	m_nMaxTexColumn = ColumnPattern;				//行数の設定
	m_nTexLine = LinePattern;						//列数の設定
	m_nMaxTexPattern = MaxPattern;					//アニメーションパターンの最大数の設定
	m_nAnimFrame = nAnimFrame;						//アニメーションフレーム数の設定

	VERTEX_2D* pVtx = nullptr;

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	UpdateTexture();

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//現在のアニメーションパターンの取得処理
const int CObject_2D::GetPresentAnimPattern(void)
{
	return m_nAnimPattern;				//アニメーションパターンを返す
}

void CObject_2D::SetAnimPattern(const int PatternNum)
{
	m_nAnimPattern = PatternNum;
}

void CObject_2D::SetAnimationBase(const int FirstPattern)
{
	m_nFirstPattern = FirstPattern;
}

void CObject_2D::MoveTexCoordinates(const D3DXVECTOR2 move)
{
	m_textureAnimSpeed = move;
}

bool CObject_2D::GetFlipX(void)
{
	return m_bFlipX;
}

void CObject_2D::FlipX(void)
{
	m_bFlipX = !m_bFlipX;

	VERTEX_2D* pVtx = nullptr;
	VERTEX_2D Vtx;

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

void CObject_2D::FlipY(void)
{
	m_bFlipY = !m_bFlipY;

	VERTEX_2D* pVtx = nullptr;
	VERTEX_2D Vtx;

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
const D3DXCOLOR CObject_2D::GetColor(void)
{
	return m_col;
}

//カーラーの設定処理
void CObject_2D::SetColor(const D3DXCOLOR col)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxbuff();		//頂点バッファの取得処理
	VERTEX_2D* pVtx = nullptr;								//頂点情報へのポインタ

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
void CObject_2D::SetVtxColor(const int nVtxIdx, const D3DXCOLOR col)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxbuff();		//頂点バッファの取得処理
	VERTEX_2D* pVtx = nullptr;								//頂点情報へのポインタ

	m_VtxCol[nVtxIdx] = col;

	//頂点バッファのロック
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//カラーの設定
	pVtx[nVtxIdx].col = m_VtxCol[nVtxIdx];

	//頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//テクスチャの種類の設定処理
void CObject_2D::SetTexture(TextType textType)
{
	m_textType = textType;				//テクスチャの種類の設定
}

//頂点バッファの取得処理
LPDIRECT3DVERTEXBUFFER9 CObject_2D::GetVtxbuff(void)
{
	return m_pVtxBuff;
}

//テクスチャ全部のロード処理
void CObject_2D::LoadTextures(void)
{
	CRenderer* Render = CApplication::GetRenderer();			//レンディングインスタンスの取得処理
	LPDIRECT3DDEVICE9 pDevice = Render->GetDevice();			//デバイスの取得処理

	for (int nCnt = 0; nCnt < TEXTURE_TYPE_MAX; nCnt++)
	{
		if (m_pTextureAll[nCnt] != nullptr)
		{//ポインタのnullチェック
			m_pTextureAll[nCnt]->Release();
			m_pTextureAll[nCnt] = nullptr;
		}

		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			m_paTextPass[nCnt],
			&m_pTextureAll[nCnt]);
	}
}

//テクスチャ全部の破棄処理
void CObject_2D::DestroyLoadedTextures(void)
{
	for (int nCnt = 0; nCnt < TEXTURE_TYPE_MAX; nCnt++)
	{
		if (m_pTextureAll[nCnt] != nullptr)
		{
			m_pTextureAll[nCnt]->Release();
			m_pTextureAll[nCnt] = nullptr;
		}
	}
}

//テクスチャの取得処理
LPDIRECT3DTEXTURE9 CObject_2D::GetTexturePointer(CObject::TextType type)
{
	return m_pTextureAll[type];
}

//生成処理
CObject_2D* CObject_2D::Create(void)
{
	CObject_2D* pObj2D = nullptr;

	pObj2D = new CObject_2D;

	if (FAILED(pObj2D->Init()))
	{
		if (pObj2D != nullptr)
		{
			delete pObj2D;
			pObj2D = nullptr;
		}

		return nullptr;
	}

	return pObj2D;
}


void CObject_2D::UpdateTexture(void)
{
	VERTEX_2D* pVtx = nullptr;					//頂点情報へのポインタ

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
	else if(m_bFlipX && !m_bFlipY)
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