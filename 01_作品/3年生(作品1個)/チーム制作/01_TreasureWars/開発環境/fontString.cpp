//=============================================================================
//
// fontString.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "fontString.h"
#include "Application.h"

//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CFontString::CFontString()
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CFontString::~CFontString()
{

}

//=====================================
// ����������
//=====================================
HRESULT CFontString::Init(void)
{
	m_fScale = 0.0f;
	m_bScale = false;
	m_bScaleChange = false;
	m_bBlink = false;
	m_bBlinkScale = false;
	m_fBlinkSpeed = 0.0f;
	m_fBlinkAlpha = 1.0f;

	return S_OK;
}

//=====================================
// �I������
//=====================================
void CFontString::Uninit(void)
{
	// �����I�u�W�F�N�g����ɂȂ�܂ŏI��������
	while (!m_apFont.empty())
	{
		// ���̃I�u�W�F�N�g2D�����������
		m_apFont.back()->Uninit();
		m_apFont.pop_back();
	}

	Release();
}

//=====================================
// �X�V����
//=====================================
void CFontString::Update(void)
{
	// �ʒu����v���Ȃ��ꍇ
	if (m_pos != m_anchor)
	{
		if ((m_pos.x - m_anchor.x < 0.1f) && (m_pos.x - m_anchor.x > -0.1f) && (m_pos.y - m_anchor.y < 0.1f) && (m_pos.y - m_anchor.y > -0.1f))
		{
			m_pos = m_anchor;
		}
		else
		{
			m_pos += (m_anchor - m_pos) * Anchor_Damping;
		}
		SetFont();
	}

	// �T�C�Y�ύX����
	if (m_bScale == true)
	{
		SizeScale();
	}

	// �t�H���g�S�̂̓_�ŏ���
	if (m_bBlink == true)
	{
		BlinkColor();
	}
}

//=====================================
// �`�揈��
//=====================================
void CFontString::Draw(void)
{

}

//=====================================
// �T�C�Y�ύX����
//=====================================
void CFontString::SizeScale(void)
{
	// �������g�債�Ă���ꍇ
	if (m_bScaleChange == false)
	{
		// �������g�傳����
		m_fScale += ScaleSpeed;

		// ���������̒l�܂Ŋg�債���ꍇ
		if (m_fScale > ScaleSize)
		{
			// �������ő�g��l�Ɏ��߁A�������k��������
			m_fScale = ScaleSize;
			m_bScaleChange = true;
		}
	}
	// �������k�����Ă���ꍇ
	else
	{
		// �������k��������
		m_fScale -= ScaleSpeed;

		// ���������̒l�܂ŏk�������ꍇ
		if (m_fScale < -ScaleSize)
		{
			// �������ŏ��g��l�Ɏ��߁A�������g�傳����
			m_fScale = -ScaleSize;
			m_bScaleChange = false;
		}
	}

	// �����̑傫���̐ݒ菈��
	for (auto pFont : m_apFont) pFont->SetSize(m_sizeYuan.x + m_fScale, m_sizeYuan.y + m_fScale);
}

//=====================================
// �t�H���g�S�̂̓_�ŏ���
//=====================================
void CFontString::BlinkColor()
{
	for (auto pFont : m_apFont) pFont->SetAlpha(m_fBlinkAlpha);

	// �_�̑�������
	m_fBlinkAlpha += m_bBlinkScale ? m_fBlinkSpeed : -m_fBlinkSpeed;

	if (m_fBlinkAlpha >= 1.0f)
	{
		// �A���t�@�l�̌���
		m_bBlinkScale = false;
		m_fBlinkAlpha = 1.0f;
	}
	if (m_fBlinkAlpha <= Mimi_Blink)
	{
		// �A���t�@�l�̑���
		m_bBlinkScale = true;
		m_fBlinkAlpha = Mimi_Blink;
	}
}

//=====================================
// �t�H���g�ʒu�̐ݒ菈��
//=====================================
void CFontString::SetFont()
{
	// �����̌��̑傫���̐ݒ�
	m_sizeYuan = m_size;

	// �����̈ʒu�Ƒ傫����ݒ�
	for (int nCnt = 0; nCnt < (int)m_apFont.size(); nCnt++)
	{
		m_apFont[nCnt]->Setposition(m_pos + D3DXVECTOR3((m_size.x + m_size.x * 0.5f) * nCnt, 0.0f, 0.0f));
		m_apFont[nCnt]->SetSize(m_sizeYuan.x + m_fScale, m_sizeYuan.y + m_fScale);
	}
}

//=====================================
// �_�ł̏I������
//=====================================
void CFontString::BlinkFinish()
{
	// ����������
	Init();

	// �����̃A���t�@�l�̐ݒ菈��
	for (auto pFont : m_apFont) pFont->SetAlpha(m_fBlinkAlpha);
}

