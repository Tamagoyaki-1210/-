//==============================================================================================
//
// 拡散弾　　　diffusion_bullet.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _DIFFUSION_BULLET_H_
#define	_DIFFUSION_BULLET_H_

#include"bullet.h"

class CDiffusion_Bullet : public CBullet
{
public:
	CDiffusion_Bullet(const PRIORITY priority);
	~CDiffusion_Bullet();

	HRESULT Init() override;	// 初期化処理
	void Uninit() override;		// 終了処理
	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	static CDiffusion_Bullet *Create(const D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 move, int number, const bool side, const int power, const float speed, const int life);

private:

};

#endif // !_DIFFUSION_BULLET_H_
