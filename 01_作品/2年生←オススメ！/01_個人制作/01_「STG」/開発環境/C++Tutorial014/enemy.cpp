//=============================================================================
//
// 敵キャラ
// Author : tanimoto kosuke
//
//=============================================================================
#include "enemy.h"
#include <assert.h>
#include <stdio.h>
#include "matrix.h"
#include "bullet.h"
#include "explosion.h"
#include "application.h"
#include "texture.h"
#include "score.h"
#include "item.h"
#include "player.h"
#include "stage.h"

//---------------------------
//マクロ関数
//---------------------------
#define ENEMY_BLINK_SPEED		(0.5f)
#define ENEMY_MAX_BLINK			(4)
#define ENEMY_FILE				"data/.txt/enemy.txt"		//読み込みファイル

//=====================================
//デフォルトコンストラクタ
//=====================================
CEnemy::CEnemy() : CObject2D(OBJECT_PRIORITY_CENTER)
{

}

//=====================================
//デストラクタ
//=====================================
CEnemy::~CEnemy()
{

}

//=====================================
//初期化処理
//=====================================
void CEnemy::Init()
{
	CObject2D::Init();
}

//=====================================
//終了処理
//=====================================
void CEnemy::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//更新処理
//=====================================
void CEnemy::Update()
{
	CObject2D::Update();

	//UV設定
	CObject2D::SetUV();

	if (m_situ != SITU_NONEANIM)
	{
		CObject2D::SetAnim(m_nAnimX, m_nAnimY, 6);
	}

	//位置取得
	D3DXVECTOR3 pos = CObject2D::GetPos();

	SituationType();

	//位置更新
	pos += m_move * m_fSpeed;

	//位置設定処理
	SetPos(pos);

	OffScreen(pos);

	//種別毎のAI
	TypeAI(pos);
}

//=====================================
//描画処理
//=====================================
void CEnemy::Draw()
{
	//画像取得処理
	switch (m_type)
	{
	case TYPE_01:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_ENEMY_01);
		break;
	case TYPE_02:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_ENEMY_02);
		break;
	case TYPE_03:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_ENEMY_03);
		break;
	case TYPE_04:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_ENEMY_04);
		break;
	default:
		break;
	}
	CObject2D::Draw();
}

