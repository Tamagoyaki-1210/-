//==============================================================================
//
// score.h
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// 2�d�C���N���[�h�K�[�h
//==============================================================================
#ifndef _SCORE_H_
#define	_SCORE_H_

//==============================================================================
// �C���N���[�h
//==============================================================================
#include "main.h"
#include "object.h"
#include "ui.h"

//==============================================================================
// �O���錾
//==============================================================================
class CNumber;

//==============================================================================
// �}�N����`
//==============================================================================
#define SCORE_SIZE_X		 (20.0f)	//�X�R�A��x�T�C�Y
#define SCORE_SIZE_Y		 (35.0f)	//�X�R�A��y�T�C�Y
#define SCORE_DIGIT			 (8)		//�X�R�A�̌���

//==============================================================================
// ������\������N���X
//==============================================================================
class CScore : public CUi
{
public:
	CScore();					// �R���X�g���N�^
	~CScore() override;					// �f�X�g���N�^

	HRESULT Init() override;				// ����������
	void Uninit() override;				// �I������
	void Update() override;				// �X�V����
	void Draw() override;				// �`�揈��

	void Roll();				// �����̉�]����
	void DigitDraw();			// ���̕`���ݒ肷�鏈��
	void NumberTex();			// �ԍ��̕`��

	void SetScore(int nScore);						//�X�R�A�̐ݒ菈��
	void AddScore(int nValue);						//�X�R�A�̉��Z����
	void SubScore(int nSubtract);					//�X�R�A�̌��Z����
	
	void SetPos(D3DXVECTOR3 pos) { ScorePos = pos; }
	void SetSize(D3DXVECTOR3 size) { m_Size = size; }
	void SetWidth(float width) { m_fWidth = width; }
	void SetRollSpeed(const int speed) { m_nRollSpeed = speed; }

	void SetFiver();	// �t�B�[�o�[�p�̐ݒ�

	int GetScore(void) { return m_nScore; }			//�X�R�A�̎擾����

	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float width);			//����

private:
	static const int Score_Roll_Speed = 20;			// �X�R�A�̉�]���x

	D3DXVECTOR3 ScorePos;							// �X�R�A�̈ʒu
	D3DXVECTOR3 m_Size;								// �T�C�Y
	float m_fWidth;									// �������m�̕�

	CNumber *m_apNumber[SCORE_DIGIT];				// �X�R�A�̐���(����)
	int m_nScore;									// ���݂̃X�R�A
	int m_nScoreOld;								// �O��p�X�R�A
	int m_nRollSpeed;								// ��]���x
};

#endif
