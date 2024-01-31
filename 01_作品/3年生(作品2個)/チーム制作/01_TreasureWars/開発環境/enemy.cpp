//=============================================================================
//
// enemy.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "enemy.h"
#include "application.h"
#include "enemymanager.h"
#include "game.h"
#include "particle_emitter.h"

const float CEnemy::Move_Inertia = 0.2f;
const float CEnemy::Enemy_Rot_Speed = 0.1f;
//=====================================
// デフォルトコンストラクタ
//=====================================
CEnemy::CEnemy() : CObject(2)
{
	// 生成時に自身のポインタをアイテムマネージャーに設定
	CGame::GetEnemyManager()->SetEnemy(this);
	m_fRotSpeed = Enemy_Rot_Speed;
}

//=====================================
// デストラクタ
//=====================================
CEnemy::~CEnemy()
{
	CEnemyManager* pManager = CGame::GetEnemyManager();

	// マネージャーが使用中の場合、解放時に自身のポインタを配列から除外する
	if (pManager != nullptr) pManager->DestroyEnemy(this);
}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CEnemy::Init()
{
	m_nMotion = 0;
	m_nCurrentMotion = 0;			// 現在のモーション番号

	m_nCurrentKey = 0;				// キーを0にする
	m_nCountMotion = 0;				// モーションカウントを0にする

	m_bNullModel = false;

	m_move = { 0.0f, 0.0f, 0.0f };

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CEnemy::Uninit()
{
	// 全てのモデルの終了
	while (!m_pModel.empty())
	{
		m_pModel.back()->Uninit();
		delete m_pModel.back();
		m_pModel.pop_back();
	}

	Release();
}

//============================================================================
// 更新処理
//============================================================================
void CEnemy::Update()
{
	Move();
}

//============================================================================
// 描画処理
//============================================================================
void CEnemy::Draw()
{
	// 描画中の場合
	if (GetDrawFlag())
	{
		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

		//計算用のマトリックス
		D3DXMATRIX mtxRot, mtxTrans;

		//ワールドマトリックス
		D3DXMatrixIdentity(&m_mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// 全てのモデルを読み込む
		for (auto pModel : m_pModel)
		{
			if (pModel != nullptr)
			{
				// モデルの描画処理
				pModel->Draw();
			}
		}
	}
}

//==============================================================================================
// モーション値の読み込み
//==============================================================================================
void CEnemy::SetMotionData(int maxkey, int key, int parts, int motion, int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop)
{
	// キーフレームの設定
	m_MotionSet[motion].aKeySet[key].nFrame = frame;

	// 位置と角度の設定
	m_MotionSet[motion].aKeySet[key].aKey[parts].KeyPos = pos;
	m_MotionSet[motion].aKeySet[key].aKey[parts].KeyRot = rot;

	// キーの最大数
	m_MotionSet[motion].nMaxKey = maxkey;

	// ループするか
	m_MotionSet[motion].bLoop = loop;
}

//==============================================================================================
// 移動処理
//==============================================================================================
void CEnemy::Move()
{
	// 移動量を更新(減衰させる)
	m_move -= m_move * Move_Inertia;

	// 位置更新
	m_pos += m_move * m_fSpeed;
}

//==============================================================================================
// モーションの再生
//==============================================================================================
void CEnemy::Motion()
{
	if (!m_pModel.empty())
	{
		for (int nCnt = 0; nCnt < (int)m_pModel.size(); nCnt++)
		{
			if (m_pModel[nCnt] != nullptr && !m_MotionSet[m_nCurrentMotion].bStop)
			{
				// 相対値
				float fRelative = (float)m_nCountMotion / (float)m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].nFrame;

				// 差分
				D3DXVECTOR3 fPosDifference;		// 位置用
				D3DXVECTOR3 fRotDifference;		// 回転用

												// 差分
												// 最後のキーじゃない場合
				if (m_nCurrentKey != m_MotionSet[m_nCurrentMotion].nMaxKey - 1)
				{
					// 位置
					fPosDifference = m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyPos
						- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;					// 終了値 - 開始値

																													// 角度
					fRotDifference = m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyRot
						- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;					// 終了値 - 開始値
				}
				// 最後のキーの場合
				else if (m_nCurrentKey == m_MotionSet[m_nCurrentMotion].nMaxKey - 1)
				{
					// 位置
					fPosDifference = m_MotionSet[m_nCurrentMotion].aKeySet[0].aKey[nCnt].KeyPos
						- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;	// 終了値 - 開始値

																									// 角度
					fRotDifference = m_MotionSet[m_nCurrentMotion].aKeySet[0].aKey[nCnt].KeyRot
						- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;	// 終了値 - 開始値
				}

				// 現在値
				D3DXVECTOR3 Pos = m_InitPos[nCnt] + m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos + (fPosDifference * fRelative);	// 位置
				D3DXVECTOR3 Rot = m_InitRot[nCnt] + m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot + (fRotDifference * fRelative);	// 回転

				m_pModel[nCnt]->SetPos(Pos);	// 位置の設定
				m_pModel[nCnt]->SetRot(Rot);	// 回転の設定

				m_bNullModel = false;
			}
			else if (m_pModel[0] == nullptr)
			{
				// nullになったら存在を無視する
				m_bNullModel = true;
			}
		}

		// モーションカウンター
		m_nCountMotion++;

		// 最後のフレームまで進んだ
		if (m_nCountMotion >= m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].nFrame
			&& !m_bNullModel)
		{
			// フレームを0に戻す
			m_nCountMotion = 0;

			// キーを進める
			m_nCurrentKey++;

			if (m_nCurrentKey >= m_MotionSet[m_nCurrentMotion].nMaxKey)
			{
				// ループする
				if (m_MotionSet[m_nCurrentMotion].bLoop)
				{
					// キーを戻してループする
					m_nCurrentKey = 0;
				}
				// ループしない
				else
				{
					m_MotionSet[m_nCurrentMotion].bStop = true;
				}
			}
		}
	}
}

