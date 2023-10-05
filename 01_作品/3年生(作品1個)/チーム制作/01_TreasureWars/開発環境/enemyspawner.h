//=============================================================================
//
// enemyspawner.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _ENEMYSPAWNER_H_
#define _ENEMYSPAWNER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "objectX.h"

//---------------------------
// �N���X�錾
//---------------------------
class CEnemySpawner : public CObjectX
{
	static const int Enemy_Spawn_Timer = 180;		// �G�L�����̐����܂ł̎���
	static const float First_Pos;					// �X�|�i�[�̏����ʒu

public:

	// �G�L�����X�|�i�[�̎��
	enum EnemySpawn_Type
	{
		EnemySpawn_1 = 0,
		EnemySpawn_2,
		EnemySpawn_3,
		EnemySpawn_MAX,
	};

	CEnemySpawner();
	virtual ~CEnemySpawner() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Appearance();		// �o���܂ł̏���
	void SpawnCount();		// �����܂ł̎��Ԃ��v�鏈��
	void EnemySpawn();		// �G�L�������o�������鏈��
	void Damage();			// ��e����

	static CEnemySpawner* Create(const D3DXVECTOR3 pos, const int type);

	const bool GetAppearance() { return m_bAppearance; }
	const float GetRadius() { return m_fRadius; }

private:

	bool m_bAppearance;			// �o���������������ǂ���
	int m_nSpawn_Counter;		// �������Ԃ̃J�E���^�[
	int m_nMaxSpawn;			// ��x�ɐ�������ő吔
	int m_nLife;				// �̗�
	float m_fRadius;			// �����蔻��̑傫��

	EnemySpawn_Type m_type;	// �G�L�����X�|�i�[�̎��
	
};

#endif// _ENEMYSPAWNER_H_