//=====================================
//種別毎のAI処理
//=====================================
void CEnemy::TypeAI(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer = CPlayer::GetPlayer();							//プレイヤーポインタ
	if (pPlayer->GetDeath() == false)
	{
		D3DXVECTOR3 pPos = pPlayer->GetPos();							//プレイヤー位置
		D3DXVECTOR3 subPos = pPos - pos;								//位置差分
		D3DXVECTOR3 nNomalizeSub = NormalizeLength(subPos.x, subPos.y);	//位置差分正規化

		switch (m_AI)
		{
			//通常
		case CEnemy::AI_NONE:
			m_move.x = -1.0f;
			switch (m_type)
			{
			case TYPE_01:
				if (SCREEN_WIDTH / 2 + 200.0f > pos.x)
				{
					//画面半分より上の場合
					if (SCREEN_HEIGHT / 2 < pos.y)
					{
						m_AI = AI_01;
					}
					//画面半分より下の場合
					else if (SCREEN_HEIGHT / 2 >= pos.y)
					{
						m_AI = AI_02;
					}
				}
				break;
			case TYPE_02:
				if (subPos.x > -100.0f)
				{
					m_AI = AI_01;
				}
				break;
			case TYPE_03:
				break;
			case TYPE_04:
				m_situ = SITU_NONEANIM;
				if (pos.y >= F_SCREEN_HEIGHT / 2)
				{
					m_AI = AI_01;
				}
				else
				{
					SetRot(D3DXVECTOR3(D3DX_PI, D3DX_PI, 0.0f));
					m_AI = AI_02;
				}
				break;
			default:
				break;
			}
			break;
			//AIその1
		case CEnemy::AI_01:
			switch (m_type)
			{
			case TYPE_01:
				m_move = NormalizeLength(1.0f, -1.0f);

				//画面上まで来た場合
				if (200.0f > pos.y)
				{
					m_AI = AI_03;
				}

				break;
			case TYPE_02:
				m_move = nNomalizeSub;	//プレイヤーをロックオン
				m_fSpeed -= 3.9f;

				m_AI = AI_02;
				break;
			case TYPE_03:
				m_move = D3DXVECTOR3(0.1f, 1.0f, 0.0f);

				//点滅処理
				if (m_bBlink)
				{
					m_fBlinkCounter -= 0.5f;
				}
				else
				{
					m_fBlinkCounter += 0.5f;
				}

				if (m_fBlinkCounter >= 1.0f / ENEMY_BLINK_SPEED)
				{
					m_bBlink = true;
					m_fBlinkCounter = 1.0f / ENEMY_BLINK_SPEED;
					m_nBlinkLoop++;
				}
				else if (m_fBlinkCounter <= 0.0f)
				{
					m_bBlink = false;
					m_fBlinkCounter = 0.0f;
				}

				//点滅させる部分の色を計算
				SetColor(D3DXCOLOR(1.0f, 0.6f, 0.6f, m_fBlinkCounter * ENEMY_BLINK_SPEED));

				if (m_nBlinkLoop >= 10)
				{
					m_nBlinkLoop = 0;
					for (int nCnt = 0; nCnt < 18; nCnt++)
					{
						CBullet::Create(pos, D3DXVECTOR3(sinf((float)D3DX_PI / 9 * nCnt), cosf((float)D3DX_PI / 9 * nCnt), 0.0f), CBullet::ENEMY_NORMAL);	//弾生成処理
					}
					Destroy(pos);
					m_AI = AI_NONE;
				}
				break;
			case TYPE_04:
				if (subPos.x <= 0.0f && subPos.y <= m_fHeight / 2)
				{
					SetAnim(2, 1, 6);
					//追尾弾
					if (m_nBulletCounter > m_nBulletTimer)
					{
						m_nBulletCounter = 0;
						CBullet::Create(pos + (m_fWidth / 2 * nNomalizeSub), D3DXVECTOR3(nNomalizeSub), CBullet::ENEMY_SPREAD);	//弾生成処理
					}
					else
					{
						m_nBulletCounter++;
					}
				}
				break;
			default:
				break;
			}
			break;
			//AIその2
		case CEnemy::AI_02:
			switch (m_type)
			{
			case TYPE_01:
				m_move = NormalizeLength(1.0f, 1.0f);

				//画面下まで来た場合
				if (SCREEN_HEIGHT - 200.0f < pos.y)
				{
					m_AI = AI_03;
				}
				break;
			case TYPE_02:
				m_fSpeed += 0.15f;
				break;
			case TYPE_04:
				if (subPos.x >= 0.0f && subPos.y >= -m_fHeight / 2)
				{
					SetAnim(2, 1, 6);
					//追尾弾
					if (m_nBulletCounter > m_nBulletTimer)
					{
						m_nBulletCounter = 0;
						CBullet::Create(pos + (m_fWidth / 2 * nNomalizeSub), D3DXVECTOR3(nNomalizeSub), CBullet::ENEMY_SPREAD);	//弾生成処理
					}
					else
					{
						m_nBulletCounter++;
					}
				}
				break;
			default:
				break;
			}
			break;
			//AIその3
		case CEnemy::AI_03:
			switch (m_type)
			{
			case TYPE_01:
				m_move.x = -1.0f;
				m_move.y = 0.0f;
				break;
			default:
				break;
			}
			break;
		case CEnemy::AI_MAX:
			m_move.x = -1.0f;
			break;
		default:
			break;
		}

		//常に働く個別の処理
		switch (m_type)
		{
		case TYPE_01:
			if (m_AI == AI_01 || m_AI == AI_02 || m_AI == AI_03)
			{
				//追尾弾
				if (m_nBulletCounter > m_nBulletTimer)
				{
					m_nBulletCounter = 0;
					CBullet::Create(pos, D3DXVECTOR3(nNomalizeSub), CBullet::ENEMY_NORMAL);	//弾生成処理
				}
				else
				{
					m_nBulletCounter++;
				}
			}
			break;
		case TYPE_02:
			if (m_AI == AI_02)
			{
				//追尾弾
				if (m_nBulletCounter > m_nBulletTimer)
				{
					m_nBulletCounter = 0;
					CBullet::Create(pos, D3DXVECTOR3(nNomalizeSub), CBullet::ENEMY_NORMAL);	//弾生成処理
				}
				else
				{
					m_nBulletCounter++;
				}
			}
			break;
		default:
			break;
		}
	}
}

//=====================================
//被弾処理
//=====================================
void CEnemy::Hit(int nDamage)
{
	m_nLife -= nDamage;
	if (m_nLife <= 0)
	{
		m_nLife = 0;
		Destroy(CObject2D::GetPos());
	}
	else
	{
		if (m_type == TYPE_03 && m_AI != AI_01)
		{
			m_situ = SITU_NONEANIM;
			m_AI = AI_01;
			SetRot(D3DXVECTOR3(-D3DX_PI / 4, -D3DX_PI / 4, 0.0f));
		}
	}
}
//=====================================
//設定処理
//=====================================
void CEnemy::SetType()
{
	Load();

	//オブジェクト設定処理
	SetObject2D(CObject::TYPE_ENEMY, m_fWidth, m_fHeight);
}

//=====================================
//画面外処理
//=====================================
void CEnemy::OffScreen(D3DXVECTOR3 pos)
{
	if (pos.x <= 0.0f || pos.y + 200.0f <= 0.0f || pos.y - 200.0f >= SCREEN_HEIGHT)
	{
		m_AI = AI_MAX;
	}
	if (pos.x + (SCREEN_WIDTH / 2) <= 0.0f || pos.x - (SCREEN_WIDTH / 2) >= SCREEN_WIDTH || pos.y + (SCREEN_HEIGHT / 2) <= 0.0f || pos.y - (SCREEN_HEIGHT / 2) >= SCREEN_HEIGHT)
	{
		Uninit();
	}
}

