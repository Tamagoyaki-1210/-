//=============================================================================
//
// parts.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "parts.h"
#include "application.h"
#include "character.h"

//=====================================
// デフォルトコンストラクタ
//=====================================
CParts::CParts(const CObject::PRIORITY priority) : CObject(priority)
{

}

//=====================================
// デストラクタ
//=====================================
CParts::~CParts()
{
}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CParts::Init()
{
	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CParts::Uninit()
{
	// 全てのモデルの終了
	for (auto pModel : m_vpModel)
	{
		pModel->Uninit();
		pModel = nullptr;
	}
	m_vpModel.clear();

	Release();
}

//============================================================================
// 更新処理
//============================================================================
void CParts::Update()
{
	// モーションを切り替える場合
	ChangeMotion();

	// モーション
	Motion();

	// 角度の正規化
	NormalizeRot();
}

//============================================================================
// 描画処理
//============================================================================
void CParts::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != nullptr)
	{
		// 親のマトリックスの取得
		mtxParent = m_pParent->GetWorldMtx();

		// 親モデルのマトリックスとの掛け算
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	}

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//==============================================================================================
// モーションの再生
//==============================================================================================
void CParts::Motion()
{
	// モーションがストップしている場合
	if (!m_bMotionStop)
	{
		// モデルセットが使用されている場合
		if (!m_vpModel.empty())
		{
			// 現在モーションのモーション情報を取得(モーション番号, 自身の名前)
			CMotion::MotionPattern MotionSet = CApplication::GetMotion()->GetMotionPattern(m_nCurrentMotion, m_sMotionName);

			// ループ状態の取得
			m_bLoop = MotionSet.bLoop;

			// 全てのモデルを読み込む
			for (int nCnt = 0; nCnt < (int)m_vpModel.size(); nCnt++)
			{
				// モデルが使用中の場合
				if (m_vpModel[nCnt] != nullptr)
				{
					// 相対値
					float fRelative = (float)m_nCountMotion / (float)MotionSet.aKeySet[m_nCurrentKey].nFrame;

					// 差分
					D3DXVECTOR3 fPosDifference;		// 位置用
					D3DXVECTOR3 fRotDifference;		// 回転用

													// 最後のキーじゃない場合
					if (m_nCurrentKey != MotionSet.nMaxKey - 1)
					{
						// 位置
						fPosDifference = MotionSet.aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyPos
							- MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;					// 終了値 - 開始値

																									// 角度
						fRotDifference = MotionSet.aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyRot
							- MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;					// 終了値 - 開始値
					}
					// 最後のキーの場合
					else if (m_nCurrentKey == MotionSet.nMaxKey - 1)
					{
						// 位置
						fPosDifference = MotionSet.aKeySet[0].aKey[nCnt].KeyPos
							- MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;	// 終了値 - 開始値

																					// 角度
						fRotDifference = MotionSet.aKeySet[0].aKey[nCnt].KeyRot
							- MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;	// 終了値 - 開始値
					}

					// パーツファイルから情報を読み込む
					CParts_File::MODEL_SET ModelSet = CApplication::GetPartsFile()->GetPartsSet(m_nPartsIndex).ModelSet[nCnt];

					// 現在値
					D3DXVECTOR3 Pos = ModelSet.InitPos + MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos + (fPosDifference * fRelative);	// 位置
					D3DXVECTOR3 Rot = ModelSet.InitRot + MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot + (fRotDifference * fRelative);	// 回転

					m_vpModel[nCnt]->SetPos(Pos);	// 位置の設定
					m_vpModel[nCnt]->SetRot(Rot);	// 回転の設定

					// 親モデルの位置を中心位置に設定
					if (ModelSet.nParentIndex < 0 && m_pParent != nullptr)
					{
						// 中心位置の設定
						m_pParent->SetCenterPos(Pos);
					}
				}
			}

			// モーションカウンター
			m_nCountMotion++;

			// 最後のフレームまで進んだ
			if (m_nCountMotion >= MotionSet.aKeySet[m_nCurrentKey].nFrame)
			{
				// フレームを0に戻す
				m_nCountMotion = 0;

				// キーを進める
				m_nCurrentKey++;

				if (m_nCurrentKey >= MotionSet.nMaxKey)
				{
					// ループする
					if (MotionSet.bLoop)
					{
						// キーを戻してループする
						m_nCurrentKey = 0;
					}
					// ループしない
					else
					{
						m_bMotionStop = true;
					}
				}
			}
		}
	}
}

