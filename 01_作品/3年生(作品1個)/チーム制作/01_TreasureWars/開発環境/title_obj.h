//==============================================================================
//
// title_obj.h	(�^�C�g���̃I�u�W�F�N�g.h)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// 2�d�C���N���[�h�K�[�h
//==============================================================================
#ifndef _TITLE_OBJ_H_
#define	_TITLE_OBJ_H_

//==============================================================================
// �C���N���[�h
//==============================================================================
#include "main.h"
#include "objectX.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//���W,�@��,�J���[	���e�N�X�`�����g���ꍇ�͂����ɒǉ�(�\���̂ɂ��ǉ�)

//==============================================================================
// �O���錾
//==============================================================================
class CScore;
class CResult;

//==============================================================================
// �}�b�v�N���X
//==============================================================================
class CTitleObj : public CObjectX
{
	static const float MOVING_ORESPEED;
	static const float MOVING_PILEOFBILLSSPEED;
	static const int CREATE_TIME;

public:
	enum ObjType
	{
		ObjType_None = 0,
		ObjType_Ore,				// �z��
		ObjType_Pileofbills,		// �D��
		ObjType_Max
	};
	CTitleObj();					// �R���X�g���N�^
	~CTitleObj();					// �f�X�g���N�^

	HRESULT Init()override;			// ����������
	void Uninit()override;			// �I������
	void Update()override;			// �X�V����

	void SetObjType(ObjType type) { m_ObjType = type; }	// �I�u�W�F�N�g�^�C�v�̐ݒ� }

	static CTitleObj *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, char *Xfilename, ObjType type);

private:
	D3DXVECTOR3 m_OreMove;				// �z�΂̈ړ���
	D3DXVECTOR3 m_PileofbillsMove;		// �D���̈ړ���
	D3DXVECTOR3 m_rot;					// ��]
	D3DXVECTOR3 m_pos;					// �ʒu

	ObjType m_ObjType;					// �I�u�W�F�N�g�^�C�v

	int m_PileofbillsTime;				// �D�����o�Ă���܂ł̎���

	float m_RandX;						// �����_����x�l
	float m_RandY;						// �����_����y�l
	float m_RandRotX;					// ��]�̃����_��x�l
};

#endif

