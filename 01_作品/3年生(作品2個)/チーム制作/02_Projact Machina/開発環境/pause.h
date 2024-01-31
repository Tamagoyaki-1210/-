//=============================================================================
//
// pause.h
// Author : tutida ryousei
//
//=============================================================================
#ifndef _PAUSE_H_
#define	_PAUSE_H_

#include"object.h"

class CObject2D;
class CFontString;

class CPause : public CObject
{
public:
	enum PAUSE
	{
		PAUSE_HEADER = 0,	// ポーズ
		PAUSE_CONTINUE,		// ゲームを続ける
		//PAUSE_STAGE_SELECT,	// ステージセレクトに戻る
		PAUSE_CHAR_SELECT,	// キャラ選択を終了する
		PAUSE_MAX,
		PAUSE_NONE
	};

	CPause();
	~CPause();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Pause();
	void Select();
	void StringCollision();

	static CPause *Create();

	bool GetPause() { return m_bPause; }

private:
	CObject2D *m_pPause;
	CFontString *m_pPause_Menu[PAUSE_MAX];
	D3DXVECTOR3 m_pPause_Pos[PAUSE_MAX];
	D3DXVECTOR2 m_StringSize;
	D3DXVECTOR2 m_StringPauseSize;
	bool m_bPause;
	bool m_Select_Pause;
	bool m_Select_Cursor;
	int m_nSelect;
};

#endif // !_PAUSE_H_
