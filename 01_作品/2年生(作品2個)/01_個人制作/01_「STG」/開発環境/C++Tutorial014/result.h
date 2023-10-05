//=============================================================================
//
// リザルト
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//---------------------------
//クラス宣言
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
	int m_nModeCounter;	//モード持続時間
};

#endif // !_RESULT_H_
