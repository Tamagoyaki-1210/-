//==============================================================================================
//
// トロッコ　　　trolley.cpp
// tutida ryousei
//
//==============================================================================================
#include"application.h"
#include"game.h"
#include"charmanager.h"
#include"map.h"
#include"gimmick_model.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CTrolley::CTrolley()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CTrolley::~CTrolley()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CTrolley::Init()
{
	m_nLevelDownCounter = 0;	// レベルダウンカウンター

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CTrolley::Uninit()
{
	// モデルの終了処理
	for (int nCnt = 0; nCnt < m_nNum_GimmickParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			m_pModel[nCnt]->Uninit();
			delete m_pModel[nCnt];
			m_pModel[nCnt] = nullptr;
		}
	}

	Release();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CTrolley::Update()
{
	// トロッコを動かしている場合
	if (CMap::GetTrolleyManager()->GetMove() == true)
	{
		// モーションの設定
		Motion();

		// 当たり判定
		Trolley_Collision();

		// 巡回地点を回る
		Turn();

		// 角度の正規化
		NormalizeRot();

		// トロッコの位置の設定
		SetPos(m_Pos);
	}

	// レベルダウンのカウンター
	if (m_nLevelDownCounter++ >= Level_Down_Timer)
	{
		// レベルダウン
		Level(false);
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CTrolley::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用のマトリックス

	//ワールドマトリックス
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCnt = 0; nCnt < m_nNum_GimmickParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
			m_pModel[nCnt]->Draw();
	}
}

//==============================================================================================
// トロッコの初期情報を設定
//==============================================================================================
void CTrolley::SetTrolleyInfomation()
{
	CTrolley_Manager *pTrolley_Manager = CMap::GetTrolleyManager();

	// トロッコマネージャーから情報を読み込む
	if (pTrolley_Manager != nullptr)
	{
		m_nStartNum = pTrolley_Manager->GetStartNum();

		for (int nCnt = 0; nCnt < m_nStartNum; nCnt++)
		{
			m_Start_Pos[nCnt] = pTrolley_Manager->GetStart_Pos(nCnt);
		}

		// 巡回地点の数
		m_nTurnNum = pTrolley_Manager->GetTurnNum();

		// 巡回地点
		for (int nCnt = 0; nCnt < m_nTurnNum; nCnt++)
		{
			m_TurnPos[nCnt] = pTrolley_Manager->GetTurn_Pos(nCnt);
		}

		// 移動速度
		for (int nCnt = 0; nCnt < CTrolley_Manager::MaxLevel; nCnt++)
		{
			m_fSpeed[nCnt] = pTrolley_Manager->GetSpeed(nCnt);
		}

		// 半径
		m_fRadius = pTrolley_Manager->GetRadius();

		// 最初の目的地
		SetStart(pTrolley_Manager->GetStartTurn_Index());
	}
}

//==============================================================================================
// 初期位置の設定
//==============================================================================================
void CTrolley::SetStart(const int index)
{
	// トロッコを描画する
	SetDrawFlag(true);

	m_fCp = 0.0f;				// 外積

	// 最初の目的地
	m_nNextTurn_Index = index;

	// 最初の目的地の代入
	m_nNextTurn_IndexInit = m_nNextTurn_Index;

	// 0...順回転	1...逆回転
	m_nTurn = rand() % 2;

	// 最初のベクトル
	m_Next_Vec = m_TurnPos[m_nNextTurn_Index] - m_Pos;

	// 最初の角度
	m_Rot_Dest.y = (float)atan2(m_Pos.x - m_TurnPos[m_nNextTurn_Index].x, m_Pos.z - m_TurnPos[m_nNextTurn_Index].z);
	m_Rot.y = m_Rot_Dest.y;

	m_bReturn = false;		// 初期位置に戻る
	m_bOneTurn = false;		// 一周したか
	m_bBack = false;		// 後退してるか

	m_Motion = TROLLEY_MOTION_MOVE;
}

//==============================================================================================
// モーション値の読み込み
//==============================================================================================
void CTrolley::SetMotionData(int maxkey, int key, int parts, int motion,
	int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop)
{
	// キーフレームの設定
	m_MotionSet.aKeySet[key].nFrame = frame;

	// 位置と角度の設定
	m_MotionSet.aKeySet[key].aKey[parts].KeyPos = pos;
	m_MotionSet.aKeySet[key].aKey[parts].KeyRot = rot;

	// キーの最大数
	m_MotionSet.nMaxKey = maxkey;

	// ループするか
	m_MotionSet.bLoop = loop;
}

//==============================================================================================
// モーションの設定
//==============================================================================================
void CTrolley::Motion()
{
	for (int nCnt = 0; nCnt < m_nNum_GimmickParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr && !m_MotionSet.bStop)
		{
			// 相対値
			float fRelative = (float)m_nCountMotion / (float)m_MotionSet.aKeySet[m_nCurrentKey].nFrame;

			// 差分
			D3DXVECTOR3 fPosDifference;		// 位置用
			D3DXVECTOR3 fRotDifference;		// 回転用

			// 差分
			// 最後のキーじゃない場合
			if (m_nCurrentKey != m_MotionSet.nMaxKey - 1)
			{
				// 位置
				fPosDifference = m_MotionSet.aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyPos
					- m_MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;					// 終了値 - 開始値

																												// 角度
				fRotDifference = m_MotionSet.aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyRot
					- m_MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;					// 終了値 - 開始値
			}
			// 最後のキーの場合
			else if (m_nCurrentKey == m_MotionSet.nMaxKey - 1)
			{
				// 位置
				fPosDifference = m_MotionSet.aKeySet[0].aKey[nCnt].KeyPos
					- m_MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;	// 終了値 - 開始値

																								// 角度
				fRotDifference = m_MotionSet.aKeySet[0].aKey[nCnt].KeyRot
					- m_MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;	// 終了値 - 開始値
			}

			// 現在値
			D3DXVECTOR3 Pos = InitPos[nCnt] + m_MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos + (fPosDifference * fRelative);	// 位置
			D3DXVECTOR3 Rot = InitRot[nCnt] + m_MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot + (fRotDifference * fRelative);	// 回転

			m_pModel[nCnt]->SetPos(Pos);	// 位置の設定
			m_pModel[nCnt]->SetRot(Rot);	// 回転の設定
		}
	}

	// モーションカウンター
	m_nCountMotion++;

	// 最後のフレームまで進んだ
	if (m_nCountMotion >= m_MotionSet.aKeySet[m_nCurrentKey].nFrame)
	{
		// フレームを0に戻す
		m_nCountMotion = 0;

		// キーを進める
		m_nCurrentKey++;

		if (m_nCurrentKey >= m_MotionSet.nMaxKey)
		{
			// ループする
			if (m_MotionSet.bLoop)
			{
				// キーを戻してループする
				m_nCurrentKey = 0;
			}
			// ループしない
			else
			{
				m_MotionSet.bStop = true;
				m_Motion = TROLLEY_MOTION_MOVE;
			}
		}
	}
}

