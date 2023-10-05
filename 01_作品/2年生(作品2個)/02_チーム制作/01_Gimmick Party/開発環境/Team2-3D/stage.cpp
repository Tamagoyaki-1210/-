//=============================================================================
//
// stage.cpp
// Author : tanimoto kosuke
//
//=============================================================================
#include "stage.h"
#include "application.h"
#include "meshfield.h"
#include "player.h"
#include "debugProc.h"
#include "halfsphere.h"
#include "coin.h"
#include "goal.h"
#include "camera.h"
#include "SpikeBall.h"
#include "lavaFloor.h"
#include "score.h"
#include "message.h"
#include "silhouette.h"
#include "environment.h"
#include <string>

#include "trampoline.h"
#include "bouncePole.h"
#include "stoneSpawner.h"
#include "icePillarSpawner.h"

//アニメーション情報のテキストファイルの相対パス
char* CStage::m_pStagePass[STAGE_TYPE_MAX] =
{
	{ "data\\STAGESET\\StageSet1.txt" },
	{ "data\\STAGESET\\StageSet2.txt" },
	{ "data\\STAGESET\\StageSet3.txt" },
	{ "data\\STAGESET\\StageSetTutorial.txt" },
};

CMeshfield *CStage::m_pField[2] = {};
CHalfSphere* CStage::m_pSphere = nullptr;
CPlayer* CStage::m_pPlayer[PLAYER_MAX] = {};
bool CStage::m_bResult = false;

//=====================================
// デフォルトコンストラクタ
//=====================================
CStage::CStage()
{

}

//=====================================
// デストラクタ
//=====================================
CStage::~CStage()
{

}

//=====================================
// 初期化処理
//=====================================
HRESULT CStage::Init(void)
{
	// スフィアメッシュ
	m_pSphere = CHalfSphere::Create(D3DXVECTOR3(0.0f, -8000.0f, 1000.0f), D3DXVECTOR3(30000.0f, 0.0f, 30000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), CHalfSphere::SPHERE_UP);

	// ステージ読み込み処理
	Load();

	StageTexture();		// ステージ毎のテクスチャ設定処理

	if (m_pField[0] != nullptr)
	{
		m_pField[0]->SetPriority(1);
	}
	if (m_pField[1] != nullptr)
	{
		m_pField[1]->SetPriority(1);
	}

	CGoal::Create(D3DXVECTOR3(0.0f, -100.0f, 900.0f));

	// プレイヤーの生成
	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		m_pPlayer[nCnt] = CPlayer::Create(D3DXVECTOR3(-75.0f + (50 * nCnt), -150.0f, ((m_pField[0]->GetLine() - 20) * -70.0f) - 200.0f), nCnt);
	}

	if (CApplication::GetCamera() != nullptr)
	{
		CApplication::GetCamera()->SetPos(D3DXVECTOR3(0.0f, 0.0f, ((m_pField[0]->GetLine() - 21) * -70.0f) -500.0f), D3DXVECTOR3(0.0f, -200.0f, 100.0f));
	}

	CSilhouette::Create();

	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CStage::Uninit(void)
{
	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		if (m_pPlayer[nCnt] != nullptr)
		{
			m_pPlayer[nCnt]->Release();
			m_pPlayer[nCnt] = nullptr;
		}
	}

	if (m_pSphere != nullptr)
	{
		m_pSphere->Release();
		m_pSphere = nullptr;
	}

	for (int nField = 0; nField < 2; nField++)
	{
		if (m_pField[nField] != nullptr)
		{
			m_pField[nField]->Release();
			m_pField[nField] = nullptr;
		}
	}

	m_bResult = false;
}

//=====================================
// 更新処理
//=====================================
void CStage::Update(void)
{
	GameResult();
}

