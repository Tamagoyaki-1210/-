//==============================================================================================
//
// 通常弾　　　normal_bullet.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _NORMAL_BULLET_H_
#define	_NORMAL_BULLET_H_

#include"bullet.h"

class CCharacter;

class CNormal_Bullet : public CBullet
{
public:
	CNormal_Bullet(const PRIORITY priority);
	~CNormal_Bullet();

	HRESULT Init() override;	// 初期化処理
	void Uninit() override;		// 終了処理
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	static CNormal_Bullet *Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, D3DXVECTOR3 move, float hypotenuse, CCharacter *chara, float enemy_speed, bool target, const bool side, const int power, const float speed, const int life);

private:
	float m_fHypotenuse;
	float m_fSpeed;
	bool m_bTarget;
	CCharacter *pChara;
};

#endif // !_NORMAL_BULLET_H_