//==============================================================================================
// モデルパーツの設定
//==============================================================================================
void CTrolley::SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename)
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
		InitPos[index] = m_pModel[index]->GetPos();
		InitRot[index] = m_pModel[index]->GetRot();
	}
}

//==============================================================================================
// 当たり判定
//==============================================================================================
void CTrolley::Trolley_Collision()
{
	// プレイヤーの情報
	D3DXVECTOR3 Char_Pos = { 0.0f,0.0f,0.0f };		// 位置
	float fChar_Radius = 0.0f;						// 半径
	bool bFlash = false;							// 点滅してるか

	// プレイヤーとの距離
	float fDistance = 0.0f;

	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		// プレイヤーの取得
		CCharacter *pChar = CApplication::GetCharManager()->GetChara(nCnt);

		if (pChar != nullptr)
		{
			Char_Pos = pChar->GetPos();	// 位置
			fChar_Radius = pChar->GetRadius();		// 半径
			bFlash = pChar->GetFlashing();			// 点滅してるか

			// プレイヤーとの距離
			fDistance = Distance(Char_Pos, m_Pos);

			// トロッコに当たった
			if (fDistance <= fChar_Radius + m_fRadius
				&& !bFlash)
				// ノックバック
				pChar->KnockBack_Set(Char_Pos, m_Pos, 10, 3);
		}
	}
}

