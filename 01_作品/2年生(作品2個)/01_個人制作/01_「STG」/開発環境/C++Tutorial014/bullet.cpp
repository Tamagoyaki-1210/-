//=============================================================================
//
// 弾
// Author : tanimoto kosuke
//
//=============================================================================
#include "bullet.h"
#include <stdio.h>
#include "matrix.h"
#include "input.h"
#include "application.h"
#include "texture.h"
#include "explosion.h"
#include "player.h"
#include "enemy.h"
#include "ray.h"
#include "obstacle.h"
#include "score.h"

//---------------------------
//マクロ関数
//---------------------------
#define PLAYER_BULLET_FILE		"data/.txt/player_bullet.txt"	//読み込みファイル
#define ENEMY_BULLET_FILE		"data/.txt/enemy_bullet.txt"	//読み込みファイル

//=====================================
//デフォルトコンストラクタ
//=====================================
CBullet::CBullet() : CObject2D(EFFECT_PRIORITY_CENTER), m_nPenetrat(0)
{
	//敵キャラポインタ初期化処理
	for (int nInd = 0; nInd < MAX_ENEMY_HIT; nInd++)
	{
		m_pEnemy[nInd] = nullptr;
	}

	//障害物ポインタ初期化処理
	for (int nInd = 0; nInd < MAX_OBSTACLE_HIT; nInd++)
	{
		m_pObstacle[nInd] = nullptr;
	}
}

//=====================================
//デストラクタ
//=====================================
CBullet::~CBullet()
{

}

//=====================================
//初期化処理
//=====================================
void CBullet::Init()
{
	CObject2D::Init();
}

//=====================================
//終了処理
//=====================================
void CBullet::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//更新処理
//=====================================
void CBullet::Update()
{
	CObject2D::Update();

	//UV設定
	CObject2D::SetUV();

	//位置取得処理
	D3DXVECTOR3 pos = CObject2D::GetPos();

	m_poslast = pos;
	
	//長さ取得処理
	float fLength = CObject2D::GetLength();

	//位置更新
	pos += m_move * m_fSpeed;

	//位置設定処理
	SetPos(pos);

	//位置設定処理
	SetLength(fLength);

	//種別AI処理
	TypeAI(pos);

	//当たり判定設定処理
	Collision(pos, fLength);

	//寿命計算
	if (m_nLife <= 0)
	{
		Destroy();
	}
	else
	{//寿命を減らす
		m_nLife--;
		OffScreen(pos);
	}
}

//=====================================
//描画処理
//=====================================
void CBullet::Draw()
{
	switch (m_collisionBullet)
	{
	case COLLISION_CIRCLE:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_OBSTACLE_02);
		break;
	case COLLISION_SQUARE:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_BULLET_02);
		break;
	default:
		break;
	}
	CObject2D::Draw();
}

//=====================================
//種別AI処理
//=====================================
void CBullet::TypeAI(D3DXVECTOR3 pos)
{
	//プレイヤー用弾
	switch (m_playerBullet)
	{
	case PLAYER_NONE :
		break;
	case PLAYER_NORMAL:
		m_collisionBullet = COLLISION_CIRCLE;
		m_bHitDestroy = true;
		CRay::Create(pos, m_move, CRay::BULLET);
		break;
	case PLAYER_LASER:
		SetColor(D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));
		m_collisionBullet = COLLISION_SQUARE;
		if (m_fWidth < m_fMaxWidth)
		{
			m_fWidth += 5;
			SetObject2D(TYPE_EFFECT, m_fWidth, m_fHeight);
		}
		m_bHitDestroy = false;
		break;
	case PLAYER_BOMM:
		SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
		m_collisionBullet = COLLISION_SQUARE;
		m_bHitDestroy = false;
		break;
	default:
		break;
	}

	//敵キャラ用弾
	switch (m_enemyBullet)
	{
	case ENEMY_NONE:
		break;
	case ENEMY_NORMAL:
		m_collisionBullet = COLLISION_CIRCLE;
		m_bHitDestroy = true;
		CRay::Create(pos, m_move, CRay::BULLET);
		break;
	case ENEMY_LASER:
		m_collisionBullet = COLLISION_SQUARE;
		m_bHitDestroy = false;
		break;

	case ENEMY_SPREAD:
		m_collisionBullet = COLLISION_CIRCLE;
		m_bHitDestroy = true;
		CRay::Create(pos, m_move, CRay::BULLET);

		if (m_fSpeed < 2.0f)
		{
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				CBullet::Create(pos, D3DXVECTOR3(-sinf(D3DX_PI / 5 * nCnt * 2), -cosf(D3DX_PI / 5 * nCnt * 2), 0.0f), CBullet::ENEMY_NORMAL);	//弾生成処理
			}
			Destroy();
		}
		else
		{
			m_fSpeed *= 0.98f;
		}

		break;
	default:
		break;
	}
}

