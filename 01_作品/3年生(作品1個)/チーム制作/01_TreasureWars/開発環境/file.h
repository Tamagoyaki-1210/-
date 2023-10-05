//==============================================================================================
//
// ファイル　　　file.h
// tutida ryousei
//
//==============================================================================================
#ifndef _FILE_H_
#define	_FILE_H_

#include <d3dx9.h>
#include "charmanager.h"

class CFile
{
public:
	CFile();
	~CFile();

	static CCharacter *LoadCharFile(const CCharManager::Chara_Type type, const int index, int timetype);	// キャラの読み込み

private:
	static const int MAX_MODEL = 128;		// 読み込む最大モデル数
};

#endif // !_FILE_H_
