//=============================================================================
//
// �����G�t�F�N�g
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _RAY_H_
#define _RAY_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//�O���錾
//---------------------------
class CTexture;

//---------------------------
//�N���X
//---------------------------
class CRay : public CObject2D
{
public:
	enum Ray_Type
	{
		NONE = 0,	//���g�p
		BULLET,		//�e
		MAX,		//�����̍ő�l
	};

	CRay();
	~CRay() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void InitType();
	void UpdateType();
	void Destroy();
	void Rand();
	void Decline();
	static CRay *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, Ray_Type type);
	void LoadRay();
private:
	D3DXVECTOR3 m_move;		//�ړ���
	int m_nLife;			//�̗�
	float m_fWidth;			//������
	float m_fHeight;		//�����c
	float m_fSpeed;			//�ړ����x
	float m_fAlphaCounter;	//�A���t�@�l�J�E���^�[
	Ray_Type m_type;
	bool m_bBlink;			//�_�Ŕ���
};

#endif // !_RAY_H_