//=====================================
// ステージ毎のテクスチャ設定処理
//=====================================
void CStage::StageTexture(void)
{
	switch ((StageType)CApplication::GetStageSelect())
	{
	case STAGE_TYPE_1:
	{
		if (m_pField[0] != nullptr)
		{
			m_pField[0]->SetTexture(CObject::TEXTURE_BLOCK);
		}
		if (m_pField[1] != nullptr)
		{
			m_pField[1]->SetTexture(CObject::TEXTURE_GRASS);
		}
		if (m_pSphere != nullptr)
		{
			LPDIRECT3DTEXTURE9 pTex = CObject_2D::GetTexturePointer(CObject::TEXTURE_SKY);
			m_pSphere->BindTexture(pTex);
		}
	}
	break;
	case STAGE_TYPE_2:
	{
		if (m_pField[0] != nullptr)
		{
			m_pField[0]->SetTexture(CObject::TEXTURE_ROCK);
		}
		if (m_pField[1] != nullptr)
		{
			m_pField[1]->SetTexture(CObject::TEXTURE_LAVA);
			m_pField[1]->SetAnimation(true, D3DX_PI * 0.001f, 2.5f);
			m_pField[1]->SetTextureAnim(D3DXVECTOR2(0.0001f, 0.000005f));
		}
		if (m_pSphere != nullptr)
		{
			LPDIRECT3DTEXTURE9 pTex = CObject_2D::GetTexturePointer(CObject::TEXTURE_ROCK);
			m_pSphere->BindTexture(pTex);
		}
	}
	break;
	case STAGE_TYPE_3:
	{
		if (m_pField[0] != nullptr)
		{
			m_pField[0]->SetTexture(CObject::TEXTURE_SNOW);
		}
		if (m_pField[1] != nullptr)
		{
			m_pField[1]->SetTexture(CObject::TEXTURE_SNOW_AREA);
		}
		if (m_pSphere != nullptr)
		{
			LPDIRECT3DTEXTURE9 pTex = CObject_2D::GetTexturePointer(CObject::TEXTURE_SKY);
			m_pSphere->BindTexture(pTex);
		}

		CMeshfield* pField = CMeshfield::Create(D3DXVECTOR3(-135.0f, -149.9f, -1200.0f), Vec3Null, D3DXVECTOR2(30.0f, 70.0f), 14, 10, 0.01f);
		pField->SetTexture(CObject::TEXTURE_ICE);
		pField->ChangeHeight(0, 150, 0.0f);
		pField->SetPriority(0);

		pField = CMeshfield::Create(D3DXVECTOR3(-135.0f, -149.9f, 400.0f), Vec3Null, D3DXVECTOR2(30.0f, 70.0f), 13, 10, 0.01f);
		pField->SetTexture(CObject::TEXTURE_ICE);
		pField->ChangeHeight(0, 150, 0.0f);
		pField->SetPriority(0);
	}
	break;
	case STAGE_TUTORIAL:
	{
		if (m_pField[0] != nullptr)
		{
			m_pField[0]->SetTexture(CObject::TEXTURE_BLOCK);
		}
		if (m_pField[1] != nullptr)
		{
			m_pField[1]->SetTexture(CObject::TEXTURE_SKY);
		}
		if (m_pSphere != nullptr)
		{
			LPDIRECT3DTEXTURE9 pTex = CObject_2D::GetTexturePointer(CObject::TEXTURE_SKY);
			m_pSphere->BindTexture(pTex);
		}
	}
	break;
	default:
		break;
	}
}

//=====================================
// モデル設定処理
//=====================================
void CStage::SetModelType(D3DXVECTOR3 pos, ModelType type)
{
	switch (type)
	{
	case CStage::MODEL_SPIKEBALL:
	{
		// 鉄球
		CSpikeBall* pSpike = CSpikeBall::Create(pos);
		pSpike->SetShadowHeight(-149.8f);
	}
	break;
	case CStage::MODEL_BOUNCEPOLE:
	{
		// 跳ね返る円柱
		CBouncePole* pPole = CBouncePole::Create(pos);
		pPole->SetShadowHeight(-149.8f);
	}
	break;
	case CStage::MODEL_TRAMPOLINE:
	{
		// 跳ね返る円柱
		CTrampoline::Create(pos);
	}
	break;
	default:
		break;
	}
}

//=====================================
// 床設定処理
//=====================================
void CStage::SetFloorType(D3DXVECTOR3 pos, FloorType type)
{
	switch (type)
	{
	case CStage::FLOOR_LAVA:
	{
		// 溶岩床
		CLavaFloor::Create(pos);
	}
	break;
	case CStage::FLOOR_SLIP:
	{
		CMeshfield* pField = CMeshfield::Create(pos, Vec3Null, D3DXVECTOR2(20.0f, 20.0f), 5, 5, 0.005f);
		pField->SetTexture(CObject::TEXTURE_ICE);
	}
	break;
	default:
		break;
	}
}

