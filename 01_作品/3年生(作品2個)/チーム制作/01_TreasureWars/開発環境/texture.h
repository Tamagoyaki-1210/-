//**********************************************************************************************
// 
// texture.h
// Author  : katsuki mizuki
// 
//**********************************************************************************************
#ifndef _TEXTURE_H_	//このマクロ定義がされてなかったら
#define _TEXTURE_H_	//２重インクルード防止のマクロ定義

//==============================================================================================
// インクルード
//==============================================================================================
#include <d3dx9.h>

//==============================================================================================
// 定義
//==============================================================================================
class CTexture
{
public: /* 定義 */
	enum TEXTURE
	{	
		TEXTURE_TITLE = 0,			// タイトル
		TEXTURE_GAME,				// ゲーム(仮)
		TEXTURE_RESULT,				// リザルト(仮)
		TEXTURE_NUMBER,				// 数字
		TEXTURE_START,				// スタート
		TEXTURE_TIMEUP,				// タイムアップ
		TEXTURE_SKYBOX,
		TEXTURE_PAPER,				// 領収書
		TEXTURE_FONT_JAPANESE,		// 日本語フォント
		TEXTURE_FONT_ALPHABET,		// アルファベットフォント
		TEXTURE_EFFECT_SMOKE,
		TEXTURE_EFFECT_FLARE,
		TEXTURE_EFFECT_STAR,
		TEXTURE_EFFECT_BRIGHTA,
		TEXTURE_EFFECT_BRIGHTB,
		TEXTURE_EFFECT_RAY,
		TEXTURE_EFFECT_COIN,
		TEXTURE_EFFECT_JEWEL,
		TEXTURE_EFFECT_SOUL,
		TEXTURE_EFFECT_UP,
		TEXTURE_BILL_SNOWSTORM,
		TEXTURE_AREA,				//エリア用テクスチャ
		TEXTURE_CHARA_TYPE,			// キャラクタータイプ
		TEXTURE_SHADOW,				// オブジェクト3D用の影
		TEXTURE_SALE,				// 売却中UX
		TEXTURE_STAGE_1,			// ステージ1
		TEXTURE_STAGE_2,			// ステージ2
		TEXTURE_STAGE_3,			// ステージ3
		TEXTURE_STAGE_RADOM,		// ステージランダム
		TEXTURE_PLAYER_1P,
		TEXTURE_PLAYER_2P,
		TEXTURE_PLAYER_3P,
		TEXTURE_PLAYER_4P,
		TEXTURE_PLAYER_CPU,
		TEXTURE_WINDOW_OUTSIDE,
		TEXTURE_TUTORIAL_ALLOW,		// 選択肢用の矢印
		TEXTURE_BUTTON_A,			// Aボタンを押してね
		TEXTURE_BUTTON_SPACE,		// SPACEキーを押してね
		TEXTURE_TUTORIAL_01,		// チュートリアル画像
		TEXTURE_TUTORIAL_02,		// チュートリアル画像
		TEXTURE_TUTORIAL_03,		// チュートリアル画像
		TEXTURE_TUTORIAL_04,		// チュートリアル画像
		TEXTURE_SHOP_DIRECTION,		// 質屋の方向
		TEXTURE_DALLER,				// ドルマーク
		TEXTURE_FLOOR,
		TEXTURE_JUNGLE,				// ジャングル
		TEXTURE_MOON,				// 月面
		TEXTURE_RANDOM_FLOAR,		// ランダム背景
		TEXTURE_GALAXY,				// 銀河
		TEXTURE_RANK,				// 順位
		TEXTURE_DROP_SCORE_ITEM_NONE,				// スコアアイテムのデフォルト
		TEXTURE_DROP_SCORE_ITEM_DIAMOND,			// ダイヤモンド
		TEXTURE_DROP_SCORE_ITEM_RUBY,				// ルビー
		TEXTURE_DROP_SCORE_ITEM_SAPPHIRE,			// サファイヤ
		TEXTURE_DROP_SCORE_ITEM_EMERALD,			// エメラルド
		TEXTURE_DROP_SCORE_ITEM_TOPAZ,				// トパーズ
		TEXTURE_DROP_SCORE_ITEM_HEAD,				// 頭の化石
		TEXTURE_DROP_SCORE_ITEM_AMBER,				// 琥珀
		TEXTURE_DROP_SCORE_ITEM_BONE,				// 骨
		TEXTURE_DROP_SCORE_ITEM_FOOTPRINTS,			// 足跡の化石
		TEXTURE_DROP_SCORE_ITEM_AMMONITE,			// アンモナイト
		TEXTURE_DROP_SCORE_ITEM_CPU,				// CPU
		TEXTURE_DROP_SCORE_ITEM_SOLARPANEL,			// ソーラーパネル
		TEXTURE_DROP_SCORE_ITEM_ANTENNA,			// アンテナ
		TEXTURE_DROP_SCORE_ITEM_ENGINE,				// エンジン
		TEXTURE_DROP_SCORE_ITEM_OXYGENTANK,			// 酸素タンク
		TEXTURE_MAX,
		TEXTURE_NONE
	};

	static const char* s_FileName[];	// ファイルパス

public:
	CTexture();		// デフォルトコンストラクタ
	~CTexture();	// デストラクタ

public: /* メンバ関数 */
	void LoadAll();										// 全ての読み込み
	//void Load(TEXTURE inTexture);						// 指定の読み込み
	void ReleaseAll();									// 全ての破棄
	void Release(TEXTURE inTexture);					// 指定の破棄
	LPDIRECT3DTEXTURE9 GetTexture(TEXTURE inTexture);	// 情報の取得

private: /* メンバ変数 */
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];			// テクスチャの情報
};

#endif // !_TEXTURE_H_
