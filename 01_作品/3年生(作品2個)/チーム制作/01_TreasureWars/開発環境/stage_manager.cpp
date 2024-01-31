#include "stage_manager.h"
#include "application.h"
#include "model.h"
#include "objectX.h"
#include "camera.h"

#include <algorithm>
#include <cctype>

namespace
{
	// �X�e�[�W�t�@�C���̃��f���^�O�p
	const std::vector<std::string> strPath = {
		"CHAIR", "CLOCK", "CUP", "FOUNDATION", "HEATER", "KETTLE", "LIGHT", "MET", "PICKEL", "RADIO",
		"REFRIGERATOR", "TABLE", "TV", "CUBE", "WALL_GATE", "WALL_WINDOW", "NOREN", "WALL_NOFLR"};
}

//��������������������������������������������������������������
// �X�e�[�W�}�l�[�W���̃R���X�g���N�^
//��������������������������������������������������������������
CStageManager::CStageManager()
{
}

//��������������������������������������������������������������
// �X�e�[�W�}�l�[�W���̃f�X�g���N�^
//��������������������������������������������������������������
CStageManager::~CStageManager()
{
}

//��������������������������������������������������������������
// �X�e�[�W�̐���
//��������������������������������������������������������������
void CStageManager::Create(std::string stagePath)
{
	m_stageData[stagePath];

	for(auto& p : m_stageData[stagePath])
	{
		m_pObject.push_back(CObjectX::Create(p.objectData.pos,
			p.objectData.rot,
			p.objectData.scale,
			ProcessPath(p.objectData.modelLabel).c_str()));
	}
}

//��������������������������������������������������������������
// �X�e�[�W�̃I�u�W�F�N�g�S�폜
//��������������������������������������������������������������
void CStageManager::Clear()
{
	if (!m_pObject.empty())
	{
		m_pObject.clear();
	}
}

//��������������������������������������������������������������
// �t�@�C���ǂݍ��݁itxt�j
//��������������������������������������������������������������
void CStageManager::LoadText(std::string path)
{
	std::map<std::string, std::vector<SStageContainer>> stageData = {};
	std::vector<std::string> tag(32, "");
	std::string name = "Data/text/" + path;
	std::ifstream file(name);
	std::string line;
	int index = 0;
	int tagIndex = 0;
	int dataIndex = 0;

	while (std::getline(file, line))
	{	// ��s���ǂݍ���
		// �󔒂��폜
		line.erase(std::remove_if(line.begin(), line.end(), [](char c) { return std::isspace(c); }), line.end());

		std::stringstream ss(line);

		if (ReadString(ss, "Tag", tag[tagIndex]))
		{
			dataIndex = tagIndex;
			tagIndex++;
			continue;
		}

		if (ReadInt(ss, "Index", index))		// �C���f�b�N�X
		{
			stageData[tag[dataIndex]].resize(index + 1);
			continue;
		}

		ReadVector3(ss, "Pos", stageData[tag[dataIndex]][index].objectData.pos);				// �ʒu
		ReadVector3(ss, "Rot", stageData[tag[dataIndex]][index].objectData.rot);				// ����
		ReadVector3(ss, "Scale", stageData[tag[dataIndex]][index].objectData.scale);			// �X�P�[��
		ReadString(ss, "ModelLabel", stageData[tag[dataIndex]][index].objectData.modelLabel);	// ���f���̃^�O
	}

	m_stageData = stageData;
}

//��������������������������������������������������������������
// �e�L�X�g�t�@�C���̃x�N�^�[�̓ǂݍ���
//��������������������������������������������������������������
void CStageManager::ReadVector3(std::stringstream& ss, std::string name, D3DXVECTOR3& vec)
{
	std::vector<float> num;
	std::string str;
	std::string result;
	float value;

	// �F��������܂ł̕����𒊏o
	size_t pos = ss.str().find(":");
	if (pos != std::string::npos)
	{
		result = ss.str().substr(0, pos);
	}

	// ss�̏�Ԃ��N���A���A�ǂݎ��ʒu��擪�ɖ߂�
	ssInitialize(ss);

	if (ss >> str && result == name)
	{
		ssInitialize(ss);

		// �擪����":"�܂œǂݔ�΂�
		std::getline(ss, str, ':');

		while (std::getline(ss, str, ','))
		{
			value = std::stof(str);
			num.push_back(value);
		}
	}

	if (num.size() == 3)
	{
		vec.x = num[0];
		vec.y = num[1];
		vec.z = num[2];
	}
}

//��������������������������������������������������������������
// �e�L�X�g�t�@�C���̐F�̓ǂݍ���
//��������������������������������������������������������������
void CStageManager::ReadColor(std::stringstream& ss, std::string name, D3DXCOLOR& color)
{
	std::vector<float> num;
	std::string str;
	std::string result;
	float value;

	// �F��������܂ł̕����𒊏o
	size_t pos = ss.str().find(":");
	if (pos != std::string::npos)
	{
		result = ss.str().substr(0, pos);
	}

	// ss�̏�Ԃ��N���A���A�ǂݎ��ʒu��擪�ɖ߂�
	ssInitialize(ss);

	if (ss >> str && result == name)
	{
		ssInitialize(ss);

		// �擪����":"�܂œǂݔ�΂�
		std::getline(ss, str, ':');

		while (std::getline(ss, str, ','))
		{
			value = std::stof(str);
			num.push_back(value);
		}
	}

	if (num.size() == 4)
	{
		color.r = num[0];
		color.g = num[1];
		color.b = num[2];
		color.a = num[3];
	}
}

