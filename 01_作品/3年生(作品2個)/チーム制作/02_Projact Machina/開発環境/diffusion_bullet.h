//==============================================================================================
//
// �g�U�e�@�@�@diffusion_bullet.h
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

	HRESULT Init() override;	// ����������
	void Uninit() override;		// �I������
	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��

	static CDiffusion_Bullet *Create(const D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 move, int number, const bool side, const int power, const float speed, const int life);

private:

};

#endif // !_DIFFUSION_BULLET_H_