//=====================================
// 生成モデル設定処理
//=====================================
void CStage::SetSpawnerType(D3DXVECTOR3 pos, float width, float length, int collDown, SpawnerType type)
{
	switch (type)
	{
	case CStage::SPAWNER_FALLSTONE:
	{
		// 落石の生成オブジェクト
		CStoneSpawner::Create(pos, -149.8f, width, length, collDown);
	}
	break;
	case CStage::SPAWNER_ICEPILLAR:
	{
		// 氷柱の生成オブジェクト
		CIcePillarSpawner::Create(pos, -149.8f, width, length, collDown);
	}
	break;
	default:
		break;
	}
}

//=====================================
// ゲームのリザルト処理
//=====================================
void CStage::GameResult()
{
	// リザルト処理が行われていない場合
	if (!m_bResult)
	{
		bool bGoal[PLAYER_MAX] = {};
		bool bRot[PLAYER_MAX] = {};

		for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
		{
			bGoal[nCnt] = m_pPlayer[nCnt]->GetGoal();
			bRot[nCnt] = m_pPlayer[nCnt]->GetRotCmp();
		}

		// 全員がゴール & 全員が振り向き完了した場合
		if (bGoal[0] && bGoal[1] && bGoal[2] && bGoal[3]
			&& bRot[0] && bRot[1] && bRot[2] && bRot[3])
		{
			m_bResult = true;
			ScoreComparison();
		}
	}
}

//=====================================
// スコア比較処理
//=====================================
void CStage::ScoreComparison()
{
	D3DXVECTOR2 PlayerScore[PLAYER_MAX] = {};
	D3DXVECTOR2 nChange;

	for (int nCount = 0; nCount < PLAYER_MAX; nCount++)
	{
		PlayerScore[nCount].x = (float)CScore::GetScore(nCount);
		PlayerScore[nCount].y = (float)nCount;
	}

	// 2つのプレイヤースコアをソートする(降順)
	for (int nCount = 0; nCount < PLAYER_MAX - 1; nCount++)
	{
		for (int nCount2 = nCount + 1; nCount2 < PLAYER_MAX; nCount2++)
		{
			if (PlayerScore[nCount].x < PlayerScore[nCount2].x)
			{
				// スコアの入れ替え
				nChange = PlayerScore[nCount2];
				PlayerScore[nCount2] = PlayerScore[nCount];
				PlayerScore[nCount] = nChange;
			}
		}
	}

	// 勝利人数を求める
	if ((int)PlayerScore[3].x == (int)PlayerScore[0].x)
	{
		m_pPlayer[(int)PlayerScore[0].y]->SetWinner(true);
		m_pPlayer[(int)PlayerScore[1].y]->SetWinner(true);
		m_pPlayer[(int)PlayerScore[2].y]->SetWinner(true);
		m_pPlayer[(int)PlayerScore[3].y]->SetWinner(true);

		CApplication::GetMsg()->GoalMessage(0);
	}
	else if ((int)PlayerScore[2].x == (int)PlayerScore[0].x)
	{
		m_pPlayer[(int)PlayerScore[0].y]->SetWinner(true);
		m_pPlayer[(int)PlayerScore[1].y]->SetWinner(true);
		m_pPlayer[(int)PlayerScore[2].y]->SetWinner(true);
		m_pPlayer[(int)PlayerScore[3].y]->SetWinner(false);

		CApplication::GetMsg()->GoalMessage(0);
	}
	else if ((int)PlayerScore[1].x == (int)PlayerScore[0].x)
	{
		m_pPlayer[(int)PlayerScore[0].y]->SetWinner(true);
		m_pPlayer[(int)PlayerScore[1].y]->SetWinner(true);
		m_pPlayer[(int)PlayerScore[2].y]->SetWinner(false);
		m_pPlayer[(int)PlayerScore[3].y]->SetWinner(false);

		CApplication::GetMsg()->GoalMessage(0);
	}
	else
	{
		m_pPlayer[(int)PlayerScore[0].y]->SetWinner(true);
		m_pPlayer[(int)PlayerScore[1].y]->SetWinner(false);
		m_pPlayer[(int)PlayerScore[2].y]->SetWinner(false);
		m_pPlayer[(int)PlayerScore[3].y]->SetWinner(false);

		CApplication::GetMsg()->GoalMessage((int)PlayerScore[0].y + 1);
	}
}

