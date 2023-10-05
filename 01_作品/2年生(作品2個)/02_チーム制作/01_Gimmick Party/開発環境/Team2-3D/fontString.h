//=============================================================================
//
// fontString.h
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _FONTSTRING_H_
#define _FONTSTRING_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "font.h"

//---------------------------
//クラス宣言
//---------------------------
class CFontString
{
public:
	CFontString();
	~CFontString();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void SizeScale(void);			// 文字の拡大処理
	void SizeReset(void);			// 文字の大きさ初期化処理
	void SetSellect(void);			// 現在番号の設定処理
	void SetColor(D3DXCOLOR col);	// 色の設定処理

	static CFontString* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const std::string letter);
private:
	static const int MaxWordLenght = 24;	// 文字列の最大文字数

	int m_nNum;						// 文字数
	float m_fScale;					// 文字の大きさ
	bool m_bScale;					// 文字の拡大判定
	bool m_bSellect;				// 文字の選択判定
	D3DXVECTOR2 m_sizeYuan;			// 文字の元の大きさ
	CFont* m_pFont[MaxWordLenght];	// 文字のポインタ
};

#endif // !_FONTSTRING_H_
