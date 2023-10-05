//=============================================================================
//
// coin.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "coin.h"
#include "BoxHitbox.h"
#include "meshfield.h"
#include "application.h"
#include "effect.h"
#include "camera.h"

//=============================================================================
//
//							静的変数の初期化
//
//=============================================================================

//コインの当たり判定のサイズ
const D3DXVECTOR3 CCoin::m_hitboxSize[COIN_MAX] =
{
	{15.0f, 20.0f, 15.0f},
	{15.0f, 20.0f, 15.0f},
	{15.0f, 20.0f, 15.0f},
	{15.0f, 20.0f, 15.0f},
	{15.0f, 20.0f, 15.0f}
};

//コンストラクタ
CCoin::CCoin()
{
	//メンバー変数をクリアする
	m_move = Vec3Null;			//移動量
	m_nLife = 0;				//ライフ
	m_pHitbox = nullptr;		//ヒットボックス
}

//デストラクタ
CCoin::~CCoin()
{

}

//初期化処理
HRESULT CCoin::Init(void)
{
	//初期化処理
	if (FAILED(CModel::Init()))
	{
		return -1;
	}

	m_move = Vec3Null;				//移動量の設定
	m_nLife = -1;					//ライフの設定
									
	m_pHitbox = nullptr;			//ヒットボックス

	return S_OK;
}

//終了処理
void CCoin::Uninit(void)
{
	//ヒットボックスの破棄処理
	if (m_pHitbox != nullptr)
	{
		m_pHitbox->Release();
		m_pHitbox = nullptr;
	}

	//基本クラスの終了処理
	CModel::Uninit();
}

//更新処理
void CCoin::Update(void)
{
	//基本クラスの更新処理
	CModel::Update();

	if (m_pHitbox != nullptr)
	{//当たり判定がnullではなかったら
		if (m_pHitbox->GetCollisionState())
		{//プレイヤーと当たった場合
			m_pHitbox->Release();		//ヒットボックスを消す
			Release();					//コインを消す
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_COIN_GET);			//サウンドを再生する

			//エフェクトを生成する
			for (int nCnt = 0; nCnt < 50; nCnt++)
			{
				CEffect::Create(GetPos(), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * ((float)CObject::random(-100, 100) * 0.001f)), D3DXVECTOR3((float)CObject::random(-50, 50) * 0.05f, (float)CObject::random(-50, 50) * 0.05f, (float)CObject::random(-50, 50) * 0.05f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.25f), D3DXCOLOR(-0.005f, -0.005f, 0.001f, 0.0f), 1.0f, -0.03f, 30);
			
			}

			return;		
		}
	}

	if (m_nLife > 0)
	{//ライフが0より大きいだったら
		m_nLife--;			//ライフをデクリメントする

		if (m_nLife <= 0)
		{//ライフが0になったら
			m_pHitbox->Release();		//ヒットボックスを消す
			Release();					//コインを消す
			return;
		}
	}

	if (D3DXVec3Length(&m_move) > 0.0f)
	{//移動量が0ではなかったら

		//重力を足す
		if (m_move.y >= -10.0f)
		{
			m_move.y += -0.7f;
		}

		D3DXVECTOR3 newPos = GetPos() + m_move;			//新しい位置の計算
		SetPos(newPos);									//位置の設定
		m_pHitbox->SetPos(newPos);						//ヒットボックスの位置の設定

		m_move.x += (0.0f - m_move.x) * 0.05f;			//減速
		m_move.z += (0.0f - m_move.z) * 0.05f;			//減速

		CMeshfield::FieldInteraction(this);				//メッシュフィールドとの当たり判定
	}

	if (GetPos().z < CApplication::GetCamera()->GetPos().z - 100.0f)
	{//見えなくなったら、消す

		Release();
	}
}

//描画処理
void CCoin::Draw(void)
{
	if (GetPos().z < CApplication::GetCamera()->GetPos().z + 650.0f)
	{//遠すぎたら、描画しない

		if ((m_nLife % 30) <= 15)
		{//落としたコインだったら、点滅させる
			CModel::Draw();
		}
	}
}

//移動量の設定処理
void CCoin::SetMove(const D3DXVECTOR3 move)
{
	m_move = move;
}

//移動量の取得処理
const D3DXVECTOR3 CCoin::GetMove(void)
{
	return m_move;
}


//=============================================================================
//
//								静的関数
//
//=============================================================================



//生成処理
CCoin* CCoin::Create(const D3DXVECTOR3 pos, const COIN_TYPE type)
{
	CCoin* pCoin = new CCoin;			//コインを生成する

	if (FAILED(pCoin->Init()))
	{//初期化処理
		return nullptr;
	}

	int nType = (int)CModel::MODEL_COIN_0 + (int)type;					//モデル番号を計算する

	pCoin->SetModel((CModel::ModelType)nType);							//モデルの設定処理
	pCoin->SetPos(pos);													//位置の設定処理
	pCoin->StartRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.01f, 0.0f));		//回転速度の設定

	int nAddScore = 0;

	//種類によってスコアを設定する
	switch (type)
	{
	case COIN_0:
	{
		nAddScore = 10;
	}
	break;
	case COIN_1:
	{
		nAddScore = 20;
	}
	break;
	case COIN_2:
	{
		nAddScore = 30;
	}
	break;
	case COIN_3:
	{
		nAddScore = 50;
	}
	break;
	default:
		break;
	}

	pCoin->m_pHitbox = CBoxHitbox::Create(pos, Vec3Null, m_hitboxSize[type], CHitbox::TYPE_VANISHING, pCoin, nAddScore);		//ヒットボックスを生成する

	return pCoin;				//生成したインスタンスを返す

}

//生成処理
CCoin* CCoin::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const int nLife, const COIN_TYPE type)
{
	CCoin* pCoin = new CCoin;			//コインを生成する

	if (FAILED(pCoin->Init()))
	{//初期化処理
		return nullptr;
	}

	int nType = (int)CModel::MODEL_COIN_0 + (int)type;					//モデル番号を計算する

	pCoin->SetModel((CModel::ModelType)nType);							//モデルの設定処理
	pCoin->SetPos(pos);													//位置の設定処理
	pCoin->m_move = move;												//移動量の設定処理
	pCoin->StartRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.01f, 0.0f));		//回転速度の設定
	pCoin->m_nLife = nLife;												//ライフの設定

	pCoin->m_pHitbox = CBoxHitbox::Create(pos, Vec3Null, m_hitboxSize[type], CHitbox::TYPE_VANISHING, pCoin, 10);		//ヒットボックスを生成する

	return pCoin;				//生成したインスタンスを返す
}