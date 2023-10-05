//=============================================================================
//
// score.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef _SCORE_H_
#define _SCORE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include <vector>

//=============================================================================
//前方宣言
//=============================================================================


class CScore
{
public:	
	CScore();							//コンストラクタ
	~CScore();							//デストラクタ
										
	HRESULT Init(void);					//初期化処理
	void Uninit(void);					//終了処理
	void AddScore(const int nScore);	//スコアを足す処理	
	const int GetScore(void);			//スコアを取得処理
									
	static void AddScore(const int nPlayerIdx, const int nScore);	//スコアを足す処理
	static const int GetScore(const int nPlayerIdx);				//スコアを取得処理
	static void Clear(void);										//クリア処理
	static CScore* Create(const int nPlayerIdx);					//生成処理

private:

	int m_nScore;					//スコア
	int m_nPlayerIdx;				//プレイヤーのインデックス

	static std::vector <CScore*> m_vScore;				//スコアへのポインタを持っているベクトル

};



#endif