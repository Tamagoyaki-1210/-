//
// StageManager.h
//
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
		LPD3DXMESH mesh;						// bVîñ
		LPD3DXBUFFER buffMat;					// }eAîñ
		DWORD dwNum;							// }eAÌÂ
		D3DXVECTOR3 maxModel;					// fÌÅål
		D3DXVECTOR3 minModel;					// fÌÅ¬l
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
	// eLXgt@CÌf[^ÇÝÝÖW
	void ReadVector3(std::stringstream& ss, std::string name, D3DXVECTOR3& vec);
	void ReadColor(std::stringstream& ss, std::string name, D3DXCOLOR& color);
	bool ReadString(std::stringstream& ss, std::string name, std::string& tag);
	bool ReadInt(std::stringstream& ss, std::string name, int& value);
	void ReadFloat(std::stringstream& ss, std::string name, float& value);
	bool ReadBool(std::stringstream& ss, std::string name, bool& value);

	// ssÌóÔðNAµAÇÝæèÊuðæªÉß·
	void ssInitialize(std::stringstream& ss) { ss.clear(), ss.seekg(0); }

	std::string ProcessPath(const std::string& filePath);

	std::map<std::string, std::vector<SStageContainer>> m_stageData;
	std::vector<CObjectX*> m_pObject;
	std::string m_stagePath;
	size_t m_selectIndex;
	bool m_isSelected;
};

#endif
