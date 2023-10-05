//=============================================================================
//
// Letter.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef LETTER_H
#define LETTER_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object2D.h"

class CLetter : public CObject_2D
{
public:
	CLetter();																//コンストラクタ
	CLetter(const int nPriority);											//コンストラクタ
	~CLetter() override;													//デストラクタ

	HRESULT Init(void) override;											//初期化処理
	void Uninit(void) override;												//終了処理
	void Update(void) override;												//更新処理
	void Draw(void) override;												//終了処理

	bool Convert(const char symbol);										//文字の変換処理

	void SetPrevLetter(CLetter* pPrev);										//前の文字の設定処理
	void SetNextLetter(CLetter* pNext);										//次の文字の設定処理
																			
	CLetter* GetPrevLetter(void);											//前の文字の取得処理
	CLetter* GetNextLetter(void);											//次の文字の取得処理

	static CLetter* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const char letter);			//生成処理
	static CLetter* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const char letter, const int nPriority);			//生成処理

private:

	CLetter* m_pPreviousLetter;		//前の文字
	CLetter* m_pNextLetter;			//次の文字
};

#endif
