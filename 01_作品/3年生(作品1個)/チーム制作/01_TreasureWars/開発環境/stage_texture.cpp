#include "stage_texture.h"
#include "application.h"
#include "renderer.h"

//��������������������������������������������������������������
// �X�e�[�W�e�N�X�`���̃R���X�g���N�^
//��������������������������������������������������������������
CStageTexture::CStageTexture()
{
}

//��������������������������������������������������������������
// �X�e�[�W�e�N�X�`���̃f�X�g���N�^
//��������������������������������������������������������������
CStageTexture::~CStageTexture()
{
}

//��������������������������������������������������������������
// �X�e�[�W�̃e�N�X�`���̉��
//��������������������������������������������������������������
void CStageTexture::Release()
{
	if (!m_texturePath.empty())
	{
		m_texturePath.clear();
	}
	if (!m_texture.empty())
	{
		m_texture.clear();
	}
}

//��������������������������������������������������������������
// �e�N�X�`���ǂݍ���
//��������������������������������������������������������������
LPDIRECT3DTEXTURE9 CStageTexture::Load(std::string fileName)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �|�C���^�̍Ō�܂łɌ��������ꍇ�e�N�X�`���f�[�^��Ԃ�
	auto it = m_texture.find(fileName);
	if (it != m_texture.end())
	{
		return it->second;
	}

	// �e�N�X�`���̓ǂݍ���
	LPDIRECT3DTEXTURE9 texture = nullptr;
	D3DXCreateTextureFromFile(pDevice, fileName.c_str(), &texture);

	if (texture != nullptr)
	{
		m_texturePath.push_back(fileName);
		m_texture[fileName] = texture;
		return texture;
	}
	return nullptr;
}

//��������������������������������������������������������������
// ���ɓ����p�X���i�[����Ă���m�F���鏈��
//��������������������������������������������������������������
bool CStageTexture::CheckDuplicateTex(std::string path)
{
	return std::count(m_texturePath.begin(), m_texturePath.end(), path) > 0;
}