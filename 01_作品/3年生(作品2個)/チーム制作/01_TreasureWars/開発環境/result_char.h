//==============================================================================================
//
// リザルトのキャラ	　　result_char.h
// tutida ryousei
//
//==============================================================================================
#ifndef _RESULT_CHAR_H_
#define _RESULT_CHAR_H_

#include"character.h"

class CModel;

class CResult_Char : public CCharacter
{
public:
	CResult_Char();
	~CResult_Char() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CResult_Char *Create(D3DXVECTOR3 pos, int num);

private:

};

#endif	// !_RESULT_CHAR_H_