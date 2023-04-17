//=============================================================================
//
// 画像
// Author : tanimoto kosuke
//
//=============================================================================
#include "texture.h"
#include "renderer.h"
#include "application.h"

//=====================================
//デフォルトコンストラクタ
//=====================================
CTexture::CTexture()
{

}

//=====================================
//デストラクタ
//=====================================
CTexture::~CTexture()
{

}

//=====================================
//終了処理
//=====================================
void CTexture::Uninit()
{
	for (int nInd = 0; nInd < TYPE_MAX; nInd++)
	{
		if (m_pTextureType[nInd] == nullptr)
		{
			continue;
		}
		//m_pTexture[nInd]->Release();
		m_pTextureType[nInd] = nullptr;
	}
}

//=====================================
//読み込み処理
//=====================================
void CTexture::Load()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//無し
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\BackGround\\white_screen.jpg", &m_pTextureType[TYPE_NONE]);

	//プレイヤー
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Player\\PlayerAnim.png", &m_pTextureType[TYPE_PLAYER]);

	//敵キャラ
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Enemy\\Enemy01Anim.png", &m_pTextureType[TYPE_ENEMY_01]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Enemy\\Enemy02Anim.png", &m_pTextureType[TYPE_ENEMY_02]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Enemy\\Enemy03Anim.png", &m_pTextureType[TYPE_ENEMY_03]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Enemy\\Enemy04Anim.png", &m_pTextureType[TYPE_ENEMY_04]);

	//障害物
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Obstacle\\Obstacle01.png", &m_pTextureType[TYPE_OBSTACLE_01]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Obstacle\\Obstacle02.png", &m_pTextureType[TYPE_OBSTACLE_02]);

	//弾
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Bullet\\Bullet01.png", &m_pTextureType[TYPE_BULLET_01]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Bullet\\white_screen.jpg", &m_pTextureType[TYPE_BULLET_02]);

	//光線エフェクト
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Ray\\effect105.png", &m_pTextureType[TYPE_RAY_01]);

	//爆発
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Explosion\\Explosion01.png", &m_pTextureType[TYPE_EXPLOSION]);

	//アイテム
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Item\\item001.png", &m_pTextureType[TYPE_ITEM]);

	//背景
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\BackGround\\white_screen.jpg", &m_pTextureType[TYPE_BG]);

	//背景(チュートリアル)
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\BackGround\\Tutorial01.png", &m_pTextureType[TYPE_BG_Tutorial01]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\BackGround\\Tutorial02.png", &m_pTextureType[TYPE_BG_Tutorial02]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\BackGround\\Tutorial03.png", &m_pTextureType[TYPE_BG_Tutorial03]);

	//スコア
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Score\\frame_score.png", &m_pTextureType[TYPE_SCORE]);

	//ランキング
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Ranking\\ranking.png", &m_pTextureType[TYPE_RANKING]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Message\\ThisNumber.png", &m_pTextureType[TYPE_THISNUMBER]);

	//タイトルロゴ
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Message\\Title.png", &m_pTextureType[TYPE_TITLE]);

	//ポーズ画面
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Message\\Pause.png", &m_pTextureType[TYPE_PAUSE]);

	//コンマ
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Number\\comma.png", &m_pTextureType[TYPE_COMMA]);

	//ナンバー
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Number\\number000.png", &m_pTextureType[TYPE_NUMBER]);

	//体力
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Life\\white_screen.jpg", &m_pTextureType[TYPE_LIFE]);

	//ゲージ
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Gauge\\white_screen.jpg", &m_pTextureType[TYPE_GAUGE]);

	//選択肢
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Choice\\Choice_TitleStart.png", &m_pTextureType[TYPE_CHOICE_01]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Choice\\Choice_TitleTutrial.png", &m_pTextureType[TYPE_CHOICE_02]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Choice\\Choice_TitleExit.png", &m_pTextureType[TYPE_CHOICE_03]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Choice\\Choice_GameContinue.png", &m_pTextureType[TYPE_CHOICE_04]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Choice\\Choice_GameExit.png", &m_pTextureType[TYPE_CHOICE_05]);
}

//=====================================
//画像種類処理
//=====================================
void CTexture::TextureType(Texture_Type textype)
{
	//テクスチャの設定
	CApplication::GetRenderer()->GetDevice()->SetTexture(0, m_pTextureType[textype]);
}