//==============================================================================================
//
// 札吹雪　　　blizzardoffallingbills.cpp
// koduna hirohito
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"blizzardoffallingbills.h"
#include "texture.h"

//==============================================================================================
//静的固定変数
//==============================================================================================
const float CBlizzardOfFallingBills::WIDTH_PER_PATTERN_ONE = 1.0f / EFFECT_PATTERN_MAX;
const float CBlizzardOfFallingBills::BASIC_ROT_MOVE = D3DXToRadian(0.5f);
const float CBlizzardOfFallingBills::BASIC_POS_MOVE = 1.0f;
const float CBlizzardOfFallingBills::RANGE_WITH_PARENT = 200.0f;
const float CBlizzardOfFallingBills::RIGHT_LEFT_WIDTH = D3DXToRadian(80);

//==============================================================================================
// コンストラクタ
//==============================================================================================
CBlizzardOfFallingBills::CBlizzardOfFallingBills(int nPriority) : CObject2D(nPriority)
{
	m_fParentRot = 0.0f;								//仮想親のRot
	m_ParentPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//仮想親のPos
	m_fParentRotMove = 0.0f;							//仮想親のRotの移動量
	m_ParentPosMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//仮想親のPosの移動量
	m_bRightLeft = false;								//右左
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CBlizzardOfFallingBills::~CBlizzardOfFallingBills()
{
}

//==============================================================================================
// 生成処理
//==============================================================================================
CBlizzardOfFallingBills *CBlizzardOfFallingBills::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nPriority)
{
	CBlizzardOfFallingBills *pObject = nullptr;

	pObject = new CBlizzardOfFallingBills(nPriority);

	if (pObject != nullptr)
	{
		pObject->SetSize(size.x, size.y);

		//登録する子の位置
		D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//データの算出
		Pos.x = sinf(0.0f) * RANGE_WITH_PARENT;
		Pos.y = cosf(0.0f) * RANGE_WITH_PARENT;

		pObject->SetParentPos(pos);
		pObject->Setposition(Pos + pos);
		pObject->Init();
	}

	return pObject;
}

//==============================================================================================
// 札吹雪の初期化
//==============================================================================================
HRESULT CBlizzardOfFallingBills::Init()
{
	//親の初期化
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	//テクスチャの設定
	SetTexture(CTexture::TEXTURE_BILL_SNOWSTORM);

	//テクスチャのランダム生成
	int nPattern = rand() % EFFECT_PATTERN_MAX;

	//テクスチャの位置
	float fTexPos = WIDTH_PER_PATTERN_ONE;

	//テクスチャの位置設定
	SetTexPos(0.0f, 1.0f, fTexPos * (nPattern + 1), fTexPos * nPattern);

	//右左のランダム化
	if (rand() % 2 == 0)
	{
		m_bRightLeft = true;
	}
	else
	{
		m_bRightLeft = false;
	}

	return S_OK;
}

//==============================================================================================
// 札吹雪の更新
//==============================================================================================
void CBlizzardOfFallingBills::Update()
{
	//仮想親の位置更新
	m_ParentPos.y += BASIC_POS_MOVE;

	//仮想親の向き更新
	if (m_bRightLeft)
	{
		m_fParentRot += BASIC_ROT_MOVE;

		if (RIGHT_LEFT_WIDTH < m_fParentRot)
		{
			m_bRightLeft = false;
		}
	}
	else
	{
		m_fParentRot -= BASIC_ROT_MOVE;

		if (-RIGHT_LEFT_WIDTH> m_fParentRot)
		{
			m_bRightLeft = true;
		}
	}

	//登録する子の位置
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//データの算出
	pos.x = sinf(m_fParentRot) * RANGE_WITH_PARENT;
	pos.y = cosf(m_fParentRot) * RANGE_WITH_PARENT;

	//位置を変更
	Setposition(pos + m_ParentPos);

	//向きの変更
	Setrot(m_fParentRot);

	//親の更新
	CObject2D::Update();
}