//==============================================================================================
// 巡回地点を回る
//==============================================================================================
void CTrolley::Turn()
{
	// 目的地までのベクトル
	D3DXVECTOR3 DestPos_Vec = { 0.0f,0.0f,0.0f };
	DestPos_Vec = m_TurnPos[m_nNextTurn_Index] - m_Pos;

	// 目的の位置についたら止まる
	if (DestPos_Vec.x < m_fSpeed[m_nLevel] && DestPos_Vec.x > -m_fSpeed[m_nLevel]
		&& DestPos_Vec.z < m_fSpeed[m_nLevel] && DestPos_Vec.z > -m_fSpeed[m_nLevel])
	{
		// 位置の調整
		m_Pos = m_TurnPos[m_nNextTurn_Index];

		// トロッコの情報
		CTrolley_Manager *pTrolley_Manager = CMap::GetTrolleyManager();

		// トロッコが使用中の場合
		if (pTrolley_Manager != nullptr)
		{
			// 次の目的の位置
			if (m_nTurn && !m_bReturn)
			{
				m_nNextTurn_Index++;

				// 最大数を超えた場合
				if (m_nNextTurn_Index > m_nTurnNum - 1)
					m_nNextTurn_Index = 0;
			}
			else if (!m_nTurn && !m_bReturn)
			{
				m_nNextTurn_Index--;

				// 最小数を超えた場合
				if (m_nNextTurn_Index < 0)
					m_nNextTurn_Index = m_nTurnNum - 1;
			}

			int Rand_Start = 0;

			Rand_Start = pTrolley_Manager->GetRandStart();

			// 一周したか
			if (m_bOneTurn)
			{
				m_bReturn = true;
				m_TurnPos[m_nNextTurn_Index] = m_Start_Pos[Rand_Start];
			}

			// 次の目的地までのベクトル
			D3DXVECTOR3 NextDestPos_Vec = { 0.0f,0.0f,0.0f };
			NextDestPos_Vec = m_TurnPos[m_nNextTurn_Index] - m_Pos;

			// 外積
			m_fCp = m_Next_Vec.x * NextDestPos_Vec.z - m_Next_Vec.z * NextDestPos_Vec.x;

			// 真後ろに移動してるか
			if (m_fCp == 0 && m_Next_Vec * -1 == NextDestPos_Vec)
				m_bBack = true;

			// 次のベクトル
			m_Next_Vec = m_TurnPos[m_nNextTurn_Index] - m_Pos;

			// 次の目的地までの角度
			m_Rot_Dest.y = (float)atan2(m_Pos.x - m_TurnPos[m_nNextTurn_Index].x, m_Pos.z - m_TurnPos[m_nNextTurn_Index].z);

			// 一周する前の目的地
			if (m_nNextTurn_IndexInit == m_nNextTurn_Index)
				m_bOneTurn = true;

			// 元の位置に戻ったら消える
			if (m_bReturn && m_Next_Vec == D3DXVECTOR3(0.0f, 0.0f, 0.0f))
			{
				// トロッコの動きを止める
				pTrolley_Manager->SetMove(false);

				// トロッコを描画しない
				SetDrawFlag(false);
			}
		}
	}
	// ベクトルの正規化
	D3DXVec3Normalize(&DestPos_Vec, &DestPos_Vec);

	// 目的の位置まで移動
	m_Pos += DestPos_Vec * m_fSpeed[m_nLevel];
}

//==============================================================================================
// レベルアップの処理
//==============================================================================================
void CTrolley::Level(const bool up)
{
	// レベルダウンカウンターをリセット
	m_nLevelDownCounter = 0;

	// 変更後のレベル
	const int nLevel = m_nLevel + (up ? 1 : -1);

	// レベル上限に達していない場合
	if (nLevel < CTrolley_Manager::MaxLevel && nLevel >= 0)
	{
		// 現在のレベルを変更
		m_nLevel = nLevel;

		// モデルの色加算処理
		for (int nCnt = 0; nCnt < m_nNum_GimmickParts; nCnt++)
		{
			if (m_pModel[nCnt] != nullptr)
			{
				m_pModel[nCnt]->AddColor(D3DXCOLOR((up ? 0.1f : -0.1f), 0.0f, 0.0f, 0.0f));
			}
		}
	}
}

//==============================================================================================
// 角度の正規化
//==============================================================================================
void CTrolley::NormalizeRot()
{
	// 目的の角度の正規化
	if (m_Rot_Dest.y - m_Rot.y > D3DX_PI)
		m_Rot_Dest.y -= D3DX_PI * 2;
	else if (m_Rot_Dest.y - m_Rot.y < -D3DX_PI)
		m_Rot_Dest.y += D3DX_PI * 2;

	// 目的の角度にする
	if (!m_bBack)
	{
		if (m_fCp < 0 && m_Rot_Dest.y > m_Rot.y)
			m_Rot.y += 0.01f * m_fSpeed[m_nLevel];
		else if (m_fCp > 0 && m_Rot_Dest.y < m_Rot.y)
			m_Rot.y -= 0.01f * m_fSpeed[m_nLevel];
	}
	else
	{
		if (m_fCp < 0 && m_Rot_Dest.y + D3DX_PI > m_Rot.y)
			m_Rot.y += 0.01f * m_fSpeed[m_nLevel];
		else if (m_fCp > 0 && m_Rot_Dest.y - D3DX_PI < m_Rot.y)
			m_Rot.y -= 0.01f * m_fSpeed[m_nLevel];
	}

	// 角度の正規化
	if (m_Rot.y > D3DX_PI)
		m_Rot.y -= D3DX_PI * 2;
	else if (m_Rot.y < -D3DX_PI)
		m_Rot.y += D3DX_PI * 2;
}

//==============================================================================================
// モデルパーツの設定
//==============================================================================================
void CTrolley::LoadFileGimmick(const char *Xfilename)
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
					if (strcmp(&m_aString[0], "FILE_NAME") == 0)
					{
						// ＝を読み飛ばす
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%s", &XFileName[nIndex][0]);
						nIndex++;
					}

					// パーツの最大数
					if (strcmp(&m_aString[0], "NUMPARTS") == 0)
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
						while (strcmp(&m_aString[0], "END_MOTIONSET") != 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);

							// ループするか
							if (strcmp(&m_aString[0], "LOOP") == 0)
							{
								int nLoop;
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nLoop);

								if (nLoop == 1)
								{
									// ループする
									bLoop = true;
								}
								else
								{
									// ループしない
									bLoop = false;
								}
							}

							// キーの数
							if (strcmp(&m_aString[0], "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nMaxKey);
							}

							if (strcmp(&m_aString[0], "KEYSET") == 0)
							{
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