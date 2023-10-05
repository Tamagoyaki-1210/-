//=============================================================================
//
// animator.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "animator.h"
#include "modelPart.h"
#include <string>



//=============================================================================
//静的変数の初期化処理
//=============================================================================

//アニメーション情報のテキストファイルの相対パス
char* CAnimator::m_pAnimPass[ANIM_TYPE_MAX] =
{
	{"data\\ANIMATION\\PlayerAnim.txt"},
};

std::vector <CAnimator::ModelAnimationSet> CAnimator::m_vAnimAll;			//全部のアニメーション情報を持っているベクトル

//コンストラクタ
CAnimator::CAnimator()
{
	//メンバー変数をクリアする
	m_vParts.clear();					//パーツのベクトルをクリアする
	m_vAnimSpeed.clear();				//アニメーションスピードのベクトルをクリアする
	m_nPresentFrame = 0;				//現在のフレーム
	m_nPresentMotionSet = 0;			//現在のモーションセット
	m_nPresentAnim = 0;					//現在のアニメーション
	m_type = (AnimType)0;				//アニメーションの種類
}

//デストラクタ
CAnimator::~CAnimator()
{

}

//初期化処理
HRESULT CAnimator::Init(void)
{
	//メンバー変数の初期化処理
	m_vParts.clear();					//パーツのベクトルをクリアする
	m_vAnimSpeed.clear();				//アニメーションスピードのベクトルをクリアする
	m_nPresentFrame = 0;				//現在のフレーム
	m_nPresentMotionSet = 0;			//現在のモーションセット
	m_nPresentAnim = 0;					//現在のアニメーション
	m_type = (AnimType)0;				//アニメーションの種類

	return S_OK;
}

//終了処理
void CAnimator::Uninit(void)
{
	//ベクトルをクリアする
	m_vParts.clear();
	m_vAnimSpeed.clear();
}

//更新処理
void CAnimator::Update(void)
{
	m_nPresentFrame++;					//現在フレームを更新する

	int nextAnim = m_vAnimAll.data()[m_type].vAnimSets[m_nPresentAnim].nNextAnim;			//次のアニメーション(省略用)

	for (int nCnt = 0; nCnt < (int)m_vParts.size(); nCnt++)
	{//パーツのアニメーション処理
		if (m_vParts.data()[nCnt] != nullptr)
		{//パーツはnullではなかったら
			if (m_vParts.data()[nCnt]->GetParent() != nullptr)
			{
				D3DXVECTOR3 pos = m_vParts.data()[nCnt]->GetPos();			//位置の取得処理
				D3DXVECTOR3 rot = m_vParts.data()[nCnt]->GetRot();			//向きの取得処理
				pos += m_vAnimSpeed.data()[nCnt].move;						//位置の更新処理
				rot += m_vAnimSpeed.data()[nCnt].rotSpeed;					//向きの更新処理
				m_vParts.data()[nCnt]->SetPos(pos);							//位置の設定処理
				m_vParts.data()[nCnt]->SetRot(rot);							//向きの設定処理
			}
		}
	}

	if (m_nPresentFrame > m_vAnimAll.data()[m_type].vAnimSets[m_nPresentAnim].vAnimKeySet[m_nPresentMotionSet].nAnimFrames)
	{//アニメーションのモーションセット最大フレームを越えた場合
		int nNextSet = m_nPresentMotionSet + 1;				//次のモーションセット

		if (nNextSet >= (int)m_vAnimAll.data()[m_type].vAnimSets[m_nPresentAnim].vAnimKeySet.size())
		{//アニメーションは終わった場合、次のアニメーションに進む
			for (int nCnt = 0; nCnt < (int)m_vParts.size(); nCnt++)
			{//パーツ毎のアニメーション速度の計算
				if (m_vParts.data()[nCnt] != nullptr)
				{//パーツがnullではなかったら

					//移動量の計算
					m_vAnimSpeed.data()[nCnt].move = (m_vAnimAll.data()[m_type].vAnimSets[nextAnim].vAnimKeySet[0].vAnimKey[nCnt].RelativePos - m_vParts.data()[nCnt]->GetPos())
						/ (float)m_vAnimAll.data()[m_type].vAnimSets[nextAnim].vAnimKeySet[0].nAnimFrames;

					//回転スピードの計算
					m_vAnimSpeed.data()[nCnt].rotSpeed = (m_vAnimAll.data()[m_type].vAnimSets[nextAnim].vAnimKeySet[0].vAnimKey[nCnt].RelativeRot - m_vParts.data()[nCnt]->GetRot())
						/ (float)m_vAnimAll.data()[m_type].vAnimSets[nextAnim].vAnimKeySet[0].nAnimFrames;
				}
			}

			//次のアニメーションの最初のモーションセットを設定する
			m_nPresentAnim = nextAnim;			
			m_nPresentMotionSet = 0;			
		}
		else
		{//アニメーションはまだ終わっていない場合、次のモーションセットに進む
			for (int nCnt = 0; nCnt < (int)m_vParts.size(); nCnt++)
			{//パーツ毎のアニメーション速度の計算
				if (m_vParts.data()[nCnt] != nullptr)
				{//パーツがnullではなかったら

				 //移動量の計算
					m_vAnimSpeed.data()[nCnt].move = (m_vAnimAll.data()[m_type].vAnimSets[m_nPresentAnim].vAnimKeySet[m_nPresentMotionSet + 1].vAnimKey[nCnt].RelativePos - m_vParts.data()[nCnt]->GetPos())
						/ (float)m_vAnimAll.data()[m_type].vAnimSets[m_nPresentAnim].vAnimKeySet[0].nAnimFrames;

					//回転スピードの計算
					m_vAnimSpeed.data()[nCnt].rotSpeed = (m_vAnimAll.data()[m_type].vAnimSets[m_nPresentAnim].vAnimKeySet[m_nPresentMotionSet + 1].vAnimKey[nCnt].RelativeRot - m_vParts.data()[nCnt]->GetRot())
						/ (float)m_vAnimAll.data()[m_type].vAnimSets[m_nPresentAnim].vAnimKeySet[m_nPresentMotionSet + 1].nAnimFrames;
				}
			}

			m_nPresentMotionSet++;			//次のモーションセットに進む
		}
		m_nPresentFrame = 0;				//フレームを0に戻す
	}
}