//=====================================
// 生成処理
//=====================================
CStage* CStage::Create(void)
{
	CStage* pStage = new CStage;

	if (FAILED(pStage->Init()))
	{
		return nullptr;
	}

	return pStage;
}

//=====================================
//読み込み処理
//=====================================
void CStage::Load()
{
	char aStr[256] = {};		//読み込む用文字列
								//ファイルを開く
	FILE* pFile = fopen(m_pStagePass[(StageType)CApplication::GetStageSelect()], "r");

	if (pFile != nullptr)
	{//ファイルが開いた場合
		fscanf(pFile, "%s", aStr);

		while (strncmp(aStr, "END_SCRIPT", 10) != 0)
		{//文字列の初期化と読み込み
			fscanf(pFile, "%s", aStr);

			if (strncmp(aStr, "FIELDALLSET", 11) == 0)
			{// 生成モデル読み込み
				int nField = 0;
				while (strncmp(aStr, "END_FIELDALLSET", 15) != 0)
				{
					fscanf(pFile, "%s", aStr);
					if (strncmp(aStr, "FIELDSET", 8) == 0)
					{// メッシュフィールドメッシュ読み込み
						while (strncmp(aStr, "END_FIELDSET", 12) != 0)
						{
							fscanf(pFile, "%s", aStr);
							if (strncmp(aStr, "FIELD", 5) == 0)
							{
								while (strncmp(aStr, "END_FIELD", 9) != 0)
								{
									fscanf(pFile, "%s", aStr);
									if (strncmp(aStr, "LINE", 4) == 0)
									{
										fscanf(pFile, "%s", aStr);
										fscanf(pFile, "%s", aStr);

										std::string s = aStr;			// std::stringに変換する
										int nLine = std::stoi(s);		//intに変換する

										if (nField == 0)
										{
											// メッシュフィールドの生成
											m_pField[nField] = CMeshfield::Create(D3DXVECTOR3(-135.0f, -150.0f, 1100.0f), Vec3Null, D3DXVECTOR2(30.0f, 70.0f), nLine, 10, 3);
										}
										else
										{
											// メッシュフィールドの生成
											m_pField[nField] = CMeshfield::Create(D3DXVECTOR3(-10000.0f, -2000.0f, 10000.0f), Vec3Null, D3DXVECTOR2(20000.0f, 20000.0f), nLine, nLine, 3);
										}
									}
									else if (strncmp(aStr, "TEXTURE_TILING", 14) == 0)
									{//この後にコメント
										fscanf(pFile, "%s", aStr);
										fscanf(pFile, "%s", aStr);			// メッシュフィールドのタイリングを読み込む処理
										std::string s = aStr;				// std::stringに変換する
										float fTaling = std::stof(s);		//floatに変換する
										m_pField[nField]->SetTextureTiling(fTaling);
									}
								}
							}
						}
						nField++;
					}
				}
			}
			else if (strncmp(aStr, "COINALLSET", 10) == 0)
			{// コイン読み込み
				int nCoinType = 0;
				while (strncmp(aStr, "END_COINALLSET", 14) != 0)
				{
					fscanf(pFile, "%s", aStr);
					if (strncmp(aStr, "COINTYPESET", 11) == 0)
					{
						fscanf(pFile, "%s", aStr);
						while (strncmp(aStr, "END_COINTYPESET", 15) != 0)
						{
							fscanf(pFile, "%s", aStr);
							if (strncmp(aStr, "COINSET", 7) == 0)
							{
								while (strncmp(aStr, "END_COINSET", 11) != 0)
								{
									fscanf(pFile, "%s", aStr);
									if (strncmp(aStr, "COIN", 4) == 0)
									{
										while (strncmp(aStr, "END_COIN", 8) != 0)
										{
											fscanf(pFile, "%s", aStr);
											if (strncmp(aStr, "POS", 3) == 0)
											{
												fscanf(pFile, "%s", aStr);
												fscanf(pFile, "%s", aStr);	//X座標の読み込む処理
												std::string s = aStr;		//std::stringに変換する
												float x = std::stof(s);		//floatに変換する

												fscanf(pFile, "%s", aStr);	//Y座標の読み込む処理
												s = aStr;					//std::stringに変換する
												float y = std::stof(s);		//floatに変換する

												fscanf(pFile, "%s", aStr);	//Z座標の読み込む処理
												s = aStr;					//std::stringに変換する
												float z = std::stof(s);		//floatに変換する

												CCoin* pCoin = CCoin::Create(D3DXVECTOR3(x, y, z), (CCoin::COIN_TYPE)nCoinType);
												pCoin->SetShadowHeight(-149.8f);
											}
										}
									}
								}
							}
						}
						nCoinType++;
					}
				}
			}
			else if (strncmp(aStr, "MODELALLSET", 11) == 0)
			{// 障害物モデル読み込み
				int nModelType = 0;
				while (strncmp(aStr, "END_MODELALLSET", 15) != 0)
				{
					fscanf(pFile, "%s", aStr);
					if (strncmp(aStr, "MODELTYPESET", 12) == 0)
					{
						fscanf(pFile, "%s", aStr);
						while (strncmp(aStr, "END_MODELTYPESET", 16) != 0)
						{
							fscanf(pFile, "%s", aStr);
							if (strncmp(aStr, "MODELSET", 8) == 0)
							{
								while (strncmp(aStr, "END_MODELSET", 12) != 0)
								{
									fscanf(pFile, "%s", aStr);
									if (strncmp(aStr, "MODEL", 5) == 0)
									{
										while (strncmp(aStr, "END_MODEL", 9) != 0)
										{
											fscanf(pFile, "%s", aStr);
											if (strncmp(aStr, "POS", 3) == 0)
											{
												fscanf(pFile, "%s", aStr);
												fscanf(pFile, "%s", aStr);	//X座標の読み込む処理
												std::string s = aStr;		//std::stringに変換する
												float x = std::stof(s);		//floatに変換する

												fscanf(pFile, "%s", aStr);	//Y座標の読み込む処理
												s = aStr;					//std::stringに変換する
												float y = std::stof(s);		//floatに変換する

												fscanf(pFile, "%s", aStr);	//Z座標の読み込む処理
												s = aStr;					//std::stringに変換する
												float z = std::stof(s);		//floatに変換する

												SetModelType(D3DXVECTOR3(x, y, z), (ModelType)nModelType);
											}
										}
									}
								}
							}
						}
						nModelType++;
					}
				}
			}
			else if (strncmp(aStr, "FLOORALLSET", 11) == 0)
			{// 床モデル読み込み
				int nFloorType = 0;
				while (strncmp(aStr, "END_FLOORALLSET", 15) != 0)
				{
					fscanf(pFile, "%s", aStr);
					if (strncmp(aStr, "FLOORTYPESET", 12) == 0)
					{
						fscanf(pFile, "%s", aStr);
						while (strncmp(aStr, "END_FLOORTYPESET", 16) != 0)
						{
							fscanf(pFile, "%s", aStr);
							if (strncmp(aStr, "FLOORSET", 8) == 0)
							{
								while (strncmp(aStr, "END_FLOORSET", 12) != 0)
								{
									fscanf(pFile, "%s", aStr);
									if (strncmp(aStr, "FLOOR", 5) == 0)
									{
										while (strncmp(aStr, "END_FLOOR", 9) != 0)
										{
											fscanf(pFile, "%s", aStr);
											if (strncmp(aStr, "POS", 3) == 0)
											{
												fscanf(pFile, "%s", aStr);
												fscanf(pFile, "%s", aStr);	//X座標の読み込む処理
												std::string s = aStr;		//std::stringに変換する
												float x = std::stof(s);		//floatに変換する

												fscanf(pFile, "%s", aStr);	//Y座標の読み込む処理
												s = aStr;					//std::stringに変換する
												float y = std::stof(s);		//floatに変換する

												fscanf(pFile, "%s", aStr);	//Z座標の読み込む処理
												s = aStr;					//std::stringに変換する
												float z = std::stof(s);		//floatに変換する

												SetFloorType(D3DXVECTOR3(x, y, z), (FloorType)nFloorType);
											}
										}
									}
								}
							}
						}
						nFloorType++;
					}
				}
			}
			else if (strncmp(aStr, "SPAWNERALLSET", 13) == 0)
			{// 生成モデル読み込み
				int nSpawnerType = 0;
				while (strncmp(aStr, "END_SPAWNERALLSET", 17) != 0)
				{
					fscanf(pFile, "%s", aStr);
					if (strncmp(aStr, "SPAWNERTYPESET", 14) == 0)
					{
						fscanf(pFile, "%s", aStr);
						while (strncmp(aStr, "END_SPAWNERTYPESET", 18) != 0)
						{
							fscanf(pFile, "%s", aStr);
							if (strncmp(aStr, "SPAWNERSET", 10) == 0)
							{
								while (strncmp(aStr, "END_SPAWNERSET", 14) != 0)
								{
									fscanf(pFile, "%s", aStr);
									if (strncmp(aStr, "SPAWNER", 7) == 0)
									{
										int nCollDown = 0;
										float x, y, z, fWidth, fLength = {};
										while (strncmp(aStr, "END_SPAWNER", 11) != 0)
										{
											fscanf(pFile, "%s", aStr);
											if (strncmp(aStr, "POS", 3) == 0)
											{
												fscanf(pFile, "%s", aStr);
												fscanf(pFile, "%s", aStr);	//X座標の読み込む処理
												std::string s = aStr;		//std::stringに変換する
												x = std::stof(s);		//floatに変換する

												fscanf(pFile, "%s", aStr);	//Y座標の読み込む処理
												s = aStr;					//std::stringに変換する
												y = std::stof(s);		//floatに変換する

												fscanf(pFile, "%s", aStr);	//Z座標の読み込む処理
												s = aStr;					//std::stringに変換する
												z = std::stof(s);		//floatに変換する
											}
											else if (strncmp(aStr, "WIDTH", 5) == 0)
											{
												fscanf(pFile, "%s", aStr);
												fscanf(pFile, "%s", aStr);	//横長さの読み込む処理
												std::string s = aStr;		//std::stringに変換する
												fWidth = std::stof(s);		//floatに変換する
											}
											else if (strncmp(aStr, "LENGTH", 6) == 0)
											{
												fscanf(pFile, "%s", aStr);
												fscanf(pFile, "%s", aStr);	//距離の長さの読み込む処理
												std::string s = aStr;		//std::stringに変換する
												fLength = std::stof(s);		//floatに変換する
											}
											else if (strncmp(aStr, "COLL", 4) == 0)
											{
												fscanf(pFile, "%s", aStr);
												fscanf(pFile, "%s", aStr);	//クールタイムの長さの読み込む処理
												std::string s = aStr;		//std::stringに変換する
												nCollDown = std::stoi(s);	//intに変換する
											}
										}
										SetSpawnerType(D3DXVECTOR3(x, y, z), fWidth, fLength, nCollDown, (SpawnerType)nSpawnerType);
									}
								}
							}
						}
						nSpawnerType++;
					}
				}
			}
			else if (strncmp(aStr, "ENVIRONMENTALLSET", 17) == 0)
			{// 環境モデル読み込み
				int nEnvironmentType = 0;
				while (strncmp(aStr, "END_ENVIRONMENTALLSET", 21) != 0)
				{
					fscanf(pFile, "%s", aStr);
					if (strncmp(aStr, "ENVIRONMENTTYPESET", 18) == 0)
					{
						fscanf(pFile, "%s", aStr);
						while (strncmp(aStr, "END_ENVIRONMENTTYPESET", 22) != 0)
						{
							fscanf(pFile, "%s", aStr);
							if (strncmp(aStr, "ENVIRONMENTSET", 14) == 0)
							{
								while (strncmp(aStr, "END_ENVIRONMENTSET", 18) != 0)
								{
									fscanf(pFile, "%s", aStr);
									if (strncmp(aStr, "ENVIRONMENT", 11) == 0)
									{
										while (strncmp(aStr, "END_ENVIRONMENT", 15) != 0)
										{
											fscanf(pFile, "%s", aStr);
											if (strncmp(aStr, "POS", 3) == 0)
											{
												fscanf(pFile, "%s", aStr);
												fscanf(pFile, "%s", aStr);	//X座標の読み込む処理
												std::string s = aStr;		//std::stringに変換する
												float x = std::stof(s);		//floatに変換する

												fscanf(pFile, "%s", aStr);	//Y座標の読み込む処理
												s = aStr;					//std::stringに変換する
												float y = std::stof(s);		//floatに変換する

												fscanf(pFile, "%s", aStr);	//Z座標の読み込む処理
												s = aStr;					//std::stringに変換する
												float z = std::stof(s);		//floatに変換する

												CEnvironment::Create(D3DXVECTOR3(x, y, z), (CEnvironment::EnvironmentType)nEnvironmentType);
											}
										}
									}
								}
							}
						}
						nEnvironmentType++;
					}
				}
			}
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}