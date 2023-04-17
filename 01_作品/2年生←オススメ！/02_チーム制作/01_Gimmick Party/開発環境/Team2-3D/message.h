//=============================================================================
//
// message.h
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _MESSAGE_H
#define _MESSAGE_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object2D.h"

//---------------------------
//クラス宣言
//---------------------------
class CMessage
{
public:
	// メッセージ種類
	enum MESSAGE
	{
		MESSAGE_COUNTDOWN = 0,
		MESSAGE_START,
		MESSAGE_GOAL,
		MESSAGE_WIN,
		MESSAGE_MAX
	};
	CMessage();
	~CMessage();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void SetCountDown(int nNum);		// カウントダウンメッセージ表示処理
	void StartMessage(void);			// スタートメッセージ表示処理
	void GoalMessage(int nMessageIdx);	// ゴールメッセージ表示処理
	void WinMessage();					// 勝利メッセージ表示処理

	void Destroy();						// 破棄処理

	static CMessage* Create();			//生成処理
	static void SetStart(bool start) { m_bStart = start; }	// ゲームスタート設定処理
	static bool GetStart(void) { return m_bStart; }			// ゲームスタート取得処理

private:
	static const int CountDownLife = 60;	// カウントダウンの寿命
	static bool m_bStart;					// ゲームスタート判定

	int m_nMessageIdx;						// 勝者の番号
	int m_nMessageCounter;					// メッセージ表示時間
	int m_nNum;								// カウントダウン現在位置
	CObject_2D* m_pObj2D;					// オブジェクト2Dのポインタ
	CObject_2D* m_pPlayer2D;				// 勝者画像のポインタ
	MESSAGE m_type;							// メッセージ種類
};

#endif // !_MESSAGE_H
