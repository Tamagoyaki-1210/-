//=========================================
// 
// ゲーム内処理(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _GAME_H_
#define _GAME_H_

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitGame(void);	// 初期化処理
void UninitGame(void);	// 終了処理
void UpdateGame(void);	// 更新処理
void DrawGame(void);	// 描画処理
void SetNeedTime(int nPoint);
void SetNeedPoint(int nPoint);
void SetNeedSet(int nPoint);
bool* GetResetScore(void);
void SetBackground(void);
void SetEnablePause(bool bUse);	// ポーズの状態を設定する

#endif // !_GAME_H_
