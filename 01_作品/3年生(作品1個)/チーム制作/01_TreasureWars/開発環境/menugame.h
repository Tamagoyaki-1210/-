//=============================================================================
//
// menugame.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _MENU_GAME_H_
#define _MENU_GAME_H_

#include "menu.h"
#include "object2D.h"

//---------------------------
// クラス宣言
//---------------------------
class CMenuGame : public CMenu
{
public:
	enum Choice_Type
	{
		Choice_None = 0,	// 何も呼ばれていない
		Choice_Continue,	// ゲームを続ける
		Choice_Entry,		// エントリー画面へ
		Choice_Title,		// タイトル画面へ
		Choice_Max
	};

	CMenuGame();
	~CMenuGame() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	void PauceObjUpdate();		// 停止中のポーズオブジェクトの更新処理
	void Pauce();				// ポーズの処理
	void Display();				// 画面表示の処理
	void Choice();				// 選択肢の処理

	static CMenuGame* Create();

private:
	bool m_bDisplay;			// 画面表示の判定
	bool m_bCurrentDisplay;		// 前回の画面表示の判定

	CFontString* m_pPauceLogo;	// ポーズロゴのオブジェクト2Dポインタ
	CObject2D* m_pFade;			// フェード擬き
};

#endif // !_MENU_GAME_H_