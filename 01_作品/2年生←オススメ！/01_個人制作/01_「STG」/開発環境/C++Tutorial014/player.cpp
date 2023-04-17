//=============================================================================
//
// プレイヤー
// Author : tanimoto kosuke
//
//=============================================================================
#include "player.h"
#include <assert.h>
#include <stdio.h>
#include "matrix.h"
#include "input.h"
#include "bullet.h"
#include "application.h"
#include "texture.h"
#include "explosion.h"
#include "life.h"
#include "gauge.h"
#include "game.h"
#include "obstacle.h"
#include "score.h"

//---------------------------
//マクロ関数
//---------------------------
#define PLAYER_FINAL_LIFE		(200)	//最大HP
#define PLAYER_MAX_MOVE_SPEED	(3.0f)
#define PLAYER_BLINK_SPEED		(0.2f)
#define PLAYER_MAX_BLINK		(4)
#define PLAYER_MAX_BULLET		(5)		//弾同時所有数
#define PLAYER_FILE				"data/.txt/player.txt"		//読み込みファイル

CPlayer *CPlayer::m_pPlayer = nullptr;
//=====================================
//デフォルトコンストラクタ
//=====================================
CPlayer::CPlayer() : CObject2D(OBJECT_PRIORITY_CENTER)
{	

}

//=====================================
//デストラクタ
//=====================================
CPlayer::~CPlayer()
{

}

//=====================================
//初期化処理
//=====================================
void CPlayer::Init()
{
	CObject2D::Init();

	//外部ファイルで変数に値を代入する
	Load();

	m_nMaxLife = m_nLife;
	m_nFastMaxLife = m_nMaxLife;
	m_nBlinkLoop = 0;
	m_nBulletCounter = 0;
	m_nHoldBullet = 1;
	m_fBlinkCounter = 0.0f;
	m_bBlink = false;
	m_nDeathTimer = 0;

	//オブジェクト設定処理
	SetObject2D(CObject::TYPE_PLAYER, m_fWidth, m_fHeight);
}

//=====================================
//終了処理
//=====================================
void CPlayer::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//更新処理
//=====================================
void CPlayer::Update()
{
	if (m_situ != SITU_DEATH)
	{
		CObject2D::Update();

		//UV設定
		CObject2D::SetUV();

		CObject2D::SetAnim(m_nAnimX, m_nAnimY, 6);

		//位置取得
		D3DXVECTOR3 pos = CObject2D::GetPos();

		//前回位置の取得
		m_posOld = pos;

		//角度取得処理
		D3DXVECTOR3 rot = CObject2D::GetRot();

		//入力処理
		Input(pos);

		//減衰
		m_move -= m_move * 0.15f;

		//位置更新
		pos += m_move * m_fSpeed;

		//位置設定処理
		SetPos(pos);

		//角度取得処理
		CObject2D::SetRot(rot);

		OffScreen(pos);

		if (m_nBulletCounter > 0)
		{
			m_nBulletCounter--;
			if (m_nBulletCounter < 0)
			{
				m_nBulletCounter = 0;
			}
		}

		//当たり判定設定処理
		Collision();
	}
	SituationType();
}

//=====================================
//描画処理
//=====================================
void CPlayer::Draw()
{
	if (m_situ != SITU_DEATH)
	{
		CApplication::GetTexture()->TextureType(CTexture::TYPE_PLAYER);
		CObject2D::Draw();
	}
}

//=====================================
//被弾処理
//=====================================
void CPlayer::Hit(int nDamage)
{
	if (nDamage > 0)
	{//被弾時爆発生成
		CExplosion::Create(CObject2D::GetPos(), EXPLOSION_WIDTH, EXPLOSION_HEIGHT);
		m_nLife -= nDamage;
		m_pLife->SetLife(m_nLife, m_nMaxLife, m_nFastMaxLife);

		m_fBlinkCounter = 0.0f;
		m_situ = SITU_DAMAGE;
		if (m_nLife <= 0)
		{
			Destroy(CObject2D::GetPos());
		}
	}
}

//=====================================
//体力加算処理
//=====================================
void CPlayer::AddLife(int addLife)
{
	int nAddLife = m_nLife += addLife;

	//加算されたHPが最大を超えた場合
	if (nAddLife >= m_nMaxLife)
	{
		m_nLife = m_nMaxLife;

		CScore *pScore = CScore::GetScore();
		pScore->AddScore(10);
	}
	else
	{
		m_nLife = nAddLife;
		CSound::PlaySound(SOUND_LABEL_SE_ENERGY_1);
	}

	m_pLife->SetLife(m_nLife, m_nMaxLife, m_nFastMaxLife);
}

