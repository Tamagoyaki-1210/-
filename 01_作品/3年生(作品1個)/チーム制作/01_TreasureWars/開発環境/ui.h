//=============================================================================
//
// ui.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include <vector>
#include "object2D.h"

//---------------------------
// �N���X�錾
//---------------------------
class CUi : public CObject
{
public:
	CUi();
	virtual ~CUi() override;

	virtual HRESULT Init() override = 0;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override = 0;

	void DeathObj2D(CObject2D* obj2D);	// UI�p�I�u�W�F�N�g2D�̍폜����
	//void Dispray();						// UI�̕\������

	void SetCompulsDispray(const bool display);														// �����I��UI�\���ݒ菈��
	//void SetUiUpdate(const bool update) { m_bUiUpdate = update; }									// UI�ɍX�V������������`����
	void SetNoneDispray(const bool dispray) { m_bNoneDispray = dispray; }							// ��\���̔����ݒ�
	void SetAlpha(const float alpha) { for (auto pObj2D : m_listObj2D) pObj2D->SetAlpha(alpha); }	// ���X�g����UI�p�I�u�W�F�N�g2D�̃A���t�@�l��ݒ�
	void SetCol(const float r, const float g, const float b, const float a) { for (auto pObj2D : m_listObj2D) pObj2D->SetCol(D3DXCOLOR(r, g, b, a)); }
	void SetUiDraw(const bool draw) { for (auto pObj2D : m_listObj2D) pObj2D->SetDrawFlag(draw); }	// ���X�g����UI�p�I�u�W�F�N�g2D�̕`�攻���ݒ�
	void SetUiUV() { for (auto pObj2D : m_listObj2D) pObj2D->UV(); }								// ���X�g����UI�p�I�u�W�F�N�g2D��UV���X�V���鏈��
	void AddObj2D(CObject2D* obj2D) { m_listObj2D.push_back(obj2D); }								// UI�p�I�u�W�F�N�g2D�ɒǉ����鏈��

	static std::vector <CUi*> GetAllUi() { return m_AllUi; }										// �ÓI�ȑS�Ă�UI�|�C���^�̎擾����
private:

	//static const int Invinsible_Dispray_Max = 120;	// �ő��\�����߃J�E���^�[
	//static const int None_Dispray_Max = 160;		// �ő��\���J�E���^�[

	//int m_nNoneDispray_Counter;						// UI��\���J�E���^�[
	//bool m_bUiUpdate;								// UI�̍X�V����
	bool m_bNoneDispray;							// UI��\���ݒ菈��

	std::vector <CObject2D*> m_listObj2D;			// UI�p�I�u�W�F�N�g2D
	static std::vector <CUi*> m_AllUi;				// �ÓI�ȑS�Ă�UI�̃|�C���^
};

#endif // !_UI_H_