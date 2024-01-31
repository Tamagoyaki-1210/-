//==============================================
//
// 弾.h (Bullet.h)
// Author : Saito Shian
// Author : Tanimoto Kosuke
//
//==============================================
#ifndef  _BULLET_H_				//このマクロ定義がされてなかったら
#define  _BULLET_H_				//二重インクルード防止のマクロ定義

//==============================================
// インクルード
//==============================================
#include "main.h"
#include "move_object.h"

//==============================================
// 前方宣言
//==============================================
class CObject3D;
class CObjectX;

//==============================================
// 弾クラス
//==============================================
class CBullet : public CMove_Object
{
	static const float BULLET_SPEED_XZ;			// 弾の速度(XZ座標)
	static const float BULLET_SPEED_Y;			// 弾の速度(Y座標)
	static const float BULLET_COLLISION_RADIUS;	// 弾の当たり判定の大きさ
public:

	//****************
	// publicな関数
	//****************
	CBullet(const PRIORITY priority);		// ナンバーのコンストラクタ
	~CBullet() override;					// ナンバーのデストラクタ

	HRESULT Init() override;				// 初期化処理
	void Uninit() override;					// 終了処理
	void Update() override;					// 更新処理
	void Draw() override;					// 描画処理

	void FieldCollision();					//床の当たり判定
	void Hit(CMove_Object* pHit) override;	// 被弾処理
	virtual void Destroy();							// 破壊処理
	void Map_Object_Collision();

	void SetLife(const int life) { m_nLife = life; }		// 弾の寿命の設定
	void SetSpeed_XZ(const float speed) { m_fSpeed_XZ = speed; }	// 弾の速度の設定
	void SetSpeed_Y(const float speed) { m_fSpeed_Y = speed; }	// 弾の速度の設定
	void SetSpeed(const float speed) { m_fSpeed_XZ = speed; m_fSpeed_Y = speed;}	// 弾の速度の設定
	void SetPower(const int power) { m_nPower = power; }
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }	// 移動量の設定
	void AddMove(const D3DXVECTOR3 move) { m_move += move; }// 移動量の加算
	void SetSize(const D3DXVECTOR2 size) { m_size = size; }	// サイズの設定
	void SetObj3D(CObject3D *obj3d) { m_pObj3D = obj3d; }
	void SetObjX(CObjectX *objx) { m_pObjX = objx; }
	void SetExplosion(bool explosion) { m_bExplosion = explosion; }

	const int GetLife() { return m_nLife; }					// 弾の寿命の取得
	const float GetSpeed_XZ() { return m_fSpeed_XZ; }				// 弾の速度の取得
	const float GetSpeed_Y() { return m_fSpeed_Y; }				// 弾の速度の取得
	const int GetPower() override { return m_nPower; }		// 威力の取得
	const D3DXVECTOR3 GetMove() { return m_move; }			// 移動量の取得
	const D3DXVECTOR2 GetSize() { return m_size; }			// サイズの取得
	CObject3D *GetObj3D() { return m_pObj3D; }
	CObjectX *GetObjX() { return m_pObjX; }

	static CBullet* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const D3DXVECTOR3 move, const bool side, const int power, const float speed, const int life);	// 生成

private:
	//****************
	// privateな変数
	//****************
	D3DXVECTOR3 m_move;						// 弾の移動量
	D3DXVECTOR3 m_nPosOld;					// 前回の位置
	D3DXVECTOR2 m_size;						// 弾のサイズ
	int m_nLife;							// 弾の寿命
	int m_nPower;							// 弾の威力
	float m_fSpeed_XZ;						// 弾のスピード
	float m_fSpeed_Y;						// 弾のスピード
	bool m_bExplosion;						// ヒット時に爆発するか

	CObject3D* m_pObj3D;					// オブジェクト3Dのポインタ
	CObjectX* m_pObjX;					// オブジェクトXのポインタ
};
#endif