//モデルパーツの設定処理
void CAnimator::SetParts(std::vector <CModelPart*>* vParts)
{
	m_vParts = *vParts;
}





//全部のアニメーションを読み込む処理
void CAnimator::LoadAllAnimation(void)
{
	m_vAnimAll.clear();			//ベクトルをクリアする

	for (int nCnt = 0; nCnt < ANIM_TYPE_MAX; nCnt++)
	{//全部のアニメーションを読み込む
		if (m_pAnimPass[nCnt] != nullptr)
		{//相対パスはnullではなかったら
			FILE* pFile = fopen(m_pAnimPass[nCnt], "r");		//ファイルを開く

			if (pFile != nullptr)
			{//開けたら
				ModelAnimationSet set;							//新しいモーションセットを宣言する
				ZeroMemory(&set, sizeof(ModelAnimationSet));	//モーションセットを初期化する
				int nAnimSet = -1;								//現在のアニメーションカウンターを初期化する
				int nKeySet = -1;								//現在のキーセットを初期化する
				int nKey = -1;									//現在のキーを初期化する
				char pStr[512] = {};							//文字列を宣言する

				fscanf(pFile, "%s", pStr);						

				while (strcmp(pStr, "END_SCRIPT") != 0)
				{//スクリプトの終わりまで進む
					fscanf(pFile, "%s", pStr);

					if (strcmp(pStr, "MOTION_SET") == 0)
					{//モーションセットを読み込む処理
						MotionSet motionSet;							//新しいモーションセットを宣言する
						ZeroMemory(&motionSet, sizeof(MotionSet));		//モーションセットの初期化処理
						nAnimSet++;										//モーションセットのカウンターをインクリメントする
						nKeySet = -1;									//キーセットを元に戻す

						while (strcmp(pStr, "END_MOTION_SET") != 0)
						{//モーションセットの読み込みが終わるまで進む
							fscanf(pFile, "%s", pStr);

							if (strcmp(pStr, "NEXT") == 0)
							{//次のアニメーションの読み込み
								int nNext = 0;
								fscanf(pFile, "%s %d", pStr, &nNext);

								motionSet.nNextAnim = nNext;		//次のアニメーションを設定する
							}
							else if (strcmp(pStr, "KEY_SET") == 0)
							{//キーセットの読み込む処理
								AnimKeySet keySet;							//新しいキーセットを宣言する
								ZeroMemory(&keySet, sizeof(AnimKeySet));	//キーセットを初期化する
								nKeySet++;									//キーセットをインクリメントする
								nKey = -1;									//キーカウンターを元に戻す

								while (strcmp(pStr, "END_KEY_SET") != 0)
								{//キーセットの読み込みが終わるまで進む
									fscanf(pFile, "%s", pStr);

									if (strcmp(pStr, "FRAME") == 0)
									{//フレーム数の読み込む処理
										int nFrames = 0;
										fscanf(pFile, "%s %d", pStr, &nFrames);

										keySet.nAnimFrames = nFrames;		//フレーム数の設定処理
									}
									else if (strcmp(pStr, "KEY") == 0)
									{//キーの読み込む処理
										AnimKey key;						//新しいキーを宣言する
										ZeroMemory(&key, sizeof(AnimKey));	//キーを初期化する
										nKey++;								//キーカウンターをインクリメントする

										char pString[512] = {};				//文字列を宣言する

										fscanf(pFile, "%s", pString);

										while (strcmp(pString, "END_KEY") != 0)
										{//キーの読み込みが終わるまで進む
											float x = 0.0f;					
											float y = 0.0f;					
											float z = 0.0f;					
											fscanf(pFile, "%s", pString);

											if (strcmp(pString, "POS") == 0)
											{//位置の読み込む処理
												fscanf(pFile, "%s", pString);

												fscanf(pFile, "%s", pStr);	//X座標の読み込む処理
												std::string s = pStr;		//std::stringに変換する
												x = std::stof(s);			//floatに変換する

												fscanf(pFile, "%s", pStr);	//Y座標の読み込む処理
												s = pStr;					//std::stringに変換する
												y = std::stof(s);			//floatに変換する

												fscanf(pFile, "%s", pStr);	//Z座標の読み込む処理
												s = pStr;					//std::stringに変換する
												z = std::stof(s);			//floatに変換する

												key.RelativePos = D3DXVECTOR3(x, y, z);		//位置の設定処理
											}
											else if (strcmp(pString, "ROT") == 0)
											{//回転の読み込む処理
												fscanf(pFile, "%s", pString);

												fscanf(pFile, "%s", pStr);	//X座標の読み込む処理
												std::string s = pStr;		//std::stringに変換する
												x = std::stof(s);			//floatに変換する

												fscanf(pFile, "%s", pStr);	//Y座標の読み込む処理
												s = pStr;					//std::stringに変換する
												y = std::stof(s);			//floatに変換する

												fscanf(pFile, "%s", pStr);	//Z座標の読み込む処理
												s = pStr;					//std::stringに変換する
												z = std::stof(s);			//floatに変換する

												key.RelativeRot = D3DXVECTOR3(x, y, z);		//回転の設定処理
											}
										}	

										keySet.vAnimKey.push_back(key);		//キーセットのベクトルにキーを入れる
									}
								}

								motionSet.vAnimKeySet.push_back(keySet);	//モーションセットのベクトルにキーセットを入れる
							}
						}

						set.vAnimSets.push_back(motionSet);			//アニメーションのベクトルにモーションセットを入れる
					}
				}

				m_vAnimAll.push_back(set);			//全部のアニメーションのベクトルに読み込んだアニメーションを入れる
			}
		}
	}
}

