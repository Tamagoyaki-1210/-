//=============================================================================
//
// pc.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _PC_H_
#define _PC_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "player.h"

class CEnergy_Gauge;

//---------------------------
// �N���X�錾
//---------------------------
class CPC : public CPlayer
{
public:

	CPC();
	virtual ~CPC() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Input();		// ���͏���
	void Destroy() override;			// �j�󏈗�
	void Perspective();	// ���_�ړ�

	static CPC* Create(const D3DXVECTOR3 pos, const int index);

private:
	D3DXVECTOR3 m_rotDest;					//�ړI�̌���
	int m_nWalk_Counter;					// ��������
};

#endif// _PC_H_