//=============================================================================
//
// scoreitem.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _SCOREITEM_H_
#define _SCOREITEM_H_

#include "objectX.h"
#include <vector>

class CShadow;

//---------------------------
// �N���X�錾
//---------------------------
class CScoreItem : public CObjectX
{
public:
	static const char* STAGE_SCOREITEM_01[];
	static const char* STAGE_SCOREITEM_02[];
	static const char* STAGE_SCOREITEM_03[];

	static const int MAX_SCOREITEM_MODEL = 5;	// �X�R�A�A�C�e���̃��f���ő吔
	static const float ScoreItem_Radius;		// �X�R�A�A�C�e���̓����蔻��̃T�C�Y

	CScoreItem();
	~CScoreItem() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CScoreItem* RandomCreate(const D3DXVECTOR3 pos);
	static CScoreItem* Create(const D3DXVECTOR3 pos, const int type);

	void Destroy();
	void Move();
	void CharColl();
	void WallColl();
	void Blink();
	void Bounce();
	void Floating();

	void SetShadow(const D3DXVECTOR3 pos);		// �e�̐�������

	const int GetNum() { return m_nNumMaxScoreItem; }
	const bool GetGround() { return m_bGround; }
	const int GetType() { return m_nType; }

private:
	static const int Floating_Timer = 60;			// ���V����
	static const int Blink_Timer = 180;				// �_�ł܂ł̎���
	static const int Max_Destroy_Timer = 360;		// ������܂ł̎���
	static const int ScoreItem_Blink_interval = 10;	// ��΂̓_�ŊԊu
	static const int m_nNumMaxScoreItem = 5;		// �X�R�A�A�C�e���̍ő��

	static const float ScoreItem_Gravity;			// �X�R�A�A�C�e���̏d��
	static const float ScoreItem_Inertia;			// �X�R�A�A�C�e���̊���
	static const float ScoreItem_Launch;			// �X�R�A�A�C�e�����c�ɑł��オ���
	static const float ScoreItem_Blink_Speed;		// �X�R�A�A�C�e���̓_�ő��x

	int m_nType;									// �X�R�A�A�C�e���̎��
	int m_nFloatingCounter;							// ���V����
	int m_nDestroyCounter;							// ������܂ł̎���
	bool m_bGround;									// ���n����

	D3DXVECTOR3 m_move;								// �ړ���
	CShadow* m_pShadow;								// �ȒP�ȉe�̃|�C���^
};

#endif// _SCOREITEM_H_