//��������������������������������������������������������������
// �e�L�X�g�t�@�C���̕����̓ǂݍ���
//��������������������������������������������������������������
bool CStageManager::ReadString(std::stringstream& ss, std::string name, std::string& tag)
{
	std::vector<float> num;
	std::string str;
	std::string result;

	// �F��������܂ł̕����𒊏o
	size_t pos = ss.str().find(":");
	if (pos != std::string::npos)
	{
		result = ss.str().substr(0, pos);
	}

	// ss�̏�Ԃ��N���A���A�ǂݎ��ʒu��擪�ɖ߂�
	ssInitialize(ss);

	if (ss >> str && result == name)
	{
		ssInitialize(ss);
		std::string str_tag;

		// �擪����":"�܂œǂݔ�΂�
		std::getline(ss, str, ':');

		if (ss >> str_tag)
		{
			tag = str_tag;
		}
	}
	else
	{
		return false;
	}

	return true;
}

//��������������������������������������������������������������
// �e�L�X�g�t�@�C���̐����̓ǂݍ���
//��������������������������������������������������������������
bool CStageManager::ReadInt(std::stringstream& ss, std::string name, int& value)
{
	std::string str;
	std::string result;

	// �F��������܂ł̕����𒊏o
	size_t pos = ss.str().find(":");
	if (pos != std::string::npos)
	{
		result = ss.str().substr(0, pos);
	}

	// ss�̏�Ԃ��N���A���A�ǂݎ��ʒu��擪�ɖ߂�
	ssInitialize(ss);

	if (ss >> str && result == name)
	{
		ssInitialize(ss);

		// �擪����":"�܂œǂݔ�΂�
		std::getline(ss, str, ':');

		while (std::getline(ss, str, ','))
		{
			value = std::stoi(str);
		}
	}
	else
	{
		return false;
	}

	return true;
}

//��������������������������������������������������������������
// �e�L�X�g�t�@�C���̕��������̓ǂݍ���
//��������������������������������������������������������������
void CStageManager::ReadFloat(std::stringstream& ss, std::string name, float& value)
{
	std::string str;
	std::string result;

	// �F��������܂ł̕����𒊏o
	size_t pos = ss.str().find(":");
	if (pos != std::string::npos)
	{
		result = ss.str().substr(0, pos);
	}

	// ss�̏�Ԃ��N���A���A�ǂݎ��ʒu��擪�ɖ߂�
	ssInitialize(ss);

	if (ss >> str && result == name)
	{
		std::string str_random;

		ssInitialize(ss);

		// �擪����":"�܂œǂݔ�΂�
		std::getline(ss, str, ':');

		while (std::getline(ss, str, ','))
		{
			value = std::stof(str);
		}
	}
}

//��������������������������������������������������������������
// �e�L�X�g�t�@�C���̐^�U�̓ǂݍ���
//��������������������������������������������������������������
bool CStageManager::ReadBool(std::stringstream& ss, std::string name, bool& value)
{
	std::string str;
	std::string result;

	// �F��������܂ł̕����𒊏o
	size_t pos = ss.str().find(":");
	if (pos != std::string::npos)
	{
		result = ss.str().substr(0, pos);
	}

	// ss�̏�Ԃ��N���A���A�ǂݎ��ʒu��擪�ɖ߂�
	ssInitialize(ss);

	if (ss >> str && result == name)
	{
		ssInitialize(ss);
		std::string str_boolean;

		// �擪����":"�܂œǂݔ�΂�
		std::getline(ss, str, ':');

		if (ss >> str_boolean)
		{
			value = (str_boolean == "true");
		}
	}
	else
	{
		return false;
	}

	return true;
}

std::string CStageManager::ProcessPath(const std::string& filePath)
{
	auto it = std::find(strPath.begin(), strPath.end(), filePath);
	if (it == strPath.end())
	{
		return nullptr;
	}

	std::string modelFilePath = "Data/model/";
	switch (std::distance(strPath.begin(), it))
	{
	case 0:
		modelFilePath += "chair.x";
		break;

	case 1:
		modelFilePath += "clock.x";
		break;

	case 2:
		modelFilePath += "cup.x";
		break;

	case 3:
		modelFilePath += "foundation.x";
		break;

	case 4:
		modelFilePath += "heater.x";
		break;

	case 5:
		modelFilePath += "kettle.x";
		break;

	case 6:
		modelFilePath += "light.x";
		break;

	case 7:
		modelFilePath += "met.x";
		break;

	case 8:
		modelFilePath += "pickel.x";
		break;

	case 9:
		modelFilePath += "radio.x";
		break;

	case 10:
		modelFilePath += "refrigerator.x";
		break;

	case 11:
		modelFilePath += "table.x";
		break;

	case 12:
		modelFilePath += "tv.x";
		break;

	case 13:
		modelFilePath += "cube.x";
		break;

	case 14:
		modelFilePath += "wall_A.x";
		break;

	case 15:
		modelFilePath += "wall_W.x";
		break;

	case 16:
		modelFilePath += "noren.x";
		break;

	case 17:
		modelFilePath += "cube_ent.x";
		break;
	}

	return modelFilePath;
}