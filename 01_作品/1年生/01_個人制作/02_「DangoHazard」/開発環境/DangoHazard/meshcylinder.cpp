//============================================================================
//
//���b�V���|���S���̏���[meshfield.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "meshcylinder.h"	//�쐬����meshcylinder.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureMeshCylinder = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
MESHCYLINDER g_aMeshCylinder;							//���b�V���̏��
int g_nMeshCyPlimitive = 1000;
int g_nMeshCyVertex = 1000;
int g_nMeshCyIndex;

//============================================================================
//���b�V���~���̏���������
//============================================================================
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	g_nMeshCyIndex = (((MAX_MESHCYLINDER + 1) * 2) * MAX_MESHCYLINDER) + ((MAX_MESHCYLINDER - 1) * 2);

	pDevice->CreateVertexBuffer(				//���_�o�b�t�@�̐���
		sizeof(VERTEX_3D) * g_nMeshCyIndex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);

	D3DXCreateTextureFromFile(					//�e�N�X�`���[�̓ǂݍ���
		pDevice,
		"data\\TEXTURE\\Skarmory.jpg",			//�e�N�X�`���̃t�@�C����
		&g_pTextureMeshCylinder
	);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nMeshCyIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);

	//�ʒu�̏���������
	g_aMeshCylinder.nMeshX = 3;
	g_aMeshCylinder.nMeshZ = 3;
	g_aMeshCylinder.pos = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
	g_aMeshCylinder.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshCylinder.nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����������������
	g_aMeshCylinder.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aMeshCylinder.fAngle = atan2f(MESHCYLINDER_WIDTH, MESHCYLINDER_HEIGHT);
	g_aMeshCylinder.fLength = sqrtf(MESHCYLINDER_WIDTH * MESHCYLINDER_WIDTH + MESHCYLINDER_HEIGHT * MESHCYLINDER_HEIGHT) / 2.0f;
	g_aMeshCylinder.Worldmtx = D3DXMATRIX();

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	//�e���_�̖@���̐ݒ�(���x�N�g���̑傫����1�ɂ���K�v������)
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshCylinder->Unlock();
}

