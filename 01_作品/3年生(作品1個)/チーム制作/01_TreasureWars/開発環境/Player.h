//==============================================================================================
//
// �v���C���[�@�@�@player.h
// tutida ryousei
//
//==============================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include"character.h"

class CModel;
class CGauge;

class CPlayer : public CCharacter
{
public:
	CPlayer();
	~CPlayer() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CPlayer *Create(D3DXVECTOR3 pos, int index);

	void Move() override;		// �ړ�����
	void Action();		// �A�N�V����

private:
	static const int First_Mine_Power = 10;	// �̌@�U���͂̏����l

	//D3DXVECTOR3 m_Pos;			// �ʒu
	D3DXVECTOR3 m_RotDest;		// �ړI�̊p�x
	D3DXVECTOR3 m_SizeMin;		// ���_�̍ŏ��l
	D3DXVECTOR3 m_SizeMax;		// ���_�̍ő�l
	D3DXVECTOR3 m_CPosV;		// �J�����̎��_�̊i�[��
	bool m_bSell;				// ���p��Ԃ̔���
	bool m_bEntry;				// �G���g���[������
	int m_nDamage;				// �U����
};

#endif // !_PLAYER_H_
