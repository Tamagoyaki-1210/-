//==============================================================================
//
// Map.cpp
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// �C���N���[�h
//==============================================================================
#include "application.h"
#include "renderer.h"
#include "map.h"
#include "objectX.h"
#include "3Dpolygon.h"
#include "gimmick_model.h"

//==============================================================================================
// ��`
//==============================================================================================
const char* CMap::MAP_OBSTACLE_OBJECT_FILENAME[] =
{// �}�b�v��Q���I�u�W�F�N�g�t�@�C���̃p�X
	"data/text/map_data/map_obstacle_object_data_00.txt",						// �}�b�v�P
	"data/text/map_data/map_obstacle_object_data_01.txt",						// �}�b�v�Q
	"data/text/map_data/map_obstacle_object_data_02.txt",						// �}�b�v�R
};

const char* CMap::MAP_BACKGROUND_OBJECT_FILENAME[] =
{// �}�b�v�w�i�I�u�W�F�N�g�t�@�C���̃p�X
	"data/text/map_data/map_background_object_data_00.txt",						// �}�b�v�P
	"data/text/map_data/map_background_object_data_01.txt",						// �}�b�v�Q
	"data/text/map_data/map_background_object_data_02.txt",						// �}�b�v�R
};

const float CMap::Map_Collision_Decreace = 10.0f;		// �}�b�v�I�u�W�F�N�g�̓����蔻�������������l
CTrolley_Manager *CMap::m_pTrolley_Manager = nullptr;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CMap::CMap()
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CMap::~CMap()
{
}

