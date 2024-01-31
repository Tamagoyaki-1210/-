//==============================================================================================
//
// �v���C���[�̊Ǘ��@�@�@player_manager.h
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
		TYPE_PC = 0,	// �v���C���[����
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

	// �L�����N�^�[�̏��̐ݒ�
	CPlayer* SetPlayer(const D3DXVECTOR3 pos, const Chara_Type type, const int index);

	// �v���C���[�̏��̎擾
	CPlayer* GetPlayer(const int index) { return m_pPlayer[index]; }

	static CPlayerManager* Create();

private:

	CPlayer* m_pPlayer[MAX_PLAYER];	// �ő�4�l�܂ł̃v���C���[�|�C���^
};

#endif // !_PLAYER_MANAGER_H_