//=====================================
//体力加算処理
//=====================================
void CPlayer::AddMaxLife(int addMaxLife)
{
	int nAddLife = m_nMaxLife += addMaxLife;

	//加算されたHPが最大を超えた場合
	if (nAddLife >= PLAYER_FINAL_LIFE)
	{
		m_nMaxLife = PLAYER_FINAL_LIFE;
		m_nLife += 10;
		if (m_nLife > m_nMaxLife)
		{
			m_nLife = m_nMaxLife;
		}
		CScore *pScore = CScore::GetScore();
		pScore->AddScore(50);
	}
	else
	{
		CSound::PlaySound(SOUND_LABEL_SE_ENERGY_4);
		m_nMaxLife = nAddLife;
		m_nLife += addMaxLife;
	}
	m_pLife->SetLife(m_nLife, m_nMaxLife, m_nFastMaxLife);
}

//=====================================
//速度加算処理
//=====================================
void CPlayer::AddSpeed(float nSpeed)
{
	//スピードが最大を超えていない場合
	if (m_fSpeed < PLAYER_MAX_MOVE_SPEED)
	{
		m_fSpeed += nSpeed;
		CSound::PlaySound(SOUND_LABEL_SE_ENERGY_2);
	}
	else
	{
		m_fSpeed = PLAYER_MAX_MOVE_SPEED;
		CScore *pScore = CScore::GetScore();
		pScore->AddScore(20);
	}
}

//=====================================
//弾所有数加算処理
//=====================================
void CPlayer::AddBullet()
{
	//弾所有数が最大を超えていない場合
	if (m_nHoldBullet < PLAYER_MAX_BULLET)
	{
		m_nHoldBullet++;
		CSound::PlaySound(SOUND_LABEL_SE_ENERGY_3);
	}
	else
	{
		CScore *pScore = CScore::GetScore();
		pScore->AddScore(30);
	}
}

//=====================================
//入力処理
//=====================================
void CPlayer::Input(D3DXVECTOR3 pos)
{
	//キーボード取得
	CInput *pCInput = CApplication::GetInput();

	//プレイヤー移動
	if (pCInput->GetKeyboardPress(DIK_D))
	{//右移動
		if (pCInput->GetKeyboardPress(DIK_S))
		{//下移動
			m_move += NormalizeLength(1.0f, 1.0f);
		}
		else if (pCInput->GetKeyboardPress(DIK_W))
		{//上移動
			m_move += NormalizeLength(1.0f, -1.0f);
		}
		else
		{
			m_move.x += 1.0f;
		}
	}
	else if (pCInput->GetKeyboardPress(DIK_A))
	{//左移動
		if (pCInput->GetKeyboardPress(DIK_S))
		{//下移動
			m_move += NormalizeLength(-1.0f, 1.0f);
		}
		else if (pCInput->GetKeyboardPress(DIK_W))
		{//上移動
			m_move += NormalizeLength(-1.0f, -1.0f);
		}
		else
		{
			m_move.x += -1.0f;
		}
	}
	else if (pCInput->GetKeyboardPress(DIK_S))
	{//下移動
		m_move.y += 1.0f;
	}
	else if (pCInput->GetKeyboardPress(DIK_W))
	{//上移動
		m_move.y += -1.0f;
	}

	if (m_nBulletCounter <= 0)
	{
		if (pCInput->GetKeyboardPress(DIK_B))
		{
			//レーザー生成処理
			CBullet::Create(D3DXVECTOR3(pos.x + (m_fWidth / 2), pos.y, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), CBullet::PLAYER_LASER);
			m_nBulletCounter = 25 - (m_nHoldBullet * 3);
		}
		else if (pCInput->GetKeyboardPress(DIK_SPACE))
		{
			if (m_nHoldBullet >= 5)
			{
				//弾生成処理
				CBullet::Create(D3DXVECTOR3(pos.x + (m_fWidth / 2), pos.y, 0.0f), D3DXVECTOR3(sinf((float)D3DX_PI / 8), cosf((float)D3DX_PI / 8), 0.0f), CBullet::PLAYER_NORMAL);
			}
			if (m_nHoldBullet >= 4)
			{
				//弾生成処理
				CBullet::Create(D3DXVECTOR3(pos.x + (m_fWidth / 2), pos.y, 0.0f), D3DXVECTOR3(sinf((float)D3DX_PI / 8 * 7), cosf((float)D3DX_PI / 8 * 7), 0.0f), CBullet::PLAYER_NORMAL);
			}
			if (m_nHoldBullet >= 3)
			{
				//弾生成処理
				CBullet::Create(D3DXVECTOR3(pos.x + (m_fWidth / 2), pos.y, 0.0f), D3DXVECTOR3(sinf((float)D3DX_PI / 8 * 3), cosf((float)D3DX_PI / 8 * 3), 0.0f), CBullet::PLAYER_NORMAL);
			}
			if (m_nHoldBullet >= 2)
			{
				//弾生成処理
				CBullet::Create(D3DXVECTOR3(pos.x + (m_fWidth / 2), pos.y, 0.0f), D3DXVECTOR3(sinf((float)D3DX_PI / 8 * 5), cosf((float)D3DX_PI / 8 * 5), 0.0f), CBullet::PLAYER_NORMAL);
			}
			//弾生成処理
			CBullet::Create(D3DXVECTOR3(pos.x + (m_fWidth / 2), pos.y, 0.0f), D3DXVECTOR3(sinf((float)D3DX_PI / 2), cosf((float)D3DX_PI / 2), 0.0f), CBullet::PLAYER_NORMAL);
			m_nBulletCounter = 10;
		}
	}
	else
	{
		//弾発射
		if (pCInput->GetKeyboardTrigger(DIK_SPACE))
		{
			if (m_nHoldBullet >= 5)
			{
				//弾生成処理
				CBullet::Create(pos, D3DXVECTOR3(sinf((float)D3DX_PI / 8), cosf((float)D3DX_PI / 8), 0.0f), CBullet::PLAYER_NORMAL);
			}
			if (m_nHoldBullet >= 4)
			{
				//弾生成処理
				CBullet::Create(pos, D3DXVECTOR3(sinf((float)D3DX_PI / 8 * 7), cosf((float)D3DX_PI / 8 * 7), 0.0f), CBullet::PLAYER_NORMAL);
			}
			if (m_nHoldBullet >= 3)
			{
				//弾生成処理
				CBullet::Create(pos, D3DXVECTOR3(sinf((float)D3DX_PI / 8 * 3), cosf((float)D3DX_PI / 8 * 3), 0.0f), CBullet::PLAYER_NORMAL);
			}
			if (m_nHoldBullet >= 2)
			{
				//弾生成処理
				CBullet::Create(pos, D3DXVECTOR3(sinf((float)D3DX_PI / 8 * 5), cosf((float)D3DX_PI / 8 * 5), 0.0f), CBullet::PLAYER_NORMAL);
			}
			//弾生成処理
			CBullet::Create(pos, D3DXVECTOR3(1.0f, 0.0f, 0.0f), CBullet::PLAYER_NORMAL);
			m_nBulletCounter = 25 - (m_nHoldBullet * 3);
		}
	}
}

