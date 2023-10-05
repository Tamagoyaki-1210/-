//=============================================================================
//
// UIString.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef _UI_STRING_H_
#define _UI_STRING_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object.h"

//=============================================================================
//前方宣言
//=============================================================================
class CLetter;

class CUIString : public CObject
{
public:
	CUIString();									//コンストラクタ
	CUIString(const int nPriority);					//コンストラクタ
	~CUIString() override;							//デストラクタ

	HRESULT Init(void) override;					//初期化処理
	void Uninit(void) override;						//終了処理
	void Update(void) override;						//更新処理
	void Draw(void) override;						//描画処理

	void ChangeString(const char* pString);
	void ChangeColor(const D3DXCOLOR col);
	void SetPos(const D3DXVECTOR3 pos) override;	//位置の設定処理
	const D3DXVECTOR2 GetSize(void) override;		//サイズの取得処理
	const D3DXVECTOR3 GetPos(void) override;		//位置の取得処理

	void ReleaseLetters(void);						//破棄処理

	static CUIString* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const D3DXCOLOR col, const char* pString);								//生成処理
	static CUIString* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const D3DXCOLOR col, const char* pString, const int nPriority);			//生成処理

private:

	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR2 m_size;			//サイズ
								
	CLetter* m_pTop;			//最初の文字のアドレス
};

#endif