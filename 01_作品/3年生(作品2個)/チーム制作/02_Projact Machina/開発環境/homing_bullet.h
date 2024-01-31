//==============================================================================================
//
// ホーミング弾　　　homing_bullet.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _HOMING_BULLET_H_
#define	_HOMING_BULLET_H_

#include"bullet.h"

class CEnemy;

class CHoming_Bullet : public CBullet
{
public:
	CHoming_Bullet(const PRIORITY priority);
	~CHoming_Bullet();

	HRESULT Init() override;	// 初期化処理
	void Uninit() override;		// 終了処理
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	static CHoming_Bullet *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 nearmob_pos, char *filename, const bool side, const int power, const int life);

private:
	D3DXVECTOR3 m_NearMob_Pos;
	bool m_bTarget;
};

#endif // !_HOMING_BULLET_H_
