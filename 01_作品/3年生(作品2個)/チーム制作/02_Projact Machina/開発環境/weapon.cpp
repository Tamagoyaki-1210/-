//=============================================================================
//
// weapon.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "weapon.h"
#include "application.h"

const char* CWeapon::m_cWeaponFileName[] =
{
	"Data/model/Weapon/Gun_Weapon/AR40.x",				// AR40
	"Data/model/Weapon/Gun_Weapon/Tec-A3.x",			// Tec-A3
	"Data/model/Weapon/Gun_Weapon/DumpMan.x",			// DumpMan
	"Data/model/Weapon/Gun_Weapon/StarVerseAR.x",		// StarVerseAR
	"Data/model/Weapon/Gun_Weapon/MM400ジンクス.x",		// MM400ジンクス

	"Data/model/Weapon/Gun_Weapon/MPC50.x",				// MPC50
	"Data/model/Weapon/Gun_Weapon/PDW-920.x",			// PDW-920
	"Data/model/Weapon/Gun_Weapon/パールSMG.x",			// パールSMG
	"Data/model/Weapon/Gun_Weapon/SilverComet.x",		// SilverComet

	"Data/model/Weapon/Gun_Weapon/HG37.x",				// HG37
	"Data/model/Weapon/Gun_Weapon/Revolver.x",			// Revolver
	"Data/model/Weapon/Gun_Weapon/アキンボ2.0.x",		// アキンボ2.0

	"Data/model/Weapon/Gun_Weapon/LA2000.x",			// LA2000
	"Data/model/Weapon/Gun_Weapon/メタリカ255.x",		// メタリカ255

	"Data/model/Weapon/Gun_Weapon/12Pump.x",			// 12Pump
	"Data/model/Weapon/Gun_Weapon/WR2154.x",			// WR2154

	"Data/model/Weapon/Gun_Weapon/Winter5000.x",		// Winter5000
	"Data/model/Weapon/Gun_Weapon/アテナ.x",			// アテナ

	"Data/model/SG_01/DummyParent.x",

	"Data/model/Weapon/Melee_Weapon/knuckle.x",			// ナックル
	"Data/model/Weapon/Melee_Weapon/pilebunker.x",		// パイルバンカー
	"Data/model/Weapon/Melee_Weapon/brandmarkers.x",	// ブランドマーカー
	"Data/model/Weapon/Melee_Weapon/stunbaton.x",		// スタンバトン

	"Data/model/Weapon/Melee_Weapon/samuraiSword.x",	// 太刀
	"Data/model/Weapon/Melee_Weapon/hammer.x",			// ハンマー
	"Data/model/Weapon/Melee_Weapon/beamSword.x",		// ビームサーベル
	"Data/model/Weapon/Melee_Weapon/basterSword.x",		// バスターソード

	"Data/model/Weapon/Melee_Weapon/lance.x",			// ランス
	"Data/model/Weapon/Melee_Weapon/rapier.x",			// レイピア
	"Data/model/Weapon/Melee_Weapon/partisan.x",		// パルチザン
	"Data/model/Weapon/Melee_Weapon/beamjavelin.x",		// ビームジャベリン
};

//=====================================
// デフォルトコンストラクタ
//=====================================
CWeapon::CWeapon(const CObject::PRIORITY priority) : CObjectX(priority)
{

}

//=====================================
// デストラクタ
//=====================================
CWeapon::~CWeapon()
{
}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CWeapon::Init()
{
	CObjectX::Init();

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CWeapon::Uninit()
{
	CObjectX::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CWeapon::Update()
{
	CObjectX::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CWeapon::Draw()
{
	CObjectX::Draw();
}

//============================================================================
// 武器の変更
//============================================================================
void CWeapon::ChangeWeapon(const int type)
{
	SetModel(CApplication::GetModel()->ReadObject(m_cWeaponFileName[type]));
}

//============================================================================
// 生成処理
//============================================================================
CWeapon* CWeapon::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int type, CObjectX* parent)
{
	CWeapon* pParts = new CWeapon;

	if (FAILED(pParts->Init()))
	{
		return nullptr;
	}

	pParts->SetPos(pos);
	pParts->SetRot(rot);
	pParts->ChangeWeapon(type);
	pParts->SetParent(parent);

	return pParts;
}
