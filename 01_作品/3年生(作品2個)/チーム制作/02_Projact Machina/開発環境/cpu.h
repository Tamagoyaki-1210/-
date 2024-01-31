//=============================================================================
//
// cpu.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _CPU_H_
#define _CPU_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "player.h"

//---------------------------
// �N���X�錾
//---------------------------
class CCPU : public CPlayer
{
public:

	CCPU();
	~CCPU() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CCPU* Create(const D3DXVECTOR3 pos, const int index);

private:

};

#endif// _CCPU_H_