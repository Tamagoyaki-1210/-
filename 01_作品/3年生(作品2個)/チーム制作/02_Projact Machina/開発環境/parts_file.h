//=============================================================================
//
// parts_file.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _PARTS_FILE_H_
#define _PARTS_FILE_H_

//=============================================================================
// インクルードファイル
//=============================================================================

#include "objectX.h"
#include "motion.h"

class CParts;

//---------------------------
// クラス宣言
//---------------------------
class CParts_File
{

public:
	// パーツの情報
	enum PARTS
	{
		PARTS_PLAYER_BODY_1 = 0,	// 体パーツ1
		PARTS_PLAYER_BODY_2,		// 体パーツ2
		PARTS_PLAYER_BODY_3,		// 体パーツ3
		PARTS_PLAYER_BODY_4,		// 体パーツ4
		//PARTS_PLAYER_BODY_5,		// 体パーツ5

		PARTS_PLAYER_ARMS_0,		// 腕パーツ0(スタンダード)
		PARTS_PLAYER_ARMS_1,		// 腕パーツ1
		PARTS_PLAYER_ARMS_2,		// 腕パーツ2
		PARTS_PLAYER_ARMS_3,		// 腕パーツ3
		PARTS_PLAYER_ARMS_4,		// 腕パーツ4
		//PARTS_PLAYER_ARMS_5,		// 腕パーツ5

		PARTS_PLAYER_LEG_0,			// 脚パーツ0(スタンダード)
		PARTS_PLAYER_LEG_1,			// 脚パーツ1
		PARTS_PLAYER_LEG_2,			// 脚パーツ2
		PARTS_PLAYER_LEG_3,			// 脚パーツ3
		PARTS_PLAYER_LEG_4,			// 脚パーツ4
		//PARTS_PLAYER_LEG_5,			// 脚パーツ5

		PARTS_MOB,					// モブパーツ

		PARTS_BOSS,					// ボスパーツ

		PARTS_MAX
	};

	// モデルセット
	struct MODEL_SET
	{
		int nModelIndex;		// モデル番号
		D3DXVECTOR3 InitPos;	// 初期位置
		D3DXVECTOR3 InitRot;	// 初期回転
		int nParentIndex;		// 親モデルの番号
	};

	// パーツセット
	struct PARTS_SET
	{
		std::string Name;					// パーツ名	
		std::vector<MODEL_SET> ModelSet;	// キャラクターのモデル情報
	};

	CParts_File();
	 ~CParts_File();

	HRESULT Init();
	void Uninit();

	void SetModel(const int partsIndex, const int modelIndex, const int parent, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* Xfilename);		// モデルパーツの設定
	
	PARTS_SET GetPartsSet(const int index) { return m_PartsSet[index]; }

	void ReleaseAllFile();	// 全てのモデルセットの終了
	void LoadFile(const char* Xfilename, const int partsIndex);
	void LoadAllFile();																// 全てのモデルパーツの読み込み

private:
	static const char* m_cPartsFileName[PARTS_MAX];		// ファイル名

	PARTS_SET m_PartsSet[PARTS_MAX];	// パーツの情報

};

#endif// _PARTS_FILE_H_