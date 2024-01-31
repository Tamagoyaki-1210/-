//=============================================================================
//
// parts_file.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _PARTS_FILE_H_
#define _PARTS_FILE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================

#include "objectX.h"
#include "motion.h"

class CParts;

//---------------------------
// �N���X�錾
//---------------------------
class CParts_File
{

public:
	// �p�[�c�̏��
	enum PARTS
	{
		PARTS_PLAYER_BODY_1 = 0,	// �̃p�[�c1
		PARTS_PLAYER_BODY_2,		// �̃p�[�c2
		PARTS_PLAYER_BODY_3,		// �̃p�[�c3
		PARTS_PLAYER_BODY_4,		// �̃p�[�c4
		//PARTS_PLAYER_BODY_5,		// �̃p�[�c5

		PARTS_PLAYER_ARMS_0,		// �r�p�[�c0(�X�^���_�[�h)
		PARTS_PLAYER_ARMS_1,		// �r�p�[�c1
		PARTS_PLAYER_ARMS_2,		// �r�p�[�c2
		PARTS_PLAYER_ARMS_3,		// �r�p�[�c3
		PARTS_PLAYER_ARMS_4,		// �r�p�[�c4
		//PARTS_PLAYER_ARMS_5,		// �r�p�[�c5

		PARTS_PLAYER_LEG_0,			// �r�p�[�c0(�X�^���_�[�h)
		PARTS_PLAYER_LEG_1,			// �r�p�[�c1
		PARTS_PLAYER_LEG_2,			// �r�p�[�c2
		PARTS_PLAYER_LEG_3,			// �r�p�[�c3
		PARTS_PLAYER_LEG_4,			// �r�p�[�c4
		//PARTS_PLAYER_LEG_5,			// �r�p�[�c5

		PARTS_MOB,					// ���u�p�[�c

		PARTS_BOSS,					// �{�X�p�[�c

		PARTS_MAX
	};

	// ���f���Z�b�g
	struct MODEL_SET
	{
		int nModelIndex;		// ���f���ԍ�
		D3DXVECTOR3 InitPos;	// �����ʒu
		D3DXVECTOR3 InitRot;	// ������]
		int nParentIndex;		// �e���f���̔ԍ�
	};

	// �p�[�c�Z�b�g
	struct PARTS_SET
	{
		std::string Name;					// �p�[�c��	
		std::vector<MODEL_SET> ModelSet;	// �L�����N�^�[�̃��f�����
	};

	CParts_File();
	 ~CParts_File();

	HRESULT Init();
	void Uninit();

	void SetModel(const int partsIndex, const int modelIndex, const int parent, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* Xfilename);		// ���f���p�[�c�̐ݒ�
	
	PARTS_SET GetPartsSet(const int index) { return m_PartsSet[index]; }

	void ReleaseAllFile();	// �S�Ẵ��f���Z�b�g�̏I��
	void LoadFile(const char* Xfilename, const int partsIndex);
	void LoadAllFile();																// �S�Ẵ��f���p�[�c�̓ǂݍ���

private:
	static const char* m_cPartsFileName[PARTS_MAX];		// �t�@�C����

	PARTS_SET m_PartsSet[PARTS_MAX];	// �p�[�c�̏��

};

#endif// _PARTS_FILE_H_