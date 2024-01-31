//=============================================================================
//
// font.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "font.h"

//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CFont::CFont(const PRIORITY priority) : CObject2D(priority)
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CFont::~CFont()
{

}

//=====================================
// ����������
//=====================================
HRESULT CFont::Init()
{
	// ��{�N���X�̏���������
	if (FAILED(CObject2D::Init()))
	{
		return -1;
	}

	return S_OK;
}

//=====================================
// �I������
//=====================================
void CFont::Uninit()
{
	// ��{�N���X�̏I������
	CObject2D::Uninit();
}

//=====================================
// �X�V����
//=====================================
void CFont::Update()
{
	// ��{�N���X�̍X�V����
	CObject2D::Update();
}

//=====================================
// �`�揈��
//=====================================
void CFont::Draw()
{
	// ��{�N���X�̕`�揈��
	CObject2D::Draw();
}

//=====================================
// ������ݒ�
//=====================================
void CFont::SetString(const char* letter, const D3DXCOLOR col)
{
	// ���{��̏ꍇ
	if (letter[0] <= -125 && letter[0] >= -127)
	{
		SetTexture(CTexture::TEXTURE_FONT_JAPANESE_VER2);		//�e�N�X�`���̐ݒ�
		SetAnimDiv(Japanese_Font_Column, Japanese_Font_Line);

		// �A�j���[�V�����p�^�[���̐ݒ�
		int Cell = 0;

		// �Ђ炪��(-126)�Ɣz��[0]����v����ꍇ
		if (letter[0] == -126)
		{
			int aLetter = -97;	// �ŏ��̕������n�܂�ʒu(-97)

			for (int nCnt = 0; nCnt < 83; nCnt++)
			{// �摜�̂Ђ炪��83��������1��؂���
				if (letter[1] == aLetter + nCnt)
				{// ���͂��ꂽ�����ƌ��݂̕�������v�����ꍇ
					Cell = nCnt;	// �����̈ʒu����
					break;
				}
			}

			// "��"�܂őΉ�
			if (Cell < 0 || Cell > 83)
			{
				Release();
				return;
			}
			else
			{
				SetAnimControl(Cell);	// �摜�̎w�肳�ꂽ�����̕����݂̂�؂���
			}
		}
		// �J�^�J�i(-125)�Ɣz��[0]����v����ꍇ
		else if (letter[0] == -125)
		{
			// "�~"�܂ł̃J�^�J�i
			if (letter[1] >= 64)
			{
				int aLetter = 64;	// �ŏ��̕������n�܂�ʒu(64)
				int aKatakana = 96;	// �J�^�J�i���n�܂�ʒu(96)

				for (int nCnt = 0; nCnt < 63; nCnt++)
				{// �摜�̃J�^�J�i63��������1��؂���
					if (letter[1] == aLetter + nCnt)
					{// ���͂��ꂽ�����ƌ��݂̕�������v�����ꍇ
						Cell = nCnt + aKatakana;	// �����̈ʒu����
						break;
					}
				}

				if (Cell < aKatakana || Cell > aKatakana + 63)
				{// �摜�̃J�^�J�i63��������1��؂���
					Release();
					return;
				}
				else
				{
					SetAnimControl(Cell);	// �摜�̎w�肳�ꂽ�����̕����݂̂�؂���
				}
			}
			// "��"�ȍ~�̃J�^�J�i
			else
			{
				int aLetter = -128;		// �ŏ��̕������n�܂�ʒu(-128)
				int aKatakana = 159;	// �J�^�J�i���n�܂�ʒu(159)

				for (int nCnt = 0; nCnt < 23; nCnt++)
				{// �摜�̃J�^�J�i23��������1��؂���
					if (letter[1] == aLetter + nCnt)
					{// ���͂��ꂽ�����ƌ��݂̕�������v�����ꍇ
						Cell = nCnt + aKatakana;	// �����̈ʒu����
						break;
					}
				}

				if (Cell < aKatakana || Cell > aKatakana + 23)
				{// �摜�̃J�^�J�i23��������1��؂���
					Release();
					return;
				}
				else
				{
					SetAnimControl(Cell);	// �摜�̎w�肳�ꂽ�����̕����݂̂�؂���
				}
			}
		}
		// �L��(-127)�Ɣz��[0]����v����ꍇ
		else if (letter[0] == -127)
		{
			if (letter[1] == -127)
			{
				SetAnimControl(95);	// ��
			}
			else if (letter[1] == 64)
			{
				SetAnimControl(199);	// �S�p�X�y�[�X
			}
			else if (letter[1] == 69)
			{
				SetAnimControl(186);	// �E
			}
			else if (letter[1] == 91)
			{
				SetAnimControl(187);	// �[
			}
			else if (letter[1] == 117)
			{
				SetAnimControl(191);	// �u
			}
			else if (letter[1] == 118)
			{
				SetAnimControl(192);	// �v
			}
		}
	}
	else
	{
		char aLetter = 33;				//Unicode��!(10�i��)

		SetTexture(CTexture::TEXTURE_FONT_ALPHABET);		//�e�N�X�`���̐ݒ�
		SetAnimDiv(Alphabet_Font_Column, Alphabet_Font_Line);

		//�A�j���[�V�����p�^�[���̐ݒ�
		int Cell = 0;

		for (int nCnt = 0; nCnt < 93; nCnt++)
		{//~�܂Ŋm�F����
			if (letter[0] == aLetter)
			{//������������A�ԍ���ۑ����āA�~�܂�
				Cell = nCnt;
				break;
			}

			aLetter += 1;
		}

		if (Cell < 0 || Cell > 93)
		{//�͈͊O�̕�����������Afalse��Ԃ�
			Release();
			return;
		}
		else
		{//�ϊ��o������A�A�j���[�V�����p�^�[����ݒ肵�āAtrue��Ԃ�
			SetAnimControl(Cell);
		}
	}

	// �F�̐ݒ�
	SetCol(col);
}

//=====================================
// ��������
//=====================================
CFont* CFont::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const char* letter, const PRIORITY priority, const D3DXCOLOR col)
{
	CFont* pFont = new CFont(priority);

	// ����������
	if (FAILED(pFont->Init()))
	{
		return nullptr;
	}

	pFont->SetPos(pos);
	pFont->SetSize(size);

	// �����̐ݒ�
	pFont->SetString(letter, col);

	return pFont;
}