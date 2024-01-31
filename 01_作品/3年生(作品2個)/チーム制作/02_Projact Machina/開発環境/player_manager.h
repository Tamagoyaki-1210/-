//==============================================================================================
//
// プレイヤーの管理　　　player_manager.h
// tutida ryousei
// tanimoto kosuke
//
//==============================================================================================
#ifndef _PLAYER_MANAGER_H_
#define	_PLAYER_MANAGER_H_

#include "player.h"

class CPlayerManager
{
public:
	static const int MAX_PLAYER = 4;

	enum Chara_Type
	{
		TYPE_PC = 0,	// プレイヤー操作
		TYPE_CPU,		// CPU
		TYPE_MAX,
		TYPE_NONE
	};

	CPlayerManager();
	~CPlayerManager();

	HRESULT Init();
	void Uninit();

	void PlayerReleaseAll();

	void PlayerRelease(const int index);

	// キャラクターの情報の設定
	CPlayer* SetPlayer(const D3DXVECTOR3 pos, const Chara_Type type, const int index);

	// プレイヤーの情報の取得
	CPlayer* GetPlayer(const int index) { return m_pPlayer[index]; }

	static CPlayerManager* Create();

private:

	CPlayer* m_pPlayer[MAX_PLAYER];	// 最大4人までのプレイヤーポインタ
};

#endif // !_PLAYER_MANAGER_H_