//=====================================
// �_�ł̏I������
//=====================================
void CFontString::SetScale(const bool scale)
{
	m_bScale = scale;	// �����̊g�又���̐ݒ�

	// �g�又�����I������ꍇ
	if (m_bScale == false)
	{
		// �����̑傫���̏�����
		m_bScaleChange = false;
		m_fScale = 0.0f;

		// �����̑傫���̐ݒ菈��
		for (auto pFont : m_apFont) pFont->SetSize(m_sizeYuan.x + m_fScale, m_sizeYuan.y + m_fScale);
	}
}

//=====================================
// ������̐ݒ菈��
//=====================================
void CFontString::SetString(const std::string letter, const int priority)
{
	// string�^�̕��������擾
	int nTex = strlen(letter.c_str());

	// �����̃������̑傫��(bit)
	int nStrSize = 0;
	int nStrSizeOld = 0;

	std::string sLatter;

	float fStrLenPos = 0.0f;	// �����̉��̈ʒu
	D3DXCOLOR col = { 1.0f, 1.0f, 1.0f, 1.0f };
	std::string stdMath;

	int nCounter = 0;	// ����ǂݍ��܂ꂽ��

	for (int nLatter = 0; nLatter < nTex; nLatter += nStrSize)
	{
		// ����R�}���h���ǂ܂ꂽ�ꍇ(�g�p�� : <c=220,20,60,255,>�e�L�X�g</>)
		if (letter[nLatter] == '<')
		{
			nLatter++;

			// ����R�}���h���I������܂œǂݍ���
			while (letter[nLatter] != '>')
			{
				switch (letter[nLatter])
				{
					// �F�ύX�R�}���h
				case 'c':
					nLatter += 2;

					for (int nCntColor = 0; nCntColor < 4; nCntColor++)
					{
						while (letter[nLatter] != ',')
						{
							stdMath.push_back(letter[nLatter]);
							nLatter++;
						}
						nLatter++;

						switch (nCntColor)
						{
						case 0:
							col.r = std::stof(stdMath);
							break;
						case 1:
							col.g = std::stof(stdMath);
							break;
						case 2:
							col.b = std::stof(stdMath);
							break;
						case 3:
							col.a = std::stof(stdMath);
							break;
						default:
							break;
						}
						stdMath.clear();
					}
					col = D3DCOLOR_RGBA((int)col.r, (int)col.g, (int)col.b, (int)col.a);

					break;
					// �I���R�}���h�R�}���h
				case '/':
					col = { 1.0f, 1.0f, 1.0f, 1.0f };
					nLatter++;
					break;
				default:
					break;
				}
			}
			nStrSize = 1;
		}
		// ������ǂݍ��ޏ���
		else
		{
			// ���{��̏ꍇ
			if (letter[nLatter] <= -125 && letter[nLatter] >= -127)
			{
				nStrSize = 2;
			}
			// �A���t�@�x�b�g�̏ꍇ
			else
			{
				nStrSize = 1;
			}

			// ���݈ʒu����1�����i�ޏ���
			sLatter = letter.substr(nLatter, nStrSize);

			// �O��ƌ��݂̕����̑傫������ʒu�𒲐�����
			fStrLenPos += m_size.x * (nStrSize < 2 ? 0.5f : 0.75f);

			// �O�񕶎������݂���ꍇ�ꍇ
			if (nStrSizeOld != 0)
			{
				fStrLenPos += m_size.x * (nStrSizeOld < 2 ? 0.5f : 0.75f);
			}

			// �������g�p����Ă��Ȃ��ꍇ
			if ((int)m_apFont.size() <= nCounter)
			{
				// �����̐���
				m_apFont.push_back(CFont::Create(
					m_pos + D3DXVECTOR3(fStrLenPos, 0.0f, 0.0f),
					m_size,
					sLatter.c_str(),
					priority,
					col
				));
			}
			else
			{
				// �����̐ݒ�
				m_apFont[nCounter]->SetString(sLatter.c_str(), col);
				m_apFont[nCounter]->Setposition(m_pos + D3DXVECTOR3(fStrLenPos, 0.0f, 0.0f));
			}

			// �ŏ��̃T�C�Y��o�^����
			m_sizeYuan = m_size;

			// ���������J�E���g
			nCounter++;
		}
		// �O�񕶎��̑傫�����L�^������
		nStrSizeOld = nStrSize;
	}

	// �g�p�ς݂̗̈悪���݂���ꍇ�폜
	for (int nCnt = nCounter; nCounter < (int)m_apFont.size(); nCnt++)
	{
		m_apFont.back()->Uninit();
		m_apFont.pop_back();
	}
}

//=====================================
// ��������
//=====================================
CFontString* CFontString::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const std::string letter, const int priority)
{
	CFontString* pFontString = new CFontString;
	
	if (FAILED(pFontString->Init()))
	{
		return nullptr;
	}

	pFontString->m_pos = pos;
	pFontString->m_anchor = pos;
	pFontString->m_size = size;

	pFontString->SetString(letter, priority);

	return pFontString;
}