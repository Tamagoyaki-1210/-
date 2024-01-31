//============================================================================
//
// ���[�V����	motion.h
// Author : Tanimoto Kosuke
//
//============================================================================
#ifndef _MOTION_H		//���̃}�N����`������ĂȂ�������
#define _MOTION_H		//2�A�C���N���[�h�h�~�̃}�N����`

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "d3dx9.h"
#include <vector>
#include <map>

//---------------------------
// �N���X�錾
//---------------------------
class CMotion
{
public:
	// ���[�V�����̏��
	enum MOTION
	{
		MOTION_PLAYER_BODY = 0,	// �̃p�[�c
		MOTION_PLAYER_ARMS,		// �r�p�[�c
		MOTION_PLAYER_LEG,		// �r�p�[�c

		MOTION_MOB,				// ���u�p�[�c

		MOTION_BOSS,			// �{�X�p�[�c

		MOTION_MAX
	};
	static const char* m_cMotionFileName[MOTION_MAX];		// �t�@�C����

	// �L�[�̏��
	struct KEY
	{
		D3DXVECTOR3 KeyPos;		// �L�[�̈ʒu
		D3DXVECTOR3 KeyRot;		// �L�[�̉�]
	};

	// �L�[�Z�b�g�̏��
	struct KEY_SET
	{
		int nFrame;				// �t���[��
		std::vector <KEY> aKey;	// �L�[�̏��
	};

	// ���[�V�����Z�b�g�̏��
	struct MotionPattern
	{
		int nMaxKey;					// �L�[�̑���
		bool bLoop;						// ���[�v����
		std::vector <KEY_SET> aKeySet;	// �L�[�Z�b�g�̏��
	};

	CMotion();
	~CMotion();

	void SetMotionData(MotionPattern motion, std::string name);	// ���[�V�����l�̓ǂݍ���
	void SetUseFileName(const char* Xfilename) { m_UseFileName.push_back(Xfilename); }

	MotionPattern GetMotionPattern(const int index, std::string name) { return m_MotionPatternData.at(name)[index]; }
	std::vector<std::string> GetUseFileName() { return m_UseFileName; };

	void ReleaseAll();							// �S�Ẵ��[�V�����̔j������

	void LoadFile(const char* Xfilename);
	void LoadAllFile();							// �S�Ẵ��f���p�[�c�̓ǂݍ���

private:

	std::map<std::string, std::vector <MotionPattern>> m_MotionPatternData;	// ���[�V�����̃f�[�^(���g�̃��[�V������, ���[�V�����̔ԍ�)
	std::vector<std::string> m_UseFileName;									// �g�p���t�@�C���̃��[�V������
};
#endif// !_MOTION_H