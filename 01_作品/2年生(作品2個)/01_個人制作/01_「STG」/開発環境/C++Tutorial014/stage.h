//=============================================================================
//
// ステージ
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

//---------------------------
//前方宣言
//---------------------------
class CPlayer;
class CBG;

enum Stage_Mode
{
	Stage_1 = 0,	//ステージ1
	Stage_2,		//ステージ2
	Stage_3,		//ステージ3
	Stage_4,		//ステージ4
	Stage_Boss,		//ボスステージ
	Stage_MAX,		//最大値
};

//---------------------------
//クラス宣言
//---------------------------
class CStage
{
public:
	CStage();
	~CStage();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void GameManager();
	void ArrayStage();
	void ChangeStage();
	void BlinkStage();

	static int GetLoop() { return m_nLoop; }
	static int GetGameTimer() { return m_nGameTimer; }		// ゲーム経過時刻取得
	static bool GetClear() { return m_bStageClear; }
	static bool GetBomm() { return m_bBomm; }
	static Stage_Mode GetStageMode() { return m_mode; }		//ステージモード取得
private:
	int m_nBlinkCounter;
	int m_nDirectLoop;			//現時点
	bool m_bWall;
	bool m_bBlink;				//点滅判定
	bool m_bChangeColor;
	static int m_nLoop;			//ループ回数
	static bool m_bBomm;		//ボム判定
	static bool m_bStageClear;	//ステージクリア判定
	static int m_nGameTimer;
	static Stage_Mode m_mode;
	static Stage_Mode m_modeNext;
	static CBG *m_pBG;
};

#endif // !_STAGE_H_
