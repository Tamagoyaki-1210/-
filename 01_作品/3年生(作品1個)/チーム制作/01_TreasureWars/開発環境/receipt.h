//=============================================================================
//
// receipt.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _RECEIPT_H_
#define _RECEIPT_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include <vector>
#include <string>
#include "object.h"

//---------------------------
// クラス宣言
//---------------------------
class CFontString;

//---------------------------
// クラス宣言
//---------------------------
class CReceipt : public CObject
{
public:

	CReceipt();
	~CReceipt() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Total();								// レシートの合計値の処理
	void DestroyString();						// 文字列の破棄処理

	void SetString(const std::string letter);	// 文字列の設定
	void SetStringAnchor();						// 文字列のアンカーポイントの設定処理
	void SetTotalValue(const int value) { m_nTotalValue += value; }	// 合計値段

	static CReceipt* Create(const int index);

private:
	static const int Max_Row = 5;						// 領収書の最大行数
	static const int Max_Len = 10;						// 領収書の最大横文字数
	static const int String_Death_Timer = 30;			// 削除した時の最後の文字列が消えるまでの時間
	static const int Setting_String_Death_Timer = 300;	// 追加した時の最後の文字列が消えるまでの時間
	static const float FontString_Blink_Speed;			// 点滅速度
	static const D3DXVECTOR3 Font_Size;					// 文字列のサイズ

	D3DXVECTOR3 m_pos;								// 位置
	bool m_bRight;									// 配置が左右かの判定
	int m_nStringDesthCounter;						// 最後の文字列が消えるまでのカウンター
	int m_nTotalValue;								// 合計値段

	CFontString* m_pTotal;							// レシートの合計
	std::vector <CFontString*> m_apFontString;		// 文字列のポインタ
};

#endif// _RECEIPT_H_