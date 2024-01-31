//==============================================================================================
//
// 曲射弾　　　parabola_bullet.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _PARABOLA_BULLET_H_
#define	_PARABOLA_BULLET_H_

#include"bullet.h"

class CParabola_Bullet : public CBullet
{
public:
	CParabola_Bullet(const PRIORITY priority);
	~CParabola_Bullet();

	HRESULT Init() override;	// 初期化処理
	void Uninit() override;		// 終了処理
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	void Destroy() override;	// 破壊処理

	static CParabola_Bullet *Create(const D3DXVECTOR3 pos, D3DXVECTOR3 move, float hypotenuse, D3DXVECTOR3 rot, char *filename, const bool side, const int power, const float speed, const int life);

private:
	static const int HIT_COUNT = 55;

	D3DXVECTOR3 m_Rot;
	float m_fHyptenuse;
	float m_fGravity;
	float m_fAngle;
};

#endif // !_PARABOLA_BULLET_H_