//=====================================
//消去処理
//=====================================
void CEnemy::Destroy(D3DXVECTOR3 pos)
{
	//爆発生成処理
	CExplosion::Create(pos, m_fWidth, m_fHeight);

	CScore *pScore = CScore::GetScore();

	switch (m_type)
	{
	case TYPE_01:
		if (rand() % 10 == 0)
		{
			//アイテム生成処理
			CItem::Create(pos, CItem::ENERGY_01);
		}
		pScore->AddScore(100);
		break;
	case TYPE_02:
		if (rand() % 6 == 0)
		{
			//アイテム生成処理
			CItem::Create(pos, CItem::ENERGY_02);
		}
		pScore->AddScore(200);
		break;
	case TYPE_03:
		if (rand() % 7 == 0)
		{
			//アイテム生成処理
			CItem::Create(pos, CItem::ENERGY_03);
		}
		pScore->AddScore(300);
		break;
	case TYPE_04:
		if (rand() % 5 == 0)
		{
			//アイテム生成処理
			CItem::Create(pos, CItem::ENERGY_04);
			pScore->AddScore(400);
		}
		break;
	default:
		break;
	}
	Uninit();
}

//=====================================
//点滅色処理
//=====================================
void CEnemy::BlinkColor()
{
	if (m_bBlink)
	{
		m_fBlinkCounter--;
	}
	else
	{
		m_fBlinkCounter++;
	}

	if (m_fBlinkCounter >= 1.0f / ENEMY_BLINK_SPEED)
	{
		m_bBlink = true;
		m_fBlinkCounter = 1.0f / ENEMY_BLINK_SPEED;
		m_nBlinkLoop++;
	}
	else if (m_fBlinkCounter <= 0.0f)
	{
		m_bBlink = false;
		m_fBlinkCounter = 0.0f;
	}

	//点滅させる部分の色を計算
	D3DXCOLOR colBlink = D3DXCOLOR(1.0f, 0.6f, 0.6f, m_fBlinkCounter * ENEMY_BLINK_SPEED);

	SetColor(colBlink);

	if (m_nBlinkLoop >= ENEMY_MAX_BLINK)
	{
		m_nBlinkLoop = 0;
		m_situ = SITU_NONE;
		//色初期化
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//=====================================
//敵キャラ状態処理
//=====================================
void CEnemy::SituationType()
{
	switch (m_situ)
	{
	case CEnemy::SITU_NONE:
		break;
	case CEnemy::SITU_DAMAGE:
		BlinkColor();
		break;
	default:
		break;
	}
}

//=====================================
//生成処理
//=====================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, Enemy_Type type)
{
	CEnemy* pEnemy = new CEnemy;	//敵キャラポインタ

	if (pEnemy != nullptr)
	{
		pEnemy->Init();
		pEnemy->SetPos(pos);
		pEnemy->m_type = type;
		pEnemy->SetType();
	}
	return pEnemy;
}

//=====================================
//読み込み処理
//=====================================
void CEnemy::Load()
{
	char s_aString[256];		//読み込む用文字列
	int nNumType = 0;			//読み込んだタイプ数

								//ファイルを開く
	FILE* pFile = fopen(ENEMY_FILE, "r");

	if (pFile != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%s", s_aString);

		while (strncmp(s_aString, "SCRIPT", 6) != 0)
		{//スタートが来るまで空白を読み込む
			s_aString[0] = {};
			fscanf(pFile, "%s", s_aString);
		}

		while (strncmp(s_aString, "END_SCRIPT", 10) != 0)
		{//文字列の初期化と読み込み
			fscanf(pFile, "%s", s_aString);

			if (strncmp(s_aString, "#", 1) == 0)
			{//この後にコメント
				fgets(s_aString, sizeof(s_aString), pFile);
				continue;
			}

			if (strcmp(s_aString, "TYPESTATE") == 0)
			{//文字列が一致した場合

			 assert(TYPE_MAX != nNumType);	//想定より多いファイルの読み込み

				while (strncmp(s_aString, "ENDSTATE", 8) != 0)
				{
					fscanf(pFile, "%s", s_aString);		//=の読み込み

					if (strcmp(s_aString, "LIFE") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_nLife);
					}
					else if (strcmp(s_aString, "ANIM_X") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_nAnimX);
					}
					else if (strcmp(s_aString, "ANIM_Y") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_nAnimY);
					}
					else if (strcmp(s_aString, "BULLET_TIMER") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_nBulletTimer);
					}
					else if (strcmp(s_aString, "WIDTH") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%f", &m_fWidth);
					}
					else if (strcmp(s_aString, "HEIGHT") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%f", &m_fHeight);
					}
					else if (strcmp(s_aString, "SPEED") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%f", &m_fSpeed);
					}
				}
				if (m_type == nNumType)
				{//指定した種別の場合
					break;
				}
				nNumType++;
			}
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}