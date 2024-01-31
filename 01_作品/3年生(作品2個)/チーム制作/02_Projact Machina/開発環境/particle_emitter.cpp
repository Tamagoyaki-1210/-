#include "particle_emitter.h"
#include "particle_manager.h"
#include "particle.h"
#include "application.h"
#include "utility.h"

namespace
{
	// �~�͈͓̔��Ń����_���Ɉʒu��ݒ�
	inline D3DXVECTOR3 CircleRandomPos(const D3DXVECTOR3& pos, float circledistance, float altitude)
	{
		D3DXVECTOR3 result(0.0f, 0.0f, 0.0f);
		float angle = (float)rand() / (float)RAND_MAX * 2.0f * D3DX_PI;
		float distance = sqrtf((float)rand() / (float)RAND_MAX) * (360.0f * circledistance);

		result = D3DXVECTOR3(pos.x + distance * cosf(angle),
			pos.y + altitude,
			pos.z + distance * sinf(angle));

		return result;
	}

	// ������o�v�Z
	inline void EmitSphere(D3DXVECTOR3& vec, int num, int numAll, float radius, D3DXVECTOR2& distortion, float attenuation)
	{
		float inclination = acosf(1.0f - (2.0f * (num + 0.5f) / numAll));	// �X��
		float azimuth = 2.0f * D3DX_PI * num / (1.0f + sqrtf(5.0f));		// ���ʊp
		azimuth *= utility::Random<float>(distortion.x, distortion.y);

		// ����ɕ��o
		vec = D3DXVECTOR3(radius * sinf(inclination) * cosf(azimuth),
			radius * cosf(inclination),
			radius * sinf(inclination) * sinf(azimuth)) * attenuation;
	}
}

//��������������������������������������������������������������
// �p�[�e�B�N���G�~�b�^�̃R���X�g���N�^
//��������������������������������������������������������������
CParticleEmitter::CParticleEmitter()
{
}

//��������������������������������������������������������������
// �p�[�e�B�N���G�~�b�^�̃f�X�g���N�^
//��������������������������������������������������������������
CParticleEmitter::~CParticleEmitter()
{
}

//��������������������������������������������������������������
// �p�[�e�B�N���G�~�b�^�̐���
//��������������������������������������������������������������
CParticleEmitter* CParticleEmitter::Create(std::string str, const D3DXVECTOR3 pos)
{
	CParticleEmitter *pEmitter = nullptr;
	pEmitter = new CParticleEmitter;

	CParticleManager *pManager = CApplication::GetParticleManager();
	std::vector<CParticleManager::SBundleData> data;

	if (pEmitter != nullptr)
	{
		data = std::move(pManager->GetParticleData(str));

		for (auto &p : data)
		{
			pEmitter->m_particleInfo.push_back(p.info);
			pEmitter->m_emitterInfo.push_back(p.emitterData);
		}
		pEmitter->Init();
		pEmitter->SetPos(pos);
	}

	return pEmitter;
}

//��������������������������������������������������������������
// �p�[�e�B�N���G�~�b�^�̏�����
//��������������������������������������������������������������
HRESULT CParticleEmitter::Init()
{
	// �p�[�e�B�N���̐���
	for (int i = 0; i < (int)m_emitterInfo.size(); i++)
	{
		CreateParticle(i);
	}

	return S_OK;
}

//��������������������������������������������������������������
// �p�[�e�B�N���G�~�b�^�̏I��
//��������������������������������������������������������������
void CParticleEmitter::Uninit()
{
	if (!m_particle.empty())
	{
		for (size_t i = 0; i < m_particle.size(); i++)
		{
			m_particle[i]->Uninit();
			m_particle[i] = nullptr;
		}
		m_particle.clear();
	}

	if (!m_particleInfo.empty())
	{
		m_particleInfo.clear();
	}

	this->Release();
}

//��������������������������������������������������������������
// �p�[�e�B�N���G�~�b�^�̍X�V
//��������������������������������������������������������������
void CParticleEmitter::Update()
{
	for (auto it = m_particle.begin(); it != m_particle.end();)
	{
		CParticle *pParticle = *it;

		if (pParticle->isExpired())
		{
			pParticle->Uninit();
			it = m_particle.erase(it);
		}
		else
		{
			it++;
		}
	}

	// �S�Ẵp�[�e�B�N���f�[�^���������ꍇ
	if (m_particle.empty())
	{
		CParticleEmitter::Uninit();
	}
}

