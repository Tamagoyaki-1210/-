//==============================================================================
//
// oremanager.h	(�z�΃}�l�[�W���[.h)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// 2�d�C���N���[�h�K�[�h
//==============================================================================
#ifndef _OREMANAGER_H_
#define	_OREMANAGER_H_

//==============================================================================
// �C���N���[�h
//==============================================================================
#include <list>
#include "main.h"
#include "object.h"

//==============================================================================
// �}�N����`
//==============================================================================

//==============================================================================
// �O���錾
//==============================================================================
class COre;

//==============================================================================
// �}�b�v�N���X
//==============================================================================
class COreManager : public CObject
{
public:
	static const float Ore_Radius;		// �z�΂̓����蔻��̑傫��(���a)

	COreManager();						// �R���X�g���N�^
	~COreManager()override;				// �f�X�g���N�^

	HRESULT Init()override;				// ����������
	void Uninit()override;				// �I������
	void Update()override;				// �X�V����
	void Draw()override;				// �`�揈��

	COre* OreCollision(const D3DXVECTOR3 pos, const float radius);
	void RandCreate();					// �����_���Ő������鏈��
	void InitCreate();					// ���������ɐ������鏈��

	void OreClear();					// �z�΂̃��X�g�𐮗����鏈��
	void OreDeath(COre* ore);			// �z�΍폜����
	void SetStageNum(int nStageNum) { m_nStageNum = nStageNum; }  //�X�e�[�W�ԍ��̕ۑ�

	std::list <COre*> GetOreList() { return m_pOreList; }	// �z�΃��X�g�̎擾

	static COreManager *Create(int nStageNum);		// ��������

private:
	static const int RANDCREATE_MAX = 10;	//�����������̍ő吔
	static const int REAPPEARANCE_MAX;		//�z�΂̍ďo����
	static const int CREATE_TIME;			//�z�΂����������܂ł̎���

	std::list <COre*> m_pOreList;		// �z�΂̃��X�g

	int m_CreateTime;					// ���������܂ł̎���
	int m_UninitTime;					// �����܂ł̎���

	float m_RandX;						// �����_����x�l
	float m_RandY;						// �����_����y�l
	float m_RandZ;						// �����_����z�l
	int m_nStageNum;					//�X�e�[�W�ԍ�
};

#endif