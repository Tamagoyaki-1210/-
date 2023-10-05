//=============================================================================
//
// ui.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include <vector>
#include "object2D.h"

//---------------------------
// クラス宣言
//---------------------------
class CUi : public CObject
{
public:
	CUi();
	virtual ~CUi() override;

	virtual HRESULT Init() override = 0;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override = 0;

	void DeathObj2D(CObject2D* obj2D);	// UI用オブジェクト2Dの削除処理
	//void Dispray();						// UIの表示処理

	void SetCompulsDispray(const bool display);														// 強制的なUI表示設定処理
	//void SetUiUpdate(const bool update) { m_bUiUpdate = update; }									// UIに更新があった事を伝える
	void SetNoneDispray(const bool dispray) { m_bNoneDispray = dispray; }							// 非表示の判定を設定
	void SetAlpha(const float alpha) { for (auto pObj2D : m_listObj2D) pObj2D->SetAlpha(alpha); }	// リスト内のUI用オブジェクト2Dのアルファ値を設定
	void SetCol(const float r, const float g, const float b, const float a) { for (auto pObj2D : m_listObj2D) pObj2D->SetCol(D3DXCOLOR(r, g, b, a)); }
	void SetUiDraw(const bool draw) { for (auto pObj2D : m_listObj2D) pObj2D->SetDrawFlag(draw); }	// リスト内のUI用オブジェクト2Dの描画判定を設定
	void SetUiUV() { for (auto pObj2D : m_listObj2D) pObj2D->UV(); }								// リスト内のUI用オブジェクト2DのUVを更新する処理
	void AddObj2D(CObject2D* obj2D) { m_listObj2D.push_back(obj2D); }								// UI用オブジェクト2Dに追加する処理

	static std::vector <CUi*> GetAllUi() { return m_AllUi; }										// 静的な全てのUIポインタの取得処理
private:

	//static const int Invinsible_Dispray_Max = 120;	// 最大非表示透過カウンター
	//static const int None_Dispray_Max = 160;		// 最大非表示カウンター

	//int m_nNoneDispray_Counter;						// UI非表示カウンター
	//bool m_bUiUpdate;								// UIの更新判定
	bool m_bNoneDispray;							// UI非表示設定処理

	std::vector <CObject2D*> m_listObj2D;			// UI用オブジェクト2D
	static std::vector <CUi*> m_AllUi;				// 静的な全てのUIのポインタ
};

#endif // !_UI_H_