//=============================================================================
//
// chardecision_window.h (キャラ決定ウィンドウ.h)
// Author : Saito Shian
//
//=============================================================================
#ifndef  _CHARDECISISION_WINDOW_H_				//このマクロ定義がされてなかったら
#define  _CHARDECISISION_WINDOW_H_				//二重インクルード防止のマクロ定義

//=============================================================================
// インクルード
//=============================================================================
#include "main.h"
#include "object.h"
#include "d3dx9.h"
#include "fontString.h"
#include <vector>

//=============================================================================
// 前方宣言
//=============================================================================
class CObject2D;
class CFontString;
class CConfirmation_Window;
class CNumber;
class CPlayer_Parameter;

//=============================================================================
// キャラ決定ウィンドウクラス
//=============================================================================
class CCharDecision_Window
{
	static const int MAX_TEXTURE = 2;							// テクスチャの最大数
	static const float BLINK_SPEED;								// 選択肢の点滅速度
	static const float MINI_ALPHA;								// 選択肢の最低透明値
	const float SizeXScaleSpeed = 31.8f;						// xサイズ拡大縮小スピード
	const float SizeYScaleSpeed = 27.5f;						// yサイズ拡大縮小スピード

public:
	CCharDecision_Window();										// キャラ決定ウィンドウのコンストラクタ
	~CCharDecision_Window();									// キャラ決定ウィンドウのデストラクタ

	HRESULT Init();												// 初期化処理
	void Uninit();												// 終了処理
	void Update();												// 更新処理

	void UninitExplanation();									// 説明用テクスチャ、フォントの破棄処理
	void CharDecisionMenuScale();								// キャラ決定のサイズ処理
	void CharSelectChoice();									// 選択肢の処理
	void BackWindow();											// ウィンドウを戻す処理
	void SlideWindow();											// ウィンドウをスライドさせる処理
	void PlayerIndex();											// プレイヤー番号処理
	bool CharDecisionMenuScaleReduce();							// キャラ決定のサイズ縮小処理

	void SetFont(const std::string lette);									// フォントの設定処理
	void SetTextue(CTexture::TEXTURE texture);		// テクスチャの設定
	void SetParameterText();						// パラメーターのテキスト設定

	int GetSelectIndex() { return m_nSelectIndex; }
	bool GetSapawnWindow() { return m_bSpawnWindow; }							// ウィンドウが生成されたかの取得処理
	bool GetMaxScale() { return m_bMaxSize; }									// ウィンドウが最大サイズまで行ったのかを取得する処理
	bool GetExplanationUninit() { return m_bExplanationUninit; }				// フォント、テクスチャが削除されたかの取得処理		
	bool GetUninit() { return m_bUninitFlag; }									// 削除フラグの取得処理
	bool GetScaleReduce() { return m_bScaleReduce; }

	std::vector<CFontString*> GetChoiceAll() { return m_vpListChoice; }										// 選択肢ポインタの取得処理
	static CCharDecision_Window* Create(D3DXVECTOR3 pos, float xsize, float ysize, D3DXCOLOR col);			// 生成処理

private:
	static CConfirmation_Window* m_pConfirmation;
	static CPlayer_Parameter *m_pPlayer_Parameter;	// プレイヤーパラメーター
	CObject2D* m_pWindow;						// ウィンドウ用オブジェクト2Dのインスタンス
	CObject2D* m_pObject2D;		// 説明用オブジェクト2Dのインスタンス
	CFontString* m_pFont;						// フォント
	CFontString* m_pParameter_Font[2];			// パラメーター用のフォント
	CNumber* m_pSelect_Number;					// 選択番号
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR2 m_size;							// サイズ
	D3DXVECTOR2 m_MaxSize;						// 最大サイズ
	D3DXCOLOR m_Color;							// 色

	std::vector<CFontString*> m_vpListChoice;	// 選択肢ポインタの配列

	int		m_nSelectIndex;						// 何番目のスキルが選択されたかの保存変数

	float	m_fBlinkSpeed;						// 点滅速度
	float	m_fMoveX;							// スライドの移動量

	bool	m_bMaxSize;							// XとYのサイズが最大値まで行ったか
	bool    m_bSlideFlag;						// スライドフラグ
	bool    m_bFontFlag;						// フォントが使用されているか
	bool    m_bLeftRight;						// 左右どっちに移動するか
	bool    m_bSpawnWindow;						// ウィンドウが生成されたか
	bool    m_bStopFlag;						// スライドした時に停止するフラグ
	bool    m_bExplanationUninit;				// テクスチャとフォントの削除フラグ
	bool    m_bPosDest;							// 目的の位置まで行ったのか
	bool    m_bScale;							// 拡大縮小フラグ
	bool	m_bDicision;						// 決定されたか
	bool    m_bUninitFlag;						// 削除フラグ
	bool    m_bScaleReduce;						// 縮小
	bool	m_bScaleExpansion;					// 拡大
	bool    m_bSelect;
	bool    m_bTextureFlag;
};
#endif