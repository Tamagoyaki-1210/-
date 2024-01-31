//==============================================================================================
//
// マップ			map.cpp
// tutida ryousei
//
//==============================================================================================
#include"map.h"
#include"map_object.h"
#include"restraint_switch.h"
#include"objectX.h"
#include <stdio.h>
#include "d3dx9.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CMap::CMap()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CMap::~CMap()
{
}

//==============================================================================================
// マップの読み込み
//==============================================================================================
void CMap::ReadMap(char *filename)
{
	// ファイル名
	FILE* pFile = fopen(filename, "r");

	char m_aString[0xff];
	memset(m_aString, 0, sizeof(m_aString));

	char XFileName[0x20][0xff] = {};	// Xファイル名

	int nNumModel = 0;
	int nIndex = 0;
	int nType = 0;
	D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 Rot = { 0.0f,0.0f,0.0f };
	int nRestraint_Switch = 0;
	int nSwitch_Index = 0;
	int nCollision = 0;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_pRestraint_Switch[nCnt] = nullptr;
	}

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

					if (strcmp(&m_aString[0], "NUM_MODEL") == 0)
					{
						// モデルの数
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%d", &nNumModel);
					}

					if (strcmp(&m_aString[0], "MODEL_FILENAME") == 0)
					{
						// ファイルネーム
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%s", &XFileName[nIndex][0]);
						nIndex++;
					}

					if (strcmp(&m_aString[0], "MODELSET") == 0)
					{
						while (strcmp(&m_aString[0], "END_MODELSET") != 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);

							if (strcmp(&m_aString[0], "TYPE") == 0)
							{
								// モデルタイプ
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nType);
							}
							if (strcmp(&m_aString[0], "POS") == 0)
							{
								// 位置
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%f", &Pos.x);
								fscanf(pFile, "%f", &Pos.y);
								fscanf(pFile, "%f", &Pos.z);
							}
							if (strcmp(&m_aString[0], "ROT") == 0)
							{
								// 角度
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%f", &Rot.x);
								fscanf(pFile, "%f", &Rot.y);
								fscanf(pFile, "%f", &Rot.z);
							}
							if (strcmp(&m_aString[0], "SWITCH") == 0)
							{
								// 拘束用のスイッチ
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nRestraint_Switch);
							}
							if (strcmp(&m_aString[0], "COLLISION_OFF") == 0)
							{
								// 当たり判定の有無
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nCollision);
							}
						}

						// 当たり判定無し
						//if (nCollision == 0)
						//	CObjectX::Create(Pos, Rot, nullptr, &XFileName[nType][0], CObject::PRIORITY_BACK);
						//// 当たり判定あり
						//else
						//{
							// モデル生成
							if (nRestraint_Switch == 0)
								CMap_Object::Create(Pos, Rot, nullptr, &XFileName[nType][0]);
							else
							{
								// 拘束スイッチ
								m_pRestraint_Switch[nSwitch_Index] = CRestraint_Switch::Create(Pos, Rot, nullptr, &XFileName[nType][0], nSwitch_Index);
								nSwitch_Index++;
							}
						//}
					}
				}
			}
		}
	}

	//ファイルを閉じる
	fclose(pFile);
}