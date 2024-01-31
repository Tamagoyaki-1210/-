//==============================================
//
// number.h (�i���o�[.h)
// Author : Saito Shian
//
//==============================================
#ifndef  _NUMBER_H_				//���̃}�N����`������ĂȂ�������
#define  _NUMBER_H_				//��d�C���N���[�h�h�~�̃}�N����`

//==============================================
// �C���N���[�h
//==============================================
#include "main.h"
#include "object2D.h"

//==============================================
// �i���o�[�N���X
//==============================================
class CNumber : public CObject2D
{
public:

	//****************
	// public�Ȋ֐�
	//****************
	CNumber(const PRIORITY priority);		// �i���o�[�̃R���X�g���N�^
	~CNumber() override;					// �i���o�[�̃f�X�g���N�^
	 
	HRESULT Init() override;				// ����������
	void Uninit() override;					// �I������
	void Update() override;					// �X�V����
	void Draw() override;					// �`�揈��

	static CNumber* Create(const D3DXVECTOR3 &pos, float xsize, float ysize, const PRIORITY priority);	// ����

private:
	//****************
	// private�ȕϐ�
	//****************
};
#endif
