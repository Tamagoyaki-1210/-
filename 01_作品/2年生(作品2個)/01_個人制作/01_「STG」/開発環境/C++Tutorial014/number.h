//=============================================================================
//
// �i���o�[
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define RNKING_FILE			"data/.txt/ranking.txt"		//�����L���O�ǂݍ��݃t�@�C��
#define THIS_SCORE_FILE		"data/.txt/thisScore.txt"	//���݃X�R�A�ǂݍ��݃t�@�C��

#define	BASE_NUMBER		(10)		//10�i��

//---------------------------
//�N���X
//---------------------------
class CNumber : public CObject2D
{
public:
	enum Number_Type
	{
		TYPE_SCORE = 0,	//�X�R�A
		TYPE_RANKING,	//�����L���O
		TYPE_MAX,
	};

	CNumber();
	~CNumber() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetNumber(int nScore);
	static CNumber *Create(D3DXVECTOR3 pos, Number_Type type);
	void Load();
private:
	float m_fWidth;		//������
	float m_fHeight;	//�����c
	Number_Type m_type;		//�i���o�[���
};

#endif // !_NUMBER_H_