//==============================================================================================
// モーション変更
//==============================================================================================
void CParts::ChangeMotion()
{
	// モーションが切り替わった場合
	if (m_nCurrentMotion != m_nMotion)
	{
		// 前回の止まったモーションをfalseにする
		m_bMotionStop = false;

		m_nCurrentMotion = m_nMotion;	// モーションを切り替える
		m_nCurrentKey = 0;				// キーを0にする
		m_nCountMotion = 0;				// モーションカウントを0にする
	}
}

//==============================================================================================
// 回転方向へ振り向かせる処理
//==============================================================================================
void CParts::Rotation()
{
	// 目的の角度にする
	m_rot.y += (m_rotDest.y - m_rot.y) * m_fRotSpeed;
}

//==============================================================================================
// 角度の正規化
//==============================================================================================
void CParts::NormalizeRot()
{
	// 角度の正規化
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}

	// 目的の角度の正規化
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rotDest.y += D3DX_PI * 2;
	}
}

//==============================================================================================
// 親パーツを他パーツの子に設定する処理
//==============================================================================================
void CParts::SetModelParent(CObjectX* parts, const bool parent)
{
	// パーツファイルから情報を読み込む
	CParts_File::MODEL_SET ModelSetBody = CApplication::GetPartsFile()->GetPartsSet(m_nPartsIndex).ModelSet[0];

	ModelSetBody.nParentIndex = parent;
	m_vpModel[0]->SetParent(parts);
}

//==============================================================================================
// モデルパーツの設定
//==============================================================================================
void CParts::SetParts(const int partsIndex)
{
	// 自身のパーツの番号
	m_nPartsIndex = partsIndex;

	int nCntModel = 0;	// モデル数

	// 指定されたパーツの全てのモデルの情報を読み込む
	for (auto ModelSet : CApplication::GetPartsFile()->GetPartsSet(m_nPartsIndex).ModelSet)
	{
		CObjectX* pParent = nullptr;

		// 親ポインタが存在する場合
		if (ModelSet.nParentIndex >= 0)
		{
			pParent = m_vpModel[ModelSet.nParentIndex];
		}

		// モデルが使用されていない場合
		if ((int)m_vpModel.size() <= nCntModel)
		{
			// モデルを生成する
			m_vpModel.push_back(CObjectX::Create(ModelSet.InitPos, ModelSet.InitRot, pParent, ModelSet.nModelIndex));
		}
		// モデルが使用されている場合
		else
		{
			// 親ポインタの設定
			m_vpModel[nCntModel]->SetParent(pParent);

			// モデル番号の設定
			m_vpModel[nCntModel]->SetModel(ModelSet.nModelIndex);
		}

		// 親ポインタが存在しない場合
		if (ModelSet.nParentIndex < 0)
		{
			m_vpModel.back()->SetParts(true);
		}
		nCntModel++;
	}
	// 設定したパーツのモデル数より領域が大きかった場合
	if ((int)m_vpModel.size() > nCntModel)
	{
		const int nModelSize = (int)m_vpModel.size();

		// 多い分の領域を削除
		for (int nCnt = 0; nCnt < nModelSize - nCntModel; nCnt++)
		{
			m_vpModel.back()->Uninit();
			m_vpModel.back() = nullptr;
			m_vpModel.pop_back();
		}
	}
}

//============================================================================
// 生成処理
//============================================================================
CParts* CParts::Create(const D3DXVECTOR3 pos, const int partsIndex, CMotion::MOTION motion, CCharacter* parent)
{
	CParts* pParts = new CParts;

	if (FAILED(pParts->Init()))
	{
		return nullptr;
	}

	pParts->SetPos(pos);
	pParts->SetParts(partsIndex);
	pParts->SetMotionName(CMotion::m_cMotionFileName[motion]);
	pParts->SetParent(parent);

	return pParts;
}