//============================================================================
//���b�V���|���S���̏I������
//============================================================================
void UninitMeshCylinder(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;

	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̉��
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//============================================================================
//���b�V���|���S���̍X�V����
//============================================================================
void UpdateMeshCylinder(void)
{
	int nCntMeshCyX;
	int nCntMeshCyZ;
	int nDeg = 0;		//�k�ރ|���S��

	if (GetKeyboardTrigger(DIK_O) == true)
	{
		if (g_aMeshCylinder.nMeshX < MAX_MESHCYLINDER)
		{
			g_aMeshCylinder.nMeshX++;
			g_aMeshCylinder.nMeshZ++;
		}
	}
	else if (GetKeyboardTrigger(DIK_P) == true)
	{
		if (g_aMeshCylinder.nMeshX > 1)
		{
			g_aMeshCylinder.nMeshX--;
			g_aMeshCylinder.nMeshZ--;
		}
	}

	//�|���S�����v�Z�� = ((�|���S����.x * �|���S����.z) * 2) + ((�|���S����.z - 1) * 4)
	g_nMeshCyPlimitive = ((g_aMeshCylinder.nMeshX * g_aMeshCylinder.nMeshZ) * 2) + ((g_aMeshCylinder.nMeshZ - 1) * 4);

	//�C���f�b�N�X���v�Z�� = (((�|���S����.x + 1) * 2) * �|���S����.z) + ((�|���S����.y - 1) * 2)
	g_nMeshCyIndex = (((g_aMeshCylinder.nMeshX + 1) * 2) * g_aMeshCylinder.nMeshZ) + ((g_aMeshCylinder.nMeshZ - 1) * 2);

	//���_���v�Z�� = (�|���S����.x + 1) * (�|���S����.z + 1)
	g_nMeshCyVertex = (g_aMeshCylinder.nMeshX + 1) * (g_aMeshCylinder.nMeshZ + 1);

	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	//�w�肳�ꂽ�|���S�������A���_��z�u���鏈��
	for (nCntMeshCyZ = 0; nCntMeshCyZ < g_aMeshCylinder.nMeshZ + 1; nCntMeshCyZ++)
	{
		for (nCntMeshCyX = 0; nCntMeshCyX < g_aMeshCylinder.nMeshX + 1; nCntMeshCyX++)
		{
			pVtx[nCntMeshCyX + (nCntMeshCyZ * (g_aMeshCylinder.nMeshX + 1))].pos =
				D3DXVECTOR3((nCntMeshCyX * MESHCYLINDER_WIDTH) - (g_aMeshCylinder.nMeshX * MESHCYLINDER_WIDTH / 2),	//(���J�E���g����) - (�|���S��������)
					-(nCntMeshCyZ * MESHCYLINDER_HEIGHT) + (g_aMeshCylinder.nMeshZ * MESHCYLINDER_HEIGHT / 2),				//(���J�E���g����) + (�|���S��������)
					0.0f
				);

			//�e���_�̖@���̐ݒ�(���x�N�g���̑傫����1�ɂ���K�v������)
			pVtx[nCntMeshCyX + (nCntMeshCyZ * (g_aMeshCylinder.nMeshX + 1))].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[nCntMeshCyX + (nCntMeshCyZ * (g_aMeshCylinder.nMeshX + 1))].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
			pVtx[nCntMeshCyX + (nCntMeshCyZ * (g_aMeshCylinder.nMeshX + 1))].tex = D3DXVECTOR2(nCntMeshCyX, nCntMeshCyZ);
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshCylinder->Unlock();

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�̐ݒ�
	for (nCntMeshCyZ = 0; nCntMeshCyZ < g_aMeshCylinder.nMeshZ; nCntMeshCyZ++)
	{//z���J�E���g
		if (nCntMeshCyZ != 0)
		{//�k�ރ|���S�����K�v�ȏꍇ
			pIdx[(((g_aMeshCylinder.nMeshX + 1) * 2) * nCntMeshCyZ) + nDeg] = ((g_aMeshCylinder.nMeshX + 1) * nCntMeshCyZ) - 1;		//pIdx[���Ō�̃C���f�b�N�X] = x,z���v�Z
			pIdx[(((g_aMeshCylinder.nMeshX + 1) * 2) * nCntMeshCyZ) + nDeg + 1] = (g_aMeshCylinder.nMeshX + 1) * (nCntMeshCyZ + 1);	//pIdx[���Ō�̃C���f�b�N�X + 1] = x,z���v�Z
			nDeg += 2;
		}
		//x���J�E���g
		for (nCntMeshCyX = 0; nCntMeshCyX < (g_aMeshCylinder.nMeshX + 1); nCntMeshCyX++)
		{
			//pIdx[(���J�E���gx�� * 2) + ((�����镪��x�� * 2) * ���J�E���gz��) + �k�ރ|���S��]
			pIdx[(nCntMeshCyX * 2) + (((g_aMeshCylinder.nMeshX + 1) * 2) * nCntMeshCyZ) + nDeg]
				// = (�����镪��x��) + ���J�E���gx�� + ((�����镪��x��) * ���J�E���gz��)
				= (g_aMeshCylinder.nMeshX + 1) + nCntMeshCyX + ((g_aMeshCylinder.nMeshX + 1) * nCntMeshCyZ);

			//pIdx[���+1]
			pIdx[(nCntMeshCyX * 2) + (((g_aMeshCylinder.nMeshX + 1) * 2) * nCntMeshCyZ) + nDeg + 1]
				// = ���J�E���gx�� + ((�����镪��x��) * ���J�E���gz��)
				= nCntMeshCyX + ((g_aMeshCylinder.nMeshX + 1) * nCntMeshCyZ);
		}
	}
	//�C���f�b�N�X�̃A�����b�N
	g_pIdxBuffMeshCylinder->Unlock();
}

//============================================================================
//���b�V���|���S���̕`�揈��
//============================================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X

											//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aMeshCylinder.Worldmtx);
	//�����𔽉f(YaW : y,Pitch : x,Roll : z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshCylinder.rot.y, g_aMeshCylinder.rot.x, g_aMeshCylinder.rot.z);
	D3DXMatrixMultiply(&g_aMeshCylinder.Worldmtx, &g_aMeshCylinder.Worldmtx, &mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_aMeshCylinder.pos.x, g_aMeshCylinder.pos.y, g_aMeshCylinder.pos.z);
	D3DXMatrixMultiply(&g_aMeshCylinder.Worldmtx, &g_aMeshCylinder.Worldmtx, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshCylinder.Worldmtx);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nMeshCyVertex, 0, g_nMeshCyPlimitive);

	//�e�N�X�`���̏I��
	pDevice->SetTexture(0, NULL);
}
