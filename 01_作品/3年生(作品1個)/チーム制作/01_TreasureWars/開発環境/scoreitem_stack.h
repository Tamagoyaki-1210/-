//=============================================================================
//
// scoreitem_stack.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _SCOREITEM_STACK_H_
#define _SCOREITEM_STACK_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "ui.h"
#include "scoreitem.h"
#include <vector>

//---------------------------
// �N���X�錾
//---------------------------
//#ifdef _DEBUG
class CObject2D;
//#endif

class CReceipt;

//---------------------------
// �N���X�錾
//---------------------------
class CScoreItemStack : public CUi
{
	// ��΂̐F�̎��
	static const D3DXCOLOR Col_Yellow;
	static const D3DXCOLOR Col_Green;
	static const D3DXCOLOR Col_Bule;
	static const D3DXCOLOR Col_Red;
	static const D3DXCOLOR Col_Gray;
	static const D3DXCOLOR Col_White;

	// ��΂̎��
	enum Gem_Type
	{
		GEM_DIAMOND = 0,	// �_�C�������h
		GEM_RUBY,			// ���r�[
		GEM_SAPPHIRE,		// �T�t�@�C�A
		GEM_EMERALD,		// �G�������h
		GEM_TOPAZ,			// �g�[�p�Y
		GEM_MAX
	};

	// ���΂̎��
	enum Fossil_Type
	{
		FOSSIL_HEAD = 0,		// ������
		FOSSIL_AMBER,		// �R�n�N
		FOSSIL_BONE,		// �z�l
		FOSSIL_FOOTPRINS,	// ��������
		FOSSIL_AMMONITE,	// �A�����i�C�g
		FOSSIL_MAX
	};

	// �f�u���̎��
	enum Debris_Type
	{
		DEBRIS_CPUCHIP = 0,	// CPU�`�b�v
		DEBRIS_PANEL,		// �p�l��
		DEBRIS_ANTENNA,		// �A���e�i
		DEBRIS_ENGINE,		// �G���W��
		DEBRIS_OXYGENBONB,	// �_�f�{���x
		DEBRIS_MAX
	};

public:
	// �X�R�A�A�C�e���̏��
	typedef struct
	{
		int nType;			// �X�R�A�A�C�e���̎��
		std::string name;			// ���O
		D3DXCOLOR col;		// �F
		int value;			// �l�i
		int getTime;		// ���l�����ꂽ��(�^�C�}�[�̐��l)
	}ScoreItem;

	CScoreItemStack();
	~CScoreItemStack() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void LostScoreItem(const int lost, const D3DXVECTOR3 pos);	// �X�R�A�A�C�e���̕�������
	int SaleScoreItem();										// �X�R�A�A�C�e���̔��p����
	void SetScoreItem(const int type);							// �X�R�A�A�C�e���̎�ނ̐ݒ�
	void SetDefault(const int num);							// �v���C���[����ő及�������l��

	int GetScoreItemSize() { return (int)m_listScoreItem.size(); }	// �X�R�A�A�C�e���̏�����

	static CScoreItemStack* Create(const D3DXVECTOR3 pos, const int index);

private:
	static const int Max_Receipt_NameString = 6;	// ���V�[�g�̖��O������
	static const float ScoreItem_Stack_Size;		// �X�R�A�A�C�e���̉摜�̃T�C�Y

	D3DXVECTOR3 m_pos;								// �ʒu

	CReceipt *m_pReceipt;							// �̎����̏��
	std::vector <ScoreItem> m_listScoreItem;		// �X�R�A�A�C�e���̏��
	CObject2D* m_apScoreItem[20];					// �X�R�A�A�C�e���̉摜
};

#endif// _SCOREITEM_STACK_H_