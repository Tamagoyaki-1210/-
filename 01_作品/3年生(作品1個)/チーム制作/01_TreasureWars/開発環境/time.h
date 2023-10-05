//==============================================================================
//
// time.h
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// 2重インクルードガード
//==============================================================================
#ifndef _TIME_H_
#define	_TIME_H_

//==============================================================================
// インクルード
//==============================================================================
#include "object.h"
#include "number.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define TIME_DIGIT		 (3)		//時間の桁数

//==============================================================================
// 時間を表示するクラス
//==============================================================================
class CTime
{
	static const int FeverStart_Count = 30;	// フィーバータイムが始まる時間
	static const int GemCreate_Count = 5;	// 宝石を生成する間隔
	static const int FlashingTime = 55;
	static const int LastSpurtTime = 500;
	static const int MaxTime = 180;
	static const int Warning_Blink_Interval = 30;	// 警報の点滅間隔
public:
	enum Flash
	{
		Flash_In = 0,
		Flash_Out
	};
	CTime();				// コンストラクタ
	~CTime();				// デストラクタ

	HRESULT Init();			// 初期化処理
	void Uninit();			// 終了処理
	void Update();			// 更新処理

	void SetTime();							// 時間の設定
	void SetRemainingTime(int time);		// 残り時間の設定処理
	void ZoroCount();						// 先頭の0を消す処理
	void Warning(const int time);			// 警報の処理

	static CTime *Create();	//生成

	// ゲッター
	int GetTime() { return m_nTime; }

private:
	CNumber *m_apNumber[TIME_DIGIT];		// 時間の数字(桁数)
	CObject2D *m_pObject2D;					// object2Dのインスタンス
	CObject2D *m_pWarning;					// 警報オブジェクト

	Flash m_Type;							// 状態

	int m_nTime;							// 時間の値
	int m_nCntTime;							// 時間のカウンター
	int m_nZeroCount;						// 0の数
	int m_count;							// 状態変化のカウント
	int m_LastCount;						// 移動までの時間
	int m_aPosTexU[TIME_DIGIT];				// 桁数
	int m_nTimeUp_Count;					// タイムアップを表示する時間

	float m_nSizeX;							// Xのサイズ
	float m_nSizeY;							// Yのサイズ
	float m_nAlpha;							// 透明度
	float m_moveY;							// Yの移動量
	float m_moveX;							// Xの移動量

	bool m_bTimeUp;							// タイムアップしたかどうかの判定
	bool m_bLast;							// 残り時間の判定
};

#endif