//=====================================
//画面外処理
//=====================================
void CPlayer::OffScreen(D3DXVECTOR3 pos)
{
	//画面内から画面外に出た場合
	if (pos.x + (m_fWidth / 2) > SCREEN_WIDTH)
	{//画面右端まで来た時の処理
		pos.x = SCREEN_WIDTH - (m_fWidth / 2);
		SetPos(pos);
	}
	if (pos.x - (m_fWidth / 2) < 0.0f)
	{//画面左端まで来た時の処理
		pos.x = 0.0f + (m_fWidth / 2);
		SetPos(pos);
	}
	if (pos.y + (m_fHeight / 2) > SCREEN_HEIGHT)
	{//画面上端まで来た時の処理
		pos.y = SCREEN_HEIGHT - (m_fHeight / 2);
		SetPos(pos);
	}
	if (pos.y - (m_fHeight / 2) < 0.0f)
	{//画面下端まで来た時の処理
		pos.y = 0.0f + (m_fHeight / 2);
		SetPos(pos);
	}
}

//=====================================
//消去処理
//=====================================
void CPlayer::Destroy(D3DXVECTOR3 pos)
{
	//爆発生成処理
	CExplosion::Create(pos, m_fWidth, m_fHeight);

	m_nLife = 0;
	m_pLife->SetLife(m_nLife, m_nMaxLife, m_nFastMaxLife);
	m_situ = SITU_DEATH;
}

