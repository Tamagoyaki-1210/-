#include "application.h"
#include "object3d.h"
#include "particle_manager.h"

#include <algorithm>
#include <cctype>

//��������������������������������������������������������������
//�p�[�e�B�N���}�l�[�W���̃R���X�g���N�^
//��������������������������������������������������������������
CParticleManager::CParticleManager()
{
}

//��������������������������������������������������������������
// �p�[�e�B�N���}�l�[�W���̃f�X�g���N�^
//��������������������������������������������������������������
CParticleManager::~CParticleManager()
{
}

//��������������������������������������������������������������
// �p�[�e�B�N���}�l�[�W���̏�����
//��������������������������������������������������������������
HRESULT CParticleManager::Init()
{
	return S_OK;
}

//��������������������������������������������������������������
// �p�[�e�B�N���}�l�[�W���̉��
//��������������������������������������������������������������
void CParticleManager::ReleaseAll()
{
	m_data.clear();
}

//��������������������������������������������������������������
// �e�L�X�g�t�@�C���̃f�[�^�ǂݍ���
//��������������������������������������������������������������
void CParticleManager::LoadText(std::string path)
{
	std::string name = "Data/text/" + path;
	std::ifstream file(name);

	std::map<std::string, std::vector<SBundleData>, std::less<>> bundleData = {};
	std::vector<std::string> tag(32, "");
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

		if (ReadInt(ss, "Index", index))														// �C���f�b�N�X
		{
			bundleData[tag[dataIndex]].resize(index + 1);
			continue;
		}

		bundleData[tag[dataIndex]][index].emitterData.emitterIndex = index;
		int behaviorIndex;
		int setRandomPosIndex;
		int lockPosIndex;
		int blendIndex;

		ReadBool(ss, "UseRandom", bundleData[tag[dataIndex]][index].info.useRandom);							// �����_�����g�p���邩�ǂ���
		ReadVector3(ss, "RandomVelocityMin", bundleData[tag[dataIndex]][index].info.random.randomVelocityMin);	// �ړ��ʂ̃����_���̍Œ�l
		ReadVector3(ss, "RandomVelocityMax", bundleData[tag[dataIndex]][index].info.random.randomVelocityMax);	// �ړ��ʂ̃����_���̍ő�l
		ReadFloat(ss, "RandomRotateMin", bundleData[tag[dataIndex]][index].info.random.randomRotate.y);			// ��]�̃����_���̍Œ�l
		ReadFloat(ss, "RandomRotateMax", bundleData[tag[dataIndex]][index].info.random.randomRotate.x);			// ��]�̃����_���̍ő�l
		ReadFloat(ss, "RandomWeightMin", bundleData[tag[dataIndex]][index].info.random.randomWeight.y);			// �d���̃����_���̍Œ�l
		ReadFloat(ss, "RandomWeightMax", bundleData[tag[dataIndex]][index].info.random.randomWeight.x);			// �d���̃����_���̍ő�l
		ReadFloat(ss, "DistortionMin", bundleData[tag[dataIndex]][index].info.random.distortion.y);
		ReadFloat(ss, "DistortionMax", bundleData[tag[dataIndex]][index].info.random.distortion.x);
		ReadInt(ss, "RandomCreate", setRandomPosIndex);															// �����_���Ȉʒu�ݒ�̗�
		ReadInt(ss, "LockPosition", lockPosIndex);																// �ʒu�̃��b�N�ݒ�
		ReadFloat(ss, "CreateRange", bundleData[tag[dataIndex]][index].info.circleDistance);					// �����ʒu�͈̔�
		ReadFloat(ss, "Altitude", bundleData[tag[dataIndex]][index].info.altitude);								// �����ʒu�̍��x�i�ʒu�̃I�t�Z�b�gY�ł��悢�j
		ReadVector3(ss, "PosOffset", bundleData[tag[dataIndex]][index].info.posOffset);							// �ʒu�̃I�t�Z�b�g
		ReadVector3(ss, "Velocity", bundleData[tag[dataIndex]][index].info.velocity);							// �ړ���
		ReadFloat(ss, "VelocityAttenuation", bundleData[tag[dataIndex]][index].info.moveAttenuation);			// �ړ��̌�����
		ReadInt(ss, "Behavior", behaviorIndex);																	// �����̗�
		ReadVector3(ss, "Scale", bundleData[tag[dataIndex]][index].info.scale);									// �傫��
		ReadColor(ss, "Color", bundleData[tag[dataIndex]][index].info.col);										// �F
		ReadColor(ss, "DestColor", bundleData[tag[dataIndex]][index].info.destCol);								// �ړI�̐F
		ReadFloat(ss, "ColorAttenuation", bundleData[tag[dataIndex]][index].info.colAttenuation);				// �F�̌�����
		ReadInt(ss, "BlendType", blendIndex);
		ReadVector3(ss, "Scaling", bundleData[tag[dataIndex]][index].info.scalingValue);						// �g�k��
		ReadFloat(ss, "Weight", bundleData[tag[dataIndex]][index].info.weight);									// �d��
		ReadFloat(ss, "Radius", bundleData[tag[dataIndex]][index].emitterData.radius);							// �~��
		ReadFloat(ss, "RotateAngle", bundleData[tag[dataIndex]][index].info.rotate.angle);						// ������̉�]��
		ReadFloat(ss, "RotateRadius", bundleData[tag[dataIndex]][index].info.rotate.radius);					// ������̔��a
		ReadFloat(ss, "RandomRotateMaxBH", bundleData[tag[dataIndex]][index].info.rotate.randomMax);				// ��]�ʂ̃����_���̍ő�l
		ReadFloat(ss, "RandomRotateMinBH", bundleData[tag[dataIndex]][index].info.rotate.randomMin);				// ��]�ʂ̃����_���̍Œ�l
		ReadInt(ss, "PopTime", bundleData[tag[dataIndex]][index].info.popTime);									// ��������
		ReadInt(ss, "Quantity", bundleData[tag[dataIndex]][index].emitterData.popParticleNum);					// ������
		ReadString(ss, "TextureTag", bundleData[tag[dataIndex]][index].emitterData.texTag);						// �e�N�X�`���̃^�O
		ReadBool(ss, "Trail", bundleData[tag[dataIndex]][index].info.useTrail);									// �g���C�����g�p���邩�ǂ���

		bundleData[tag[dataIndex]][index].emitterData.behavior = (CParticleEmitter::EBehavior)behaviorIndex;
		bundleData[tag[dataIndex]][index].info.blendState = (CParticle::EBlend)blendIndex;
		bundleData[tag[dataIndex]][index].info.random.randomBehavior = (CParticle::ERandomBehavior)setRandomPosIndex;
		bundleData[tag[dataIndex]][index].info.random.randomLockVector = (CParticle::ELockVector)lockPosIndex;
	}

	int cnt = 0;
	std::string old_tag;
	for (size_t i = 0; i < bundleData.size(); i++)
	{
		if (tag[i] == "")
		{
			break;
		}

		if (tag[i] != old_tag)
		{
			cnt = 0;
		}

		old_tag = tag[i];
		m_data[tag[i]] = std::move(bundleData[tag[i]]);
		cnt++;
	}
}

