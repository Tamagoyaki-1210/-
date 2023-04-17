//=============================================================================
//
// 画像
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

//---------------------------
//クラス
//---------------------------
class CTexture
{
public:
	enum Texture_Type
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,		//プレイヤー
		TYPE_ENEMY_01,		//敵キャラ1
		TYPE_ENEMY_02,		//敵キャラ2
		TYPE_ENEMY_03,		//敵キャラ3
		TYPE_ENEMY_04,		//敵キャラ4
		TYPE_OBSTACLE_01,	//障害物1
		TYPE_OBSTACLE_02,	//障害物2
		TYPE_BULLET_01,		//弾1
		TYPE_BULLET_02,		//弾2
		TYPE_RAY_01,		//光線エフェクト1
		TYPE_EXPLOSION,		//爆発
		TYPE_ITEM,			//アイテム
		TYPE_BG,			//背景
		TYPE_BG_Tutorial01,	//背景(チュートリアル01)
		TYPE_BG_Tutorial02,	//背景(チュートリアル02)
		TYPE_BG_Tutorial03,	//背景(チュートリアル03)
		TYPE_SCORE,			//スコア
		TYPE_RANKING,		//ランキング
		TYPE_THISNUMBER,	//今回のスコア
		TYPE_TITLE,			//タイトルロゴ
		TYPE_PAUSE,			//ポーズ画面
		TYPE_COMMA,			//コンマ
		TYPE_NUMBER,		//ナンバー
		TYPE_LIFE,			//体力
		TYPE_GAUGE,			//ゲージ
		TYPE_CHOICE_01,		//選択肢1
		TYPE_CHOICE_02,		//選択肢2
		TYPE_CHOICE_03,		//選択肢3
		TYPE_CHOICE_04,		//選択肢4
		TYPE_CHOICE_05,		//選択肢5
		TYPE_MAX
	};

	CTexture();
	~CTexture();

	void Uninit();
	void Load();
	void TextureType(Texture_Type textype);
private:
	LPDIRECT3DTEXTURE9 m_pTextureType[TYPE_MAX];		//Texture種別
};

#endif // !_TEXTURE_H_
