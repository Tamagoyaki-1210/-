//=============================================================================
//
// shadow.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object3D.h"

//---------------------------
// �N���X�錾
//---------------------------
class CShadow : public CObject3D
{
public:
	CShadow();
	~CShadow() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetShadow(const D3DXVECTOR3 pos);

	static CShadow* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
private:
	D3DXVECTOR3 m_move;			//�ړ���
};

#endif // !_SHADOW_H_