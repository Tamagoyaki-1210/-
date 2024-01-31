//==============================================
//
// 武器攻撃.h (weapon_attack.h)
// Author : Tanimoto Kosuke
//
//==============================================
#ifndef  _WEAPON_ATTACK_H_				//このマクロ定義がされてなかったら
#define  _WEAPON_ATTACK_H_				//二重インクルード防止のマクロ定義

//==============================================
// インクルード
//==============================================
#include "attack_collision.h"

//==============================================
// クラス
//==============================================
class CWeapon_Attack : public CAttack_Collision
{

public:

	CWeapon_Attack(const PRIORITY priority);		// ナンバーのコンストラクタ
	~CWeapon_Attack() override;					// ナンバーのデストラクタ

	HRESULT Init() override;				// 初期化処理
	void Uninit() override;					// 終了処理
	void Update() override;					// 更新処理
	void Draw() override;					// 描画処理

	// 武器攻撃(位置、大きさ、向き、サイド、威力, 寿命)
	static CWeapon_Attack* Create(const D3DXVECTOR3 pos, const float radius, const bool side, const int power, const int life);

private:

};
#endif //_WEAPON_ATTACK_H_