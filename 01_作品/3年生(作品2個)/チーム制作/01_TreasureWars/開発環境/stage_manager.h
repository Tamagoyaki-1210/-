//＝＝＝＝＝＝＝＝＝＝＝＝＝
// StageManager.h
//＝＝＝＝＝＝＝＝＝＝＝＝＝
#ifndef _STAGE_MANAGER_H_
#define _STAGE_MANAGER_H_

#include "application.h"
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

class CObjectX;

class CStageManager
{
public:
	struct SModelData
	{
		LPD3DXMESH mesh;						// メッシュ情報
		LPD3DXBUFFER buffMat;					// マテリアル情報
		DWORD dwNum;							// マテリアルの個数
		D3DXVECTOR3 maxModel;					// モデルの最大値
		D3DXVECTOR3 minModel;					// モデルの最小値
	};

	struct SObjectData
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXVECTOR3 scale;
		D3DXMATRIX mtxWorld;
		std::string modelLabel;
	};

	struct SStageContainer
	{
		SModelData modelData;
		SObjectData objectData;
	};

	CStageManager();
	~CStageManager();

	void Create(std::string stagePath);
	void Clear();
	void LoadText(std::string path);

	void SetStageData(std::string& label, std::vector<SStageContainer>& info) { m_stageData[label] = info; }
	void SetSelectIndex(size_t index) { m_selectIndex = index, m_isSelected = true; }
	void SetSelectStatus(bool set) { m_isSelected = set; }

	std::vector<SStageContainer> GetStageData(std::string label) { return m_stageData[label]; }
	size_t GetSelectIndex() { return m_selectIndex; }

private:
	// テキストファイルのデータ読み込み関係
	void ReadVector3(std::stringstream& ss, std::string name, D3DXVECTOR3& vec);
	void ReadColor(std::stringstream& ss, std::string name, D3DXCOLOR& color);
	bool ReadString(std::stringstream& ss, std::string name, std::string& tag);
	bool ReadInt(std::stringstream& ss, std::string name, int& value);
	void ReadFloat(std::stringstream& ss, std::string name, float& value);
	bool ReadBool(std::stringstream& ss, std::string name, bool& value);

	// ssの状態をクリアし、読み取り位置を先頭に戻す処理
	void ssInitialize(std::stringstream& ss) { ss.clear(), ss.seekg(0); }

	std::string ProcessPath(const std::string& filePath);

	std::map<std::string, std::vector<SStageContainer>> m_stageData;
	std::vector<CObjectX*> m_pObject;
	std::string m_stagePath;
	size_t m_selectIndex;
	bool m_isSelected;
};

#endif
