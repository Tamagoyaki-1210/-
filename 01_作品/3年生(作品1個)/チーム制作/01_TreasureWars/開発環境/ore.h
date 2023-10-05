//==============================================================================
//
// ore.h	(�z��.h)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// 2�d�C���N���[�h�K�[�h
//==============================================================================
#ifndef _ORE_H_
#define	_ORE_H_

//==============================================================================
// �C���N���[�h
//==============================================================================
#include "main.h"
#include "objectX.h"

//==============================================================================
// �}�N����`
//==============================================================================

//==============================================================================
// �O���錾
//==============================================================================

//==============================================================================
// �}�b�v�N���X
//==============================================================================
class COre : public CObjectX
{
	static const float MOVING_DISTANCE;
	static const int UNINIT_TIME;
	static const char* STAGE_MODEL_01[];
	static const char* STAGE_MODEL_02[];
	static const char* STAGE_MODEL_03[];

public:
	COre();						// �R���X�g���N�^
	~COre();					// �f�X�g���N�^

	HRESULT Init() override;	// ����������
	HRESULT Init(int nStageNum);	// ����������
	void Uninit() override;		// �I������
	void Update() override;		// �X�V����

	int Damage(int damage);

	static COre *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size,int nStageNum);

	const bool GetOreDeath() { return m_bOreDeath; }
	const bool GetAppearance() { return m_bAppearance; }

private:
	static const int Normal_Ore_Life = 20;	// �ʏ�z�΂̑ϋv��
	static const int Ore_Blink_Speed;		// �z�΂̓_�ł��鑬�x
	static const float Ore_Blink_Timer;		// �z�΂��_�ł��n�߂鎞��

	D3DXVECTOR3 m_move;			// �ړ���
	int m_nLife;				// �̗�
	int m_UninitTime;			// �����܂ł̎���
	int m_nOreModelIndex;		// �z�΃��f���ԍ�
	int m_nBigOreModelIndex;	// �z��(��)���f���ԍ�
	bool m_bOnce;
	bool m_bAppearance;			// �o���������������ǂ���
	bool m_bOreDeath;			// ���g���I��������
};

#endif

