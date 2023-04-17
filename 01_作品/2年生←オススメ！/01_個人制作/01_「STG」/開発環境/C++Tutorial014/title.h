//=============================================================================
//
// タイトル
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//---------------------------
//前方宣言
//---------------------------
class CBG;

enum Title_Mode
{
	Title_1 = 0,	//ステージ1
	Title_2,		//ステージ2
	Title_3,		//ステージ3
	Title_4,		//ステージ4
	Title_MAX,		//最大値
};

//---------------------------
//クラス宣言
//---------------------------
class CTitle
{
public:
	CTitle();
	~CTitle();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void ArrayStage();
	void ChangeStage();
	void BlinkStage();
private:
	int m_nLoop;			//ループ回数
	int m_nModeCounter;		//モード持続時間
	int m_nBlinkCounter;
	int m_nGameTimer;
	bool m_bBlink;			//点滅判定
	bool m_bChangeColor;
	static Title_Mode m_mode;
	static Title_Mode m_modeNext;
	static CBG *m_pBG;
};

#endif // !_TITLE_H_
