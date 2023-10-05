//==================================================
//
// FPD制作 ( menu.h )
// Author  : katsuki mizuki
//
//==================================================
#ifndef _MENU_H_		//このマクロ定義がされてなかったら
#define _MENU_H_		//２重インクルード防止のマクロ定義

//--------------------------------------------------
// マクロ定義
//--------------------------------------------------
#define MAX_OPTION		(16)		// 選択肢の最大数
#define NO_SELECT_COLOR		(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))		//選択されていない時の色

//--------------------------------------------------
// 構造体
//--------------------------------------------------

/*↓ 選択肢 ↓*/

typedef struct
{
	D3DXVECTOR3				pos;			// 位置
	D3DXCOLOR				col;			// 色
	LPDIRECT3DTEXTURE9		pTexture;		// テクスチャ
	float					fWidth;			// 幅
	float					fHeight;		// 高さ
}Option;

/*↓ メニュー ↓*/

typedef struct
{
	D3DXVECTOR3				pos;					// 位置
	LPDIRECT3DTEXTURE9		pTexture;				// テクスチャ
	Option					Option[MAX_OPTION];		// 選択肢の情報
	int						nNumUse;				// 使用数
	float					fWidth;					// 幅
	float					fHeight;				// 高さ
	float					fInterval;				// 選択肢の間隔
	float					fBlinkSpeed;			// 点滅速度
	bool					bFrame;					// 枠がいるかどうか [ true : いる false : いらない ]
	bool					bUse;					// 使用しているかどうか
}Menu;

/*↓ メニューの引数 ↓*/

typedef struct
{
	LPDIRECT3DTEXTURE9		pTexture[MAX_OPTION];		// テクスチャ
	int						nNumUse;					// 使用数
	float					fLeft;						// 左端
	float					fRight;						// 右端
	float					fTop;						// 上端
	float					fBottom;					// 下端
	float					fWidth;						// 選択肢の幅
	float					fHeight;					// 選択肢の高さ
	bool					bSort;						// 並べ方 [ true : 縦 false : 横 ]
}MenuArgument;

/*↓ 枠の引数 ↓*/

typedef struct
{
	LPDIRECT3DTEXTURE9		pTexture;		// テクスチャ
	D3DXCOLOR				col;			// 色
	bool					bUse;			// 枠がいるかどうか [ true : いる false : いらない ]
}FrameArgument;

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
void InitMenu(void);
void UninitMenu(void);
void UpdateMenu(void);
void DrawMenu(void);
int SetMenu(const MenuArgument &menu, const FrameArgument &Frame);
void InitColorOption(void);
void ChangeOption(int nIdx);
void DecisionOption(void);
void ResetMenu(int nIdx);
Menu *GetMenu(void);

#endif // !_MENU_H_
