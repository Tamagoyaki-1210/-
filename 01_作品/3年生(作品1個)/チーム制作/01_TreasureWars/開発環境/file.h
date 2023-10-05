//==============================================================================================
//
// �t�@�C���@�@�@file.h
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

	static CCharacter *LoadCharFile(const CCharManager::Chara_Type type, const int index, int timetype);	// �L�����̓ǂݍ���

private:
	static const int MAX_MODEL = 128;		// �ǂݍ��ލő僂�f����
};

#endif // !_FILE_H_
