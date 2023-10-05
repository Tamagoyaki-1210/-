//=============================================================================
//
// 障害物
// Author : tanimoto kosuke
//
//=============================================================================
#include "obstacle.h"
#include <assert.h>
#include <stdio.h>
#include "matrix.h"
#include "explosion.h"
#include "application.h"
#include "texture.h"
#include "score.h"

//---------------------------
//マクロ関数
//---------------------------
#define OBSTACLE_BLINK_SPEED		(0.5f)
#define OBSTACLE_MAX_BLINK			(4)
#define OBSTACLE_MOVE_SPEED			(2.0f)
#define OBSTACLE_FILE				"data/.txt/obstacle.txt"		//読み込みファイル

//=====================================
//デフォルトコンストラクタ
//=====================================
CObstacle::CObstacle() : CObject2D(OBJECT_PRIORITY_BACK)
{

}

//=====================================
//デストラクタ
//=====================================
CObstacle::~CObstacle()
{

}

//=====================================
//初期化処理
//=====================================
void CObstacle::Init()
{
	CObject2D::Init();
}

//=====================================
//終了処理
//=====================================
void CObstacle::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//更新処理
//=====================================
void CObstacle::Update()
{
	CObject2D::Update();

	//UV設定
	CObject2D::SetUV();

	CObject2D::SetAnim(m_animX, m_animY, 6);

	//位置取得
	D3DXVECTOR3 pos = CObject2D::GetPos();

	m_posOld = pos;

	SituationType();

	//種別毎のAI
	TypeAI();

	//位置更新
	pos += m_move;

	//位置設定処理
	SetPos(pos);

	OffScreen(pos);
}

//=====================================
//描画処理
//=====================================
void CObstacle::Draw()
{
	//画像取得処理
	switch (m_type)
	{
	case TYPE_01:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_OBSTACLE_01);
		break;
	case TYPE_02:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_BULLET_01);
		break;
	default:
		break;
	}
	CObject2D::Draw();
}

//=====================================
//種別毎のAI処理
//=====================================
void CObstacle::TypeAI()
{
	switch (m_type)
	{
	case TYPE_01:
		m_move.x = -OBSTACLE_MOVE_SPEED;
		break;
	case TYPE_02:
		m_move.x = -OBSTACLE_MOVE_SPEED;
		break;
	default:
		break;
	}
}

//=====================================
//被弾処理
//=====================================
void CObstacle::Hit(int nDamage)
{
	m_nLife -= nDamage;
	if (m_nLife <= 0)
	{
		m_nLife = 0;
		Destroy(CObject2D::GetPos());
	}
}
//=====================================
//設定処理
//=====================================
void CObstacle::SetType()
{
	Load();
	switch (m_type)
	{
	case TYPE_01:
		m_flip = FLIP_SQUARE;
		break;
	case TYPE_02:
		m_flip = FLIP_CIRCLE;
		break;
	default:
		break;
	}
	//オブジェクト設定処理
	SetObject2D(CObject::TYPE_OBSTACLE, m_fWidth, m_fHeight);
}

//=====================================
//画面外処理
//=====================================
void CObstacle::OffScreen(D3DXVECTOR3 pos)
{
	if (pos.x + (SCREEN_WIDTH / 2) <= 0.0f || pos.y + (SCREEN_HEIGHT / 2) <= 0.0f || pos.y - (SCREEN_HEIGHT / 2) >= SCREEN_HEIGHT)
	{
		Uninit();
	}
}

//=====================================
//消去処理
//=====================================
void CObstacle::Destroy(D3DXVECTOR3 pos)
{
	//破壊した場合10点加算
	CScore *pScore = CScore::GetScore();
	pScore->AddScore(50);
	//爆発生成処理
	CExplosion::Create(pos, m_fWidth, m_fHeight);
	Uninit();
}

//=====================================
//四角形障害物の弾き処理
//=====================================
D3DXVECTOR3 CObstacle::FlipCircle(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fLength)
{
	float fSumLengthRad = (GetLength() + fLength) / 2;	//円の半径同士の和

	D3DXVECTOR3 posObst = GetPos();
	float fSubX = posObst.x - pos.x;									//X座標の差分
	float fSubY = posObst.y - pos.y;									//Y座標の差分
	float fSubLength = sqrtf((fSubX * fSubX) + (fSubY * fSubY));	//現在の2点の距離

	float fSubXOld = m_posOld.x - posOld.x;											//前回X座標の差分
	float fSubYOld = m_posOld.y - posOld.y;											//前回Y座標の差分

	D3DXVECTOR3 flipPos = posOld;

	//(現在位置が重なり、前回位置が重なっていない場合)
	//円半径長さの和が現在2点の距離より大きく、円半径長さの和が前回2点の距離より小さい場合
	if (fSumLengthRad >= fSubLength)
	{
		if (fSubXOld != 0.0f)
		{
			//長さの正規化
			fSubXOld = Normalize(fSubXOld) * (OBSTACLE_MOVE_SPEED + 1);
		}
		if (fSubYOld != 0.0f)
		{
			fSubYOld = Normalize(fSubYOld) * (OBSTACLE_MOVE_SPEED + 1);
		}
		//前回位置と前回座標の差分の長さで減算する
		flipPos -= D3DXVECTOR3(fSubXOld, fSubYOld, 0.0f);
	}
	return flipPos;
}

