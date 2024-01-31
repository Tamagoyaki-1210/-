//==============================================
//
// player_ui.h
// Author : Saito Shian
//
//==============================================
#ifndef _PLAYER_UI_H
#define _PLAYER_UI_H

//==============================================
// インクルード
//==============================================
#include "main.h"
#include "object2D.h"

//==============================================
// 前方宣言
//==============================================

//==============================================
// プレイヤーUIクラス
//==============================================
class CPlayerUi : public CObject2D
{
public:
	const int ALPHA_COUNT = 600;
	const int COLOR_COUNT = 10;
	const int USE_COUNT = 100;

	enum UITYPE
	{
		UITYPE_SUPPORT = 0,		// サポートスキル
		UITYPE_ATTACK,			// アタックスキル
		UITYPE_WEAPON,			// 武器情報
		UITYPE_MAX
	};
	CPlayerUi(const PRIORITY priority);
	~CPlayerUi() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetAlpha();
	void SetColor();
	void SetData(float posX, float posY, float sizeX, float sizeY, CTexture::TEXTURE texture);

	UITYPE GetType() { return m_Type; }
	void SetType(UITYPE type) { m_Type = type; }

	CObject2D* GetSkillUI() { return m_pSkill_UI; }	// スキル画像のUIの取得

	static CPlayerUi* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, UITYPE type, const PRIORITY priority);

private:
	CObject2D* m_pSkill_UI;		// スキル画像のUI
	UITYPE m_Type;				// スキルの種類

	int m_AlphaCount;			// 半透明の時間
	int m_ColorCount;			// 色が変わる時間
	int m_UseCount;				// 点滅時間

	float m_Alpha;				// 透明度の値
	float m_Color;				// 色の値			

	bool m_bAlphaFlag;			// 半透明になったか
	bool m_bColorFlag;			// 色が変わったか
	bool m_bFlag;				// スキルが使用されたか
};

#endif // !_PLAYER_UI_H
