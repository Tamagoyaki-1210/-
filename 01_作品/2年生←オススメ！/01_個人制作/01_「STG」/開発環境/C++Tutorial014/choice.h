//=============================================================================
//
// �I����
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _CHOICE_H_
#define _CHOICE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//�N���X
//---------------------------
class CChoice : public CObject2D
{
public:
	//�I�������
	enum Choice_Type
	{
		TITLE_GAMESTART = 0,	//�^�C�g��_�Q�[���J�n
		TITLE_EXIT,			//�^�C�g��_�I��
		GAME_CONTINUE,		//�Q�[��_�ĊJ
		GAME_EXIT,			//�Q�[��_�I��
		TYPE_MAX
	};

	CChoice();
	~CChoice() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void InputChoice(void);

	static void ResetId();

	static CChoice *Create(D3DXVECTOR3 pos, Choice_Type type);
	void Load();
private:
	int m_nId;					//�I�����̔ԍ�
	static int m_nNumAll;		//�S�̐�
	static int m_nSelect;		//���݂̑I�����̈ʒu
	static int m_nSkipSelect;	//�X�L�b�v��
	float m_fWidth;				//������
	float m_fHeight;			//�����c
	Choice_Type m_type;
};

#endif // !_CHOICE_H_
