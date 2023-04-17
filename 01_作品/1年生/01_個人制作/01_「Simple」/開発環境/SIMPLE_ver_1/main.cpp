//============================================================================
//
//�|���S���`��̏���[main.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "background.h"	//�쐬����background.h���C���N���[�h����
#include "block.h"		//�쐬����block.h���C���N���[�h����
#include "item.h"		//�쐬����item.h���C���N���[�h����
#include "score.h"		//�쐬����score.h���C���N���[�h����
#include "fade.h"		//�쐬����fade.h���C���N���[�h����
#include "title.h"		//�쐬����title.h���C���N���[�h����
#include "game.h"		//�쐬����game.h���C���N���[�h����
#include "result.h"		//�쐬����result.h���C���N���[�h����
#include "gameover.h"	//�쐬����gameover.h���C���N���[�h����
#include "sound.h"		//�쐬����sound.h���C���N���[�h����

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM IParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;				//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;		//Direct3D�f�o�C�X�ւ̃|�C���^

MODE g_mode = MODE_TITLE;						//���݂̃��[�h

//============================================================================
//���C���֐� (�֐��w�b�_�R�����g)
//============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine,int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,							//�E�C���h�E�̃X�^�C��
		WindowProc,							//�E�C���h�E�v���V�[�W��
		0,									//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,									//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hInstance,							//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),		//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),			//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),			//�N���C�A���g�̈�̔w�i�F
		NULL,								//���j���[�o�[
		CLASS_NAME,							//�E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)		//�t�@�C���̃A�C�R��
	};
	HWND hWnd;	//�E�C���h�E�n���h��(���ʎq)
	MSG msg;	//���b�Z�[�W���i�[����ϐ�

	//��ʃT�C�Y�̍\����
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//�E�C���h�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E���쐬
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,				//�E�C���h�E�̍���X���W
		CW_USEDEFAULT,				//�E�C���h�E�̍���Y���W
		(rect.right - rect.left),	//�E�C���h�E�̕�
		(rect.bottom - rect.top),	//�E�C���h�E�̍���
		NULL,
		NULL,
		hInstance,
		NULL);

	DWORD dwCurrentTime;	//���ݎ���
	DWORD dwExecLastTime;	//�Ō�ɏ�����������

	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//���������������s�����ꍇ
		return -1;
	}

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);		//�E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);				//�N���C�A���g�̈���X�V

	//����\�͂�ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;				//����������
	dwExecLastTime = timeGetTime();	//���ݎ������擾(�ۑ�)

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W�𔲂���
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();		//���ݎ������擾
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExecLastTime = dwCurrentTime;	//�����J�n�̎���[���ݎ���]���擾
				Update();	//�X�V����
				Draw();		//�`�揈��
			}
		}
	}
	//�I������
	Uninit();
	//����\�͂�߂�
	timeEndPeriod(1);

	//�E�C���h�E�N���X�̓o�^����
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}
//============================================================================
//�E�C���h�E�v���V�[�W���̏���
//============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM IParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:	//�E�C���h�E�j���̃��b�Z�[�W
						//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:	//[ESC]�L�[�������ꂽ���̏���
						//��1������hWnd�Ɏw�肵���ꍇ�A�E�C���h�E���e(�I�[�i�[)�ɂȂ�
			nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{
				//�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:	//����{�^�������̃��b�Z�[�W

		nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO);
		if (nID == IDYES)
		{
			//�E�C���h�E��j������
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;	//0��Ԃ��Ȃ��ƏI�����Ă��܂�
		}
		break;
	case WM_LBUTTONDOWN:	//�}�E�X���N���b�N�̃��b�Z�[�W

		//�E�C���h�E�Ƀt�H�[�J�X�����킹��
		SetFocus(hWnd);		//�N���b�N�����E�C���h�E���A�N�e�B�u�ɂȂ鏈��
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, IParam);
}
//============================================================================
//�������̏���
//============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^

	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;		//�Q�[����ʃT�C�Y�̉���
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		//�Q�[����ʃT�C�Y�̍���
	d3dpp.BackBufferFormat = d3ddm.Format;		//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;					//�o�b�N�o�b�t�@�̌�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//�_�u���o�b�t�@�̐؂�ւ�
	d3dpp.EnableAutoDepthStencil = TRUE;		//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;					//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//�C���^�[�o��

	//Direct3D�f�o�C�X�̐���(�`�ʏ����ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�)
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		//Direct3D�f�o�C�X�̐���(�`�ʏ����̓n�[�h�E�F�A�A���_������CPU�ōs�Ȃ�)
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{//Direct3D�f�o�C�X�̐���(�`�ʏ����ƒ��_������CPU�ōs�Ȃ�)
			if (FAILED(g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//�J�����O�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//���[�h�̐ݒ�
	SetMode(g_mode);

	//InitSound(hWnd);	//�T�E���h�̏������ݒ�
	InitFade(g_mode);	//�t�F�[�h�̏������ݒ�

	return S_OK;
}
//============================================================================
//�I������
//============================================================================
void Uninit(void)
{
	UninitTitle();		//�^�C�g����ʂ̏I������
	UninitGame();		//�Q�[����ʂ̏I������
	UninitResult();		//���U���g�̏I������
	UninitFade();		//�t�F�[�h�̏I������
	UninitGameOver();	//�Q�[���I�[�o�[�̏I������
	//UninitSound();		//�T�E���h�̏I������

	UninitKeyboard();	//�L�[�{�[�h�̏I������
	if (g_pD3DDevice != NULL)
	{//Direct3D�f�o�C�X�̔j��
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	if (g_pD3D != NULL)
	{//Direct3D�I�u�W�F�N�g�̔j��
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}
//============================================================================
//�X�V����
//============================================================================
void Update(void)
{
	FADE fade;
	fade = GetFade();

	UpdateKeyboard();	//�L�[�{�[�h�̍X�V����

	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();	//�^�C�g�����
		break;
	case MODE_GAME:
		UpdateGame();	//�Q�[�����
		break;
	case MODE_RESULT:
		UpdateResult();	//���U���g���
		break;
	case MODE_GAMEOVER:
		UpdateGameOver();	//�Q�[���I�[�o�[���
		break;
	}

	UpdateFade();	//�t�F�[�h�̍X�V����
}
//============================================================================
//�`�揈��
//============================================================================
void Draw(void)
{
	//��ʃN���A(�o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(
		0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`��J�n���J�n�����������ꍇ
		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();	//�^�C�g����ʂ̕`�揈��
			break;
		case MODE_GAME:
			DrawGame();		//�Q�[����ʂ��`�揈��
			break;
		case MODE_RESULT:
			DrawResult();	//���U���g��ʂ̕`�揈��
			break;
		case MODE_GAMEOVER:
			DrawGameOver();	//�Q�[���I�[�o�[��ʂ̕`�揈��
			break;
		}

		DrawFade();	//�t�F�[�h�̕`�揈��

		//�`��I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//============================================================================
//���[�h�ݒ菈��
//============================================================================
void SetMode(MODE mode)
{
	//���݉��(���[�h)�̏I������
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();	//�^�C�g����ʂ̏I������
		break;
	case MODE_GAME:
		UninitGame();	//�Q�[����ʂ̏I������
		break;
	case MODE_RESULT:
		UninitResult();	//���U���g��ʂ̏I������
		break;
	case MODE_GAMEOVER:
		UninitGameOver();	//�Q�[���I�[�o�[��ʂ̏I������
		break;
	}

	g_mode = mode;		//���݂̉��(���[�h)��؂�ւ���
						//�V���(���[�h)�̏���������
	switch (mode)
	{
	case MODE_TITLE:	
		InitTitle();	//�^�C�g����ʂ̏���������
		break;

	case MODE_GAME:		
		InitGame();		//�Q�[����ʂ̏���������
		break;

	case MODE_RESULT:	
		InitResult();	//���U���g��ʂ̏���������
		break;
	case MODE_GAMEOVER:
		InitGameOver();	//�Q�[���I�[�o�[��ʂ̏���������
		break;
	}
}
//============================================================================
//�f�o�C�X�̎擾
//============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//============================================================================
//���[�h�擾����
//============================================================================
MODE GetMode(void)
{
	return g_mode;
}