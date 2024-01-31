//==============================================================================================
//
// ���U���g�@�@�@result.h
// tutida ryousei
// tanimoto kosuke
// Saito shian
//
//==============================================================================================
#ifndef _RESULT_H_
#define	_RESULT_H_

//==============================================================================================
// �C���N���[�h
//==============================================================================================
#include"mode.h"

//==============================================================================================
// �O���錾
//==============================================================================================
class CHalfSphere;
class CObjectX;
class CPlayerManager;
class CPlayer_Parameter;
class CWeapon_Parameter;
class CObject2D;
class CParts;
class CFontString;
class CScore;
class CTime;
class CNumber;

//==============================================================================================
// ���U���g�N���X
//==============================================================================================
class CResult : public CMode
{
	const float SizeXScaleSpeed = 26.8f;			// x�T�C�Y�g��k���X�s�[�h
	const float SizeYScaleSpeed = 22.5f;			// y�T�C�Y�g��k���X�s�[�h
public:

	CResult();										// �R���X�g���N�^
	~CResult();										// �f�X�g���N�^

	HRESULT Init() override;						// ����������
	void Uninit() override;							// �I������
	void Update() override;							// �X�V����

	void SetTotalDamage(int num, D3DXVECTOR3 pos, D3DXVECTOR2 size);			// �󂯂��_���[�W�̐ݒ菈��
	void SetDeathCount(int num, D3DXVECTOR3 pos, D3DXVECTOR2 size);				// �|�����G�̐ݒ菈��
	void ScaleExpansion();							// �T�C�Y�g�又��
	void InformationUninit();						// ���̔j��
	void ZoroDamageCount();
	void ZoroDeathCount();
			
	static CResult* Create();						// ��������
	//static CPlayer_Parameter *GetPlayerParameter() { return m_pPlayerParameter; }	// �v���C���[�p�����[�^�擾����
	static CPlayerManager* GetPlayerManager() { return m_pPlayerManager; }

private:
	//static CPlayer_Parameter *m_pPlayerParameter;	// �v���C���[�p�����[�^�[
	static CPlayerManager* m_pPlayerManager;		// �v���C���[�}�l�[�W���[
	CWeapon_Parameter *m_pWeaponParameter;			// ����p�����[�^�[
	CParts* m_pParts[3];							// �p�[�c
	CHalfSphere* m_pHalfSphere;						// �n�[�t�X�t�B�A
	CObject2D* m_pObject2D;							// �I�u�W�F�N�g2D
	CObject2D* m_pFontTexture[3];					// �t�H���g�e�N�X�`��
	CScore* m_pScore;								// �X�R�A
	CTime* m_pTime;									// �^�C��
	CNumber* m_apTotalDamage[6];					// �i���o�[
	CNumber* m_apDeathCount[3];						// �i���o�[
	D3DXVECTOR3 m_pos;								// �ʒu
	D3DXVECTOR3 m_move;								// �ړ���
	D3DXVECTOR2 m_size;								// �T�C�Y
	int m_aPosTexU[6];								//�e���̃X�R�A���i�[
	int m_nZeroCountTotalDamage;					// 0�̐�
	int m_nZeroCountDeathCount;					// 0�̐�
	int m_nNum;										// �l
	int m_MoveCount;								// �ړ�����
	int m_PlayerIndex;								// �v���C���[�̔ԍ�
	bool m_bGetFlag;								// �擾�t���O
	bool m_bCreateFlag;								// �����t���O
};

#endif // !_GAME_H_
