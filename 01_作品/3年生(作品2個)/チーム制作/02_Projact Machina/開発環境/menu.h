//=============================================================================
//
// menu.h
// Author : Tanimoto Kosuke
// Author : Saito Shian
//
//=============================================================================
#ifndef _MENU_H_
#define _MENU_H_

//=============================================================================
// インクルード
//=============================================================================
#include "d3dx9.h"
#include "fontString.h"
#include <vector>

//=============================================================================
// クラス宣言
//=============================================================================
class CMenu
{
	static const float CHOICE_BLINK_SPEED;		// 選択肢の点滅速度
	static const float CHOICE_MINI_ALPHA;		// 選択肢の最低透明値

public:
	CMenu();									// コンストラクタ
	virtual ~CMenu();							// デストラクタ

	virtual HRESULT Init();						// 初期化処理
	virtual void Uninit();						// 終了処理
	virtual void Update();						// 更新処理

	virtual void Choice();						// 選択肢の処理
	void ChangeChoice(const int nextChoice);	// 選択肢を変更した時の処理

	void SetMenuInitiative(const int index) { m_nMenuInitiative = index; }		// メニューの主導権処理
	void SetDecition(const bool decition) { m_bDecition = decition; }			// 選択肢の番号の設定処理
	void SetChoice(CFontString* choice) { m_vpListChoice.push_back(choice); }	// 選択肢処理
	void SetDisplay(const bool display);										// 選択肢の表示判定を設定する

	const int GetMenuInitiative() { return m_nMenuInitiative; }					// メニューの主導権の取得処理
	const int GetSelectChoice() { return m_nSelectChoice; }						// 選択肢の取得処理
	const bool GetDecition() { return m_bDecition; }							// 選んだ選択肢の取得処理
	const bool GetDisplay() { return m_bDisplay; }								// 表示中の判定取得処理
	CFontString* GetChoice(const int index) { return m_vpListChoice[index]; }	// 選択肢ポインタ番号の取得処理
	std::vector<CFontString*> GetChoiceAll() { return m_vpListChoice; }			// 選択肢ポインタの取得処理

private:

	int		m_nMenuInitiative;					// メニューの主導権
	int		m_nSelectChoice;					// 選択肢の番号

	float	m_fBlinkSpeed;						// 点滅速度

	bool	m_bDecition;						// 決定の判定
	bool	m_bDisplay;							// 表示中の判定

	std::vector<CFontString*> m_vpListChoice;	// 選択肢ポインタの配列
};

#endif // !_MENU_H_