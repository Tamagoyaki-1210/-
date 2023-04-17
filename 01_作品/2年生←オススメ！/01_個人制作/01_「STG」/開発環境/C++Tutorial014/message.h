//=============================================================================
//
// ���b�Z�[�W
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//�N���X
//---------------------------
class CMessage : public CObject2D
{
public:
	enum Message_Type
	{
		TYPE_THISNUMBER = 0,	//����̃X�R�A
		TYPE_TITLE,				//�^�C�g�����S
		TYPE_PAUSE,				//�|�[�Y���
		TYPE_MAX,
	};

	CMessage();
	~CMessage() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CMessage *Create(D3DXVECTOR3 pos, Message_Type type);
	void Load();
private:
	float m_fWidth;			//������
	float m_fHeight;		//�����c
	Message_Type m_type;	//�i���o�[���
};

#endif // !_MESSAGE_H_
