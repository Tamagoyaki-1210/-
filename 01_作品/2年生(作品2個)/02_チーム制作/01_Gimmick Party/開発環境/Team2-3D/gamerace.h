//=============================================================================
//
// gamerace.h
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _GAMERACE_H_
#define _GAMERACE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "game.h"

//---------------------------
//前方宣言
//---------------------------
class CStage;

//---------------------------
//クラス宣言
//---------------------------
class CGameRace : public CGame
{
public:
	CGameRace();
	~CGameRace();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;

	static CStage* GetStage(void) { return m_pStage; }	// ステージの取得処理

	static CGameRace* Create(void);			//生成処理
private:
	static CStage* m_pStage;
};

#endif // !_GAMERACE_H_

