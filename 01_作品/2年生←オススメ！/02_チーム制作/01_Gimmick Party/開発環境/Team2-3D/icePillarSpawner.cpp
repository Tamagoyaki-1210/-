//=============================================================================
//
// icePillarSpawner.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "icePillarSpawner.h"
#include "icePillar.h"


//コンストラクタ
CIcePillarSpawner::CIcePillarSpawner()
{
	m_pos = Vec3Null;				//位置
	m_shadowY = 0.0f;				//影の高さ
	m_laterlBounds = 0.0f;  		//スポーンエリアの幅
	m_nCntTime = 0;					//フレームカウンター
	m_nSpawnTime = 0;				//スポーンタイム
	m_nMaxSpawn = 0;				//最大タイム
	m_fAreaLength = 0.0f;			//スポーンエリアの長さ
}

//デストラクタ
CIcePillarSpawner::~CIcePillarSpawner()
{

}

//初期化処理
HRESULT CIcePillarSpawner::Init(void)
{
	m_pos = Vec3Null;				//位置
	m_shadowY = 0.0f;				//影の高さ
	m_laterlBounds = 0.0f;  		//スポーンエリアの幅
	m_nCntTime = 0;					//フレームカウンター
	m_nSpawnTime = 0;				//スポーンタイム
	m_nMaxSpawn = 0;				//最大タイム
	m_fAreaLength = 0.0f;			//スポーンエリアの長さ

	return S_OK;
}

//終了処理
void CIcePillarSpawner::Uninit(void)
{

}

//更新処理
void CIcePillarSpawner::Update(void)
{
	m_nCntTime--;			//スポーンカウンターを更新する

	if (m_nCntTime <= 0)
	{//スポーン処理
		if (m_nMaxSpawn > 60)
		{//スポーン時間を設定する(1秒以下だったら、そのまま設定する。1秒以上だったら、ランダムで決めます)
			m_nCntTime = CObject::random(60, m_nMaxSpawn);
		}
		else
		{
			m_nCntTime = m_nMaxSpawn;
		}

		//ランダムで向きを決めます
		float fX, fZ, fDir;

		if (random(1, 101) % 2 == 0)
		{
			fDir = 1.0f;
		}
		else
		{
			fDir = -1.0f;
		}

		//スポーン位置の設定
		fX = m_pos.x + (fDir * ((float)random(5, (int)m_laterlBounds)));
		fZ = m_pos.z + (((float)random(-(int)m_fAreaLength, (int)m_fAreaLength)));

		//オブジェクトを生成する
		CIcePillar::Create(CModel::MODEL_ICE_PILLAR, D3DXVECTOR3(fX, m_pos.y, fZ), m_shadowY);
	}
}

//描画処理
void CIcePillarSpawner::Draw(void)
{

}

//位置の設定処理
void CIcePillarSpawner::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//位置の取得処理
const D3DXVECTOR3 CIcePillarSpawner::GetPos(void)
{
	return m_pos;
}

//サイズの取得処理
const D3DXVECTOR2 CIcePillarSpawner::GetSize(void)
{
	return Vec2Null;
}



//=============================================================================
//
//								静的関数
//
//=============================================================================



//生成処理
CIcePillarSpawner* CIcePillarSpawner::Create(const D3DXVECTOR3 pos, const float fShadowHeight, const float fLaterlBounds, const float fLength, const int nMaxTime)
{
	CIcePillarSpawner* pSpawner = new CIcePillarSpawner;			//オブジェクトを生成する

	if (FAILED(pSpawner->Init()))
	{//初期化処理
		return nullptr;
	}

	int nTime = nMaxTime;				//スポーン時間の設定

	if (nMaxTime <= 0)
	{//スポーン時間が1以下ではないように確認する
		nTime = 1;
	}

	pSpawner->m_pos = pos;								//位置の設定
	pSpawner->m_shadowY = fShadowHeight;				//影の高さの設定
	pSpawner->m_laterlBounds = fLaterlBounds;			//スポーンエリアの幅の設定
	pSpawner->m_nMaxSpawn = nTime;						//最大スポーン時間の設定
	pSpawner->m_nCntTime = 400;							//ディレイの設定
	pSpawner->m_fAreaLength = fLength;					//スポーンエリアの長さの設定

	return pSpawner;
}