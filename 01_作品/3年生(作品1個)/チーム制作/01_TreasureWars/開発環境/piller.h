//==============================================================================
//
// piller.h	(���̃I�u�W�F�N�g.h)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// 2�d�C���N���[�h�K�[�h
//==============================================================================
#ifndef _PILLER_H_
#define	_PILLER_H_

//==============================================================================
// �C���N���[�h
//==============================================================================
#include "objectX.h"

//==============================================================================
// �O���錾
//==============================================================================
class CRanking;
class CCharacter;

//==============================================================================
// ���N���X
//==============================================================================
class CPiller : public CObjectX
{
	static const float MOVING_PILLERSPEED;

public:
	CPiller();						// �R���X�g���N�^
	~CPiller();						// �f�X�g���N�^

	HRESULT Init()override;			// ����������
	void Uninit()override;
	void Update()override;			// �X�V����

	static void SetScore(int score, int index) { m_anScore[index] = score; }

	static CPiller *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, int index, float moving, int num);

private:
	CRanking *m_pRanking;				// ���U���g�\���̂̃C���X�^���X
	CCharacter *m_pResult_Char;

	static int m_anScore[4];		// �X�R�A
	static int m_nScore;			// �X�R�A
	D3DXVECTOR3 m_move;				// �ړ���
	D3DXVECTOR3 m_rot;				// ��]
	D3DXVECTOR3 m_pos;				// �ʒu

	float m_fMaxMoving;				// �c�̍ő�l
	int m_no1;
	int m_num;
};

#endif