//アニメーションの破棄処理
void CAnimator::DestroyLoadedAnimation(void)
{
	//アニメーションのベクトルをクリアする
	m_vAnimAll.clear();
}

//生成処理
CAnimator* CAnimator::Create(std::vector <CModelPart*>* vParts, AnimType type)
{
	CAnimator* pAnimator = new CAnimator;				//アニメーターを生成する

	if (FAILED(pAnimator->Init()))
	{//初期化処理
		return nullptr;
	}

	pAnimator->SetParts(vParts);				//パーツの設定処理
	pAnimator->m_type = type;					//種類の設定

	for (int nCnt = 0; nCnt < (int)pAnimator->m_vParts.size(); nCnt++)
	{//パーツ毎のアニメーションスピードの計算
		animSpeed speed;
		speed.move = (m_vAnimAll.data()[type].vAnimSets[0].vAnimKeySet[0].vAnimKey[nCnt].RelativePos - pAnimator->m_vParts.data()[nCnt]->GetPos())
			/ (float)m_vAnimAll.data()[type].vAnimSets[0].vAnimKeySet[0].nAnimFrames;
		speed.rotSpeed = (m_vAnimAll.data()[type].vAnimSets[0].vAnimKeySet[0].vAnimKey[nCnt].RelativeRot - pAnimator->m_vParts.data()[nCnt]->GetRot())
			/ (float)m_vAnimAll.data()[type].vAnimSets[0].vAnimKeySet[0].nAnimFrames;
		pAnimator->m_vAnimSpeed.push_back(speed);			//計算したスピードの設定
	}

	return pAnimator;					//生成したアニメーターを返す
}

