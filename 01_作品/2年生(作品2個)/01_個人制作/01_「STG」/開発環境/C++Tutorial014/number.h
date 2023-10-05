//=============================================================================
//
// ナンバー
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//マクロ関数
//---------------------------
#define RNKING_FILE			"data/.txt/ranking.txt"		//ランキング読み込みファイル
#define THIS_SCORE_FILE		"data/.txt/thisScore.txt"	//現在スコア読み込みファイル

#define	BASE_NUMBER		(10)		//10進数

//---------------------------
//クラス
//---------------------------
class CNumber : public CObject2D
{
public:
	enum Number_Type
	{
		TYPE_SCORE = 0,	//スコア
		TYPE_RANKING,	//ランキング
		TYPE_MAX,
	};

	CNumber();
	~CNumber() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetNumber(int nScore);
	static CNumber *Create(D3DXVECTOR3 pos, Number_Type type);
	void Load();
private:
	float m_fWidth;		//長さ横
	float m_fHeight;	//長さ縦
	Number_Type m_type;		//ナンバー種別
};

#endif // !_NUMBER_H_
