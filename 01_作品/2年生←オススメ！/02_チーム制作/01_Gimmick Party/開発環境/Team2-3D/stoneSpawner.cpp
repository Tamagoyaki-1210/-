//=============================================================================
//
// stoneSpawner.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "stoneSpawner.h"
#include "fallingStone.h"

//コンストラクタ
CStoneSpawner::CStoneSpawner()
{
	//メンバー変数をクリアする
	m_pos = Vec3Null;				//位置
	m_shadowY = 0.0f;				//影の高さ
	m_laterlBounds = 0.0f;  		//横の限界
	m_nCntTime = 0;					//フレームカウンター
	m_nSpawnTime = 0;				//スポーンタイム
	m_nMaxSpawn = 0;				//最大タイム
	m_fAreaLength = 0.0f;			//スポーンエリアの長さ
}

//デストラクタ
CStoneSpawner::~CStoneSpawner()
{

}

//初期化処理
HRESULT CStoneSpawner::Init(void)
{
	m_pos = Vec3Null;				//位置
	m_shadowY = 0.0f;				//影の高さ
	m_laterlBounds = 0.0f;  		//横の限界
	m_nCntTime = 0;					//フレームカウンター
	m_nSpawnTime = 0;				//スポーンタイム
	m_nMaxSpawn = 0;				//最大タイム
	m_fAreaLength = 0.0f;			//スポーンエリアの長さ

	return S_OK;
}

//終了処理
void CStoneSpawner::Uninit(void)
{

}

//更新処理
void CStoneSpawner::Update(void)
{
	m_nCntTime--;					//スポーンカウンターを更新する

	if (m_nCntTime <= 0)
	{//0になったら、

		//次のオブジェクトのスポーンディレイを設定する(最大スポーン時間が1秒以下だったら、そのまま使います。そうではなかったら、1秒から設定された時間までランダムで決めます)
		if (m_nMaxSpawn > 60)
		{
			m_nCntTime = CObject::random(60, m_nMaxSpawn);
		}
		else
		{
			m_nCntTime = m_nMaxSpawn;
		}

		//スポーンエリア内ランダムでスポーンの位置を決めます
		float fX, fZ, fDir;

		if (random(1, 101) % 2 == 0)
		{
			fDir = 1.0f;
		}
		else
		{
			fDir = -1.0f;
		}

		fX = m_pos.x + (fDir * ((float)random(5, (int)m_laterlBounds)));
		fZ = m_pos.z + (((float)random(-(int)m_fAreaLength, (int)m_fAreaLength)));

		//オブジェクトを生成する
		CFallingStone::Create(CModel::MODEL_FALLING_STONE, D3DXVECTOR3(fX, m_pos.y, fZ), m_pos, m_shadowY, m_laterlBounds, fDir);
	}
}

//描画処理
void CStoneSpawner::Draw(void)
{

}

//位置の設定処理
void CStoneSpawner::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//位置の取得処理
const D3DXVECTOR3 CStoneSpawner::GetPos(void)
{
	return m_pos;
}

//サイズの取得処理
const D3DXVECTOR2 CStoneSpawner::GetSize(void)
{
	return Vec2Null;
}




//=============================================================================
//
//								静的関数
//
//=============================================================================




//生成処理
CStoneSpawner* CStoneSpawner::Create(const D3DXVECTOR3 pos, const float fShadowHeight, const float fLaterlBounds, const float fLength, const int nMaxTime)
{
	CStoneSpawner* pSpawner = new CStoneSpawner;			//インスタンスを生成する

	if (FAILED(pSpawner->Init()))
	{//初期化処理
		return nullptr;
	}

	//最大スポーン時間の設定
	int nTime = nMaxTime;

	if (nMaxTime <= 0)
	{//0フレーム以下だったら、1フレームに設定する
		nTime = 1;
	}

	pSpawner->m_pos = pos;							//位置の設定
	pSpawner->m_shadowY = fShadowHeight;			//影の高さの設定
	pSpawner->m_laterlBounds = fLaterlBounds;		//スポーンエリアの幅の設定
	pSpawner->m_nMaxSpawn = nTime;					//最大スポーン時間の設定
	pSpawner->m_nCntTime = 400;						//スポーンカウンターの初期化
	pSpawner->m_fAreaLength = fLength;				//スポーンエリアの長さ

	//エリアの前のモデルの生成
	CModel* pModel = CModel::Create(CModel::MODEL_CAUTION_SIGNAL, D3DXVECTOR3(pos.x - fLaterlBounds, fShadowHeight, pos.z - fLength));
	pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));				//向きの設定

	pModel = CModel::Create(CModel::MODEL_CAUTION_SIGNAL, D3DXVECTOR3(pos.x + fLaterlBounds, fShadowHeight, pos.z - fLength));
	pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));				//向きの設定
	pModel->SetShadowDraw(false);									//影を描画しないように設定する

	return pSpawner;								//生成したインスタンスを返す
}