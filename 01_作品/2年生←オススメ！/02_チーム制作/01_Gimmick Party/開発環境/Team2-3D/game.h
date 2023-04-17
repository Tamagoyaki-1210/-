//=============================================================================
//
// game.h
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "mode.h"

//---------------------------
//クラス宣言
//---------------------------
class CGame : public CMode
{
public:
	CGame();
	~CGame();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;

	static void SetEndGame();							// ゲーム終了の設定処理
	static bool GetEndGame() { return m_bEndGame; }		// ゲーム終了の取得処理

private:
	static bool m_bEndGame;	// ゲーム終了判定
};

#endif // !_GAME_H_
