//============================================================================
//
// モーション	motion.h
// Author : Tanimoto Kosuke
//
//============================================================================
#ifndef _MOTION_H		//このマクロ定義がされてなかったら
#define _MOTION_H		//2連インクルード防止のマクロ定義

//=============================================================================
// インクルードファイル
//=============================================================================
#include "d3dx9.h"
#include <vector>
#include <map>

//---------------------------
// クラス宣言
//---------------------------
class CMotion
{
public:
	// モーションの情報
	enum MOTION
	{
		MOTION_PLAYER_BODY = 0,	// 体パーツ
		MOTION_PLAYER_ARMS,		// 腕パーツ
		MOTION_PLAYER_LEG,		// 脚パーツ

		MOTION_MOB,				// モブパーツ

		MOTION_BOSS,			// ボスパーツ

		MOTION_MAX
	};
	static const char* m_cMotionFileName[MOTION_MAX];		// ファイル名

	// キーの情報
	struct KEY
	{
		D3DXVECTOR3 KeyPos;		// キーの位置
		D3DXVECTOR3 KeyRot;		// キーの回転
	};

	// キーセットの情報
	struct KEY_SET
	{
		int nFrame;				// フレーム
		std::vector <KEY> aKey;	// キーの情報
	};

	// モーションセットの情報
	struct MotionPattern
	{
		int nMaxKey;					// キーの総数
		bool bLoop;						// ループ判定
		std::vector <KEY_SET> aKeySet;	// キーセットの情報
	};

	CMotion();
	~CMotion();

	void SetMotionData(MotionPattern motion, std::string name);	// モーション値の読み込み
	void SetUseFileName(const char* Xfilename) { m_UseFileName.push_back(Xfilename); }

	MotionPattern GetMotionPattern(const int index, std::string name) { return m_MotionPatternData.at(name)[index]; }
	std::vector<std::string> GetUseFileName() { return m_UseFileName; };

	void ReleaseAll();							// 全てのモーションの破棄処理

	void LoadFile(const char* Xfilename);
	void LoadAllFile();							// 全てのモデルパーツの読み込み

private:

	std::map<std::string, std::vector <MotionPattern>> m_MotionPatternData;	// モーションのデータ(自身のモーション名, モーションの番号)
	std::vector<std::string> m_UseFileName;									// 使用中ファイルのモーション名
};
#endif// !_MOTION_H