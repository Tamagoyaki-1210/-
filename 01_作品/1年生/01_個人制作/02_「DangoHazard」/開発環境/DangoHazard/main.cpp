//============================================================================
//
//�|���S���`��̏���[main.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include <stdio.h>		//sprintf�ɕK�v
#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����

#include "fade.h"		//�쐬����fade.h���C���N���[�h����
#include "title.h"		//�쐬����title.h���C���N���[�h����
#include "game.h"		//�쐬����game.h���C���N���[�h����
#include "result.h"		//�쐬����result.h���C���N���[�h����
#include "gameover.h"	//�쐬����gameover.h���C���N���[�h����

#include "camera.h"		//�쐬����camera.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "enemy.h"		//�쐬����enemy.h���C���N���[�h����
#include "item.h"		//�쐬����item.h���C���N���[�h����
#include "attack.h"		//�쐬����attack.h���C���N���[�h����

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
LPD3DXFONT			pFont;						// �t�H���g�I�u�W�F�N�g

MODE g_mode = MODE_TITLE;						//���݂̃��[�h
bool g_MeshDebug = false;						//���b�V���f�o�b�O
bool g_TextDebug = true;						//�e�L�X�g�f�o�b�O

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

	//�t�H���g
	D3DXCreateFont(
		g_pD3DDevice,				// Direct3D�f�o�C�X
		TEXT_HEIGHT,				// ����
		TEXT_WIDTH,					// ��
		FW_REGULAR,					// �t�H���g�̑��� ����
		NULL,						// ����
		FALSE,						// �Α�
		SHIFTJIS_CHARSET,			// �����Z�b�g
		OUT_DEFAULT_PRECIS,			// �o�͐��x�͕���
		PROOF_QUALITY,				// �����i�����d��
		FIXED_PITCH | FF_SCRIPT,	// �s�b�`�ƃt�@�~��
		TEXT("�l�r�o�S�V�b�N"),	// �t�H���g��
		&pFont);					// ID3DXFont�|�C���^

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
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//���[�h�̐ݒ�
	SetMode(g_mode);

	//�t�F�[�h�̏������ݒ�
	InitFade(g_mode);	

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

	if (GetKeyboardTrigger(DIK_F2) == true)
	{
		g_TextDebug = !g_TextDebug;
		//if (g_TextDebug == false)
		//{
		//	//���C���[�t���[�����[�h�̐ݒ�(�ǉ�����)
		//	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//}
		//else
		//{
		//	//���ɖ߂�(���C���[�t���[�����[�h)
		//	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		//}
		//g_MeshDebug = !g_MeshDebug;
		//if (g_MeshDebug == false)
		//{
		//	//���C���[�t���[�����[�h�̐ݒ�(�ǉ�����)
		//	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//}
		//else
		//{
		//	//���ɖ߂�(���C���[�t���[�����[�h)
		//	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		//}
	}
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

	char aStr[MAX_TEXT][256];	//�e�L�X�g�̕�����
	PLAYER *pPlayer = GetPlayer();
	ENEMY *pEnemy = GetEnemy();
	CAMERA *pCamera = GetCamera();

		//�e�L�X�g�W
		sprintf(aStr[0], "<�������>\n");
		sprintf(aStr[1], "�v���C���[�ړ� :	[W]\n");
		sprintf(aStr[2], "------------[A] [S] [D]\n");
		sprintf(aStr[3], "�W�����v\n");
		sprintf(aStr[4], "[SPACE]\n");
		sprintf(aStr[5], "------------\n");
		sprintf(aStr[6], "�U��\n");
		sprintf(aStr[7], "[ENTER]\n");
		sprintf(aStr[8], "------------\n");
		sprintf(aStr[9], "�J�����ړ� :	[��]\n");
		sprintf(aStr[10], "-------[��] [��] [��]\n");
		sprintf(aStr[11], "���� :	[U] [I] [O]\n");
		sprintf(aStr[12], "------[J] [K] [L]\n");
		sprintf(aStr[13], "��������̕��� : [F2]\n");
	//sprintf(aStr[0], "<�e�L�X�g>\n");
	//sprintf(aStr[1], "[F3] : �J�����Ǐ]\n");
	////sprintf(aStr[2], "�v���C���[_x���W:%.2f\n", pPlayer->pos.x);
	////sprintf(aStr[3], "�v���C���[_y���W:%.2f\n", pPlayer->pos.y);
	////sprintf(aStr[4], "�v���C���[_z���W:%.2f\n", pPlayer->pos.z);
	////sprintf(aStr[2], "�v���C���[_x�ړ���:%.2f\n", pPlayer->move.x);
	////sprintf(aStr[3], "�v���C���[_y�ړ���:%.2f\n", pPlayer->move.y);
	////sprintf(aStr[4], "�v���C���[_z�ړ���:%.2f\n", pPlayer->move.z);
	////sprintf(aStr[2], "�G�L����1_x���W:%.2f\n", pEnemy->pos.x);
	////sprintf(aStr[3], "�G�L����1_y���W:%.2f\n", pEnemy->pos.y);
	////sprintf(aStr[4], "�G�L����1_z���W:%.2f\n", pEnemy->pos.z);
	//sprintf(aStr[2], "�G�L����1_x�ړ���:%.2f\n", pEnemy->move.x);
	//sprintf(aStr[3], "�G�L����1_y�ړ���:%.2f\n", pEnemy->move.y);
	//sprintf(aStr[4], "�G�L����1_z�ړ���:%.2f\n", pEnemy->move.z);
	//sprintf(aStr[5], "���_�̍��W:(%.2f,%.2f,%.2f)\n", pCamera->posV.x, pCamera->posV.y, pCamera->posV.z);
	//sprintf(aStr[6], "�����_�̍��W:(%.2f,%.2f,%.2f)\n", pCamera->posR.x, pCamera->posR.y, pCamera->posR.z);
	//sprintf(aStr[7], "�J�����̊p�x:(%.2f,%.2f,%.2f)\n", pCamera->rot.x, pCamera->rot.y, pCamera->rot.z);
	//sprintf(aStr[8], "�v���C���[�ڒn����[%d]\n", pPlayer->bGround);
	//sprintf(aStr[9], "�v���C���[���[�V����[%d]\n", pPlayer->motion);
	//sprintf(aStr[10], "�v���C���[�o�߃t���[����[%.0f]\n", pPlayer->fFlame);
	//sprintf(aStr[11], "�v���C���[���[�v����[%d]\n", pPlayer->bLoop);
	//sprintf(aStr[12], "�p�[�c1_x�p�x:%.2f\n", pPlayer->aParts->rot.x);
	//sprintf(aStr[13], "�v���C���[���C�t:[%d]\n", pPlayer->nLife);

	for (int nCntText = 0; nCntText < MAX_TEXT; nCntText++)
	{
		//�e�L�X�g�ʒu
		RECT rc = {
			0,		// �����x���W
			100 + (TEXT_HEIGHT * nCntText),		// �����y���W
			SCREEN_WIDTH,	// �E����x���W
			SCREEN_HEIGHT	// �E����y���W
		};

		if (g_TextDebug == true)
		{
			// �e�L�X�g�`��
			pFont->DrawText(
				NULL,					// NULL
				&aStr[nCntText][0],		// �`��e�L�X�g
				-1,						// �S�ĕ\��
				&rc,						// �\���͈�
				DT_LEFT,					// ����
				D3DCOLOR_XRGB(0, 255, 0)	//�ΐF
				//D3DCOLOR_XRGB(255, 255, 255)	// ���F
			);
		}
		else
		{
			// �e�L�X�g�`��
			pFont->DrawText(
				NULL,					// NULL
				&aStr[nCntText][0],		// �`��e�L�X�g
				0,						// �S�Ĕ�\��
				&rc,						// �\���͈�
				DT_LEFT,					// ����
				D3DCOLOR_XRGB(0, 255, 0)	//�ΐF
			);
		}
	}

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