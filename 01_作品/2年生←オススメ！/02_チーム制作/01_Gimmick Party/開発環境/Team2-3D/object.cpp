//=============================================================================
//
// object.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object.h"
#include "application.h"
#include <random>

//=============================================================================
//静的メンバー変数の宣言 
//=============================================================================
CObject* CObject::m_pTop[CObject::Max_Priority] = {};				//先頭のオブジェクトへのポイン
CObject* CObject::m_pCurrent[CObject::Max_Priority] = {};			//現在(一番後ろ)のオブジェクトへのポンタ

//=============================================================================
//コンストラクタ
//=============================================================================
CObject::CObject()
{
	if (m_pCurrent[2] != nullptr)
	{//オブジェクトがもうあったら
		m_pCurrent[2]->m_pNext = this;		//前のオブジェクトの次のオブジェクトへのポインタを設定する
		m_pPrev = m_pCurrent[2];			//前のオブジェクトへのポインタの設定
		m_pCurrent[2] = this;				//現在のオブジェクトの更新
	}
	else
	{//まだオブジェクトがなかったら
		m_pTop[2] = this;					//先頭のオブジェクトへのポインをこのインスタンスのアドレスにする
		m_pCurrent[2] = this;				//現在(一番後ろ)のオブジェクトへのポンタをこのインスタンスのアドレスにする
		m_pPrev = nullptr;					//前のオブジェクトへのポインタをnullに設定する
	}

	m_pNext = nullptr;						//次のオブジェクトへのポインタをnullにする

	m_nPriority = 3;						//プライオリティを3に設定する
	m_bDeath = false;						//死亡フラグをfalseに設定する
}

CObject::CObject(int nPriority)
{
	if (m_pCurrent[nPriority - 1] != nullptr)
	{//オブジェクト既に存在する場合
		m_pCurrent[nPriority - 1]->m_pNext = this;		//前のオブジェクトの次のオブジェクトへのポインタを設定する	
		m_pPrev = m_pCurrent[nPriority - 1];			//前のオブジェクトへのポインタの設定
		m_pCurrent[nPriority - 1] = this;				//現在のオブジェクトの更新
	}
	else
	{//これは最初のオブジェクトだったら	
		m_pTop[nPriority - 1] = this;					//先頭のオブジェクトへのポインをこのインスタンスのアドレスにする
		m_pCurrent[nPriority - 1] = this;				//現在(一番後ろ)のオブジェクトへのポンタをこのインスタンスのアドレスにする
		m_pPrev = nullptr;								//前のオブジェクトへのポインタをnullに設定する
	}

	m_pNext = nullptr;									//次のオブジェクトへのポインタをnullにする

	m_nPriority = nPriority;							//死亡フラグをfalseに設定する
}														

//=============================================================================
//デストラクタ
//=============================================================================
CObject::~CObject()
{

}

//オブジェクトを消す処理
void CObject::Release(void)
{
	Uninit();				//終了処理
	m_bDeath = true;		//死亡フラグをtrueにする
}

//プライオリティの設定処理
void CObject::SetPriority(int nPriority)
{
	//プライオリティが範囲外ではないように確認する
	if (nPriority < 1)
	{
		nPriority = 1;
	}
	else if(nPriority > 5)
	{
		nPriority = 5;
	}
	
	if (m_pPrev != nullptr)
	{//前のオブジェクトへのポインタがnullではなかったら
		if (m_pNext != nullptr)
		{//前のオブジェクトへのポインタがnullではなかったら
			//前のオブジェクトと次のオブジェクトを繋ぐ
			m_pPrev->m_pNext = m_pNext;			
			m_pNext->m_pPrev = m_pPrev;			
		}
		else
		{//このオブジェクトが一番後ろだったら、前のオブジェクトを一番後ろにする
			m_pPrev->m_pNext = nullptr;					
			m_pCurrent[m_nPriority - 1] = m_pPrev;		
		}
	}
	else
	{//前のオブジェクトへのポインタがnullだったら
		if (m_pNext != nullptr)
		{//次のオブジェクトへのポインタがnullではなかったら、次のオブジェクトを先頭にする
			m_pNext->m_pPrev = nullptr;
			m_pTop[m_nPriority - 1] = m_pNext;
		}
		else
		{//このオブジェクトしかなかったら、静的変数をnullにする
			m_pTop[m_nPriority - 1] = nullptr;
			m_pCurrent[m_nPriority - 1] = nullptr;
		}
	}

	m_nPriority = nPriority;			//プライオリティの設定処理

	//新しいプライオリティの配列に入れる
	if (m_pCurrent[nPriority - 1] != nullptr)
	{//このプライオリティの配列にオブジェクトが既にあったら、一番後ろのオブジェクトとして入れる
		m_pCurrent[nPriority - 1]->m_pNext = this;		
		m_pPrev = m_pCurrent[nPriority - 1];			
		m_pCurrent[nPriority - 1] = this;				
	}
	else
	{//このプライオリティの配列の最初のオブジェクトだったら、普通に入れる
		m_pTop[nPriority - 1] = this;
		m_pCurrent[nPriority - 1] = this;
		m_pPrev = nullptr;
	}

	m_pNext = nullptr;			//次のオブジェクトへのポインタをnullにする
}

