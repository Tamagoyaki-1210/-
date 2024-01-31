//==============================================
//
// score.h (�X�R�A.h)
// Author : Saito Shian
//
//==============================================
#ifndef  _SCORE_H_				//���̃}�N����`������ĂȂ�������
#define  _SCORE_H_				//��d�C���N���[�h�h�~�̃}�N����`

//==============================================
// �C���N���[�h
//==============================================
#include "main.h"
#include "object.h"

//==============================================
// �O���錾
//==============================================
class CNumber;

//==============================================
// �X�R�A�N���X
//==============================================
class CScore : public CObject
{
public:
	static const int MAX_DIGIT = 10;		// �ő包��

	//****************
	// public�Ȋ֐�
	//****************
	CScore();								// �X�R�A�̃R���X�g���N�^
	~CScore() override;						// �X�R�A�̃f�X�g���N�^

	HRESULT Init() override;				// ����������
	void Uninit() override;					// �I������
	void Update() override;					// �X�V����
	void Draw() override;					// �`�揈��

	void SetScore(__int64 nScore);				//�X�R�A�̐ݒ菈��
	void AddScore(__int64 nValue);				//�X�R�A�̉��Z����

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

	static CScore* Create(D3DXVECTOR3 pos);				// ����

private:
	//****************
	// private�ȕϐ�
	//****************
	CNumber* m_apNumber[MAX_DIGIT];			//�X�R�A�̐���(����)
	D3DXVECTOR3 m_pos;						// �ʒu
	__int64 m_aPosTexU[MAX_DIGIT];				//�e���̃X�R�A���i�[
	__int64 m_nScore;							//�X�R�A
	
};
#endif