//=====================================
//四角形障害物の弾き処理
//=====================================
D3DXVECTOR3 CObstacle::FlipSquare(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fWidth, float fHeight)
{
	float fLeft = pos.x - (fWidth / 2);				//P四角形左側
	float fRight = pos.x + (fWidth / 2);			//P四角形右側
	float fUpp = pos.y - (fHeight / 2);				//P四角形上側
	float fLow = pos.y + (fHeight / 2);				//P四角形下側

	float fLeftOld = posOld.x - (fWidth / 2);		//P前回四角形左側
	float fRightOld = posOld.x + (fWidth / 2);		//P前回四角形右側
	float fUppOld = posOld.y - (fHeight / 2);		//P前回四角形上側
	float fLowOld = posOld.y + (fHeight / 2);		//P前回四角形下側

	D3DXVECTOR3 posObst = GetPos();
	float fWidthObst = m_fWidth / 2;
	float fHeightObst = m_fHeight / 2;

	float fLeftObst = posObst.x - fWidthObst;		//O四角形左側
	float fRightObst = posObst.x + fWidthObst;		//O四角形右側
	float fUppObst = posObst.y - fHeightObst;		//O四角形上側
	float fLowObst = posObst.y + fHeightObst;		//O四角形下側

	float fLeftObstOld = m_posOld.x - fWidthObst;	//O前回四角形左側
	float fRightObstOld = m_posOld.x + fWidthObst;	//O前回四角形右側
	float fUppObstOld = m_posOld.y - fHeightObst;	//O前回四角形上側
	float fLowObstOld = m_posOld.y + fHeightObst;	//O前回四角形下側

	D3DXVECTOR3 flipPos;
	//P左側当たり判定　P左側がO右側より左に在る場合
	if (fLeft < fRightObst && fLeftOld >= fRightObstOld)
	{
		flipPos = D3DXVECTOR3(fRightObst + (fWidth / 2), pos.y, 0.0f);
	}
	//P右側当たり判定 P右側がO左側より右に在る場合
	if (fRight > fLeftObst && fRightOld <= fLeftObstOld)
	{
		flipPos = D3DXVECTOR3(fLeftObst - (fWidth / 2), pos.y, 0.0f);
	}
	//P上側当たり判定 P上側がO下側より上に在る場合
	if (fUpp < fLowObst && fUppOld >= fLowObstOld)
	{
		flipPos = D3DXVECTOR3(pos.x, fLowObst + (fHeight / 2), 0.0f);
	}
	//P下側当たり判定 P下側がO上側より下に在る場合
	if (fLow > fUppObst && fLowOld <= fUppObstOld)
	{
		flipPos = D3DXVECTOR3(pos.x, fUppObst - (fHeight / 2), 0.0f);
	}

	return flipPos;
}


//=====================================
//点滅色処理
//=====================================
void CObstacle::BlinkColor()
{
	if (m_bBlink)
	{
		m_fBlinkCounter--;
	}
	else
	{
		m_fBlinkCounter++;
	}

	if (m_fBlinkCounter >= 1.0f / OBSTACLE_BLINK_SPEED)
	{
		m_bBlink = true;
		m_fBlinkCounter = 1.0f / OBSTACLE_BLINK_SPEED;
		m_nBlinkLoop++;
	}
	else if (m_fBlinkCounter <= 0.0f)
	{
		m_bBlink = false;
		m_fBlinkCounter = 0.0f;
	}

	//点滅させる部分の色を計算
	D3DXCOLOR colBlink = D3DXCOLOR(1.0f, 0.6f, 0.6f, m_fBlinkCounter * OBSTACLE_BLINK_SPEED);

	SetColor(colBlink);

	if (m_nBlinkLoop >= OBSTACLE_MAX_BLINK)
	{
		m_nBlinkLoop = 0;
		m_situ = SITU_NONE;
		//色初期化
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//=====================================
//障害物状態処理
//=====================================
void CObstacle::SituationType()
{
	switch (m_situ)
	{
	case CObstacle::SITU_NONE:
		break;
	case CObstacle::SITU_DAMAGE:
		BlinkColor();
		break;
	default:
		break;
	}
}

//=====================================
//生成処理
//=====================================
CObstacle *CObstacle::Create(D3DXVECTOR3 pos, Obstacle_Type type)
{
	CObstacle* pObstacle = new CObstacle;	//敵キャラポインタ

	if (pObstacle != nullptr)
	{
		pObstacle->Init();
		pObstacle->SetPos(pos);
		pObstacle->m_type = type;
		pObstacle->SetType();
	}

	return pObstacle;
}

//=====================================
//読み込み処理
//=====================================
void CObstacle::Load()
{
	char s_aString[256];		//読み込む用文字列
	int nNumType = 0;			//読み込んだタイプ数

								//ファイルを開く
	FILE* pFile = fopen(OBSTACLE_FILE, "r");

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
					else if (strcmp(s_aString, "ANIM_X") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_animX);
					}
					else if (strcmp(s_aString, "ANIM_Y") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%d", &m_animY);
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