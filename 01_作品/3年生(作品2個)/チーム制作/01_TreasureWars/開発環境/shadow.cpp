//=============================================================================
//
// shadow.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "shadow.h"
#include "application.h"
#include "renderer.h"

//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CShadow::CShadow() : CObject3D(1)
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CShadow::~CShadow()
{

}

//=====================================
// ����������
//=====================================
HRESULT CShadow::Init()
{
	CObject3D::Init();

	SetCol({1.0f, 1.0f, 1.0f, 1.0f});
	Setbillboard(true);

	return S_OK;
}

//=====================================
// �I������
//=====================================
void CShadow::Uninit()
{
	CObject3D::Uninit();
}

//=====================================
// �X�V����
//=====================================
void CShadow::Update()
{
	CObject3D::Update();
}

//=====================================
// �`�揈��
//=====================================
void CShadow::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject3D::Draw();

	//�ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//============================================================================
// �e�̏���
//============================================================================
void CShadow::SetShadow(const D3DXVECTOR3 pos)
{
	SetPos({ pos.x, 0.0f, pos.z });

	// �����ŐF��ύX
	SetCol({ 1.0f, 1.0f, 1.0f, 1.0f - (pos.y * 0.005f) });
}

//=====================================
// ��������
//=====================================
CShadow* CShadow::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CShadow* pShadow = new CShadow;

	if (FAILED(pShadow->Init()))
	{
		return nullptr;
	}

	pShadow->SetPos(pos);
	pShadow->SetSize(size);
	pShadow->SetTexture(CTexture::TEXTURE_SHADOW);

	return pShadow;
}