//===============================================================================
//
//								静的関数
//
//===============================================================================

//=============================================================================
//全部の終了処理
//=============================================================================
void CObject::ReleaseAll(void)
{
	for (int nCnt = 0; nCnt < Max_Priority; nCnt++)
	{
		if (m_pTop[nCnt] != nullptr)
		{
			CObject* pCurrent = m_pTop[nCnt];

			while (pCurrent != nullptr)
			{
				CObject* pNext = pCurrent->m_pNext;

				if (pCurrent->m_pPrev != nullptr)
				{
					if (pCurrent->m_pNext != nullptr)
					{//リストの中だったら
						pCurrent->m_pPrev->m_pNext = pCurrent->m_pNext;
						pCurrent->m_pNext->m_pPrev = pCurrent->m_pPrev;
					}
					else
					{//リストの最後のオブジェクトだったら
						pCurrent->m_pPrev->m_pNext = nullptr;
						m_pCurrent[pCurrent->m_nPriority - 1] = pCurrent->m_pPrev;
					}
				}
				else
				{
					if (pCurrent->m_pNext != nullptr)
					{//リストの最初のオブジェクトだったら
						pCurrent->m_pNext->m_pPrev = nullptr;
						m_pTop[pCurrent->m_nPriority - 1] = pCurrent->m_pNext;
					}
					else
					{//このオブジェクトしかない場合
						m_pTop[pCurrent->m_nPriority - 1] = nullptr;
						m_pCurrent[pCurrent->m_nPriority - 1] = nullptr;
					}
				}

				pCurrent->Uninit();			//終了処理
				delete pCurrent;			//メモリを解放する

				pCurrent = pNext;
			}
		}
	}
}

//=============================================================================
//全部の更新処理
//=============================================================================
void CObject::UpdateAll(void)
{
	for (int nCnt = 0; nCnt < Max_Priority; nCnt++)
	{
		if (m_pTop[nCnt] != nullptr)
		{
			CObject* pCurrent = m_pTop[nCnt];

			while (pCurrent != nullptr)
			{
				CObject* pNext = pCurrent->m_pNext;

				if (!pCurrent->m_bDeath)
				{
					if (CApplication::GetPause() == false)
					{// ポーズ中ではない場合
						pCurrent->Update();
					}
					else if(pCurrent->m_nPriority >= 4)
					{// UIは動かす
						pCurrent->Update();
					}
				}
				pCurrent = pNext;
			}
		}
	}

	for (int nCnt = 0; nCnt < Max_Priority; nCnt++)
	{
		if (m_pTop[nCnt] != nullptr)
		{
			CObject* pCurrent = m_pTop[nCnt];

			while (pCurrent != nullptr)
			{

				CObject* pNext = pCurrent->m_pNext;

				if (pCurrent->m_bDeath)
				{
					if (pCurrent->m_pPrev != nullptr)
					{
						if (pCurrent->m_pNext != nullptr)
						{//リストの中だったら
							pCurrent->m_pPrev->m_pNext = pCurrent->m_pNext;
							pCurrent->m_pNext->m_pPrev = pCurrent->m_pPrev;
						}
						else
						{//リストの最後のオブジェクトだったら
							pCurrent->m_pPrev->m_pNext = nullptr;
							m_pCurrent[pCurrent->m_nPriority - 1] = pCurrent->m_pPrev;
						}
					}
					else
					{
						if (pCurrent->m_pNext != nullptr)
						{//リストの最初のオブジェクトだったら
							pCurrent->m_pNext->m_pPrev = nullptr;
							m_pTop[pCurrent->m_nPriority - 1] = pCurrent->m_pNext;
						}
						else
						{//このオブジェクトしかない場合
							m_pTop[pCurrent->m_nPriority - 1] = nullptr;
							m_pCurrent[pCurrent->m_nPriority - 1] = nullptr;
						}
					}

					//pCurrent->Uninit();
					delete pCurrent;
				}

				pCurrent = pNext;
			}
		}
	}
}

//=============================================================================
//全部の描画処理
//=============================================================================
void CObject::DrawAll(void)
{
	for (int nCnt = 0; nCnt < Max_Priority; nCnt++)
	{
		if (m_pTop[nCnt] != nullptr)
		{
			CObject* pCurrent = m_pTop[nCnt];

			while (pCurrent != nullptr)
			{
				if (!pCurrent->m_bDeath)
				{
					pCurrent->Draw();
				}

				pCurrent = pCurrent->m_pNext;
			}
		}
	}
}

//=============================================================================
//生成処理
//=============================================================================

//オブジェクトの取得処理
CObject** CObject::GetObj(void)
{
	return &m_pTop[0];
}

std::random_device rd;
std::mt19937 gen(rd());

//乱数を返す処理
int CObject::random(const int low, const int high)
{
	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);
}