//=============================================================================
//
// 選択肢
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _CHOICE_H_
#define _CHOICE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//クラス
//---------------------------
class CChoice : public CObject2D
{
public:
	//選択肢種類
	enum Choice_Type
	{
		TITLE_GAMESTART = 0,	//タイトル_ゲーム開始
		TITLE_EXIT,			//タイトル_終了
		GAME_CONTINUE,		//ゲーム_再開
		GAME_EXIT,			//ゲーム_終了
		TYPE_MAX
	};

	CChoice();
	~CChoice() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void InputChoice(void);

	static void ResetId();

	static CChoice *Create(D3DXVECTOR3 pos, Choice_Type type);
	void Load();
private:
	int m_nId;					//選択肢の番号
	static int m_nNumAll;		//全体数
	static int m_nSelect;		//現在の選択肢の位置
	static int m_nSkipSelect;	//スキップ回数
	float m_fWidth;				//長さ横
	float m_fHeight;			//長さ縦
	Choice_Type m_type;
};

#endif // !_CHOICE_H_
