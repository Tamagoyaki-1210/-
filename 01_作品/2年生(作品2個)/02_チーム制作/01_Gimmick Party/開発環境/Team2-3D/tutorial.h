//=============================================================================
//
// tutorial.h
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "game.h"

//---------------------------
//前方宣言
//---------------------------
class CStage;
class CObject_2D;
class CFontString;

//---------------------------
//クラス宣言
//---------------------------
class CTutorial : public CGame
{
public:
	CTutorial();
	~CTutorial();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void SetObject(void);

	static CStage* GetStage(void) { return m_pStage; }	// ステージの取得処理

	static CTutorial* Create(void);			//生成処理
private:
	static const D3DXVECTOR2 m_fontSize;	// フォントの大きさ
	static const D3DXCOLOR m_fontColor;		// フォントの色
	static CStage* m_pStage;				// ステージのインスタンスへのポインタ

	CObject_2D* m_pUi;
	CFontString* m_pFont;
	bool m_bChange;
};

#endif // !_GAME_H_
