//=============================================================================
//
// receipt.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _RECEIPT_H_
#define _RECEIPT_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include <vector>
#include <string>
#include "object.h"

//---------------------------
// �N���X�錾
//---------------------------
class CFontString;

//---------------------------
// �N���X�錾
//---------------------------
class CReceipt : public CObject
{
public:

	CReceipt();
	~CReceipt() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Total();								// ���V�[�g�̍��v�l�̏���
	void DestroyString();						// ������̔j������

	void SetString(const std::string letter);	// ������̐ݒ�
	void SetStringAnchor();						// ������̃A���J�[�|�C���g�̐ݒ菈��
	void SetTotalValue(const int value) { m_nTotalValue += value; }	// ���v�l�i

	static CReceipt* Create(const int index);

private:
	static const int Max_Row = 5;						// �̎����̍ő�s��
	static const int Max_Len = 10;						// �̎����̍ő剡������
	static const int String_Death_Timer = 30;			// �폜�������̍Ō�̕����񂪏�����܂ł̎���
	static const int Setting_String_Death_Timer = 300;	// �ǉ��������̍Ō�̕����񂪏�����܂ł̎���
	static const float FontString_Blink_Speed;			// �_�ő��x
	static const D3DXVECTOR3 Font_Size;					// ������̃T�C�Y

	D3DXVECTOR3 m_pos;								// �ʒu
	bool m_bRight;									// �z�u�����E���̔���
	int m_nStringDesthCounter;						// �Ō�̕����񂪏�����܂ł̃J�E���^�[
	int m_nTotalValue;								// ���v�l�i

	CFontString* m_pTotal;							// ���V�[�g�̍��v
	std::vector <CFontString*> m_apFontString;		// ������̃|�C���^
};

#endif// _RECEIPT_H_