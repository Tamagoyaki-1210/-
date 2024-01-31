//==============================================
//
// time.h (タイム.h)
// Author : Saito Shian
//
//==============================================
#ifndef  _TIME_H_				//このマクロ定義がされてなかったら
#define  _TIME_H_				//二重インクルード防止のマクロ定義

//==============================================
// インクルード
//==============================================
#include "main.h"
#include "object.h"

//==============================================
// 前方宣言
//==============================================
class CNumber;

//==============================================
// スコアクラス
//==============================================
class CTime
{
public:
	static const int TIME_DIGIT = 3;	// 時間の桁数
	static const int MAX_TIME = 180;	// 最大時間

	//****************
	// publicな関数
	//****************
	CTime();							// スコアのコンストラクタ
	~CTime();							// スコアのデストラクタ

	HRESULT Init();						// 初期化処理
	void Uninit();						// 終了処理
	void Update();						// 更新処理

	void SetTime();						// 時間の設定
	void SetTime(int time);
	void ZoroCount();					// 先頭の0を消す処理
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

	int GetTime();						// 時間の取得

	static CTime* Create(D3DXVECTOR3 pos);		// 生成

private:
	//****************
	// privateな変数
	//****************
	CNumber* m_apNumber[TIME_DIGIT];		// ナンバーの桁数
	D3DXVECTOR3 m_pos;						// 位置

	int m_aPosTexU[TIME_DIGIT];				// 桁数
	int m_nCntTime;							// 時間のカウント
	int m_nTime;							// 時間の値
	int m_nKeepTime;						// 時間の保存用変数
	int m_nZeroCount;						// 0の数

};
#endif
