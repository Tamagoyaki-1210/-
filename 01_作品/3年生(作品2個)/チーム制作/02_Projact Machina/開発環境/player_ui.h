//==============================================
//
// player_ui.h
// Author : Saito Shian
//
//==============================================
#ifndef _PLAYER_UI_H
#define _PLAYER_UI_H

//==============================================
// �C���N���[�h
//==============================================
#include "main.h"
#include "object2D.h"

//==============================================
// �O���錾
//==============================================

//==============================================
// �v���C���[UI�N���X
//==============================================
class CPlayerUi : public CObject2D
{
public:
	const int ALPHA_COUNT = 600;
	const int COLOR_COUNT = 10;
	const int USE_COUNT = 100;

	enum UITYPE
	{
		UITYPE_SUPPORT = 0,		// �T�|�[�g�X�L��
		UITYPE_ATTACK,			// �A�^�b�N�X�L��
		UITYPE_WEAPON,			// ������
		UITYPE_MAX
	};
	CPlayerUi(const PRIORITY priority);
	~CPlayerUi() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetAlpha();
	void SetColor();
	void SetData(float posX, float posY, float sizeX, float sizeY, CTexture::TEXTURE texture);

	UITYPE GetType() { return m_Type; }
	void SetType(UITYPE type) { m_Type = type; }

	CObject2D* GetSkillUI() { return m_pSkill_UI; }	// �X�L���摜��UI�̎擾

	static CPlayerUi* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, UITYPE type, const PRIORITY priority);

private:
	CObject2D* m_pSkill_UI;		// �X�L���摜��UI
	UITYPE m_Type;				// �X�L���̎��

	int m_AlphaCount;			// �������̎���
	int m_ColorCount;			// �F���ς�鎞��
	int m_UseCount;				// �_�Ŏ���

	float m_Alpha;				// �����x�̒l
	float m_Color;				// �F�̒l			

	bool m_bAlphaFlag;			// �������ɂȂ�����
	bool m_bColorFlag;			// �F���ς������
	bool m_bFlag;				// �X�L�����g�p���ꂽ��
};

#endif // !_PLAYER_UI_H
