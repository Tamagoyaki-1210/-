//**********************************************************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// 
//**********************************************************************************************

//==============================================================================================
// インクルード
//==============================================================================================
#include "application.h"
#include "texture.h"
#include "renderer.h"

#include <assert.h>

//==============================================================================================
// 定義
//==============================================================================================
const char* CTexture::s_FileName[] =
{// テクスチャのパス
	"Data/texture/title.png",						// タイトル(仮)
	"Data/texture/game.jpg",						// ゲーム(仮)
	"Data/texture/result.jpg",						// リザルト(仮)
	"Data/texture/number.png",						// 数字
	"Data/texture/start.png",						// スタート
	"Data/texture/timeup.png",						// タイムアップ
	"Data/texture/sky000.jpg",						// スカイボックス
	"Data/texture/paper_00104.jpg",					// 領収書
	"Data/texture/Font/Japanese2000unicode.png",	// 日本語フォント
	"Data/texture/Font/Alphabet.png",				// アルファベットフォントフォント
	"Data/texture/particle/smoke.png",				// 煙
	"Data/texture/particle/flare.png",				// エフェクト1
	"Data/texture/particle/star.png",				// エフェクト2
	"Data/texture/particle/bright.png",				// キラキラA
	"Data/texture/particle/bright2.png",			// キラキラB
	"Data/texture/particle/ray.png",				// レイ
	"Data/texture/particle/coin.png",				// コイン
	"Data/texture/particle/jewel.png",				// パーティクル用宝石
	"Data/texture/particle/soul.png",				// 魂
	"Data/texture/particle/up.png",					// 上矢印
	"Data/texture/blizzardoffallingbills.png",		// 札吹雪
	"Data/texture/Area.png",						// エリア
	"Data/texture/PlayerNumber.png",				// キャラクタータイプ
	"Data/texture/shadow000.jpg",					// オブジェクト3D用の影
	"Data/texture/sale.png",						// 売却中UX
	"Data/texture/stage_1.png",						// ステージ1
	"Data/texture/stage_2.png",						// ステージ2
	"Data/texture/stage_3.png",						// ステージ3
	"Data/texture/stage_random.png",				// ステージランダム
	"Data/texture/1P.png",
	"Data/texture/2P.png",
	"Data/texture/3P.png",
	"Data/texture/4P.png",
	"Data/texture/CPU.png",
	"Data/texture/窓の外.png",
	"Data/texture/select_arrow.png",				// 選択肢用の矢印
	"Data/texture/button_A.png",					// Aボタンを押してね
	"Data/texture/button_SPACE.png",				// SPACEキーを押してね
	"Data/texture/tutorial_01.png",					// チュートリアル画像_01
	"Data/texture/tutorial_02.png",					// チュートリアル画像_02
	"Data/texture/tutorial_03.png",					// チュートリアル画像_03
	"Data/texture/tutorial_04.png",					// チュートリアル画像_04
	"Data/texture/shop_direction.png",
	"Data/texture/daller.png",
	"Data/texture/floor.png",						// 床
	"Data/texture/Jungle.png",
	"Data/texture/Moon.png",
	"Data/texture/random_Floar.png",
	"Data/texture/Galaxy.png",
	"Data/texture/順位.png",						// 順位
	"Data/texture/Drop_Score_Item/scoreitem_none.png",		// スコアアイテムのデフォルト
	"Data/texture/Drop_Score_Item/Stage_01/Diamond.png",	// ダイヤモンド
	"Data/texture/Drop_Score_Item/Stage_01/Ruby.png",		// ルビー
	"Data/texture/Drop_Score_Item/Stage_01/Sapphire.png",	// サファイヤ
	"Data/texture/Drop_Score_Item/Stage_01/Emerald.png",	// エメラルド
	"Data/texture/Drop_Score_Item/Stage_01/Topaz.png",		// トパーズ
	"Data/texture/Drop_Score_Item/Stage_02/Head.png",		// 頭の化石
	"Data/texture/Drop_Score_Item/Stage_02/Amber.png",		// 琥珀
	"Data/texture/Drop_Score_Item/Stage_02/Bone.png",		// 骨
	"Data/texture/Drop_Score_Item/Stage_02/Footprints.png",	// 足跡の化石
	"Data/texture/Drop_Score_Item/Stage_02/Ammonite.png",	// アンモナイト
	"Data/texture/Drop_Score_Item/Stage_03/Cpu.png",		// CPU
	"Data/texture/Drop_Score_Item/Stage_03/Solarpanel.png",	// ソーラーパネル
	"Data/texture/Drop_Score_Item/Stage_03/Antenna.png",	// アンテナ
	"Data/texture/Drop_Score_Item/Stage_03/Engine.png",		// エンジン
	"Data/texture/Drop_Score_Item/Stage_03/OxygenTank.png",	// 酸素タンク
};

static_assert(sizeof(CTexture::s_FileName) / sizeof(CTexture::s_FileName[0]) == CTexture::TEXTURE_MAX, "aho");

//----------------------------------------------------------------------------------------------
// デフォルトコンストラクタ
//----------------------------------------------------------------------------------------------
CTexture::CTexture() :
	s_pTexture()
{
	memset(s_pTexture, 0, sizeof(s_pTexture));
}

//----------------------------------------------------------------------------------------------
// デストラクタ
//----------------------------------------------------------------------------------------------
CTexture::~CTexture()
{
}

//----------------------------------------------------------------------------------------------
// 全ての読み込み
//----------------------------------------------------------------------------------------------
void CTexture::LoadAll()
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != nullptr)
		{// テクスチャの読み込みがされている
			continue;
		}

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			s_FileName[i],
			&s_pTexture[i]);
	}
}

////----------------------------------------------------------------------------------------------
//// 読み込み
////----------------------------------------------------------------------------------------------
//void CTexture::Load(TEXTURE inTexture)
//{
//	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);
//
//	if (s_pTexture[inTexture] != nullptr)
//	{// テクスチャの読み込みがされている
//		return;
//	}
//
//	// デバイスへのポインタの取得
//	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
//
//	// テクスチャの読み込み
//	D3DXCreateTextureFromFile(pDevice,
//		s_FileName[inTexture],
//		&s_pTexture[inTexture]);
//}

//----------------------------------------------------------------------------------------------
// 全ての解放
//----------------------------------------------------------------------------------------------
void CTexture::ReleaseAll(void)
{
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != NULL)
		{// テクスチャの解放
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}
}

//----------------------------------------------------------------------------------------------
// 解放
//----------------------------------------------------------------------------------------------
void CTexture::Release(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != NULL)
	{// テクスチャの解放
		s_pTexture[inTexture]->Release();
		s_pTexture[inTexture] = NULL;
	}
}

//----------------------------------------------------------------------------------------------
// 取得
//----------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE inTexture)
{
	if (inTexture == TEXTURE_NONE)
	{// テクスチャを使用しない
		return nullptr;
	}

	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	// 読み込み
	//Load(inTexture);

	return s_pTexture[inTexture];
}
