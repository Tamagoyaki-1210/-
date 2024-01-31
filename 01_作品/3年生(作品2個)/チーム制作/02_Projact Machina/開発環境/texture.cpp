//**********************************************************************************************
// 
// texture.cpp
// Author  : tanimoto kosuke
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

	"Data/texture/Font/Font_Japanese002.png",		// 日本語フォント
	"Data/texture/Font/Font_Japanese003.png",		// 日本語フォント
	"Data/texture/Font/Font_Alphabet.png",			// アルファベットフォント
	"Data/texture/number.png",						// ナンバーのテクスチャ
	"Data/texture/particle/effect001.png",			// 弾のテクスチャ
	"Data/texture/Concrete.png",					// 地面のテクスチャ
	"Data/texture/map.png",							// マップのテクスチャ
	"Data/texture/reticle.png",						// レティクルのテクスチャ
	"Data/texture/circle.png",						// 円のテクスチャ
	"Data/texture/ItemPointer.png",					// アイテムポインターのテクスチャ
	"Data/texture/UI_00.png",						// UIのテクスチャ
	"Data/texture/Ekey.png",						// Eキーのテクスチャ

	/* タイトルロゴ */
	"Data/texture/Title/logo/title_00.png",
	"Data/texture/Title/logo/title_01.png",
	"Data/texture/Title/logo/title_02.png",
	"Data/texture/Title/logo/title_03.png",
	"Data/texture/Title/logo/title_04.png",
	"Data/texture/Title/logo/title_05.png",
	"Data/texture/Title/logo/title_06.png",
	"Data/texture/Title/logo/title_07.png",
	"Data/texture/Title/logo/title_08.png",
	"Data/texture/Title/logo/title_robot.png",

	/* タイトル歯車　*/
	"Data/texture/Title/gear/gear_00.png",
	"Data/texture/Title/gear/gear_01.png",
	"Data/texture/Title/gear/gear_02.png",
	"Data/texture/Title/gear/gear_03.png",
	"Data/texture/Title/gear/gear_04.png",
	"Data/texture/Title/gear/gear_05.png",
	"Data/texture/Title/gear/gear_06.png",
	
	/* エフェクト群 */
	"Data/texture/particle/smoke.png",				// 煙
	"Data/texture/particle/flare.png",				// エフェクト1
	"Data/texture/particle/star.png",				// エフェクト2
	"Data/texture/particle/bright.png",				// キラキラA
	"Data/texture/particle/bright2.png",			// キラキラB
	"Data/texture/particle/ray.png",				// レイ
	"Data/texture/particle/effect001.png",			// コイン
	"Data/texture/particle/effect003.png",			// playerの炎
	"Data/texture/particle/soul.png",				// 魂
	"Data/texture/particle/up.png",					// 上矢印
	"Data/texture/reticle.png",						// レティクル
	"Data/texture/chain.png",						// チェーン
	"Data/texture/NotEnter.png",					// 移動制限

	"Data/texture/Weapon/AR_icon.png",				// アサルトライフルアイコン
	"Data/texture/Weapon/SMG_icon.png",				// サブマシンガンアイコン
	"Data/texture/Weapon/SHandgun_icon.png",		// ハンドガンアイコン
	"Data/texture/Weapon/MG_icon.png",				// マシンガンアイコン
	"Data/texture/Weapon/SG_icon.png",				// ショットガンアイコン
	"Data/texture/Weapon/SR_icon.png",				// スナイパーライフルアイコン
	"Data/texture/Weapon/Knuckle_icon.png",			// 素手アイコン
	"Data/texture/Weapon/Slash_icon.png",			// スラッシュアイコン
	"Data/texture/Weapon/stabbing_icon.png",		// 刺突アイコン

	"Data/texture/WeaponSkill/WeaponSkill_AR.png",		// アサルトライフルのアタックスキルアイコン
	"Data/texture/WeaponSkill/WeaponSkill_SMG.png",		// サブマシンガンのアタックスキルアイコン
	"Data/texture/WeaponSkill/WeaponSkill_SHandgun.png",	// ハンドガンのアタックスキルアイコン
	"Data/texture/WeaponSkill/WeaponSkill_MG.png",		// マシンガンのアタックスキルアイコン
	"Data/texture/WeaponSkill/WeaponSkill_SG.png",		// ショットガンのアタックスキルアイコン
	"Data/texture/WeaponSkill/WeaponSkill_SR.png",		// スナイパーライフルのアタックスキルアイコン
	"Data/texture/WeaponSkill/WeaponSkill_Knuckle.png",	// 素手のアタックスキルアイコン
	"Data/texture/WeaponSkill/WeaponSkill_Slash.png",	// スラッシュのアタックスキルアイコン
	"Data/texture/WeaponSkill/WeaponSkill_stabbing.png",// 刺突のアタックスキルアイコン

	"Data/texture/job/job_Rush.png",				// ラッシュ
	"Data/texture/job/job_Vanguard.png",			// ヴァンガード
	"Data/texture/job/job_Eagleeye.png",			// イーグルアイ
	"Data/texture/job/job_Marshall.png",			// マーシャル

	"Data/texture/job/status_rush.png",				// ラッシュ
	"Data/texture/job/status_vanguard.png",			// ヴァンガード
	"Data/texture/job/status_eagleeye.png",			// イーグルアイ
	"Data/texture/job/status_standard.png",			// マーシャル

	"Data/texture/TEX001.png",						// 機体選択
	"Data/texture/TEX002.png",						// 最短撃破
	"Data/texture/TEX003.png",						// 耐久ダメージ
	"Data/texture/TEX004.png",						// 撃破数

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

	return s_pTexture[inTexture];
}
