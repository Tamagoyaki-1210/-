//==============================================
//
// 攻撃当たり判定.h (attack_collision.h)
// Author : Tanimoto Kosuke
//
//==============================================
#ifndef  _ATTACK_COLLISION_H_				//このマクロ定義がされてなかったら
#define  _ATTACK_COLLISION_H_				//二重インクルード防止のマクロ定義

//==============================================
// インクルード
//==============================================
#include "main.h"
#include "move_object.h"

//==============================================
// 前方宣言
//==============================================
class CObject3D;

//==============================================
// クラス
//==============================================
class CAttack_Collision : public CMove_Object
{

public:

	CAttack_Collision(const PRIORITY priority);		// ナンバーのコンストラクタ
	virtual ~CAttack_Collision() override;					// ナンバーのデストラクタ

	virtual HRESULT Init() override;				// 初期化処理
	virtual void Uninit() override;					// 終了処理
	virtual void Update() override;					// 更新処理
	virtual void Draw() override;					// 描画処理

	void Hit(CMove_Object* pHit) override;	// 被弾処理
	virtual void Destroy();							// 破壊処理

	void SetLife(const int life) { m_nLife = life; }		// 弾の寿命の設定
	void SetPower(const int power) { m_nPower = power; }
	void SetObj3D(CObject3D *obj3d) { m_pObj3D = obj3d; }

	const int GetLife() { return m_nLife; }					// 弾の寿命の取得
	const int GetPower() override { return m_nPower; }		// 威力の取得
	CObject3D* GetObj3D() { return m_pObj3D; }

private:

	D3DXVECTOR3 m_nPosOld;					// 前回の位置
	int m_nLife;							// 弾の寿命
	int m_nPower;							// 弾の威力

	CObject3D* m_pObj3D;					// オブジェクト3Dのポインタ
};
#endif //_ATTACK_COLLISION_H_