//��������������������������������������������������������������
// �e�L�X�g�t�@�C���̃x�N�^�[�̓ǂݍ���
//��������������������������������������������������������������
void CParticleManager::ReadVector3(std::stringstream& ss, std::string name, D3DXVECTOR3& vec)
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
void CParticleManager::ReadColor(std::stringstream& ss, std::string name, D3DXCOLOR& color)
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
bool CParticleManager::ReadString(std::stringstream& ss, std::string name, std::string& tag)
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
bool CParticleManager::ReadInt(std::stringstream& ss, std::string name, int& value)
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
void CParticleManager::ReadFloat(std::stringstream& ss, std::string name, float& value)
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
bool CParticleManager::ReadBool(std::stringstream& ss, std::string name, bool& value)
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

//��������������������������������������������������������������
// �f�[�^��ݒ肷�鏈��
//��������������������������������������������������������������
void CParticleManager::SetParticleData(std::string& str, std::vector<SBundleData>& info)
{
	m_data[str] = info;
}

//��������������������������������������������������������������
// ���o�����f�[�^��Ԃ�����
//��������������������������������������������������������������
std::vector<CParticleManager::SBundleData> CParticleManager::GetParticleData(std::string str)
{
	if (m_data.count(str) == 0)
	{	//���f���f�[�^�������ĂȂ������ꍇ
		assert(false);
	}

	return m_data[str];
}