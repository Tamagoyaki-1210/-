//=============================================================================
//
// 武器攻撃.cpp (weapon_attack.cpp)
// Author : Tanimoto Kosuke
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "application.h"
#include "renderer.h"
#include "weapon_attack.h"
#include "game.h"
#include "object3D.h"
#include "tutorial.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CWeapon_Attack::CWeapon_Attack(const CObject::PRIORITY priority) : CAttack_Collision(priority)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CWeapon_Attack::~CWeapon_Attack()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CWeapon_Attack::Init()
{
	CAttack_Collision::Init();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CWeapon_Attack::Uninit()
{
	CAttack_Collision::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CWeapon_Attack::Update()
{
	CAttack_Collision::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CWeapon_Attack::Draw()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CWeapon_Attack* CWeapon_Attack::Create(const D3DXVECTOR3 pos, const float radius, const bool side, const int power, const int life)
{
	//クラスの生成
	CWeapon_Attack* pWeapon_Attack = new CWeapon_Attack(PRIORITY_BACK);

	//nullチェック
	if (pWeapon_Attack != nullptr)
	{
		//初期化処理
		pWeapon_Attack->Init();
		//設定処理
		pWeapon_Attack->SetPos(pos);
		pWeapon_Attack->SetRadius(radius);
		pWeapon_Attack->SetPlayerSide(side);
		pWeapon_Attack->SetPower(power);
		pWeapon_Attack->SetLife(life);

#ifdef _DEBUG
		// デバッグオブジェクトの生成
		CObject3D* pObj3D = CObject3D::Create(pos, { radius, radius }, PRIORITY_BACK);

		// テクスチャ
		pObj3D->SetTexture(CTexture::TEXTURE_CIRCLE);

		// ビルボードの設定
		pObj3D->Setbillboard(true);

		// デバッグオブジェクトの設定
		pWeapon_Attack->SetObj3D(pObj3D);
#endif
	}
	else
	{
		assert(false);
	}

	return pWeapon_Attack;
}