//��������������������������������������������������������������
// �r���{�[�h�p�[�e�B�N���̐���
//��������������������������������������������������������������
void CParticleEmitter::CreateParticle(int index)
{
	D3DXVECTOR3 velocity = m_particleInfo[m_particleIndex].velocity;
	float radius = m_emitterInfo[index].radius;
	float attenuation = m_particleInfo[m_particleIndex].moveAttenuation;
	m_particle.reserve(m_emitterInfo[index].popParticleNum);

	for (int j = 0; j < m_emitterInfo[index].popParticleNum; j++)
	{
		switch (m_emitterInfo[index].behavior)
		{
		case BEHAVIOR_CIRCLE:
			// �~��ɕ��o
			m_move = D3DXVECTOR3(-sinf((j * ((radius / m_emitterInfo[index].popParticleNum) * (D3DX_PI / 180)))),
				velocity.y,
				-cosf((j * ((radius / m_emitterInfo[index].popParticleNum) * (D3DX_PI / 180))))) * attenuation;
			break;

		case BEHAVIOR_SPHERE:
			EmitSphere(m_move, j, m_emitterInfo[index].popParticleNum, radius, m_particleInfo[m_particleIndex].random.distortion, attenuation);
			break;

		case BEHAVIOR_ROTATE:
			m_move = velocity;
			m_particleInfo[m_particleIndex].rotate.useRotate = true;
			break;

		case BEHAVIOR_NONE:
			m_move = velocity;
			break;

		default:
			break;
		}

		m_particleInfo[m_particleIndex].velocity = m_move;
		m_particle.push_back(CParticle::Create(m_particleInfo[m_particleIndex], m_emitterInfo[index].texTag));
	}

	m_particleIndex++;
}

//��������������������������������������������������������������
// �p�[�e�B�N���G�~�b�^�̈ʒu�ݒ�
//��������������������������������������������������������������
void CParticleEmitter::SetPos(D3DXVECTOR3 pos)
{
	if (m_particle.empty())
	{
		return;
	}

	for (auto& p : m_particle)
	{
		CParticle::SInfo info = {};
		info = p->GetInfo();

		D3DXVECTOR3 currentPos = pos + info.posOffset;
		D3DXVECTOR3 calcPos = CircleRandomPos(pos, info.circleDistance, info.altitude);

		switch (info.random.randomBehavior)
		{
		case CParticle::RandomBehavior_Default:
			p->SetPos(currentPos);
			break;

		case CParticle::RandomBehavior_Circle:	// �~�͈͓̔��Ń����_���ɐ���
			switch (info.random.randomLockVector)
			{
			case CParticle::Lock_None:
				p->SetPos(calcPos);
				break;
			case CParticle::Lock_X:
				p->SetPos(D3DXVECTOR3(currentPos.x, calcPos.y, calcPos.z));
				break;
			case CParticle::Lock_Y:
				p->SetPos(D3DXVECTOR3(calcPos.x, currentPos.y, calcPos.z));
				break;
			case CParticle::Lock_Z:
				p->SetPos(D3DXVECTOR3(calcPos.x, calcPos.y, currentPos.z));
				break;
			case CParticle::Lock_XY:
				p->SetPos(D3DXVECTOR3(currentPos.x, currentPos.y, calcPos.z));
				break;
			case CParticle::Lock_XZ:
				p->SetPos(D3DXVECTOR3(currentPos.x, calcPos.y, currentPos.z));
				break;
			case CParticle::Lock_YZ:
				p->SetPos(D3DXVECTOR3(calcPos.x, currentPos.y, currentPos.z));
				break;
			default:
				assert(false);
				break;
			}
			break;

		default:
			assert(false);
			break;
		}
	}
}

//��������������������������������������������������������������
// �p�[�e�B�N���G�~�b�^�̈ʒu�擾
//��������������������������������������������������������������
D3DXVECTOR3 CParticleEmitter::GetPos()
{
	return m_particle[0]->GetPos();
}