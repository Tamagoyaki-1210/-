//==============================================================================================
//
// CPU�@�@�@cpu.h
// tutida ryousei
//
//==============================================================================================
#ifndef _CPU_H_
#define _CPU_H_

#include"character.h"

class CCpu : public CCharacter
{
public:
	CCpu();
	~CCpu() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CCpu *Create(D3DXVECTOR3 pos, int index);

	void Move() override;											// �ړ�����
	void Action();													// �A�N�V����
	void Ai();														// AI����
	void Search(const int search);									// �T������
	void CharaSearch(const D3DXVECTOR3 pos, const float radius);	// �L�����̒T������
	void OreSearch(const D3DXVECTOR3 pos, const float radius);		// �z�΂̒T������
	void ScoreItemSearch(const D3DXVECTOR3 pos);					// �X�R�A�A�C�e���̒T������
	void ItemSearch(const D3DXVECTOR3 pos);							// �A�C�e���̒T������
	void EnemySearch(const D3DXVECTOR3 pos, const float radius);	// �G�L�����̒T������
	void ShopSearch(const D3DXVECTOR3 pos, const float radius);		// �����̒T������

private:
	static const int First_Mine_Power = 10;			// �̌@�U���͂̏����l

	static const float Radius_Search_Ore;			// �z�΂̌����̃T�C�Y(���a)
	static const float Radius_Search_ScoreItem;		// �X�R�A�A�C�e���̌����̃T�C�Y(���a)
	static const float Radius_Search_Chara;			// �L�����̌����̃T�C�Y(���a)
	static const float Radius_Search_Enemy;			// �G�L�����̌����̃T�C�Y(���a)

	// �v�l���[�`���̎��
	enum Trial_Routine
	{
		TRIAL_ROUTINE_NEUTRAL = 0,	// �F�X�l����v�l
		TRIAL_ROUTINE_ATTACK,		// ������U������v�l
		TRIAL_ROUTINE_SALE,			// ��΂𔄋p����v�l
		TRIAL_ROUTINE_MAX,
		TRIAL_ROUTINE_NONE
	};

	// �����̎��
	enum Search
	{
		SEARCH_NONE = 0x00,			// �������Ȃ�
		SEARCH_CHARA = 0x01,		// �L�����̒T��
		SEARCH_ORE = 0x02,			// �z�΂̒T��
		SEARCH_SCOREITEM = 0x04,	// �X�R�A�A�C�e���̒T��
		SEARCH_ITEM = 0x08,			// �A�C�e���̒T��
		SEARCH_ENEMY = 0x10,		// �G�L�����̒T��
		SEARCH_SHOP = 0x20,			// �����̒T��
		SEARCH_MAX = 0xff,
	};

	int d;						// �^�C���X���b�v�̃f�o�b�O�ϐ�(���O��ς��ė~����)
	bool m_bSetAttack;			// �U�������邩�ǂ����̔���
	D3DXVECTOR3 m_targetPos;	// �ړI�ʒu

	Trial_Routine m_trialRoutine;	// ���݂̎v�l���[�`��
};

#endif // !_CPU_H_