//=====================================
//画面外処理
//=====================================
void CBullet::OffScreen(D3DXVECTOR3 pos)
{
	if (pos.x < -m_fWidth / 2 || pos.x > F_SCREEN_WIDTH || pos.y  < -m_fHeight / 2 || pos.y > F_SCREEN_HEIGHT + m_fHeight / 2)
	{
		Uninit();
	}
}

//=====================================
//消去処理
//=====================================
void CBullet::Destroy()
{
	CBullet::Uninit();
}

//=====================================
//当たり判定設定処理
//=====================================
void CBullet::Collision(D3DXVECTOR3 pos, float fLength)
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

			//プレイヤー弾の場合
			if (m_playerBullet != PLAYER_NONE)
			{
				//対象が敵キャラの場合
				if (objtype == CObject::TYPE_ENEMY)
				{
					CEnemy *pEnemy = (CEnemy*)pObject;

					//円形当たり判定の場合
					if (m_collisionBullet == COLLISION_CIRCLE)
					{//重なっている場合
						if (CollisionCircle(pos, fLength, pObject->GetPos(), pObject->GetLength()))
						{
							//爆発生成処理
							CExplosion::Create(pos, EXPLOSION_WIDTH, EXPLOSION_HEIGHT);

							//被弾処理
							pEnemy->Hit(m_nPower);

							//10点加算
							CScore *pScore = CScore::GetScore();
							pScore->AddScore(20);

							//弾破壊
							Destroy();
						}
					}
					//四角形当たり判定の場合
					else if (m_collisionBullet == COLLISION_SQUARE)
					{//重なっている場合
						if (CollisionSquare(pos, pObject->GetPos(), m_fWidth, m_fHeight, pObject->GetWidth(), pObject->GetHeight()))
						{
							//弾とオブジェクトが当たった判定
							bool bHit = false;

							for (int nIndEnemy = 0; nIndEnemy < MAX_ENEMY_HIT; nIndEnemy++)
							{
								//敵キャラのポインタが一致した場合
								if (m_pEnemy[nIndEnemy] == pEnemy)
								{
									bHit = true;
									break;
								}
							}
							//弾がヒット前の場合
							if (!bHit)
							{
								if (pEnemy->GetType() != CEnemy::TYPE_03)
								{
									pEnemy->SetSitu(CEnemy::SITU_DAMAGE);
								}

								if (m_playerBullet != PLAYER_BOMM)
								{
									CSound::PlaySound(SOUND_LABEL_SE_BULLET_LASER_DAMAGE);
								}
								//被弾処理
								pEnemy->Hit(m_nPower);

								CScore *pScore = CScore::GetScore();
								if (m_playerBullet == PLAYER_BOMM)
								{
									if (m_nPenetrat < 5)
									{
										m_nPenetrat++;
									}
									pScore->AddScore(100 * m_nPenetrat);
								}
								else
								{
									pScore->AddScore(10);
								}

								for (int nIndEnemy = 0; nIndEnemy < MAX_ENEMY_HIT; nIndEnemy++)
								{
									//中身が存在しない場合
									if (m_pEnemy[nIndEnemy] == nullptr)
									{
										//Hitしたオブジェクトのポインタを中身に登録する
										m_pEnemy[nIndEnemy] = pEnemy;
										break;
									}
								}
							}
						}
					}
				}
			}
			//敵キャラ弾の場合
			else if (m_enemyBullet != ENEMY_NONE)
			{	//プレイヤーの場合
				if (objtype == CObject::TYPE_PLAYER)
				{
					CPlayer *pPlayer = (CPlayer*)pObject;

					//プレイヤーが通常状態の場合
					if (pPlayer->GetSitu() == CPlayer::SITU_NONE)
					{
						//円形の場合
						if (m_collisionBullet == COLLISION_CIRCLE)
						{//重なっている場合
							if (CollisionCircle(pos, fLength, pObject->GetPos(), pObject->GetLength()))
							{
								//弾破壊
								Destroy();

								//被弾処理
								pPlayer->Hit(m_nPower);
							}
						}
						//四角形の場合
						else if (m_collisionBullet == COLLISION_SQUARE)
						{//重なっている場合
							if (CollisionSquare(pos, pObject->GetPos(), m_fWidth, m_fHeight, pObject->GetWidth(), pObject->GetHeight()))
							{
								//弾破壊
								Destroy();

								//被弾処理
								pPlayer->Hit(m_nPower);
							}
						}
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
			//プレイヤー弾の場合
			if (m_playerBullet != PLAYER_NONE)
			{
				//障害物の場合
				if (objtype == CObject::TYPE_OBSTACLE)
				{
					CObstacle *pObstacle = (CObstacle*)pObject;

					//円形の場合
					if (m_collisionBullet == COLLISION_CIRCLE)
					{
						//重なっている場合
						if (CollisionCircle(pos, fLength, pObject->GetPos(), pObject->GetLength()))
						{
							//爆発生成処理
							CExplosion::Create(pos, EXPLOSION_WIDTH, EXPLOSION_HEIGHT);
							//弾破壊
							Destroy();
							//被弾処理
							pObstacle->Hit(m_nPower);
						}
					}
					//四角形の場合
					else if (m_collisionBullet == COLLISION_SQUARE)
					{
						//重なっている場合無視
						if (CollisionSquare(pos, pObject->GetPos(), m_fWidth, m_fHeight, pObject->GetWidth(), pObject->GetHeight()))
						{
							//弾とオブジェクトが当たった判定
							bool bHit = false;

							for (int nIndObstacle = 0; nIndObstacle < MAX_OBSTACLE_HIT; nIndObstacle++)
							{
								//敵キャラのポインタが一致した場合
								if (m_pObstacle[nIndObstacle] == pObstacle)
								{
									bHit = true;
									break;
								}
							}
							//弾がヒット前の場合
							if (!bHit)
							{
								pObstacle->SetSitu(CObstacle::SITU_DAMAGE);

								if (m_playerBullet != PLAYER_BOMM)
								{
									CSound::PlaySound(SOUND_LABEL_SE_BULLET_LASER_DAMAGE);
								}
								//被弾処理
								pObstacle->Hit(m_nPower * 5);

								CScore *pScore = CScore::GetScore();
								if (m_playerBullet == PLAYER_BOMM)
								{
									if (m_nPenetrat < 5)
									{
										m_nPenetrat++;
									}
									pScore->AddScore(100 * m_nPenetrat);
								}

								for (int nIndObstacle = 0; nIndObstacle < MAX_OBSTACLE_HIT; nIndObstacle++)
								{
									//中身が存在しない場合
									if (m_pObstacle[nIndObstacle] == nullptr)
									{
										//Hitしたオブジェクトのポインタを中身に登録する
										m_pObstacle[nIndObstacle] = pObstacle;
										break;
									}
								}
							}
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
//プレイヤー弾生成処理
//=====================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, Bullet_Player playerBullet)
{
	CBullet* pBullet = new CBullet;	//弾のポインタ

	if (pBullet != nullptr)
	{
		pBullet->Init();
		pBullet->SetPos(pos);
		pBullet->m_move = move;	//弾進行方向
		pBullet->m_playerBullet = playerBullet;
		pBullet->LoadPlayerBullet();

		switch (pBullet->m_playerBullet)
		{
		case PLAYER_NORMAL:
			CSound::PlaySound(SOUND_LABEL_SE_BULLET_NORMAL);
			break;
		case PLAYER_LASER:
			CSound::PlaySound(SOUND_LABEL_SE_BULLET_LASER);
			pBullet->m_fMaxWidth = pBullet->m_fWidth;
			pBullet->m_fWidth = 20.0f;
			break;
		default:
			break;
		}
		//オブジェクト設定処理
		pBullet->SetObject2D(TYPE_EFFECT, pBullet->m_fWidth, pBullet->m_fHeight);
	}

	return pBullet;
}

//=====================================
//敵キャラ弾生成処理
//=====================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, Bullet_Enemy enemyBullet)
{
	CBullet* pBullet = new CBullet;	//弾のポインタ

	if (pBullet != nullptr)
	{
		pBullet->Init();
		pBullet->SetPos(pos);
		pBullet->m_move = move;	//弾進行方向
		pBullet->m_enemyBullet = enemyBullet;
		pBullet->LoadEnemyBullet();
		pBullet->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));	//黄色
		//オブジェクト設定処理
		pBullet->SetObject2D(TYPE_EFFECT, pBullet->m_fWidth, pBullet->m_fHeight);
	}
	return pBullet;
}

//=====================================
//読み込み処理(プレイヤー弾)
//=====================================
void CBullet::LoadPlayerBullet()
{
	char s_aString[256];		//読み込む用文字列
	int nNumType = 0;			//読み込んだタイプ数

								//ファイルを開く
	FILE* pFile = fopen(PLAYER_BULLET_FILE, "r");

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

				while (strncmp(s_aString, "ENDSTATE", 8) != 0)
				{
					fscanf(pFile, "%s", s_aString);		//=の読み込み

					if (strcmp(s_aString, "LIFE") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_nLife);
					}
					else if (strcmp(s_aString, "POWER") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_nPower);
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
				if (m_playerBullet == nNumType)
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


//=====================================
//読み込み処理(敵キャラ弾)
//=====================================
void CBullet::LoadEnemyBullet()
{
	char s_aString[256];		//読み込む用文字列
	int nNumType = 0;			//読み込んだタイプ数

								//ファイルを開く
	FILE* pFile = fopen(ENEMY_BULLET_FILE, "r");

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

				while (strncmp(s_aString, "ENDSTATE", 8) != 0)
				{
					fscanf(pFile, "%s", s_aString);		//=の読み込み

					if (strcmp(s_aString, "LIFE") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_nLife);
					}
					else if (strcmp(s_aString, "POWER") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_nPower);
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
				if (m_enemyBullet == nNumType)
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