//==============================================================================================
// モーション変更
//==============================================================================================
void CEnemy::ChangeMotion()
{
	// 前回の止まったモーションをfalseにする
	m_MotionSet[m_nCurrentMotion].bStop = false;

	m_nCurrentMotion = m_nMotion;	// モーションを切り替える
	m_nCurrentKey = 0;				// キーを0にする
	m_nCountMotion = 0;				// モーションカウントを0にする
}

//==============================================================================================
// 角度の正規化
//==============================================================================================
const float CEnemy::NormalizeRot()
{
	// 目的の角度の正規化
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rotDest.y += D3DX_PI * 2;
	}

	// 目的の角度にする
	m_rot.y += (m_rotDest.y - m_rot.y) * m_fRotSpeed;

	// 角度の正規化
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}

	return m_rot.y;
}

//============================================================================
// 自身の終了処理
//============================================================================
void CEnemy::Death()
{
	// 幽霊消失時パーティクル
	CParticleEmitter::Create("EraseGhost", GetPos());

	// 終了処理
	Uninit();
}

//==============================================================================================
// モデルパーツの設定
//==============================================================================================
void CEnemy::LoadFileEnemy(const char *Xfilename)
{
	// ファイル名
	FILE* pFile = fopen(Xfilename, "r");

	// キャラクターセット用
	D3DXVECTOR3 Rot;				// 角度
	int nIndex = 0;					// 番号
	int nParentIndex;				// 親の番号
	int nPartsIndex = 0;			// パーツの番号
	D3DXVECTOR3 PartsStartPos;		// 初期位置
	D3DXVECTOR3 PartsStartRot;		// 初期角度
	char XFileName[0x20][0xff] = {};	// Xファイル名

	int nNum = 0;

	// モーション用
	D3DXVECTOR3 PartsPos;	// 位置
	D3DXVECTOR3 PartsRot;	// 回転
	int nFrame;				// キーのフレーム数
	int nMaxKey;			// キーの数
	int nMaxParts;			// パーツの数
	bool bLoop = false;		// ループするか
	int nKey = 0;			// 現在のキーの数
	int nParts = 0;			// 現在のパーツ
	int nNumMotion = 0;		// 現在のモーション

	char m_aString[0xff];
	memset(m_aString, 0, sizeof(m_aString));

	// ファイルが開いた場合
	if (pFile != NULL)
	{
		while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", &m_aString[0]);

			// SCRIPTが一致
			if (strcmp(&m_aString[0], "SCRIPT") == 0)
			{
				while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &m_aString[0]);

					// ファイル名
					if (strcmp(&m_aString[0], "MODEL_FILENAME") == 0)
					{
						// ＝を読み飛ばす
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%s", &XFileName[nIndex][0]);
						nIndex++;
					}

					// パーツの最大数
					if (strcmp(&m_aString[0], "NUM_MODEL") == 0)
					{
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%d", &nMaxParts);
					}

					if (strcmp(&m_aString[0], "ALL_PARTSET") == 0)
					{
						while (strcmp(&m_aString[0], "END_ALL_PARTSET") != 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);

							if (strcmp(&m_aString[0], "PARTSSET") == 0)
							{
								m_pModel.push_back(nullptr);
								m_InitPos.push_back({ 0.0f, 0.0f, 0.0f });
								m_InitRot.push_back({ 0.0f, 0.0f, 0.0f });
								while (strcmp(&m_aString[0], "END_PARTSSET") != 0)
								{
									fscanf(pFile, "%s", &m_aString[0]);

									// 番号
									if (strcmp(&m_aString[0], "INDEX") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%d", &nPartsIndex);
									}

									// 親の番号
									if (strcmp(&m_aString[0], "PARENT") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%d", &nParentIndex);
									}

									// パーツの位置
									if (strcmp(&m_aString[0], "POS") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%f", &PartsStartPos.x);
										fscanf(pFile, "%f", &PartsStartPos.y);
										fscanf(pFile, "%f", &PartsStartPos.z);
									}

									// パーツの向き
									if (strcmp(&m_aString[0], "ROT") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%f", &PartsStartRot.x);
										fscanf(pFile, "%f", &PartsStartRot.y);
										fscanf(pFile, "%f", &PartsStartRot.z);
									}
								}
								// モデルパーツの設定
								SetModel(nPartsIndex, nParentIndex, PartsStartPos, PartsStartRot, &XFileName[nPartsIndex][0]);
							}
						}
					}

					if (strcmp(&m_aString[0], "MOTIONSET") == 0)
					{
						m_MotionSet.emplace_back();
						while (strcmp(&m_aString[0], "END_MOTIONSET") != 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);

							// ループするか
							if (strcmp(&m_aString[0], "LOOP") == 0)
							{
								int nLoop;
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nLoop);

								// ループ判定の代入(true = 1, false = 0)
								bLoop = nLoop == 1;
							}

							// キーの数
							if (strcmp(&m_aString[0], "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nMaxKey);
							}

							if (strcmp(&m_aString[0], "KEYSET") == 0)
							{
								m_MotionSet[nNumMotion].aKeySet.emplace_back();
								while (strcmp(&m_aString[0], "END_KEYSET") != 0)
								{
									fscanf(pFile, "%s", &m_aString[0]);

									// キーフレーム
									if (strcmp(&m_aString[0], "FRAME") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%d", &nFrame);
									}

									if (strcmp(&m_aString[0], "KEY") == 0)
									{
										m_MotionSet[nNumMotion].aKeySet[nKey].aKey.emplace_back();
										while (strcmp(&m_aString[0], "END_KEY") != 0)
										{
											fscanf(pFile, "%s", &m_aString[0]);

											// 位置
											if (strcmp(&m_aString[0], "POS") == 0)
											{
												fscanf(pFile, "%s", &m_aString[0]);
												fscanf(pFile, "%f", &PartsPos.x);
												fscanf(pFile, "%f", &PartsPos.y);
												fscanf(pFile, "%f", &PartsPos.z);
											}

											// 角度
											if (strcmp(&m_aString[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &m_aString[0]);
												fscanf(pFile, "%f", &PartsRot.x);
												fscanf(pFile, "%f", &PartsRot.y);
												fscanf(pFile, "%f", &PartsRot.z);

											}

											if (strcmp(&m_aString[0], "END_KEY") == 0)
											{
												// モーション値の設定
												SetMotionData(nMaxKey, nKey, nParts, nNumMotion, nFrame, PartsPos, PartsRot, bLoop);

												nParts++;

												if (nParts == nMaxParts)
												{
													nParts = 0;
												}
											}
										}
									}
								}
								nKey++;
							}
						}
						nKey = 0;	// キーの初期化

						nNumMotion++;
					}
				}
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//==============================================================================================
// モデルパーツの設定
//==============================================================================================
CModel* CEnemy::SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename)
{
	if (m_pModel[index] == nullptr)
	{
		if (parent >= 0)
		{// 親がいる場合
			m_pModel[index] = CModel::Create(pos, rot, m_pModel[parent], Xfilename);
		}
		else
		{// 親がいない場合
			m_pModel[index] = CModel::Create(pos, rot, nullptr, Xfilename);
		}

		// パーツの初期位置
		m_InitPos[index] = m_pModel[index]->GetPos();
		m_InitRot[index] = m_pModel[index]->GetRot();
	}
	return m_pModel[index];
}