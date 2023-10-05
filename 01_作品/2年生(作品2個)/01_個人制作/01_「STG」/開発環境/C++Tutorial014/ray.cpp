//=============================================================================
//
// 光線エフェクト
// Author : tanimoto kosuke
//
//=============================================================================
#include "ray.h"
#include <stdio.h>
#include "application.h"
#include "texture.h"

//---------------------------
//マクロ関数
//---------------------------
#define RAY_FILE			"data/.txt/ray.txt"	//読み込みファイル
#define SHRINK_EFFECT		(0.2f)				// エフェクト縮小速度
#define RAND_EFFECT			(2.0f)				// エフェクトの振れ幅
#define BLINK_EFFECT_SPEED	(0.02f)				//エフェクト点滅速度
#define BLINK_EFFECT_MAX	(0.6f)				//エフェクト点滅最大値
#define BLINK_EFFECT_MIN	(0.2f)				//エフェクト点滅最小値

//=====================================
//デフォルトコンストラクタ
//=====================================
CRay::CRay() : CObject2D(EFFECT_PRIORITY_BACK), m_fAlphaCounter(BLINK_EFFECT_MAX), m_bBlink(false)
{

}

//=====================================
//デストラクタ
//=====================================
CRay::~CRay()
{

}

//=====================================
//初期化処理
//=====================================
void CRay::Init()
{
	CObject2D::Init();
}

//=====================================
//終了処理
//=====================================
void CRay::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//更新処理
//=====================================
void CRay::Update()
{
	CObject2D::Update();

	//UV設定
	CObject2D::SetUV();

	//位置取得処理
	D3DXVECTOR3 pos = CObject2D::GetPos();

	//種別更新処理
	UpdateType();

	//位置更新
	pos += m_move * m_fSpeed;

	//位置設定処理
	SetPos(pos);

	//減少処理
	Decline();
}

//=====================================
//描画処理
//=====================================
void CRay::Draw()
{
	//画像取得処理
	CApplication::GetTexture()->TextureType(CTexture::TYPE_RAY_01);
	CObject2D::Draw();
}

//=====================================
//種別初期化処理
//=====================================
void CRay::InitType()
{
	switch (m_type)
	{
	case CRay::NONE:
		break;
	case CRay::BULLET:
		Rand();
		CObject2D::SetColor(D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.8f, 0.8f, 1.0f));
		break;
	default:
		break;
	}
}

//=====================================
//種別更新処理
//=====================================
void CRay::UpdateType()
{
	//長さ取得処理
	float fLength = CObject2D::GetLength();

	switch (m_type)
	{
	case CRay::NONE:
		break;
	case CRay::BULLET:
		if (m_bBlink == false)
		{
			if (m_fAlphaCounter < BLINK_EFFECT_MAX)
			{
				m_fAlphaCounter += BLINK_EFFECT_SPEED;
			}
			else
			{
				m_fAlphaCounter = BLINK_EFFECT_MAX;
				m_bBlink = true;
			}
		}
		else
		{
			if (m_fAlphaCounter > BLINK_EFFECT_MIN)
			{
				m_fAlphaCounter -= BLINK_EFFECT_SPEED;
			}
			else
			{
				m_fAlphaCounter = BLINK_EFFECT_MIN;
				m_bBlink = false;
			}
		}
		CObject2D::SetColor(D3DXCOLOR(0.5f, 0.0f, 0.0f, m_fAlphaCounter), D3DXCOLOR(1.0f, 0.2f, 0.0f, m_fAlphaCounter), D3DXCOLOR(1.0f, 1.0f, 0.0f, m_fAlphaCounter), D3DXCOLOR(1.0f, 0.8f, 0.8f, m_fAlphaCounter));

		fLength -= fLength * SHRINK_EFFECT;
		SetLength(fLength);
		break;
	default:
		break;
	}
}

//=====================================
//消去処理
//=====================================
void CRay::Destroy()
{
	CObject2D::Uninit();
}

//=====================================
//ランダム設定処理
//=====================================
void CRay::Rand()
{
	D3DXVECTOR3 pos = GetPos();

	float randX = (rand() / (float)RAND_MAX) * (RAND_EFFECT * 2) - RAND_EFFECT;
	float randY = (rand() / (float)RAND_MAX) * (RAND_EFFECT * 2) - RAND_EFFECT;

	pos.x += randX;
	pos.y += randY;
	m_move.x *= randX;
	m_move.y *= -randY;

	//位置設定処理
	SetPos(pos);
}

//=====================================
//減少処理
//=====================================
void CRay::Decline()
{
	//寿命計算
	if (m_nLife <= 0)
	{
		Destroy();
	}
	else
	{//寿命を減らす
		m_nLife--;
	}
}

//=====================================
//プレイヤー弾生成処理
//=====================================
CRay *CRay::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, Ray_Type type)
{
	CRay* pRay = new CRay;	//弾のポインタ

	if (pRay != nullptr)
	{
		pRay->Init();
		//オブジェクト設定処理
		pRay->m_move = move;
		pRay->SetPos(pos - move * 10.0f);
		pRay->m_type = type;
		pRay->LoadRay();
		pRay->InitType();
		pRay->SetObject2D(TYPE_EFFECT, pRay->m_fWidth, pRay->m_fHeight);
	}
	return pRay;
}

//=====================================
//読み込み処理(プレイヤー弾)
//=====================================
void CRay::LoadRay()
{
	char s_aString[256];		//読み込む用文字列
	int nNumType = 0;			//読み込んだタイプ数

								//ファイルを開く
	FILE* pFile = fopen(RAY_FILE, "r");

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