//=============================================================================
//
// player_parameter.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _PLAYER_PARAMETER_H_
#define _PLAYER_PARAMETER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "d3dx9.h"
#include "player.h"

//---------------------------
// �N���X�錾
//---------------------------
class CPlayer_Parameter
{

public:
	static const int PLAYER_PARTS_TYPE_MAX = 17;	// �v���C���[�̃p�[�c��ނ̍ő吔
	static const int PARTS_RARITY = 3;				// �p�[�c�̃��A���e�B

	// �p�����[�^�[
	struct PARAMETERS
	{
		int nLife;					// �ϋv�l
		int nStamina;				// �X�^�~�i�e��
		int nStan_Tolerance;		// �X�^�����e�l
		int nGravity;				// �d��

									//Skill m_skill;					// �X�L���^�C�v
	};

	CPlayer_Parameter();
	~CPlayer_Parameter();

	HRESULT Init();
	void Uninit();

	void LoadJobFile();		// �W���u�̓ǂݍ���
	void LoadPartsFile();	// �p�[�c�̓ǂݍ���

	// �v���C���[�p�����[�^�[�̎擾
	PARAMETERS GetParameterJob(const int job) { return m_Parameter_Job[job]; }

	// �r�p�����[�^�[�̎擾
	PARAMETERS GetParameterArms(const int type, const int rearity) { return m_Parameter_Arms[type][rearity]; }

	// �r�p�����[�^�[�̎擾
	PARAMETERS GetParameterLeg(const int type, const int rearity) { return m_Parameter_Leg[type][rearity]; }

private:

	PARAMETERS m_Parameter_Job[CPlayer::JOB_MAX];						// �W���u�����̃p�����[�^�[
	PARAMETERS m_Parameter_Arms[PLAYER_PARTS_TYPE_MAX][PARTS_RARITY];	// �r�̃p�����[�^�[
	PARAMETERS m_Parameter_Leg[PLAYER_PARTS_TYPE_MAX][PARTS_RARITY];	// �r�̃p�����[�^�[
};

#endif// _PLAYER_PARAMETER_H_