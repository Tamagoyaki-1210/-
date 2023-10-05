//=============================================================================
//
// ゲーム
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

//---------------------------
//前方宣言
//---------------------------
class CScore;
class CStage;

//---------------------------
//クラス宣言
//---------------------------
class CGame
{
public:
	CGame();
	~CGame();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void SetPause(bool bUse) { m_bPause = bUse; }	// ポーズ状態を設定する
	static void SetEndGame() { m_bEndGame = true; }			// ゲーム終了を設定する

	static bool GetPause() { return m_bPause; }				// ポーズ状態取得
	static CScore *GetScore() { return m_pScore; }
	static CStage *GetStage() { return m_pStage; }
private:
	static bool m_bPause;
	static bool m_bEndGame;
	static CScore *m_pScore;
	static CStage *m_pStage;
};

#endif // !_GAME_H_
