//==============================================================================================
//
// キャラクターの情報　　　charmanager.h
// tutida ryousei
//
//==============================================================================================
#ifndef _CHARA_MANAGER_H_
#define	_CHARA_MANAGER_H_

#include "d3dx9.h"
#include "character.h"

class CCharManager
{
public:
	static const int Max_Character = 4;

	enum Chara_Type
	{
		TYPE_PLAYER = 0,
		TYPE_CPU,
		TYPE_RESULT_CHAR,
		TYPE_MAX,
		TYPE_NONE
	};

	CCharManager();
	~CCharManager();

	HRESULT Init();
	void Uninit();

	void CharaReleaseAll();

	static CCharManager* Create();

	// キャラクターの情報の設定
	CCharacter* EntryChara(const D3DXVECTOR3 pos, const Chara_Type type, const int index, int timetype);

	void SetAllChara(int timetype);

	// キャラクターの情報の取得
	CCharacter* GetChara(const int index) { return m_pCharacter[index]; }

private:
	CCharacter* m_pCharacter[Max_Character];	// 最大4人までのキャラクターポインタ
};

#endif // !_CHARA_MANAGER_H_