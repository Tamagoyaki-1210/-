//==============================================================================
//
// Map.cpp
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// インクルード
//==============================================================================
#include "application.h"
#include "renderer.h"
#include "map.h"
#include "objectX.h"
#include "3Dpolygon.h"
#include "gimmick_model.h"

//==============================================================================================
// 定義
//==============================================================================================
const char* CMap::MAP_OBSTACLE_OBJECT_FILENAME[] =
{// マップ障害物オブジェクトファイルのパス
	"data/text/map_data/map_obstacle_object_data_00.txt",						// マップ１
	"data/text/map_data/map_obstacle_object_data_01.txt",						// マップ２
	"data/text/map_data/map_obstacle_object_data_02.txt",						// マップ３
};

const char* CMap::MAP_BACKGROUND_OBJECT_FILENAME[] =
{// マップ背景オブジェクトファイルのパス
	"data/text/map_data/map_background_object_data_00.txt",						// マップ１
	"data/text/map_data/map_background_object_data_01.txt",						// マップ２
	"data/text/map_data/map_background_object_data_02.txt",						// マップ３
};

const float CMap::Map_Collision_Decreace = 10.0f;		// マップオブジェクトの当たり判定を小さくする値
CTrolley_Manager *CMap::m_pTrolley_Manager = nullptr;

