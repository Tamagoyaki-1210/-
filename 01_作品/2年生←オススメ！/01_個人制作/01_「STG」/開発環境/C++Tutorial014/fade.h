//=============================================================================
//
// フェード.h
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//クラス
//---------------------------
class CFade : public CObject2D
{
public:
	enum FADE
	{
		FADE_NONE = 0,	// 何もしていない状態
		FADE_OUT,		// フェードアウト(見えない状態)
		FADE_IN,		// フェードイン(見える状態)
		FADE_CHANGE,	// フェード切り替え
		FADE_MAX
	};

	CFade();
	~CFade() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void StartFadeOut();

	static CFade *Create(float fAlpha, FADE fade);

	float GetAlpha() { return m_fAlpha; }
	FADE GetFade() { return m_fade; }	//フェード取得処理
private:
	float m_fAlpha;		// ポリゴン(フェード)のa値
	FADE m_fade;		// 現在のフェード
};

#endif // !_PLAYER_H_