//アニメーションの設定処理
void CAnimator::SetPresentAnim(int nPresentAnim)
{
	if (m_nPresentAnim != nPresentAnim)
	{
		m_nPresentAnim = nPresentAnim;
		m_nPresentFrame = 0;
		m_nPresentMotionSet = 0;

		for (int nCnt = 0; nCnt < (int)m_vParts.size(); nCnt++)
		{//パーツ毎のアニメーションスピードの計算

			m_vAnimSpeed.data()[nCnt].move = (m_vAnimAll.data()[m_type].vAnimSets[nPresentAnim].vAnimKeySet[0].vAnimKey[nCnt].RelativePos - m_vParts.data()[nCnt]->GetPos())
				/ (float)m_vAnimAll.data()[m_type].vAnimSets[nPresentAnim].vAnimKeySet[0].nAnimFrames;
			m_vAnimSpeed.data()[nCnt].rotSpeed = (m_vAnimAll.data()[m_type].vAnimSets[nPresentAnim].vAnimKeySet[0].vAnimKey[nCnt].RelativeRot - m_vParts.data()[nCnt]->GetRot())
				/ (float)m_vAnimAll.data()[m_type].vAnimSets[nPresentAnim].vAnimKeySet[0].nAnimFrames;
		}
	}
}

//アニメーションの設定処理
void CAnimator::SetLoopingAnim(int nPresentAnim)
{
	if (m_nPresentAnim == nPresentAnim)
	{
		int a, b;
		a = (int)m_vAnimAll.data()[m_type].vAnimSets[m_nPresentAnim].vAnimKeySet.size();
		b = (int)m_vAnimAll.data()[m_type].vAnimSets[m_nPresentAnim].vAnimKeySet[m_nPresentMotionSet].nAnimFrames;

		if (m_nPresentMotionSet >= a - 1 &&
			m_nPresentFrame >= b - 1)
		{
			m_nPresentAnim = nPresentAnim;
			m_nPresentFrame = 0;
			m_nPresentMotionSet = 0;

			for (int nCnt = 0; nCnt < (int)m_vParts.size(); nCnt++)
			{//パーツ毎のアニメーションスピードの計算

				m_vAnimSpeed.data()[nCnt].move = (m_vAnimAll.data()[m_type].vAnimSets[nPresentAnim].vAnimKeySet[0].vAnimKey[nCnt].RelativePos - m_vParts.data()[nCnt]->GetPos())
					/ (float)m_vAnimAll.data()[m_type].vAnimSets[nPresentAnim].vAnimKeySet[0].nAnimFrames;
				m_vAnimSpeed.data()[nCnt].rotSpeed = (m_vAnimAll.data()[m_type].vAnimSets[nPresentAnim].vAnimKeySet[0].vAnimKey[nCnt].RelativeRot - m_vParts.data()[nCnt]->GetRot())
					/ (float)m_vAnimAll.data()[m_type].vAnimSets[nPresentAnim].vAnimKeySet[0].nAnimFrames;
			}
		}
	}
	else
	{
		m_nPresentAnim = nPresentAnim;
		m_nPresentFrame = 0;
		m_nPresentMotionSet = 0;

		for (int nCnt = 0; nCnt < (int)m_vParts.size(); nCnt++)
		{//パーツ毎のアニメーションスピードの計算

			m_vAnimSpeed.data()[nCnt].move = (m_vAnimAll.data()[m_type].vAnimSets[nPresentAnim].vAnimKeySet[0].vAnimKey[nCnt].RelativePos - m_vParts.data()[nCnt]->GetPos())
				/ (float)m_vAnimAll.data()[m_type].vAnimSets[nPresentAnim].vAnimKeySet[0].nAnimFrames;
			m_vAnimSpeed.data()[nCnt].rotSpeed = (m_vAnimAll.data()[m_type].vAnimSets[nPresentAnim].vAnimKeySet[0].vAnimKey[nCnt].RelativeRot - m_vParts.data()[nCnt]->GetRot())
				/ (float)m_vAnimAll.data()[m_type].vAnimSets[nPresentAnim].vAnimKeySet[0].nAnimFrames;
		}
	}
}