//==============================================================================
// コンストラクタ
//==============================================================================
CMap::CMap()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CMap::~CMap()
{
}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CMap::Init(int nMopNum)
{
	m_pFloor = C3DPolygon::Create(D3DXVECTOR3(800.0f,0.0f,800.0f), D3DXVECTOR3(D3DXToRadian(0), 0.0f, 0.0f),D3DXVECTOR2(2000.0f, 2000.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
	
	//各ステージごとに地面のテクスチャを変える
	switch (nMopNum)
	{
	case 0:
		m_pFloor->SetTextIndex(CTexture::TEXTURE_FLOOR);
		break;
	case 1:
		m_pFloor->SetTextIndex(CTexture::TEXTURE_JUNGLE);
		break;
	case 2:
		m_pFloor->SetTextIndex(CTexture::TEXTURE_MOON);
		break;
	default:
		assert(false);
		break;
	}

	//マップ番号の保存
	m_nMapNum = nMopNum;
	// マップ読み込み
	LoadMap(true, MAP_OBSTACLE_OBJECT_FILENAME[m_nMapNum]);
	LoadMap(false, MAP_BACKGROUND_OBJECT_FILENAME[m_nMapNum]);

	// トロッコのルートの設定
	Trolley_Route();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CMap::Uninit()
{
	//リストの削除
	m_MapObjectList.clear();

	// トロッコマネージャーの終了
	if (m_pTrolley_Manager != nullptr)
	{
		m_pTrolley_Manager->Uninit();
		m_pTrolley_Manager = nullptr;
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void CMap::Update()
{
	
}

//==============================================================================
// 描画処理
//==============================================================================
void CMap::Draw()
{
	
}

//==============================================================================
// マップ内のオブジェクトとの当たり判定
//==============================================================================
D3DXVECTOR3 CMap::CollisionMapObject(const D3DXVECTOR3 pos, const float radius)
{
	// 押し戻す値
	D3DXVECTOR3 add = {0.0f,0.0f,0.0f};

	// イテレーターループ
	for (auto pObjectX : m_MapObjectList)
	{
		// マップ内のオブジェクトのポインタの解放
		if (pObjectX == nullptr)
		{
			assert(false);
		}

		// マップ内のオブジェクトの情報
		const D3DXVECTOR3 atherPos = pObjectX->GetPos();
		const float atherRadius = pObjectX->GetMax().x - Map_Collision_Decreace;

		// 衝突検知
		if (pObjectX->CollisionCircle(pos, radius, atherPos, atherRadius))
		{
			// ベクトルの算出
			D3DXVECTOR3 vec = pos - atherPos;

			// ベクトルの方向を計算
			D3DXVECTOR3 vecNolmalize;
			D3DXVec3Normalize(&vecNolmalize, &vec);

			// マップオブジェクトとキャラの大きさのベクトル * ベクトルの方向
			D3DXVECTOR3 vec2 = {(atherRadius + radius) * vecNolmalize.x, 0.0f, (atherRadius + radius) * vecNolmalize.z };

			// 加算する
			add += vec2 - vec;
		}
	}

	return add;
}

//==============================================================================
// マップ読み込み
//==============================================================================
void CMap::LoadMap(bool bCollision, const char* s_FileName)
{
	FILE *pFile = NULL;			//ファイルポインター宣言

	char cBff[LINE_MAX_READING_LENGTH];		//一行分読み取るための変数
	char cBffHead[LINE_MAX_READING_LENGTH];	//頭の文字を読み取るための変数

	char cListBffFile[128][LINE_MAX_READING_LENGTH];	//ファイル名の読み取り用

	//登録したモデルの数
	int nRegistrationModel = 0;

	//基準位置
	D3DXVECTOR3 MapPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//モデルのサイズ
	D3DXVECTOR3 ModelSize = D3DXVECTOR3(100.0f, 0.0f, 100.0f);

	//ファイルを開く
	pFile = fopen(s_FileName, "r");

	if (pFile == nullptr)
	{//開けなかった時用
		assert(false);
	}

	//文字列の読み取りループ処理
	while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != nullptr)
	{
		//文字列の分析
		sscanf(cBff, "%s", &cBffHead);

		if (strcmp(&cBffHead[0], "NUM_MODEL") == 0)
		{//読み込むモデル数の読み込み

			//文字列の分析
			sscanf(cBff, "%s = %d", &cBffHead, &m_nMapTypeMax);
		}
		else if (strcmp(&cBffHead[0], "MODEL_FILENAME") == 0)
		{//読み込むモデル数の読み込み

		 //登録予定数より多くなったエラー
			if (nRegistrationModel >= m_nMapTypeMax)
			{
				assert(false);
			}

			//文字列の分析
			sscanf(cBff, "%s = %s", &cBffHead, &cListBffFile[nRegistrationModel]);

			//登録完了
			nRegistrationModel++;

		}
		else if (strcmp(&cBffHead[0], "MAP_NUM_MAX") == 0)
		{//読み込むマップのサイズ
			//文字列の分析
			sscanf(cBff, "%s = %d", &cBffHead, &m_nMapSizeMax);

		}
		else if (strcmp(&cBffHead[0], "MAP_XZ") == 0)
		{//読み込むマップのデータ

		 //登録したマップ番号数
			int nSetMapNum = 0;

			//文字列の読み取りループ処理
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != nullptr)
			{
				//チェック
				if (nSetMapNum >= m_nMapSizeMax * m_nMapSizeMax)
				{
					break;
				}

				//一時保存
				int nMapNum = 0;
				D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				//文字列の分析
				sscanf(cBff, "%d %f %f %f", &nMapNum ,&rot.x ,&rot.y ,&rot.z);

				//オブジェクトがある場合
				if (nMapNum != 0)
				{
					//情報の登録
	
					CObjectX* pObjectX = CObjectX::Create(MapPos, rot, { 1.0f,1.0f,1.0f }, cListBffFile[nMapNum]);

					//当たりのある障害物にするか、無い背景用にするか
					if (bCollision)
					{
						//リストに情報を追加
						m_MapObjectList.push_back(pObjectX);
					}
				}

				//位置の更新
				if ((nSetMapNum % m_nMapSizeMax) == m_nMapSizeMax - 1)
				{
					MapPos.x += ModelSize.x;
					MapPos.z = 0.0f;
				}
				else
				{
					MapPos.z += ModelSize.z;
				}

				//登録した番号
				nSetMapNum++;
			}
		}
		else if (strcmp(&cBffHead[0], "END_SCRIPT") == 0)
		{//スクリプトの終わり
			break;
		}

		//保存中の文字列の初期化
		ZeroMemory(&cBff, sizeof(cBff));
		ZeroMemory(&cBffHead, sizeof(cBffHead));
	}

	//ファイルを閉じる
	fclose(pFile);
}

//==============================================================================
// トロッコのルートの設定
//==============================================================================
void CMap::Trolley_Route()
{
	// トロッコマネージャー
	switch (m_nMapNum)
	{
	case 0:
		// ステージ1
		m_pTrolley_Manager = CTrolley_Manager::Create(1, 0, "Data/model/Motion_Gimmick/motion_gimmick.txt");
		break;
	case 1:
		// ステージ2
		m_pTrolley_Manager = CTrolley_Manager::Create(2, 0, "Data/model/Motion_Triceratops/motion_triceratops.txt");
		break;
	case 2:
		// ステージ3
		m_pTrolley_Manager = CTrolley_Manager::Create(0, 0, "Data/model/Motion_Space_Probe/motion_space_probe.txt");
		break;
	default:
		break;
	}

	// トロッコマネージャーの確立に成功した場合
	if (m_pTrolley_Manager != nullptr)
	{
		// トロッコの情報を設定する
		m_pTrolley_Manager->GetGimmick_Model()->SetTrolleyInfomation();
	}
}