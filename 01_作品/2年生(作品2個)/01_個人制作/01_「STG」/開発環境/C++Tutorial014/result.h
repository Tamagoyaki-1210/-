//=============================================================================
//
// ���U���g
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//---------------------------
//�N���X�錾
//---------------------------
class CResult
{
public:
	CResult();
	~CResult();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Input();
private:
	int m_nModeCounter;	//���[�h��������
};

#endif // !_RESULT_H_