//=====================================
//当たり判定処理
//=====================================
void CPlayer::Collision()
{
	CObject *pCenter = (CObject*)CObject2D::GetTop(OBJECT_PRIORITY_CENTER);	//オブジェクトの先頭ポインタ
	if (pCenter != nullptr)
	{
		//現在位置ポインタに先頭ポインタを代入
		CObject *pObject = pCenter;

		//現在位置ポインタが使用中の場合
		while (pObject != nullptr)
		{
			CObject::Object_Type objtype = pObject->GetType();

			if (objtype == CObject::TYPE_ENEMY)
			{//敵キャラで無い場合
				if (CollisionCircle(GetPos(), GetLength(), pObject->GetPos(), pObject->GetLength()))
				{//重なった場合
					if (m_situ == SITU_NONE)
					{//状態変化中の複数判定を消す処理
						Hit(20);
					}
				}
			}
			//現在位置ポインタに次回ポインタを代入する(ポインタを進める処理)
			pObject = pObject->GetNext();
		}
	}

	CObject *pBack = (CObject*)CObject2D::GetTop(OBJECT_PRIORITY_BACK);	//オブジェクトの先頭ポインタ
	if (pBack != nullptr)
	{
		//現在位置ポインタに先頭ポインタを代入
		CObject *pObject = pBack;

		//現在位置ポインタが使用中の場合
		while (pObject != nullptr)
		{
			CObject::Object_Type objtype = pObject->GetType();

			if (objtype == CObject::TYPE_OBSTACLE)
			{//障害物の場合
				CObstacle *pObstacle = (CObstacle*)pObject;

				if (pObstacle->GetFlip() == CObstacle::FLIP_CIRCLE)
				{
					if (CollisionCircle(GetPos(), GetLength(), pObject->GetPos(), pObject->GetLength()))
					{//重なった場合
						SetPos(pObstacle->FlipCircle(GetPos(), m_posOld, GetLength()));
						if (GetPos().x - (m_fWidth / 2) <= 0.0f)
						{
							Destroy(GetPos());
						}
						if (m_situ == SITU_NONE)
						{//状態変化中の複数判定を消す処理
							Hit(20);
						}
					}
				}
				else if (pObstacle->GetFlip() == CObstacle::FLIP_SQUARE)
				{
					if (CollisionSquare(GetPos(), pObject->GetPos(), m_fWidth, m_fHeight, pObject->GetWidth(), pObject->GetHeight()))
					{//重なった場合無視
						SetPos(pObstacle->FlipSquare(GetPos(), m_posOld, m_fWidth, m_fHeight));
						if (GetPos().x - (m_fWidth / 2) <= 0.0f
							&& GetPos().y - (m_fHeight / 2) < pObject->GetPos().y + (pObject->GetHeight() / 2)
							&& GetPos().y + (m_fHeight / 2) > pObject->GetPos().y - (pObject->GetHeight() / 2))
						{
							Destroy(GetPos());
						}
						if (m_situ == SITU_NONE)
						{//状態変化中の複数判定を消す処理
							Hit(20);
						}
					}
				}
			}
			//現在位置ポインタに次回ポインタを代入する(ポインタを進める処理)
			pObject = pObject->GetNext();
		}
	}
}

//=====================================
//点滅色処理
//=====================================
void CPlayer::BlinkColor()
{
	if (m_bBlink)
	{
		m_fBlinkCounter--;
	}
	else
	{
		m_fBlinkCounter++;
	}

	if (m_fBlinkCounter >= 1.0f / PLAYER_BLINK_SPEED)
	{
		m_bBlink = true;
		m_fBlinkCounter = 1.0f / PLAYER_BLINK_SPEED;
		m_nBlinkLoop++;
	}
	else if(m_fBlinkCounter <= 0.0f)
	{
		m_bBlink = false;
		m_fBlinkCounter = 0.0f;
	}

	//点滅させる部分の色を計算
	D3DXCOLOR colBlink = D3DXCOLOR(1.0f, 0.6f, 0.6f, m_fBlinkCounter * PLAYER_BLINK_SPEED);

	SetColor(colBlink);

	if (m_nBlinkLoop >= PLAYER_MAX_BLINK)
	{
		m_nBlinkLoop = 0;
		m_situ = SITU_NONE;
		//色初期化
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//=====================================
//プレイヤー状態処理
//=====================================
void CPlayer::SituationType()
{
	switch (m_situ)
	{
	case CPlayer::SITU_NONE:
		break;
	case CPlayer::SITU_DAMAGE:
		BlinkColor();
		break;
	case CPlayer::SITU_DEATH:

		if (m_nDeathTimer > 120)
		{
			CGame::SetEndGame();
			Uninit();
		}
		else
		{
			m_nDeathTimer++;
		}
		break;
	default:
		break;
	}
}

//=====================================
//プレイヤー生成処理
//=====================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	m_pPlayer = new CPlayer;		//プレイヤーポインタ

	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Init();
		m_pPlayer->SetPos(pos);

		m_pPlayer->m_pLife = CLife::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 50.0f, 0.0f));
	}
	return m_pPlayer;
}

//=====================================
//読み込み処理
//=====================================
void CPlayer::Load()
{
	char s_aString[256];		//読み込む用文字列
	int nNumType = 0;			//読み込んだタイプ数

								//ファイルを開く
	FILE* pFile = fopen(PLAYER_FILE, "r");

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

				//assert(PLAYER_TYPE_MAX != nNumType);	//想定より多いファイルの読み込み

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
				nNumType++;
			}
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}