//==============================================================================
// ����������
//==============================================================================
HRESULT CMap::Init(int nMopNum)
{
	m_pFloor = C3DPolygon::Create(D3DXVECTOR3(800.0f,0.0f,800.0f), D3DXVECTOR3(D3DXToRadian(0), 0.0f, 0.0f),D3DXVECTOR2(2000.0f, 2000.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
	
	//�e�X�e�[�W���Ƃɒn�ʂ̃e�N�X�`����ς���
	switch (nMopNum)
	{
	case 0:
		m_pFloor->SetTextIndex(CTexture::TEXTURE_FLOOR);
		break;
	case 1:
		m_pFloor->SetTextIndex(CTexture::TEXTURE_JUNGLE);
		break;
	case 2:
		m_pFloor->SetTextIndex(CTexture::TEXTURE_MOON);
		break;
	default:
		assert(false);
		break;
	}

	//�}�b�v�ԍ��̕ۑ�
	m_nMapNum = nMopNum;
	// �}�b�v�ǂݍ���
	LoadMap(true, MAP_OBSTACLE_OBJECT_FILENAME[m_nMapNum]);
	LoadMap(false, MAP_BACKGROUND_OBJECT_FILENAME[m_nMapNum]);

	// �g���b�R�̃��[�g�̐ݒ�
	Trolley_Route();

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CMap::Uninit()
{
	//���X�g�̍폜
	m_MapObjectList.clear();

	// �g���b�R�}�l�[�W���[�̏I��
	if (m_pTrolley_Manager != nullptr)
	{
		m_pTrolley_Manager->Uninit();
		m_pTrolley_Manager = nullptr;
	}
}

//==============================================================================
// �X�V����
//==============================================================================
void CMap::Update()
{
	
}

//==============================================================================
// �`�揈��
//==============================================================================
void CMap::Draw()
{
	
}

//==============================================================================
// �}�b�v���̃I�u�W�F�N�g�Ƃ̓����蔻��
//==============================================================================
D3DXVECTOR3 CMap::CollisionMapObject(const D3DXVECTOR3 pos, const float radius)
{
	// �����߂��l
	D3DXVECTOR3 add = {0.0f,0.0f,0.0f};

	// �C�e���[�^�[���[�v
	for (auto pObjectX : m_MapObjectList)
	{
		// �}�b�v���̃I�u�W�F�N�g�̃|�C���^�̉��
		if (pObjectX == nullptr)
		{
			assert(false);
		}

		// �}�b�v���̃I�u�W�F�N�g�̏��
		const D3DXVECTOR3 atherPos = pObjectX->GetPos();
		const float atherRadius = pObjectX->GetMax().x - Map_Collision_Decreace;

		// �Փˌ��m
		if (pObjectX->CollisionCircle(pos, radius, atherPos, atherRadius))
		{
			// �x�N�g���̎Z�o
			D3DXVECTOR3 vec = pos - atherPos;

			// �x�N�g���̕������v�Z
			D3DXVECTOR3 vecNolmalize;
			D3DXVec3Normalize(&vecNolmalize, &vec);

			// �}�b�v�I�u�W�F�N�g�ƃL�����̑傫���̃x�N�g�� * �x�N�g���̕���
			D3DXVECTOR3 vec2 = {(atherRadius + radius) * vecNolmalize.x, 0.0f, (atherRadius + radius) * vecNolmalize.z };

			// ���Z����
			add += vec2 - vec;
		}
	}

	return add;
}

//==============================================================================
// �}�b�v�ǂݍ���
//==============================================================================
void CMap::LoadMap(bool bCollision, const char* s_FileName)
{
	FILE *pFile = NULL;			//�t�@�C���|�C���^�[�錾

	char cBff[LINE_MAX_READING_LENGTH];		//��s���ǂݎ�邽�߂̕ϐ�
	char cBffHead[LINE_MAX_READING_LENGTH];	//���̕�����ǂݎ�邽�߂̕ϐ�

	char cListBffFile[128][LINE_MAX_READING_LENGTH];	//�t�@�C�����̓ǂݎ��p

	//�o�^�������f���̐�
	int nRegistrationModel = 0;

	//��ʒu
	D3DXVECTOR3 MapPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���f���̃T�C�Y
	D3DXVECTOR3 ModelSize = D3DXVECTOR3(100.0f, 0.0f, 100.0f);

	//�t�@�C�����J��
	pFile = fopen(s_FileName, "r");

	if (pFile == nullptr)
	{//�J���Ȃ��������p
		assert(false);
	}

	//������̓ǂݎ�胋�[�v����
	while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != nullptr)
	{
		//������̕���
		sscanf(cBff, "%s", &cBffHead);

		if (strcmp(&cBffHead[0], "NUM_MODEL") == 0)
		{//�ǂݍ��ރ��f�����̓ǂݍ���

			//������̕���
			sscanf(cBff, "%s = %d", &cBffHead, &m_nMapTypeMax);
		}
		else if (strcmp(&cBffHead[0], "MODEL_FILENAME") == 0)
		{//�ǂݍ��ރ��f�����̓ǂݍ���

		 //�o�^�\�萔��葽���Ȃ����G���[
			if (nRegistrationModel >= m_nMapTypeMax)
			{
				assert(false);
			}

			//������̕���
			sscanf(cBff, "%s = %s", &cBffHead, &cListBffFile[nRegistrationModel]);

			//�o�^����
			nRegistrationModel++;

		}
		else if (strcmp(&cBffHead[0], "MAP_NUM_MAX") == 0)
		{//�ǂݍ��ރ}�b�v�̃T�C�Y
			//������̕���
			sscanf(cBff, "%s = %d", &cBffHead, &m_nMapSizeMax);

		}
		else if (strcmp(&cBffHead[0], "MAP_XZ") == 0)
		{//�ǂݍ��ރ}�b�v�̃f�[�^

		 //�o�^�����}�b�v�ԍ���
			int nSetMapNum = 0;

			//������̓ǂݎ�胋�[�v����
			while (fgets(cBff, LINE_MAX_READING_LENGTH, pFile) != nullptr)
			{
				//�`�F�b�N
				if (nSetMapNum >= m_nMapSizeMax * m_nMapSizeMax)
				{
					break;
				}

				//�ꎞ�ۑ�
				int nMapNum = 0;
				D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				//������̕���
				sscanf(cBff, "%d %f %f %f", &nMapNum ,&rot.x ,&rot.y ,&rot.z);

				//�I�u�W�F�N�g������ꍇ
				if (nMapNum != 0)
				{
					//���̓o�^
	
					CObjectX* pObjectX = CObjectX::Create(MapPos, rot, { 1.0f,1.0f,1.0f }, cListBffFile[nMapNum]);

					//������̂����Q���ɂ��邩�A�����w�i�p�ɂ��邩
					if (bCollision)
					{
						//���X�g�ɏ���ǉ�
						m_MapObjectList.push_back(pObjectX);
					}
				}

				//�ʒu�̍X�V
				if ((nSetMapNum % m_nMapSizeMax) == m_nMapSizeMax - 1)
				{
					MapPos.x += ModelSize.x;
					MapPos.z = 0.0f;
				}
				else
				{
					MapPos.z += ModelSize.z;
				}

				//�o�^�����ԍ�
				nSetMapNum++;
			}
		}
		else if (strcmp(&cBffHead[0], "END_SCRIPT") == 0)
		{//�X�N���v�g�̏I���
			break;
		}

		//�ۑ����̕�����̏�����
		ZeroMemory(&cBff, sizeof(cBff));
		ZeroMemory(&cBffHead, sizeof(cBffHead));
	}

	//�t�@�C�������
	fclose(pFile);
}

//==============================================================================
// �g���b�R�̃��[�g�̐ݒ�
//==============================================================================
void CMap::Trolley_Route()
{
	// �g���b�R�}�l�[�W���[
	switch (m_nMapNum)
	{
	case 0:
		// �X�e�[�W1
		m_pTrolley_Manager = CTrolley_Manager::Create(1, 0, "Data/model/Motion_Gimmick/motion_gimmick.txt");
		break;
	case 1:
		// �X�e�[�W2
		m_pTrolley_Manager = CTrolley_Manager::Create(2, 0, "Data/model/Motion_Triceratops/motion_triceratops.txt");
		break;
	case 2:
		// �X�e�[�W3
		m_pTrolley_Manager = CTrolley_Manager::Create(0, 0, "Data/model/Motion_Space_Probe/motion_space_probe.txt");
		break;
	default:
		break;
	}

	// �g���b�R�}�l�[�W���[�̊m���ɐ��������ꍇ
	if (m_pTrolley_Manager != nullptr)
	{
		// �g���b�R�̏���ݒ肷��
		m_pTrolley_Manager->GetGimmick_Model()->SetTrolleyInfomation();
	}
}