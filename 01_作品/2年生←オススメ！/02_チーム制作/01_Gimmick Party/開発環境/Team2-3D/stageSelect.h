//=============================================================================
//
// stageSelect.h
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _STAGESELECT_H_
#define _STAGESELECT_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "mode.h"
#include "fontString.h"

//---------------------------
// 前方宣言
//---------------------------
class CFontString;

//---------------------------
//クラス宣言
//---------------------------
class CStageSelect : public CMode
{
public:
	CStageSelect();
	~CStageSelect() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;

	void Input(void);	// 入力処理

	static CStageSelect* Create(void);
private:
	int m_nNumAll;						// 選択肢の全体数
	int m_nSelectNum;					// 現在選択されている選択肢
	static CFontString* m_pStr;			// 文字列
	static CObject_2D* m_pFrame;		// 選択枠
};

#endif // !_STAGESELECT_H_
