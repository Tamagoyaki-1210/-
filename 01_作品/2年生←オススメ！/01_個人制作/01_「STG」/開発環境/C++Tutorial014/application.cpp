//=============================================================================
//
// �}�l�[�W���[
// Author : tanimoto kosuke
//
//=============================================================================
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "texture.h"
#include "fade.h"
#include "mode.h"

CRenderer *CApplication::m_pRenderer = nullptr;	// �����_���[�|�C���^
CInput *CApplication::m_pInput = nullptr;		// ���̓|�C���^
CTexture *CApplication::m_pTexture = nullptr;	// �摜�|�C���^
CMode *CApplication::m_pMode = nullptr;			// ���[�h�|�C���^
CSound *CApplication::m_pSound = nullptr;		// �T�E���h�|�C���^

//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CApplication::CApplication()
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CApplication::~CApplication()
{

}

//=====================================
// ����������
//=====================================
HRESULT CApplication::Init(HINSTANCE hInstance, HWND hWnd)
{
	m_pRenderer = new CRenderer;
	// true : �E�B���h�E�T�C�Y, false : �S���
	if (FAILED(m_pRenderer->Init(hWnd, true)))
	{
		return -1;
	}

	m_pInput = new CInput;
	// �L�[�{�[�h�̏���������
	if (FAILED(m_pInput->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	m_pTexture = new CTexture;
	m_pTexture->Load();

	m_pMode = new CMode;
	//�ŏ��̃��[�h
	m_pMode->FirstMode(CMode::MODE_TITLE);

	m_pSound = new CSound;
	m_pSound->Init(hWnd);

	return S_OK;
}

//=====================================
// �I������
//=====================================
void CApplication::Uninit()
{
	// �摜
	if (m_pTexture != nullptr)
	{
		m_pTexture->Uninit();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	// ����
	if (m_pInput != nullptr)
	{
		m_pInput->Uninit();
		delete m_pInput;
		m_pInput = nullptr;
	}

	// �����_�����O
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// ���[�h
	if (m_pMode != nullptr)
	{
		m_pMode->Uninit();
		delete m_pMode;
		m_pMode = nullptr;
	}

	// �T�E���h
	if (m_pSound != nullptr)
	{
		CSound::StopSound();
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}
}

//=====================================
//�X�V����
//=====================================
void CApplication::Update()
{
	m_pInput->Update();
	m_pRenderer->Update();
	m_pMode->Update();
	m_pMode->SetMode();
}

//=====================================
//�`�揈��
//=====================================
void CApplication::Draw()
{
	m_pRenderer->Draw();
	m_pMode->Draw();
}