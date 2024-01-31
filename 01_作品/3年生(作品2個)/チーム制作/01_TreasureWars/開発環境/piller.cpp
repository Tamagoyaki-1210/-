//==============================================================================
//
// piller.cpp	(柱のオブジェクト.cpp)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// インクルード
//==============================================================================
#include <time.h>
#include "application.h"
#include "renderer.h"
#include "piller.h"
#include "objectX.h"
#include "ore.h"
#include "ranking.h"
#include "score.h"
#include "result.h"
#include"character.h"
#include "charmanager.h"
#include "stage_select.h"

//==============================================================================
// 静的メンバ変数宣言
//==============================================================================
const float CPiller::MOVING_PILLERSPEED = 3.0f;
int CPiller::m_nScore = 0;
int CPiller::m_anScore[4] = {};

//==============================================================================
// コンストラクタ
//==============================================================================
CPiller ::CPiller()
{

}


//==============================================================================
// デストラクタ
//==============================================================================
CPiller::~CPiller()
{

}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CPiller::Init()
{
	//objectXの初期化処理
	CObjectX::Init();

	//==================================================
	// メンバ変数の初期化
	//==================================================
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, MOVING_PILLERSPEED, 0.0f);	
	m_no1 = 0;

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CPiller::Uninit()
{
	//objectXの終了処理
	CObjectX::Uninit();
}

//==============================================================================
// 更新処理
//==============================================================================
void CPiller::Update()
{
	//位置の取得
	D3DXVECTOR3 pos = GetPos();

	for (int i = 0; i < 4; i++)
	{
		//最大値
		if (m_no1 < m_anScore[i])
		{//スコアが0より大きかったら
			m_no1 = m_anScore[i];			//最大値格納変数そのスコアを格納
		}
		if (m_no1 == m_anScore[i] && pos.y >= CResult::Moving_Piller[0])
		{
			// 終了フラグを真にする
			CResult::SetFlag(true);
		}
	}

	// 指定された位置より下の場合
	if (pos.y <= m_fMaxMoving)
	{
		pos.y += m_move.y;
	}
	//  指定された位置以上の場所に達した場合
	else if (pos.y >= m_fMaxMoving)
	{
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	//位置の設定
	SetPos(pos);

	// キャラの移動
	m_pResult_Char->SetPos(pos);
	
	//objectXの更新処理
	CObjectX::Update();
}

//==============================================================================
// 生成処理
//==============================================================================
CPiller *CPiller::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, int index, float moving, int num)
{
	CPiller *pPiller = nullptr;

	pPiller = new CPiller;

	if (pPiller != nullptr)
	{
		pPiller->SetPos(pos);
		pPiller->SetRot(rot);
		pPiller->SetSize(size);
		pPiller->SetIndex(index);
		pPiller->m_fMaxMoving = moving;	// 縦の最大値の設定

		// リザルト用のキャラ
		pPiller->m_pResult_Char = CApplication::GetCharManager()->EntryChara(pos, CCharManager::TYPE_RESULT_CHAR, num, CStage_Select::GetStage_Number());
		pPiller->Init();
	}

	return pPiller;
}
