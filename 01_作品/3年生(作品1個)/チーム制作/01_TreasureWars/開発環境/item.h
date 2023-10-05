//==============================================================================================
//
// �A�C�e���@�@�@	item.h
// tutida ryousei
//
//==============================================================================================
#ifndef _ITEM_H_
#define	_ITEM_H_

#include"objectX.h"
#include<vector>

class CShadow;
class CCharacter;

class CItem : public CObjectX
{
public:
	CItem();
	~CItem();

	enum Item_Type
	{
		ITEM_SPEEDUP = 0,	// �X�s�[�hUP
		ITEM_KNOCKBACK,		// �m�b�N�o�b�N����
		ITEM_STACK,			// ��x�ɂ��Ă�z�΂𑝂₷
		ITEM_MAX,
		ITEM_NONE,
	};

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	void CollItem();	// �L�����N�^�[�Ƃ̓����蔻��
	void Type(CCharacter* pChara);
	void Flash_Item();	// �A�C�e���̓_��
	void Life();

	void SetItemPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetItemSize(D3DXVECTOR3 size) { m_Size = size; }
	void SetItemCol(D3DXCOLOR col) { m_Col = col; }

	float GetRadius() { return m_fRadius; }

private:
	D3DXVECTOR3 m_Pos;	// �ʒu
	D3DXVECTOR3 m_Rot;	// �p�x
	D3DXVECTOR3 m_Size;	// �T�C�Y
	D3DXCOLOR m_Col;	// �F
	float m_fRadius;	// �����蔻��̔��a
	float m_fAlpha;		// �A���t�@�l
	bool m_bAlpha;		// �����Ă邩
	int m_nLife;		// ����
	int m_emitTime;
	CShadow *m_pShadow;
	Item_Type m_nRand_Type;
};

#endif // !_ITEM_H_